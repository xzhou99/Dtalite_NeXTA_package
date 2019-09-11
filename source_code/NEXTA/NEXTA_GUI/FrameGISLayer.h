#pragma once


// CFrameGISLayer dialog

class CFrameGISLayer : public CDialog
{
	DECLARE_DYNAMIC(CFrameGISLayer)

public:
	CFrameGISLayer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFrameGISLayer();

// Dialog Data
//	enum { IDD = IDD_DIALOG_LayerFrame };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
