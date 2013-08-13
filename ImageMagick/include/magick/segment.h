/*
  ImageMagick Image Segment Methods.
*/
#ifndef _MAGICK_SEGMENT_H
#define _MAGICK_SEGMENT_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport MagickPixelPacket
  GetImageDynamicThreshold(const Image *,const double,const double,
    ExceptionInfo *);

extern MagickExport MagickBooleanType
  SegmentImage(Image *,const ColorspaceType,const MagickBooleanType,
    const double,const double);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
