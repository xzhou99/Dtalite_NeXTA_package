// LinePlot.h : header file
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//  CLinePlot, CPlotData - a simple 2D graph control
//
//  Author: Paul Grenz
//  Email:  pgrenz@irlabs.com
//
//  You may freely use or modify this code provided this
//  message is included in all derived versions.
//
//  History - 2004/10/28 Initial release to codeguru.com
//
//
//  This class implements a simple 2D graph control which supports
//  multiple plots, locking and unlocking of axes, printing, and 
//  zooming.
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEPLOT_H__7752C288_1A29_4D99_9777_DE3596070E6A__INCLUDED_)
#define AFX_LINEPLOT_H__7752C288_1A29_4D99_9777_DE3596070E6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum _LinePlotLinkType {_lp_default,_lp_freeway, _lp_arterial,_lp_highway,_lp_ramp};

//  the basic data type for the plot.
typedef struct t_FLOATPOINT
{
public:
  float x;
  float y;
  COLORREF     crColor;
  int LinkNo;
  int Hour;

  _LinePlotLinkType LinkType ;

  t_FLOATPOINT()
  {
	  Hour = -1;

	 LinkNo= -1;
  
  crColor =  RGB (100, 149, 237);
  };

}
FLOATPOINT;

//  the basic data type for the zooming feature
typedef struct t_FLOATRECT
{
  float left;
  float top;
  float right;
  float bottom;
}
FLOATRECT;

#include <afxtempl.h>
#include <vector>
#include "MemDC.h"

//  notification messages.
#define NM_PLOT_SEL_CHANGE      (WM_USER+100)
#define NM_PLOT_LIMITS_CHANGE   (WM_USER+101)
#define NM_PLOT_MOUSE_MOVE      (WM_USER+102)

//  mouse over flags.
#define PLOT_OVER_NOTHING (0)
#define PLOT_OVER_KEY     (1)
#define PLOT_OVER_DATA    (2)
#define PLOT_OVER_X_MIN   (4)
#define PLOT_OVER_X_MAX   (8)
#define PLOT_OVER_Y_MIN   (16)
#define PLOT_OVER_Y_MAX   (32)
#define PLOT_OVER_X_LOCK  (64)
#define PLOT_OVER_Y_LOCK  (128)

//  id of the edit box.
#define ID_EDT_DATA_ENTRY       (WM_USER+10)

/////////////////////////////////////////////////////////////////////////////
//  CPlotData class
/////////////////////////////////////////////////////////////////////////////

class CPlotData
{
  //  constants.
  private:
    //  how to draw the plot.
    enum
    {
      nStyleLine    = 0,
      nStyleBar     = 1,
    };

  // Construction/Destruction
  public:
	  CPlotData(CString szName, COLORREF crColor, int nStyle, FLOATPOINT *pptData, UINT uiPointCount);
    CPlotData(CString szName, COLORREF crColor, int nStyle, std::vector<FLOATPOINT> *pvecData);
    CPlotData(const CPlotData &pd);
	  virtual ~CPlotData();

  // Attributes
  public:


	  CString           m_szName;
    int               m_nStyle;
    FLOATPOINT        *m_PointData;
    //  store screen data as an array of float for "polyline" function.
    POINT             *m_pptScreen;
    UINT              m_uiPointCount;
    COLORREF          m_crColor;
	  bool              m_oIsDirty;
    CRect             m_rcKeyArea;
    float             m_eXDataMin;
    float             m_eXDataMax;
    float             m_eYDataMin;
    float             m_eYDataMax;
    float             m_eYDataMean;

  //  methods.
  public:
    void              Clear();
    bool              UpdateData(FLOATPOINT *pptData);
    bool              UpdateData(std::vector<FLOATPOINT> *pvecData);

  //  operators
  public:
    CPlotData&        operator=(const CPlotData& pd);

};

/////////////////////////////////////////////////////////////////////////////
//  CLinePlot control
/////////////////////////////////////////////////////////////////////////////

#define LINE_PLOT_UNDO_SIZE (50)

    enum enumPlotStyle
    {
      enum_LpLine    = 0,
      enum_LpBar     = 1,
	  enum_LpScatter     = 3
    };
class CLinePlot : public CWnd
{
  //  constants.
  public:



	  int              m_SelectedDataItemNo;
	  int                m_nSelected;
	  int m_StartHour, m_AggregationWindow;
  // Construction/Destruction
  public:
    CLinePlot();
    virtual ~CLinePlot();


  //  variables
  private:
    CString           m_szName;
    CString           m_szXCaption;
    CString           m_szYCaption;
    int               m_nXIntervals;
    int               m_nYIntervals;
    UINT              m_uiXPrecision;
    UINT              m_uiYPrecision;
    float             m_eXMin;
    float             m_eXMax;
    float             m_eYMin;
    float             m_eYMax;
    FLOATRECT         m_prcUndoLimits[LINE_PLOT_UNDO_SIZE];
    int               m_nUndoIndex;
    float             m_eXRatio;
    float             m_eYRatio;
    int               m_nCursor;
    UINT              m_uiMouseOver;
    CEdit             m_edtDataEntry;

    int               m_nMouseOverKey;
    CRect             m_rcZoom;
    bool              m_oIsMouseDown;
    bool              m_oHasPassedOverPlot;
    bool              m_oIsXLocked;
    bool              m_oIsYLocked;
    CRect             m_rcXMin;
    CRect             m_rcXMax;
    CRect             m_rcYMin;
    CRect             m_rcYMax;
    CRect             m_rcXLock;
    CRect             m_rcYLock;
    CRect             m_rcKeyArea;
    CRect             m_rcPlotArea;
    //  the vector that holds all the plots
    std::vector<CPlotData> m_lstPlotData;

  // Methods
  public:
	int m_XScaleInterval;
  	BOOL              Create(const RECT& rect, UINT uiFlags, CWnd* pwndParent, UINT uiID);
    int               Add(CString szName, COLORREF crColor, enumPlotStyle nStyle, FLOATPOINT *pptData, UINT uiPointCount, int LinkNo);
    int               Add(CString szName, COLORREF crColor, enumPlotStyle nStyle, std::vector<FLOATPOINT> *pvecData);
	  void              Clear();
	  bool              Remove(int nIndex);
    void              Refresh();
    int               Count() { return m_lstPlotData.size(); };
    void              Print();
    int               Selected() { return m_nSelected; };

  //  operators
  public:
    CPlotData &       operator[](UINT unIndex);

  //  notifications
  private:
    void              EmitSelectionChanged();
    void              EmitLimitsChanged();
    void              EmitMouseMove();

  //  sub-attribute get/set
  public:
	  CString           GetName(int nIndex);
	  bool              SetName(int nIndex, CString szName);
	  FLOATPOINT *      GetData(int nIndex);
	  bool              SetData(int nIndex, FLOATPOINT *pptData);
	  COLORREF          GetColor(int nIndex);
	  bool              SetColor(int nIndex, COLORREF crColor);
	  enumPlotStyle     GetStyle(int nIndex);
	  bool              SetStyle(int nIndex, enumPlotStyle nStyle);
	  UINT              GetPointCount(int nIndex);

  //  attributes get/set
  public:
    void              SetName(CString &szName) { m_szName = szName; };
    CString           GetName() { return m_szName; };
    void              SetXCaption(CString &szXCaption) { m_szXCaption = szXCaption; };
    CString           GetXCaption() { return m_szXCaption; };
    void              SetYCaption(CString &szYCaption) { m_szYCaption = szYCaption; };
    CString           GetYCaption() { return m_szYCaption; };
    void              SetXPrecision(UINT &uiXPrecision) { m_uiXPrecision = uiXPrecision; };
    UINT              GetXPrecision() { return m_uiXPrecision; };
    void              SetYPrecision(UINT &uiYPrecision) { m_uiYPrecision = uiYPrecision; };
    UINT              GetYPrecision() { return m_uiYPrecision; };
    void              SetXIntervals(int &nXIntervals) { m_nXIntervals = nXIntervals; };
    int               GetXIntervals() { return m_nXIntervals; };
    void              SetYIntervals(int &nYIntervals) { m_nYIntervals = nYIntervals; };
    int               GetYIntervals() { return m_nYIntervals; };
    bool              SetXMin(float &eXMin);
    float             GetXMin() { return m_eXMin; };
    bool              SetXMax(float &eXMax);
    float             GetXMax() { return m_eXMax; };
    bool              SetYMin(float &eYMin);
    float             GetYMin() { return m_eYMin; };
    bool              SetYMax(float &eYMax);
    float             GetYMax() { return m_eYMax; };
    bool              SetIsXLocked(bool &oLock);
    bool              GetIsXLocked() { return m_oIsXLocked; };
    bool              SetIsYLocked(bool &oLock);
    bool              GetIsYLocked() { return m_oIsYLocked; };

  //  functions.
  private:
    CRect             DrawFramework(CDC *pdc, CRect rcBounds, int nFontHeight, bool oIsPrinting);
    void              DrawData(CDC *pDC, CRect rcBounds, bool oIsPrinting);
    void              DrawPlotData(CDC* pDC, UINT uiIndex, CRect &rcBounds);
    void              HideEditWindow();
    void              AddToZoomUndo(FLOATRECT &rcLimits);
    FLOATRECT         GetLastZoomUndo();

  // Overrides
  public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CLinePlot)
    //}}AFX_VIRTUAL

  // Generated message map functions
  protected:
    //{{AFX_MSG(CLinePlot)
	  afx_msg void OnPaint();
	  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	  //afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	  //afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	  afx_msg void OnSize(UINT nType, int cx, int cy);
	  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEPLOT_H__7752C288_1A29_4D99_9777_DE3596070E6A__INCLUDED_)
