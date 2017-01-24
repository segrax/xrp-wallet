//---------------------------------------------------------------------------
//
// Name:        Xrp_WalletApp.cpp
// Author:
// Created:     4/01/2017 07:55:31
// Description:
//
//---------------------------------------------------------------------------

#include "stdafx.hpp"
#include "Xrp_WalletFrm.h"

IMPLEMENT_APP( cXrp_WalletFrmApp )

bool cXrp_WalletFrmApp::OnInit()
{
    cXrp_WalletFrm* frame = new cXrp_WalletFrm( NULL );
    SetTopWindow( frame );
    frame->Show();
    return true;
}

int cXrp_WalletFrmApp::OnExit()
{
    delete &g_WalletManager;
    return 0;
}