// Microsoft Excel Automation Class by Val Golovlev
//http://www.codeguru.com/cpp/data/mfc_database/microsoftexcel/article.php/c11745
// TestDataSource.cpp: implementation of the CTestDataSource class.
// This class simulates acquisition of data from an external device
// Calculate a new set of (x,y) values when GenNextXYPoint() is called 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "XLAutomationTester.h"
#include "XLTestDataSource.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTestDataSource::CTestDataSource()
{
	Reset();
	
}

CTestDataSource::~CTestDataSource()
{

}
//Reset data source to initial "zero-time" state
void CTestDataSource::Reset()
{
	m_szX = _T("X");
	m_szY = _T("Y");
	m_nDataEle = 0;
}
//Calculate XY values; format it as (x,y) pair separated by TAB and return to user
CString CTestDataSource::GetNextXYPoint()
{
	CString szXYData;
	double x, y;
	//Frequencies used to calculate Demo plot
	double Q1, Q2, Q3;
	Q1 = 0.08;
	Q2 = 0.16;
	Q3 = 0.24;

	x = (double)m_nDataEle;
	y = 2.0 + cos(Q1*x) + cos(Q2*x)+cos(Q3*x);
	
	szXYData.Format("%f\t%f\n", x, y);
	m_szX.Format("%f", x);
	m_szY.Format("%f",y);
	m_nDataEle++;

	return szXYData;
}

//Return x value calculated by last call of GetNextXYPoint()
CString CTestDataSource::GetLastXValue()
{
	return m_szX;
}
//Return y value calculated by last call of GetNextXYPoint()
CString CTestDataSource::GetLastYValue()
{
	return m_szY;
}
