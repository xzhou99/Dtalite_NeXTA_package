// Microsoft Excel Automation Class by Val Golovlev
//http://www.codeguru.com/cpp/data/mfc_database/microsoftexcel/article.php/c11745
// TestDataSource.h: interface for the CTestDataSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTDATASOURCE_H__969215C6_000A_4E26_A734_9719C8BF5087__INCLUDED_)
#define AFX_TESTDATASOURCE_H__969215C6_000A_4E26_A734_9719C8BF5087__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTestDataSource  
{
public:
	CString GetLastYValue();
	CString GetLastXValue();
	CString GetNextXYPoint();
	void Reset();
	CTestDataSource();
	virtual ~CTestDataSource();

protected:
	long m_nDataEle;
	CString m_szX;
	CString m_szY;

};

#endif // !defined(AFX_TESTDATASOURCE_H__969215C6_000A_4E26_A734_9719C8BF5087__INCLUDED_)
