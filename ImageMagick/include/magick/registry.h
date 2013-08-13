/*
  Magick Registry Methods.
*/
#ifndef _MAGICK_REGISTRY_H
#define _MAGICK_REGISTRY_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef enum
{
  UndefinedRegistryType,
  ImageRegistryType,
  ImageInfoRegistryType
} RegistryType;

extern MagickExport Image
  *GetImageFromMagickRegistry(const char *,long *id,ExceptionInfo *);

extern MagickExport long
  SetMagickRegistry(const RegistryType,const void *,const size_t,
    ExceptionInfo *);

extern MagickExport MagickBooleanType
  DeleteMagickRegistry(const long);

extern MagickExport void
  DestroyMagickRegistry(void),
  *GetMagickRegistry(const long,RegistryType *,size_t *,ExceptionInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
