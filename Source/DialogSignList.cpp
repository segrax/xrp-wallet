///-----------------------------------------------------------------
///
/// @file      DialogSignList.cpp
/// @author
/// Created:   13/01/2017 17:44:37
/// @section   DESCRIPTION
///            cDialogSignList class implementation
///
///------------------------------------------------------------------
#include "stdafx.hpp"
#include "DialogSignList.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cDialogSignList
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialogSignList,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialogSignList::OnClose)
	EVT_BUTTON(ID_MBTNDELETE,cDialogSignList::mBtnDeleteClick)
	EVT_BUTTON(ID_MBTNADD,cDialogSignList::mBtnAddClick)
	EVT_BUTTON(ID_MBTNDONE,cDialogSignList::mBtnSignClick)
	
	EVT_LIST_END_LABEL_EDIT(ID_MLISTSIGNERS,cDialogSignList::mListSignersEndLabelEdit)
END_EVENT_TABLE()
////Event Table End

cDialogSignList::cDialogSignList( wxWindow *parent, cAccount* pWallet, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxDialog( parent, id, title, position, size, style ) {
    mWallet = pWallet;

    this->SetTitle( mWallet->getAddress() );

    CreateGUIControls();
}

cDialogSignList::~cDialogSignList() {
}

void cDialogSignList::CreateGUIControls() {
    //Do not add custom code between
    //GUI Items Creation Start and GUI Items Creation End.
    //wxDev-C++ designer will remove them.
    //Add the custom code before or after the blocks
    ////GUI Items Creation Start

	mBtnDelete = new wxButton(this, ID_MBTNDELETE, _("Remove"), wxPoint(88, 208), wxSize(73, 25), 0, wxDefaultValidator, _("mBtnDelete"));

	mBtnAdd = new wxButton(this, ID_MBTNADD, _("Add"), wxPoint(8, 208), wxSize(73, 25), 0, wxDefaultValidator, _("mBtnAdd"));

	mBtnDone = new wxButton(this, ID_MBTNDONE, _("Sign"), wxPoint(296, 208), wxSize(80, 25), 0, wxDefaultValidator, _("mBtnDone"));

	mListSigners = new wxListCtrl(this, ID_MLISTSIGNERS, wxPoint(8, 48), wxSize(369, 145), wxLC_REPORT | wxLC_EDIT_LABELS, wxDefaultValidator, _("mListSigners"));
	mListSigners->InsertColumn(0, _("Weight"), wxLIST_FORMAT_LEFT, 60);
	mListSigners->InsertColumn(1, _("Address"), wxLIST_FORMAT_LEFT, 300);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, _("Signer Target Weight"), wxPoint(16, 16), wxDefaultSize, 0, _("WxStaticText1"));

	mSignerQuorum = new wxTextCtrl(this, ID_MSIGNERQUORUM, _("2"), wxPoint(152, 8), wxSize(65, 25), 0, wxDefaultValidator, _("mSignerQuorum"));

	SetTitle(_("Signer List"));
	SetIcon(wxNullIcon);
	SetSize(8,8,404,275);
	Center();
	
    ////GUI Items Creation End
    mSigners = mWallet->getSigners();

    ReloadSignersList();
}

void cDialogSignList::OnClose( wxCloseEvent& /*event*/ ) {
    Destroy();
}

/*
 * mBtnSignClick
 */
void cDialogSignList::mBtnSignClick( wxCommandEvent& event ) {
    const auto Quorum = std::stoi( mSignerQuorum->GetValue().ToStdString() );
    uint32_t TotalWeight = 0;

    for (const auto& Signer : mSigners) {
        TotalWeight += Signer.weight;
    }

    if (TotalWeight < Quorum) {
        wxMessageBox( _( "Total weight of addresses is less than required weight") );
        return;
    }

    mWallet = 0;
    Close();
}

/*
 * mBtnCancelClick
 */
void cDialogSignList::mBtnCancelClick( wxCommandEvent& event ) {

    Close();
}

/*
 * mBtnAddClick
 */
void cDialogSignList::mBtnAddClick( wxCommandEvent& event ) {
    std::string Address = std::string(wxGetTextFromUser( _( "Address"), _( "Enter a signing address" )));
    std::string Quorum = std::string(wxGetTextFromUser( _( "Weight"), _( "Enter a signing weight") ));

    auto Account = ripple::parseBase58<ripple::AccountID>( Address );

    if (!Account) {
        wxMessageBox( _( "Invalid Address") );
        return;
    }

    if (Address == mWallet->getAddress()) {
        wxMessageBox( _( "Cannot add yourself to the sign list") );
        return;
    }
    mSigners.emplace_back( *Account, std::stoi( Quorum ) );
    mWallet->setSignerQuorum( std::stoi( mSignerQuorum->GetValue().ToStdString() ) );
    ReloadSignersList();
}

/*
 * mBtnDeleteClick
 */
void cDialogSignList::mBtnDeleteClick( wxCommandEvent& pEvent ) {
    int itemIndex = -1;

    while ((itemIndex = mListSigners->GetNextItem( itemIndex,
                                                   wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED )) != wxNOT_FOUND) {
        ripple::SignerEntries::SignerEntry* Entry = reinterpret_cast<ripple::SignerEntries::SignerEntry*>(mListSigners->GetItemData( itemIndex ));

        mSigners.remove( *Entry );
        mListSigners->DeleteItem( itemIndex );
    }
}

/*
 * mListSignersBeginLabelEdit
 */
void cDialogSignList::mListSignersEndLabelEdit( wxListEvent& pEvent ) {
    ripple::SignerEntries::SignerEntry* Entry = reinterpret_cast<ripple::SignerEntries::SignerEntry*>(pEvent.GetData());

    Entry->weight = std::stoi( pEvent.GetText().ToStdString() );
}

void cDialogSignList::ReloadSignersList() {
    mListSigners->DeleteAllItems();
    mSignerQuorum->SetValue( std::to_string( mWallet->getSignerQuorum() ) );

    for (const auto& Signer : mSigners) {
        int i = mListSigners->InsertItem( mListSigners->GetItemCount(), wxString::Format( wxT( "%u" ), (uint32_t)Signer.weight ) );

        mListSigners->SetItem( i, 1, wxString( ripple::toBase58( Signer.account ) ) );
        mListSigners->SetItemPtrData( i, (wxUIntPtr)&Signer );
    }
}
