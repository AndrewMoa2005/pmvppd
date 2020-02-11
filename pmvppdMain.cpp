/***************************************************************
 * Name:      pmvppdMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    陈瑞祥 (Andrew.Moa2005@163.om)
 * Created:   2019-12-10
 * Copyright: 陈瑞祥 ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "pmvppdMain.h"
#include "pmvppd.h"

pmvppdDialog::pmvppdDialog(wxDialog *dlg)
    : GUIDialog(dlg)
{
    keyUpdate();
}

pmvppdDialog::~pmvppdDialog()
{
}

void pmvppdDialog::closeDialog( wxCloseEvent& event )
{
    //event.Skip();
    Destroy();
}
void pmvppdDialog::OnText( wxCommandEvent& event )
{
    keyUpdate();
    event.Skip();
}
void pmvppdDialog::OnChoice( wxCommandEvent& event )
{
    choiceUpdate();
    event.Skip();
}
void pmvppdDialog::OnCalc( wxCommandEvent& event )
{
    keyUpdate();
    calcPmvppd();
    event.Skip();
}
void pmvppdDialog::keyUpdate()
{
    double clo,met,wme;
    if(!cloCtrl->GetValue().ToDouble(&clo))
        return;
    if(!metCtrl->GetValue().ToDouble(&met))
        return;
    if(!wmeCtrl->GetValue().ToDouble(&wme))
        return;

    wxString result(wxT("CLO="));
    result.Append(wxString::FromDouble(clo*0.155,3));
    result.Append(wxT("m2K/W, MET="));
    result.Append(wxString::FromDouble(met*58.15,2));
    result.Append(wxT("W/m2, WME="));
    result.Append(wxString::FromDouble(wme*58.15,2));
    result.Append(wxT("W/m2"));

    resultText->SetLabel(result);
}
void pmvppdDialog::choiceUpdate()
{
    double choice,ta;
    if(!taCtrl->GetValue().ToDouble(&ta))
        return;
    if(!choiceCtrl->GetValue().ToDouble(&choice))
        return;

    if(m_choice->GetSelection()>0)
        choiceCtrl->SetValue(wxString::FromDouble(rh2pa(choice,ta)));
    else
        choiceCtrl->SetValue(wxString::FromDouble(pa2rh(choice,ta)));
}
void pmvppdDialog::calcPmvppd()
{
    double clo,met,wme,ta,tr,vel,rh,pa;
    double pmv,ppd;
    if(!cloCtrl->GetValue().ToDouble(&clo))
        return;
    if(!metCtrl->GetValue().ToDouble(&met))
        return;
    if(!wmeCtrl->GetValue().ToDouble(&wme))
        return;
    if(!taCtrl->GetValue().ToDouble(&ta))
        return;
    if(!trCtrl->GetValue().ToDouble(&tr))
        return;
    if(!velCtrl->GetValue().ToDouble(&vel))
        return;

    if(m_choice->GetSelection()>0)
    {
        if(!choiceCtrl->GetValue().ToDouble(&pa))
            return;
        rh=pa2rh(pa,ta);
    }
    else
    {
        if(!choiceCtrl->GetValue().ToDouble(&rh))
            return;
        pa=rh2pa(rh,ta);
    }

    if(pmvppd(&pmv,&ppd,clo,met,wme,ta,tr,vel,rh,pa)<=0)
        return;
    pmvCtrl->SetValue(wxString::FromDouble(pmv,2));
    ppdCtrl->SetValue(wxString::FromDouble(ppd,1));
/*
    wxString result(resultText->GetLabel());
    result.Append(wxT(", PMV="));
    result.Append(wxString::FromDouble(pmv,2));
    result.Append(wxT(", PPD="));
    result.Append(wxString::FromDouble(ppd,1));
    result.Append(wxT("%"));

    resultText->SetLabel(result);

*/
}
