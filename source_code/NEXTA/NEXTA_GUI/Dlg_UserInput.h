#pragma once


// CDlg_UserInput dialog

class CDlg_UserInput : public CDialog
{
	DECLARE_DYNAMIC(CDlg_UserInput)

public:
	CDlg_UserInput(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_UserInput();

// Dialog Data
	enum { IDD = IDD_DIALOG_USER_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_StrQuestion;
	CString m_InputValue;
};
