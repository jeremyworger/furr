#include "stdafx.h"
#include "furrimimage.h"

/*
 * IMLoader()
 *
 * Handles loading of all image formats
 * supported by ImageMagick.
 */
bool ImageMagickLoader(CTSTRING &tsFile, FURRImage **ppImg) {

  bool r = false;

  FURRIMImage *pImg = new FURRIMImage;

  if (NULL != pImg) {

    if (pImg->ReadImage(tsFile)) {

      *ppImg = pImg;
      r      = true;

    } else {

      delete pImg;

    }

  }

  return r;

}

/*
 * PICTLoader()
 *
 * Handles loading of PICT images
 */
bool PICTLoader(CTSTRING &tsFile, FURRImage **ppImg) {

  bool r = false;

  *ppImg = NULL;

  return r;

}

/*
 * TGALoader()
 *
 * Handles loading of Targa images
 */
bool TGALoader(CTSTRING &tsFile, FURRImage **ppImg) {

  bool r = false;

  *ppImg = NULL;

  return r;

}

/*
 * EPSLoader()
 *
 * Handles loading of EPS images
 */
bool EPSLoader(CTSTRING &tsFile, FURRImage **ppImg) {

  bool r = false;

  *ppImg = NULL;

  return r;

}
