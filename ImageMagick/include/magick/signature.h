/*
  Digital signature methods.
*/
#ifndef _MAGICK_SIGNATURE_H
#define _MAGICK_SIGNATURE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define MagickSignatureSize  64

typedef struct _SignatureInfo
{
  unsigned long
    digest[8],
    low_order,
    high_order;

  long
    offset;

  unsigned char
    message[MagickSignatureSize];

  MagickBooleanType
    lsb_first;

  unsigned long
    signature;
} SignatureInfo;

extern MagickExport MagickBooleanType
  SignatureImage(Image *);

extern MagickExport void
  FinalizeSignature(SignatureInfo *),
  GetSignatureInfo(SignatureInfo *),
  UpdateSignature(SignatureInfo *,const unsigned char *,const size_t);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
