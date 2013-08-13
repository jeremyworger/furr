/*
  ImageMagick String Methods.
*/
#ifndef _MAGICK_STRING_H_
#define _MAGICK_STRING_H_

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/error.h"

typedef struct _StringInfo
{
  char
    path[MaxTextExtent];

  unsigned char
    *datum;

  size_t
    length;

  unsigned long
    signature;
} StringInfo;

extern MagickExport char
  *AcquireString(const char *),
  *EscapeString(const char *,const char),
  *FileToString(const char *),
  **StringToArgv(const char *,int *),
  **StringToList(const char *),
  *StringInfoToString(const StringInfo *);

extern MagickExport double
  StringToDouble(const char *,const double);

extern MagickExport int
  FormatMagickString(char *,const size_t,const char *,...)
    __attribute__((format (printf,3,4))),
  LocaleCompare(const char *,const char *),
  LocaleNCompare(const char *,const char *,const size_t);

extern MagickExport MagickBooleanType
  CloneString(char **,const char *),
  CompareStringInfo(const StringInfo *,const StringInfo *),
  ConcatenateString(char **,const char *),
  SubstituteString(char **,const char *,const char *);

extern MagickExport size_t
  ConcatenateMagickString(char *,const char *,const size_t),
  CopyMagickString(char *,const char *,const size_t);

extern MagickExport StringInfo
  *AcquireStringInfo(const size_t),
  *CloneStringInfo(const StringInfo *),
  *FileToStringInfo(const char *),
  *SplitStringInfo(StringInfo *,const size_t),
  *StringToStringInfo(const char *);

extern MagickExport void
  ConcatenateStringInfo(StringInfo *,const StringInfo *),
  DestroyString(char *),
  DestroyStringInfo(StringInfo *),
  DestroyStringList(char **),
  LocaleLower(char *),
  LocaleUpper(char *),
  PrintStringInfo(const char *,const StringInfo *),
  ResetStringInfo(StringInfo *),
  SetStringInfoDatum(StringInfo *,const unsigned char *),
  SetStringInfo(StringInfo *,const StringInfo *),
  SetStringInfoLength(StringInfo *,const size_t),
  StripString(char *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
