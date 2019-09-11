// DlgMOETab.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "DlgMOETabView.h"


// CDlgMOETabView dialog

IMPLEMENT_DYNAMIC(CDlgMOETabView, CDialog)

CDlgMOETabView::CDlgMOETabView(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMOETabView::IDD, pParent)
{
	m_number_of_MOE_files = 2;

}

CDlgMOETabView::~CDlgMOETabView()
{
}

void CDlgMOETabView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MOETAB, m_TabCtrl);
}

BOOL CDlgMOETabView::OnInitDialog()
{
	CDialog::OnInitDialog();


	std::vector<CString> MOE_FileName_List;
	
	MOE_FileName_List.push_back("output_ODMOE.csv");
	MOE_FileName_List.push_back("output_LinkMOE.csv");

	for (int i=0;i < MOE_FileName_List.size();i++)
	{

		TCITEM tcItem;
		tcItem.mask = TCIF_TEXT;
		tcItem.pszText = const_cast<LPTSTR>((LPCTSTR)MOE_FileName_List[i]);
		m_TabCtrl.InsertItem(i, &tcItem);

		string fileName = m_pDoc->m_ProjectDirectory + MOE_FileName_List[i] + ".csv";


		//add individual tab dialog
		p_Tabs[i] = new CDlgMOETabSubView(fileName);
		//p_Tabs[i]->SetTabText();
		p_Tabs[i]->Create(IDD_DIALOG_MOETABVIEW,&m_TabCtrl);

	}

	p_Tabs[0]->ShowWindow(SW_SHOW);

	for (int i=1;i<MOE_FileName_List.size();i++)
	{
		p_Tabs[i]->ShowWindow(SW_HIDE);
	}

	m_PrevTab=0;

	SetRectangle();

	return TRUE;
}

void CDlgMOETabView::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	m_TabCtrl.GetClientRect(&tabRect);
	m_TabCtrl.GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	p_Tabs[0]->SetWindowPos(&m_TabCtrl.wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < m_number_of_MOE_files; nCount++)
	{
		p_Tabs[nCount]->SetWindowPos(&m_TabCtrl.wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
}

BEGIN_MESSAGE_MAP(CDlgMOETabView, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MOETAB, &CDlgMOETabView::OnTcnSelchangeMoetab)
END_MESSAGE_MAP()


// CDlgMOETabView message handlers

void CDlgMOETabView::OnTcnSelchangeMoetab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	if (m_PrevTab == m_TabCtrl.GetCurSel())
		return;
	p_Tabs[m_PrevTab]->EnableWindow(FALSE);
	p_Tabs[m_PrevTab]->ShowWindow(SW_HIDE);
	m_PrevTab = m_TabCtrl.GetCurSel();
	p_Tabs[m_PrevTab]->EnableWindow(TRUE);
	p_Tabs[m_PrevTab]->ShowWindow(SW_SHOW);

	*pResult = 0;
}
