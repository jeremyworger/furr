#ifndef _ADJUSTPARAMS_H
#define _ADJUSTPARAMS_H

#include "stdafx.h"
#include "furrconfigs.h"

/*
 * Constants
 */
#define AP_SAVEDLG    _T("FURR - Save Image Adjustment Parameters")
#define AP_LOADDLG    _T("FURR - Load Image Adjustment Parameters")
#define AP_WEDGEDLG   _T("FURR - Choose Wedge Image File")
#define AP_PREVIEWDLG _T("FURR - Choose Preview Image")
#define AP_LUTDLG     _T("FURR - Choose LUT File")
#define AP_WDGEFILTER _T("All Image Files\0*.*\0\0")
#define AP_LUTFILTER  _T("Look Up Table Files (.LUT)\0*.lut\0\0")
#define AP_FILEFILTER _T("FURR Image Parameter Files (.FIP)\0*.fip\0\0")
#define AP_IMGFILTER  _T("All Image Formats\0*.*\0\0")
#define AP_PREVIEWCAP _T("FURR - Image Preview")
#define AP_FILENONE   _T("<none>")
#define AP_PREVIEWX   640
#define AP_PREVIEWY   480
#define AP_TWOK       _T("2048x1556")
#define AP_FOURK      _T("3840x2400")
#define AP_TESTRES    _T("1600x1200")
#define AP_INVALIDRES _T("<invalid>")
#define AP_24BPP      _T("24 bpp")
#define AP_32BPP      _T("32 bpp")
#define AP_TESTRESX   1600UL
#define AP_TESTRESY   1200UL
#define AP_NWEDGEPATH 48

/*
 * ImageResolution enumeration
 *
 * Possible image resolutions for
 * all sessions
 */
enum ImageResolution {
  TWO_K = 0,
  FOUR_K,
  TEST_RES,
  INVALID_RES
};

/*
 * BitDepth enumeration
 *
 * Possible bit depth values
 * for all sessions
 */
enum BitDepth {
  TWENTYFOUR = 0,
  THIRTYTWO,
  INVALIDDEPTH
};

/*
 * ShootMode enumeration
 *
 * Possible modes for session
 */
enum ShootMode {

  CHANNELSPLIT = 0,
  COLORWHEEL,
  FULLIMAGE

};

/*
 * APGLOBAL Structure
 *
 * Contains 'global' (not per-channel)
 * settings for the ADJUSTPARAMS structure.
 */
typedef struct tagAPGLOBAL {

  tagAPGLOBAL() {

    Reset();

  }

  tagAPGLOBAL &operator=(const tagAPGLOBAL &in) {

    ImgRes        = in.ImgRes;
    Depth         = in.Depth;
    Mode          = in.Mode;
    ZeroMemory(szLUTFile, MAX_PATH);
    CopyMemory(szLUTFile, in.szLUTFile, MAX_PATH);

    return (*this);

  }

  ImageResolution ImgRes;     // Image resolution
  BitDepth Depth;             // Image bitdepth
  TCHAR szLUTFile[MAX_PATH];  // LUT File Path
  ShootMode Mode;             // Image display mode

  void Reset(void) {

    ImgRes        = TWO_K;
    Depth         = THIRTYTWO;
    Mode          = FULLIMAGE;
    ZeroMemory(szLUTFile, MAX_PATH);

  }

  TSTRING Mode2String(void) {

    switch (Mode) {

      case CHANNELSPLIT: return _T("Channel Split");
      case COLORWHEEL: return _T("Color Wheel");
      default:
      case FULLIMAGE: return _T("Full Image");

    }

  }

} APGLOBAL, *PAPGLOBAL;

/*
 * APCHANNEL Structure
 *
 * Contains per-channel settings
 * for the ADJUSTPARAMS structure.
 */
typedef struct tagAPCHANNEL {

  tagAPCHANNEL() {

    Reset();

  }

  tagAPCHANNEL &operator=(const tagAPCHANNEL &in) {

    dwET        = in.dwET;
    iBrightness = in.iBrightness;
    iContrast   = in.iContrast;
    dGamma      = in.dGamma;

    return (*this);

  }

  DWORD dwET;       // Exposure time
  int iBrightness;  // Brightness
  int iContrast;    // Contrast
  double dGamma;    // Gamma

  void Reset(void) {

    dwET        = 0UL;
    iBrightness = 0;
    iContrast   = 0;
    dGamma      = 0.0;

  }

} APCHANNEL, *PAPCHANNEL;

/*
 * APWEDGE Structure
 *
 * Contains wedge session settings
 * for the ADJUSTPARAMS structure.
 */
typedef struct tagAPWEDGE {

  tagAPWEDGE() {

    Reset();

  }

  tagAPWEDGE &operator=(const tagAPWEDGE &in) {

    bWedgeSession = in.bWedgeSession;
    dwStartMsec   = in.dwStartMsec;
    dwStepMsec    = in.dwStepMsec;
    dwEndMsec     = in.dwEndMsec;
    dwSteps       = in.dwSteps;
    ZeroMemory(szWedgeFile, MAX_PATH);
    CopyMemory(szWedgeFile, in.szWedgeFile, _tcslen(in.szWedgeFile) * sizeof(TCHAR));

    return (*this);

  }

  bool bWedgeSession;           // Used to signal the FURRSession object that this session
                                // IS a wedge session
  DWORD_PTR dwStartMsec;        // Starting exposure time, in milliseconds
  DWORD_PTR dwStepMsec;         // Step-per-iteration expsoure time, in milliseconds
  DWORD_PTR dwEndMsec;          // Ending exposure time, in milliseconds
  DWORD_PTR dwSteps;            // # of steps in this session (42, etc)
  TCHAR szWedgeFile[MAX_PATH];  // Full path to image file used in wedge session

  void Reset(void) {

    bWedgeSession = false;
    dwStartMsec   = 0UL;
    dwStepMsec    = 0UL;
    dwEndMsec     = 0UL;
    dwSteps       = 0UL;
    ZeroMemory(szWedgeFile, MAX_PATH);

  }

} APWEDGE, *PAPWEDGE;

/*
 * ADJUSTPARAMS Structure
 *
 * Contains all data required to
 * adjust the parameters for a session
 */
typedef struct tagADJUSTPARAMS {

  tagADJUSTPARAMS() {

    Reset();

  }

  tagADJUSTPARAMS &operator=(const tagADJUSTPARAMS &in) {

    Global = in.Global;
    Red    = in.Red;
    Green  = in.Green;
    Blue   = in.Blue;
    Full   = in.Full;
    Wedge  = in.Wedge;

    return (*this);

  }

  APGLOBAL Global;
  APCHANNEL Red;
  APCHANNEL Green;
  APCHANNEL Blue;
  APCHANNEL Full;
  APWEDGE Wedge;

  void Reset(void) {

    Global.Reset();
    Red.Reset();
    Green.Reset();
    Blue.Reset();
    Full.Reset();
    Wedge.Reset();

  }

} ADJUSTPARAMS, *PADJUSTPARAMS;

void GetCurrentParamsPath(TSTRING &tsPath);
bool SetCurrentParamsPath(CTSTRING &tsPath);
void GetCurrentPreviewImage(TSTRING &tsImgPath);
ImageResolution StrRes2ImgRes(CTSTRING &tsStrRes);
void ImgRes2StrRes(ImageResolution res, TSTRING &tsOut);
DWORD_PTR ImgRes2NumericX(ImageResolution res);
DWORD_PTR ImgRes2NumericY(ImageResolution res);
void BitDepth2String(BitDepth bd, TSTRING &tsOut);
DWORD BitDepth2Numeric(BitDepth bd);
BitDepth String2BitDepth(CTSTRING &tsBitDepth);

#endif // !_ADJUSTPARAMS_H
