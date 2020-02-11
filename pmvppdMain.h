/***************************************************************
 * Name:      pmvppdMain.h
 * Purpose:   Defines Application Frame
 * Author:    陈瑞祥 (Andrew.Moa2005@163.om)
 * Created:   2019-12-10
 * Copyright: 陈瑞祥 ()
 * License:
 **************************************************************/

#ifndef PMVPPDMAIN_H
#define PMVPPDMAIN_H

#include "GUIDialog.h"

class pmvppdDialog: public GUIDialog
{
public:
    pmvppdDialog(wxDialog *dlg);
    ~pmvppdDialog();
private:
    virtual void closeDialog( wxCloseEvent& event );
    virtual void OnText( wxCommandEvent& event );
    virtual void OnChoice( wxCommandEvent& event );
    virtual void OnCalc( wxCommandEvent& event );

    void keyUpdate();
    void choiceUpdate();
    void calcPmvppd();
};
#endif // PMVPPDMAIN_H
