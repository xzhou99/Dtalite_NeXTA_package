// Dlg_DemandType.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_DemandType.h"


// CDlg_DemandType dialog

IMPLEMENT_DYNCREATE(CDlg_DemandType, CDHtmlDialog)

CDlg_DemandType::CDlg_DemandType(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CDlg_DemandType::IDD, CDlg_DemandType::IDH, pParent)
{

}

CDlg_DemandType::~CDlg_DemandType()
{
}

void CDlg_DemandType::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CDlg_DemandType::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

		std::vector<std::string> m_Column_names;




      


	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CDlg_DemandType, CDHtmlDialog)
	ON_BN_CLICKED(IDOK, &CDlg_DemandType::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DemandTypeLIST, &CDlg_DemandType::OnLvnItemchangedDemandtypelist)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDlg_DemandType)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CDlg_DemandType message handlers

HRESULT CDlg_DemandType::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CDlg_DemandType::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

void CDlg_DemandType::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}

void CDlg_DemandType::OnLvnItemchangedDemandtypelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
