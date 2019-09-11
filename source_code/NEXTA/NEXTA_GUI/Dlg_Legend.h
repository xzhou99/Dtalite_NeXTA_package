#pragma once

#include "Network.h"
#include "TLiteDoc.h"
#include "BaseDialog.h"
#include "afxwin.h"
// CDlg_Legend dialog

class CDlg_Legend : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlg_Legend)

public:
	void DrawObjects(CDC* pDC);
	CDlg_Legend(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Legend();

	CTLiteDoc* m_pDoc;


// Dialog Data
	enum { IDD = IDD_DIALOG_Legend };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnPaint();
   afx_msg void OnSize(UINT nType, int cx, int cy);
   CComboBox m_ComboBox_EmissionType;
   afx_msg void OnCbnSelchangeComboEmissiontype();
   CComboBox m_QueueCutOffComboBox;
   afx_msg void OnCbnSelchangeComboAggregation();
   CStatic m_TextCutOff;
   afx_msg void OnStnClickedStaticQueueCutOff();
   CStatic m_StudyPeriodText;
   CComboBox m_ComboxStudyPeriod;
   afx_msg void OnCbnSelchangeComboStudyPeriod();
   BOOL m_bShowRadarChart;
   CButton m_RadarChartButton;
   afx_msg void OnBnClickedCheckRadarChart();
};
