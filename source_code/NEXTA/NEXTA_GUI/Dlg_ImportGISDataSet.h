#pragma once

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// CDlg_ImportGISDataSet dialog

class CDlg_ImportGISDataSet : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDlg_ImportGISDataSet)

public:

	CString m_ImportIniFile;
	CDlg_ImportGISDataSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ImportGISDataSet();
// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_DIALOG_GIS_Import, IDH = IDR_HTML_DLG_IMPORTGISDATASET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnBnClickedOk();
};
