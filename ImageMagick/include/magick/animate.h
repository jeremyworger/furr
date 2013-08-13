/*
  Methods to Interactively Animate an Image Sequence.
*/
#ifndef _MAGICK_ANIMATE_H
#define _MAGICK_ANIMATE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if defined(HasX11)

#include "magick/xwindow.h"

extern MagickExport Image
  *XAnimateImages(Display *,XResourceInfo *,char **,const int,Image *);

extern MagickExport MagickBooleanType
  AnimateImageCommand(ImageInfo *,int,char **,char **,ExceptionInfo *),
  AnimateImages(const ImageInfo *,Image *);

extern MagickExport void
  XAnimateBackgroundImage(Display *,XResourceInfo *,Image *);

#endif

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
