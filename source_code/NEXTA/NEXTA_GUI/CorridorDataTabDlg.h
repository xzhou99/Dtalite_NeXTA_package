#pragma once

#include <vector>
#include <string>
#include "CGridListCtrlEx\\CGridListCtrlEx.h"

#include "TLiteDoc.h"
// CCorridorDataTabDlg dialog

class CCorridorDataTabDlg : public CDialog
{
	DECLARE_DYNAMIC(CCorridorDataTabDlg)

public:

	int m_nSelectedColumn;

	CTLiteDoc* m_pDoc;
	CCorridorDataTabDlg(CWnd* pParent = NULL);   // standard constructor
	CCorridorDataTabDlg(std::vector<std::string> names, std::vector<CString> default_value, std::vector<std::vector<std::string>> values);
	BOOL CopyRow();
	BOOL AddRow();
	BOOL AddRowForLink(int from_node_name,int to_node_name);
	BOOL DeleteRow();
	void ZoomToSelectedObject();
	CString GenerateRecordString();
	void SetTabText(CString s);
	CString GetTabText();
	int ValidityCheck();

	virtual ~CCorridorDataTabDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTING_TAB_CORRIDOR };

public:
	CGridListCtrlEx m_ListCtrl;
	std::vector<std::string> names;
	std::vector<std::vector<std::string>> values;
	std::vector<CString> default_value;

	int m_NumOfCols;
	int m_NumOfRows;

	int m_SelectedFromNodeName;
	int m_SelectedToNodeName;
	BOOL m_ZoomToSelectedObject;
	CString m_TabText;

	CImageList m_ImageList;

	void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeSettingTab(NMHDR *pNMHDR, LRESULT *pResult);
};
