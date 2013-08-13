/*
  ImageMagick Image Quantization Methods.
*/
#ifndef _MAGICK_QUANTIZE_H
#define _MAGICK_QUANTIZE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef struct _QuantizeInfo
{
  unsigned long
    number_colors;

  unsigned long
    tree_depth;

  MagickBooleanType
    dither;

  ColorspaceType
    colorspace;

  MagickBooleanType
    measure_error;

  unsigned long
    signature;
} QuantizeInfo;

extern MagickExport MagickBooleanType
  GetImageQuantizeError(Image *),
  MapImage(Image *,const Image *,const MagickBooleanType),
  MapImages(Image *,const Image *,const MagickBooleanType),
  OrderedDitherImage(Image *),
  PosterizeImage(Image *,const unsigned long,const MagickBooleanType),
  QuantizeImage(const QuantizeInfo *,Image *),
  QuantizeImages(const QuantizeInfo *,Image *);

extern MagickExport QuantizeInfo
  *CloneQuantizeInfo(const QuantizeInfo *);

extern MagickExport void
  CompressImageColormap(Image *),
  DestroyQuantizeInfo(QuantizeInfo *),
  GetQuantizeInfo(QuantizeInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
