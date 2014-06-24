#if !defined(AFX_DISASSEMBLE_H__CA10E857_7D76_4B19_A62B_D0677040FD0F__INCLUDED_)
#define AFX_DISASSEMBLE_H__CA10E857_7D76_4B19_A62B_D0677040FD0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Disassemble.h : header file
//

#include "IUpdate.h"
#include "ResizeDlg.h"
#include "../System.h"  // Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// Disassemble dialog

class Disassemble : public ResizeDlg, IUpdateListener
{
  // Construction
 public:
  virtual void update();
  void refresh();
  int count;
  bool autoUpdate;
  u32 address;
  Disassemble(CWnd* pParent = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(Disassemble)
  enum { IDD = IDD_DISASSEMBLE };
  CEdit  m_address;
  CListBox  m_list;
  BOOL  m_c;
  BOOL  m_f;
  BOOL  m_i;
  BOOL  m_n;
  BOOL  m_t;
  BOOL  m_v;
  BOOL  m_z;
  int    mode;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(Disassemble)
 protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void PostNcDestroy();
  //}}AFX_VIRTUAL

  // Implementation
 protected:

  // Generated message map functions
  //{{AFX_MSG(Disassemble)
  afx_msg void OnAutoUpdate();
  afx_msg void OnAutomatic();
  afx_msg void OnArm();
  afx_msg void OnClose();
  afx_msg void OnGo();
  afx_msg void OnGopc();
  afx_msg void OnNext();
  afx_msg void OnRefresh();
  afx_msg void OnThumb();
  virtual BOOL OnInitDialog();
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISASSEMBLE_H__CA10E857_7D76_4B19_A62B_D0677040FD0F__INCLUDED_)
