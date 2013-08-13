#ifndef _IMAGELOADERS_H
#define _IMAGELOADERS_H

/*
 * PFNLOADIMAGE Typedef
 *
 * Defines the signature for
 * an image-loading routine
 * used by FURR.
 */
typedef bool (*PFNLOADIMAGE)(CTSTRING &, class FURRImage **);

/*
 * Function signatures for known image formats
 */
bool ImageMagickLoader(CTSTRING &tsFile, FURRImage **ppImg);
bool PICTLoader(CTSTRING &tsFile, FURRImage **ppImg);
bool TGALoader(CTSTRING &tsFile, FURRImage **ppImg);
bool EPSLoader(CTSTRING &tsFile, FURRImage **ppImg);

#endif // !_IMAGELOADERS_H
