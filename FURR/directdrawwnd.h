#ifndef _DIRECTDRAWWND_H
#define _DIRECTDRAWWND_H

#include "stdafx.h"
#include "furrtypes.h"
#include "furrmaindlg.h"
#include "furrimimage.h"
#include "resource.h"
#include <ddraw.h>
#include <atlbase.h>

/*
 * Constants
 */
#define DDWND_DEFAULTX  1600//3840
#define DDWND_DEFAULTY  1200//2400

/*
 * DDRAWDEVICE Structure
 *
 * Defines an available video
 * output device.
 */
typedef struct tagDDRAWDEVICE {

  tagDDRAWDEVICE() {

    guid    = IID_NULL;
    tsDesc  = _T("");
    tsName  = _T("");
    hMon    = NULL;

  }

  GUID guid;
  TSTRING tsDesc;
  TSTRING tsName;
  HMONITOR hMon;

} DDRAWDEVICE, *PDDRAWDEVICE;

/*
 * DDRAWDEVICELIST Type
 *
 * Defines a list of DDRAWDEVICE structures.
 */
typedef std::list<DDRAWDEVICE> DDRAWDEVICELIST, *PDDRAWDEVICELIST;

/*
 * Definitions
 */
#define DD_WNDCLASS _T("FURRDirectDrawWnd")
#define DD_PREVIEWX 640
#define DD_PREVIEWY 480

/*
 * DDRAWWNDINIT Structure
 *
 * Contains initialization
 * parameters for a DirectDrawWnd.
 */
typedef struct tagDDRAWWNDINIT {

  tagDDRAWWNDINIT() {

    Reset();

  }

  tagDDRAWWNDINIT &operator=(const tagDDRAWWNDINIT &in) {

    bWindowed       = in.bWindowed;
    bPrimaryDisplay = in.bPrimaryDisplay;
    bInitVisible    = in.bInitVisible;
    tsCaption       = in.tsCaption;
    hEventRender    = in.hEventRender;
    CopyMemory(&rcSize, &in.rcSize, sizeof(RECT));

    return (*this);
  }

  void Reset(void) {

    bWindowed       = false;
    bPrimaryDisplay = false;
    bInitVisible    = true;
    tsCaption       = _T("");
    hEventRender    = NULL;
    ZeroMemory(&rcSize, sizeof(RECT));

  }

  bool bWindowed;         // Is the window to be displayed fullscreen or windowed?
  bool bPrimaryDisplay;   // Should the window be created on the primary display?
  bool bInitVisible;      // Is the window initially visible?
  TSTRING tsCaption;      // Caption of window, if in windowed mode
  HANDLE hEventRender;    // Handle to an event that the session object will poll for
                          // being signalled before notifying camera to shoot
  RECT rcSize;            // Size of window, if in windowed mode

} DDRAWWNDINIT, *PDDRAWWNDINIT;

/*
 * DirectDrawWnd Class
 *
 * Wraps all Direct Draw functionality
 * of FURR.
 */
class DirectDrawWnd : public Window {

public:

  DirectDrawWnd() { 
    
    m_hCursor = static_cast<HCURSOR>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_INVISCUR), 
                                     IMAGE_CURSOR, 16, 16, 0));

  }

  virtual ~DirectDrawWnd() { 

    Destroy();
    DestroyCursor(m_hCursor);
  
  }

  bool Display(const DDRAWWNDINIT &ddwi, const RECORDSESSION &rs);
  bool Destroy(void);

  bool DisplayImage(DWORD_PTR dwChan, FURRImage &image);
  bool Clear(void);

private:

  bool _DDrawInit(void);
  bool _DDrawInitSurfaces(void);
  bool _DDrawGetWriteSurface(IDirectDrawSurface **ppSurface);
  bool _DDrawSetBackImage(IDirectDrawSurface *pSurface, DWORD_PTR dwChan, FURRImage &image);
  bool _Render(void);
  bool _DDrawKill(void);
  static BOOL WINAPI _DDrawEnumCB(GUID *pGuid, LPTSTR szDesc, LPTSTR szName, LPVOID pVoid, HMONITOR hMon);

  BYTE *_PointToAddress(const void *pSurfaceMem, LONG lSurfacePitch, const POINT *pPoint);
  bool _BitBlt(void *pSurfaceOffset, BYTE *pPixels, LONG lSurfacePitch, DWORD_PTR dwX, DWORD_PTR dwY);
  bool _FloodSurface(IDirectDrawSurface *pSurface, DWORD dwColour = 0x00000000UL);

  bool _Reset(void);

protected:
  RECORDSESSION               m_RS;
  DDRAWWNDINIT                m_DDWI;
  CComPtr<IDirectDraw>        m_iDD;
  CComPtr<IDirectDrawSurface> m_iDDPrimarySurface;
  CComPtr<IDirectDrawSurface> m_iDDOffscreenSurface;
  CComPtr<IDirectDrawClipper> m_iClipper;
  HCURSOR                     m_hCursor;

};

#endif // !_DIRECTDRAWWND_H