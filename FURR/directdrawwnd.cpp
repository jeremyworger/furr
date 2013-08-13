#include "stdafx.h"
#include "directdrawwnd.h"

/*
 * DirectDrawWnd : Public
 */
bool DirectDrawWnd::Display(const DDRAWWNDINIT &ddwi, const RECORDSESSION &rs) {

  bool r = false;

  WNDCLASSEX wnd      = {0};
  DWORD_PTR dwStyle   = (ddwi.bInitVisible ? WS_VISIBLE : 0) | (ddwi.bWindowed ? WS_CAPTION : WS_POPUP);
  DWORD_PTR dwStyleEx = WS_EX_TOOLWINDOW | (ddwi.bWindowed ? WS_EX_TOPMOST : 0);

  wnd.cbSize        = sizeof(wnd);
  wnd.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
  wnd.hInstance     = GetModuleHandle(NULL);
  wnd.lpfnWndProc   = Window::WindowProc;
  wnd.lpszClassName = DD_WNDCLASS;
  wnd.hCursor       = ddwi.bWindowed ? NULL : m_hCursor;

  RegisterClassEx(&wnd);

  if (NULL != Create(dwStyleEx, DD_WNDCLASS, ddwi.tsCaption, dwStyle, 
                    ddwi.rcSize.left, ddwi.rcSize.top, ddwi.rcSize.right, ddwi.rcSize.bottom, 
                    NULL, 0, GetModuleHandle(NULL)))
  {
                                
    m_RS   = rs;
    m_DDWI = ddwi;
  
    if (_DDrawInit()) {

      r = true;

    } else {

      _Reset();

    }
      
  }

  return r;
}

bool DirectDrawWnd::Destroy(void) {

  return (_Reset()&& Window::Destroy());
  
}

bool DirectDrawWnd::DisplayImage(DWORD_PTR dwChan, FURRImage &image) {

  bool r                        = false;
  IDirectDrawSurface *pSurface  = NULL;

  if (_DDrawGetWriteSurface(&pSurface)) {

    if (_DDrawSetBackImage(pSurface, dwChan, image)) {

      if (_Render()) {

        r = true;

      }

    }

  }

  return r;
}

bool DirectDrawWnd::Clear() {

  bool r                       = false;
  IDirectDrawSurface *pSurface = NULL;

  if (_DDrawGetWriteSurface(&pSurface)) {

    if (_FloodSurface(pSurface)) {

      r = _Render();

    }

  }

  return r;
}


/*
 * DirectDrawWnd : Private
 */

bool DirectDrawWnd::_DDrawInit(void) {

  bool r = false;

  DDRAWDEVICELIST dddl;

  if (SUCCEEDED(DirectDrawEnumerateEx(_DDrawEnumCB, &dddl, DDENUM_ATTACHEDSECONDARYDEVICES))) {

    GUID *pGuid = NULL;

    if (!m_DDWI.bPrimaryDisplay) {

      if (dddl.size() > 1) {

        DDRAWDEVICELIST::iterator it = dddl.begin();

        for (; it != dddl.end(); it++) {

          if ((*it).guid != IID_NULL) {

              pGuid = &(*it).guid;
              break;

          }

        }

      }

    }

    if (SUCCEEDED(DirectDrawCreate(pGuid, &m_iDD, NULL))) {

      DWORD_PTR dwMode = m_DDWI.bWindowed ? DDSCL_NORMAL : DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN;
      HWND hWnd        = m_DDWI.bWindowed ? NULL : m_hWnd;

      if (SUCCEEDED(m_iDD->SetCooperativeLevel(hWnd, dwMode))) {

        if (!m_DDWI.bWindowed) {

          if (SUCCEEDED(m_iDD->SetDisplayMode(DDWND_DEFAULTX, DDWND_DEFAULTY, BitDepth2Numeric(m_RS.AP.Global.Depth)))) {

            if (_DDrawInitSurfaces()) {

              r = true;

            }

          }

        } else {

          if (SUCCEEDED(m_iDD->CreateClipper(0, &m_iClipper, NULL))) {

            if (SUCCEEDED(m_iClipper->SetHWnd(0, m_hWnd))) {

              r = _DDrawInitSurfaces();

            }

          }

        }

      }

    }
            
  }

  return r;
}

bool DirectDrawWnd::_DDrawInitSurfaces(void) {

  bool r                        = false;
  DDSURFACEDESC ddsd            = {0};
  IDirectDrawSurface **pSurface = NULL;

  if (!m_DDWI.bWindowed) {

    ddsd.dwSize            = sizeof(ddsd);
    ddsd.dwFlags           = DDSD_BACKBUFFERCOUNT | DDSD_CAPS;
    ddsd.dwBackBufferCount = 1;
    ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;

    pSurface = &m_iDDPrimarySurface;

  } else {

    ddsd.dwSize          = sizeof(ddsd);
    ddsd.dwFlags         = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps  = DDSCAPS_PRIMARYSURFACE;

    pSurface = &m_iDDPrimarySurface;

  }

  if (SUCCEEDED(m_iDD->CreateSurface(&ddsd, pSurface, NULL))) {

    if (m_DDWI.bWindowed) {

      ddsd.dwFlags         = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
      ddsd.ddsCaps.dwCaps  = DDSCAPS_OFFSCREENPLAIN;
      ddsd.dwWidth         = DD_PREVIEWX;
      ddsd.dwHeight        = DD_PREVIEWY;

      pSurface = &m_iDDOffscreenSurface;

      if (SUCCEEDED(m_iDD->CreateSurface(&ddsd, pSurface, NULL))) {
        
        if (SUCCEEDED(m_iDDPrimarySurface->SetClipper(m_iClipper))) {

          r = true;

        }

      }

    } else {

      r = true;

    }

  }

  return r;
}

bool DirectDrawWnd::_DDrawGetWriteSurface(IDirectDrawSurface **ppSurface) {

  bool r = false;

  if (NULL != ppSurface) {

    if (!m_DDWI.bWindowed) {

      DDSCAPS ddsc = {0};

      ddsc.dwCaps = DDSCAPS_BACKBUFFER;

      if (NULL != m_iDDPrimarySurface.p) {

        if (SUCCEEDED(m_iDDPrimarySurface->GetAttachedSurface(&ddsc, ppSurface))) {

          r = true;

        }

      }

    } else {

      *ppSurface = m_iDDOffscreenSurface;
       r         = true;
    }

  }

  return r;
}

bool DirectDrawWnd::_DDrawSetBackImage(IDirectDrawSurface *pSurface, DWORD_PTR dwChan, FURRImage &image) {

  bool r = false;

  if (NULL != pSurface) {

    if (_FloodSurface(pSurface)) {

      DDSURFACEDESC ddsdb = {0};

      ddsdb.dwSize = sizeof(ddsdb);

      if (SUCCEEDED(pSurface->Lock(NULL, &ddsdb, DDLOCK_WRITEONLY | DDLOCK_WAIT, NULL))) {

        void *pSurfaceMem = NULL;
        POINT p           = {0, 0};
        BYTE *pPixels     = NULL;

        switch (dwChan) {

          case FIC_FULL:
            pPixels = image.AllPixels();
          break;
          case FIC_RED:
            pPixels = image.RedPixels();
          break;
          case FIC_GREEN:
            pPixels = image.GreenPixels();
          break;
          case FIC_BLUE:
            pPixels = image.BluePixels();
          break;

        }

        if (NULL != pPixels) {

          DWORD_PTR dwX   = image.Width();
          DWORD_PTR dwY   = image.Height();

          if (!m_DDWI.bWindowed) {

            if ((dwX == DDWND_DEFAULTX) && (dwY == DDWND_DEFAULTY)) {

              // Image is the same resolution as the DD surface;
              // start copying at (0, 0)
              pSurfaceMem = ddsdb.lpSurface;

            } else {

              // Image is smaller than the DD surface;
              // copy at a centered offset
              p.x = ((DDWND_DEFAULTX >> 1) - (dwX >> 1));
              p.y = ((DDWND_DEFAULTY >> 1) - (dwY >> 1));

              pSurfaceMem = _PointToAddress(ddsdb.lpSurface, ddsdb.lPitch, &p);

            }

            if (NULL != pSurfaceMem) {

              r = _BitBlt(pSurfaceMem, pPixels, ddsdb.lPitch, dwX, dwY);

            }

          } else {

            pSurfaceMem = ddsdb.lpSurface;

            if ((image.Resize(DD_PREVIEWX, DD_PREVIEWY)) && (NULL != pSurfaceMem)) {

              r = _BitBlt(pSurfaceMem, pPixels, ddsdb.lPitch, dwX, dwY);

            }

          }

          pSurface->Unlock(NULL);

        }

      }

    }

  }

  return r;
}

bool DirectDrawWnd::_Render(void) {

  bool r = false;

  if (!m_DDWI.bWindowed) {

    if (NULL != m_iDDPrimarySurface.p) {

      HRESULT hr = E_FAIL;

      do {

        hr = m_iDDPrimarySurface->Flip(NULL, DDFLIP_WAIT);

      } while((DDERR_WASSTILLDRAWING == hr) || (DDERR_SURFACEBUSY == hr));

      do {

        hr = m_iDDPrimarySurface->GetFlipStatus(DDGFS_ISFLIPDONE);

      } while ((DDERR_WASSTILLDRAWING == hr) || (DDERR_SURFACEBUSY == hr));

      if (DD_OK == hr) {

        SetEvent(m_DDWI.hEventRender);
        r = true;

      }

    }

  } else {

    if ((NULL != m_iDDPrimarySurface.p) && (NULL != m_iDDOffscreenSurface.p)) {

      RECT rcClient = {0};

      GetClientRect(&rcClient);

      ClientToScreenRect(m_hWnd, &rcClient);

      if (SUCCEEDED(m_iDDPrimarySurface->Blt(&rcClient, m_iDDOffscreenSurface, NULL, DDBLT_WAIT, NULL))) {

        r = true;

      }

    }

  }

  return r;
}

bool DirectDrawWnd::_DDrawKill(void) {

  bool r = true;

  m_iDDPrimarySurface.Release();
  m_iDDOffscreenSurface.Release();
  m_iClipper.Release();
  m_iDD.Release();

  return r;
}

BOOL WINAPI DirectDrawWnd::_DDrawEnumCB(GUID *pGuid, LPTSTR szDesc, LPTSTR szName, LPVOID pVoid, HMONITOR hMon) {

  PDDRAWDEVICELIST pdddl = static_cast<PDDRAWDEVICELIST>(pVoid);

  if (NULL != pdddl) {

   DDRAWDEVICE ddd;

   ddd.guid   = pGuid ? (*pGuid) : IID_NULL;
   ddd.tsDesc = szDesc;
   ddd.tsName = szName;
   ddd.hMon   = hMon;

   odsf(_T("DirectDrawWnd::_DDrawEnumCB() : Found device '%s' (%s) on monitor 0x%08X.\n"),
        szName, szDesc, hMon);

   pdddl->push_back(ddd);

  }

  return TRUE;
}

BYTE *DirectDrawWnd::_PointToAddress(const void *pSurfaceMem, LONG lSurfacePitch, const POINT *pPoint) {

 BYTE *r = NULL;

  if ((NULL != pSurfaceMem) && (NULL != pPoint)) {

    if ((0 < pPoint->x) && (0 < pPoint->y)) {

      r = reinterpret_cast<BYTE *>(const_cast<void *>(pSurfaceMem)) + 
          (pPoint->x * (BitDepth2Numeric(m_RS.AP.Global.Depth) >> 3) + (pPoint->y * lSurfacePitch));

    }

  }

  return r;
}

bool DirectDrawWnd::_BitBlt(void *pSurfaceOffset, BYTE *pPixels, LONG lSurfacePitch, DWORD_PTR dwX, DWORD_PTR dwY) {

  bool r              = true;
  BYTE *pSurface      = reinterpret_cast<BYTE *>(pSurfaceOffset);
  BYTE *pImage        = pPixels;
  DWORD *pTempImage   = NULL;
  DWORD *pTempSurface = NULL;
  DWORD_PTR dwWidth   = dwX;
  DWORD_PTR dwHeight  = dwY;
  DWORD_PTR dwPitch   = (((dwWidth << 2) + 3) & ~3);

  for (DWORD_PTR dwY = 0UL; dwY < dwHeight; dwY++) {

    pTempSurface = reinterpret_cast<DWORD *>(pSurface);
    pTempImage   = reinterpret_cast<DWORD *>(pImage);

    for (DWORD_PTR dwX = 0UL; dwX < dwWidth; dwX++) {

      pTempSurface[0] = pTempImage[0];

      pTempSurface++;
      pTempImage++;

    }

    pSurface += lSurfacePitch;
    pImage   += dwPitch;

  }

  return r;
}

bool DirectDrawWnd::_FloodSurface(IDirectDrawSurface *pSurface, DWORD dwColour) {

  bool r = false;

  DDBLTFX ddbfx = {0};

  ddbfx.dwSize      = sizeof(ddbfx);
  ddbfx.dwFillColor = dwColour;
  
  if (NULL != pSurface) {

    if (SUCCEEDED(pSurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbfx))) {
      
      r = true;
  
    }

  }

  return r;
}

bool DirectDrawWnd::_Reset(void) {

  bool r = false;
  
  if (_DDrawKill()) {

    m_RS.Reset();
    m_DDWI.Reset();
    r = true;

  }

  return r;
}
