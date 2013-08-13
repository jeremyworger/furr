#include "stdafx.h"
#include "adjustparams.h"

void GetCurrentParamsPath(TSTRING &tsPath) {

  RegistryKey Key;

  if (!Key.Create(FURR_ROOTREGISTRYKEY, FURR_REGISTRYCONFIGSPATH)) {

    MessageBox(GetForegroundWindow(), _T("Failed to open registry key that contains current image parameters file path;")
                                      _T(" reverting to defaults."), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

    GetAppPath(tsPath);
    tsPath += FURR_DEFIMAGEPARAMSFILE;

  }

  if (!Key.ReadString(FURR_CURIMAGEPARAMSVALUE, tsPath)) {

    MessageBox(GetForegroundWindow(), _T("Failed to read registry key that contains current image parameters file path;")
                                      _T(" reverting to defaults."), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

    GetAppPath(tsPath);
    tsPath += FURR_DEFIMAGEPARAMSFILE;

  }

  if (_T("") == tsPath) {

    MessageBox(GetForegroundWindow(), _T("No current image parameters file is saved in the registry; reverting to defaults."),
                                      _T("FURR - Information"), MB_OK | MB_ICONINFORMATION);

    GetAppPath(tsPath);
    tsPath += FURR_DEFIMAGEPARAMSFILE;

  }

}

bool SetCurrentParamsPath(CTSTRING &tsPath) {

  bool r = false;
  RegistryKey Key;

  if (Key.Create(FURR_ROOTREGISTRYKEY, FURR_REGISTRYCONFIGSPATH)) {

    if (Key.WriteString(FURR_CURIMAGEPARAMSVALUE, tsPath)) {
    
      r = true;

    }

  }

  return r;
}

void GetCurrentPreviewImage(TSTRING &tsImgPath) {


  RegistryKey Key;

  if (!Key.Create(FURR_ROOTREGISTRYKEY, FURR_REGISTRYCONFIGSPATH)) {

    MessageBox(GetForegroundWindow(), _T("Failed to open registry key that contains current preview image path!"),
                                      _T("FURR - Error"), MB_OK | MB_ICONSTOP);

  }

  Key.ReadString(FURR_CURPREVIEWIMAGEVALUE, tsImgPath);

}

ImageResolution StrRes2ImgRes(CTSTRING &tsStrRes) {

  ImageResolution r = INVALID_RES;

  if (AP_FOURK == tsStrRes) {
    r = FOUR_K;
  } else if (AP_TWOK == tsStrRes) {
    r = TWO_K;
  } else if (AP_TESTRES == tsStrRes) {
    r = TEST_RES;
  }

  return r;
}

void ImgRes2StrRes(ImageResolution res, TSTRING &tsOut) {

  switch (res) {

    case FOUR_K:
      tsOut = AP_FOURK;
    break;
    case TWO_K:
      tsOut = AP_TWOK;
    break;
    case TEST_RES:
      tsOut = AP_TESTRES;
    break;
    case INVALID_RES:
      tsOut = AP_INVALIDRES;
    break;

  }

}

DWORD_PTR ImgRes2NumericX(ImageResolution res) {

  DWORD_PTR r = 0;

  switch (res) {

    case FOUR_K:
      r = 3840UL;
    break;
    case TWO_K:
      r = 2048UL;
    break;
    case TEST_RES:
      r = AP_TESTRESX;
    break;
    case INVALID_RES:
      r = 0UL;
    break;
  }

  return r;
}

DWORD_PTR ImgRes2NumericY(ImageResolution res) {

  DWORD_PTR r = 0;

  switch (res) {

    case FOUR_K:
      r = 2400UL;
    break;
    case TWO_K:
      r = 1556UL;
    break;
    case TEST_RES:
      r = AP_TESTRESY;
    break;
    case INVALID_RES:
      r = 0UL;
    break;

  }

  return r;
}

void BitDepth2String(BitDepth bd, TSTRING &tsOut) {

  switch (bd) {

    case TWENTYFOUR:
      tsOut = AP_24BPP;
    break;
    case THIRTYTWO:
      tsOut = AP_32BPP;
    break;
    case INVALIDDEPTH:
      tsOut = AP_INVALIDRES;
    break;

  }

}

DWORD BitDepth2Numeric(BitDepth bd) {

  DWORD r = 0UL;

  switch (bd) {

    case TWENTYFOUR:
      r = 24;
    break;
    case THIRTYTWO:
      r = 32;
    break;
    case INVALIDDEPTH:
      r = 0;
    break;

  }

  return r;
}

BitDepth String2BitDepth(CTSTRING &tsBitDepth) {

  BitDepth r = INVALIDDEPTH;

  if (AP_24BPP == tsBitDepth) {
    r = TWENTYFOUR;
  } else if (AP_32BPP == tsBitDepth) {
    r = THIRTYTWO;
  }

  return r;
}