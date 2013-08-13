/*
  ImageMagick image profile methods.
*/
#ifndef _MAGICK_PROFILE_H
#define _MAGICK_PROFILE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/hashmap.h"
#include "magick/string_.h"

MagickExport char
  *GetNextImageProfile(const Image *);

extern MagickExport MagickBooleanType
  CloneImageProfiles(Image *,const Image *),
  ProfileImage(Image *,const char *,const unsigned char *,const unsigned long,
    const MagickBooleanType),
  SetImageProfile(Image *,const char *,const StringInfo *);

MagickExport StringInfo
  *GetImageProfile(const Image *,const char *),
  *RemoveImageProfile(Image *,const char *);

extern MagickExport void
  DestroyImageProfiles(Image *),
  ResetImageProfileIterator(const Image *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif 
#endif
