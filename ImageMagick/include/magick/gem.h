/*
  Graphic Gems - Graphic Support Methods.
*/
#ifndef _MAGICK_GEM_H
#define _MAGICK_GEM_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport double
  ExpandAffine(const AffineMatrix *);

extern MagickExport PixelPacket
  InterpolateColor(const Image *,const double,const double,ExceptionInfo *);

extern MagickExport unsigned long
  GetOptimalKernelWidth(const double,const double),
  GetOptimalKernelWidth1D(const double,const double),
  GetOptimalKernelWidth2D(const double,const double);

extern MagickExport void
  Contrast(const int,Quantum *,Quantum *,Quantum *),
  HSBTransform(const double,const double,const double,Quantum *,Quantum *,
    Quantum *),
  HSLTransform(const double,const double,const double,Quantum *,Quantum *,
    Quantum *),
  HWBTransform(const double,const double,const double,Quantum *,Quantum *,
    Quantum *),
  Hull(const long,const long,const unsigned long,const unsigned long,Quantum *,
    Quantum *,const int),
  Modulate(const double,const double,const double,Quantum *,Quantum *,
    Quantum *),
  TransformHSB(const Quantum,const Quantum,const Quantum,double *,double *,
    double *),
  TransformHSL(const Quantum,const Quantum,const Quantum,double *,double *,
    double *),
  TransformHWB(const Quantum,const Quantum,const Quantum,double *,double *,
    double *),
  Upsample(const unsigned long,const unsigned long,const unsigned long,
    unsigned char *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
