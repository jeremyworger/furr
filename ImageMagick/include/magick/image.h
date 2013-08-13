/*
  ImageMagick Image Methods.
*/
#ifndef _MAGICK_IMAGE_H
#define _MAGICK_IMAGE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define ColorMatch(p,q) ((MagickBooleanType) (((p)->red == (q)->red) && \
  ((p)->green == (q)->green) && ((p)->blue == (q)->blue)))
#define OpaqueOpacity  ((Quantum) 0UL)
#define PixelIntensity(pixel) ((MagickRealType) \
  (0.299*(pixel)->red+0.587*(pixel)->green+0.114*(pixel)->blue+0.5))
#define PixelIntensityToQuantum(pixel) ((Quantum) \
  (0.299*(pixel)->red+0.587*(pixel)->green+0.114*(pixel)->blue+0.5))
#define ScaleColor5to8(x)  (((x) << 3) | ((x) >> 2))
#define ScaleColor6to8(x)  (((x) << 2) | ((x) >> 4))
#define TransparentOpacity  ((Quantum) MaxRGB)

typedef enum
{
  UndefinedTransmitType,
  FileTransmitType,
  BlobTransmitType,
  StreamTransmitType,
  ImageTransmitType
} TransmitType;

extern MagickExport const char
  *BackgroundColor,
  *BorderColor,
  *DefaultTileFrame,
  *DefaultTileGeometry,
  *DefaultTileLabel,
  *ForegroundColor,
  *MatteColor,
  *LoadImageTag,
  *LoadImagesTag,
  *PSDensityGeometry,
  *PSPageGeometry,
  *SaveImageTag,
  *SaveImagesTag;

extern MagickExport const unsigned long
  UndefinedCompressionQuality;

extern MagickExport const PixelPacket
  *AcquireImagePixels(const Image *,const long,const long,const unsigned long,
    const unsigned long,ExceptionInfo *);

extern MagickExport ExceptionType
  CatchImageException(Image *);

extern MagickExport Image
  *AllocateImage(const ImageInfo *),
  *AppendImages(const Image *,const MagickBooleanType,ExceptionInfo *),
  *AverageImages(const Image *,ExceptionInfo *),
  *CloneImage(const Image *,const unsigned long,const unsigned long,
    const MagickBooleanType,ExceptionInfo *),
  *CombineImages(const Image *,const ChannelType,ExceptionInfo *),
  *GetImageClipMask(const Image *,ExceptionInfo *),
  *ReferenceImage(Image *);

extern MagickExport ImageInfo
  *CloneImageInfo(const ImageInfo *);

extern MagickExport ImageType
  GetImageType(const Image *,ExceptionInfo *);

extern MagickExport IndexPacket
  *GetIndexes(const Image *);

extern MagickExport MagickBooleanType
  AllocateImageColormap(Image *,const unsigned long),
  AnimateImages(const ImageInfo *,Image *),
  ClipImage(Image *),
  ClipPathImage(Image *,const char *,const MagickBooleanType),
  GradientImage(Image *,const PixelPacket *,const PixelPacket *),
  CycleColormapImage(Image *,const long),
  DescribeImage(Image *,FILE *,const MagickBooleanType),
  DisplayImages(const ImageInfo *,Image *),
  GetImageChannelExtrema(const Image *,const ChannelType,unsigned long *,
    unsigned long *,ExceptionInfo *),
  GetImageExtrema(const Image *,unsigned long *,unsigned long *,
    ExceptionInfo *),
  GetImageChannelMean(const Image *,const ChannelType,double *,double *,
    ExceptionInfo *),
  GetImageMean(const Image *,double *,double *,ExceptionInfo *),
  GradientImage(Image *,const PixelPacket *,const PixelPacket *),
  IsTaintImage(const Image *),
  IsMagickConflict(const char *),
  ListMagickInfo(FILE *,ExceptionInfo *),
  PlasmaImage(Image *,const SegmentInfo *,unsigned long,unsigned long),
  SeparateImageChannel(Image *,const ChannelType),
  SetImageChannelDepth(Image *,const ChannelType,const unsigned long),
  SetImageClipMask(Image *,const Image *),
  SetImageInfo(ImageInfo *,const MagickBooleanType,ExceptionInfo *),
  SetImageDepth(Image *,const unsigned long),
  SetImageType(Image *,const ImageType),
  SignatureImage(Image *),
  SortColormapByIntensity(Image *),
  StripImage(Image *),
  SyncImage(Image *),
  SyncImagePixels(Image *),
  TextureImage(Image *,const Image *);

extern MagickExport PixelPacket
  AcquireOnePixel(const Image *,const long,const long,ExceptionInfo *),
  *GetImagePixels(Image *,const long,const long,const unsigned long,
    const unsigned long),
  GetOnePixel(Image *,const long,const long),
  *GetPixels(const Image *),
  *SetImagePixels(Image *,const long,const long,const unsigned long,
    const unsigned long);

extern MagickExport RectangleInfo
  GetImageBoundingBox(const Image *,ExceptionInfo *exception);

extern MagickExport unsigned long
  GetImageChannelDepth(const Image *,const ChannelType,ExceptionInfo *),
  GetImageDepth(const Image *,ExceptionInfo *),
  GetImageQuantumDepth(const Image *);

extern MagickExport void
  AllocateNextImage(const ImageInfo *,Image *),
  DestroyColorList(void),
  DestroyImage(Image *),
  DestroyImageInfo(ImageInfo *),
  DestroyImagePixels(Image *),
  GetImageException(Image *,ExceptionInfo *),
  GetImageInfo(ImageInfo *),
  ModifyImage(Image **,ExceptionInfo *),
  SetImage(Image *,const Quantum),
  SetImageOpacity(Image *,const Quantum);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
