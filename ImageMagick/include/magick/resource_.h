/*
  ImageMagick Resource Methods.
*/
#ifndef _MAGICK_RESOURCE_H
#define _MAGICK_RESOURCE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef enum
{
  UndefinedResource,
  AreaResource,
  DiskResource,
  FileResource,
  MapResource,
  MemoryResource
} ResourceType;

extern MagickExport int
  AcquireUniqueFileResource(char *);

extern MagickExport MagickBooleanType
  AcquireMagickResource(const ResourceType,const MagickSizeType),
  RelinquishUniqueFileResource(const char *),
  ListMagickResourceInfo(FILE *file,ExceptionInfo *exception),
  SetMagickResourceLimit(const ResourceType,const unsigned long);

extern MagickExport unsigned long
  GetMagickResource(const ResourceType);

extern MagickExport void
  AsynchronousDestroyMagickResources(void),
  DestroyMagickResources(void),
  InitializeMagickResources(void),
  RelinquishMagickResource(const ResourceType,const MagickSizeType);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
