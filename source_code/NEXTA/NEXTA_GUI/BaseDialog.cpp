/////////////////////////////////////////////////////////////////////////////
//
// Author:	if (it works)
//				Author = _T("Patty You");
//			else
//				Author = _T("IUnknown");
// Date:	07/16/98
// email:   ptn@worldnet.att.net
/////////////////////////////////////////////////////////////////////////////
//
// BaseDialog.cpp : implementation file
#include "stdafx.h"
#include <afxpriv.h>
#include "BaseDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseDialog
IMPLEMENT_DYNAMIC(CBaseDialog, CDialog)

   CBaseDialog::CBaseDialog()
:CDialog()
{
   m_bIsModeless = FALSE; // assuming this is a modal dialog
   m_crBackground = GetSysColor(COLOR_3DFACE);
   m_wndbkBrush.CreateSolidBrush(m_crBackground);
}

CBaseDialog::CBaseDialog(UINT nIDTemplate, CWnd* pParentWnd)
   :CDialog(nIDTemplate, pParentWnd)
{
   m_bIsModeless = FALSE; // assuming this is a modal dialog
   m_crBackground = GetSysColor(COLOR_3DFACE);
   m_wndbkBrush.CreateSolidBrush(m_crBackground);
}

CBaseDialog::~CBaseDialog()
{
   if(m_wndbkBrush.GetSafeHandle())
      m_wndbkBrush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CBaseDialog, CDialog)
   //{{AFX_MSG_MAP(CBaseDialog)
   ON_WM_NCDESTROY()
ON_WM_CTLCOLOR()
ON_WM_CLOSE()
//}}AFX_MSG_MAP
ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
   END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBaseDialog message handlers

void CBaseDialog::PostNcDestroy()
{
   // TODO: Add your specialized code here and/or call the base class
   CDialog::PostNcDestroy();

   if(m_bIsModeless)
      delete this; // for modeless dialog only
}

void CBaseDialog::OnOK()
{
   if(!UpdateData())
      return;

   if(m_bIsModeless) // modeless dialog
      DestroyWindow();
   else
      CDialog::OnOK();
}

void CBaseDialog::OnCancel()
{
   if(m_bIsModeless) // modeless dialog
      DestroyWindow();
   else
      CDialog::OnCancel();
}

void CBaseDialog::OnNcDestroy()
{
   CDialog::OnNcDestroy();
}


void CBaseDialog::SetModelessFlag(bool bIsModeless)
{
   m_bIsModeless = bIsModeless;
}

void CBaseDialog::SetBackgroundColor(COLORREF crBackground)
{
   m_crBackground = crBackground;

   if(m_wndbkBrush.GetSafeHandle())
      m_wndbkBrush.DeleteObject();

   m_wndbkBrush.CreateSolidBrush(m_crBackground);
}

// This member function enables ON_UPDATE_COMMAND_UI message maps for the dialog
LRESULT CBaseDialog::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
   UpdateDialogControls(this, FALSE);
   return 0;
}

HBRUSH CBaseDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
   HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

   // TODO: Change any attributes of the DC here
   if (nCtlColor == CTLCOLOR_DLG)
      return (HBRUSH) m_wndbkBrush.GetSafeHandle();

   // TODO: Return a different brush if the default is not desired
   return hbr;
}


void CBaseDialog::OnClose()
{
   if(m_bIsModeless) // modeless dialog
      DestroyWindow();
   else 
   {
      CDialog::OnClose();
   }
   
  }
