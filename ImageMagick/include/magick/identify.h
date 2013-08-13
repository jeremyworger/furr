/*
  Image Identify Command Method.
*/
#ifndef _MAGICK_IDENTIFY_H
#define _MAGICK_IDENTIFY_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport MagickBooleanType
  IdentifyImageCommand(ImageInfo *,int,char **,char **,ExceptionInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
