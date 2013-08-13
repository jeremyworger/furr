/*
  Deprecated methods.
*/
#ifndef _MAGICK_DEPRECATE_H
#define _MAGICK_DEPRECATE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/blob.h"

#if !defined(__attribute__)
#  define __attribute__(x) /*nothing*/
#endif

#define Downscale(quantum)  ScaleQuantumToChar(quantum)
#define False  0
#define Intensity(color)  PixelIntensityToQuantum(color)
#define LiberateUniqueFileResource(resource) \
  RelinquishUniqueFileResource(resource)
#define LiberateMagickResource(resource)  RelinquishMagickResource(resource)
#define LiberateSemaphore(semaphore)  RelinquishSemaphore(semaphore)
#define RunlengthEncodedCompression  RLECompression
#define True  1
#define Upscale(value)  ScaleCharToQuantum(value)
#define XDownscale(value)  ScaleShortToQuantum(value)
#define XUpscale(quantum)  ScaleQuantumToShort(quantum)

typedef struct _DoublePixelPacket
{
  double
    red,
    green,
    blue,
    opacity,
    index;
} DoublePixelPacket;

#if !defined(__WINDOWS__)
#if (SIZEOF_LONG_LONG == 8)
typedef long long ExtendedSignedIntegralType;
typedef unsigned long long ExtendedUnsignedIntegralType;
#else
typedef long ExtendedSignedIntegralType;
typedef unsigned long ExtendedUnsignedIntegralType;
#endif
#else
typedef __int64 ExtendedSignedIntegralType;
typedef unsigned __int64 ExtendedUnsignedIntegralType;
#endif
#if defined(HAVE_LONG_DOUBLE)
typedef long double ExtendedRationalType;
#else
typedef double ExtendedRationalType;
#endif

extern MagickExport char
  *AllocateString(const char *),
  *PostscriptGeometry(const char *);

extern MagickExport MagickOffsetType
  SizeBlob(const Image *image);

extern MagickExport Image
  *GetImageList(const Image *,const long,ExceptionInfo *),
  *GetNextImage(const Image *),
  *GetPreviousImage(const Image *),
  *PopImageList(Image **),
  *ShiftImageList(Image **),
  *SpliceImageList(Image *,const long,const unsigned long,const Image *,
    ExceptionInfo *);

extern MagickExport long
  GetImageListIndex(const Image *);

extern MagickExport IndexPacket
  ValidateColormapIndex(Image *,const unsigned long);

extern MagickExport int
  GetImageGeometry(Image *,const char *,const unsigned int,RectangleInfo *),
  ParseImageGeometry(const char *,long *,long *,unsigned long *,
    unsigned long *);

extern MagickExport unsigned int
  ChannelImage(Image *,const ChannelType),
  ChannelThresholdImage(Image *,const char *),
  DispatchImage(const Image *,const long,const long,const unsigned long,
    const unsigned long,const char *,const StorageType,void *,ExceptionInfo *),
  FuzzyColorMatch(const PixelPacket *,const PixelPacket *,const double),
  GetNumberScenes(const Image *),
  GetMagickGeometry(const char *,long *,long *,unsigned long *,unsigned long *),
  IsSubimage(const char *,const unsigned int),
  PushImageList(Image **,const Image *,ExceptionInfo *),
  QuantizationError(Image *),
  RandomChannelThresholdImage(Image *,const char *,const char *,
    ExceptionInfo *),
  SetImageList(Image **,const Image *,const long,ExceptionInfo *),
  TransformColorspace(Image *,const ColorspaceType),
  ThresholdImage(Image *,const double),
  ThresholdImageChannel(Image *,const char *),
  UnshiftImageList(Image **,const Image *,ExceptionInfo *);

extern MagickExport unsigned long
  GetImageListSize(const Image *);

extern MagickExport void
  *AcquireMemory(const size_t),
  *CloneMemory(void *,const void *,const size_t),
  DeleteImageFromList(Image **),
  DestroyBlobInfo(BlobInfo *),
  DestroyImages(Image *),
  *GetConfigureBlob(const char *,char *,size_t *,ExceptionInfo *),
  IdentityAffine(AffineMatrix *),
  LiberateMemory(void **),
  LiberateSemaphoreInfo(void **),
  FormatString(char *,const char *,...) __attribute__((format (printf,2,3))),
  ReacquireMemory(void **,const size_t),
  SetCacheThreshold(const unsigned long),
  Strip(char *),
  TemporaryFilename(char *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
