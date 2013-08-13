#include "stdafx.h"
#include "channeladjust.h"
#include "lookuptable.h"
#include <math.h>

/*
 * FUNCTION: ChannelAdjust()
 *
 * SYNOPSIS: Adjusts the value of each channel
 * (ARGB) of a specified pixel data set.
 *
 * ARGUMENTS: Reference to constant 
 * CHANNELADJUST Structure
 *
 * RETURN VALUE: Boolean success value
 */
bool ChannelAdjust(const CHANNELADJUST &ca) {

  DWORD_PTR dwPitch = (((ca.dwCX << 2) + 3) & ~3); // 4-byte aligned pitch
  BYTE *pWrite      = ca.pPixels;
  DWORD *pTemp      = NULL;
  LookupTable LUT(TWELVE);

  if (ca.bLUT) {

    // If we're doing LUT adjustment, we have to load
    // the LUT data now, so that it is present for the duration.

    if (!LUT.ParseLUTFile(ca.tsLUTFile.c_str())) {
      return false;
    }

  }

  for (DWORD_PTR Y = 0UL; Y < ca.dwCY; Y++) {

    pTemp = reinterpret_cast<DWORD *>(pWrite);

    for (DWORD_PTR X = 0UL; X < ca.dwCX; X++) { 

      UCHAR ucRed   = GET_RED(*pTemp);
      UCHAR ucGreen = GET_GREEN(*pTemp);
      UCHAR ucBlue  = GET_BLUE(*pTemp);
      UCHAR ucAlpha = GET_ALPHA(*pTemp);

      if (ca.bLUT) { // LUT adjustment
            
        if (!ca.bZeroChans) {

          SET_RED(pTemp, ucRed = static_cast<BYTE>(LUT.RedLUT[ucRed]));
          SET_GREEN(pTemp, ucGreen = static_cast<BYTE>(LUT.GreenLUT[ucGreen]));
          SET_BLUE(pTemp, ucBlue = static_cast<BYTE>(LUT.BlueLUT[ucBlue]));

        } else {

          // We're going to be zeroing out 2 channels, so discover which,
          // and set only the remaining channel
          switch (ca.dwKeepChan) {

            case CA_RED:
              SET_RED(pTemp, ucRed = static_cast<BYTE>(LUT.RedLUT[ucRed]));
            break;

            case CA_GREEN:
              SET_GREEN(pTemp, ucGreen = static_cast<BYTE>(LUT.GreenLUT[ucGreen]));
            break;

            case CA_BLUE:
              SET_BLUE(pTemp, ucBlue = static_cast<BYTE>(LUT.BlueLUT[ucBlue]));
            break;

          }

        }

      } // !LUT adjustment

      if (ca.bBrightness) {  // Intensity adjustment

        BOOL bRed         = ca.fMask & CA_RED;
        BOOL bGreen       = ca.fMask & CA_GREEN;
        BOOL bBlue        = ca.fMask & CA_BLUE;
        BOOL bAlpha       = ca.fMask & CA_ALPHA;
        BOOL bAbsolute    = ca.fMask & CA_ABSOLUTE;
        BOOL bRelative    = ca.fMask & CA_RELATIVE;

        if ((bRed || bGreen || bBlue || bAlpha) && (bAbsolute || bRelative)) {

          UCHAR ucTemp;

          if (bRed && (ca.bZeroChans && ca.dwKeepChan == CA_RED))   { 

            ucTemp = ucRed;

            if (bAbsolute) { 
              ucTemp = ca.ibRed; 
            } else {
              if (ucTemp + ca.ibRed > 255) {
                ucTemp = 255;
              } else if (ucTemp + ca.ibRed < 0) {
                ucTemp = 0;
              } else {
                ucTemp = ucTemp + ca.ibRed;
              }
            }

            SET_RED(pTemp, ucTemp); 
          
          }

          if (bGreen && (ca.bZeroChans &&ca.dwKeepChan == CA_GREEN))   { 
            
            ucTemp = ucGreen;

            if (bAbsolute) { 
              ucTemp = ca.ibGreen; 
            } else {
              if (ucTemp + ca.ibGreen > 255) {
                ucTemp = 255;
              } else if (ucTemp + ca.ibGreen < 0) {
                ucTemp = 0;
              } else {
                ucTemp = ucTemp + ca.ibGreen;
              }
            }

            SET_GREEN(pTemp, ucTemp);
          
          }

          if (bBlue && (ca.bZeroChans &&ca.dwKeepChan == CA_BLUE))   { 
            
            ucTemp = ucBlue;

            if (bAbsolute) { 
              ucTemp = ca.ibBlue; 
            } else {
              if (ucTemp + ca.ibBlue > 255) {
                ucTemp = 255;
              } else if (ucTemp + ca.ibBlue < 0) {
                ucTemp = 0;
              } else {
                ucTemp = ucTemp + ca.ibBlue;
              }
            }

            SET_BLUE(pTemp, ucTemp);
          
          }

          if (bAlpha)   { 
            
            ucTemp = ucAlpha;

            if (bAbsolute) { 
              ucTemp = ca.ibAlpha; 
            } else {
              if (ucTemp + ca.ibAlpha > 255) {
                ucTemp = 255;
              } else if (ucTemp + ca.ibAlpha < 0) {
                ucTemp = 0;
              } else {
                ucTemp = ucTemp + ca.ibAlpha;
              }
            }

            SET_ALPHA(pTemp, ucTemp);
          
          }

        }

      } // !Intensity adjustment

      if (ca.bContrast) { // Contrast adjustment

        int irContrast = (ca.icRed + 100);
        int igContrast = (ca.icGreen + 100);
        int ibContrast = (ca.icBlue + 100);

        double drContrast = irContrast * 0.01f;
        double dgContrast = igContrast * 0.01f;
        double dbContrast = ibContrast * 0.01f;

        int iTemp;

        if (irContrast > 99) {
          drContrast = drContrast * drContrast * drContrast;
        }

        if (igContrast > 99) {
          dgContrast = igContrast * igContrast * igContrast;
        }

        if (ibContrast > 99) {
          ibContrast = ibContrast * ibContrast * ibContrast;
        }

        if (!ca.bZeroChans) {

          // We're keeping all channels; set contrast for all
          iTemp = static_cast<int>(floor((ucRed - CA_GREY) * drContrast) + CA_GREY);
          SET_RED(pTemp, iTemp > 0xFF ? 0xFF : iTemp < 0 ? 0 : iTemp);

          iTemp = static_cast<int>(floor((ucGreen - CA_GREY) * dgContrast) + CA_GREY);
          SET_GREEN(pTemp, iTemp > 0xFF ? 0xFF : iTemp < 0 ? 0 : iTemp);

          iTemp = static_cast<int>(floor((ucBlue - CA_GREY) * dbContrast) + CA_GREY);
          SET_BLUE(pTemp, iTemp > 0xFF ? 0xFF : iTemp < 0 ? 0 : iTemp);

        } else {

          // We're only keeping one channel, so only apply contrast
          // the single channel
          switch (ca.dwKeepChan) {

            case CA_RED:
              iTemp = static_cast<int>(floor((ucRed - CA_GREY) * drContrast) + CA_GREY);
              SET_RED(pTemp, iTemp > 0xFF ? 0xFF : iTemp < 0 ? 0 : iTemp);
            break;

            case CA_GREEN:
              iTemp = static_cast<int>(floor((ucGreen - CA_GREY) * dgContrast) + CA_GREY);
              SET_GREEN(pTemp, iTemp > 0xFF ? 0xFF : iTemp < 0 ? 0 : iTemp);
            break;

            case CA_BLUE:
              iTemp = static_cast<int>(floor((ucBlue - CA_GREY) * dbContrast) + CA_GREY);
              SET_BLUE(pTemp, iTemp > 0xFF ? 0xFF : iTemp < 0 ? 0 : iTemp);
            break;

          }

        }

      } // !Contrast adjustment

      // Zero-out any channels that are not listed to keep
      // if zeroing is enabled
      if (ca.bZeroChans) {

        switch (ca.dwKeepChan) {

          case CA_RED:

            SET_GREEN(pTemp, 0);
            SET_BLUE(pTemp, 0);

          break;
          case CA_GREEN:

            SET_RED(pTemp, 0);
            SET_BLUE(pTemp, 0);

          break;
          case CA_BLUE:

            SET_RED(pTemp, 0);
            SET_GREEN(pTemp, 0);

          break;

        }

      }

      pTemp++;
    }

    pWrite += dwPitch;

  }

  return true;
}
