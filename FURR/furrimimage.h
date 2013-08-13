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

    m_AdjustParams  = ap;
    m_Annotation    = an;

    return _Recalculate();

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

  bool _Recalculate(void) {

    if (IsValid()) {

      switch (m_AdjustParams.Global.Mode) {

        case CHANNELSPLIT:
        {

          // Split into 3 images; zero channel data

          try {

            m_RedChannel    = m_FullImage;
            m_GreenChannel  = m_FullImage;
            m_BlueChannel   = m_FullImage;

          } catch (Exception &e) {

            odsf(_T("FURRIMImage::_Recalculate(CHANNELSPLIT) :")
                _T(" Exception '%s' occurred while splitting image(s)\n"),
                e.what());
            return false;

          }

          CHANNELADJUST ca;

          ca.ibRed        = m_AdjustParams.Red.iBrightness;
          ca.bBrightness  = (0 < ca.ibRed) ? true : false;
          ca.icRed        = m_AdjustParams.Red.iContrast;
          ca.bContrast    = (0 < ca.icRed) ? true : false;
          ca.bLUT         = (0 < _tcslen(m_AdjustParams.Global.szLUTFile)) ? true : false;
          ca.tsLUTFile    = m_AdjustParams.Global.szLUTFile;
          ca.bZeroChans   = true;
          ca.dwCX         = Width();
          ca.dwCY         = Height();
          ca.pPixels      = RedPixels();
          ca.dwKeepChan   = CA_RED;
          ca.fMask        = CA_RELATIVE | CA_RED;

          // Red channel
          if (!::ChannelAdjust(ca)) {

            odsf(_T("FURRIMImage::_Recalculate(CHANNELSPLIT) : Failed to adjust RED channel.\n"));
            return false;

          }

          if (!_Gamma(m_RedChannel, m_AdjustParams.Red.dGamma, 
                      m_AdjustParams.Green.dGamma,
                      m_AdjustParams.Blue.dGamma)) {

            odsf(_T("FURRIMImage::_Gamma(RED) : Failed!\n"));
            return false;

          }

          // Green channel
          ca.ibGreen      = m_AdjustParams.Green.iBrightness;
          ca.bBrightness  = (0 < ca.ibGreen) ? true : false;
          ca.icGreen      = m_AdjustParams.Green.iContrast;
          ca.bContrast    = (0 < ca.icGreen) ? true : false;
          ca.dwKeepChan   = CA_GREEN;
          ca.pPixels      = GreenPixels();
          ca.fMask        = CA_RELATIVE | CA_GREEN;

          if (!::ChannelAdjust(ca)) {

            odsf(_T("FURRIMImage::_Recalculate(CHANNELSPLIT) : Failed to adjust GREEN channel.\n"));
            return false;

          }

          if (!_Gamma(m_GreenChannel, m_AdjustParams.Red.dGamma, 
                      m_AdjustParams.Green.dGamma,
                      m_AdjustParams.Blue.dGamma)) {

            odsf(_T("FURRIMImage::_Gamma(GREEN) : Failed!\n"));
            return false;

          }

          // Blue channel
          ca.ibBlue       = m_AdjustParams.Blue.iBrightness;
          ca.bBrightness  = (0 < ca.ibBlue) ? true : false;
          ca.icBlue       = m_AdjustParams.Blue.iContrast;
          ca.bContrast    = (0 < ca.icBlue) ? true : false;
          ca.dwKeepChan   = CA_BLUE;
          ca.pPixels      = BluePixels();
          ca.fMask        = CA_RELATIVE | CA_BLUE;

          if (!::ChannelAdjust(ca)) {

            odsf(_T("FURRIMImage::_Recalculate(CHANNELSPLIT) : Failed to adjust BLUE channel.\n"));
            return false;

          }

          if (!_Gamma(m_BlueChannel, m_AdjustParams.Red.dGamma, 
                      m_AdjustParams.Green.dGamma,
                      m_AdjustParams.Blue.dGamma)) {

            odsf(_T("FURRIMImage::_Gamma(BLUE) : Failed!\n"));
            return false;

          }

          return true;
        }

        case COLORWHEEL:
        {

          // Split into 3 images; greyscale

          CHANNELADJUST ca;

          ca.ibRed    = m_AdjustParams.Red.iBrightness;
          ca.ibGreen  = m_AdjustParams.Green.iBrightness;
          ca.ibBlue   = m_AdjustParams.Blue.iBrightness;

          if ((0 != ca.ibRed) || (0 != ca.ibGreen) || (0 != ca.ibBlue)) {

            ca.bBrightness = true;

          } else {

            ca.bBrightness = false;

          }

          ca.icRed    = m_AdjustParams.Red.iContrast;
          ca.icGreen  = m_AdjustParams.Green.iContrast;
          ca.icBlue   = m_AdjustParams.Blue.iContrast;

          if ((0 != ca.icRed) || (0 != ca.icGreen) || (0 != ca.icBlue)) {

            ca.bContrast = true;

          } else {

            ca.bContrast = false;

          }

          ca.bLUT         = (0 < _tcslen(m_AdjustParams.Global.szLUTFile)) ? true : false;
          ca.tsLUTFile    = m_AdjustParams.Global.szLUTFile;
          ca.bZeroChans   = false;
          ca.dwCX         = Width();
          ca.dwCY         = Height();
          ca.dwKeepChan   = 0UL;
          ca.fMask        = CA_RELATIVE | CA_RED | CA_GREEN | CA_BLUE;
          ca.pPixels      = AllPixels();

          if (!::ChannelAdjust(ca)) {

            odsf(_T("FURRIMImage::_Recalculate(COLORWHEEL) : Failed to adjust full image.\n"));
            return false;

          }

          if (!_Gamma(m_FullImage, m_AdjustParams.Red.dGamma, m_AdjustParams.Green.dGamma, m_AdjustParams.Blue.dGamma)) {

            odsf(_T("FURRIMImage::_Gamma(COLORWHEEL) : Failed!\n"));
            return false;

          }
          
          try {
            
            m_RedChannel    = m_FullImage;
            m_RedChannel.channel(RedChannel);
            m_GreenChannel  = m_FullImage;
            m_GreenChannel.channel(GreenChannel);
            m_BlueChannel   = m_FullImage;
            m_BlueChannel.channel(BlueChannel);

            return true;

          } catch (Exception &e) {

            odsf(_T("FURRIMImage::_Recalculate(COLORWHEEL) : Exception when splitting image(s) : '%s'\n"),
                e.what());
            return false;

          }

          return true;
        }

        case FULLIMAGE:
        {

          // Do not split image, but apply adjustments

          CHANNELADJUST ca;

          ca.ibRed    = m_AdjustParams.Red.iBrightness;
          ca.ibGreen  = m_AdjustParams.Green.iBrightness;
          ca.ibBlue   = m_AdjustParams.Blue.iBrightness;

          if ((0 != ca.ibRed) || (0 != ca.ibGreen) || (0 != ca.ibBlue)) {

            ca.bBrightness = true;

          } else {

            ca.bBrightness = false;

          }

          ca.icRed    = m_AdjustParams.Red.iContrast;
          ca.icGreen  = m_AdjustParams.Green.iContrast;
          ca.icBlue   = m_AdjustParams.Blue.iContrast;

          if ((0 != ca.icRed) || (0 != ca.icGreen) || (0 != ca.icBlue)) {

            ca.bContrast = true;

          } else {

            ca.bContrast = false;

          }

          ca.bLUT         = (0 < _tcslen(m_AdjustParams.Global.szLUTFile)) ? true : false;
          ca.tsLUTFile    = m_AdjustParams.Global.szLUTFile;
          ca.bZeroChans   = false;
          ca.dwCX         = Width();
          ca.dwCY         = Height();
          ca.dwKeepChan   = 0UL;
          ca.fMask        = CA_RELATIVE | CA_RED | CA_GREEN | CA_BLUE;
          ca.pPixels      = AllPixels();

          if (!::ChannelAdjust(ca)) {

            odsf(_T("FURRIMImage::_Recalculate(FULLIMAGE) : Failed to adjust full image!\n"));
            return false;

          }

          if (!_Gamma(m_FullImage, m_AdjustParams.Red.dGamma, m_AdjustParams.Green.dGamma, m_AdjustParams.Blue.dGamma)) {

            odsf(_T("FURRIMImage::_Gamma(FULLIMAGE) : Failed!\n"));
            return false;

          }

          return true;
        }
        default: return false;

      }

    }

    return false;
  }

  bool _Gamma(Image &img, double dblRed, double dblGreen, double dblBlue) {

    try {

      img.gamma(dblRed, dblGreen, dblBlue);

    } catch (Exception &e) {

      odsf(_T("FURRIMImage::_Gamma() : Failed to adjust gamma. Error = %s\n"),
            e.what());
      return false;

    }

    return true;
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