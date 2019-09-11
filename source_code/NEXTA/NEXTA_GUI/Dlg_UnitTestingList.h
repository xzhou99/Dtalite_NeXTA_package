#pragma once
#include "afxcmn.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "BaseDialog.h"
#include "CGridListCtrlEx\\CGridListCtrlEx.h"
#include "afxwin.h"

// CDlg_UnitTestingList dialog

class CDlg_UnitTestingList : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlg_UnitTestingList)
	CTLiteDoc* m_pDoc;
public:
	CDlg_UnitTestingList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_UnitTestingList();

// Dialog Data
	enum { IDD = IDD_DIALOG_UNIT_TEST_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CGridListCtrlEx m_ListCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
