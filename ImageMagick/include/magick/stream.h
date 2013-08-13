/*
  ImageMagick Image Stream Methods.
*/
#ifndef _MAGICK_STREAM_H
#define _MAGICK_STREAM_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport Image
  *ReadStream(const ImageInfo *,StreamHandler,ExceptionInfo *);

extern MagickExport MagickBooleanType
  WriteStream(const ImageInfo *,Image *,StreamHandler);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
