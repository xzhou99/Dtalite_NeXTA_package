#pragma once
#include "TLiteDoc.h"
#include "GridCtrl_src\\GridCtrl.h"
#include "CGridListCtrlEx\\CGridListCtrlEx.h"

#include "afxcmn.h"

#ifdef _WIN32_WCE
#error "CDHtmlDialog is not supported for Windows CE."
#endif 

// CDlg_DemandType dialog

class CDlg_DemandType : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDlg_DemandType)

public:
	CDlg_DemandType(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_DemandType();

	CTLiteDoc* m_pDoc;

// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_DIALOG_Demand_Type, IDH = IDR_HTML_DLG_DEMANDTYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedDemandtypelist(NMHDR *pNMHDR, LRESULT *pResult);
};
