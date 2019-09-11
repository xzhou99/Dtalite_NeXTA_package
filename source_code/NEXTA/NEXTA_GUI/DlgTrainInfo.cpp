// DlgTrainInfo.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "DlgTrainInfo.h"


// CDlgTrainInfo dialog

#define NUM_TRAIN_INFO 13

static _TCHAR *_gColumnTrainLabel[NUM_TRAIN_INFO] =
{
	_T("Train Header"), _T("Origin Node"), _T("Destination Node"),
	_T("Direction"), _T("Speed Multiplier"), _T("TOB"), _T("Hazmat"),_T("SA Status at Origin"),_T("Entry Time"),  _T("Terminal Want Time"), _T("Acutal Arrival Time"), _T("Schedule Delay at Terminal (min)"),  _T("Main Track Delay")
};


static int _gColumnWidth[NUM_TRAIN_INFO] =
{
	100, 80, 80, 90, 80, 100, 80, 80, 120, 120, 120, 120, 120
};

IMPLEMENT_DYNAMIC(CDlgTrainInfo, CDialog)

CDlgTrainInfo::CDlgTrainInfo(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgTrainInfo::IDD, pParent)
{

}

CDlgTrainInfo::~CDlgTrainInfo()
{
}

void CDlgTrainInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRAIN_LIST_CONTROL, m_TrainListControl);
}


BEGIN_MESSAGE_MAP(CDlgTrainInfo, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TRAIN_LIST_CONTROL, &CDlgTrainInfo::OnLvnItemchangedTrainListControl)
END_MESSAGE_MAP()


// CDlgTrainInfo message handlers
BOOL CDlgTrainInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_SmallImageList.Create(IDB_PATH_ICON, 16, 1, RGB(255, 255, 255));
	m_SmallImageList.SetOverlayImage(5, 1);

	m_TrainListControl.SetImageList(&m_SmallImageList, LVSIL_SMALL);

	// insert columns

	int i;
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for(i = 0; i<NUM_TRAIN_INFO; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = _gColumnTrainLabel[i];
		lvc.cx = _gColumnWidth[i];
		lvc.fmt = LVCFMT_LEFT;
		m_TrainListControl.InsertColumn(i,&lvc);
	}

	m_TrainListControl.SetExtendedStyle(LVS_EX_AUTOSIZECOLUMNS | LVS_EX_FULLROWSELECT |LVS_EX_HEADERDRAGDROP);

	InsertTrainInfoItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgTrainInfo::InsertTrainInfoItem()
{
//	m_TrainListControl.DeleteAllItems ();
	// insert items

	

		std::map<string, train_info>	:: const_iterator itr;

		unsigned int i = 0;
		for(itr = m_pDoc->m_train_map.begin(); itr != m_pDoc->m_train_map.end(); ++itr, i++)
		{

		LV_ITEM lvi;
		char text[100];

		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = i;
		lvi.iSubItem = 0;

		train_info train = (*itr).second ;

		sprintf_s(text, "%s",train.train_header.c_str () );

		lvi.pszText = text;
		int ImageNo = 2;
		if(train.direction == "EASTBOUND")
			ImageNo = 0;
		if(train.direction == "WESTBOUND")
			ImageNo = 1;
		
		lvi.iImage = ImageNo;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.state = INDEXTOSTATEIMAGEMASK(ImageNo);

		int count = 1;
		m_TrainListControl.InsertItem(&lvi);

		sprintf_s(text, "%d",train.origin_node_id );
		m_TrainListControl.SetItemText(i,count++,text);

		sprintf_s(text, "%d",train.destination_node_id);
		m_TrainListControl.SetItemText(i,count++,text);

		sprintf_s(text, "%s",train.direction.c_str());
		m_TrainListControl.SetItemText(i,count++,text);

		sprintf_s(text, "%4.3f",train.speed_multiplier );
		m_TrainListControl.SetItemText(i,count++,text);

		sprintf_s(text, "%d",train.tob);
		m_TrainListControl.SetItemText(i,count++,text);

		sprintf_s(text, "%s",train.hazmat.c_str () );
		m_TrainListControl.SetItemText(i,count++,text);

		sprintf_s(text, "%d",train.sa_status_at_origin);
		m_TrainListControl.SetItemText(i,count++,text);

		sprintf_s(text, "%d (%s)",train.entry_time, m_pDoc->GetTimeStampString24HourFormat(train.entry_time));
		m_TrainListControl.SetItemText(i,count++,text);

		sprintf_s(text, "%d (%s) ",train.terminal_want_time,m_pDoc->GetTimeStampString24HourFormat(train.terminal_want_time));
		m_TrainListControl.SetItemText(i,count++,text);


		sprintf_s(text, "%d (%s)",train.terminal_arrival_time,m_pDoc->GetTimeStampString24HourFormat(train.terminal_arrival_time));
		m_TrainListControl.SetItemText(i,count++,text);

		sprintf_s(text, "%d ",train.terminal_arrival_time - train.terminal_want_time);
		m_TrainListControl.SetItemText(i,count++,text);

		sprintf_s(text, "%d min (%d sec)",train.main_track_speed_difference_delay_in_second/60,train.main_track_speed_difference_delay_in_second );
		m_TrainListControl.SetItemText(i,count++,text);

	}
}
void CDlgTrainInfo::OnLvnItemchangedTrainListControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_pDoc->m_SelectedTrainHeader.empty ();

	POSITION pos = m_TrainListControl.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int nSelectedRow = m_TrainListControl.GetNextSelectedItem(pos);

		char str[100];
		m_TrainListControl.GetItemText (nSelectedRow,0,str,20);
		
		m_pDoc->m_SelectedTrainHeader 	= str ;

		Invalidate();

		m_pDoc->UpdateAllViews(0);
	}
}
