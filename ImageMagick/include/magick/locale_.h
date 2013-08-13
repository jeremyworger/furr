/*
  Locale methods.
*/
#ifndef _MAGICK_LOCALE_H
#define _MAGICK_LOCALE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/hashmap.h"

typedef struct _LocaleInfo
{
  char
    *path,
    *tag,
    *message;
                                                                                
  MagickBooleanType
    stealth;
                                                                                
  struct _LocaleInfo
    *previous,
    *next;  /* deprecated, use GetLocaleInfoList() */

  unsigned long
    signature;
} LocaleInfo;

extern MagickExport char
  **GetLocaleList(const char *,unsigned long *);

extern MagickExport const char
  *GetLocaleMessage(const char *);

extern MagickExport const LocaleInfo
  *GetLocaleInfo_(const char *,ExceptionInfo *),
  **GetLocaleInfoList(const char *,unsigned long *);

extern MagickExport LinkedListInfo
  *GetLocaleOptions(const char *,ExceptionInfo *);

extern MagickExport MagickBooleanType
  ListLocaleInfo(FILE *,ExceptionInfo *);

extern MagickExport void
  DestroyLocaleList(void),
  DestroyLocaleOptions(LinkedListInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
