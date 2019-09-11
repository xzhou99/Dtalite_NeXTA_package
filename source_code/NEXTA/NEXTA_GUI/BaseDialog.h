//   If you help write or modify the code, please also list your names here.
//   The reason of having Copyright info here is to ensure all the modified version, as a whole, under the GPL 
//   and further prevent a violation of the GPL.

// More about "How to use GNU licenses for your own software"
// http://www.gnu.org/licenses/gpl-howto.html


//    This file is part of NeXTA Version 3 (Open-source).

//    NEXTA is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    NEXTA is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with NEXTA.  If not, see <http://www.gnu.org/licenses/>.


#if !defined(AFX_BASEDIALOG_H__A6AE4F31_1C29_11D2_BC27_0060970A2B51__INCLUDED_)
#define AFX_BASEDIALOG_H__A6AE4F31_1C29_11D2_BC27_0060970A2B51__INCLUDED_

#if _MSC_VER >= 1000
#pragma	once
#endif // _MSC_VER >= 1000
// BaseDialog.h : header file
/////////////////////////////////////////////////////////////////////////////
// CBaseDialog window

class CBaseDialog : public CDialog
{
   DECLARE_DYNAMIC(CBaseDialog)

      // Construction
      public:
      CBaseDialog();
   CBaseDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);

   // Attributes
public:

   // Operations
public:

   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CBaseDialog)
protected:
   virtual void PostNcDestroy();
   //}}AFX_VIRTUAL
protected:
   virtual void OnOK();
   virtual void OnCancel();

   // Implementation
public:
   void SetBackgroundColor(COLORREF crBackground);
   void SetModelessFlag(bool bIsModeless);
   virtual ~CBaseDialog();

   // Generated message map functions
protected:
   COLORREF m_crBackground;
   bool m_bIsModeless;
   CBrush m_wndbkBrush;	// background brush

   //{{AFX_MSG(CBaseDialog)
   afx_msg void OnNcDestroy();
   afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
   afx_msg void OnClose();
   //}}AFX_MSG
   afx_msg LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam); // works for modal dialog
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEDIALOG_H__A6AE4F31_1C29_11D2_BC27_0060970A2B51__INCLUDED_)
