#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "mwidget.hxx"

void onBnCalc(Fl_Button*, void*);
void onBnGen(Fl_Button*, void*);
void onBnHandle(Fl_Button*, void*);

class Widget : public MWidget
{
public:
	Widget();
	Fl_Double_Window* window();
};

#endif //_WIDGET_H_
