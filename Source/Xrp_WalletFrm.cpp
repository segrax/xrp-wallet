///-----------------------------------------------------------------
///
/// @file      Xrp_WalletFrm.cpp
/// @author
/// Created:   4/01/2017 07:55:31
/// @section   DESCRIPTION
///            cXrp_WalletFrm class implementation
///
///------------------------------------------------------------------

#include "stdafx.hpp"
#include "Xrp_WalletFrm.h"
#include <wx/clipbrd.h>
#include "DialogQR.h"
#include "DialogSusPay.h"
#include "DialogSignList.h"
#include "DialogAccountSettings.h"

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cXrp_WalletFrm
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cXrp_WalletFrm,wxFrame)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cXrp_WalletFrm::OnClose)
	EVT_MENU(ID_MNU_CHANGEPASSWORD_1046, cXrp_WalletFrm::mMenuDatabase_ChangePassword)
	EVT_MENU(ID_MNU_QRCODE_1039 , cXrp_WalletFrm::mWalletListPopupMenu_ShowQR)
	EVT_MENU(ID_MNU_MENUITEM1_1021 , cXrp_WalletFrm::mWalletListPopupMenu_ShowSecret)
	EVT_MENU(ID_MNU_ACCOUNTINFO_1081 , cXrp_WalletFrm::mWalletListPopupMenu_AccountInfo)
	EVT_MENU(ID_MNU_ACCOUNTSETTINGS_1060 , cXrp_WalletFrm::mWalletListPopupMenu_AccountSettings)
	EVT_MENU(ID_MNU_SETSIGNERLIST_1055 , cXrp_WalletFrm::mWalletListPopupMenu_SetSignerList)
	EVT_MENU(ID_MNU_EXECUTE_1083 , cXrp_WalletFrm::mWalletListPopupMenu_SusPay_Execute)
	EVT_MENU(ID_MNU_CANCEL_1084 , cXrp_WalletFrm::mWalletListPopupMenu_SusPay_Cancel)
	EVT_MENU(ID_MNU_COPYADDRESS_1027 , cXrp_WalletFrm::mWalletListPopupMenu_CopyAddress)
	EVT_MENU(ID_MNU_COPYSECRET_1026 , cXrp_WalletFrm::mWalletListPopupMenu_CopySecret)
	EVT_MENU(ID_MNU_SETXRP_1057 , cXrp_WalletFrm::mWalletListPopupMenu_SetXRP)
	EVT_MENU(ID_MNU_SETCURRENTSEQUENCE_1058 , cXrp_WalletFrm::mWalletListPopupMenu_SetSequenceNumber)
	EVT_BUTTON(ID_MRPCSHOWQRCODE,cXrp_WalletFrm::mRPCShowQRCodeClick)
	EVT_RADIOBUTTON(ID_MOUTPUTFORMAT_WEBSOCKET,cXrp_WalletFrm::mOutputFormat_WebSocketClick)
	EVT_RADIOBUTTON(ID_MOUTPUTFORMAT_JSON,cXrp_WalletFrm::mOutputFormat_JsonClick)
	EVT_BUTTON(ID_MADDRESSBOOKADD,cXrp_WalletFrm::mAddressBookAddClick)
	
	EVT_LIST_END_LABEL_EDIT(ID_MADDRESSBOOKLIST,cXrp_WalletFrm::mAddressBookListEndLabelEdit)
	EVT_LIST_ITEM_ACTIVATED(ID_MADDRESSBOOKLIST,cXrp_WalletFrm::mAddressBookListItemActivated)
	EVT_BUTTON(ID_MBUTTONSIGNFOR,cXrp_WalletFrm::mButtonSignForClick)
	EVT_COMBOBOX(ID_MTXSIGNWITHWALLET,cXrp_WalletFrm::mTxSignWithWalletSelected)
	EVT_BUTTON(ID_MTXBTNSIGN,cXrp_WalletFrm::mTxBtnSignClick)
	
	EVT_TEXT(ID_MTXSEQNUM,cXrp_WalletFrm::mTxSeqNumUpdated)
	EVT_COMBOBOX(ID_MTXSOURCEWALLET,cXrp_WalletFrm::mTxSourceWalletSelected)
	EVT_BUTTON(ID_MBUTTONLOADACCOUNTINFO,cXrp_WalletFrm::mWalletListPopupMenu_LoadAccountInfo)
	
	EVT_TEXT(ID_MTXFEEDROPS,cXrp_WalletFrm::mTxFeeDropsUpdated)
	EVT_BUTTON(ID_MWALLETBTNIMPORT,cXrp_WalletFrm::mWalletBtnImportClick)
	EVT_BUTTON(ID_MWALLETBTNCREATE,cXrp_WalletFrm::mWalletBtnCreateClick)
	
	EVT_LIST_ITEM_ACTIVATED(ID_MWALLETS,cXrp_WalletFrm::mWalletsListItemActivated)
END_EVENT_TABLE()
////Event Table End

// Nasty
static ripple::Blob gTX_LastBlob;
static Json::Value  gCmd_Params;
static std::string  gCmd;

cXrp_WalletFrm::cXrp_WalletFrm( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxFrame( parent, id, title, position, size, style ) {
    CreateGUIControls();
}

cXrp_WalletFrm::~cXrp_WalletFrm() {
}

void cXrp_WalletFrm::CreateGUIControls() {
    //Do not add custom code between
    //GUI Items Creation Start and GUI Items Creation End
    //wxDev-C++ designer will remove them.
    //Add the custom code before or after the blocks
    ////GUI Items Creation Start

	WxNotebook1 = new wxNotebook(this, ID_WXNOTEBOOK1, wxPoint(2, 2), wxSize(620, 276), wxNB_DEFAULT);

	WxNoteBookPage1 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE1, wxPoint(4, 26), wxSize(612, 246));
	WxNotebook1->AddPage(WxNoteBookPage1, _("Overview"));

	WxNoteBookPage2 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE2, wxPoint(4, 26), wxSize(612, 246));
	WxNotebook1->AddPage(WxNoteBookPage2, _("Accounts"));

	mWalletsList = new wxListCtrl(WxNoteBookPage2, ID_MWALLETS, wxPoint(13, 16), wxSize(585, 166), wxLC_REPORT | wxLC_EDIT_LABELS, wxDefaultValidator, _("mWalletsList"));
	mWalletsList->InsertColumn(0, _("Created"), wxLIST_FORMAT_LEFT, 140);
	mWalletsList->InsertColumn(1, _("Address"), wxLIST_FORMAT_LEFT, 280);
	mWalletsList->InsertColumn(2, _("Seq."), wxLIST_FORMAT_LEFT, 50);
	mWalletsList->InsertColumn(3, _("XRP"), wxLIST_FORMAT_LEFT, 100);

	mWalletBtnCreate = new wxButton(WxNoteBookPage2, ID_MWALLETBTNCREATE, _("Create Account"), wxPoint(14, 200), wxSize(107, 32), 0, wxDefaultValidator, _("mWalletBtnCreate"));

	mWalletBtnImport = new wxButton(WxNoteBookPage2, ID_MWALLETBTNIMPORT, _("Import Account"), wxPoint(150, 200), wxSize(107, 32), 0, wxDefaultValidator, _("mWalletBtnImport"));

	WxStaticText4 = new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT4, _("Drops"), wxPoint(554, 212), wxDefaultSize, 0, _("WxStaticText4"));

	mTxFeeDrops = new wxTextCtrl(WxNoteBookPage2, ID_MTXFEEDROPS, _("20"), wxPoint(498, 208), wxSize(49, 23), 0, wxDefaultValidator, _("mTxFeeDrops"));

	WxStaticText3 = new wxStaticText(WxNoteBookPage2, ID_WXSTATICTEXT3, _("Current Fee"), wxPoint(426, 212), wxDefaultSize, 0, _("WxStaticText3"));

	mButtonLoadAccountInfo = new wxButton(WxNoteBookPage2, ID_MBUTTONLOADACCOUNTINFO, _("Parse Account Info"), wxPoint(286, 200), wxSize(131, 32), 0, wxDefaultValidator, _("mButtonLoadAccountInfo"));

	WxNoteBookPage3 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE3, wxPoint(4, 26), wxSize(612, 246));
	WxNotebook1->AddPage(WxNoteBookPage3, _("Send"));

	WxStaticText1 = new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT1, _("Pay To"), wxPoint(26, 53), wxDefaultSize, 0, _("WxStaticText1"));

	WxStaticText2 = new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT2, _("Amount"), wxPoint(26, 92), wxDefaultSize, 0, _("WxStaticText2"));

	WxStaticText5 = new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT5, _("Sequence Number"), wxPoint(26, 132), wxDefaultSize, 0, _("WxStaticText5"));

	WxStaticText6 = new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT6, _("From Account"), wxPoint(26, 16), wxDefaultSize, 0, _("WxStaticText6"));

	wxArrayString arrayStringFor_mTxSourceWallet;
	mTxSourceWallet = new wxComboBox(WxNoteBookPage3, ID_MTXSOURCEWALLET, _(""), wxPoint(138, 12), wxSize(281, 23), arrayStringFor_mTxSourceWallet, 0, wxDefaultValidator, _("mTxSourceWallet"));

	mTxDestination = new wxTextCtrl(WxNoteBookPage3, ID_MTXDESTINATION, _(""), wxPoint(138, 53), wxSize(283, 23), 0, wxDefaultValidator, _("mTxDestination"));

	mTxDestinationTag = new wxTextCtrl(WxNoteBookPage3, ID_MTXDESTINATIONTAG, _(""), wxPoint(466, 53), wxSize(113, 23), wxDOUBLE_BORDER, wxDefaultValidator, _("mTxDestinationTag"));

	mTxAmount = new wxTextCtrl(WxNoteBookPage3, ID_MTXAMOUNT, _("0"), wxPoint(138, 92), wxSize(113, 23), 0, wxDefaultValidator, _("mTxAmount"));

	wxArrayString arrayStringFor_mTxAmountType;
	arrayStringFor_mTxAmountType.Add(_("XRP"));
	mTxAmountType = new wxComboBox(WxNoteBookPage3, ID_MTXAMOUNTTYPE, _("XRP"), wxPoint(258, 92), wxSize(73, 23), arrayStringFor_mTxAmountType, 0, wxDefaultValidator, _("mTxAmountType"));

	mTxSeqNum = new wxTextCtrl(WxNoteBookPage3, ID_MTXSEQNUM, _(""), wxPoint(138, 132), wxSize(113, 23), 0, wxDefaultValidator, _("mTxSeqNum"));

	mTxSuspendPayment = new wxCheckBox(WxNoteBookPage3, ID_MTXSUSPENDPAYMENT, _("Escrow Payment  "), wxPoint(10, 182), wxSize(145, 17), wxALIGN_RIGHT, wxDefaultValidator, _("mTxSuspendPayment"));

	mTxBtnSign = new wxButton(WxNoteBookPage3, ID_MTXBTNSIGN, _("Sign"), wxPoint(482, 188), wxSize(105, 25), 0, wxDefaultValidator, _("mTxBtnSign"));

	mLabelMultiSigsWarning = new wxStaticText(WxNoteBookPage3, ID_MLABELMULTISIGS, _("Requires Multiple Signatures"), wxPoint(346, 132), wxDefaultSize, 0, _("mLabelMultiSigsWarning"));
	mLabelMultiSigsWarning->Show(false);
	mLabelMultiSigsWarning->SetForegroundColour(wxColour(255,0,0));
	mLabelMultiSigsWarning->SetFont(wxFont(14, wxSWISS, wxNORMAL, wxNORMAL, false, _("Segoe UI Symbol")));

	WxStaticText9 = new wxStaticText(WxNoteBookPage3, ID_WXSTATICTEXT9, _("Destination Tag"), wxPoint(466, 24), wxDefaultSize, 0, _("WxStaticText9"));

	WxNoteBookPage4 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE4, wxPoint(4, 26), wxSize(612, 246));
	WxNotebook1->AddPage(WxNoteBookPage4, _("Sign Transaction"));

	mSignForTransaction = new wxRichTextCtrl(WxNoteBookPage4, ID_MSIGNTRANSACTIONINPUT, _(""), wxPoint(26, 76), wxSize(433, 153), 0, wxDefaultValidator, _("mSignForTransaction"));
	mSignForTransaction->SetMaxLength(0);
	mSignForTransaction->SetFocus();
	mSignForTransaction->SetInsertionPointEnd();

	wxArrayString arrayStringFor_mTxSignWithWallet;
	mTxSignWithWallet = new wxComboBox(WxNoteBookPage4, ID_MTXSIGNWITHWALLET, _(""), wxPoint(138, 12), wxSize(281, 23), arrayStringFor_mTxSignWithWallet, 0, wxDefaultValidator, _("mTxSignWithWallet"));

	WxStaticText7 = new wxStaticText(WxNoteBookPage4, ID_WXSTATICTEXT7, _("With Wallet"), wxPoint(26, 16), wxDefaultSize, 0, _("WxStaticText7"));

	WxStaticText8 = new wxStaticText(WxNoteBookPage4, ID_WXSTATICTEXT8, _("Transaction JSON"), wxPoint(26, 52), wxDefaultSize, 0, _("WxStaticText8"));

	mButtonSignFor = new wxButton(WxNoteBookPage4, ID_MBUTTONSIGNFOR, _("Sign"), wxPoint(482, 204), wxSize(105, 25), 0, wxDefaultValidator, _("mButtonSignFor"));

	WxNoteBookPage5 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE5, wxPoint(4, 26), wxSize(612, 246));
	WxNotebook1->AddPage(WxNoteBookPage5, _("Address Book"));

	mAddressBookList = new wxListCtrl(WxNoteBookPage5, ID_MADDRESSBOOKLIST, wxPoint(18, 20), wxSize(569, 177), wxLC_REPORT | wxLC_EDIT_LABELS, wxDefaultValidator, _("mAddressBookList"));
	mAddressBookList->InsertColumn(0, _("Name"), wxLIST_FORMAT_LEFT, 250);
	mAddressBookList->InsertColumn(1, _("Address"), wxLIST_FORMAT_LEFT, 300);

	mAddressBookAdd = new wxButton(WxNoteBookPage5, ID_MADDRESSBOOKADD, _("Add Address"), wxPoint(498, 204), wxSize(89, 33), 0, wxDefaultValidator, _("mAddressBookAdd"));

	WxNoteBookPage6 = new wxPanel(WxNotebook1, ID_WXNOTEBOOKPAGE6, wxPoint(4, 26), wxSize(612, 246));
	WxNotebook1->AddPage(WxNoteBookPage6, _("History"));

	mTransactionsList = new wxListCtrl(WxNoteBookPage6, ID_MTRANSACTIONSLIST, wxPoint(10, 12), wxSize(585, 225), wxLC_REPORT, wxDefaultValidator, _("mTransactionsList"));
	mTransactionsList->InsertColumn(0, _("Date"), wxLIST_FORMAT_LEFT, 150);
	mTransactionsList->InsertColumn(1, _("Type"), wxLIST_FORMAT_LEFT, 80);
	mTransactionsList->InsertColumn(2, _("To"), wxLIST_FORMAT_LEFT, 260);
	mTransactionsList->InsertColumn(3, _("Amount"), wxLIST_FORMAT_LEFT, 90);

	WxPanel1 = new wxPanel(this, ID_WXPANEL1, wxPoint(8, 288), wxSize(608, 153));

	WxRichTextCtrl1 = new wxRichTextCtrl(WxPanel1, ID_WXRICHTEXTCTRL1, _(""), wxPoint(10, 6), wxSize(433, 129), wxRE_READONLY | wxRE_MULTILINE, wxDefaultValidator, _("WxRichTextCtrl1"));
	WxRichTextCtrl1->SetMaxLength(0);
	WxRichTextCtrl1->SetFocus();
	WxRichTextCtrl1->SetInsertionPointEnd();

	mOutputFormat_Json = new wxRadioButton(WxPanel1, ID_MOUTPUTFORMAT_JSON, _("JSON-RPC"), wxPoint(482, 28), wxSize(81, 17), 0, wxDefaultValidator, _("mOutputFormat_Json"));

	mOutputFormat_WebSocket = new wxRadioButton(WxPanel1, ID_MOUTPUTFORMAT_WEBSOCKET, _("WS-RPC"), wxPoint(482, 54), wxSize(81, 17), 0, wxDefaultValidator, _("mOutputFormat_WebSocket"));
	mOutputFormat_WebSocket->SetValue(true);

	mRPCShowQRCode = new wxButton(WxPanel1, ID_MRPCSHOWQRCODE, _("Show as QR Code"), wxPoint(472, 112), wxSize(113, 25), 0, wxDefaultValidator, _("mRPCShowQRCode"));

	mWalletListPopupMenu = new wxMenu(_(""));
	mWalletListPopupMenu->Append(ID_MNU_QRCODE_1039, _("Show QR Code"), _(""), wxITEM_NORMAL);
	mWalletListPopupMenu->Append(ID_MNU_MENUITEM1_1021, _("Show Secret"), _(""), wxITEM_NORMAL);
	mWalletListPopupMenu->AppendSeparator();
	mWalletListPopupMenu->Append(ID_MNU_ACCOUNTINFO_1081, _("Account Info"), _(""), wxITEM_NORMAL);
	mWalletListPopupMenu->Append(ID_MNU_ACCOUNTSETTINGS_1060, _("Account Settings"), _(""), wxITEM_NORMAL);
	mWalletListPopupMenu->Append(ID_MNU_SETSIGNERLIST_1055, _("Set Signer List"), _(""), wxITEM_NORMAL);
	mWalletListPopupMenu->AppendSeparator();
	wxMenu *ID_MNU_SUSPENDEDPAYMENT_1082_Obj = new wxMenu();
	ID_MNU_SUSPENDEDPAYMENT_1082_Obj->Append(ID_MNU_EXECUTE_1083, _("Execute"), _(""), wxITEM_NORMAL);
	ID_MNU_SUSPENDEDPAYMENT_1082_Obj->Append(ID_MNU_CANCEL_1084, _("Cancel"), _(""), wxITEM_NORMAL);
	mWalletListPopupMenu->Append(ID_MNU_SUSPENDEDPAYMENT_1082, _("Escrow Payment"), ID_MNU_SUSPENDEDPAYMENT_1082_Obj);
	mWalletListPopupMenu->Append(ID_MNU_COPYADDRESS_1027, _("Copy Address"), _(""), wxITEM_NORMAL);
	mWalletListPopupMenu->Append(ID_MNU_COPYSECRET_1026, _("Copy Secret"), _(""), wxITEM_NORMAL);
	mWalletListPopupMenu->AppendSeparator();
	mWalletListPopupMenu->Append(ID_MNU_SETXRP_1057, _("Set XRP"), _(""), wxITEM_NORMAL);
	mWalletListPopupMenu->Append(ID_MNU_SETCURRENTSEQUENCE_1058, _("Set Current Sequence"), _(""), wxITEM_NORMAL);

	WxMenuBar1 = new wxMenuBar();
	wxMenu *ID_MNU_DATABASE_1045_Mnu_Obj = new wxMenu();
	ID_MNU_DATABASE_1045_Mnu_Obj->Append(ID_MNU_CHANGEPASSWORD_1046, _("Change Password"), _(""), wxITEM_NORMAL);
	WxMenuBar1->Append(ID_MNU_DATABASE_1045_Mnu_Obj, _("&Database"));
	SetMenuBar(WxMenuBar1);

	SetTitle(_("XRP Wallet"));
	SetIcon(wxNullIcon);
	SetSize(8,8,641,490);
	Center();
	
    ////GUI Items Creation End

    WxNotebook1->SetPosition( wxPoint( 12, 10 ) );
    WxNotebook1->SetSize( GetSize().GetWidth() - 40, GetSize().GetHeight() - 224 );

    SetMinSize( wxSize( 640, 480 ) );

    // Prepare the wallet
    std::string Password = wxGetPasswordFromUser( "Enter password", "Open Wallet" );
    new cAccountManager( "wallet.dat", Password );

    if (g_WalletManager.Load() == false) {
        wxMessageBox( "Invalid Password", "Wallet" );
        exit( 1 );
    }

    ReloadWallets();
    ReloadAddressBook();
}

void cXrp_WalletFrm::OnClose( wxCloseEvent& pEvent ) {

    Destroy();
}

/*
 * mWalletBtnCreateClick
 */
void cXrp_WalletFrm::mWalletBtnCreateClick( wxCommandEvent& event ) {
	cAccount* Wallet = 0;
	
#if 0
	cAccount* WalletSeek = 0;
		std::string StartsWith = wxGetTextFromUser( "Starts With", "Generate Account", "r" );
		StartsWith.resize( 3 );
		while (!WalletSeek) {
			WalletSeek = new cAccount( "", 1 );

			if (WalletSeek->getAddress().find( StartsWith ) == 0)
				break;

			delete WalletSeek;
			WalletSeek = 0;
		}

		Wallet = g_WalletManager.AccountImport( WalletSeek->getSecret(), 1 );
		delete WalletSeek;
#else
	 Wallet = g_WalletManager.AccountGenerate();
#endif
    if (!Wallet)
        return;

    wxGetTextFromUser( "Generated Secret", Wallet->getAddress(), Wallet->getSecret() );

    ReloadWallets();
}

/*
 * mTxBtnSignClick
 */
void cXrp_WalletFrm::mTxBtnSignClick( wxCommandEvent& event ) {
    WxRichTextCtrl1->Clear();

    cAccount* Wallet = g_WalletManager.AccountGetByAddress( mTxSourceWallet->GetValue().ToStdString() );

    if (!Wallet)
        return;
    
	auto DestinationTag = mTxDestinationTag->GetValue().ToStdString();

    const std::string Destination = mTxDestination->GetValue().ToStdString();
    const double AmountXRP = std::stod( mTxAmount->GetValue().ToStdString() );
    const uint64_t Sequence = std::stoi( mTxSeqNum->GetValue().ToStdString() );
	uint64_t DestTag = 0;
	
	if (DestinationTag.size()) {
		try {
			DestTag = std::stoi( DestinationTag );
		} catch (std::exception &p) {
			wxMessageBox( "Invalid Destination Tag", "Sign Transaction Failed" );
			DestTag = 0;
			return;
		}
	}

	uint64_t AmountDrops = 0;

    if (!Destination.size()) {
        wxMessageBox( "Please enter a destination", "Sign Transaction Failed" );
        return;
    }

    if (AmountXRP == 0.0) {
        wxMessageBox( "Amount must be positive", "Sign Transaction Failed" );
        return;
    }

    // If XRP mode, convert to drops
    if (mTxAmountType->GetValue() == "XRP")
        AmountDrops = AmountXRP * 1000000;
    else {
        wxMessageBox( "Invalid Amount Type", "Sign Transaction Failed" );
        return;
    }

    // Ensure we have enough currency, but only if we have some... otherwise its upto the user
    if (Wallet->getCurrency() > 0) {
        if (Wallet->getCurrency() - (AmountXRP + Wallet->calculateFeeXrp()) < 0) {
            wxMessageBox( "Not enough XRP", "Sign Transaction Failed" );
            return;
        }
    }

    // Sequence Number provided?
    if (!mTxSeqNum->GetValue().empty())
        Wallet->setSequence( Sequence );

    std::shared_ptr<ripple::STTx> Tx;

    // Doing a suspended payment?
    if (mTxSuspendPayment->IsChecked()) {
        cDialog_SusPay DialogSus( this, Wallet );
        DialogSus.ShowModal();

        if (DialogSus.mWallet)
            return;

        std::string ProofText = DialogSus.GetProofText();
        uint64_t ExecuteTime = DialogSus.GetExecuteAfter();
        uint64_t CancelTime = DialogSus.GetCancelledAfter();

        // Ensure we have atleast one executable state
        if (CancelTime || ExecuteTime || ProofText.size()) {
            Tx = std::make_shared<ripple::STTx>( std::move( Wallet->CreateEscrow( Destination, AmountDrops, CancelTime, ExecuteTime, DestTag, ProofText ) ) );
        } else {
            wxMessageBox( "No suspended payment options provided" );
            return;
        }

    } else {
        // Standard Payment
        Tx = std::make_shared<ripple::STTx>( std::move( Wallet->CreatePayment( Destination, AmountDrops, DestTag ) ) );
    }

    gTX_LastBlob = Tx->getSerializer().getData();

    // Update the sequence on the GUI
    mTxSeqNum->SetValue( std::to_string( Wallet->getSequence() ) );

    // If the signing wallet is multi-sig, output the TX JSON instead of the RPC JSON
    if (Wallet->getSignerQuorum()) {
        WxRichTextCtrl1->Clear();
        WxRichTextCtrl1->AppendText( Tx->getJson( 0 ).toStyledString() );
    }
    else {
        ReloadTransactionOutput();
    }

    SaveAndReload();
}

/*
 * mWalletsListItemActivated
 */
void cXrp_WalletFrm::mWalletsListItemActivated( wxListEvent& pEvent ) {
    cAccount* Wallet = reinterpret_cast<cAccount*>(pEvent.GetData());

    mWalletListPopupMenu->SetClientData( (void*)Wallet );

    PopupMenu( mWalletListPopupMenu );
}

/*
 * mWalletListPopupMenu_ShowSecret
 */
void cXrp_WalletFrm::mWalletListPopupMenu_ShowSecret( wxCommandEvent& event ) {
    const cAccount* Wallet = reinterpret_cast<cAccount*>(mWalletListPopupMenu->GetClientData());

    if (Wallet) {
        std::string Secret = Wallet->getSecret();
        std::string Address = Wallet->getAddress();

        wxGetTextFromUser( "Secret", Address, Secret );
    }
}

/*
* mWalletListPopupMenu_SetXRP
*/
void cXrp_WalletFrm::mWalletListPopupMenu_SetXRP( wxCommandEvent& event ) {
    cAccount* Wallet = reinterpret_cast<cAccount*>(mWalletListPopupMenu->GetClientData());

    std::string XRP = wxGetTextFromUser( "XRP", "Set XRP" );

    if (XRP.empty())
        return;

    Wallet->setCurrency( std::stod( XRP ) );
    SaveAndReload();
}

/*
* mWalletListPopupMenu_CopySecret
*/
void cXrp_WalletFrm::mWalletListPopupMenu_CopySecret( wxCommandEvent& event ) {
    const cAccount* Wallet = reinterpret_cast<cAccount*>(mWalletListPopupMenu->GetClientData());

    wxTheClipboard->Clear();
    wxTheClipboard->SetData( new wxTextDataObject( Wallet->getSecret() ) );
    wxTheClipboard->Flush();
    wxTheClipboard->Close();
}

/*
* mWalletListPopupMenu_CopyAddress
*/
void cXrp_WalletFrm::mWalletListPopupMenu_CopyAddress( wxCommandEvent& event ) {
    const cAccount* Wallet = reinterpret_cast<cAccount*>(mWalletListPopupMenu->GetClientData());

    wxTheClipboard->Clear();
    wxTheClipboard->SetData( new wxTextDataObject( Wallet->getAddress() ) );
    wxTheClipboard->Flush();
    wxTheClipboard->Close();
}

/*
* mWalletListPopupMenu_ShowQR
*/
void cXrp_WalletFrm::mWalletListPopupMenu_ShowQR( wxCommandEvent& event ) {
    const cAccount* Wallet = reinterpret_cast<cAccount*>(mWalletListPopupMenu->GetClientData());

    cDialogQR QrCode( this, Wallet->getAddress() );
    QrCode.ShowModal();
}

/*
 * mTxSourceWalletSelected
 */
void cXrp_WalletFrm::mTxSourceWalletSelected( wxCommandEvent& pEvent ) {
    const cAccount* Wallet = g_WalletManager.AccountGetByAddress( pEvent.GetString().ToStdString() );

    mTxSeqNum->SetValue( "" );
    mLabelMultiSigsWarning->Hide();

    if (Wallet) {
        mTxSeqNum->SetValue( std::to_string( Wallet->getSequence() ) );

        // If multisign, show the 'Requires' message
        if(Wallet->getSignerQuorum())
            mLabelMultiSigsWarning->Show();
    }
    else {
        mLabelMultiSigsWarning->Hide();
    }
}

/*
* mTxSignWithWalletSelected
*/
void cXrp_WalletFrm::mTxSignWithWalletSelected( wxCommandEvent& pEvent ) {
    //const cWallet* Wallet = g_WalletManager.WalletGetByAddress( pEvent.GetString().ToStdString() );

    // Nothing to do?
}

/*
 * mWalletBtnImportClick
 */
void cXrp_WalletFrm::mWalletBtnImportClick( wxCommandEvent& event ) {
    std::string Seed = wxGetTextFromUser( "Enter secret", "Import Wallet" );

    g_WalletManager.AccountImport( Seed, 0 );
    ReloadWallets();
}

/*
 * mOutputFormat_JsonClick
 */
void cXrp_WalletFrm::mOutputFormat_JsonClick( wxCommandEvent& event ) {

    if (gTX_LastBlob.size())
        ReloadTransactionOutput();
    else
        ReloadCommandOutput();
}

/*
 * mOutputFormat_WebSocketClick
 */
void cXrp_WalletFrm::mOutputFormat_WebSocketClick( wxCommandEvent& event ) {
    if (gTX_LastBlob.size())
        ReloadTransactionOutput();
    else
        ReloadCommandOutput();
}

/*
* mAddressBookAddClick
*/
void cXrp_WalletFrm::mAddressBookAddClick( wxCommandEvent& event ) {
    const std::string Address = wxGetTextFromUser( "Address", "AddressBook", "" );

    g_WalletManager.AddressBookAdd( "", Address );

    ReloadAddressBook();
}

/*
* mMenuDatabase_ChangePassword
*/
void cXrp_WalletFrm::mMenuDatabase_ChangePassword( wxCommandEvent& event ) {
    std::string Password = wxGetPasswordFromUser( "Enter your current password", "Change Wallet Password" );
    g_WalletManager.SetDatabasePassword( Password );

    if (g_WalletManager.Load() == false) {
        exit( 1 );
    }

    // Get the new password
    Password = wxGetPasswordFromUser( "Enter new password", "Change Wallet Password" );
    const std::string VerifyPassword = wxGetPasswordFromUser( "Please verify new password", "Change Wallet Password" );

    // Verify the new password
    if (Password != VerifyPassword) {
        wxMessageBox( "Password did not match!" );
        return;
    }

    // Update the database password
    g_WalletManager.SetDatabasePassword( Password );

    // Save using the new password
    g_WalletManager.Save();
}

/**
 * Reload GUI Wallet Lists
 */
void cXrp_WalletFrm::ReloadWallets() {
    std::string CurrentWallet = mTxSourceWallet->GetValue().ToStdString();

    mTxFeeDrops->SetValue( std::to_string( g_WalletManager.GetCurrentFee() ) );
    mTxSeqNum->SetValue( "" );
    mTxSourceWallet->Clear();
    mTxSignWithWallet->Clear();
    mWalletsList->DeleteAllItems();
    mLabelMultiSigsWarning->Hide();

    // Loop each wallet
    for (auto &Wallet : g_WalletManager.mAccounts) {
        // Creation Time string
        time_t Created = (time_t)Wallet.getCreated();
        std::string CreatedOn = std::asctime( std::localtime( &Created ) );

        // Add the wallet to the list
        int i = mWalletsList->InsertItem( mWalletsList->GetItemCount(), CreatedOn );
        mWalletsList->SetItem( i, 1, wxString( Wallet.getAddress() ) );
        mWalletsList->SetItem( i, 2, wxString::Format( wxT( "%u" ), (uint32_t)Wallet.getSequence() ) );
        mWalletsList->SetItem( i, 3, wxString::Format( wxT( "%f" ), (float)Wallet.getCurrency( "XRP" ) ) );
        mWalletsList->SetItemPtrData( i, (wxUIntPtr)&Wallet );

        // Add the address to the 'send' source drop down
        mTxSourceWallet->AppendString( Wallet.getAddress() );
        mTxSignWithWallet->AppendString( Wallet.getAddress() );

        // Set the sequence number of the selected wallet
        if (CurrentWallet == Wallet.getAddress() || !CurrentWallet.size()) {
            CurrentWallet = Wallet.getAddress();
            mTxSeqNum->SetValue( std::to_string( Wallet.getSequence() ) );

            if(Wallet.getSignerQuorum())
                mLabelMultiSigsWarning->Show();
        }
    }

    // Set the source address back to the previously selected one
    mTxSourceWallet->SetStringSelection( CurrentWallet );

    // Reload all transactions
    ReloadTransactions();
}

/**
 * Reload GUI Transactions List
 */
void cXrp_WalletFrm::ReloadTransactions() {
    mTransactionsList->DeleteAllItems();
    std::list<sTransaction> List;

    for (const auto &Wallet : g_WalletManager.mAccounts) {
        for (const auto &Transaction : Wallet.getTransactions()) {
            List.push_back( Transaction );
        }
    }
    List.sort(
        []( const sTransaction& a, const sTransaction& b ) { return a.mTimestamp > b.mTimestamp; } );

    for (const auto &Transaction : List) {
        time_t Created = (time_t)Transaction.mTimestamp;
        std::string CreatedOn = std::asctime( std::localtime( &Created ) );

        int i = mTransactionsList->InsertItem( mTransactionsList->GetItemCount(), CreatedOn );
        mTransactionsList->SetItem( i, 1, wxString( Transaction.mType ) );
        mTransactionsList->SetItem( i, 2, wxString( Transaction.mDestination ) );
        mTransactionsList->SetItem( i, 3, wxString( Transaction.mAmount ) );
    }

}

/**
 * Reload the transaction output box
 */
void cXrp_WalletFrm::ReloadTransactionOutput() {
    if (gTX_LastBlob.empty())
        return;

    Json::Value FinalJsonObj;
    if (mOutputFormat_Json->GetValue() == true) {
        // Json RPC
        FinalJsonObj["method"] = "submit";
        FinalJsonObj["params"]["tx_blob"] = ripple::strHex( gTX_LastBlob );
    }
    else {
        // WebSocket RPC
        FinalJsonObj["command"] = "submit";
        FinalJsonObj["tx_blob"] = ripple::strHex( gTX_LastBlob );
    }

    WxRichTextCtrl1->Clear();
    WxRichTextCtrl1->AppendText( FinalJsonObj.toStyledString() );
}

void cXrp_WalletFrm::ReloadCommandOutput() {
    if (gCmd.empty())
        return;

    Json::Value FinalJsonObj;
    // Json RPC?
    if (mOutputFormat_Json->GetValue() == true) {
        FinalJsonObj["method"] = gCmd;
        FinalJsonObj["params"] = gCmd_Params;
    } else {
        // WebSocket RPC
        FinalJsonObj = gCmd_Params;
        FinalJsonObj["command"] = gCmd;
    }

    WxRichTextCtrl1->Clear();
    WxRichTextCtrl1->AppendText( FinalJsonObj.toStyledString() );
}

/**
 *
 */
void cXrp_WalletFrm::ReloadAddressBook() {
    mAddressBookList->DeleteAllItems();

    for (const auto &AddressBookEntry : g_WalletManager.mAddressBook) {
        int i = mAddressBookList->InsertItem( mAddressBookList->GetItemCount(), AddressBookEntry.mName );

        mAddressBookList->SetItem( i, 1, AddressBookEntry.mAddress );
        mAddressBookList->SetItemPtrData( i, (wxUIntPtr)&AddressBookEntry );
    }
}

/*
 * mAddressBookListEndLabelEdit
 */
void cXrp_WalletFrm::mAddressBookListEndLabelEdit( wxListEvent& pEvent ) {
    cAddressBookEntry* AddressBookEntry = reinterpret_cast<cAddressBookEntry*>(pEvent.GetData());

    AddressBookEntry->mName = pEvent.GetText();

    g_WalletManager.Save();
}

/*
 * mWalletListPopupMenu_SetSignerList
 */
void cXrp_WalletFrm::mWalletListPopupMenu_SetSignerList( wxCommandEvent& event ) {
    cAccount* Wallet = reinterpret_cast<cAccount*>(mWalletListPopupMenu->GetClientData());

    uint32_t StartQuorum = 0;

    cDialogSignList SignList( this, Wallet );
    SignList.ShowModal();

    if (SignList.mWallet)
        return;

    const auto Quorum = std::stoi( SignList.mSignerQuorum->GetValue().ToStdString() );

    // Create the transaction
    const ripple::STTx Tx = Wallet->CreateSignerListSet( SignList.mSigners, Quorum );

    // If the transaction is signed, output the blob
    if (Tx.checkSign( true ).first) {
        gTX_LastBlob = Tx.getSerializer().getData();
        ReloadTransactionOutput();
    }
    else {
        WxRichTextCtrl1->Clear();
        WxRichTextCtrl1->AppendText( Tx.getJson( 0 ).toStyledString() );
    }
    SaveAndReload();
}

/*
 * mWalletListPopupMenu_SetSequenceNumber
 */
void cXrp_WalletFrm::mWalletListPopupMenu_SetSequenceNumber( wxCommandEvent& event ) {
    cAccount* Wallet = reinterpret_cast<cAccount*>(mWalletListPopupMenu->GetClientData());

    std::string Sequence = wxGetTextFromUser( "Sequence Number", "Enter the current sequence" );
    const uint64_t SequenceNo = std::stoi( Sequence );
    if (SequenceNo == 0)
        return;

    Wallet->setSequence( SequenceNo );
    SaveAndReload();
}

void cXrp_WalletFrm::SaveAndReload() {
    
    g_WalletManager.Save();
    ReloadWallets();
    ReloadTransactions();
}

/*
 * mWalletListPopupMenu_AccountSettings
 */
void cXrp_WalletFrm::mWalletListPopupMenu_AccountSettings( wxCommandEvent& event ) {
    cAccount* Wallet = reinterpret_cast<cAccount*>(mWalletListPopupMenu->GetClientData());

    if (!Wallet)
        return;

    bool isMasterDisabled = Wallet->getFlagIsSet( ripple::asfDisableMaster );

    cDialogAccountSettings DialogAccountSettings( this, Wallet );
    DialogAccountSettings.ShowModal();

    if (DialogAccountSettings.mWallet)
        return;

    const ripple::STTx Tx = Wallet->CreateAccountSet();

    if (isMasterDisabled) {
        WxRichTextCtrl1->Clear();
        WxRichTextCtrl1->AppendText( Tx.getJson( 0 ).toStyledString() );
    }
    else {
        gTX_LastBlob = Tx.getSerializer().getData();
        ReloadTransactionOutput();
    }

    SaveAndReload();
}

/*
 * mButtonSignForClick
 */
void cXrp_WalletFrm::mButtonSignForClick( wxCommandEvent& pEvent ) {
    cAccount* Wallet = g_WalletManager.AccountGetByAddress( mTxSignWithWallet->GetValue().ToStdString() );

    if (!Wallet)
        return;

    Json::Value inTx;

    // If no transaction is provided, load the output field and finalise it
    if (mSignForTransaction->GetValue().size() == 0) {
        if (Json::Reader().parse( WxRichTextCtrl1->GetValue().ToStdString(), inTx )) {
            mSignForTransaction->SetValue( WxRichTextCtrl1->GetValue() );

            ripple::STParsedJSONObject parsed( std::string( ripple::jss::tx_json ), inTx );

            std::shared_ptr<ripple::STTx> Tx = std::make_shared<ripple::STTx>( std::move( parsed.object.get() ) );;

            gTX_LastBlob = Tx->getSerializer().getData();
            ReloadTransactionOutput();
        }

        return;
    }

    // Attempt to sign the transaction in the TextBox
    if (Json::Reader().parse( mSignForTransaction->GetValue().ToStdString(), inTx )) {
        ripple::STParsedJSONObject parsed( std::string( ripple::jss::tx_json ), inTx );

        std::shared_ptr<ripple::STTx> Tx = std::make_shared<ripple::STTx>( std::move( parsed.object.get() ) );;

        if (Wallet->SignTx( *Tx )) {
            WxRichTextCtrl1->Clear();
            WxRichTextCtrl1->AppendText( Tx->getJson( 0 ).toStyledString() );
        } else {
            wxMessageBox( "Signature validation failed" );
        }

        mSignForTransaction->Clear();
    }
}

/*
 * mTxFeeDropsUpdated
 */
void cXrp_WalletFrm::mTxFeeDropsUpdated(wxCommandEvent& event) {

    const uint64_t FeeDrops = std::stoi( mTxFeeDrops->GetValue().ToStdString() );
    if (FeeDrops < 15) {
        return;
    }

    g_WalletManager.SetCurrentFee( FeeDrops );
    g_WalletManager.Save();
}

/*
 * mRPCShowQRCodeClick
 */
void cXrp_WalletFrm::mRPCShowQRCodeClick(wxCommandEvent& pEvent) {

    cDialogQR QrCode( this, WxRichTextCtrl1->GetValue().ToStdString() );
    QrCode.ShowModal();
}

/*
 * mAddressBookListItemActivated
 */
void cXrp_WalletFrm::mAddressBookListItemActivated(wxListEvent& pEvent) {
    cAddressBookEntry* AddressBookEntry = reinterpret_cast<cAddressBookEntry*>(pEvent.GetData());

    mTxDestination->SetValue( AddressBookEntry->mAddress );
    mTxAmount->SetFocus();
    mTxAmount->SelectAll();

    WxNotebook1->ChangeSelection( 2 );
}


/*
 * mWalletListPopupMenu_AccountInfo
 */
void cXrp_WalletFrm::mWalletListPopupMenu_AccountInfo(wxCommandEvent& event) {
    cAccount* Wallet = reinterpret_cast<cAccount*>(mWalletListPopupMenu->GetClientData());

    gCmd_Params.clear();

    gCmd = "account_info";
    gCmd_Params["account"] = Wallet->getAddress();
    gCmd_Params["signer_lists"] = "true";
    gTX_LastBlob.clear();

    ReloadCommandOutput();
}

/*
 * mWalletListPopupMenu_LoadAccountInfo
 */
void cXrp_WalletFrm::mWalletListPopupMenu_LoadAccountInfo(wxCommandEvent& pEvent) {

    wxString str;

    wxTextEntryDialog dialog( this, "Paste Response", "Account Info", "", wxTE_MULTILINE | wxTextEntryDialogStyle, wxPoint( wxDefaultCoord, wxDefaultCoord )  );

    if (dialog.ShowModal() == wxID_OK)
        str = dialog.GetValue();
    else
        return;

    Json::Value inJson;
    if (Json::Reader().parse( str.ToStdString(), inJson ) == false)
        return;

    if (inJson["result"]["account_data"].size()) {

        if( g_WalletManager.AccountDataProcess( inJson["result"]["account_data"] ) )
            SaveAndReload();
    }

}

/*
 * mWalletListPopupMenu_SusPay_Execute
 */
void cXrp_WalletFrm::mWalletListPopupMenu_SusPay_Execute(wxCommandEvent& event) {
    cAccount* Wallet = reinterpret_cast<cAccount*>(mWalletListPopupMenu->GetClientData());
    
    std::string Owner = wxGetTextFromUser( "Owner Address", "Execute Suspended Payment", "" );
    std::string SeqNo = wxGetTextFromUser( "Payment Sequence Number", "Execute Suspended Payment", "0" );
    std::string Phrase = wxGetTextFromUser( "Phrase", "Execute Suspended Payment", "" );

	if (SeqNo.empty())
		return;

    uint64_t SequenceNumber = std::stoul( SeqNo );
    if (!SequenceNumber)
        return;

    auto const OwnerAccount = ripple::parseBase58<ripple::AccountID>( Owner );
    if (!OwnerAccount) {
        wxMessageBox( "Invalid Owner Account" );
        return;
    }

    ripple::STTx Tx = Wallet->CreateEscrowFinish( Owner, SequenceNumber, Phrase );
    gTX_LastBlob = Tx.getSerializer().getData();

    // If the signing wallet is multi-sig, output the TX JSON instead of the RPC JSON
    if (Wallet->getSignerQuorum()) {
        WxRichTextCtrl1->Clear();
        WxRichTextCtrl1->AppendText( Tx.getJson( 0 ).toStyledString() );
    } else {
        ReloadTransactionOutput();
    }

    return;
}

/*
 * mWalletListPopupMenu_SusPay_Cancel
 */
void cXrp_WalletFrm::mWalletListPopupMenu_SusPay_Cancel(wxCommandEvent& event) {
    cAccount* Wallet = reinterpret_cast<cAccount*>(mWalletListPopupMenu->GetClientData());
    
    std::string Owner = wxGetTextFromUser( "Owner Address", "Execute Suspended Payment", Wallet->getAddress() );
    std::string SeqNo = wxGetTextFromUser( "Payment Sequence Number", "Execute Suspended Payment", "0" );

    uint64_t SequenceNumber = std::stoul( SeqNo );
    if (!SequenceNumber)
        return;

    auto const OwnerAccount = ripple::parseBase58<ripple::AccountID>( Owner );
    if (!OwnerAccount) {
        wxMessageBox( "Invalid Owner Account" );
        return;
    }

    ripple::STTx Tx = Wallet->CreateEscrowCancel( Owner, SequenceNumber );
    gTX_LastBlob = Tx.getSerializer().getData();

    // If the signing wallet is multi-sig, output the TX JSON instead of the RPC JSON
    if (Wallet->getSignerQuorum()) {
        WxRichTextCtrl1->Clear();
        WxRichTextCtrl1->AppendText( Tx.getJson( 0 ).toStyledString() );
    } else {
        ReloadTransactionOutput();
    }

    return;
}

/*
 * mTxSeqNumUpdated
 */
void cXrp_WalletFrm::mTxSeqNumUpdated(wxCommandEvent& event)
{
	// insert your code here
}
