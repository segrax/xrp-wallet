///-----------------------------------------------------------------
///
/// @file      Xrp_WalletFrm.h
/// @author
/// Created:   4/01/2017 07:55:31
/// @section   DESCRIPTION
///            cXrp_WalletFrm class declaration
///
///------------------------------------------------------------------

#ifndef __CXRP_WALLETFRM_H__
#define __CXRP_WALLETFRM_H__

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/frame.h>
#else
#include <wx/wxprec.h>
#endif

//Do not add custom headers between
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/menu.h>
#include <wx/radiobut.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/notebook.h>
////Header Include End
#include "wx/timectrl.h"

////Dialog Style Start
#undef cXrp_WalletFrm_STYLE
#define cXrp_WalletFrm_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class cXrp_WalletFrm : public wxFrame
{
    private:
    DECLARE_EVENT_TABLE();

    public:
    cXrp_WalletFrm( wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT( "cXrp_Wallet" ), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = cXrp_WalletFrm_STYLE );
    virtual ~cXrp_WalletFrm();
    void mWalletBtnCreateClick( wxCommandEvent& event );
    void mTxBtnSignClick( wxCommandEvent& event );
    void mWalletsListItemActivated( wxListEvent& event );
    void mWalletListPopupMenu_ShowSecret( wxCommandEvent& event );
    void mTxSourceWalletSelected( wxCommandEvent& event );
    void mWalletBtnImportClick( wxCommandEvent& event );
    void mWalletListPopupMenu_CopyAddress( wxCommandEvent& event );
    void mWalletListPopupMenu_CopySecret( wxCommandEvent& event );
    void mWalletListPopupMenu_SetXRP( wxCommandEvent& event );
    void mWalletListPopupMenu_ShowQR( wxCommandEvent& event );
    void mOutputFormat_JsonClick( wxCommandEvent& event );
    void mOutputFormat_WebSocketClick( wxCommandEvent& event );
    void mAddressBookAddClick( wxCommandEvent& event );
    void ReloadWallets();
    void ReloadTransactionOutput();
    void ReloadTransactions();
    void ReloadAddressBook();
    void ReloadCommandOutput();
    void mAddressBookListEndLabelEdit( wxListEvent& event );
    void mMenuDatabase_ChangePassword( wxCommandEvent& event );
    void mWalletListPopupMenu_SetSignerList( wxCommandEvent& event );
    void mWalletListPopupMenu_SetSequenceNumber( wxCommandEvent& event );
    void mWalletListPopupMenu_AccountSettings( wxCommandEvent& event );
    void mButtonSignForClick( wxCommandEvent& event );
    void mTxSignWithWalletSelected( wxCommandEvent& event );
	void mTxFeeDropsUpdated(wxCommandEvent& event);
	void mRPCShowQRCodeClick(wxCommandEvent& event);
	void mAddressBookListItemActivated(wxListEvent& event);
	void mWalletListPopupMenu_AccountInfo(wxCommandEvent& event);
	void mWalletListPopupMenu_LoadAccountInfo(wxCommandEvent& event);
	void mWalletListPopupMenu_SusPay_Execute(wxCommandEvent& event);
	void mWalletListPopupMenu_SusPay_Cancel(wxCommandEvent& event);
		void mTxSeqNumUpdated(wxCommandEvent& event);

    private:
    //Do not add custom control declarations between
    //GUI Control Declaration Start and GUI Control Declaration End.
    //wxDev-C++ will remove them. Add custom code after the block.
    ////GUI Control Declaration Start
		wxMenuBar *WxMenuBar1;
		wxMenu *mWalletListPopupMenu;
		wxButton *mRPCShowQRCode;
		wxRadioButton *mOutputFormat_WebSocket;
		wxRadioButton *mOutputFormat_Json;
		wxRichTextCtrl *WxRichTextCtrl1;
		wxPanel *WxPanel1;
		wxListCtrl *mTransactionsList;
		wxPanel *WxNoteBookPage6;
		wxButton *mAddressBookAdd;
		wxListCtrl *mAddressBookList;
		wxPanel *WxNoteBookPage5;
		wxButton *mButtonSignFor;
		wxStaticText *WxStaticText8;
		wxStaticText *WxStaticText7;
		wxComboBox *mTxSignWithWallet;
		wxRichTextCtrl *mSignForTransaction;
		wxPanel *WxNoteBookPage4;
		wxStaticText *WxStaticText9;
		wxStaticText *mLabelMultiSigsWarning;
		wxButton *mTxBtnSign;
		wxCheckBox *mTxSuspendPayment;
		wxTextCtrl *mTxSeqNum;
		wxComboBox *mTxAmountType;
		wxTextCtrl *mTxAmount;
		wxTextCtrl *mTxDestinationTag;
		wxTextCtrl *mTxDestination;
		wxComboBox *mTxSourceWallet;
		wxStaticText *WxStaticText6;
		wxStaticText *WxStaticText5;
		wxStaticText *WxStaticText2;
		wxStaticText *WxStaticText1;
		wxPanel *WxNoteBookPage3;
		wxButton *mButtonLoadAccountInfo;
		wxStaticText *WxStaticText3;
		wxTextCtrl *mTxFeeDrops;
		wxStaticText *WxStaticText4;
		wxButton *mWalletBtnImport;
		wxButton *mWalletBtnCreate;
		wxListCtrl *mWalletsList;
		wxPanel *WxNoteBookPage2;
		wxPanel *WxNoteBookPage1;
		wxNotebook *WxNotebook1;
    ////GUI Control Declaration End

    private:
    //Note: if you receive any error with these enum IDs, then you need to
    //change your old form code that are based on the #define control IDs.
    //#defines may replace a numeric value for the enum names.
    //Try copy and pasting the below block in your old form header files.
    enum
    {
        ////GUI Enum Control ID Start
			ID_MNU_DATABASE_1045 = 1045,
			ID_MNU_CHANGEPASSWORD_1046 = 1046,
			
			ID_MNU_QRCODE_1039 = 1039,
			ID_MNU_MENUITEM1_1021 = 1021,
			ID_MNU_ACCOUNTINFO_1081 = 1081,
			ID_MNU_ACCOUNTSETTINGS_1060 = 1060,
			ID_MNU_SETSIGNERLIST_1055 = 1055,
			ID_MNU_SUSPENDEDPAYMENT_1082 = 1082,
			ID_MNU_EXECUTE_1083 = 1083,
			ID_MNU_CANCEL_1084 = 1084,
			ID_MNU_COPYADDRESS_1027 = 1027,
			ID_MNU_COPYSECRET_1026 = 1026,
			ID_MNU_SETXRP_1057 = 1057,
			ID_MNU_SETCURRENTSEQUENCE_1058 = 1058,
			
			ID_MRPCSHOWQRCODE = 1080,
			ID_MOUTPUTFORMAT_WEBSOCKET = 1036,
			ID_MOUTPUTFORMAT_JSON = 1035,
			ID_WXRICHTEXTCTRL1 = 1034,
			ID_WXPANEL1 = 1030,
			ID_MTRANSACTIONSLIST = 1056,
			ID_WXNOTEBOOKPAGE6 = 1050,
			ID_MADDRESSBOOKADD = 1044,
			ID_MADDRESSBOOKLIST = 1043,
			ID_WXNOTEBOOKPAGE5 = 1042,
			ID_MBUTTONSIGNFOR = 1073,
			ID_WXSTATICTEXT8 = 1068,
			ID_WXSTATICTEXT7 = 1067,
			ID_MTXSIGNWITHWALLET = 1066,
			ID_MSIGNTRANSACTIONINPUT = 1063,
			ID_WXNOTEBOOKPAGE4 = 1062,
			ID_WXSTATICTEXT9 = 1089,
			ID_MLABELMULTISIGS = 1057,
			ID_MTXBTNSIGN = 1013,
			ID_MTXSUSPENDPAYMENT = 1048,
			ID_MTXSEQNUM = 1019,
			ID_MTXAMOUNTTYPE = 1012,
			ID_MTXAMOUNT = 1011,
			ID_MTXDESTINATIONTAG = 1090,
			ID_MTXDESTINATION = 1009,
			ID_MTXSOURCEWALLET = 1022,
			ID_WXSTATICTEXT6 = 1021,
			ID_WXSTATICTEXT5 = 1020,
			ID_WXSTATICTEXT2 = 1010,
			ID_WXSTATICTEXT1 = 1008,
			ID_WXNOTEBOOKPAGE3 = 1007,
			ID_MBUTTONLOADACCOUNTINFO = 1081,
			ID_WXSTATICTEXT3 = 1079,
			ID_MTXFEEDROPS = 1078,
			ID_WXSTATICTEXT4 = 1077,
			ID_MWALLETBTNIMPORT = 1025,
			ID_MWALLETBTNCREATE = 1006,
			ID_MWALLETS = 1005,
			ID_WXNOTEBOOKPAGE2 = 1004,
			ID_WXNOTEBOOKPAGE1 = 1003,
			ID_WXNOTEBOOK1 = 1002,
        ////GUI Enum Control ID End
        ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
    };

    private:
    void OnClose( wxCloseEvent& event );
    void CreateGUIControls();
    void SaveAndReload();
};

#endif
