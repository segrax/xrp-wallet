//---------------------------------------------------------------------------
//
// Name:        Xrp_WalletApp.h
// Author:
// Created:     4/01/2017 07:55:31
// Description:
//
//---------------------------------------------------------------------------

#ifndef __CXRP_WALLETFRMApp_h__
#define __CXRP_WALLETFRMApp_h__

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#else
#include <wx/wxprec.h>
#endif

class cXrp_WalletFrmApp : public wxApp
{
    public:
    bool OnInit();
    int OnExit();
};

#endif
