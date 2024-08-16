#include "widget.hxx"
#include <Fl/fl_ask.H>
#include <cstdio>
#include <string>
#include "engine_pmv_ppd.h"
#include <FL/Fl_Native_File_Chooser.H>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#ifdef WIN32
#include <FL/x.H>
#include <windows.h>
#include "resource.h"
#endif

Widget::Widget()
{
	make_window();
#ifdef WIN32
	window()->icon((const void*)LoadIcon(fl_display, MAKEINTRESOURCE(IDI_ICON1)));
#endif
}

Fl_Double_Window* Widget::window()
{
	return mWindow;
}

void onBnCalc(Fl_Button* b, void* v)
{
	MWidget* w = (MWidget*)v;

	double clo = w->spCLO->value();
	double met = w->spMET->value();
	double wme = w->spWME->value();
	double ta = w->spTA->value();
	double tr = w->spTR->value();
	double vel = w->spVEL->value();
	double rh = w->spRH->value();
	double pa = 0.0;
	double pmv = 0.0;
	double ppd = 0.0;
	double set = 0.0;
	if (PMVboth(clo, met, wme, ta, tr, vel, rh, pa, &pmv, &ppd, &set) < 1)
		return;
	char num[64];
	sprintf(num, "%.2f", pmv);
	w->spPMV->value(num);
	sprintf(num, "%.2f", ppd);
	w->spPPD->value(num);
	sprintf(num, "%.2f", set);
	w->spSET->value(num);
}

void onBnGen(Fl_Button* b, void* v)
{
	MWidget* w = (MWidget*)v;

	Fl_Native_File_Chooser fnfc;
	fnfc.title("Save template file");
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	fnfc.filter("CSV\t*.csv");
	fnfc.options(Fl_Native_File_Chooser::SAVEAS_CONFIRM | Fl_Native_File_Chooser::USE_FILTER_EXT);
	std::string fname;
	// Show native chooser
	switch (fnfc.show()) {
	case -1:
		return; // ERROR
	case  1:
		return; // CANCEL
	default:
		fname.clear();
		fname.assign(fnfc.filename());
		break; // FILE CHOSEN
	}
	std::filesystem::path fpath(fname);
	if (!std::filesystem::exists(fpath)) {
		if (!fpath.has_extension()) {
			fpath.replace_extension(".csv");
		}
	}

	std::fstream outfile;
	outfile.open(fpath, std::ios::out | std::ios::trunc);
	if (!outfile.is_open())
		return;

	char headlist[2048];
	sprintf(headlist, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"",
		w->spCLO->label(), w->spMET->label(),
		w->spWME->label(), w->spTA->label(),
		w->spVEL->label(), w->spTR->label(),
		w->spRH->label());
	outfile << headlist << std::endl;
	char numlist[1024];
	sprintf(numlist, "%.2f,%.2f,%.2f,%.2f,%.4f,%.2f,%.2f",
		w->spCLO->value(), w->spMET->value(),
		w->spWME->value(), w->spTA->value(),
		w->spVEL->value(), w->spTR->value(),
		w->spRH->value());

	outfile << numlist << std::endl;
	outfile.close();

	fl_message("Template file has been saved : %s", fpath.string().c_str());
}

void splitString(std::vector<std::string>& v,
	char* str, char delimiter = ',')
{
	std::istringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delimiter)) {
		v.push_back(token);
	}
}

bool isContainsStr(std::string& str, const char* sub)
{
	std::string s_sub(sub);
	std::string::size_type idx = str.find(s_sub);
	if (idx != std::string::npos)
		return true;
	else
		return false;
}

void onBnHandle(Fl_Button* b, void* v)
{
	MWidget* w = (MWidget*)v;

	Fl_Native_File_Chooser fnfc;
	fnfc.title("Select table file");
	fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
	fnfc.filter("CSV\t*.csv");
	fnfc.options(Fl_Native_File_Chooser::SAVEAS_CONFIRM | Fl_Native_File_Chooser::USE_FILTER_EXT);
	std::string fname;
	// Show native chooser
	switch (fnfc.show()) {
	case -1:
		return; // ERROR
	case  1:
		return; // CANCEL
	default:
		fname.clear();
		fname.assign(fnfc.filename());
		break; // FILE CHOSEN
	}
	std::filesystem::path outpath(fname);
	if (!std::filesystem::exists(outpath)) {
		return;
	}

	std::filesystem::path inpath(fname);
	inpath.replace_extension(".csv_bak");

	std::filesystem::rename(outpath, inpath);

	std::fstream infile;
	infile.open(inpath, std::ios::in);
	if (!infile.is_open())
		return;
	std::fstream outfile;
	outfile.open(outpath, std::ios::out | std::ios::trunc);
	if (!outfile.is_open())
		return;

	char inheadstr[2048];
	infile.getline(inheadstr, 2048);
	std::vector<std::string> inheadlist;
	splitString(inheadlist, inheadstr);

	char outheadstr[2048];
	sprintf(outheadstr, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"",
		w->spCLO->label(), w->spMET->label(),
		w->spWME->label(), w->spTA->label(),
		w->spVEL->label(), w->spTR->label(),
		w->spRH->label(), w->spPMV->label(),
		w->spPPD->label(), w->spSET->label());
	outfile << outheadstr << std::endl;

	double clo, met, wme, ta, vel, tr, rh, pmv, ppd, set;
	int countIn = 0;
	bool errRead = false;
	char innumstr[1024];
	std::vector<std::string> innumlist;
	char outnumberstr[1024];

	while (!infile.eof()) {
		infile.getline(innumstr, 1024);
		if (!innumlist.empty())
			innumlist.clear();
		splitString(innumlist, innumstr);
		if (innumlist.size() != inheadlist.size())
			continue;
		countIn = 0;
		for (int i = 0; i < inheadlist.size(); i++) {
			if (isContainsStr(inheadlist[i], "CLO")) {
				clo = std::stod(innumlist[i]);
				countIn++;
			}
			else if (isContainsStr(inheadlist[i], "MET")) {
				met = std::stod(innumlist[i]);
				countIn++;
			}
			else if (isContainsStr(inheadlist[i], "WME")) {
				wme = std::stod(innumlist[i]);
				countIn++;
			}
			else if (isContainsStr(inheadlist[i], "TA")) {
				ta = std::stod(innumlist[i]);
				countIn++;
			}
			else if (isContainsStr(inheadlist[i], "VEL")) {
				vel = std::stod(innumlist[i]);
				countIn++;
			}
			else if (isContainsStr(inheadlist[i], "MRT")) {
				tr = std::stod(innumlist[i]);
				countIn++;
			}
			else if (isContainsStr(inheadlist[i], "RH")) {
				rh = std::stod(innumlist[i]);
				countIn++;
			}
		}
		if (countIn < 7) {
			infile.close();
			outfile.close();
			std::filesystem::remove(outpath);
			std::filesystem::rename(inpath, outpath);
			fl_message("Not enough parameter in : %s", outpath.string().c_str());
			return;
		}
		if (errRead) {
			infile.close();
			outfile.close();
			std::filesystem::remove(outpath);
			std::filesystem::rename(inpath, outpath);
			fl_message("Parameter error in : %s", outpath.string().c_str());
			return;
		}
		if (PMVboth(clo, met, wme, ta, tr, vel, rh, 0, &pmv, &ppd, &set) < 1)
			set = -273.15;
		sprintf(outnumberstr, "%.2f,%.2f,%.2f,%.2f,%.4f,%.2f,%.2f,%.2f,%.2f,%.2f",
			clo, met, wme, ta, vel, tr, rh, pmv, ppd, set);
		outfile << outnumberstr << std::endl;
	}
	infile.close();
	outfile.close();
	fl_message("Table file has been saved : %s\nOriginal file has been moved : %s",
		outpath.string().c_str(), inpath.string().c_str());
}
