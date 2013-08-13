/*
  Configure methods.
*/
#ifndef _MAGICK_CONFIGURE_H
#define _MAGICK_CONFIGURE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/hashmap.h"

typedef struct _ConfigureInfo
{
  char
    *path,
    *name,
    *value;
                                                                                
  MagickBooleanType
    stealth;
                                                                                
  struct _ConfigureInfo
    *previous,
    *next;  /* deprecated, use GetConfigureInfoList() */

  unsigned long
    signature;
} ConfigureInfo;

extern MagickExport char
  **GetConfigureList(const char *,unsigned long *);

extern MagickExport const char
  *GetConfigureValue(const ConfigureInfo *);

extern MagickExport const ConfigureInfo
  *GetConfigureInfo(const char *,ExceptionInfo *),
  **GetConfigureInfoList(const char *,unsigned long *);

extern MagickExport LinkedListInfo
  *GetConfigurePaths(const char *,ExceptionInfo *),
  *GetConfigureOptions(const char *,ExceptionInfo *);

extern MagickExport MagickBooleanType
  ListConfigureInfo(FILE *,ExceptionInfo *);

extern MagickExport void
  DestroyConfigureList(void),
  DestroyConfigureOptions(LinkedListInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
