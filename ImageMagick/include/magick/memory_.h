/*
  ImageMagick Image Memory Methods.
*/
#ifndef _MAGICK_MEMORY_H
#define _MAGICK_MEMORY_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport void
  *AcquireMagickMemory(const size_t),
  *CopyMagickMemory(void *,const void *,const size_t),
  DestroyMagickMemory(void),
  *RelinquishMagickMemory(void *),
  *ResetMagickMemory(void *,int,const size_t),
  *ResizeMagickMemory(void *,const size_t);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
