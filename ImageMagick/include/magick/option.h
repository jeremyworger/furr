/*
  ImageMagick Option methods.
*/
#ifndef _MAGICK_OPTION_H
#define _MAGICK_OPTION_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef enum
{
  MagickAlignOptions,
  MagickBooleanOptions,
  MagickChannelOptions,
  MagickClassOptions,
  MagickClipPathOptions,
  MagickColorspaceOptions,
  MagickCompositeOptions,
  MagickCompressionOptions,
  MagickDecorationOptions,
  MagickDisposeOptions,
  MagickEndianOptions,
  MagickEvaluateOptions,
  MagickFillRuleOptions,
  MagickFilterOptions,
  MagickGravityOptions,
  MagickImageOptions,
  MagickIntentOptions,
  MagickInterlaceOptions,
  MagickLineCapOptions,
  MagickLineJoinOptions,
  MagickLogEventOptions,
  MagickMetricOptions,
  MagickMethodOptions,
  MagickModeOptions,
  MagickMogrifyOptions,
  MagickNoiseOptions,
  MagickOrientationOptions,
  MagickPreviewOptions,
  MagickPrimitiveOptions,
  MagickResolutionOptions,
  MagickResourceOptions,
  MagickStretchOptions,
  MagickStyleOptions,
  MagickVirtualPixelOptions
} MagickOption;

typedef struct _OptionInfo
{
  char
    *mnemonic;

  long
    type;
} OptionInfo;

extern MagickExport char
  **GetMagickOptions(const MagickOption),
  *GetNextImageOption(const ImageInfo *),
  *RemoveImageOption(ImageInfo *,const char *);

extern MagickExport const char
  *GetImageOption(const ImageInfo *,const char *),
  *MagickOptionToMnemonic(const MagickOption,const long);

extern MagickExport long
  ParseChannelOption(const char *),
  ParseMagickOption(const MagickOption,const MagickBooleanType,const char *);

extern MagickExport MagickBooleanType
  CloneImageOptions(ImageInfo *,const ImageInfo *),
  DefineImageOption(ImageInfo *,const char *),
  DeleteImageOption(ImageInfo *,const char *),
  SetImageOption(ImageInfo *,const char *,const char *);

extern MagickExport void
  DestroyImageOptions(ImageInfo *),
  ResetImageOptionIterator(const ImageInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
