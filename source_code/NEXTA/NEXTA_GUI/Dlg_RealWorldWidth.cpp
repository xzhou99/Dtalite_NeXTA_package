// Dlg_RealWorldWidth.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_RealWorldWidth.h"


// CDlg_RealWorldWidth dialog

IMPLEMENT_DYNAMIC(CDlg_RealWorldWidth, CDialog)

CDlg_RealWorldWidth::CDlg_RealWorldWidth(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_RealWorldWidth::IDD, pParent)
	, m_RealWorldWidth(10)
{

}

CDlg_RealWorldWidth::~CDlg_RealWorldWidth()
{
}

void CDlg_RealWorldWidth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_RealWorldWidth);
	DDX_Control(pDX, IDC_COMBO1, m_DistanaceUnitCombo);
}


BEGIN_MESSAGE_MAP(CDlg_RealWorldWidth, CDialog)
	ON_BN_CLICKED(IDC_BUTTONOK, &CDlg_RealWorldWidth::OnBnClickedButtonok)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlg_RealWorldWidth::OnBnClickedButton2)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_RealWorldWidth::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CDlg_RealWorldWidth message handlers

void CDlg_RealWorldWidth::OnBnClickedButtonok()
{
	UpdateData(1);

	if(m_DistanaceUnitCombo.GetCurSel()==1)
		m_RealWorldWidth/=1.60934;

	OnOK();
}

BOOL CDlg_RealWorldWidth::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_DistanaceUnitCombo.AddString ("miles");
	m_DistanaceUnitCombo.AddString ("km");

	m_DistanaceUnitCombo.SetCurSel (0);

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_RealWorldWidth::OnBnClickedButton2()
{
	g_OpenDocument("http://www.daftlogic.com/projects-google-maps-distance-calculator.htm", SW_SHOW);
}

void CDlg_RealWorldWidth::OnBnClickedButtonLoadFile()
{
	


}

void CDlg_RealWorldWidth::OnPaint()
{
   CPaintDC dc(this); // device context for painting

   // if(m_Bitmap)
   //{
   //   // Create palette
   //   CPalette palette;
   //   CreateDIBPalette(m_Bitmap, &palette);

   //   // Source rectangle - entire bitmap
   //   CRect source(0, 0, m_BitmapWidth, m_BitmapHeight);

 
   //   double whratio = (double)m_BitmapWidth / m_BitmapHeight;
   //   int right = m_BitmapWidth > m_BitmapHeight ? 310 : (int)(310 * whratio);
   //   int bottom = m_BitmapWidth < m_BitmapHeight ? 310 : (int)(310 / whratio);
   //   CRect dest(15, 15, right, bottom);

   //   // Resize the frame to fit the bitmap
   //   m_BitmapControl.MoveWindow(dest, FALSE);

   //   // Paint the bitmap
   //   PaintDIB(dc.m_hDC, dest, m_Bitmap, source, &palette);
   //}
   //CDialog::OnPaint();

}

void CDlg_RealWorldWidth::OnCbnSelchangeCombo2()
{
	// TODO: Add your control notification handler code here
}

void CDlg_RealWorldWidth::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}
