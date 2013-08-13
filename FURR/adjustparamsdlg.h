#ifndef _ADJUSTPARAMSDLG_H
#define _ADJUSTPARAMSDLG_H

#include "stdafx.h"
#include "adjustparams.h"

/*
 * AdjustParamsPage Class
 *
 * Derives from Dialog to provide
 * child-page functionality for the
 * adjust parameters dialog.
 */
class AdjustParamsPage : public Dialog {

public:
  AdjustParamsPage() {
    m_pParent = NULL;
  }
  virtual ~AdjustParamsPage() { }

  void SetParentDlg(class AdjustParamsDlg *pParent) { m_pParent = pParent; }

  DLG_VMSG OnCreate(WPARAM wParam, LPARAM lParam) = 0;
  DLG_VMSG OnCommand(WPARAM wParam, LPARAM lParam) = 0;

protected:
  class AdjustParamsDlg *m_pParent;

};

/*
 * APGlobalPage Class
 *
 * Derives from AdjustParamsPage to
 * provide a wrapper for global parameters
 * settings
 */
class APGlobalPage : public AdjustParamsPage {

public:
  APGlobalPage() {
    m_pAPGlobal = NULL;
    m_tsLUTFile = _T("");
  }
  ~APGlobalPage() { }

  BOOL OnCreate(WPARAM wParam, LPARAM lParam);
  BOOL OnCommand(WPARAM wParam, LPARAM lParam);

  bool SetLUTFile(CTSTRING &tsLUTFile);
  void SetStructPointer(PAPGLOBAL papg) { m_pAPGlobal = papg; }
  bool InitFields(void);
  bool SaveFields(void);

private:
  bool _BrowseForLUTFile(void);

private:
  PAPGLOBAL m_pAPGlobal;
  ComboBox  m_Resolution;
  ComboBox  m_Bitdepth;
  Window    m_LUTFile;
  TSTRING   m_tsLUTFile;

};

/*
 * APChannelPage Class
 *
 * Derives from AdjustParamsPage to
 * provide a wrapper for per-channel
 * settings
 */
class APChannelPage : public AdjustParamsPage {

public:
  APChannelPage() {
    m_pAPChannel = NULL;
  }
  ~APChannelPage() { }

  BOOL OnCreate(WPARAM wParam, LPARAM lParam);
  BOOL OnCommand(WPARAM wParam, LPARAM lParam);

  void SetStructPointer(PAPCHANNEL papc) { m_pAPChannel = papc; }
  bool InitFields(void);
  bool SaveFields(void);

private:
  PAPCHANNEL m_pAPChannel;
  TrackBar   m_Brightness;
  TrackBar   m_Contrast;
  Window     m_Gamma;
  Window     m_ET;
  Window     m_ED;

};

/*
 * APFullImagePage Class
 *
 * Derives from AdjustParamsPage to
 * provide a wrapper for full image
 * settings
 */
class APFullImagePage : public AdjustParamsPage {

public:
  APFullImagePage() {
    m_pAPChannel = NULL;
  }
  ~APFullImagePage() { }

  BOOL OnCreate(WPARAM wParam, LPARAM lParam);
  BOOL OnCommand(WPARAM wParam, LPARAM lParam);

  void SetStructPointer(PAPCHANNEL papc) { m_pAPChannel = papc; }
  bool InitFields(void);
  bool SaveFields(void);

private:
  PAPCHANNEL m_pAPChannel;
  Window m_ET;

};

/*
 * APWedgePage Class
 *
 * Derives from AdjustParamsPage to
 * provide a wrapper for Wedge session
 * settings
 */
class APWedgePage : public AdjustParamsPage {

public:
  APWedgePage() {
    m_pWedge = NULL;
  }
  ~APWedgePage() { }

  BOOL OnCreate(WPARAM wParam, LPARAM lParam);
  BOOL OnCommand(WPARAM wParam, LPARAM lParam);

  void SetStructPoitner(PAPWEDGE pws) { m_pWedge = pws; }
  bool InitFields(void);
  bool SaveFields(void);

private:
  bool _BrowseForWedgeFile(void);
  void _SetCurrentWedgeFile(CTSTRING &tsWedgeFile);

private:
  PAPWEDGE  m_pWedge;
  Window    m_WedgeFile;
  Window    m_Steps;
  Window    m_StartMsec;
  Window    m_StepMsec;
  Window    m_EndMsec;

};

/*
 * AdjustParamsDlg Class
 *
 * Wraps the dialog that allows
 * the user to adjust gamma/contrast/
 * brightness/etc. before shooting
 * a session.
 */
class AdjustParamsDlg : public Dialog {

public:
  AdjustParamsDlg() {
    
    m_GlobalPage.SetParentDlg(this);
    m_RedPage.SetParentDlg(this);
    m_GreenPage.SetParentDlg(this);
    m_BluePage.SetParentDlg(this);

  }
  ~AdjustParamsDlg() { }

  BOOL OnCreate(WPARAM wParam, LPARAM lParam);
  BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  BOOL OnNotify(WPARAM wParam, LPARAM lParam);
  BOOL OnClose(WPARAM wParam, LPARAM lParam);
  BOOL OnDestroy(WPARAM wParam, LPARAM lParam);

  typedef TreeView<int>::TVDATA APTVDATA, *PAPTVDATA;
  typedef TreeView<int>::TVNODE APTVNODE, *PAPTVNODE;

private:
  ADJUSTPARAMS m_Params;
  Serializer<ADJUSTPARAMS> m_Serializer;
  TreeView<int> m_TreeView;
  APGlobalPage m_GlobalPage;
  APChannelPage m_RedPage;
  APChannelPage m_GreenPage;
  APChannelPage m_BluePage;
  APFullImagePage m_FullPage;
  APWedgePage m_WedgePage;

  void _SetupControls(void);

  void _LoadDefaultParamsFile(void);
  void _SetCurrentParamsFile(CTSTRING &tsPath);

  bool _ReadSettings(CTSTRING &tsConfPath);
  bool _WriteSettings(CTSTRING &tsConfPath);

  bool _SaveFile(void);
  bool _LoadFile(void);
  // REMOVED: 1/13/2004 10:16:03 PM bool _LoadPreviewImage(void);
  // REMOVED: 1/13/2004 10:16:00 PM bool _DisplayPreviewImage(CTSTRING &tsImgFile);
  // REMOVED: 1/13/2004 10:16:05 PM void _SetCurrentPreviewImage(CTSTRING &tsPath);
 
  void _SetCurrentPageValue(int iPageID);
  bool _CreatePages(void);
  void _SwitchToPage(int iPageID, bool bSelNode = false);

};

/*
 * Global Functions
 */
double GetFloatValue(Window &Wnd);
bool SetFloatValue(double dFloat, HWND hWnd);
DWORD GetDWORDValue(Window &Wnd);
bool SetDWORDValue(DWORD dwValue, HWND hWnd);

#endif // !_ADJUSTPARAMSDLG_H
