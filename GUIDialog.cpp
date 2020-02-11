///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIDialog.h"

///////////////////////////////////////////////////////////////////////////

GUIDialog::GUIDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* vSizer1;
	vSizer1 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* hSizer1;
	hSizer1 = new wxBoxSizer( wxHORIZONTAL );


	hSizer1->Add( 0, 0, 1, wxEXPAND, 5 );

	cloText = new wxStaticText( this, wxID_ANY, wxT("Clothing (clo)"), wxDefaultPosition, wxDefaultSize, 0 );
	cloText->Wrap( -1 );
	hSizer1->Add( cloText, 0, wxALL, 5 );

	cloCtrl = new wxTextCtrl( this, wxID_ANY, wxT("1.0"), wxDefaultPosition, wxDefaultSize, 0 );
	hSizer1->Add( cloCtrl, 0, wxALL, 5 );


	vSizer1->Add( hSizer1, 1, wxEXPAND, 5 );

	wxBoxSizer* hSizer2;
	hSizer2 = new wxBoxSizer( wxHORIZONTAL );


	hSizer2->Add( 0, 0, 1, wxEXPAND, 5 );

	metText = new wxStaticText( this, wxID_ANY, wxT("Metabolic rate (met)"), wxDefaultPosition, wxDefaultSize, 0 );
	metText->Wrap( -1 );
	hSizer2->Add( metText, 0, wxALL, 5 );

	metCtrl = new wxTextCtrl( this, wxID_ANY, wxT("1.2"), wxDefaultPosition, wxDefaultSize, 0 );
	hSizer2->Add( metCtrl, 0, wxALL, 5 );


	vSizer1->Add( hSizer2, 1, wxEXPAND, 5 );

	wxBoxSizer* hSizer3;
	hSizer3 = new wxBoxSizer( wxHORIZONTAL );


	hSizer3->Add( 0, 0, 1, wxEXPAND, 5 );

	wmeText = new wxStaticText( this, wxID_ANY, wxT("External work (met)"), wxDefaultPosition, wxDefaultSize, 0 );
	wmeText->Wrap( -1 );
	hSizer3->Add( wmeText, 0, wxALL, 5 );

	wmeCtrl = new wxTextCtrl( this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, 0 );
	hSizer3->Add( wmeCtrl, 0, wxALL, 5 );


	vSizer1->Add( hSizer3, 1, wxEXPAND, 5 );

	wxBoxSizer* hSizer4;
	hSizer4 = new wxBoxSizer( wxHORIZONTAL );


	hSizer4->Add( 0, 0, 1, wxEXPAND, 5 );

	taText = new wxStaticText( this, wxID_ANY, wxT("Air temperature (℃)"), wxDefaultPosition, wxDefaultSize, 0 );
	taText->Wrap( -1 );
	hSizer4->Add( taText, 0, wxALL, 5 );

	taCtrl = new wxTextCtrl( this, wxID_ANY, wxT("19.0"), wxDefaultPosition, wxDefaultSize, 0 );
	hSizer4->Add( taCtrl, 0, wxALL, 5 );


	vSizer1->Add( hSizer4, 1, wxEXPAND, 5 );

	wxBoxSizer* hSizer5;
	hSizer5 = new wxBoxSizer( wxHORIZONTAL );


	hSizer5->Add( 0, 0, 1, wxEXPAND, 5 );

	trText = new wxStaticText( this, wxID_ANY, wxT("Mean radiant temperature (℃)"), wxDefaultPosition, wxDefaultSize, 0 );
	trText->Wrap( -1 );
	hSizer5->Add( trText, 0, wxALL, 5 );

	trCtrl = new wxTextCtrl( this, wxID_ANY, wxT("18.0"), wxDefaultPosition, wxDefaultSize, 0 );
	hSizer5->Add( trCtrl, 0, wxALL, 5 );


	vSizer1->Add( hSizer5, 1, wxEXPAND, 5 );

	wxBoxSizer* hSizer6;
	hSizer6 = new wxBoxSizer( wxHORIZONTAL );


	hSizer6->Add( 0, 0, 1, wxEXPAND, 5 );

	velText = new wxStaticText( this, wxID_ANY, wxT("Relative air velocity (m/s)"), wxDefaultPosition, wxDefaultSize, 0 );
	velText->Wrap( -1 );
	hSizer6->Add( velText, 0, wxALL, 5 );

	velCtrl = new wxTextCtrl( this, wxID_ANY, wxT("0.1"), wxDefaultPosition, wxDefaultSize, 0 );
	hSizer6->Add( velCtrl, 0, wxALL, 5 );


	vSizer1->Add( hSizer6, 1, wxEXPAND, 5 );

	wxBoxSizer* hSizer7;
	hSizer7 = new wxBoxSizer( wxHORIZONTAL );

	wxString m_choiceChoices[] = { wxT("Relative humidity (%)"), wxT("Partial water vapour pressure (Pa)") };
	int m_choiceNChoices = sizeof( m_choiceChoices ) / sizeof( wxString );
	m_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceNChoices, m_choiceChoices, 0 );
	m_choice->SetSelection( 0 );
	hSizer7->Add( m_choice, 0, wxALL, 5 );

	choiceCtrl = new wxTextCtrl( this, wxID_ANY, wxT("40"), wxDefaultPosition, wxDefaultSize, 0 );
	hSizer7->Add( choiceCtrl, 0, wxALL, 5 );


	vSizer1->Add( hSizer7, 1, wxEXPAND, 5 );

	calcButton = new wxButton( this, wxID_ANY, wxT("Calculate"), wxDefaultPosition, wxDefaultSize, 0 );
	vSizer1->Add( calcButton, 0, wxALL|wxEXPAND, 5 );

	hLine1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	vSizer1->Add( hLine1, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* hSizer8;
	hSizer8 = new wxBoxSizer( wxHORIZONTAL );


	hSizer8->Add( 0, 0, 1, wxEXPAND, 5 );

	pmvText = new wxStaticText( this, wxID_ANY, wxT("PMV"), wxDefaultPosition, wxDefaultSize, 0 );
	pmvText->Wrap( -1 );
	hSizer8->Add( pmvText, 0, wxALL, 5 );

	pmvCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	hSizer8->Add( pmvCtrl, 0, wxALL, 5 );

	ppdText = new wxStaticText( this, wxID_ANY, wxT("PPD (%)"), wxDefaultPosition, wxDefaultSize, 0 );
	ppdText->Wrap( -1 );
	hSizer8->Add( ppdText, 0, wxALL, 5 );

	ppdCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	hSizer8->Add( ppdCtrl, 0, wxALL, 5 );


	vSizer1->Add( hSizer8, 1, wxEXPAND, 5 );

	hLine2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	vSizer1->Add( hLine2, 0, wxEXPAND | wxALL, 5 );

	resultText = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	resultText->Wrap( -1 );
	vSizer1->Add( resultText, 0, wxALL, 5 );


	this->SetSizer( vSizer1 );
	this->Layout();
	vSizer1->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIDialog::closeDialog ) );
	cloCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIDialog::OnText ), NULL, this );
	metCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIDialog::OnText ), NULL, this );
	wmeCtrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIDialog::OnText ), NULL, this );
	m_choice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIDialog::OnChoice ), NULL, this );
	calcButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialog::OnCalc ), NULL, this );
}

GUIDialog::~GUIDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIDialog::closeDialog ) );
	cloCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIDialog::OnText ), NULL, this );
	metCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIDialog::OnText ), NULL, this );
	wmeCtrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GUIDialog::OnText ), NULL, this );
	m_choice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GUIDialog::OnChoice ), NULL, this );
	calcButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialog::OnCalc ), NULL, this );

}
