#pragma once
#include "afxwin.h"
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"

// CDlgDefaultLinkProperties dialog

class CDlgDefaultLinkProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgDefaultLinkProperties)

public:
	CDlgDefaultLinkProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgDefaultLinkProperties();

	bool m_bLongLatSystem; 
	int m_StartNodeNumberForNewNodes;
// Dialog Data
	enum { IDD = IDD_DIALOG_DefaultLinkProperties };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CTLiteDoc* m_pDoc;
	
	float SpeedLimit;
	int LaneCapacity;
	int NumLanes;
	int LinkType;
	afx_msg void OnLbnSelchangeList1();
	virtual BOOL OnInitDialog();
	CListBox m_LinkTypeList;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComboBox m_CoordinateSystemList;
	afx_msg void OnCbnSelchangeCombo1();

};
