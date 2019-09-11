#pragma once


// CDlg_Find_Vehicle dialog

class CDlg_Find_Vehicle : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Find_Vehicle)

public:
	CDlg_Find_Vehicle(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Find_Vehicle();

// Dialog Data
	enum { IDD = IDD_DIALOG_Find_Vehicle };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_VehicleID;
};
