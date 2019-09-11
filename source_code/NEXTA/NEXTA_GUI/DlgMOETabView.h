#pragma once
#include "TLiteDoc.h"
#include "DlgMOETabSubView.h"

// CDlgMOETabView dialog

class CDlgMOETabView : public CDialog
{
	DECLARE_DYNAMIC(CDlgMOETabView)

public:
	CDlgMOETabView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMOETabView();
	CTLiteDoc* m_pDoc;

	int m_number_of_MOE_files;
// Dialog Data
	enum { IDD = IDD_DIALOG_MOETAB };

private:
	CTabCtrl m_TabCtrl;
	CDlgMOETabSubView* p_Tabs[2];
	int m_PrevTab;

	void SetRectangle();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeMoetab(NMHDR *pNMHDR, LRESULT *pResult);
};
