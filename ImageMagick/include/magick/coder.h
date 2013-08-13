/*
  ImageMagick Image Coder Methods.
*/
#ifndef _MAGICK_CODER_H
#define _MAGICK_CODER_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef struct _CoderInfo
{
  char
    *path,
    *magick,
    *name;
                                                                                
  MagickBooleanType
    stealth;
                                                                                
  struct _CoderInfo
    *previous,
    *next;  /* deprecated, use GetCoderInfoList() */

  unsigned long
    signature;
} CoderInfo;

extern MagickExport char
  **GetCoderList(const char *,unsigned long *);

extern MagickExport const CoderInfo
  *GetCoderInfo(const char *,ExceptionInfo *),
  **GetCoderInfoList(const char *,unsigned long *);

extern MagickExport MagickBooleanType
  ListCoderInfo(FILE *,ExceptionInfo *);

MagickExport void
  DestroyCoderList(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
