// Microsoft Excel Automation Class by Val Golovlev
//http://www.codeguru.com/cpp/data/mfc_database/microsoftexcel/article.php/c11745
// XLAutomation.h: interface for the CXLAutomation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLAUTOMATION_H__E020CE95_7428_4BEF_A24C_48CE9323C450__INCLUDED_)
#define AFX_XLAUTOMATION_H__E020CE95_7428_4BEF_A24C_48CE9323C450__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXLAutomation  
{

#define MAX_DISP_ARGS 10
#define DISPARG_NOFREEVARIANT 0x01
#define DISP_FREEARGS 0x02
#define DISP_NOSHOWEXCEPTIONS 0x03
#define xlWorksheet -4167
#define xl3DPie -4102
#define xlRows 1
#define xlXYScatter -4169
#define xlXYScatterLines 74
#define xlXYScatterSmoothNoMarkers 73
#define xlXYScatterSmooth 72
#define xlXYScatterLinesNoMarkers 75
#define xlColumns 2
#define xlNormal -4143
#define xlUp -4162

public:

	BOOL OpenExcelFile(CString szFileName, CString WorkSheetName, int 	ActiveSheet);
	BOOL OpenExcelFile(CString szFileName, int nWorkSheet);
	BOOL InsertPictureToWorksheet(BYTE* pImage, int Column, int Row, double dPicWidth, double dPicHeight);
	BOOL PlaceImageToClipboard(BYTE* pImage);
	BOOL InsertPictureToWorksheet(CString szFileName, int Column, int Row, double dPicWidth, double dPicHeight);
	CString GetCellValueCString(int nColumn, int nRow);
	BOOL SaveAs(CString szFileName, int nFileFormat, CString szPassword, CString szWritePassword, BOOL bReadOnly, BOOL bBackUp);
	BOOL DeleteRow(long nRow);
	BOOL ReleaseExcel();
	BOOL PasteStringToWorksheet(CString pDataBuffer);
	BOOL UpdatePlotRange(int nYColumn);
	BOOL AddArgumentCStringArray(LPOLESTR lpszArgName,WORD wFlags, LPOLESTR *paszStrings, int iCount);
	BOOL SetRangeValueDouble(LPOLESTR lpszRef, double d);
	BOOL CreateXYChart(int nYColumn);
	BOOL SetCellsValueToString(double Column, double Row, CString szStr);
	BOOL AddArgumentOLEString(LPOLESTR lpszArgName, WORD wFlags, LPOLESTR lpsz);
	BOOL AddArgumentCString(LPOLESTR lpszArgName, WORD wFlags, CString szStr);
	BOOL CreateWorkSheet();
	int AddNewWorkSheet();
	int GetWorksheetsCount();
	CString GetWorksheetName(int nWorksheet);
	BOOL AddArgumentDouble(LPOLESTR lpszArgName, WORD wFlags, double d);
	BOOL AddArgumentBool(LPOLESTR lpszArgName, WORD wFlags, BOOL b);
	BOOL AddArgumentInt2(LPOLESTR lpszArgName, WORD wFlags, int i);
	BOOL AddArgumentDispatch(LPOLESTR lpszArgName, WORD wFlags, IDispatch * pdisp);
	void AddArgumentCommon(LPOLESTR lpszArgName, WORD wFlags, VARTYPE vt);

	BOOL SetActiveWorksheet(int nWorksheet);
	BOOL EnableAlert(BOOL bShowAlert);
	BOOL SetChartTitle(CString cTitle);
	BOOL SetWorksheetTitle(CString wsTitle);
	BOOL SetAxesTitle(CString szXTitle, CString szYTitle);

	BOOL InitOLE();
	CXLAutomation();
	CXLAutomation::CXLAutomation(BOOL bVisible);
	virtual ~CXLAutomation();

protected:
	void ShowException(LPOLESTR szMember, HRESULT hr, EXCEPINFO *pexcep, unsigned int uiArgErr);
	void ReleaseDispatch();
	BOOL SetExcelVisible(BOOL bVisible);
	int ReleaseVariant(VARIANTARG *pvarg);
	void ClearAllArgs();
	void ClearVariant(VARIANTARG *pvarg);
	 
	int			m_iArgCount;
	int			m_iNamedArgCount;
	VARIANTARG	m_aVargs[MAX_DISP_ARGS];
	DISPID		m_aDispIds[MAX_DISP_ARGS + 1];		// one extra for the member name
	LPOLESTR	m_alpszArgNames[MAX_DISP_ARGS + 1];	// used to hold the argnames for GetIDs
	WORD		m_awFlags[MAX_DISP_ARGS];
	IDispatch*	m_pdispActiveWorksheet;



	BOOL ExlInvoke(IDispatch *pdisp, LPOLESTR szMember, VARIANTARG * pvargReturn,
			WORD wInvokeAction, WORD wFlags);
	IDispatch* m_pdispExcelApp;
	IDispatch *m_pdispWorkbook;
	IDispatch *m_pdispWorksheet;
	IDispatch *m_pdispActiveChart;
	BOOL StartExcel();
};

#endif // !defined(AFX_XLAUTOMATION_H__E020CE95_7428_4BEF_A24C_48CE9323C450__INCLUDED_)
