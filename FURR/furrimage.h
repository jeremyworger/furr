#ifndef _FURRIMAGE_H
#define _FURRIMAGE_H

#include "stdafx.h"
#include "furrtypes.h"

/*
 * Constants
 */
#define FIC_FULL  0x00000000UL
#define FIC_RED   0x00000001UL
#define FIC_GREEN 0x00000002UL
#define FIC_BLUE  0x00000003UL

/*
 * FURRImage Class
 *
 * Pure virtual base class
 * that abstracts image data
 * from the rest of the application
 */
class FURRImage {

public:

  FURRImage() { }
  virtual ~FURRImage() { }

  virtual bool ReadImage(CTSTRING &tsImgFile) = 0;
  virtual bool PrepareForShoot(ADJUSTPARAMS &ap, ANNOTATION &an) = 0;
  virtual bool Resize(DWORD_PTR dwX, DWORD_PTR dwY) = 0;
  virtual DWORD_PTR Width(void) = 0;
  virtual DWORD_PTR Height(void) = 0;
  virtual BYTE *AllPixels(void) = 0;
  virtual BYTE *RedPixels(void) = 0;
  virtual BYTE *GreenPixels(void) = 0;
  virtual BYTE *BluePixels(void) = 0;
  virtual bool IsValid(void) = 0;

protected:
  virtual bool _Gamma(Image &imsg, double dblRed, double dblGreen, double dblBlue) = 0;
  virtual bool _Annotate(DWORD_PTR dwChan, const ANNOTATION &a) = 0;

};

#endif // !_FURRIMAGE_H