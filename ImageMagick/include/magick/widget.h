/*
  ImageMagick X11 Widget Methods.
*/
#ifndef _MAGICK_WIDGET_H
#define _MAGICK_WIDGET_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if defined(HasX11)

extern MagickExport int
  XCommandWidget(Display *,XWindows *,const char **,XEvent *),
  XConfirmWidget(Display *,XWindows *,const char *,const char *),
  XDialogWidget(Display *,XWindows *,const char *,const char *,char *),
  XMenuWidget(Display *,XWindows *,const char *,const char **,char *);

extern MagickExport MagickBooleanType
  XPreferencesWidget(Display *,XResourceInfo *,XWindows *);

extern MagickExport void
  DestroyXWidget(void),
  XColorBrowserWidget(Display *,XWindows *,const char *,char *),
  XFileBrowserWidget(Display *,XWindows *,const char *,char *),
  XFontBrowserWidget(Display *,XWindows *,const char *,char *),
  XInfoWidget(Display *,XWindows *,const char *),
  XListBrowserWidget(Display *,XWindows *,XWindowInfo *,const char **,
    const char *,const char *,char *),
  XMonitorWidget(Display *,XWindows *,const char *,
    const MagickOffsetType,const MagickSizeType),
  XNoticeWidget(Display *,XWindows *,const char *,const char *),
  XTextViewWidget(Display *,const XResourceInfo *,XWindows *,
    const MagickBooleanType,const char *,const char **);

#endif

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
