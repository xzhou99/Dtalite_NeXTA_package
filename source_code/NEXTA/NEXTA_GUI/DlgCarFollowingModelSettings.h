#pragma once


// CDlgCarFollowingModelSettings dialog

class CDlgCarFollowingModelSettings : public CDialog
{
	DECLARE_DYNAMIC(CDlgCarFollowingModelSettings)

public:
	CDlgCarFollowingModelSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCarFollowingModelSettings();

// Dialog Data
	enum { IDD = IDD_DIALOG_CarFollowingSettings };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_Kjam;
	float m_WaveSpeed;
	afx_msg void OnBnClickedOk();
	float m_FreeflowSpeed;
};
