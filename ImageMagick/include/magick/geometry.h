/*
  ImageMagick image geometry methods.
*/
#ifndef _MAGICK_GEOMETRY_H
#define _MAGICK_GEOMETRY_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef enum
{
#undef NoValue
  NoValue = 0x0000,
#undef XValue
  PsiValue = 0x0001,
  XValue = 0x0001,
#undef YValue
  XiValue = 0x0002,
  YValue = 0x0002,
#undef WidthValue
  RhoValue = 0x0004,
  WidthValue = 0x0004,
#undef HeightValue
  SigmaValue = 0x0008,
  HeightValue = 0x0008,
  ChiValue = 0x0010,
#undef AllValues
  AllValues = 0x7fffffff,
  XiNegative = 0x0020,
#undef XNegative
  XNegative = 0x0020,
  PsiNegative = 0x0040,
#undef YNegative
  YNegative = 0x0040,
  ChiNegative = 0x0080,
  PercentValue = 0x1000,
  AspectValue = 0x2000,
  LessValue = 0x4000,
  GreaterValue = 0x8000,
  AreaValue = 0x10000
} GeometryFlags;

typedef struct _GeometryInfo
{
  double
    rho,
    sigma,
    xi,
    psi,
    chi;
} GeometryInfo;

extern MagickExport char
  *GetPageGeometry(const char *);

extern MagickExport MagickBooleanType
  IsGeometry(const char *),
  IsSceneGeometry(const char *,const MagickBooleanType);

extern MagickExport MagickStatusType
  GetGeometry(const char *,long *,long *,unsigned long *,unsigned long *),
  ParseAbsoluteGeometry(const char *,RectangleInfo *),
  ParseGeometry(const char *,GeometryInfo *),
  ParseGravityGeometry(Image *,const char *,RectangleInfo *),
  ParseMetaGeometry(const char *,long *,long *,unsigned long *,unsigned long *),
  ParsePageGeometry(Image *,const char *,RectangleInfo *),
  ParseSizeGeometry(Image *,const char *,RectangleInfo *);

extern MagickExport void
  SetGeometry(const Image *,RectangleInfo *),
  SetGeometryInfo(GeometryInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
