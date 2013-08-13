/*
  ImageMagick Binary Large OBjects Methods.
*/
#ifndef _MAGICK_BLOB_H
#define _MAGICK_BLOB_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/image.h"
#include "magick/stream.h"

typedef enum
{
  UndefinedBlobMode,
  ReadBlobMode,
  ReadBinaryBlobMode,
  WriteBlobMode,
  WriteBinaryBlobMode,
  IOBinaryBlobMode
} BlobMode;

typedef enum
{
  ReadMode,
  WriteMode,
  IOMode
} MapMode;

typedef int
  *(*BlobFifo)(const Image *,const void *,const size_t);

typedef struct _BlobInfo
  BlobInfo;

extern MagickExport BlobInfo
  *CloneBlobInfo(const BlobInfo *),
  *ReferenceBlob(BlobInfo *);

extern MagickExport char
  *ReadBlobString(Image *,char *);

extern MagickExport FILE
  *GetBlobFileHandle(const Image *);

extern MagickExport Image
  *BlobToImage(const ImageInfo *,const void *,const size_t,ExceptionInfo *),
  *PingBlob(const ImageInfo *,const void *,const size_t,ExceptionInfo *);

extern MagickExport int
  EOFBlob(const Image *),
  ReadBlobByte(Image *),
  SyncBlob(Image *);

extern MagickExport  MagickBooleanType
  BlobToFile(char *,const void *,const size_t,ExceptionInfo *),
  GetBlobError(const Image *),
  ImageToFile(Image *,char *,ExceptionInfo *),
  IsBlobExempt(const Image *),
  IsBlobSeekable(const Image *),
  IsBlobTemporary(const Image *),
  OpenBlob(const ImageInfo *,Image *,const BlobMode,ExceptionInfo *),
  UnmapBlob(void *,const size_t);

extern MagickExport MagickOffsetType
  SeekBlob(Image *,const MagickOffsetType,const int),
  TellBlob(const Image *image);

extern MagickExport MagickSizeType
  GetBlobSize(const Image *image);

extern MagickExport ssize_t
  ReadBlob(Image *,const size_t,unsigned char *),
  WriteBlob(Image *,const size_t,const unsigned char *),
  WriteBlobByte(Image *,const unsigned char),
  WriteBlobLSBLong(Image *,const unsigned long),
  WriteBlobLSBShort(Image *,const unsigned short),
  WriteBlobMSBLong(Image *,const unsigned long),
  WriteBlobMSBShort(Image *,const unsigned short),
  WriteBlobString(Image *,const char *);

extern MagickExport StreamHandler
  GetBlobStreamHandler(const Image *);

extern MagickExport unsigned char
  *FileToBlob(const char *,size_t *,ExceptionInfo *),
  *GetBlobStreamData(const Image *),
  *ImageToBlob(const ImageInfo *,Image *,size_t *,ExceptionInfo *),
  *MapBlob(int,const MapMode,const MagickOffsetType,const size_t);

extern MagickExport unsigned long
  ReadBlobLSBLong(Image *),
  ReadBlobMSBLong(Image *);

extern MagickExport unsigned short
  ReadBlobLSBShort(Image *),
  ReadBlobMSBShort(Image *);

extern MagickExport void
  AttachBlob(BlobInfo *,const void *,const size_t),
  CloseBlob(Image *),
  DestroyBlob(Image *),
  DetachBlob(BlobInfo *),
  GetBlobInfo(BlobInfo *),
  MSBOrderLong(unsigned char *,const size_t),
  MSBOrderShort(unsigned char *,const size_t),
  SetBlobExempt(Image *,const MagickBooleanType);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
