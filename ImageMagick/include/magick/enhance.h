/*
  ImageMagick Image Enhance Methods.
*/
#ifndef _MAGICK_ENHANCE_H
#define _MAGICK_ENHANCE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport MagickBooleanType
  ContrastImage(Image *,const MagickBooleanType),
  EqualizeImage(Image *image),
  GammaImage(Image *,const char *),
  GammaImageChannel(Image *,const ChannelType,const double),
  LevelImage(Image *,const char *),
  LevelImageChannel(Image *,const ChannelType,const double,const double,
    const double),
  ModulateImage(Image *,const char *),
  NegateImage(Image *,const MagickBooleanType),
  NegateImageChannel(Image *,const ChannelType,const MagickBooleanType),
  NormalizeImage(Image *),
  NormalizeImageChannel(Image *,const ChannelType);

extern MagickExport Image
  *EnhanceImage(const Image *,ExceptionInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
