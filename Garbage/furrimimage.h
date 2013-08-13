 #ifndef _FURRIMIMAGE_H
#define _FURRIMIMAGE_H

#include "furrtypes.h"
#include "furrimage.h"
#include "channeladjust.h"

class FURRIMImage : public FURRImage, Image {

public:

  FURRIMImage() { }
  ~FURRIMImage() { }

  bool ReadImage(CTSTRING &tsImgFile) {

    if (0xFFFFFFFF != GetFileAttributes(tsImgFile.c_str())) {

      const char *szImg = NULL;

#if defined(UNICODE)

      char szTemp[MAX_PATH] = {0};

      if (-1 == wcstombs(szTemp, tsImgFile.c_str(), MAX_PATH)) {

        return false;

      } else {

        szImg = szTemp;

      }

#else

      szImg = tsImgFile.c_str();

#endif // !UNICODE

      try {

        // Read the image file from disk
        m_FullImage.read(szImg);

      } catch (Exception &e) {

        odsf(_T("FURRIMImage::ReadImage() : An error occurred reading image file '%s'; Error = '%s'\n"),
            tsImgFile.c_str(),
            e.what());

        return false;
      }

    } else {

      // File doesn't exist
      return false;

    }

    return true;
  }

  bool PrepareForShoot(ADJUSTPARAMS &ap, ANNOTATION &an) {

    // Apply image adjustments
    if (!_ChannelAdjust(FIC_FULL, 0, 0, ap.Global.szLUTFile)) {
      odsf(_T("FURRIMImage::PrepareForShoot() : Failed to apply image adjustments!\n"));
      return false;
    }

    if (!_Gamma(ap.Red.dGamma, ap.Green.dGamma, ap.Blue.dGamma)) {
      odsf(_T("FURRIMImage::PrepareForShoot() : Failed to apply gamma adjustment!\n"));
      return false;
    }

    // Full image is now properly adjusted.
    // Split RGB data into 3 seperate channel images
    _Reset();
    m_AdjustParams = ap;
    m_Annotation   = an;

    //
    // If we're using black/white, we don't need to split
    // into 3 channel images.
    //
    //return ap.Global.bChannelSplit ? _Recalculate() : true;
// TODO:     Which mode? act accordingly
    return true;

  }

  bool Resize(DWORD_PTR dwX, DWORD_PTR dwY) {

    Geometry g(dwX, dwY);

    try {
      
      m_FullImage.zoom(g);

    } catch (Exception &e) {

      odsf(_T("FURRIMImage::Resize() : An error occurred while resizing image; Error = '%s'\n"),
           e.what());

      return false;
    }

    return _Recalculate();
  }

  DWORD_PTR Width(void) {

    return static_cast<DWORD_PTR>(m_FullImage.size().width());

  }

  DWORD_PTR Height(void) {

    return static_cast<DWORD_PTR>(m_FullImage.size().height());

  }

  BYTE *AllPixels(void) {

    return reinterpret_cast<BYTE *>(m_FullImage.getPixels(0, 0, m_FullImage.columns(), m_FullImage.rows()));

  }

  BYTE *RedPixels(void) {

    return reinterpret_cast<BYTE *>(m_RedChannel.getPixels(0, 0, m_RedChannel.columns(), m_RedChannel.rows()));

  }

  BYTE *GreenPixels(void) {

    return reinterpret_cast<BYTE *>(m_GreenChannel.getPixels(0, 0, m_GreenChannel.columns(), m_GreenChannel.rows()));

  }

  BYTE *BluePixels(void) {

    return reinterpret_cast<BYTE *>(m_BlueChannel.getPixels(0, 0, m_BlueChannel.columns(), m_BlueChannel.rows()));

  }

  bool IsValid(void) {

    return m_FullImage.isValid();

  }

private:

 bool _Gamma(double dblRed, double dblGreen, double dblBlue) {

   try {

     odsf(_T("FURRIMImage::_Gamma() : Red: %f, Green: %f, Blue: %f\n"),
          dblRed,
          dblGreen,
          dblBlue);

     m_FullImage.gamma(dblRed, dblGreen, dblBlue);

   } catch (Exception &e) {

     odsf(_T("FURRIMImage::_Gamma() : Failed to adjust gamma. Error = %s\n"),
          e.what());
     return false;

   }

   return true;
 }

 bool _ChannelAdjust(DWORD_PTR dwChan, int iBrightness, int iContrast, CTSTRING &tsLUTFile) {

   bool r = true;

   odsf(_T("FURRIMImage::_ChannelAdjust() : Channel: %08X, Brightness: %d, Contrast: %d, ")
        _T("using LUT file %s\n"),
        dwChan,
        iBrightness,
        iContrast,
        tsLUTFile.c_str());

   CHANNELADJUST ca;

   switch (dwChan) {

     case FIC_FULL:
       ca.pPixels = AllPixels();
     break;
     case FIC_RED:
       ca.pPixels = RedPixels();
     break;
     case FIC_GREEN:
       ca.pPixels = GreenPixels();
     break;
     case FIC_BLUE:
       ca.pPixels = BluePixels();
     break;
     default: return false;

   }

   ca.dwCX        = Width();
   ca.dwCY        = Height();
   ca.iBrightness = iBrightness;
   ca.iContrast   = iContrast;
   ca.tsLUTFile   = tsLUTFile;

   ca.fMask = CA_RELATIVE;

   if (0 != iBrightness) { ca.bBrightness = true; }
   if (0 != iContrast) { ca.bContrast = true; }
   if (0 != tsLUTFile.size()) { ca.bLUT = true; }

   ::ChannelAdjust(ca);

   return r;
 }

  bool _Annotate(DWORD_PTR dwChan, const ANNOTATION &a) {

    bool r = false;

    Image *pImg = NULL;

    switch (dwChan) {

      case FIC_FULL:
        pImg = &m_FullImage;
      break;
      case FIC_RED:
        pImg = &m_RedChannel;
      break;
      case FIC_GREEN:
        pImg = &m_GreenChannel;
      break;
      case FIC_BLUE:
        pImg = &m_BlueChannel;
      break;
      default: return false;

    }

    try {

      ColorRGB clrText(a.dTextRed, a.dTextGreen, a.dTextBlue);

      pImg->draw(DrawableRectangle(a.drawX, a.drawY, a.drawCY, a.drawCX));

      Geometry g(a.drawCX, a.drawCY, a.drawX, a.drawY);

      pImg->fontPointsize(a.dFontSize);

      pImg->fillColor(clrText);

      pImg->annotate(a.szText, g, CenterGravity);

      r = true;

    } catch (Exception &e) {

      odsf(_T("FURRIMImage::_Annotate() : Exception occurred: '%s'\n"), e.what());

    }

    return r;
  }

  bool _Recalculate(void) {

    // Split main image into 3 channels
    try {

      m_RedChannel = m_FullImage;
      m_RedChannel.channel(RedChannel);

      m_GreenChannel = m_FullImage;
      m_GreenChannel.channel(GreenChannel);

      m_BlueChannel = m_FullImage;
      m_BlueChannel.channel(BlueChannel);

    } catch (Exception &e) {

      odsf(_T("FURRIMImage::_Recalculate() : Exception occurred while splitting channels: '%s'\n"), e.what());
      return false;

    }

    // Apply brightness/contrast to each channel
    if (m_AdjustParams.Red.iBrightness > 0 || m_AdjustParams.Red.iContrast > 0) {

      if (!_ChannelAdjust(FIC_RED, m_AdjustParams.Red.iBrightness, m_AdjustParams.Red.iContrast, _T(""))) {
        odsf(_T("FURRIMImage::_Recalculate() : _ChannelAdjust() failed for RED channel!\n"));
        return false;
      }

    }

    if (m_AdjustParams.Green.iBrightness > 0 || m_AdjustParams.Green.iContrast > 0) {

      if (!_ChannelAdjust(FIC_GREEN, m_AdjustParams.Green.iBrightness, m_AdjustParams.Green.iContrast, _T(""))) {
        odsf(_T("FURRIMImage::_Recalculate() : _ChannelAdjust() failed for GREEN channel!\n"));
        return false;
      }

    }

    if (m_AdjustParams.Blue.iBrightness > 0 || m_AdjustParams.Blue.iContrast > 0) {

      if (!_ChannelAdjust(FIC_BLUE, m_AdjustParams.Blue.iBrightness, m_AdjustParams.Blue.iContrast, _T(""))) {
        odsf(_T("FURRIMImage::_Recalculate() : _ChannelAdjust() failed for BLUE channel!\n"));
        return false;
      }

    }

    // Apply annotation text to each channel, if applicable
    if (m_Annotation.bUseAnnotation) {

      if (!_Annotate(FIC_RED, m_Annotation)) {
        odsf(_T("FURRIMImage::_Recalculate() : _Annotate() failed for RED channel!\n"));
        return false;
      }

      if (!_Annotate(FIC_GREEN, m_Annotation)) {
        odsf(_T("FURRIMImage::_Recalculate() : _Annotate() failed for GREEN channel!\n"));
        return false;
      }

      if (!_Annotate(FIC_BLUE, m_Annotation)) {
        odsf(_T("FURRIMImage::_Recalculate() : _Annotate() failed for BLUE channel!\n"));
        return false;
      }

    }

    // All 3 channels have been seperated and adjusted;
    // this image is ready for shooting
    return true;
  }

  void _Reset(void) {

    m_AdjustParams.Reset();
    m_Annotation.Reset();

  }


private:
  ADJUSTPARAMS m_AdjustParams;
  ANNOTATION m_Annotation;
  Image m_FullImage;
  Image m_RedChannel;
  Image m_GreenChannel;
  Image m_BlueChannel;

};

#endif // !_FURRIMIMAGE_H