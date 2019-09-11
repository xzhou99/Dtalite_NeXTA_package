// LinePlotTestDlg.h : header file
//
//A 2D Lite Graph Control with Multiple Plot Support
//Posted by Paul Grenz on November 23rd, 2004
//http://www.codeguru.com/cpp/cpp/cpp_mfc/general/article.php/c8535/A-2D-Lite-Graph-Control-with-Multiple-Plot-Support.htm

#if !defined(AFX_LINEPLOTTESTDLG_H__2B7C15E7_EF20_49C5_A59A_100B36A474EA__INCLUDED_)
#define AFX_LINEPLOTTESTDLG_H__2B7C15E7_EF20_49C5_A59A_100B36A474EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLinePlotTestDlg dialog

#include "LinePlot.h"
#include <math.h>
#include "..\\TLite.h"
#include "..\\Network.h"
#include "..\\TLiteDoc.h"
#include "afxwin.h"
#include "..\\BaseDialog.h"

#define ID_CTRL_LINE_PLOT (WM_USER+1001)



class CLinePlotData
{
	public: 
	CLinePlotData()
	{
		lineType = enum_LpLine;  // line
    //  get a random color.
    srand((unsigned)time(NULL));
    BYTE yRed = (BYTE)(rand() / (RAND_MAX / 256) + 1);
    BYTE yGreen = (BYTE)(rand() / (RAND_MAX / 256) + 1);
    BYTE yBlue = (BYTE)(rand() / (RAND_MAX / 256) + 1);
    crPlot = RGB(yRed, yGreen, yBlue);	
	
	LinkType = _lp_default;
	}

	_LinePlotLinkType  LinkType;
	 
	 CString szName;
	 enumPlotStyle lineType;
     std::vector<FLOATPOINT> vecData;
	COLORREF crPlot;

};
class CLinePlotTestDlg : public CBaseDialog
{
  // Construction/Destruction
  public:

	  CLinePlotTestDlg(CWnd* pParent = NULL);	// standard constructor

	  	CTLiteDoc* m_pDoc;
	  void ShowSelection(int SelNo);
  private:
    void SizeControls();

  // Dialog Data
  public:
	  //{{AFX_DATA(CLinePlotTestDlg)
	  enum { IDD = IDD_XYPLOT_DIALOG };
      CLinePlot m_LinePlot;
	  CListBox m_InfoList;

	  CString m_XCaption, m_YCaption;


	CListBox m_TimeWindowList;

	  std::vector<CLinePlotData> m_PlotDataVector;
	  std::vector<CString> m_MessageVector;

	  int m_AggregationTimeIntervalInMin;
	  void RegenerateData();
	  //}}AFX_DATA

	  // ClassWizard generated virtual function overrides
	  //{{AFX_VIRTUAL(CLinePlotTestDlg)
	  protected:
	  virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	  //}}AFX_VIRTUAL

  // Implementation
  protected:
	  HICON m_hIcon;

	  // Generated message map functions
	  //{{AFX_MSG(CLinePlotTestDlg)
	  virtual BOOL OnInitDialog();
	  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	  afx_msg void OnPaint();
	  afx_msg HCURSOR OnQueryDragIcon();
	  afx_msg void OnSize(UINT nType, int cx, int cy);
	  afx_msg void OnBtnCurrColor();
	  afx_msg void OnBtnCurrRemove();
	  afx_msg void OnResizePlot();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	  afx_msg void OnCloseupCmbStyle();
    afx_msg void OnPlotSelChange(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnPlotLimitsChange(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnPlotMouseMove(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnLoadFromFile();
	afx_msg void OnBtnPrint();
	afx_msg void OnBtnSaveToFile();
	//}}AFX_MSG
	  DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	BOOL m_bZoomToLink;
	afx_msg void OnLbnSelchangeListTimewindow();
	afx_msg void OnLbnSelchangeListDatasource();
	CListBox m_ListDataSource;
	CListBox m_ListStartHour;
	afx_msg void OnLbnSelchangeListStarthour();
	afx_msg void OnBnClickedCheckZoomToLink();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEPLOTTESTDLG_H__2B7C15E7_EF20_49C5_A59A_100B36A474EA__INCLUDED_)
