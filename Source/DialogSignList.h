///-----------------------------------------------------------------
///
/// @file      DialogSignList.h
/// @author
/// Created:   13/01/2017 17:44:37
/// @section   DESCRIPTION
///            cDialogSignList class declaration
///
///------------------------------------------------------------------

#ifndef __CDIALOGSIGNLIST_H__
#define __CDIALOGSIGNLIST_H__

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
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
////Header Include End

////Dialog Style Start
#undef cDialogSignList_STYLE
#define cDialogSignList_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class cDialogSignList : public wxDialog
{
    friend class cXrp_WalletFrm;

    private:
    DECLARE_EVENT_TABLE();
    cAccount* mWallet;
    std::list< ripple::SignerEntries::SignerEntry >	mSigners;

    public:
    cDialogSignList( wxWindow *parent, cAccount* pWallet, wxWindowID id = 1, const wxString &title = wxT( "Untitled5" ), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = cDialogSignList_STYLE );
    virtual ~cDialogSignList();
    void mBtnSignClick( wxCommandEvent& event );
    void mBtnCancelClick( wxCommandEvent& event );
    void mBtnAddClick( wxCommandEvent& event );
    void mBtnDeleteClick( wxCommandEvent& event );
    void mListSignersEndLabelEdit( wxListEvent& event );

    private:
    //Do not add custom control declarations between
    //GUI Control Declaration Start and GUI Control Declaration End.
    //wxDev-C++ will remove them. Add custom code after the block.
    ////GUI Control Declaration Start
		wxButton *mBtnDelete;
		wxButton *mBtnAdd;
		wxButton *mBtnDone;
		wxListCtrl *mListSigners;
		wxStaticText *WxStaticText1;
		wxTextCtrl *mSignerQuorum;
    ////GUI Control Declaration End

    private:
    //Note: if you receive any error with these enum IDs, then you need to
    //change your old form code that are based on the #define control IDs.
    //#defines may replace a numeric value for the enum names.
    //Try copy and pasting the below block in your old form header files.
    enum
    {
        ////GUI Enum Control ID Start
			ID_MBTNDELETE = 1009,
			ID_MBTNADD = 1007,
			ID_MBTNDONE = 1005,
			ID_MLISTSIGNERS = 1004,
			ID_WXSTATICTEXT1 = 1003,
			ID_MSIGNERQUORUM = 1002,
        ////GUI Enum Control ID End
        ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
    };

    private:
    void OnClose( wxCloseEvent& event );
    void CreateGUIControls();
    void ReloadSignersList();
};

#endif
