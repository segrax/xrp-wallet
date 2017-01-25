///-----------------------------------------------------------------
///
/// @file      DialogSusPay.h
/// @author
/// Created:   8/01/2017 16:57:34
/// @section   DESCRIPTION
///            cDialog_SusPay class declaration
///
///------------------------------------------------------------------

#ifndef __CDIALOG_SUSPAY_H__
#define __CDIALOG_SUSPAY_H__

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
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/checkbox.h>
////Header Include End
#include "wx/timectrl.h"

////Dialog Style Start
#undef cDialog_SusPay_STYLE
#define cDialog_SusPay_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End

class cDialog_SusPay : public wxDialog
{
    friend class cXrp_WalletFrm;

    private:
    DECLARE_EVENT_TABLE();
    cAccount* mWallet;

    public:
    cDialog_SusPay( wxWindow *parent, cAccount* pWallet, wxWindowID id = 1, const wxString &title = wxT( "Suspended Payment" ), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = cDialog_SusPay_STYLE );
    virtual ~cDialog_SusPay();

    const uint64_t GetCancelledAfter() const;
    const uint64_t GetExecuteAfter() const;
    const std::string GetProofText() const;
		void mFinishButtonClick(wxCommandEvent& event);
		void mCancelButtonClick(wxCommandEvent& event);

    private:
    //Do not add custom control declarations between
    //GUI Control Declaration Start and GUI Control Declaration End.
    //wxDev-C++ will remove them. Add custom code after the block.
    ////GUI Control Declaration Start
		wxStaticText *WxStaticText1;
		wxTextCtrl *mProofText;
		wxTimePickerCtrl *mExecuteAfterTime;
		wxDatePickerCtrl *mExecuteAfterDate;
		wxCheckBox *mExecuteAfterEnabled;
		wxButton *mFinishButton;
		wxButton *mCancelButton;
		wxTimePickerCtrl *mCancelAfterTime;
		wxDatePickerCtrl *mCancelAfterDate;
		wxCheckBox *mCancelAfterEnabled;
    ////GUI Control Declaration End

    private:
    //Note: if you receive any error with these enum IDs, then you need to
    //change your old form code that are based on the #define control IDs.
    //#defines may replace a numeric value for the enum names.
    //Try copy and pasting the below block in your old form header files.
    enum
    {
        ////GUI Enum Control ID Start
			ID_WXSTATICTEXT1 = 1016,
			ID_MPROOFTEXT = 1015,
			ID_MEXECUTEAFTERTIME = 1013,
			ID_MEXECUTEAFTERDATE = 1012,
			ID_MEXECUTEAFTERENABLED = 1011,
			ID_MFINISHBUTTON = 1007,
			ID_MCANCELBUTTON = 1005,
			ID_MCANCELAFTERTIME = 1004,
			ID_MCANCELAFTERDATE = 1003,
			ID_MCANCELAFTER = 1002,
        ////GUI Enum Control ID End
        ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
    };

    private:
    void OnClose( wxCloseEvent& event );
    void CreateGUIControls();
};

#endif
