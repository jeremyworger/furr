/*
  ImageMagick image constitution methods.
*/
#ifndef _MAGICK_CONSTITUTE_H
#define _MAGICK_CONSTITUTE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef enum
{
  UndefinedQuantum,
  AlphaQuantum,
  BlackQuantum,
  BlueQuantum,
  CMYKAQuantum,
  CMYKQuantum,
  CyanQuantum,
  GrayAlphaQuantum,
  GrayAlphaQuark,
  GrayQuantum,
  GrayQuark,
  GreenQuantum,
  IndexAlphaQuantum,
  IndexAlphaQuark,
  IndexQuantum,
  IndexQuark,
  MagentaQuantum,
  OpacityQuantum,
  RedQuantum,
  RGBAQuantum,
  RGBQuantum,
  YellowQuantum
} QuantumType;

typedef enum
{
  UndefinedPixel,
  CharPixel,
  ShortPixel,
  IntegerPixel,
  LongPixel,
  FloatPixel,
  DoublePixel
} StorageType;

extern MagickExport Image
  *ConstituteImage(const unsigned long,const unsigned long,const char *,
    const StorageType,const void *,ExceptionInfo *),
  *PingImage(const ImageInfo *,ExceptionInfo *),
  *ReadImage(const ImageInfo *,ExceptionInfo *),
  *ReadInlineImage(const ImageInfo *,const char *,ExceptionInfo *);

extern MagickExport MagickBooleanType
  ExportImagePixels(const Image *,const long,const long,const unsigned long,
    const unsigned long,const char *,const StorageType,void *,ExceptionInfo *),
  ImportImagePixels(Image *,const long,const long,const unsigned long,
    const unsigned long,const char *,const StorageType,const void *),
  PopImagePixels(Image *,const QuantumType,unsigned char *),
  PushImagePixels(Image *,const QuantumType,const unsigned char *),
  WriteImage(const ImageInfo *,Image *),
  WriteImages(const ImageInfo *,Image *,const char *,ExceptionInfo *);

extern MagickExport void
  DestroyConstitute(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
