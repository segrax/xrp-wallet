///-----------------------------------------------------------------
///
/// @file      DialogQR.h
/// @author
/// Created:   7/01/2017 13:35:14
/// @section   DESCRIPTION
///            cDialogQR class declaration
///
///------------------------------------------------------------------

#ifndef __CDIALOGQR_H__
#define __CDIALOGQR_H__

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/dialog.h>
#else
#include <wx/wxprec.h>
#endif

//Do not add custom headers between
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/panel.h>
////Header Include End

#include "PanelQRCode.h"

////Dialog Style Start
#undef cDialogQR_STYLE
#define cDialogQR_STYLE wxNO_BORDER | wxCAPTION | wxSYSTEM_MENU | wxSTAY_ON_TOP | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End

class cDialogQR : public wxDialog
{
    private:
    DECLARE_EVENT_TABLE();

    public:
    cDialogQR( wxWindow *parent, const std::string& pQrCode, wxWindowID id = 1, const wxString &title = wxT( "Untitled2" ), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = cDialogQR_STYLE );
    virtual ~cDialogQR();

    private:
    //Do not add custom control declarations between
    //GUI Control Declaration Start and GUI Control Declaration End.
    //wxDev-C++ will remove them. Add custom code after the block.
    ////GUI Control Declaration Start
		cPanelQRCode *mAddressQR;
    ////GUI Control Declaration End

    private:
    //Note: if you receive any error with these enum IDs, then you need to
    //change your old form code that are based on the #define control IDs.
    //#defines may replace a numeric value for the enum names.
    //Try copy and pasting the below block in your old form header files.
    enum
    {
        ////GUI Enum Control ID Start
			ID_MADDRESSQR = 1001,
        ////GUI Enum Control ID End
        ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
    };

    private:
    void OnClose( wxCloseEvent& event );
    void CreateGUIControls();
};

#endif
