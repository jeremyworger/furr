#ifndef _LOOKUPTABLE_H
#define _LOOKUPTABLE_H

#include "stdafx.h"
#include <fstream>

/*
 * Constants
 */
#define LUT_EIGHT_BIT_ENTRIES   512
#define LUT_TEN_BIT_ENTRIES     1024
#define LUT_TWELVE_BIT_ENTRIES  4096
#define LUT_SIXTEEN_BIT_ENTRIES 65536

#define LUT_TEN_BIT_RATIO       2
#define LUT_TWELVE_BIT_RATIO    4
#define LUT_SIXTEEN_BIT_RATIO   8

/*
 * LUTInputStream typedefine
 *
 * Provides an input stream for reading LUT files
 */
typedef std::basic_ifstream< short, std::char_traits<short> > LUTInputStream;

/*
 * Bit-depth enumeration
 */
enum LUTDepth {

  EIGHT = 0,
  TEN,
  TWELVE,
  SIXTEEN

};

/*
 * LookupTable Class
 *
 * Provides LUT functionality
 * with unique tables for each
 * channel (R, G, B) in a pixel.
 */
class LookupTable {

public:
  LookupTable(LUTDepth Depth) {

    m_Depth   = Depth;
    RedLUT    = NULL;
    GreenLUT  = NULL;
    BlueLUT   = NULL;

  }

  virtual ~LookupTable() {
  
    ResetLUTs();

  }

  bool ParseLUTFile(const TCHAR *szFilePath, bool bConvertToEightBit = true) {

    bool r              = false;
    const char *szFile  = NULL;
    LUTInputStream iStream;

#ifdef UNICODE

    char szTemp[MAX_PATH] = {0};

    if (-1 != wcstombs(szTemp, szFilePath, MAX_PATH)) {

      szFile = szTemp;

    } else {

      return r;

    }

#else

    szFile = szFilePath;

#endif // !UNICODE

    iStream.open(szFile, std::ios_base::in | std::ios_base::binary);

    if (iStream.is_open()) {

      ResetLUTs();

      short sTemp;

      //
      // Determine number of pixel entries,
      // and the ratio in case of conversion needs
      //
      int iEntries;
      int iRatio;

      switch (m_Depth) {

        case EIGHT:
        {
          iEntries = LUT_EIGHT_BIT_ENTRIES;
          iRatio   = 0;
        }
        break;
        case TEN:
        {
          iEntries  = LUT_TEN_BIT_ENTRIES;
          iRatio    = LUT_TEN_BIT_RATIO;
        }
        break;
        case TWELVE:
        {
          iEntries  = LUT_TWELVE_BIT_ENTRIES;
          iRatio    = LUT_TWELVE_BIT_RATIO;
        }
        break;
        case SIXTEEN:
        {
          iEntries  = LUT_SIXTEEN_BIT_ENTRIES;
          iRatio    = LUT_SIXTEEN_BIT_RATIO;
        }
        break;

      }

      //
      // Allocate buffers for channel data
      //
      RedLUT = reinterpret_cast<unsigned short *>(calloc(iEntries, 2));

      if (NULL != RedLUT) {

        GreenLUT = reinterpret_cast<unsigned short *>(calloc(iEntries, 2));

        if (NULL != GreenLUT) {

          BlueLUT = reinterpret_cast<unsigned short *>(calloc(iEntries, 2));

          if (NULL != BlueLUT) {

            //
            // Read the RED channel data
            //
            for (int n = 0; n < iEntries; n++) {

              iStream.read(&sTemp, 1);
              RedLUT[n] = (static_cast<unsigned short>(((sTemp << 8) & 0xFF00) | ((sTemp >> 8) & 0x00FF)) >> iRatio);

            }

            //
            // Read the GREEN channel data
            //
            for (n = 0; n < iEntries; n++) {

              iStream.read(&sTemp, 1);
              GreenLUT[n] = (static_cast<unsigned short>(((sTemp << 8) & 0xFF00) | ((sTemp >> 8) & 0x00FF)) >> iRatio);

            }

            //
            // Read the BLUE channel data
            //
            for (n = 0; n < iEntries; n++) {

              iStream.read(&sTemp, 1);
              BlueLUT[n] = (static_cast<unsigned short>(((sTemp << 8) & 0xFF00) | ((sTemp >> 8) & 0x00FF)) >> iRatio);

            }

            //
            // Done. All pixel values mapped.
            //
            iStream.close();
            r = true;

          }

        }

      }

    }

    return r;
  }

  void ResetLUTs(void) {

    if (NULL != RedLUT) {

      free(RedLUT);
      RedLUT = NULL;

    }

    if (NULL != GreenLUT) {

      free(GreenLUT);
      GreenLUT = NULL;

    }

    if (NULL != BlueLUT) {

      free(BlueLUT);
      BlueLUT = NULL;

    }

  }

  unsigned short *RedLUT;
  unsigned short *GreenLUT;
  unsigned short *BlueLUT;

protected:
  LUTDepth m_Depth;

};

#endif // !_LOOKUPTABLE_H
