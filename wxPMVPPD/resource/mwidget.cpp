///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-0-g733bf3d)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "mwidget.h"

///////////////////////////////////////////////////////////////////////////

MWidget::MWidget( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 300,520 ), wxSize( 300,520 ) );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Input") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	label_CLO = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("CLO [clo]"), wxDefaultPosition, wxDefaultSize, 0 );
	label_CLO->Wrap( -1 );
	fgSizer1->Add( label_CLO, 0, wxALL, 5 );

	en_CLO = new wxSpinCtrlDouble( sbSizer1->GetStaticBox(), wxID_ANY, wxT("0.5"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 999999, 0.5, 0.1 );
	en_CLO->SetDigits( 2 );
	en_CLO->SetToolTip( wxT("clothing insulation [clo]") );

	fgSizer1->Add( en_CLO, 0, wxALL|wxEXPAND, 5 );

	label_MET = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("MET [met]"), wxDefaultPosition, wxDefaultSize, 0 );
	label_MET->Wrap( -1 );
	fgSizer1->Add( label_MET, 0, wxALL, 5 );

	en_MET = new wxSpinCtrlDouble( sbSizer1->GetStaticBox(), wxID_ANY, wxT("1.2"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 999999, 1.2, 0.1 );
	en_MET->SetDigits( 2 );
	en_MET->SetToolTip( wxT("metabolic rate [met]") );

	fgSizer1->Add( en_MET, 0, wxALL|wxEXPAND, 5 );

	label_WME = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("WME [met]"), wxDefaultPosition, wxDefaultSize, 0 );
	label_WME->Wrap( -1 );
	fgSizer1->Add( label_WME, 0, wxALL, 5 );

	en_WME = new wxSpinCtrlDouble( sbSizer1->GetStaticBox(), wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 999999, 0, 0.1 );
	en_WME->SetDigits( 2 );
	en_WME->SetToolTip( wxT("mechanical work [met]") );

	fgSizer1->Add( en_WME, 0, wxALL|wxEXPAND, 5 );

	label_TA = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("TA [Celsius]"), wxDefaultPosition, wxDefaultSize, 0 );
	label_TA->Wrap( -1 );
	fgSizer1->Add( label_TA, 0, wxALL, 5 );

	en_TA = new wxSpinCtrlDouble( sbSizer1->GetStaticBox(), wxID_ANY, wxT("22"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -273.15, 999999, 22, 1 );
	en_TA->SetDigits( 2 );
	en_TA->SetToolTip( wxT("air temperature [Celsius]") );

	fgSizer1->Add( en_TA, 0, wxALL|wxEXPAND, 5 );

	label_VEL = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("VEL [m/s]"), wxDefaultPosition, wxDefaultSize, 0 );
	label_VEL->Wrap( -1 );
	fgSizer1->Add( label_VEL, 0, wxALL, 5 );

	en_VEL = new wxSpinCtrlDouble( sbSizer1->GetStaticBox(), wxID_ANY, wxT(".1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 9999, 0.1, 0.1 );
	en_VEL->SetDigits( 4 );
	en_VEL->SetToolTip( wxT("air speed [m/s]") );

	fgSizer1->Add( en_VEL, 0, wxALL|wxEXPAND, 5 );

	label_TR = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("MRT [Celsius]"), wxDefaultPosition, wxDefaultSize, 0 );
	label_TR->Wrap( -1 );
	fgSizer1->Add( label_TR, 0, wxALL, 5 );

	en_TR = new wxSpinCtrlDouble( sbSizer1->GetStaticBox(), wxID_ANY, wxT("22"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -273.15, 999999, 22, 1 );
	en_TR->SetDigits( 2 );
	en_TR->SetToolTip( wxT("mean radiant temperature [Celsius]") );

	fgSizer1->Add( en_TR, 0, wxALL|wxEXPAND, 5 );

	label_RH = new wxStaticText( sbSizer1->GetStaticBox(), wxID_ANY, wxT("RH [%]"), wxDefaultPosition, wxDefaultSize, 0 );
	label_RH->Wrap( -1 );
	fgSizer1->Add( label_RH, 0, wxALL, 5 );

	en_RH = new wxSpinCtrlDouble( sbSizer1->GetStaticBox(), wxID_ANY, wxT("60"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 999999, 60, 1 );
	en_RH->SetDigits( 2 );
	en_RH->SetToolTip( wxT("relative humidity [%]") );

	fgSizer1->Add( en_RH, 0, wxALL|wxEXPAND, 5 );


	sbSizer1->Add( fgSizer1, 1, wxEXPAND, 5 );


	bSizer1->Add( sbSizer1, 1, wxEXPAND, 5 );

	bn_calc = new wxButton( this, wxID_ANY, wxT("CALCULAR"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( bn_calc, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Output") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->AddGrowableCol( 1 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	label_PMV = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("PMV"), wxDefaultPosition, wxDefaultSize, 0 );
	label_PMV->Wrap( -1 );
	fgSizer2->Add( label_PMV, 0, wxALL, 5 );

	en_PMV = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	en_PMV->SetToolTip( wxT("predicted mean vote") );

	fgSizer2->Add( en_PMV, 0, wxALL|wxEXPAND, 5 );

	label_PPD = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("PPD [%]"), wxDefaultPosition, wxDefaultSize, 0 );
	label_PPD->Wrap( -1 );
	fgSizer2->Add( label_PPD, 0, wxALL, 5 );

	en_PPD = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	en_PPD->SetToolTip( wxT("predicted percentage of dissatisfied [%]") );

	fgSizer2->Add( en_PPD, 0, wxALL|wxEXPAND, 5 );

	label_SET = new wxStaticText( sbSizer2->GetStaticBox(), wxID_ANY, wxT("SET [Celsius]"), wxDefaultPosition, wxDefaultSize, 0 );
	label_SET->Wrap( -1 );
	fgSizer2->Add( label_SET, 0, wxALL, 5 );

	en_SET = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	en_SET->SetToolTip( wxT("standard effective temperature [Celsius]") );

	fgSizer2->Add( en_SET, 0, wxALL|wxEXPAND, 5 );


	sbSizer2->Add( fgSizer2, 1, wxEXPAND, 5 );


	bSizer1->Add( sbSizer2, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	bn_gen = new wxButton( this, wxID_ANY, wxT("Generate template"), wxDefaultPosition, wxDefaultSize, 0 );
	bn_gen->SetToolTip( wxT("Generate template table file, using .csv format.") );

	bSizer2->Add( bn_gen, 0, wxALL, 5 );


	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );

	bn_handle = new wxButton( this, wxID_ANY, wxT("Handle .csv table"), wxDefaultPosition, wxDefaultSize, 0 );
	bn_handle->SetToolTip( wxT("Upload and handle table file, using .csv format.") );

	bSizer2->Add( bn_handle, 0, wxALL, 5 );


	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	bn_calc->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MWidget::OnButtonCalcClicked ), NULL, this );
	bn_gen->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MWidget::OnButtonGenClicked ), NULL, this );
	bn_handle->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MWidget::OnButtonHandleClicked ), NULL, this );
}

MWidget::~MWidget()
{
	// Disconnect Events
	bn_calc->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MWidget::OnButtonCalcClicked ), NULL, this );
	bn_gen->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MWidget::OnButtonGenClicked ), NULL, this );
	bn_handle->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MWidget::OnButtonHandleClicked ), NULL, this );

}
