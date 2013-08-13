/*
  ImageMagick image montage methods.
*/
#ifndef _MAGICK_MONTAGE_H
#define _MAGICK_MONTAGE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef enum
{
  UndefinedMode,
  FrameMode,
  UnframeMode,
  ConcatenateMode
} MontageMode;

typedef struct _MontageInfo
{
  char
    *geometry,
    *tile,
    *title,
    *frame,
    *texture,
    *font;

  double
    pointsize;

  unsigned long
    border_width;

  MagickBooleanType
    shadow;

  PixelPacket
    fill,
    stroke,
    background_color,
    border_color,
    matte_color;

  GravityType
    gravity;

  char
    filename[MaxTextExtent];

  MagickBooleanType
    debug;

  unsigned long
    signature;
} MontageInfo;

extern MagickExport Image
  *MontageImages(const Image *,const MontageInfo *,ExceptionInfo *);

extern MagickExport MagickBooleanType
  MontageImageCommand(ImageInfo *,int,char **,char **,ExceptionInfo *);

extern MagickExport MontageInfo
  *CloneMontageInfo(const ImageInfo *,const MontageInfo *);

extern MagickExport void
  DestroyMontageInfo(MontageInfo *),
  GetMontageInfo(const ImageInfo *,MontageInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
