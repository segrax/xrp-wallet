///-----------------------------------------------------------------
///
/// @file      DialogQR.cpp
/// @author
/// Created:   7/01/2017 13:35:14
/// @section   DESCRIPTION
///            cDialogQR class implementation
///
///------------------------------------------------------------------
#include "stdafx.hpp"
#include "DialogQR.h"
//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cDialogQR
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialogQR,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialogQR::OnClose)
END_EVENT_TABLE()
////Event Table End

cDialogQR::cDialogQR( wxWindow *parent, const std::string& pQrCode, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxDialog( parent, id, title, position, size, style )
{
    CreateGUIControls();

    SetTitle( pQrCode );
    mAddressQR->SetQRCode( pQrCode );
}

cDialogQR::~cDialogQR()
{
}

void cDialogQR::CreateGUIControls()
{
    //Do not add custom code between
    //GUI Items Creation Start and GUI Items Creation End.
    //wxDev-C++ designer will remove them.
    //Add the custom code before or after the blocks
    ////GUI Items Creation Start

	mAddressQR = new cPanelQRCode(this, ID_MADDRESSQR, wxPoint(2, 2), wxSize(289, 281), wxNO_BORDER);

	SetTitle(_("Untitled2"));
	SetIcon(wxNullIcon);
	SetSize(0,0,312,338);
	Center();
	
    ////GUI Items Creation End
}

void cDialogQR::OnClose( wxCloseEvent& /*event*/ )
{
    Destroy();
}
