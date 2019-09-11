// Dlg_KML_Configuration.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_TDMOE_Configuration.h"


// CDlg_TDMOE_Configuration dialog


IMPLEMENT_DYNAMIC(CDlg_TDMOE_Configuration, CDialog)

CDlg_TDMOE_Configuration::CDlg_TDMOE_Configuration(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlg_TDMOE_Configuration::IDD, pParent)
{
	m_AggregationValueVector.push_back(1);
	m_AggregationValueVector.push_back(5);
	m_AggregationValueVector.push_back(15);
	m_AggregationValueVector.push_back(30);
	m_AggregationValueVector.push_back(60);
	m_AggregationValueVector.push_back(120);
	m_AggregationValueVector.push_back(1440);

	m_pDoc = NULL;
	for(int i = 0; i <=7; i++)
	{
	m_ColorCategoryValue[i] = i*1000;
	}
}
BOOL CDlg_TDMOE_Configuration::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pDoc == NULL)
	{
	
	AfxMessageBox("Doc has not been defined."); 
	return false;
	}
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

			m_ProjectList.AddString(ProjectString);

			if(m_pDoc  == (*iDoc))
			{
			m_ProjectList.SetCurSel (ProjectNo-1);
		
			}

			ProjectNo++;
		}
		iDoc++;
	}

	for(unsigned int i = 0;  i< m_AggregationValueVector.size (); i++)
	{
		CString str;
		str.Format("%d min",m_AggregationValueVector[i]);

		m_AggregationIntervalList.AddString (str);

		if(g_MOEAggregationIntervalInMin  == m_AggregationValueVector[i])
		{
		m_AggregationIntervalList.SetCurSel (i);
	
		}
	
	}
	m_List.AddString ("Number of lanes");
	m_List.AddString ("Speed limit");
	m_List.AddString ("Link capacity per hour");
	m_List.AddString ("Lane capacity per hour");

	m_List.AddString ("--Time-dependent MOE--");
	CString CrossProjectComparisonString;
	m_List.AddString ("Link Volume");

	if(m_ProjectTitleVector.size()>=2)
	{
		CrossProjectComparisonString.Format ("Volume Difference (Projects [%s]-[%s])", m_ProjectTitleVector[1],m_ProjectTitleVector[0]);
	}else
	{
		CrossProjectComparisonString.Format ("Volume Difference (Required 2 Projects)");
	}

	m_List.AddString (CrossProjectComparisonString);

     CClientDC dc(&m_List);

     CFont * f = m_List.GetFont();
     dc.SelectObject(f);

    int  width = 0;
     for(int i = 0; i < m_List.GetCount(); i++)
	 { /* scan strings */
	  CString s;
	  m_List.GetText(i, s);
	  CSize sz = dc.GetTextExtent(s);
          sz.cx += 3 * ::GetSystemMetrics(SM_CXBORDER);
	  if(sz.cx > width)
	      width = sz.cx;
	 } /* scan strings */
     m_List.SetHorizontalExtent(width);



	m_List.SetCurSel (0);
	UpdateCategoryValues();
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CDlg_TDMOE_Configuration::~CDlg_TDMOE_Configuration()
{
}

void CDlg_TDMOE_Configuration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_L1, m_ColorCategoryValue[1]);
	DDX_Text(pDX, IDC_EDIT_L2, m_ColorCategoryValue[2]);
	DDX_Text(pDX, IDC_EDIT_L3, m_ColorCategoryValue[3]);
	DDX_Text(pDX, IDC_EDIT_L4, m_ColorCategoryValue[4]);
	DDX_Text(pDX, IDC_EDIT_L5, m_ColorCategoryValue[5]);
	DDX_Text(pDX, IDC_EDIT_L6, m_ColorCategoryValue[6]);
	DDX_Text(pDX, IDC_EDIT_L7, m_ColorCategoryValue[7]);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_BITMAP, m_ColorRampStatic);
	DDX_Control(pDX, IDC_PROJECT_LIST, m_ProjectList);
	DDX_Control(pDX, IDC_COMBO_AGG_INTERVAL, m_AggregationIntervalList);
}


BEGIN_MESSAGE_MAP(CDlg_TDMOE_Configuration, CDialog)
	ON_BN_CLICKED(IDOK, &CDlg_TDMOE_Configuration::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlg_TDMOE_Configuration::OnLbnSelchangeList1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_REVERSE, &CDlg_TDMOE_Configuration::OnBnClickedButtonReverse)
	ON_CBN_SELCHANGE(IDC_COMBO_AGG_INTERVAL, &CDlg_TDMOE_Configuration::OnCbnSelchangeComboAggInterval)
	ON_LBN_SELCHANGE(IDC_PROJECT_LIST, &CDlg_TDMOE_Configuration::OnLbnSelchangeProjectList)
END_MESSAGE_MAP()


// CDlg_TDMOE_Configuration message handlers

void CDlg_TDMOE_Configuration::OnBnClickedButtonSpaceevenly()
{
	// TODO: Add your control notification handler code here
}

void CDlg_TDMOE_Configuration::OnBnClickedOk()
{
	UpdateData(1);
	for(int i = 1; i<=7; i++)
	{
		m_pDoc->m_LOSBound[MOE_user_defined][i] = m_ColorCategoryValue[i];
	}
	m_pDoc->GenerateOffsetLinkBand();
	m_pDoc->UpdateAllViews(0);

}
void CDlg_TDMOE_Configuration::UpdateCategoryValues()
{
	CWaitCursor wait;

	bool bWithDataFlag = false;
	m_KML_MOE_selection  = (e_TDMOE_ColorMode) m_List.GetCurSel ();

	std::list<DTALink*>::iterator iLink;
	float max_link_volume = 0;

	float max_value = -999999;
	float min_value = 999999;

	int t;

	for (iLink = m_pDoc->m_LinkSet.begin(); iLink != m_pDoc->m_LinkSet.end(); iLink++)
	{
		DTALink* pLink = (*iLink); 
		float KML_color_value = 0;

				float value2 = 0;
				float value1= 0;

			for(t = 0; t < pLink->m_LinkMOEArySize; t++)
			{
				pLink->m_LinkMOEAry [t] .UserDefinedValue  = 0;
					switch (m_KML_MOE_selection)
					{
					case e_TDMOE_number_of_lanes: 
						pLink->m_LinkMOEAry [t] .UserDefinedValue  =  pLink->m_NumberOfLanes ; break; 
					case e_TDMOE_speed_limit_mph: 
						pLink->m_LinkMOEAry [t] .UserDefinedValue =  pLink->m_SpeedLimit  ; break; 
					case e_TDMOE_speed_limit_kmph: 
						pLink->m_LinkMOEAry [t] .UserDefinedValue =  pLink->m_SpeedLimit*1.60934  ; break; 
					case e_TDMOE_link_capacity: 
						pLink->m_LinkMOEAry [t] .UserDefinedValue =  pLink->m_LaneCapacity *  pLink->m_NumberOfLanes   ; break; 
					case e_TDMOE_lane_capacity: 
						pLink->m_LinkMOEAry [t] .UserDefinedValue =  pLink->m_LaneCapacity  ; break; 
					case e_TDMOE_volume:
						m_pDoc->GetLinkMOE(pLink, MOE_volume,t,g_MOEAggregationIntervalInMin, value1);

						pLink->m_LinkMOEAry [t] .UserDefinedValue = value1;
						break;

					};

					if(m_pProjectDocVector.size()>=2)
					{
						DTALink* pLink1= m_pProjectDocVector[0]->FindLinkWithNodeNumbers(pLink->m_FromNodeNumber , pLink->m_ToNodeNumber );
						DTALink* pLink2= m_pProjectDocVector[1]->FindLinkWithNodeNumbers(pLink->m_FromNodeNumber , pLink->m_ToNodeNumber );

						if(pLink1!=NULL && pLink2!=NULL)
						{
							switch (m_KML_MOE_selection)
							{
							case  e_TDMOE_volume_difference:

								m_pProjectDocVector[0]->GetLinkMOE(pLink1, MOE_volume,t,g_MOEAggregationIntervalInMin, value1);
								m_pProjectDocVector[1]->GetLinkMOE(pLink2, MOE_volume,t,g_MOEAggregationIntervalInMin, value2);


								for(int tt = t; tt < t+g_MOEAggregationIntervalInMin; tt++)
								{
									if(tt < pLink->m_LinkMOEArySize)
									{
										pLink->m_LinkMOEAry [tt] .UserDefinedValue = value2 - value1;
									}
								}

								break;

							}


						}  // with related link


				}  // with second project  
					// for each time
						max_value = max(max_value, pLink->m_LinkMOEAry [t] .UserDefinedValue );
						min_value = min(min_value, pLink->m_LinkMOEAry [t] .UserDefinedValue );
						bWithDataFlag = true;

			if(g_MOEAggregationIntervalInMin > 1)
				t+= g_MOEAggregationIntervalInMin; // advance time clock

			}

	}

	if(m_KML_MOE_selection == e_TDMOE_number_of_lanes)
	{
		min_value = 1;
		max_value = 7;
	}

	if(bWithDataFlag == false)
	{
	min_value = 1;
	max_value = 7;
	}
	m_ColorCategoryValue[1] = min_value;
	m_ColorCategoryValue[7] = max_value;

	float incremental = (max_value - min_value)/ 6;

	for(int i = 2; i<=6; i++)
	{
		m_ColorCategoryValue[i] = m_ColorCategoryValue[i-1] + incremental;
	}

	for(int i = 1; i<=7; i++)
	{
		m_pDoc->m_LOSBound[MOE_user_defined][i] = m_ColorCategoryValue[i];
	}
		

	UpdateData(0);
}

void CDlg_TDMOE_Configuration::OnLbnSelchangeList1()
{
	UpdateCategoryValues();


}


void CDlg_TDMOE_Configuration::OnPaint()
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

	  dc.SelectObject(penLOS[i]);
      dc.SelectObject(brushLOS[i]);

	  CString lengend_interval_str;
      dc.Rectangle(lr);
	}

}

void CDlg_TDMOE_Configuration::OnBnClickedButtonReverse()
{
	float temp  = m_ColorCategoryValue[1];
	m_ColorCategoryValue[1] = m_ColorCategoryValue[7] ;
	m_ColorCategoryValue[7] = temp;

	float max_value = max(m_ColorCategoryValue[1], m_ColorCategoryValue[7]);
	float min_value = min(m_ColorCategoryValue[1], m_ColorCategoryValue[7]);

	float incremental = (max_value - min_value)/ 6;

	for(int i = 2; i<=6; i++)
	{
		m_ColorCategoryValue[i] = m_ColorCategoryValue[i-1] + incremental;
	}

	for(int i = 1; i<=7; i++)
	{
		m_pDoc->m_LOSBound[MOE_user_defined][i] = m_ColorCategoryValue[i];
	}

	UpdateData(0);
	m_pDoc->UpdateAllViews(0);
}

void CDlg_TDMOE_Configuration::OnCbnSelchangeComboAggInterval()
{
	g_MOEAggregationIntervalInMin = m_AggregationValueVector [ m_AggregationIntervalList.GetCurSel()];
	 UpdateCategoryValues();

}

void CDlg_TDMOE_Configuration::OnLbnSelchangeProjectList()
{
	// TODO: Add your control notification handler code here
}
