/*
  ImageMagick Cache Methods.
*/
#ifndef _MAGICK_CACHE_H
#define _MAGICK_CACHE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/blob.h"

typedef enum
{
  UndefinedCache,
  MemoryCache,
  MapCache,
  DiskCache
} CacheType;

typedef enum
{
  UndefinedVirtualPixelMethod,
  ConstantVirtualPixelMethod,
  EdgeVirtualPixelMethod,
  MirrorVirtualPixelMethod,
  TileVirtualPixelMethod
} VirtualPixelMethod;

typedef void
  *Cache;

typedef IndexPacket
  *(*GetIndexesFromHandler)(const Image *);

typedef MagickBooleanType
  (*SyncPixelHandler)(Image *);

typedef const PixelPacket
  *(*AcquirePixelHandler)(const Image *,const long,const long,
    const unsigned long,const unsigned long,ExceptionInfo *);

typedef PixelPacket
  (*AcquireOnePixelFromHandler)(const Image *,const long,const long,
    ExceptionInfo *);

typedef PixelPacket
  (*GetOnePixelFromHandler)(Image *,const long,const long);

typedef PixelPacket
  *(*GetPixelHandler)(Image *,const long,const long,const unsigned long,
    const unsigned long);

typedef PixelPacket
  *(*GetPixelsFromHandler)(const Image *);

typedef PixelPacket
  *(*SetPixelHandler)(Image *,const long,const long,const unsigned long,
    const unsigned long);

typedef void
  (*DestroyPixelHandler)(Image *);

typedef struct _CacheMethods
{
  AcquireOnePixelFromHandler
    acquire_one_pixel_from_handler;

  AcquirePixelHandler
    acquire_pixel_handler;

  DestroyPixelHandler
    destroy_pixel_handler;

  GetIndexesFromHandler
    get_indexes_from_handler;

  GetOnePixelFromHandler
    get_one_pixel_from_handler;

  GetPixelHandler
    get_pixel_handler;

  GetPixelsFromHandler
    get_pixels_from_handler;

  SetPixelHandler
    set_pixel_handler;

  SyncPixelHandler
    sync_pixel_handler;
} CacheMethods;

typedef struct _NexusInfo NexusInfo;

typedef struct _CacheInfo
{
  unsigned long
    id;

  NexusInfo
    *nexus_info;

  unsigned long
    number_views;

  ClassType
    storage_class;

  ColorspaceType
    colorspace;

  CacheType
    type;

  MagickBooleanType
    mapped;

  unsigned long
    columns,
    rows;

  MagickOffsetType
    offset;

  MagickSizeType
    length;

  PixelPacket
    *pixels;

  IndexPacket
    *indexes;

  VirtualPixelMethod
    virtual_pixel_method;

  PixelPacket
    virtual_pixel;

  int
    file;

  char
    filename[MaxTextExtent],
    cache_filename[MaxTextExtent];

  CacheMethods
    methods;

  MagickBooleanType
    debug;

  long
    reference_count;

  void
    *semaphore;

  unsigned long
    signature;
} CacheInfo;

extern MagickExport Cache
  ReferenceCache(Cache);

extern MagickExport ClassType
  GetCacheClass(const Cache);

extern MagickExport ColorspaceType
  GetCacheColorspace(const Cache);

extern MagickExport const PixelPacket
  *AcquireCacheNexus(const Image *,const long,const long,const unsigned long,
    const unsigned long,const unsigned long,ExceptionInfo *);

extern MagickExport MagickSizeType
  GetPixelCacheArea(const Image *);

extern MagickExport IndexPacket
  *GetNexusIndexes(const Cache,const unsigned long);

extern MagickExport MagickBooleanType
  OpenCache(Image *,const MapMode),
  PersistCache(Image *,const char *,const MagickBooleanType,MagickOffsetType *,
    ExceptionInfo *),
  SetImageVirtualPixelMethod(const Image *,const VirtualPixelMethod),
  SyncCacheNexus(Image *,const unsigned long);

extern MagickExport PixelPacket
  *GetCacheNexus(Image *,const long,const long,const unsigned long,
    const unsigned long,const unsigned long),
  *GetNexusPixels(const Cache,const unsigned long),
  *SetCacheNexus(Image *,const long,const long,const unsigned long,
    const unsigned long,const unsigned long);

extern MagickExport unsigned long
  GetNexus(Cache);

extern MagickExport VirtualPixelMethod
  GetImageVirtualPixelMethod(const Image *);

extern MagickExport void
  ClonePixelCacheMethods(Cache,const Cache),
  DestroyCacheInfo(Cache),
  DestroyCacheNexus(Cache,const unsigned long),
  GetCacheInfo(Cache *),
  SetPixelCacheMethods(Cache,AcquirePixelHandler,GetPixelHandler,
    SetPixelHandler,SyncPixelHandler,GetPixelsFromHandler,GetIndexesFromHandler,
    AcquireOnePixelFromHandler,GetOnePixelFromHandler,DestroyPixelHandler);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
