/*
  Image Convert Command Methods.
*/
#ifndef _MAGICK_CONVERT_H
#define _MAGICK_CONVERT_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport MagickBooleanType
  ConvertImageCommand(ImageInfo *,int,char **,char **,ExceptionInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
