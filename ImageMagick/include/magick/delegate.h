/*
  Methods to Read/Write/Invoke Delegates.
*/
#ifndef _MAGICK_DELEGATE_H
#define _MAGICK_DELEGATE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if defined(HasGS)
#include "ps/iapi.h"
#include "ps/errors.h"
#endif

#ifndef gs_main_instance_DEFINED
# define gs_main_instance_DEFINED
typedef struct gs_main_instance_s gs_main_instance;
#endif

#if !defined(MagickDLLCall)
#  if defined(__WINDOWS__)
#    define MagickDLLCall __stdcall
#  else
#    define MagickDLLCall
#  endif
#endif

typedef struct _GhostscriptVectors
{
  int
    (MagickDLLCall *exit)(gs_main_instance *);

  int
    (MagickDLLCall *init_with_args)(gs_main_instance *,int,char **);

  int
    (MagickDLLCall *new_instance)(gs_main_instance **,void *);

  int
    (MagickDLLCall *run_string)(gs_main_instance *,const char *,int,int *);

  void
    (MagickDLLCall *delete_instance)(gs_main_instance *);
} GhostscriptVectors;

typedef struct _DelegateInfo
{
  char
    *path,
    *decode,
    *encode,
    *commands;
                                                                                
  long
    mode;
                                                                                
  MagickBooleanType
    spawn,
    stealth;
                                                                                
  struct _DelegateInfo
    *previous,
    *next;  /* deprecated, use GetDelegateInfoList() */

  unsigned long
    signature;
} DelegateInfo;

extern MagickExport char
  *GetDelegateCommand(const ImageInfo *,Image *,const char *,const char *,
    ExceptionInfo *),
  **GetDelegateList(const char *,unsigned long *);

extern MagickExport const char
  *GetDelegateCommands(const DelegateInfo *);

extern MagickExport const DelegateInfo
  *GetDelegateInfo(const char *,const char *,ExceptionInfo *exception),
  **GetDelegateInfoList(const char *,unsigned long *);

extern MagickExport DelegateInfo
  *SetDelegateInfo(DelegateInfo *);

extern MagickExport long
  GetDelegateMode(const DelegateInfo *);

extern MagickExport MagickBooleanType
  InvokePostscriptDelegate(const MagickBooleanType,const char *),
  InvokeDelegate(ImageInfo *,Image *,const char *,const char *,ExceptionInfo *),
  ListDelegateInfo(FILE *,ExceptionInfo *);

extern MagickExport void
  DestroyDelegateList(void);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
