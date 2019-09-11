// LinePlotTest.h : main header file for the LINEPLOTTEST application
//

#if !defined(AFX_LINEPLOTTEST_H__A81FEF7B_6354_4807_B7AF_7AC639353BE0__INCLUDED_)
#define AFX_LINEPLOTTEST_H__A81FEF7B_6354_4807_B7AF_7AC639353BE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


/////////////////////////////////////////////////////////////////////////////
// CLinePlotTestApp:
// See LinePlotTest.cpp for the implementation of this class
//

class CLinePlotTestApp : public CWinApp
{
public:
	CLinePlotTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinePlotTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLinePlotTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEPLOTTEST_H__A81FEF7B_6354_4807_B7AF_7AC639353BE0__INCLUDED_)
