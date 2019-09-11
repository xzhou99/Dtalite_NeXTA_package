#pragma once


// CDlgNetworkAlignment dialog

class CDlgNetworkAlignment : public CDialog
{
	DECLARE_DYNAMIC(CDlgNetworkAlignment)

public:
	CDlgNetworkAlignment(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNetworkAlignment();

// Dialog Data
	enum { IDD = IDD_DIALOG_NETWORK_ALIGNMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_Node1;
	int m_Node2;
	float m_NodeY1;
	float m_NodeY2;
	float m_NodeX1;
	float m_NodeX2;
	afx_msg void OnBnClickedOk();
};
