#ifndef _CHANNELADJUST_H
#define _CHANNELADJUST_H

#include "stdafx.h"

/*
 * Constants
 */
#define CA_RED      0x00000001UL
#define CA_GREEN    0x00000002UL
#define CA_BLUE     0x00000004UL
#define CA_ALPHA    0x00000008UL
#define CA_ABSOLUTE 0x00000010UL
#define CA_RELATIVE 0x00000020UL
#define CA_GREY     0x0000007F

/*
 * CHANNELADJUST Structure
 *
 * Contains data used by
 * the ChannelAdjust() function.
 */
typedef struct tagCHANNELADJUST {

  tagCHANNELADJUST() {

    pPixels     = NULL;
    dwCX        = 0UL;
    dwCY        = 0UL;
    dwKeepChan  = 0UL;
    fMask       = 0UL;
    ibRed       = 0;
    ibGreen     = 0;
    ibBlue      = 0;
    ibAlpha     = 0;
    icRed       = 0;
    icGreen     = 0;
    icBlue      = 0;
    bContrast   = false;
    bBrightness = false;
    bLUT        = false;
    bZeroChans  = false;
    tsLUTFile   = _T("");

  }

  tagCHANNELADJUST &operator=(const tagCHANNELADJUST &in) {

    pPixels     = in.pPixels;
    dwCX        = in.dwCX;
    dwCY        = in.dwCY;
    dwKeepChan  = in.dwKeepChan;
    fMask       = in.fMask;
    ibRed       = in.ibRed;
    ibGreen     = in.ibGreen;
    ibBlue      = in.ibBlue;
    ibAlpha     = in.ibAlpha;
    icRed       = in.icRed;
    icGreen     = in.icGreen;
    icBlue      = in.icBlue;
    iBrightness = in.iBrightness;
    bContrast   = in.bContrast;
    bBrightness = in.bBrightness;
    bLUT        = in.bLUT;
    bZeroChans  = in.bZeroChans;
    tsLUTFile   = in.tsLUTFile;

    return (*this);
  }

  BYTE *pPixels;
  DWORD_PTR fMask;
  DWORD_PTR dwCX;
  DWORD_PTR dwCY;
  DWORD_PTR dwKeepChan;
  int ibRed;
  int ibGreen;
  int ibBlue;
  int ibAlpha;
  int icRed;
  int icGreen;
  int icBlue;
  int iBrightness;
  bool bContrast;
  bool bBrightness;
  bool bLUT;
  bool bZeroChans;
  TSTRING tsLUTFile;

} CHANNELADJUST, *PCHANNELADJUST;

/*
 * Macros
 */
#define GET_ALPHA(p)    (static_cast<BYTE>((p >> 24)))
#define GET_RED(p)      (static_cast<BYTE>((p >> 16)))
#define GET_GREEN(p)    (static_cast<BYTE>((p >> 8)))
#define GET_BLUE(p)     (static_cast<BYTE>((p)))
#define SET_RED(p, r)   (reinterpret_cast<BYTE *>(p)[2] = r)
#define SET_GREEN(p, g) (reinterpret_cast<BYTE *>(p)[1] = g)
#define SET_BLUE(p, b)  (reinterpret_cast<BYTE *>(p)[0] = b)
#define SET_ALPHA(p, a) (reinterpret_cast<BYTE *>(p)[3] = a)


bool ChannelAdjust(const CHANNELADJUST &ca);

#endif // !_CHANNELADJUST_H
