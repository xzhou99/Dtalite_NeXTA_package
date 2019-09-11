// Dlg_ImportPlanningModel.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"
#include "Dlg_ImportPlanningModel.h"


// CDlg_ImportPlanningModel dialog

IMPLEMENT_DYNAMIC(CDlg_ImportPlanningModel, CDialog)

CDlg_ImportPlanningModel::CDlg_ImportPlanningModel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_ImportPlanningModel::IDD, pParent)
{

}

CDlg_ImportPlanningModel::~CDlg_ImportPlanningModel()
{
}

void CDlg_ImportPlanningModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_ImportPlanningModel, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_View_DSP_File, &CDlg_ImportPlanningModel::OnBnClickedButtonViewDspFile)
	ON_BN_CLICKED(IDC_BUTTON_Import_DSP_FIle, &CDlg_ImportPlanningModel::OnBnClickedButtonImportDspFile)
	ON_BN_CLICKED(ID_View_Sample_TRANSIMS_File, &CDlg_ImportPlanningModel::OnBnClickedViewSampleTransimsFile)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_TRANSIMS, &CDlg_ImportPlanningModel::OnBnClickedButtonImportTransims)
END_MESSAGE_MAP()


// CDlg_ImportPlanningModel message handlers

void CDlg_ImportPlanningModel::OnBnClickedButtonViewDspFile()
{
	// TODO: Add your control notification handler code here
}

void CDlg_ImportPlanningModel::OnBnClickedButtonImportDspFile()
{
	// TODO: Add your control notification handler code here
}

void CDlg_ImportPlanningModel::OnBnClickedViewSampleTransimsFile()
{
	// TODO: Add your control notification handler code here
}

void CDlg_ImportPlanningModel::OnBnClickedButtonImportTransims()
{
	// TODO: Add your control notification handler code here
}
