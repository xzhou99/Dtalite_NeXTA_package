// FrameGISLayer.cpp : implementation file
//

#include "stdafx.h"
#include "TLite.h"

// CFrameGISLayer dialog

IMPLEMENT_DYNAMIC(CFrameGISLayer, CDialog)

CFrameGISLayer::CFrameGISLayer(CWnd* pParent /*=NULL*/)
	: CDialog(CFrameGISLayer::IDD, pParent)
{

}

CFrameGISLayer::~CFrameGISLayer()
{
}

void CFrameGISLayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFrameGISLayer, CDialog)
END_MESSAGE_MAP()


// CFrameGISLayer message handlers
