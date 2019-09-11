#include "StdAfx.h"
#include "MyPropertySheet.h"

CMyPropertySheet::CMyPropertySheet(void)
{
	CPropertySheet::CPropertySheet();
}
CMyPropertySheet::CMyPropertySheet(CString str)
{
	CPropertySheet::CPropertySheet(str);
}

CMyPropertySheet::~CMyPropertySheet(void)
{
}

BOOL CMyPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();
		CWnd* p = GetDlgItem(IDOK);
		p->SetWindowText("Confirm");
		p = GetDlgItem(IDCANCEL);
		p->SetWindowText("Cancel");
		return true;
}