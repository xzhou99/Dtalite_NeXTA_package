#pragma once


// CDlg_ImportODDemand dialog

class CDlg_ImportODDemand : public CDialog
{
	DECLARE_DYNAMIC(CDlg_ImportODDemand)

public:
	CDlg_ImportODDemand(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_ImportODDemand();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMPORT_ODTABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_NumberOfZones;
};
