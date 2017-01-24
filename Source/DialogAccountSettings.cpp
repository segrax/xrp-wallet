///-----------------------------------------------------------------
///
/// @file      DialogAccountSettings.cpp
/// @author
/// Created:   14/01/2017 12:02:12
/// @section   DESCRIPTION
///            cDialogAccountSettings class implementation
///
///------------------------------------------------------------------

#include "stdafx.hpp"
#include "DialogAccountSettings.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cDialogAccountSettings
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialogAccountSettings,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialogAccountSettings::OnClose)
	EVT_BUTTON(ID_MBUTTONCANCEL,cDialogAccountSettings::mButtonCancelClick)
	EVT_BUTTON(ID_MBUTTONDONE,cDialogAccountSettings::mButtonDoneClick)
END_EVENT_TABLE()
////Event Table End

cDialogAccountSettings::cDialogAccountSettings( wxWindow *parent, cAccount* pWallet, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxDialog( parent, id, title, position, size, style ) {
    mWallet = pWallet;
    CreateGUIControls();
}

cDialogAccountSettings::~cDialogAccountSettings() {
}

void cDialogAccountSettings::CreateGUIControls() {
    //Do not add custom code between
    //GUI Items Creation Start and GUI Items Creation End.
    //wxDev-C++ designer will remove them.
    //Add the custom code before or after the blocks
    ////GUI Items Creation Start

	mButtonCancel = new wxButton(this, ID_MBUTTONCANCEL, _("Cancel"), wxPoint(16, 320), wxSize(73, 33), 0, wxDefaultValidator, _("mButtonCancel"));

	mButtonDone = new wxButton(this, ID_MBUTTONDONE, _("Sign Tx"), wxPoint(208, 320), wxSize(73, 33), 0, wxDefaultValidator, _("mButtonDone"));

	wxArrayString arrayStringFor_mFlags;
	mFlags = new wxCheckListBox(this, ID_MFLAGS, wxPoint(32, 176), wxSize(233, 129), arrayStringFor_mFlags, wxLB_SINGLE, wxDefaultValidator, _("mFlags"));
	mFlags->Append(_("asfRequireDest"));
	mFlags->Append(_("asfRequireAuth"));
	mFlags->Append(_("asfDisallowXRP"));
	mFlags->Append(_("asfDisableMaster"));
	mFlags->Append(_("asfAccountTxnID"));
	mFlags->Append(_("asfNoFreeze"));
	mFlags->Append(_("asfGlobalFreeze"));
	mFlags->Append(_("asfDefaultRipple"));

	mTransferRate = new wxTextCtrl(this, ID_MTRANSFERRATE, _("0"), wxPoint(136, 136), wxSize(153, 25), 0, wxDefaultValidator, _("mTransferRate"));

	WxStaticText4 = new wxStaticText(this, ID_WXSTATICTEXT4, _("Transfer Rate"), wxPoint(16, 136), wxDefaultSize, 0, _("WxStaticText4"));

	mMessageKey = new wxTextCtrl(this, ID_MMESSAGEKEY, _(""), wxPoint(136, 96), wxSize(153, 25), 0, wxDefaultValidator, _("mMessageKey"));

	WxStaticText3 = new wxStaticText(this, ID_WXSTATICTEXT3, _("Message Key"), wxPoint(16, 96), wxDefaultSize, 0, _("WxStaticText3"));

	WxStaticText2 = new wxStaticText(this, ID_WXSTATICTEXT2, _("Email Address"), wxPoint(16, 56), wxDefaultSize, 0, _("WxStaticText2"));

	mEmailAddress = new wxTextCtrl(this, ID_MEMAILADDRESS, _(""), wxPoint(136, 56), wxSize(153, 25), 0, wxDefaultValidator, _("mEmailAddress"));

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, _("Domain"), wxPoint(16, 16), wxDefaultSize, 0, _("WxStaticText1"));

	mDomain = new wxTextCtrl(this, ID_MDOMAIN, _(""), wxPoint(136, 16), wxSize(153, 25), 0, wxDefaultValidator, _("mDomain"));

	SetTitle(_("Account Settings"));
	SetIcon(wxNullIcon);
	SetSize(8,8,313,404);
	Center();
	
    ////GUI Items Creation End

    mDomain->SetValue( mWallet->getDomain() );
    mEmailAddress->SetValue( mWallet->getEmailAddress() );
    mMessageKey->SetValue( mWallet->getMessageKey() );
    mTransferRate->SetValue( std::to_string( mWallet->getTransferRate() ) );

    for (auto Flags : mWallet->getFlags()) {
        mFlags->Check( Flags - 1, true );
    }
}

void cDialogAccountSettings::OnClose( wxCloseEvent& /*event*/ ) {
    Destroy();
}

/*
 * mButtonDoneClick
 */
void cDialogAccountSettings::mButtonDoneClick( wxCommandEvent& pEvent ) {
    const auto WalletFlags = mWallet->getFlags();

    mWallet->setDomain( mDomain->GetValue().ToStdString() );
    mWallet->setEmailAddress( mEmailAddress->GetValue().ToStdString() );
    mWallet->setMessageKey( mMessageKey->GetValue().ToStdString() );
    
    if(mTransferRate->GetValue().size())
        mWallet->setTransferRate( std::stoi( mTransferRate->GetValue().ToStdString() ) );

    for (int i = 0; i < 8; ++i) {
        if ( mFlags->IsChecked( i )  ) {
            mWallet->setNewFlag( i + 1 );
        }
    }

    mWallet = 0;
    Close();
}

/*
 * mButtonCancelClick
 */
void cDialogAccountSettings::mButtonCancelClick( wxCommandEvent& pEvent ) {
    Close();
}
