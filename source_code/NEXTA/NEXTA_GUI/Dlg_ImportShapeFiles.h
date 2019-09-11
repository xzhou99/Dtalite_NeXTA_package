#pragma once
#include <vector>
#include "TLiteDoc.h"
// CPage_Node_Movement dialog
#include "CGridListCtrlEx\\CGridListCtrlEx.h"

// CDlg_ImportShapeFiles dialog

class CDlg_ImportShapeFiles : public CDialog
{
	DECLARE_DYNAMIC(CDlg_ImportShapeFiles)

public:
	CDlg_ImportShapeFiles(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ImportShapeFiles();
	CGridListCtrlEx m_ListCtrl;

// Dialog Data
	enum { IDD = IDD_DIALOG_SHAPEFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_NodeShapeFile;
	CString m_LinkShapeFile;
	CString m_ZoneShapeFile;
	afx_msg void OnBnClickedButtonFindNodeFile();
	afx_msg void OnBnClickedButtonFindLinkFile();
	afx_msg void OnBnClickedButtonFindZoneFile();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedLink();
};
