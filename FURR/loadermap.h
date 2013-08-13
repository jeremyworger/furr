#ifndef _LOADERMAP_H
#define _LOADERMAP_H

#include "stdafx.h"
#include "imageloaders.h"
#include "furrmaindlg.h"
#include "furrimage.h"
#include <map>

/*
 * Typedef for an STL map
 * TSTRING -> loader function
 */
typedef std::map<TSTRING, PFNLOADIMAGE> LOADERMAP, *PLOADERMAP;

/*
 * LoaderMap Class
 *
 * Provides a hash map for
 * mapping file extensions
 * to loader functions.
 */
class LoaderMap {

public:

  LoaderMap() { }

  ~LoaderMap() { }

  void Rewind(void) { m_it = m_Map.begin(); }

  bool LoadImage(CTSTRING &tsImg, FURRImage **ppImg) {

    bool r = false;

    if (NULL != ppImg) {

      Rewind();

      TSTRING tsExt;

      if (GetFileExtension(tsImg, tsExt)) {

        StrToUpper(tsExt);

        if (m_Map.end() != (m_it = m_Map.find(tsExt))) {

          if (NULL != (*m_it).second) {

            r = (*m_it).second(tsImg, ppImg);

          }

        }

      }

    }

    return r;
  }

  void InitKnownFormats(void) {

    // TODO: Add more known image formats here

    // TIFFs
    m_Map[_T("TIFF")] = ImageMagickLoader;
    m_Map[_T("TIF")]  = ImageMagickLoader;

    // PICTs
    m_Map[_T("PICT")] = PICTLoader;

    // Targas
    m_Map[_T("TGA")]  = TGALoader;

    // EPSs
    m_Map[_T("EPS")]  = EPSLoader;

    // Cineons
    m_Map[_T("CIN")]  = ImageMagickLoader;

    // RGBs
    m_Map[_T("RGB")]  = ImageMagickLoader;

  }

private:
  LOADERMAP           m_Map;
  LOADERMAP::iterator m_it;

};

#endif // !_LOADERMAP_H