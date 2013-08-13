/*
  Methods to Interactively Display and Edit an Image.
*/
#ifndef _MAGICK_DISPLAY_H
#define _MAGICK_DISPLAY_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if defined(HasX11)

#include "magick/xwindow.h"

extern MagickExport Image
  *XDisplayImage(Display *,XResourceInfo *,char **,int,Image **,
    unsigned long *);

extern MagickExport MagickBooleanType
  DisplayImageCommand(ImageInfo *,int,char **,char **,ExceptionInfo *),
  DisplayImages(const ImageInfo *,Image *),
  XDisplayBackgroundImage(Display *,XResourceInfo *,Image *);

#endif

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
