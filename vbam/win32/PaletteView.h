#if !defined(AFX_PALETTEVIEW1_H__0873E3FF_9486_4B2C_8EF0_59C3B4F47162__INCLUDED_)
#define AFX_PALETTEVIEW1_H__0873E3FF_9486_4B2C_8EF0_59C3B4F47162__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaletteView.h : header file
//

#include "ColorControl.h"
#include "PaletteViewControl.h"
#include "ResizeDlg.h"

class GBAPaletteViewControl : public PaletteViewControl {
 public:
  virtual void updatePalette();
};

/////////////////////////////////////////////////////////////////////////////
// PaletteView dialog

class PaletteView : public ResizeDlg, IUpdateListener
{
 private:
  GBAPaletteViewControl paletteView;
  GBAPaletteViewControl paletteViewOBJ;
  ColorControl colorControl;
  bool autoUpdate;
  // Construction
 public:
  void save(int which);
  PaletteView(CWnd* pParent = NULL);   // standard constructor
  ~PaletteView();
  afx_msg LRESULT OnPalInfo(WPARAM wParam, LPARAM lParam);

  // Dialog Data
  //{{AFX_DATA(PaletteView)
  enum { IDD = IDD_PALETTE_VIEW };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(PaletteView)
 protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
  //}}AFX_VIRTUAL

  virtual void update();

  // Implementation
 protected:

  // Generated message map functions
  //{{AFX_MSG(PaletteView)
  virtual BOOL OnInitDialog();
  afx_msg void OnSaveBg();
  afx_msg void OnSaveObj();
  afx_msg void OnRefresh2();
  afx_msg void OnAutoUpdate();
  afx_msg void OnClose();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedChangeBackdrop();
};

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALETTEVIEW1_H__0873E3FF_9486_4B2C_8EF0_59C3B4F47162__INCLUDED_)
