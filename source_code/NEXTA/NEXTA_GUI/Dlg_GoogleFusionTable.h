#pragma once
#include "TLiteDoc.h"

// CDlg_GoogleFusionTable dialog

class CDlg_GoogleFusionTable : public CDialog
{
	DECLARE_DYNAMIC(CDlg_GoogleFusionTable)

public:
	CTLiteDoc* m_pDoc;
	CDlg_GoogleFusionTable(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_GoogleFusionTable();

// Dialog Data
	enum { IDD = IDD_DIALOG_Goolge_Fusion_Table };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonDownload();
	CString m_ProjectFolder;
	afx_msg void OnBnClickedButtonUpload();
	CString m_NodeTableID;
	CString m_LinkTableID;
	CString m_UploadNodeTableID;
	CString m_UploadLinkTableID;
	afx_msg void OnEnChangeEditExcelfile5();
	afx_msg void OnBnClickedButtonLinkWeb();
	afx_msg void OnBnClickedButtonLinkWebSubarea();
	long m_SubareaLinkSize;
	afx_msg void OnEnChangeEditExcelfile();
	afx_msg void OnEnChangeEditExcelfile2();
};
