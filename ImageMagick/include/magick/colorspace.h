/*
  ImageMagick Image Colorspace Methods.
*/
#ifndef _MAGICK_COLORSPACE_H
#define _MAGICK_COLORSPACE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport MagickBooleanType
  RGBTransformImage(Image *,const ColorspaceType),
  SetImageColorspace(Image *,const ColorspaceType),
  TransformRGBImage(Image *,const ColorspaceType);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
