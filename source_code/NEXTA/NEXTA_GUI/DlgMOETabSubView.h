#pragma once
#include "CGridListCtrlEx\\CGridListCtrlEx.h"
#include <string>

// CDlgMOETabSubView dialog

class CDlgMOETabSubView : public CDialog
{
	DECLARE_DYNAMIC(CDlgMOETabSubView)

public:
	CDlgMOETabSubView(CWnd* pParent = NULL);   // standard constructor
	CDlgMOETabSubView(std::string fileName):m_FileName(fileName){};
	virtual ~CDlgMOETabSubView();

// Dialog Data
	enum { IDD = IDD_DIALOG_MOETABVIEW };

private:
	CGridListCtrlEx m_ListCtrl;
	std::string m_FileName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
