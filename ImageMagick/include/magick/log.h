/*
  Log methods.
*/
#ifndef _MAGICK_LOG_H
#define _MAGICK_LOG_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/error.h"

#if !defined(__attribute__)
#  define __attribute__(x) /*nothing*/
#endif

#if !defined(GetUnadornedModuleName)
# if (((defined(__cplusplus) || defined(c_plusplus)) && defined(HAS_CPP__func__)) || \
      (!(defined(__cplusplus) || defined(c_plusplus)) && defined(HAS_C__func__)))
#  define GetUnadornedModuleName() (__func__)
# elif defined(_VISUALC_) && defined(__FUNCTION__)
#  define GetUnadornedModuleName() (__FUNCTION__)
# else
#  define GetUnadornedModuleName() ("unknown")
# endif
#endif
#if !defined(GetMagickModule)
# define GetMagickModule()  \
  __FILE__,GetUnadornedModuleName(),(unsigned long) __LINE__
#endif

#define MagickLogFilename  "log.mgk"

typedef enum
{
  UndefinedEvents,
  NoEvents = 0x0000,
  TraceEvent = 0x0001,
  AnnotateEvent = 0x0002,
  BlobEvent = 0x0004,
  CacheEvent = 0x0008,
  CoderEvent = 0x0010,
  ConfigureEvent = 0x0020,
  DeprecateEvent = 0x0040,
  DrawEvent = 0x0080,
  ExceptionEvent = 0x0100,
  LocaleEvent = 0x0200,
  ModuleEvent = 0x0400,
  ResourceEvent = 0x0800,
  TransformEvent = 0x1000,
  X11Event = 0x2000,
  UserEvent = 0x4000,
  AllEvents = 0x7fffffff
} LogEventType;

typedef enum
{
  UndefinedHandler = 0x0000,
  NoHandler = 0x0000,
  ConsoleHandler = 0x0001,
  StdoutHandler = 0x0002,
  StderrHandler = 0x0004,
  FileHandler = 0x0008
} LogHandlerType;

typedef struct _LogInfo
{
  LogEventType
    event_mask;

  LogHandlerType
    handler_mask;

  char
    *path,
    *name,
    *filename,
    *format;

  unsigned long
    generations,
    limit;

  FILE
    *file;

  unsigned long
    generation,
    count;

  MagickBooleanType
    stealth;

  TimerInfo
    timer;

  struct _LogInfo
    *previous,
    *next;  /* deprecated, use GetLogInfoList() */

  unsigned long
    signature;
} LogInfo;

extern MagickExport char
  **GetLogList(const char *,unsigned long *);
                                                                                
extern MagickExport const LogInfo
  *GetLogInfo(const char *,ExceptionInfo *),
  **GetLogInfoList(const char *,unsigned long *);

extern MagickExport LogEventType
  SetLogEventMask(const char *);

extern MagickExport MagickBooleanType
  IsEventLogging(void),
  ListLogInfo(FILE *,ExceptionInfo *),
  LogMagickEvent(const LogEventType,const char *,const char *,
    const unsigned long,const char *,...) __attribute__((format (printf,5,6)));

extern MagickExport void
  DestroyLogList(void),
  SetLogFormat(const char *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
