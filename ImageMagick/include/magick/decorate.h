/*
  ImageMagick Image Decoration Methods.
*/
#ifndef _MAGICK_DECORATE_H
#define _MAGICK_DECORATE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef struct _FrameInfo
{
  unsigned long
    width,
    height;

  long
    x,
    y,
    inner_bevel,
    outer_bevel;
} FrameInfo;

extern MagickExport Image
  *BorderImage(const Image *,const RectangleInfo *,ExceptionInfo *),
  *FrameImage(const Image *,const FrameInfo *,ExceptionInfo *);

extern MagickExport MagickBooleanType
  RaiseImage(Image *,const RectangleInfo *,const MagickBooleanType);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
