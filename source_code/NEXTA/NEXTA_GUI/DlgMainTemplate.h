#pragma once
#include "afxwin.h"


// CDlgMainTemplate dialog

class CDlgMainTemplate : public CDialog
{
	DECLARE_DYNAMIC(CDlgMainTemplate)

public:
	CDlgMainTemplate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMainTemplate();

// Dialog Data
	enum { IDD = IDD_DIALOG_Visualization_Mode };

	int m_TemplateNo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_TemplateList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnDblclkList1();
};
