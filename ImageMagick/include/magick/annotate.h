/*
  ImageMagick image annotate methods.
*/
#ifndef _MAGICK_ANNOTATE_H
#define _MAGICK_ANNOTATE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/draw.h"

extern MagickExport MagickBooleanType
  AnnotateImage(Image *,const DrawInfo *),
  GetTypeMetrics(Image *,const DrawInfo *,TypeMetric *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
