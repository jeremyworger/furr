/*
  ImageMagick Progress Monitor Methods.
*/
#ifndef _MAGICK_MONITOR_H
#define _MAGICK_MONITOR_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef MagickBooleanType
  (*MonitorHandler)(const char *,const MagickOffsetType,const MagickSizeType,
    ExceptionInfo *);

extern MagickExport MagickBooleanType
  MagickMonitor(const char *,const MagickOffsetType,const MagickSizeType,
    ExceptionInfo *);

extern MagickExport MonitorHandler
  SetMonitorHandler(MonitorHandler);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
