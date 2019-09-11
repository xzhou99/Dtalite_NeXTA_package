// DlgMOETabSubView.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "DlgMOETabSubView.h"


#include <fstream>
#include <sstream>

#include "CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "CGridListCtrlEx\CGridRowTraitXP.h"


// CDlgMOETabSubView dialog

IMPLEMENT_DYNAMIC(CDlgMOETabSubView, CDialog)

CDlgMOETabSubView::CDlgMOETabSubView(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMOETabSubView::IDD, pParent)
{

}

CDlgMOETabSubView::~CDlgMOETabSubView()
{
}

void CDlgMOETabSubView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_GRIDLIST_MOE,m_ListCtrl);
}

BOOL CDlgMOETabSubView::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;
	m_ListCtrl.SetDefaultRowTrait(pRowTrait);

	std::ifstream inFile(m_FileName.c_str());
	if (inFile.is_open())
	{
		std::string line;
		std::string subStr;

		std::getline(inFile,line);
		std::istringstream iss(line);

		int n=0;

		while(std::getline(iss,subStr,','))
		{
			m_ListCtrl.InsertColumnTrait(n,subStr.c_str(),LVCFMT_LEFT,-1,-1, new CGridColumnTraitEdit());
			m_ListCtrl.SetColumnWidth(n,LVSCW_AUTOSIZE_USEHEADER);
			n++;
		}

		m_ListCtrl.SetColumnWidth(0,100);

		int line_no = 0;

		while(std::getline(inFile,line))
		{
			int nItem=-1;
			std::istringstream LineStringStream(line);
			int field_no = 0;
			while(std::getline(LineStringStream,subStr,','))
			{

				if (field_no == 0)
				{
					nItem = m_ListCtrl.InsertItem(LVIF_TEXT,line_no,subStr.c_str(),0,0,0,NULL);
				}
				else
				{
					m_ListCtrl.SetItemText(nItem,field_no,subStr.c_str());
				}
				field_no++;
			}
			line_no++;

		}
		inFile.close();
	}


	return TRUE;

}


BEGIN_MESSAGE_MAP(CDlgMOETabSubView, CDialog)
END_MESSAGE_MAP()


// CDlgMOETabSubView message handlers
