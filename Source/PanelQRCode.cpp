///-----------------------------------------------------------------
///
/// @file      TileView.cpp
/// @author    Robbie
/// Created:   30/04/2016 17:39:36
/// @section   DESCRIPTION
///            cPanelTileView class implementation
///
///------------------------------------------------------------------

#include "stdafx.hpp"
#include "QrCode.hpp"
#include "PanelQRCode.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// cPanelTileView
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE( cPanelQRCode, wxPanel )
////Manual Code Start
////Manual Code End

EVT_CLOSE( cPanelQRCode::OnClose )
EVT_PAINT( cPanelQRCode::cTileViewPaint )
END_EVENT_TABLE()
////Event Table End

cPanelQRCode::cPanelQRCode( wxWindow *parent, wxWindowID id, const wxPoint &position, const wxSize& size, long style )
    : wxPanel( parent, id, position, size, wxFRAME_NO_TASKBAR | wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX) )
{
    mScale = 0;
    mDimension = 0;
    mFinalSize = 0;

    CreateGUIControls();
}

cPanelQRCode::~cPanelQRCode()
{
}

void cPanelQRCode::CreateGUIControls()
{
    //Do not add custom code between
    //GUI Items Creation Start and GUI Items Creation End.
    //wxDev-C++ designer will remove them.
    //Add the custom code before or after the blocks
    ////GUI Items Creation Start

    SetSize( 4, 4, 320, 334 );
    Center();

    ////GUI Items Creation End
}

void cPanelQRCode::OnClose( wxCloseEvent& /*event*/ )
{
    Destroy();
}

void cPanelQRCode::SetQRCode( const std::string& pQRCode ) {
    mQrCode = pQRCode;

    mQrCodeFinal = std::make_shared<qrcodegen::QrCode>( std::move( qrcodegen::QrCode::encodeText( mQrCode.c_str(), qrcodegen::QrCode::Ecc::HIGH ) ));

    mScale = 8;

    if (mQrCode.size() > 64)
        mScale = 6;

    if (mQrCode.size() > 1000)
        mScale = 3;

    mDimension = mQrCodeFinal->size;
    mFinalSize = (mDimension * mScale);

    this->SetSize( mFinalSize + 20, mFinalSize + 40 );
    this->GetParent()->SetSize( mFinalSize + 20, mFinalSize + 40 );

    Refresh();
}

/*
 * cTileViewPaint
 */
void cPanelQRCode::cTileViewPaint( wxPaintEvent& event ) {
    this->Centre();
    if (!mQrCodeFinal)
        return;

    wxBufferedPaintDC tileView( this, wxBUFFER_VIRTUAL_AREA );

    tileView.Clear();

    // Loop Height / Width
    for (int y = 0; y < (mFinalSize); y++) {
        for (int x = 0; x < (mFinalSize); x++) {
            // Should we draw a point here?
            if (mQrCodeFinal->getModule( x / mScale, y / mScale ) == 1) {
                tileView.DrawPoint( x , y  );
            }
        }
    }
}
