/*
  ImageMagick Application Programming Interface declarations.
*/
#ifndef _MAGICK_MAGICK_H
#define _MAGICK_MAGICK_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef Image
  *(DecoderHandler)(const ImageInfo *,ExceptionInfo *);

typedef MagickBooleanType
  (EncoderHandler)(const ImageInfo *,Image *);

typedef MagickBooleanType
  (MagickHandler)(const unsigned char *,const size_t);

typedef struct _MagickInfo
{
  char
    *name,
    *description,
    *version,
    *note,
    *module;

  ImageInfo
    *image_info;

  DecoderHandler
    *decoder;

  EncoderHandler
    *encoder;

  MagickHandler
    *magick;

  void
    *client_data;

  MagickBooleanType
    adjoin,
    raw,
    seekable_stream,
    blob_support,
    thread_support,
    stealth;

  struct _MagickInfo
    *previous,
    *next;  /* deprecated, use GetMagickInfoList() */

  unsigned long
    signature;
} MagickInfo;

extern MagickExport char
  **GetMagickList(const char *,unsigned long *),
  *MagickToMime(const char *);

extern MagickExport const char
  *GetImageMagick(const unsigned char *,const size_t),
  *GetMagickDescription(const MagickInfo *);

extern MagickExport DecoderHandler
  *GetMagickDecoder(const MagickInfo *);

extern MagickExport EncoderHandler
  *GetMagickEncoder(const MagickInfo *);

extern MagickExport MagickBooleanType
  GetMagickAdjoin(const MagickInfo *),
  GetMagickBlobSupport(const MagickInfo *),
  GetMagickSeekableStream(const MagickInfo *),
  GetMagickThreadSupport(const MagickInfo *),
  IsMagickInstantiated(void),
  UnregisterMagickInfo(const char *);

extern const MagickExport MagickInfo
  *GetMagickInfo(const char *,ExceptionInfo *),
  **GetMagickInfoList(const char *,unsigned long *);

extern MagickExport MagickInfo
  *RegisterMagickInfo(MagickInfo *),
  *SetMagickInfo(const char *);

extern MagickExport void
  DestroyMagick(void),
  DestroyMagickList(void),
  InitializeMagick(const char *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
