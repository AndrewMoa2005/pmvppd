#include "widget.hpp"
#include "engine_pmv_ppd.h"
#include <wx/wx.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/tokenzr.h>
#include <wx/arrstr.h>

Widget::Widget(wxWindow* parent, wxWindowID id, const wxString& title,
	const wxPoint& pos, const wxSize& size, long style) :
	MWidget(parent, id, title, pos, size, style)
{
	SetIcon(wxIcon(wxT("IDI_ICON1")));
}

void Widget::OnButtonCalcClicked(wxCommandEvent& event)
{
	double clo, met, wme, ta, tr, vel, rh, pmv, ppd, set;
	clo = en_CLO->GetValue();
	met = en_MET->GetValue();
	wme = en_WME->GetValue();
	ta = en_TA->GetValue();
	tr = en_TR->GetValue();
	vel = en_VEL->GetValue();
	rh = en_RH->GetValue();
	if (PMVboth(clo, met, wme, ta, tr, vel, rh, 0, &pmv, &ppd, &set) < 1)
		return;
	en_PMV->SetValue(wxString::FromDouble(pmv, 2));
	en_PPD->SetValue(wxString::FromDouble(ppd, 2));
	en_SET->SetValue(wxString::FromDouble(set, 2));
}

void Widget::OnButtonGenClicked(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Save template file"), "", "",
		"CSV(*.csv)|*.csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (dlg.ShowModal() == wxID_CANCEL)
		return;

	wxFFileOutputStream output(dlg.GetPath());
	if (!output.IsOk()) {
		wxMessageDialog msg(this,
			wxString::Format(wxT("Cannot save current contents in file : %s"), dlg.GetPath()));
		msg.ShowModal();
		return;
	}
	wxTextOutputStream txtout(output);

	wxString head = wxString::Format(wxT("\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\""),
		label_CLO->GetLabelText(), label_MET->GetLabelText(),
		label_WME->GetLabelText(), label_TA->GetLabelText(),
		label_VEL->GetLabelText(), label_TR->GetLabelText(),
		label_RH->GetLabelText());
	txtout << head.ToUTF8() << endl;
	wxString data = wxString::Format(wxT("%.2f,%.2f,%.2f,%.2f,%.4f,%.2f,%.2f"),
		en_CLO->GetValue(), en_MET->GetValue(),
		en_WME->GetValue(), en_TA->GetValue(),
		en_VEL->GetValue(), en_TR->GetValue(),
		en_RH->GetValue());
	txtout << data.ToUTF8() << endl;
	output.Close();
	wxMessageDialog finMsg(this, wxString::Format(wxT("Template file has been saved : %s"), dlg.GetPath()));
	finMsg.ShowModal();
}

wxArrayString StringSplit(const wxString src,
	const wxString seperstor = wxT(","))
{
	wxArrayString s;
	wxStringTokenizer t(src, seperstor);
	while (t.HasMoreTokens()) {
		wxString item = t.GetNextToken();
		s.Add(item);
	}
	return s;
}

void Widget::OnButtonHandleClicked(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Select table file"), "", "",
		"CSV(*.csv)|*.csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (dlg.ShowModal() == wxID_CANCEL)
		return;

	wxString fileName = dlg.GetPath();
	wxString bakName = fileName + wxT(".bak");

	if (!wxRenameFile(fileName, bakName)) {
		wxMessageDialog msgBox1(this, wxString::Format(wxT("Cannot backup file : %s"), fileName));
		msgBox1.ShowModal();
		return;
	}

	wxFFileInputStream input(bakName);
	wxFFileOutputStream output(fileName);
	if (!input.IsOk() || !output.IsOk())
		return;
	wxTextInputStream tin(input);
	wxTextOutputStream tout(output);

	wxString inHead = tin.ReadLine();
	wxArrayString inHeadArray = StringSplit(inHead);

	wxString outHead = wxString::Format(wxT("\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\""),
		label_CLO->GetLabelText(), label_MET->GetLabelText(),
		label_WME->GetLabelText(), label_TA->GetLabelText(),
		label_VEL->GetLabelText(), label_TR->GetLabelText(),
		label_RH->GetLabelText(), label_PMV->GetLabelText(),
		label_PPD->GetLabelText(), label_SET->GetLabelText());
	tout << outHead.ToUTF8() << endl;

	double clo, met, wme, ta, vel, tr, rh, pmv, ppd, set;
	int countIn = 0;
	bool errRead = true;
	while (1) {
		wxString inData = tin.ReadLine();
		if (inData.IsEmpty())
			break;
		wxArrayString inDataArray = StringSplit(inData);
		if (inDataArray.Count() != inHeadArray.Count())
			continue;
		countIn = 0;
		for (int i = 0; i < inHeadArray.Count(); i++) {
			if (inHeadArray[i].Contains("CLO")) {
				errRead = inDataArray[i].ToDouble(&clo);
				countIn++;
			}
			else if (inHeadArray[i].Contains("MET")) {
				errRead = inDataArray[i].ToDouble(&met);
				countIn++;
			}
			else if (inHeadArray[i].Contains("WME")) {
				errRead = inDataArray[i].ToDouble(&wme);
				countIn++;
			}
			else if (inHeadArray[i].Contains("TA")) {
				errRead = inDataArray[i].ToDouble(&ta);
				countIn++;
			}
			else if (inHeadArray[i].Contains("VEL")) {
				errRead = inDataArray[i].ToDouble(&vel);
				countIn++;
			}
			else if (inHeadArray[i].Contains("MRT")) {
				errRead = inDataArray[i].ToDouble(&tr);
				countIn++;
			}
			else if (inHeadArray[i].Contains("RH")) {
				errRead = inDataArray[i].ToDouble(&rh);
				countIn++;
			}
		}
		if (countIn < 7) {
			output.Close();
			wxRemoveFile(fileName);
			wxRenameFile(bakName, fileName);
			wxMessageDialog msgBox2(this, wxString::Format(wxT("Not enough parameter in : %s"), fileName));
			msgBox2.ShowModal();
			return;
		}
		if (!errRead) {
			output.Close();
			wxRemoveFile(fileName);
			wxRenameFile(bakName, fileName);
			wxMessageDialog msgBox3(this, wxString::Format(wxT("Parameter error in : %s"), fileName));
			msgBox3.ShowModal();
			return;
		}
		if (PMVboth(clo, met, wme, ta, tr, vel, rh, 0, &pmv, &ppd, &set) < 1)
			set = -273.15;
		tout << wxString::FromDouble(clo, 2) << wxString(wxT(",")).ToUTF8()
			<< wxString::FromDouble(met, 2) << wxString(wxT(",")).ToUTF8()
			<< wxString::FromDouble(wme, 2) << wxString(wxT(",")).ToUTF8()
			<< wxString::FromDouble(ta, 2) << wxString(wxT(",")).ToUTF8()
			<< wxString::FromDouble(vel, 4) << wxString(wxT(",")).ToUTF8()
			<< wxString::FromDouble(tr, 2) << wxString(wxT(",")).ToUTF8()
			<< wxString::FromDouble(rh, 2) << wxString(wxT(",")).ToUTF8()
			<< wxString::FromDouble(pmv, 2) << wxString(wxT(",")).ToUTF8()
			<< wxString::FromDouble(ppd, 2) << wxString(wxT(",")).ToUTF8()
			<< wxString::FromDouble(set, 2) << wxString(wxT(",")).ToUTF8()
			<< endl;
	}
	output.Close();
	wxMessageDialog msgBox4(this,
		wxString::Format(wxT("Table file has been saved : %s\nOriginal file has been moved : %s"),
			fileName, bakName));
	msgBox4.ShowModal();
}
