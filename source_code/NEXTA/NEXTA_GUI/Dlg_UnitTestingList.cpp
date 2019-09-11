// DlgPathList.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_UnitTestingList.h"
#include "CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "CGridListCtrlEx\CGridRowTraitXP.h"
#include "MainFrm.h"
#include "Geometry.h"
#include "CSVParser.h"
#include <string>
#include <sstream>


// CDlg_UnitTestingList dialog
extern CDlg_UnitTestingList* g_pUnitTestingListDlg;

IMPLEMENT_DYNAMIC(CDlg_UnitTestingList, CDialog)

CDlg_UnitTestingList::CDlg_UnitTestingList(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlg_UnitTestingList::IDD, pParent)
{
}

CDlg_UnitTestingList::~CDlg_UnitTestingList()
{
	g_pUnitTestingListDlg = NULL;
}

void CDlg_UnitTestingList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDlg_UnitTestingList, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CDlg_UnitTestingList::OnLvnItemchangedList)
	ON_BN_CLICKED(IDOK, &CDlg_UnitTestingList::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlg_UnitTestingList message handlers
#define UNITTESTINGCOLUMNSIZE 6

BOOL CDlg_UnitTestingList::OnInitDialog()
{
		CDialog::OnInitDialog();

// Give better margin to editors
	m_ListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;  // Hao: this ponter should be delete. 
	m_ListCtrl.SetDefaultRowTrait(pRowTrait);

	std::vector<std::string> m_Column_names;

_TCHAR *ColumnMOELabel[UNITTESTINGCOLUMNSIZE] =
{
	_T("No."),_T("Category"),_T("Feature Name"), _T("Current Results"), _T("Expected Results"), _T("Notes")
};


	//Add Columns and set headers
	for (size_t i=0;i< UNITTESTINGCOLUMNSIZE;i++)
	{
		CGridColumnTrait* pTrait = NULL;
//		pTrait = new CGridColumnTraitEdit();
		m_ListCtrl.InsertColumnTrait((int)i,ColumnMOELabel[i],LVCFMT_LEFT,-1,-1, pTrait);
		m_ListCtrl.SetColumnWidth((int)i,LVSCW_AUTOSIZE_USEHEADER);
	}
	m_ListCtrl.SetColumnWidth(0, 80);

	//load unit-testing-feature-list

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	char FeatureListFile[_MAX_PATH];
	sprintf(FeatureListFile, "%s\\unit-testing-feature-list.csv", pMainFrame->m_CurrentDirectory);

	CCSVParser parser;
	if (parser.OpenCSVFile(FeatureListFile))
	{

		int i=0;
		while(parser.ReadRecord())
		{
			std::string feature_id,category,feature_name,results,expected_results,notes;

			if(!parser.GetValueByFieldName("feature_id",feature_id))
				feature_id = "";

			if(!parser.GetValueByFieldName("category",category))
				category = "";

			if(!parser.GetValueByFieldName("feature_name",feature_name))
				feature_name = "";

			if(!parser.GetValueByFieldName("results",results))
				results = "";

			if(!parser.GetValueByFieldName("expected_results",expected_results))
				expected_results = "";

			if(!parser.GetValueByFieldName("notes",notes))
				notes = "";

		char text[300];
		sprintf_s(text, "%s",feature_id.c_str ());
		int Index = m_ListCtrl.InsertItem(LVIF_TEXT,i,text , 0, 0, 0, NULL);
		m_ListCtrl.SetItemText(Index,1,category.c_str () );
		m_ListCtrl.SetItemText(Index,2,feature_name.c_str () );
		m_ListCtrl.SetItemText(Index,3,results.c_str () );
		m_ListCtrl.SetItemText(Index,4,expected_results.c_str () );
		m_ListCtrl.SetItemText(Index,5,notes.c_str () );

		i++;
		}
	}
		return true;
}

void CDlg_UnitTestingList::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	UpdateData(1);
	
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	while(pos!=NULL)
	{
		int nSelectedRow = m_ListCtrl.GetNextSelectedItem(pos);
		char str[100];
		m_ListCtrl.GetItemText (nSelectedRow,0,str,20);
		int LinkNo = atoi(str);
			m_pDoc->m_SelectedLinkNo = LinkNo;
			DTALink* pLink = m_pDoc->m_LinkNoMap [LinkNo];

			g_AddLinkIntoSelectionList(pLink,LinkNo, m_pDoc->m_DocumentNo);

	}

	Invalidate();
		
	m_pDoc->UpdateAllViews(0);

}

void CDlg_UnitTestingList::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}

void CDlg_UnitTestingList::OnBnClickedCancel()
{
	CDialog::OnOK();
}

