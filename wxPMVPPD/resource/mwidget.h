///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-0-g733bf3d)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/textctrl.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MWidget
///////////////////////////////////////////////////////////////////////////////
class MWidget : public wxFrame
{
	private:

	protected:
		wxStaticText* label_CLO;
		wxSpinCtrlDouble* en_CLO;
		wxStaticText* label_MET;
		wxSpinCtrlDouble* en_MET;
		wxStaticText* label_WME;
		wxSpinCtrlDouble* en_WME;
		wxStaticText* label_TA;
		wxSpinCtrlDouble* en_TA;
		wxStaticText* label_VEL;
		wxSpinCtrlDouble* en_VEL;
		wxStaticText* label_TR;
		wxSpinCtrlDouble* en_TR;
		wxStaticText* label_RH;
		wxSpinCtrlDouble* en_RH;
		wxButton* bn_calc;
		wxStaticText* label_PMV;
		wxTextCtrl* en_PMV;
		wxStaticText* label_PPD;
		wxTextCtrl* en_PPD;
		wxStaticText* label_SET;
		wxTextCtrl* en_SET;
		wxButton* bn_gen;
		wxButton* bn_handle;

		// Virtual event handlers, override them in your derived class
		virtual void OnButtonCalcClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonGenClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonHandleClicked( wxCommandEvent& event ) { event.Skip(); }


	public:

		MWidget( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("PMV PPD Calc"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 300,520 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MWidget();

};

