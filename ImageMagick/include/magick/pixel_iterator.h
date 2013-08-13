/*
  ImageMagick Pixel Iterator API.
*/
#ifndef _MAGICK_PIXEL_ITERATOR_H
#define _MAGICK_PIXEL_ITERATOR_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick_wand.h"
#include "pixel_wand.h"

typedef struct _PixelIterator
  PixelIterator;

extern WandExport char
  *PixelIteratorGetException(const PixelIterator *,ExceptionType *);

extern WandExport MagickBooleanType
  PixelSetIteratorRow(PixelIterator *,const long),
  PixelSyncIterator(PixelIterator *);

extern WandExport PixelIterator
  *NewPixelIterator(MagickWand *),
  *NewPixelRegionIterator(MagickWand *,const long,const long,
    const unsigned long,const unsigned long);

extern WandExport PixelWand
  **PixelGetNextRow(PixelIterator *);

extern WandExport void
  DestroyPixelIterator(PixelIterator *),
  PixelResetIterator(PixelIterator *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
