///-----------------------------------------------------------------
///
/// @file      DialogSusPay.cpp
/// @author
/// Created:   8/01/2017 16:57:34
/// @section   DESCRIPTION
///            cDialog_SusPay class implementation
///
///------------------------------------------------------------------
#include "stdafx.hpp"
#include "DialogSusPay.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cDialog_SusPay
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(cDialog_SusPay,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(cDialog_SusPay::OnClose)
	EVT_BUTTON(ID_MFINISHBUTTON,cDialog_SusPay::mFinishButtonClick)
	EVT_BUTTON(ID_MCANCELBUTTON,cDialog_SusPay::mCancelButtonClick)
END_EVENT_TABLE()
////Event Table End

cDialog_SusPay::cDialog_SusPay( wxWindow *parent, cAccount* pWallet, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxDialog( parent, id, title, position, size, style )
{
    mWallet = pWallet;

    CreateGUIControls();
}

cDialog_SusPay::~cDialog_SusPay()
{
}

void cDialog_SusPay::CreateGUIControls()
{
    //Do not add custom code between
    //GUI Items Creation Start and GUI Items Creation End.
    //wxDev-C++ designer will remove them.
    //Add the custom code before or after the blocks
    ////GUI Items Creation Start

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, _("Proof"), wxPoint(16, 128), wxDefaultSize, 0, _("WxStaticText1"));

	mProofText = new wxTextCtrl(this, ID_MPROOFTEXT, _(""), wxPoint(8, 152), wxSize(217, 25), 0, wxDefaultValidator, _("mProofText"));

	wxDateTime mExecuteAfterTime_Date(8,wxDateTime::Jan,2017,16,59,57,770);
	mExecuteAfterTime = new wxTimePickerCtrl(this, ID_MEXECUTEAFTERTIME, mExecuteAfterTime_Date, wxPoint(120, 96), wxSize(73, 23) , wxDP_DROPDOWN, wxDefaultValidator, _("mExecuteAfterTime"));

	wxDateTime mExecuteAfterDate_Date(8,wxDateTime::Jan,2017,16,59,49,193);
	mExecuteAfterDate = new wxDatePickerCtrl(this, ID_MEXECUTEAFTERDATE, mExecuteAfterDate_Date, wxPoint(32, 96), wxSize(81, 23) , wxDP_DROPDOWN, wxDefaultValidator, _("mExecuteAfterDate"));

	mExecuteAfterEnabled = new wxCheckBox(this, ID_MEXECUTEAFTERENABLED, _("Execute After"), wxPoint(16, 72), wxSize(129, 25), 0, wxDefaultValidator, _("mExecuteAfterEnabled"));

	mFinishButton = new wxButton(this, ID_MFINISHBUTTON, _("Complete"), wxPoint(136, 184), wxSize(73, 33), 0, wxDefaultValidator, _("mFinishButton"));

	mCancelButton = new wxButton(this, ID_MCANCELBUTTON, _("Cancel"), wxPoint(16, 184), wxSize(73, 33), 0, wxDefaultValidator, _("mCancelButton"));

	wxDateTime mCancelAfterTime_Date(8,wxDateTime::Jan,2017,16,59,57,770);
	mCancelAfterTime = new wxTimePickerCtrl(this, ID_MCANCELAFTERTIME, mCancelAfterTime_Date, wxPoint(120, 40), wxSize(73, 23) , wxDP_DROPDOWN, wxDefaultValidator, _("mCancelAfterTime"));

	wxDateTime mCancelAfterDate_Date(8,wxDateTime::Jan,2017,16,59,49,193);
	mCancelAfterDate = new wxDatePickerCtrl(this, ID_MCANCELAFTERDATE, mCancelAfterDate_Date, wxPoint(32, 40), wxSize(81, 23) , wxDP_DROPDOWN, wxDefaultValidator, _("mCancelAfterDate"));

	mCancelAfterEnabled = new wxCheckBox(this, ID_MCANCELAFTER, _("Can Cancel After"), wxPoint(16, 16), wxSize(129, 25), 0, wxDefaultValidator, _("mCancelAfterEnabled"));

	SetTitle(_("Escrow Payment"));
	SetIcon(wxNullIcon);
	SetSize(8,8,248,264);
	Center();
	
    ////GUI Items Creation End
    
	wxDateTime TimeNow;

	mCancelAfterTime->SetValue( TimeNow.Now() );
	mCancelAfterDate->SetValue( TimeNow.Now() );

	mExecuteAfterTime->SetValue( TimeNow.Now() );
	mExecuteAfterDate->SetValue( TimeNow.Now() );
}

void cDialog_SusPay::OnClose( wxCloseEvent& /*event*/ )
{
    Destroy();
}

const uint64_t cDialog_SusPay::GetCancelledAfter() const {
	if (!mCancelAfterEnabled->IsChecked())
		return 0;

    // Date
    wxDateTime Time = mCancelAfterTime->GetValue();

    wxDateTime Date = mCancelAfterDate->GetValue();
    Date.ResetTime();

    Date.Set( Time.GetHour(), Time.GetMinute(), Time.GetSecond() );

    return Date.GetTicks();
}

const uint64_t cDialog_SusPay::GetExecuteAfter() const {
    if (!mExecuteAfterEnabled->IsChecked())
        return 0;

    // Date
    wxDateTime Time = mExecuteAfterTime->GetValue();

    wxDateTime Date = mExecuteAfterTime->GetValue();
    Date.ResetTime();

    Date.Set( Time.GetHour(), Time.GetMinute(), Time.GetSecond() );

    return Date.GetTicks();
}

const std::string cDialog_SusPay::GetProofText() const {
    return mProofText->GetValue().ToStdString();
}

/*
 * mFinishButtonClick
 */
void cDialog_SusPay::mFinishButtonClick(wxCommandEvent& event)
{
    mWallet = 0;
	Close();
}

/*
 * mCancelButtonClick
 */
void cDialog_SusPay::mCancelButtonClick(wxCommandEvent& event)
{
    Close();
}
