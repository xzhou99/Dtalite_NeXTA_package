#pragma once
#include "afxdlgs.h"

class CMyPropertySheet :
	public CPropertySheet
{
public:
	CMyPropertySheet(void);
	CMyPropertySheet(CString str);
	~CMyPropertySheet(void);
	BOOL CMyPropertySheet::OnInitDialog();
};
