/*
  ImageMagick Image Type Methods.
*/
#ifndef _MAGICK_TYPE_H
#define _MAGICK_TYPE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef enum
{
  UndefinedStretch,
  NormalStretch,
  UltraCondensedStretch,
  ExtraCondensedStretch,
  CondensedStretch,
  SemiCondensedStretch,
  SemiExpandedStretch,
  ExpandedStretch,
  ExtraExpandedStretch,
  UltraExpandedStretch,
  AnyStretch
} StretchType;

typedef enum
{
  UndefinedStyle,
  NormalStyle,
  ItalicStyle,
  ObliqueStyle,
  AnyStyle
} StyleType;

typedef struct _TypeInfo
{
  unsigned long
    face;

  char
    *path,
    *name,
    *description,
    *family;

  StyleType
    style;

  StretchType
    stretch;

  unsigned long
    weight;

  char
    *encoding,
    *foundry,
    *format,
    *metrics,
    *glyphs;

  MagickBooleanType
    stealth;

  struct _TypeInfo
    *previous,
    *next;  /* deprecated, use GetTypeInfoList() */

  unsigned long
    signature;
} TypeInfo;

extern MagickExport char
  **GetTypeList(const char *,unsigned long *);

extern MagickExport MagickBooleanType
  ListTypeInfo(FILE *,ExceptionInfo *);

extern MagickExport const TypeInfo
  *GetTypeInfo(const char *,ExceptionInfo *),
  *GetTypeInfoByFamily(const char *,const StyleType,const StretchType,
    const unsigned long,ExceptionInfo *),
  **GetTypeInfoList(const char *,unsigned long *);

MagickExport void
  DestroyTypeList(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
