/*
  ImageMagick token methods.
*/
#ifndef _MAGICK_TOKEN_H
#define _MAGICK_TOKEN_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*
  Typedef declarations.
*/
typedef struct _TokenInfo
{
  int
    state;

  MagickStatusType
    flag;

  long
    offset;

  char
    quote;
} TokenInfo;

extern MagickExport int
  Tokenizer(TokenInfo *,unsigned,char *,size_t,char *,char *,char *,char *,
    char,char *,int *,char *);

extern MagickExport MagickBooleanType
  GlobExpression(const char *,const char *),
  IsGlob(const char *);

extern MagickExport void
  GetToken(const char *,char **,char *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
