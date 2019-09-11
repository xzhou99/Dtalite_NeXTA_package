#pragma once


// CDlg_Information dialog

class CDlg_Information : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Information)

public:
	bool m_bLoadGISSampleFile;

	CString m_OnLineDocumentLink;
	CString m_SampleFileDirectory;
	CDlg_Information(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Information();

// Dialog Data
	enum { IDD = IDD_DIALOG_INFORMATION_WITH_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOnlineDocument();
	CString m_InformationString;
	CString m_StringInfo;
	afx_msg void OnBnClickedButtonSamplefile();
};
