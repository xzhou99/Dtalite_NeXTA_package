//  Portions Copyright 2010 Xuesong Zhou (xzhou99@gmail.com)

//   If you help write or modify the code, please also list your names here.
//   The reason of having Copyright info here is to ensure all the modified version, as a whole, under the GPL 
//   and further prevent a violation of the GPL.

// More about "How to use GNU licenses for your own software"
// http://www.gnu.org/licenses/gpl-howto.html


//    This file is part of NeXTA Version 3 (Open-source).

//    NEXTA is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    NEXTA is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with NEXTA.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "BaseDialog.h"


// CDlgTrainInfo dialog

class CDlgTrainInfo : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgTrainInfo)

public:

	CTLiteDoc* m_pDoc;
	CImageList m_SmallImageList;
	void InsertTrainInfoItem();

	CDlgTrainInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTrainInfo();
	virtual BOOL OnInitDialog();
    

// Dialog Data
	enum { IDD = IDD_DIALOG_Train };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_TrainListControl;
	afx_msg void OnLvnItemchangedTrainListControl(NMHDR *pNMHDR, LRESULT *pResult);
};
