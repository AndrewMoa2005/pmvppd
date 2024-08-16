#include "widget.h"
#include "engine_pmv_ppd.h"
#include <stdio.h>

#define BUFFER_SIZE 2048

typedef struct {
	GtkWidget* label_CLO;
	GtkWidget* en_CLO;
	GtkWidget* label_MET;
	GtkWidget* en_MET;
	GtkWidget* label_WME;
	GtkWidget* en_WME;
	GtkWidget* label_TA;
	GtkWidget* en_TA;
	GtkWidget* label_VEL;
	GtkWidget* en_VEL;
	GtkWidget* label_TR;
	GtkWidget* en_TR;
	GtkWidget* label_RH;
	GtkWidget* en_RH;
	GtkWidget* label_PMV;
	GtkWidget* en_PMV;
	GtkWidget* label_PPD;
	GtkWidget* en_PPD;
	GtkWidget* label_SET;
	GtkWidget* en_SET;
} MWidget;

static void
on_bn_calc(GtkWidget* widget,
	MWidget* m)
{
	double clo = gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_CLO));
	double met = gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_MET));
	double wme = gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_WME));
	double ta = gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_TA));
	double vel = gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_VEL));
	double tr = gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_TR));
	double rh = gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_RH));
	double pmv, ppd, set;
	if (PMVboth(clo, met, wme, ta, tr, vel, rh, 0, &pmv, &ppd, &set) < 1)
		return;
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(m->en_PMV), pmv);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(m->en_PPD), ppd);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(m->en_SET), set);
}

static gboolean
str_find_match(const gchar* str,
	const gchar* c,
	gboolean case_sensitive)
{
	gboolean b;
	GString* sstr = g_string_new(str);
	GString* sc = g_string_new(c);
	if (case_sensitive) {
		sstr = g_string_ascii_up(sstr);
		sc = g_string_ascii_up(sc);
	}
	gchar* e = strchr(sstr->str, *sc->str);
	if (!e) {
		b = FALSE;
		g_free(sstr);
		g_free(sc);
		return b;
	}
	for (gsize i = 0; i < sc->len; i++) {
		if (e[i] == sc->str[i])
			b = TRUE;
		else {
			b = FALSE;
			break;
		}
	}

	g_free(sstr);
	g_free(sc);
	return b;
}

static gboolean
str_suffix_match(const gchar* str,
	const gchar* c,
	gboolean case_sensitive)
{
	GString* tail = g_string_new(str);
	GString* sc = g_string_new(c);
	tail = g_string_erase(tail, 0, tail->len - sc->len);
	gboolean b;
	if (!case_sensitive) {
		tail = g_string_ascii_up(tail);
		sc = g_string_ascii_up(sc);
		b = g_string_equal(tail, sc);
	}
	else {
		for (gsize i = 0; i <= sc->len; i++) {
			b = TRUE;
			if (sc->str[i] == tail->str[i]) {
				continue;
			}
			else {
				b = FALSE;
				break;
			}
		}
	}
	g_free(tail);
	g_free(sc);
	return b;
}

static void
on_bn_gen_response(GtkWidget* dialog,
	GAsyncResult* res,
	MWidget* m)
{
	GFile* file = gtk_file_dialog_save_finish(GTK_FILE_DIALOG(dialog), res, NULL);
	if (!file)
		return;
	GString* filepath = g_string_new(g_file_get_path(file));
	if (!g_file_query_exists(file, NULL))
		// GTK+4 file dialog has no filter, so we use this method to add ".csv" suffix ...
		if (!str_suffix_match(filepath->str, ".csv", FALSE))
			filepath = g_string_append(filepath, ".csv");
	g_object_unref(file);

	FILE* out_file = fopen(filepath->str, "w");
	if (!out_file)
		return;

	GString* head = g_string_new(NULL);
	g_string_printf(head, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
		gtk_label_get_text(GTK_LABEL(m->label_CLO)), gtk_label_get_text(GTK_LABEL(m->label_MET)),
		gtk_label_get_text(GTK_LABEL(m->label_WME)), gtk_label_get_text(GTK_LABEL(m->label_TA)),
		gtk_label_get_text(GTK_LABEL(m->label_VEL)), gtk_label_get_text(GTK_LABEL(m->label_TR)),
		gtk_label_get_text(GTK_LABEL(m->label_RH)));
	fputs(head->str, out_file);
	GString* data = g_string_new(NULL);
	g_string_printf(data, "%.2f,%.2f,%.2f,%.2f,%.4f,%.2f,%.2f\n",
		gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_CLO)),
		gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_MET)),
		gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_WME)),
		gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_TA)),
		gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_VEL)),
		gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_TR)),
		gtk_spin_button_get_value(GTK_SPIN_BUTTON(m->en_RH)));
	fputs(data->str, out_file);
	fclose(out_file);

	GtkAlertDialog* msg = gtk_alert_dialog_new("Template file has been saved : %s\n", filepath->str);
	gtk_alert_dialog_set_modal(msg, TRUE);
	gtk_alert_dialog_show(msg, GTK_WINDOW(gtk_widget_get_root(m->label_CLO)));

	g_free(head);
	g_free(data);
	g_free(filepath);
}

static void
on_bn_gen(GtkWidget* widget,
	MWidget* m)
{
	GtkFileDialog* dialog = gtk_file_dialog_new();
	gtk_file_dialog_set_title(dialog, "Save template file");
	gtk_file_dialog_set_modal(dialog, TRUE);
	GtkFileFilter* filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "CSV Files");
	gtk_file_filter_add_pattern(filter, "*.csv");
	//gtk_file_filter_add_suffix(filter, "csv");
	gtk_file_dialog_set_default_filter(dialog, filter);
	gtk_file_dialog_save(dialog, GTK_WINDOW(gtk_widget_get_root(widget)),
		NULL, (GAsyncReadyCallback)(on_bn_gen_response), m);
}

static gboolean
str_move_file(const gchar* src,
	const gchar* dst)
{
	GFile* srcFile = g_file_new_for_path(src);
	GFile* dstFile = g_file_new_for_path(dst);
	gboolean b = g_file_move(srcFile, dstFile, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, NULL);
	g_object_unref(srcFile);
	g_object_unref(dstFile);
	return b;
}

static void
on_bn_handle_response(GtkWidget* dialog,
	GAsyncResult* res,
	MWidget* m)
{
	GFile* file = gtk_file_dialog_save_finish(GTK_FILE_DIALOG(dialog), res, NULL);
	if (!file)
		return;
	if (!g_file_query_exists(file, NULL))
		return;
	GString* filepath = g_string_new(g_file_get_path(file));
	GString* bakpath = g_string_new(filepath->str);
	bakpath = g_string_append(bakpath, ".bak");
	g_object_unref(file);
	if (!str_move_file(filepath->str, bakpath->str)) {
		GtkAlertDialog* msg1 = gtk_alert_dialog_new("Cannot backup file : %s", filepath->str);
		gtk_alert_dialog_set_modal(msg1, TRUE);
		gtk_alert_dialog_show(msg1, GTK_WINDOW(gtk_widget_get_root(m->label_CLO)));
		return;
	}

	FILE* bak_file = fopen(bakpath->str, "r");
	FILE* out_file = fopen(filepath->str, "w");
	if (!bak_file || !out_file) {
		return;
	}
	gchar buffer[BUFFER_SIZE];

	fgets(buffer, BUFFER_SIZE, bak_file);
	GStrv head_var = g_strsplit(buffer, ",", -1);
	GString* head = g_string_new(NULL);
	g_string_printf(head, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
		gtk_label_get_text(GTK_LABEL(m->label_CLO)), gtk_label_get_text(GTK_LABEL(m->label_MET)),
		gtk_label_get_text(GTK_LABEL(m->label_WME)), gtk_label_get_text(GTK_LABEL(m->label_TA)),
		gtk_label_get_text(GTK_LABEL(m->label_VEL)), gtk_label_get_text(GTK_LABEL(m->label_TR)),
		gtk_label_get_text(GTK_LABEL(m->label_RH)), gtk_label_get_text(GTK_LABEL(m->label_PMV)),
		gtk_label_get_text(GTK_LABEL(m->label_PPD)), gtk_label_get_text(GTK_LABEL(m->label_SET)));
	fputs(head->str, out_file);

	while (!feof(bak_file)) {
		if (!fgets(buffer, BUFFER_SIZE, bak_file))
			break;
		GStrv data;
		data = g_strsplit(buffer, ",", -1);
		if (g_strv_length(data) != g_strv_length(head_var))
			continue;
		double clo, met, wme, ta, vel, tr, rh, pmv, ppd, set;
		int countIn = 0;
		for (int i = 0; i < g_strv_length(head_var); i++) {
			if (str_find_match(head_var[i], "CLO", FALSE)) {
				clo = atof(data[i]);
				countIn++;
			}
			else if (str_find_match(head_var[i], "MET", FALSE)) {
				met = atof(data[i]);
				countIn++;
			}
			else if (str_find_match(head_var[i], "WME", FALSE)) {
				wme = atof(data[i]);
				countIn++;
			}
			else if (str_find_match(head_var[i], "TA", FALSE)) {
				ta = atof(data[i]);
				countIn++;
			}
			else if (str_find_match(head_var[i], "VEL", FALSE)) {
				vel = atof(data[i]);
				countIn++;
			}
			else if (str_find_match(head_var[i], "MRT", FALSE)) {
				tr = atof(data[i]);
				countIn++;
			}
			else if (str_find_match(head_var[i], "RH", FALSE)) {
				rh = atof(data[i]);
				countIn++;
			}
		}
		if (countIn < 7) {
			str_move_file(bakpath->str, filepath->str);

			GtkAlertDialog* msg2 = gtk_alert_dialog_new("Not enough parameter in : %s", filepath->str);
			gtk_alert_dialog_set_modal(msg2, TRUE);
			gtk_alert_dialog_show(msg2, GTK_WINDOW(gtk_widget_get_root(m->label_CLO)));

			g_free(filepath);
			g_free(bakpath);
			g_free(head_var);
			g_free(head);
			fclose(bak_file);
			fclose(out_file);

			return;
		}
		if (PMVboth(clo, met, wme, ta, tr, vel, rh, 0, &pmv, &ppd, &set) < 1)
			set = -273.15;
		snprintf(buffer, BUFFER_SIZE,
			"%.2f,%.2f,%.2f,%.2f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
			clo, met, wme, ta, vel, tr, rh, pmv, ppd, set);
		fputs(buffer, out_file);
		g_free(data);
	}

	GtkAlertDialog* msg_end = gtk_alert_dialog_new("Table file has been saved : %s\nOriginal file has been moved : %s",
		filepath->str, bakpath->str);
	gtk_alert_dialog_set_modal(msg_end, TRUE);
	gtk_alert_dialog_show(msg_end, GTK_WINDOW(gtk_widget_get_root(m->label_CLO)));

	fclose(bak_file);
	fclose(out_file);
	g_free(filepath);
	g_free(bakpath);
	g_free(head_var);
	g_free(head);
}

static void
on_bn_handle(GtkWidget* widget,
	MWidget* m)
{
	GtkFileDialog* dialog = gtk_file_dialog_new();
	gtk_file_dialog_set_title(dialog, "Select table file");
	gtk_file_dialog_set_modal(dialog, TRUE);
	GtkFileFilter* filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "CSV Files");
	gtk_file_filter_add_pattern(filter, "*.csv");
	//gtk_file_filter_add_suffix(filter, "csv");
	gtk_file_dialog_set_default_filter(dialog, filter);
	gtk_file_dialog_save(dialog, GTK_WINDOW(gtk_widget_get_root(widget)),
		NULL, (GAsyncReadyCallback)(on_bn_handle_response), m);
}

static void
on_destroy(GtkWidget* widget,
	MWidget* m)
{
	g_free(m);
}

void
activate_windows(GtkApplication* app,
	gpointer  user_data)
{
	MWidget* m_widget = g_new(MWidget, 1);

	GtkWidget* window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "PMV PPD Calc");
	//gtk_window_set_default_size (GTK_WINDOW (window), 250, 500);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_window_set_child(GTK_WINDOW(window), vbox);

	GtkWidget* input_frame = gtk_frame_new("Input");
	gtk_frame_set_label_align(GTK_FRAME(input_frame), 0.5);
	gtk_box_append(GTK_BOX(vbox), input_frame);

	GtkWidget* input_grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(input_grid), 5);
	gtk_frame_set_child(GTK_FRAME(input_frame), input_grid);

	{
		m_widget->label_CLO = gtk_label_new("CLO [clo]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->label_CLO, 0, 0, 1, 1);

		m_widget->en_CLO = gtk_spin_button_new(gtk_adjustment_new(0.5, 0.0, 999999, 0.1, 1.0, 0.0), 1.0, 2);
		gtk_widget_set_tooltip_text(m_widget->en_CLO, "clothing insulation [clo]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->en_CLO, 1, 0, 1, 1);

		m_widget->label_MET = gtk_label_new("MET [met]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->label_MET, 0, 1, 1, 1);

		m_widget->en_MET = gtk_spin_button_new(gtk_adjustment_new(1.2, 0.0, 999999, 0.1, 1.0, 0.0), 1.0, 2);
		gtk_widget_set_tooltip_text(m_widget->en_MET, "metabolic rate [met]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->en_MET, 1, 1, 1, 1);

		m_widget->label_WME = gtk_label_new("WME [met]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->label_WME, 0, 2, 1, 1);

		m_widget->en_WME = gtk_spin_button_new(gtk_adjustment_new(0.0, 0.0, 999999, 0.1, 1.0, 0.0), 1.0, 2);
		gtk_widget_set_tooltip_text(m_widget->en_WME, "mechanical work [met]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->en_WME, 1, 2, 1, 1);

		m_widget->label_TA = gtk_label_new("TA [Celsius]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->label_TA, 0, 3, 1, 1);

		m_widget->en_TA = gtk_spin_button_new(gtk_adjustment_new(22.0, -273.15, 999999, 0.1, 1.0, 0.0), 1.0, 2);
		gtk_widget_set_tooltip_text(m_widget->en_TA, "air temperature [Celsius]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->en_TA, 1, 3, 1, 1);

		m_widget->label_VEL = gtk_label_new("VEL [m/s]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->label_VEL, 0, 4, 1, 1);

		m_widget->en_VEL = gtk_spin_button_new(gtk_adjustment_new(0.1, 0, 999999, 0.01, 0.1, 0.0), 1.0, 4);
		gtk_widget_set_tooltip_text(m_widget->en_VEL, "air speed [m/s]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->en_VEL, 1, 4, 1, 1);

		m_widget->label_TR = gtk_label_new("MRT [Celsius]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->label_TR, 0, 5, 1, 1);

		m_widget->en_TR = gtk_spin_button_new(gtk_adjustment_new(22.0, -273.15, 999999, 0.1, 1.0, 0.0), 1.0, 2);
		gtk_widget_set_tooltip_text(m_widget->en_TR, "mean radiant temperature [Celsius]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->en_TR, 1, 5, 1, 1);

		m_widget->label_RH = gtk_label_new("RH [%]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->label_RH, 0, 6, 1, 1);

		m_widget->en_RH = gtk_spin_button_new(gtk_adjustment_new(60.0, 0, 100, 0.5, 1.0, 0.0), 1.0, 2);
		gtk_widget_set_tooltip_text(m_widget->en_RH, ">relative humidity [%]");
		gtk_grid_attach(GTK_GRID(input_grid), m_widget->en_RH, 1, 6, 1, 1);
	}

	GtkWidget* bn_calc = gtk_button_new_with_label("CALCULAR");
	g_signal_connect(bn_calc, "clicked", G_CALLBACK(on_bn_calc), m_widget);
	gtk_box_append(GTK_BOX(vbox), bn_calc);

	GtkWidget* output_frame = gtk_frame_new("Output");
	gtk_frame_set_label_align(GTK_FRAME(output_frame), 0.5);
	gtk_box_append(GTK_BOX(vbox), output_frame);

	GtkWidget* output_grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(output_grid), 5);
	gtk_frame_set_child(GTK_FRAME(output_frame), output_grid);

	{
		m_widget->label_PMV = gtk_label_new("PMV");
		gtk_grid_attach(GTK_GRID(output_grid), m_widget->label_PMV, 0, 0, 1, 1);

		m_widget->en_PMV = gtk_spin_button_new(gtk_adjustment_new(0.0, -999999, 999999, 0.1, 1.0, 0.0), 1.0, 2);
		gtk_widget_set_tooltip_text(m_widget->en_PMV, "predicted mean vote");
		gtk_grid_attach(GTK_GRID(output_grid), m_widget->en_PMV, 1, 0, 1, 1);

		m_widget->label_PPD = gtk_label_new("PPD [%]");
		gtk_grid_attach(GTK_GRID(output_grid), m_widget->label_PPD, 0, 1, 1, 1);

		m_widget->en_PPD = gtk_spin_button_new(gtk_adjustment_new(0.0, 0.0, 100, 0.1, 1.0, 0.0), 1.0, 2);
		gtk_widget_set_tooltip_text(m_widget->en_PPD, "predicted percentage of dissatisfied [%]");
		gtk_grid_attach(GTK_GRID(output_grid), m_widget->en_PPD, 1, 1, 1, 1);

		m_widget->label_SET = gtk_label_new("SET [Celsius]");
		gtk_grid_attach(GTK_GRID(output_grid), m_widget->label_SET, 0, 2, 1, 1);

		m_widget->en_SET = gtk_spin_button_new(gtk_adjustment_new(0.0, -273.15, 999999, 0.1, 1.0, 0.0), 1.0, 2);
		gtk_widget_set_tooltip_text(m_widget->en_SET, "standard effective temperature [Celsius]");
		gtk_grid_attach(GTK_GRID(output_grid), m_widget->en_SET, 1, 2, 1, 1);
	}

	GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_set_spacing(GTK_BOX(hbox), 5);
	gtk_box_append(GTK_BOX(vbox), hbox);

	GtkWidget* bn_gen = gtk_button_new_with_label("Generate template");
	gtk_widget_set_tooltip_text(bn_gen, "Generate template table file, using .csv format.");
	g_signal_connect(bn_gen, "clicked", G_CALLBACK(on_bn_gen), m_widget);
	gtk_box_append(GTK_BOX(hbox), bn_gen);

	GtkWidget* bn_handle = gtk_button_new_with_label("Handle .csv table");
	gtk_widget_set_tooltip_text(bn_handle, "Upload and handle table file, using .csv format.");
	g_signal_connect(bn_handle, "clicked", G_CALLBACK(on_bn_handle), m_widget);
	gtk_box_append(GTK_BOX(hbox), bn_handle);

	g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), m_widget);
	gtk_window_present(GTK_WINDOW(window));
}
