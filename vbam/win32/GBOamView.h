#if !defined(AFX_GBOAMVIEW_H__FE8105E6_9693_479A_8C57_DEEA1B2EA3D6__INCLUDED_)
#define AFX_GBOAMVIEW_H__FE8105E6_9693_479A_8C57_DEEA1B2EA3D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GBOamView.h : header file
//

#include "BitmapControl.h"
#include "ZoomControl.h"
#include "ColorControl.h"

#include "IUpdate.h"
#include "ResizeDlg.h"

/////////////////////////////////////////////////////////////////////////////
// GBOamView dialog

class GBOamView : public ResizeDlg, IUpdateListener
{
 private:
  BITMAPINFO bmpInfo;
  u8 *data;
  int w;
  int h;
  int number;
  bool autoUpdate;
  BitmapControl oamView;
  ZoomControl oamZoom;
  ColorControl color;


  // Construction
 public:
  void updateScrollInfo();
  void save();
  void savePNG(const char *name);
  void saveBMP(const char *name);
  void render();
  void setAttributes(int y, int x, int tile, int flags);
  void paint();
  ~GBOamView();
  GBOamView(CWnd* pParent = NULL);   // standard constructor

  afx_msg LRESULT OnColInfo(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnMapInfo(WPARAM wParam, LPARAM lParam);
  virtual void update();

  // Dialog Data
  //{{AFX_DATA(GBOamView)
  enum { IDD = IDD_GB_OAM_VIEW };
  CEdit  m_sprite;
  BOOL  m_stretch;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(GBOamView)
 protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
  //}}AFX_VIRTUAL

  // Implementation
 protected:

  // Generated message map functions
  //{{AFX_MSG(GBOamView)
  virtual BOOL OnInitDialog();
  afx_msg void OnStretch();
  afx_msg void OnAutoUpdate();
  afx_msg void OnChangeSprite();
  afx_msg void OnClose();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GBOAMVIEW_H__FE8105E6_9693_479A_8C57_DEEA1B2EA3D6__INCLUDED_)
