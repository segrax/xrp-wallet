///-----------------------------------------------------------------
///
/// @file      DialogAccountSettings.h
/// @author
/// Created:   14/01/2017 12:02:12
/// @section   DESCRIPTION
///            cDialogAccountSettings class declaration
///
///------------------------------------------------------------------

#ifndef __CDIALOGACCOUNTSETTINGS_H__
#define __CDIALOGACCOUNTSETTINGS_H__

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
#include <wx/button.h>
#include <wx/checklst.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
////Header Include End

////Dialog Style Start
#undef cDialogAccountSettings_STYLE
#define cDialogAccountSettings_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class cAccount;

class cDialogAccountSettings : public wxDialog
{
    friend class cXrp_WalletFrm;

    private:
    DECLARE_EVENT_TABLE();
    cAccount*		mWallet;

    public:
    cDialogAccountSettings( wxWindow *parent, cAccount* pWallet, wxWindowID id = 1, const wxString &title = wxT( "Account Settings" ), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = cDialogAccountSettings_STYLE );
    virtual ~cDialogAccountSettings();
    void mButtonDoneClick( wxCommandEvent& event );
    void mButtonCancelClick( wxCommandEvent& event );

    private:
    //Do not add custom control declarations between
    //GUI Control Declaration Start and GUI Control Declaration End.
    //wxDev-C++ will remove them. Add custom code after the block.
    ////GUI Control Declaration Start
		wxButton *mButtonCancel;
		wxButton *mButtonDone;
		wxCheckListBox *mFlags;
		wxTextCtrl *mTransferRate;
		wxStaticText *WxStaticText4;
		wxTextCtrl *mMessageKey;
		wxStaticText *WxStaticText3;
		wxStaticText *WxStaticText2;
		wxTextCtrl *mEmailAddress;
		wxStaticText *WxStaticText1;
		wxTextCtrl *mDomain;
    ////GUI Control Declaration End

    private:
    //Note: if you receive any error with these enum IDs, then you need to
    //change your old form code that are based on the #define control IDs.
    //#defines may replace a numeric value for the enum names.
    //Try copy and pasting the below block in your old form header files.
    enum
    {
        ////GUI Enum Control ID Start
			ID_MBUTTONCANCEL = 1017,
			ID_MBUTTONDONE = 1016,
			ID_MFLAGS = 1015,
			ID_MTRANSFERRATE = 1012,
			ID_WXSTATICTEXT4 = 1011,
			ID_MMESSAGEKEY = 1008,
			ID_WXSTATICTEXT3 = 1007,
			ID_WXSTATICTEXT2 = 1005,
			ID_MEMAILADDRESS = 1004,
			ID_WXSTATICTEXT1 = 1002,
			ID_MDOMAIN = 1001,
        ////GUI Enum Control ID End
        ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
    };

    private:
    void OnClose( wxCloseEvent& event );
    void CreateGUIControls();
};

#endif
