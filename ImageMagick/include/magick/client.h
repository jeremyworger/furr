/*
  ImageMagick client methods.
*/
#ifndef _MAGICK_CLIENT_H
#define _MAGICK_CLIENT_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport const char
  *GetClientPath(void),
  *GetClientName(void),
  *SetClientName(const char *),
  *SetClientPath(const char *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
