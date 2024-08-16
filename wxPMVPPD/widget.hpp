#ifndef WIDGET_H
#define WIDGET_H

#include "mwidget.h"

class Widget : public MWidget
{
public:
	Widget(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("PMV PPD Calc"),
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	~Widget() {}

protected:
	void OnButtonCalcClicked(wxCommandEvent& event);
	void OnButtonGenClicked(wxCommandEvent& event);
	void OnButtonHandleClicked(wxCommandEvent& event);
};

#endif // WIDGET_H
