/*
  ImageMagick Magic Methods.
*/
#ifndef _MAGICK_MAGIC_H
#define _MAGICK_MAGIC_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef struct _MagicInfo
{
  char
    *path,
    *name,
    *target;

  unsigned char
    *magic;

  size_t
    length;

  ssize_t
    offset;

  MagickBooleanType
    stealth;

  struct _MagicInfo
    *previous,
    *next;  /* deprecated, use GetMagicInfoList() */

  unsigned long
    signature;
} MagicInfo;

extern MagickExport char
  **GetMagicList(const char *,unsigned long *);

extern MagickExport const char
  *GetMagicName(const MagicInfo *);

extern MagickExport MagickBooleanType
  ListMagicInfo(FILE *,ExceptionInfo *);

extern MagickExport const MagicInfo
  *GetMagicInfo(const unsigned char *,const size_t,ExceptionInfo *),
  **GetMagicInfoList(const char *,unsigned long *);

extern MagickExport void
  DestroyMagicList(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
