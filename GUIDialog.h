///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUIDialog__
#define __GUIDialog__

// Define WX_GCH in order to support precompiled headers with GCC compiler.
// You have to create the header "wx_pch.h" and include all files needed
// for compile your gui inside it.
// Then, compile it and place the file "wx_pch.h.gch" into the same
// directory that "wx_pch.h".
#ifdef WX_GCH
#include <wx_pch.h>
#else
#include <wx/wx.h>
#endif

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class GUIDialog
///////////////////////////////////////////////////////////////////////////////
class GUIDialog : public wxDialog
{
	private:

	protected:
		wxStaticText* cloText;
		wxTextCtrl* cloCtrl;
		wxStaticText* metText;
		wxTextCtrl* metCtrl;
		wxStaticText* wmeText;
		wxTextCtrl* wmeCtrl;
		wxStaticText* taText;
		wxTextCtrl* taCtrl;
		wxStaticText* trText;
		wxTextCtrl* trCtrl;
		wxStaticText* velText;
		wxTextCtrl* velCtrl;
		wxChoice* m_choice;
		wxTextCtrl* choiceCtrl;
		wxButton* calcButton;
		wxStaticLine* hLine1;
		wxStaticText* pmvText;
		wxTextCtrl* pmvCtrl;
		wxStaticText* ppdText;
		wxTextCtrl* ppdCtrl;
		wxStaticLine* hLine2;
		wxStaticText* resultText;

		// Virtual event handlers, overide them in your derived class
		virtual void closeDialog( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCalc( wxCommandEvent& event ) { event.Skip(); }


	public:

		GUIDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ISO 7730 - PMV/PPD Calculator By AndrewMoa"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~GUIDialog();

};

#endif //__GUIDialog__
