#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include "widget.hxx"

int main(int argc, char** argv)
{
	Widget m_win;
	m_win.window()->show();
	return Fl::run();
}
