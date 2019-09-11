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
#include "afxwin.h"
#include "TLiteDoc.h"
#include "BaseDialog.h"


// CDlgFindALink dialog

class CDlgFindALink : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgFindALink)

public:

	CTLiteDoc* m_pDoc;
	std::vector<CString> m_SearchHistoryVector;
	CDlgFindALink(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFindALink();

// Dialog Data
	enum { IDD = IDD_DIALOG_FIND_LINK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	eSEARCHMODE m_SearchMode;
	int m_NodeNumber;
	int m_FromNodeNumber;
	int m_ToNodeNumber;
    int m_VehicleNumber;

	CString m_PathNodeSequence; 


	CString m_StrFind;
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedFindVehicle();
	CListBox m_FindObjectList;
	afx_msg void OnBnClickedButtonFind();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnCbnSelchangeCombo1();
	CString m_UserInputString;
	CEdit m_EditorControl;
};
