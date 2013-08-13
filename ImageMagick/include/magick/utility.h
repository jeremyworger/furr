/*
  ImageMagick Utility Methods.
*/
#ifndef _MAGICK_UTILITY_H
#define _MAGICK_UTILITY_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(__attribute__)
#  define __attribute__(x) /*nothing*/
#endif

typedef enum
{
  UndefinedPath,
  MagickPath,
  RootPath,
  HeadPath,
  TailPath,
  BasePath,
  ExtensionPath,
  SubimagePath,
  CanonicalPath
} PathType;

extern MagickExport char
  *Base64Encode(const unsigned char *,const size_t,size_t *),
  **ListFiles(const char *,const char *,unsigned long *),
  *TranslateText(const ImageInfo *,Image *,const char *);

extern MagickExport int
  SystemCommand(const MagickBooleanType,const char *);

extern MagickExport MagickBooleanType
  AcquireUniqueFilename(char *),
  ExpandFilenames(int *,char ***),
  GetExecutionPath(char *),
  IsAccessible(const char *);

extern MagickExport unsigned char
  *Base64Decode(const char *, size_t *);

extern MagickExport unsigned long
  MultilineCensus(const char *);

extern MagickExport void
  AppendImageFormat(const char *,char *),
  ChopPathComponents(char *,const unsigned long),
  ExpandFilename(char *),
  FormatSize(const MagickSizeType,char *),
  GetPathComponent(const char *,PathType,char *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
