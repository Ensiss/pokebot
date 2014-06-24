#if !defined(AFX_MEMORYVIEWER_H__52C50474_5399_4D0B_A3E4_4C52C4E0EAA0__INCLUDED_)
#define AFX_MEMORYVIEWER_H__52C50474_5399_4D0B_A3E4_4C52C4E0EAA0__INCLUDED_

#include "../System.h"  // Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MemoryViewer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MemoryViewer window

class IMemoryViewerDlg {
 public:
  virtual void setCurrentAddress(u32 address)=0;
};

class MemoryViewer : public CWnd
{
  u32 address;
  int addressSize;
  int dataSize;
  bool hasCaret;
  int caretWidth;
  int caretHeight;
  HFONT font;
  CSize fontSize;
  u32 editAddress;
  int editNibble;
  int maxNibble;
  int displayedLines;
  int beginAscii;
  int beginHex;
  bool editAscii;
  IMemoryViewerDlg *dlg;

  static bool isRegistered;
  // Construction
 public:
  MemoryViewer();

  // Attributes
 public:

  // Operations
 public:
  virtual void readData(u32,int,u8 *) = 0;
  virtual void editData(u32,int,int,u32)=0;

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(MemoryViewer)
  //}}AFX_VIRTUAL

  // Implementation
 public:
  int getSize();
  u32 getCurrentAddress();
  void setAddressSize(int s);
  void registerClass();
  void beep();
  bool OnEditInput(UINT c);
  void moveAddress(s32 offset, int nibbleOff);
  void setCaretPos();
  void destroyEditCaret();
  void createEditCaret(int w, int h);
  void updateScrollInfo(int lines);
  void setSize(int s);
  void setAddress(u32 a);
  void setDialog(IMemoryViewerDlg *d);
  virtual ~MemoryViewer();

  // Generated message map functions
 protected:
  //{{AFX_MSG(MemoryViewer)
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnPaint();
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg UINT OnGetDlgCode();
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
    afx_msg LRESULT OnWMChar(WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMORYVIEWER_H__52C50474_5399_4D0B_A3E4_4C52C4E0EAA0__INCLUDED_)
