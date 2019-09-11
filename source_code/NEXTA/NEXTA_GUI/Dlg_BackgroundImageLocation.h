#pragma once


// CDlg_BackgroundImageLocation dialog

class CDlg_BackgroundImageLocation : public CDialog
{
	DECLARE_DYNAMIC(CDlg_BackgroundImageLocation)


public:
	CDlg_BackgroundImageLocation(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_BackgroundImageLocation();

// Dialog Data
	enum { IDD = IDD_DIALOG_Image_Location };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	double m_PointA_x,m_PointA_y,m_PointB_x,m_PointB_y;
	double m_PointA_long,m_PointA_lat,m_PointB_long,m_PointB_lat;
	bool m_bPointA_Initialized, m_bPointB_Initialized;
	CString m_LatLongA;
	CString m_PointA;
	CString m_PointB;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_LatLongB;
};
