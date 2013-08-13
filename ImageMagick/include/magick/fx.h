/*
  ImageMagick image F/X methods.
*/
#ifndef _MAGICK_FX_H
#define _MAGICK_FX_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef enum
{
  UndefinedEvaluateOperator,
  AddEvaluateOperator,
  AndEvaluateOperator,
  DivideEvaluateOperator,
  LeftShiftEvaluateOperator,
  MaxEvaluateOperator,
  MinEvaluateOperator,
  MultiplyEvaluateOperator,
  OrEvaluateOperator,
  RightShiftEvaluateOperator,
  SetEvaluateOperator,
  SubtractEvaluateOperator,
  XorEvaluateOperator
} MagickEvaluateOperator;

extern MagickExport Image
  *CharcoalImage(const Image *,const double,const double,ExceptionInfo *),
  *ColorizeImage(const Image *,const char *,const PixelPacket,ExceptionInfo *),
  *ConvolveImage(const Image *,const unsigned long,const double *,
    ExceptionInfo *),
  *ConvolveImageChannel(const Image *,ChannelType,const unsigned long,
    const double *,ExceptionInfo *),
  *EdgeImage(const Image *,const double,ExceptionInfo *),
  *FxImage(const Image *,const char *,ExceptionInfo *),
  *FxImageChannel(const Image *,const ChannelType,const char *,ExceptionInfo *),
  *ImplodeImage(const Image *,const double,ExceptionInfo *),
  *MorphImages(const Image *,const unsigned long,ExceptionInfo *),
  *OilPaintImage(const Image *,const double,ExceptionInfo *),
  *SteganoImage(const Image *,const Image *,ExceptionInfo *),
  *StereoImage(const Image *,const Image *,ExceptionInfo *),
  *SwirlImage(const Image *,double,ExceptionInfo *),
  *TintImage(const Image *,const char *,const PixelPacket,ExceptionInfo *),
  *WaveImage(const Image *,const double,const double,ExceptionInfo *);

extern MagickExport MagickBooleanType
  EvaluateImage(Image *,const MagickEvaluateOperator,const double,
    ExceptionInfo *),
  EvaluateImageChannel(Image *,const ChannelType,const MagickEvaluateOperator,
    const double,ExceptionInfo *),
  SolarizeImage(Image *,const double);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
