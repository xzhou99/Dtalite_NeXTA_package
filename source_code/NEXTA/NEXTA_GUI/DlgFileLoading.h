#pragma once
#include "afxwin.h"


// CDlgFileLoading dialog
#include "TLiteDoc.h"

class CDlgFileLoading : public CDialog
{
	DECLARE_DYNAMIC(CDlgFileLoading)

public:
	CDlgFileLoading(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFileLoading();

	CTLiteDoc* m_pDoc;

// Dialog Data
	enum { IDD = IDD_DIALOG_FILE_LOADING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListBox_NetworkData;
	CListBox m_ListBox_SensorData;
	virtual BOOL OnInitDialog();
	CListBox m_ListBox_OutputData;
	afx_msg void OnLbnSelchangeListSensorFile();
};
