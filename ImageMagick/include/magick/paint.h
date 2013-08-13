/*
  ImageMagick Image Paint Methods.
*/
#ifndef _MAGICK_PAINT_H
#define _MAGICK_PAINT_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/draw.h"

extern MagickExport MagickBooleanType
  ColorFloodfillImage(Image *,const DrawInfo *,const PixelPacket,const long,
    const long,const PaintMethod),
  MatteFloodfillImage(Image *,const PixelPacket,const Quantum,const long,
    const long,const PaintMethod),
  OpaqueImage(Image *,const PixelPacket,const PixelPacket),
  TransparentImage(Image *,const PixelPacket,const Quantum);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
