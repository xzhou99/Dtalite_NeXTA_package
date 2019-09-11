// Dlg_ImportGISDataSet.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_ImportGISDataSet.h"


// CDlg_ImportGISDataSet dialog

IMPLEMENT_DYNCREATE(CDlg_ImportGISDataSet, CDHtmlDialog)

CDlg_ImportGISDataSet::CDlg_ImportGISDataSet(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CDlg_ImportGISDataSet::IDD, CDlg_ImportGISDataSet::IDH, pParent)
{

}

CDlg_ImportGISDataSet::~CDlg_ImportGISDataSet()
{
}

void CDlg_ImportGISDataSet::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CDlg_ImportGISDataSet::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CDlg_ImportGISDataSet, CDHtmlDialog)
	ON_BN_CLICKED(IDOK, &CDlg_ImportGISDataSet::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDlg_ImportGISDataSet)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CDlg_ImportGISDataSet message handlers

HRESULT CDlg_ImportGISDataSet::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CDlg_ImportGISDataSet::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

void CDlg_ImportGISDataSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}
