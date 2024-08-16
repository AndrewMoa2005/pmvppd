#include <wx/app.h>
#include "widget.hpp"

class mApp : public wxApp
{
public:
	virtual bool OnInit();
};

IMPLEMENT_APP(mApp)

bool mApp::OnInit()
{
	Widget* w = new Widget(NULL);
	w->Show(TRUE);
	return TRUE;
}
