// LinePlotTestDlg.cpp : implementation file
/

//A 2D Lite Graph Control with Multiple Plot Support
//Posted by Paul Grenz on November 23rd, 2004
//http://www.codeguru.com/cpp/cpp/cpp_mfc/general/article.php/c8535/A-2D-Lite-Graph-Control-with-Multiple-Plot-Support.htm

#include "stdafx.h"
#include "../TLite.h"
#include "LinePlotTest.h"
#include "LinePlotTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CLinePlotTestDlg dialog
;
CLinePlotTestDlg::CLinePlotTestDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CLinePlotTestDlg::IDD, pParent)
	, m_bZoomToLink(TRUE)
{
	m_pDoc = NULL;
	m_AggregationTimeIntervalInMin = 60;
	//{{AFX_DATA_INIT(CLinePlotTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinePlotTestDlg)
	DDX_Control(pDX, IDC_LIST1, m_InfoList);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_ZOOM_TO_LINK, m_bZoomToLink);
	DDX_Control(pDX, IDC_LIST_TimeWindow, m_TimeWindowList);
	DDX_Control(pDX, IDC_LIST_DataSource, m_ListDataSource);
	DDX_Control(pDX, IDC_LIST_StartHour, m_ListStartHour);
}
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CLinePlotTestDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CLinePlotTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	  ON_NOTIFY(NM_PLOT_SEL_CHANGE, ID_CTRL_LINE_PLOT, OnPlotSelChange)
	  ON_NOTIFY(NM_PLOT_LIMITS_CHANGE, ID_CTRL_LINE_PLOT, OnPlotLimitsChange)
	  ON_NOTIFY(NM_PLOT_MOUSE_MOVE, ID_CTRL_LINE_PLOT, OnPlotMouseMove)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_LBN_SELCHANGE(IDC_LIST_TimeWindow, &CLinePlotTestDlg::OnLbnSelchangeListTimewindow)
	ON_LBN_SELCHANGE(IDC_LIST_DataSource, &CLinePlotTestDlg::OnLbnSelchangeListDatasource)
	ON_LBN_SELCHANGE(IDC_LIST_StartHour, &CLinePlotTestDlg::OnLbnSelchangeListStarthour)
	ON_BN_CLICKED(IDC_CHECK_ZOOM_TO_LINK, &CLinePlotTestDlg::OnBnClickedCheckZoomToLink)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinePlotTestDlg message handlers
/////////////////////////////////////////////////////////////////////////////
void CLinePlotTestDlg::RegenerateData()
{



}
BOOL CLinePlotTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	if( m_pDoc->m_DemandLoadingStartTimeInMin > m_pDoc->m_DemandLoadingEndTimeInMin)
	{
	
		m_pDoc->m_DemandLoadingEndTimeInMin = 1440;
		m_pDoc->m_DemandLoadingStartTimeInMin = 0;

	
	}

	for(int hour = m_pDoc->m_DemandLoadingStartTimeInMin  /60; hour <=m_pDoc->m_DemandLoadingEndTimeInMin /60; hour++)
	{
	CString str_hour;
	str_hour.Format ("%d", hour);
	m_ListStartHour .AddString (str_hour);
	
	}



	m_TimeWindowList .AddString ("24");
	m_TimeWindowList .AddString ("1");
	m_TimeWindowList .AddString ("2");

	m_TimeWindowList.SetCurSel (0);
	m_ListStartHour.SetCurSel (0);
	
	CString str;
	m_ListStartHour.GetText (m_ListStartHour.GetCurSel(),str);
	m_LinePlot.m_StartHour  = atoi(str);
	m_TimeWindowList.GetText (m_TimeWindowList.GetCurSel(),str);
	m_LinePlot.m_AggregationWindow  = atoi(str);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

  CRect rcClient;
  GetWindowRect(&rcClient);
  rcClient.bottom = rcClient.top + 300;
  rcClient.right = rcClient.left + 900;
  MoveWindow(&rcClient);

  //  create the line plot control.
	m_LinePlot.Create(CRect(0, 0, 1, 1), WS_CHILD | WS_VISIBLE, this, ID_CTRL_LINE_PLOT);

	m_LinePlot.SetXCaption (m_XCaption);
	m_LinePlot.SetYCaption (m_YCaption);

  //  size all controls correctly;
	SizeControls();

		unsigned int i;
	for(i = 0; i < m_PlotDataVector.size(); i++)
	{
		CLinePlotData element = m_PlotDataVector[i];
		m_ListDataSource.AddString(element.szName);
		m_LinePlot.Add(element.szName, element.crPlot, element.lineType,  &(element.vecData) );
	}

	if(m_PlotDataVector.size() >0)
	{
	m_ListDataSource.SetCurSel(0);
	m_LinePlot.m_nSelected = 0;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
		CDialog::OnSysCommand(nID, lParam);
}
/////////////////////////////////////////////////////////////////////////////

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLinePlotTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
/////////////////////////////////////////////////////////////////////////////

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLinePlotTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
/////////////////////////////////////////////////////////////////////////////
    
void CLinePlotTestDlg::SizeControls()
{
  CRect rcClient;
  GetClientRect(&rcClient);
  int left_boarder = 0;
  int upper_boarder = 80;

  if (m_LinePlot.m_hWnd!=NULL)
    m_LinePlot.MoveWindow(left_boarder, upper_boarder, rcClient.Width(), rcClient.Height()-upper_boarder);

    CRect rcArea;
    ScreenToClient(&rcArea);
    rcArea.bottom = rcClient.bottom;
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	SizeControls();	
}
/////////////////////////////////////////////////////////////////////////////

BOOL CLinePlotTestDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//cs.style |= WS_CLIPCHILDREN;
	return CDialog::PreCreateWindow(cs);
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::OnBtnCurrColor() 
{
  //  get the current plot index.
  int nIndex = m_LinePlot.Selected();

  if (nIndex<0 || nIndex>=m_LinePlot.Count())
    AfxMessageBox("There is no plot selected.");
  else
  {
    //  get the color.
    COLORREF crCurr = m_LinePlot.GetColor(nIndex);
    //  show the color dialog.
    CColorDialog dlgColor(crCurr, CC_FULLOPEN);

    if (dlgColor.DoModal()==IDOK)
    {
      //  update the current color.
      m_LinePlot.SetColor(nIndex, dlgColor.GetColor());
    }
  }
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::OnBtnCurrRemove() 
{
  //  get the current plot index.
  int nIndex = m_LinePlot.Selected();

  if (nIndex<0 || nIndex>=m_LinePlot.Count())
    AfxMessageBox("There is no plot selected.");
  else
  {
    if (m_LinePlot.Remove(nIndex)!=true)
    {
      AfxMessageBox("Could not remove current plot.");
    }
  }
}

void CLinePlotTestDlg::OnResizePlot() 
{

	for(int nIndex= m_LinePlot.Count(); nIndex>=0; nIndex--)
	{
		m_LinePlot.Remove(nIndex);
	}


	for(unsigned int i = 0; i < m_PlotDataVector.size(); i++)
	{

		CLinePlotData element = m_PlotDataVector[i];

		m_LinePlot.Add(element.szName, element.crPlot, element.lineType,  &(element.vecData));
	}

}
/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::OnPlotMouseMove(NMHDR* pNMHDR, LRESULT* pResult)
{
  //  add this message to the notification list.

}
/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::OnPlotLimitsChange(NMHDR* pNMHDR, LRESULT* pResult)
{
  //  add this message to the notification list.
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::OnPlotSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
  //  add this message to the notification list.
  //  get the current plot index.

	
	int SelDataItemNo = m_LinePlot.m_SelectedDataItemNo;

	UpdateData(1);

	if(m_bZoomToLink && SelDataItemNo>=0) 
	{
		int LinkNo = m_PlotDataVector[0].vecData[SelDataItemNo].LinkNo ;
		if(LinkNo>=0)
		{
		m_pDoc->m_SelectedLinkNo = LinkNo;
		DTALink* pLink = m_pDoc->m_LinkNoMap [LinkNo];

		g_AddLinkIntoSelectionList(pLink,LinkNo,m_pDoc->m_DocumentNo );
		m_pDoc->ZoomToSelectedLink(m_pDoc->m_SelectedLinkNo);

		m_InfoList.ResetContent ();

		CString str;
		str.Format("Link %d->%d is selected.", pLink->m_FromNodeNumber, pLink->m_ToNodeNumber);

		m_InfoList.AddString (str);

		m_LinePlot.Invalidate (1);
		}

	}

}
/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::OnCloseupCmbStyle() 
{
  //  get the current plot index.
  int nIndex = m_LinePlot.Selected();

}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::OnBtnPrint() 
{
	m_LinePlot.Print();
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::OnBtnLoadFromFile() 
{	
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::OnBtnSaveToFile() 
{
  //  which plot is the current one?
  int nIndex = m_LinePlot.Selected();
  if (nIndex < 0)
    AfxMessageBox("No plot selected - select one and try again.");
  else
  {
    //  get  a pointer to the data.
    FLOATPOINT *pptData = m_LinePlot.GetData(nIndex);
    //  get the number of points.
    UINT uiPointCount = m_LinePlot.GetPointCount(nIndex);
    //  get the name of the plot.
    CString szName = m_LinePlot.GetName(nIndex);
    if (pptData == NULL || uiPointCount == 0)
      AfxMessageBox("Selected plot has no data - try another.");
    else
    {
      CString szTitle = "Save Plot Data To....";
      CString szSelFilename = szName + ".txt";
      CString szDefExtension = "txt";

      long nFilterIndex = 0;
      long nNumFilters = 0;
      CString szFilter = "";
      // do for certain file types.
      szFilter += "csv File (*.csv)|*.csv|";
      nNumFilters++;
      szFilter += "|";

      //  create the dialog.
      CFileDialog dlgFile(FALSE, NULL, NULL, 0, szFilter);

      //  set the title
      dlgFile.m_ofn.lpstrTitle = szTitle.GetBuffer(MAX_PATH);
      //  set the flags.
      dlgFile.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
      //  set the default filename.
      dlgFile.m_ofn.lpstrFile = szSelFilename.GetBuffer(MAX_PATH);
      //  set the default extension.
      dlgFile.m_ofn.lpstrDefExt = szDefExtension.GetBuffer(MAX_PATH);
      //  set the number of filters
      dlgFile.m_ofn.nMaxCustFilter = nNumFilters;
      //  set the default filter.
      dlgFile.m_ofn.nFilterIndex = 0;

      //  show the dialog.
      if (dlgFile.DoModal()==IDOK)
      {
        //  release the buffer used for the filename.
        szSelFilename.ReleaseBuffer();

        FILE *fPlot;
        if ((fPlot = fopen(szSelFilename, "w"))!=NULL)
        {
          int nErr = 0;
          for (UINT ii=0; ii<uiPointCount; ii++)
          {
            nErr = fprintf(fPlot, "%f\t%f\n",pptData[ii].x, pptData[ii].y);
          }
          fclose(fPlot);
        }
      }
    }
  }	
}
/////////////////////////////////////////////////////////////////////////////

void CLinePlotTestDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	OnResizePlot();
}
void CLinePlotTestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnLButtonDown(nFlags, point);
}

void CLinePlotTestDlg::ShowSelection(int SelNo)
{

	TRACE("Sel: %d,", SelNo);
}

void CLinePlotTestDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_LinePlot.OnLButtonDblClk(nFlags,point);
	int SelDataItemNo = m_LinePlot.m_SelectedDataItemNo;
	CDialog::OnLButtonDblClk(nFlags, point);

}


void CLinePlotTestDlg::OnLbnSelchangeListTimewindow()
{
	CString str;
	m_TimeWindowList.GetText (m_TimeWindowList.GetCurSel(),str);
	m_LinePlot.m_AggregationWindow  = atoi(str);
	m_LinePlot.Invalidate ();

}

void CLinePlotTestDlg::OnLbnSelchangeListDatasource()
{
	m_LinePlot.m_nSelected = m_ListDataSource.GetCurSel ();
	m_LinePlot.Invalidate ();

}

void CLinePlotTestDlg::OnLbnSelchangeListStarthour()
{
	CString str;
	m_ListStartHour.GetText (m_ListStartHour.GetCurSel(),str);
	m_LinePlot.m_StartHour  = atoi(str);
	m_LinePlot.Invalidate ();
}

void CLinePlotTestDlg::OnBnClickedCheckZoomToLink()
{
	// TODO: Add your control notification handler code here
}
