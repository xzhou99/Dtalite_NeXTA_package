#pragma once


// CDlg_ImageSettings dialog

class CDlg_ImageSettings : public CDialog
{
	DECLARE_DYNAMIC(CDlg_ImageSettings)

public:
	CDlg_ImageSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ImageSettings();

	CTLiteDoc *m_pDoc;
// Dialog Data
	enum { IDD = IDD_DIALOG_Image_Width };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditWidth();
	float m_RealworldWidth;
	float m_RealworldHeight;
	afx_msg void OnBnClickedOk();
	float m_X1, m_X2, m_Y1,m_Y2;
	afx_msg void OnEnChangeEditLeft();
	CString m_Image_File_Message;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonReset();
};
