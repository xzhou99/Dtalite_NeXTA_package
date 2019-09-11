#pragma once
#include "afxcmn.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "BaseDialog.h"
#include "CGridListCtrlEx\\CGridListCtrlEx.h"
#include "afxwin.h"

// CDlgLinkList dialog
enum eLinkSelectionMode {eLinkSelection_AllLinks = 0,
eLinkSelection_SelectedLinksOnly,
eLinkSelection_WithinSubarea,
eLinkSelection_FreewayOnly,eLinkSelection_HighwayOnly,eLinkSelection_RampOnly,eLinkSelection_ArterialOnly,eLinkSelection_NoConnectors};

class CDlgLinkList : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgLinkList)
	CTLiteDoc* m_pDoc;
	CTLiteDoc* m_pDoc2;
	bool m_bDoc2Ready;


	bool ExportDataToCSVFile(CString csv_file);

public:

	std::vector <int> m_ListedLinkNoVector;
	eLinkSelectionMode m_LinkSelectionMode;
	bool m_AVISensorFlag;
	CDlgLinkList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLinkList();

	void ReloadData();
// Dialog Data
	enum { IDD = IDD_DIALOG_LINK_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CGridListCtrlEx m_ListCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLinkselectionShowalllinks();
	afx_msg void OnLinkselectionShowhighwaylinksonly();
	afx_msg void OnLinkselectionShowramplinksonly();
	afx_msg void OnLinkselectionShowarteriallinksonly();
	afx_msg void OnLinkselectionShowalllinksexceptconnectors();
	afx_msg void OnUpdateLinkselectionShowalllinks(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLinkselectionShowhighwaylinksonly(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLinkselectionShowramplinksonly(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLinkselectionShowarteriallinksonly(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLinkselectionShowalllinksexceptconnectors(CCmdUI *pCmdUI);
	BOOL m_ZoomToSelectedLink;
	CString m_StrDocTitles;

	afx_msg void OnBnClickedCheckZoomToSelectedLink();
	afx_msg void OnBnClickedBarchartpiechart();
	afx_msg void OnLinkselectionShowselectedlinksonly();
	afx_msg void OnUpdateLinkselectionShowselectedlinksonly(CCmdUI *pCmdUI);
	CComboBox m_ComboBox;
	afx_msg void OnCbnSelchangeComboLinkSelection();
	afx_msg void OnBnClickedExport();
	CComboBox m_StartHour;
	CComboBox m_EndHour;
	afx_msg void OnCbnSelchangeComboStarttime();
	afx_msg void OnCbnSelchangeComboEndtime();
	afx_msg void OnStnClickedDocTitle();
};
