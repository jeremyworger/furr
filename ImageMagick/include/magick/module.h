/*
  ImageMagick Modules Methods.
*/
#ifndef _MAGICK_MODULE_H
#define _MAGICK_MODULE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef enum
{
  MagickCoderModule,
  MagickFilterModule
} MagickModuleType;

typedef struct _ModuleInfo
{
  char
    *path,
    *tag;

  void
    *handle,
    (*register_module)(void),
    (*unregister_module)(void);

  time_t
    load_time;

  MagickBooleanType
    stealth;

  struct _ModuleInfo
    *previous,
    *next;  /* deprecated, use GetModuleInfoList() */

  unsigned long
    signature;
} ModuleInfo;

extern MagickExport char
  **GetModuleList(const char *,unsigned long *);

extern MagickExport char
  *TagToModule(const char *);

extern MagickExport const ModuleInfo
  *GetModuleInfo(const char *,ExceptionInfo *),
  **GetModuleInfoList(const char *,unsigned long *);

extern MagickExport MagickBooleanType
  ExecuteModuleProcess(const char *,Image **,const int,char **),
  ExecuteStaticModuleProcess(const char *,Image **,const int,char **),
  ListModuleInfo(FILE *,ExceptionInfo *),
  OpenModule(const char *,ExceptionInfo *),
  OpenModules(ExceptionInfo *);

extern MagickExport void
  DestroyModuleList(void),
  *GetModuleBlob(const char *,char *,size_t *,ExceptionInfo *),
  RegisterStaticModules(void),
  UnregisterStaticModules(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
