//Dlg_KML_Configuration.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "TLiteDoc.h"
#include "TLiteView.h"

#include "Dlg_KML_Configuration.h"


// CDlg_KML_Configuration dialog


IMPLEMENT_DYNAMIC(CDlg_KML_Configuration, CDialog)

CDlg_KML_Configuration::CDlg_KML_Configuration(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_KML_Configuration::IDD, pParent)
	, m_KML_Height_Ratio(0.01)
	, m_Transparency(0)
	, m_BandWidth(0)
	, m_MaxHeightValue(1)
{
	m_BandWidth = 20;
	m_KML_Height_Ratio = 0.01;
	m_Transparency = 50;

	for(int i = 0; i <=7; i++)
	{
	m_ColorCategoryValue[i] = i*1000;
	}
}

CDlg_KML_Configuration::~CDlg_KML_Configuration()
{
}

void CDlg_KML_Configuration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_KML_Height_Ratio);
	DDX_Text(pDX, IDC_EDIT_Transparency, m_Transparency);
	DDV_MinMaxInt(pDX, m_Transparency, 0, 100);
	DDX_Text(pDX, IDC_EDIT_L1, m_ColorCategoryValue[1]);
	DDX_Text(pDX, IDC_EDIT_L2, m_ColorCategoryValue[2]);
	DDX_Text(pDX, IDC_EDIT_L3, m_ColorCategoryValue[3]);
	DDX_Text(pDX, IDC_EDIT_L4, m_ColorCategoryValue[4]);
	DDX_Text(pDX, IDC_EDIT_L5, m_ColorCategoryValue[5]);
	DDX_Text(pDX, IDC_EDIT_L6, m_ColorCategoryValue[6]);
	DDX_Text(pDX, IDC_EDIT_L7, m_ColorCategoryValue[7]);
	DDX_Text(pDX, IDC_EDIT_BandWidth, m_BandWidth);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_BITMAP, m_ColorRampStatic);
	DDX_Control(pDX, IDC_COMBO_Height, m_Height_ComboBox);
}


BEGIN_MESSAGE_MAP(CDlg_KML_Configuration, CDialog)
	ON_BN_CLICKED(IDOK, &CDlg_KML_Configuration::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlg_KML_Configuration::OnLbnSelchangeList1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_REVERSE, &CDlg_KML_Configuration::OnBnClickedButtonReverse)
	ON_BN_CLICKED(IDOK3, &CDlg_KML_Configuration::OnBnClickedOk3)
	ON_BN_CLICKED(IDOK4, &CDlg_KML_Configuration::OnBnClickedOk4)
	ON_CBN_SELCHANGE(IDC_COMBO_Height, &CDlg_KML_Configuration::OnCbnSelchangeComboHeight)

	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &CDlg_KML_Configuration::OnEnChangeEditHeight)
END_MESSAGE_MAP()


// CDlg_KML_Configuration message handlers

void CDlg_KML_Configuration::OnBnClickedButtonSpaceevenly()
{
	// TODO: Add your control notification handler code here
}

void CDlg_KML_Configuration::OnBnClickedOk()
{
	OnOK();
}
void CDlg_KML_Configuration::UpdateCategoryValues()
{

	std::vector<CTLiteDoc*> m_pProjectDocVector;
	std::vector<CString> m_ProjectTitleVector;

		std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
	int ProjectNo = 1;
	while (iDoc != g_DocumentList.end())
	{
		if ((*iDoc)->m_NodeSet.size()>0)
		{

			CString ProjectString;
			ProjectString.Format ("%d: %s", ProjectNo, (*iDoc)->m_ProjectTitle );
			
			m_ProjectTitleVector.push_back(ProjectString);
			m_pProjectDocVector.push_back ((*iDoc));

			ProjectNo++;
		}
		iDoc++;
	}


	///
	m_KML_MOE_selection  = (eKML_ColorMode) m_List.GetCurSel ();

	std::list<DTALink*>::iterator iLink;
	float max_link_volume = 0;

	float max_value = 0;
	float min_value = 999999;


		CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_L1);
		CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_L2);
		CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT_L3);
		CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT_L4);
		CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT_L5);
		CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT_L6);
		CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT_L7);

	if(m_KML_MOE_selection == e_KML_green ||
		m_KML_MOE_selection == e_KML_yellow ||
		m_KML_MOE_selection == e_KML_red)
	{

		for(int i = 1; i<=7; i++)
		{
			m_ColorCategoryValue[i] = 0;
		}


		pEdit1->EnableWindow (0);
		pEdit2->EnableWindow (0);
		pEdit3->EnableWindow (0);
		pEdit4->EnableWindow (0);
		pEdit5->EnableWindow (0);
		pEdit6->EnableWindow (0);
		pEdit7->EnableWindow (0);



	}else
	{
		pEdit1->EnableWindow (1);
		pEdit2->EnableWindow (1);
		pEdit3->EnableWindow (1);
		pEdit4->EnableWindow (1);
		pEdit5->EnableWindow (1);
		pEdit6->EnableWindow (1);
		pEdit7->EnableWindow (1);


	for (iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
	{
		DTALink* pLink = (*iLink); 
		float KML_color_value = 0;

		pLink->KML_single_color_code = -1;

		pLink->KML_color_value  = 0;
		switch (m_KML_MOE_selection)
		{
		case e_KML_green: pLink->KML_single_color_code =  1 ; break; 
		case e_KML_yellow: pLink->KML_single_color_code = 3 ; break; 
		case e_KML_red: pLink->KML_single_color_code = 6 ; break; 

		case e_KML_speed_limit: pLink->KML_color_value  =  pLink->m_SpeedLimit  ; break; 
		case e_KML_avg_speed: pLink->KML_color_value =  pLink->m_avg_simulated_speed; break;
		case e_KML_avg_speed_to_speed_limit_raio: 
			 pLink->KML_color_value =  pLink->m_avg_simulated_speed/max(1,  pLink->m_SpeedLimit); break;

		case e_KML_user_defined_attribue: 
			
					if(m_pProjectDocVector.size()>=2)
					{
						DTALink* pLink1= m_pProjectDocVector[0]->FindLinkWithNodeNumbers(pLink->m_FromNodeNumber , pLink->m_ToNodeNumber );
						DTALink* pLink2= m_pProjectDocVector[1]->FindLinkWithNodeNumbers(pLink->m_FromNodeNumber , pLink->m_ToNodeNumber );

						
					pLink->KML_color_value  = pLink1 ->m_total_link_volume - pLink2->m_total_link_volume;
					}


			 break; 
		}

		max_value = max(max_value, pLink->KML_color_value );
		min_value = min(min_value, pLink->KML_color_value );

	}
	min_value = 0;

	m_ColorCategoryValue[1] = min_value;
	m_ColorCategoryValue[7] = max_value;

	float incremental = (max_value - min_value)/ 6;

	for(int i = 2; i<=6; i++)
	{
		m_ColorCategoryValue[i] = m_ColorCategoryValue[i-1] + incremental;
	}

	}
	UpdateData(0);
}

void CDlg_KML_Configuration::OnLbnSelchangeList1()
{
	UpdateCategoryValues();



}

BOOL CDlg_KML_Configuration::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Height_ComboBox.AddString ("Default Height of 1");
	m_Height_ComboBox.AddString("Total Hourly Link Volume");
	m_Height_ComboBox.AddString("Total Delay");

	m_Height_ComboBox.SetCurSel(0);

	m_List.AddString ("Green");
	m_List.AddString ("Yellow");
	m_List.AddString ("Red");
	m_List.AddString ("Speed Limit");
	m_List.AddString ("Avg Speed");
	m_List.AddString ("Volume Difference of Project 1 - 2 ");

	m_List.SetCurSel (3);

	UpdateCategoryValues();
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_KML_Configuration::OnPaint()
{
   CPaintDC dc(this); 
   CPen penLOS[MAX_LOS_SIZE];
   CBrush brushLOS[MAX_LOS_SIZE];

   int i;

   if(m_pDoc==NULL)
	   return;
   for(i = 1; i<MAX_LOS_SIZE-1; i++)
   {
      penLOS[i].CreatePen(PS_SOLID, 1, m_pDoc->m_colorLOS[i]);
      brushLOS[i].CreateSolidBrush(m_pDoc->m_colorLOS[i]);
   }

   dc.SetBkMode(TRANSPARENT);

    CRect entire_rect;

	m_ColorRampStatic.GetWindowRect(&entire_rect);
	ScreenToClient(&entire_rect); 

   
   CRect lr;

   float height = entire_rect.Height () / 6.0;

	  for(i = 1; i< MAX_LOS_SIZE-1; i++)
   {
      lr.top = entire_rect.top +height*(i-1) ;
      lr.bottom = lr.top + height;

      lr.left = entire_rect.left;
      lr.right = entire_rect.right; 

	  if(m_pDoc->m_ColorDirection==-1)  // reverse direction 
	  {
	  dc.SelectObject(penLOS[MAX_LOS_SIZE-1-i]);
      dc.SelectObject(brushLOS[MAX_LOS_SIZE-1-i]);
	  }else
	  {
	  dc.SelectObject(penLOS[i]);
      dc.SelectObject(brushLOS[i]);
  	  }

	  CString lengend_interval_str;
      dc.Rectangle(lr);
	}

}

void CDlg_KML_Configuration::OnBnClickedButtonReverse()
{
	if(m_pDoc->m_ColorDirection == 1)
		m_pDoc->m_ColorDirection = -1;
	else
		m_pDoc->m_ColorDirection = 1;

	Invalidate();
}

void CDlg_KML_Configuration::SelchangeComboHeight()
{
}



void CDlg_KML_Configuration::OnBnClickedOk3()
{
	UpdateData(1);

	CString directory  = m_pDoc->m_ProjectDirectory ;

	int SelNo = m_Height_ComboBox.GetCurSel ();

	double hour = max(1,( m_pDoc->m_DemandLoadingEndTimeInMin - m_pDoc->m_DemandLoadingStartTimeInMin)/60.0);
		for (std::list<DTALink*>::iterator  iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
	{

	DTALink* pLink = (*iLink); 
	
	if(SelNo == 0)
	{
		pLink->m_UserDefinedHeight = 0.01;
	}
	
	if(SelNo == 1)  // volume 
	{
		pLink->m_UserDefinedHeight = pLink->m_total_link_volume/hour;

		if(pLink->m_total_link_volume < 1)
			pLink->m_UserDefinedHeight = 0;
	}
		
	if(SelNo == 2)  // delay
	{
		pLink->m_UserDefinedHeight = pLink->m_total_delay/hour;
	}
}

	CString KML_Link_3D_File = directory+"AMS_link.kml";
	DeleteFile(KML_Link_3D_File);
	m_pDoc->ExportLinkSingleAttributeLayerToKMLFiles(KML_Link_3D_File,"LIBKML",m_BandWidth, m_Transparency,m_ColorCategoryValue, m_KML_Height_Ratio);
	HINSTANCE result = ShellExecute(NULL, _T("open"), KML_Link_3D_File, NULL,NULL, SW_SHOW);

}


void CDlg_KML_Configuration::OnBnClickedOk4()
{
	UpdateData(1);

	CString directory  = m_pDoc->m_ProjectDirectory ;
	DeleteFile(directory+"AMS_link.shp");
	m_pDoc->ExportLinkLayerToGISFiles(directory+"AMS_link.shp","ESRI Shapefile");
	CString SHP_Link_File = directory+"AMS_link.shp";

	HINSTANCE result = ShellExecute(NULL, _T("open"), SHP_Link_File, NULL,NULL, SW_SHOW);

}

void CDlg_KML_Configuration::OnCbnSelchangeComboHeight()
{
	SelchangeComboHeight();

}

void CDlg_KML_Configuration::OnEnChangeEditHeightMax()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CDlg_KML_Configuration::OnEnChangeEditHeight()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
