// Microsoft Excel Automation Class by Val Golovlev
//http://www.codeguru.com/cpp/data/mfc_database/microsoftexcel/article.php/c11745
// XLAutomation.cpp: implementation of the CXLAutomation class.
//This is C++ modification of the AutoXL C-sample from 
//Microsoft Excel97 Developer Kit, Microsoft Press 1997 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "XLAutomationTester.h"
#include "XLAutomation.h"
#include <ole2ver.h>
#include <string.h>
#include <winuser.h>
#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
 *  Arrays of argument information, which are used to build up the arg list
 *  for an IDispatch call.  These arrays are statically allocated to reduce
 *  complexity, but this code could be easily modified to perform dynamic
 *  memory allocation.
 *
 *  When arguments are added they are placed into these arrays.  The
 *  Vargs array contains the argument values, and the lpszArgNames array
 *  contains the name of the arguments, or a NULL if the argument is unnamed.
 *  Flags for the argument such as NOFREEVARIANT are kept in the wFlags array.
 *
 *  When Invoke is called, the names in the lpszArgNames array are converted
 *  into the DISPIDs expected by the IDispatch::Invoke function.  The
 *  IDispatch::GetIDsOfNames function is used to perform the conversion, and
 *  the resulting IDs are placed in the DispIds array.  There is an additional
 *  slot in the DispIds and lpszArgNames arrays to allow for the name and DISPID
 *  of the method or property being invoked.
 *  
 *  Because these arrays are static, it is important to call the ClearArgs()
 *  function before setting up arguments.  ClearArgs() releases any memory
 *  in use by the argument array and resets the argument counters for a fresh
 *  Invoke.
 */
//int			m_iArgCount;
//int			m_iNamedArgCount;
//VARIANTARG	m_aVargs[MAX_DISP_ARGS];
//DISPID		m_aDispIds[MAX_DISP_ARGS + 1];		// one extra for the member name
//LPOLESTR	m_alpszArgNames[MAX_DISP_ARGS + 1];	// used to hold the argnames for GetIDs
//WORD		m_awFlags[MAX_DISP_ARGS];
//////////////////////////////////////////////////////////////////////

CXLAutomation::CXLAutomation()
{
	m_pdispExcelApp = NULL;
	m_pdispWorkbook = NULL;
	m_pdispWorksheet = NULL;
	m_pdispActiveChart = NULL;
	InitOLE();
	StartExcel();
	SetExcelVisible(false);
	CreateWorkSheet();
	//CreateXYChart();
}

CXLAutomation::CXLAutomation(BOOL bVisible)
{
	m_pdispExcelApp = NULL;
	m_pdispWorkbook = NULL;
	m_pdispWorksheet = NULL;
	m_pdispActiveChart = NULL;
	InitOLE();
	StartExcel();
	SetExcelVisible(false);
	CreateWorkSheet();
	//CreateXYChart();
}

CXLAutomation::~CXLAutomation()
{
	ReleaseExcel();
	ReleaseDispatch();
	OleUninitialize();
}

BOOL CXLAutomation::InitOLE()
{
	DWORD dwOleVer;
	
	dwOleVer = CoBuildVersion();
	
	// check the OLE library version
	if (rmm != HIWORD(dwOleVer)) 
	{
		MessageBox(NULL, _T("Incorrect version of OLE libraries."), "Failed", MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	
	// could also check for minor version, but this application is
	// not sensitive to the minor version of OLE
	
	// initialize OLE, fail application if we can't get OLE to init.
	if (FAILED(OleInitialize(NULL))) 
	{
		MessageBox(NULL, _T("Cannot initialize OLE."), "Failed", MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	
		
	return TRUE;

}

BOOL CXLAutomation::StartExcel()
{
	CLSID clsExcelApp;

	// if Excel is already running, return with current instance
	if (m_pdispExcelApp != NULL)
		return TRUE;

	/* Obtain the CLSID that identifies EXCEL.APPLICATION
	 * This value is universally unique to Excel versions 5 and up, and
	 * is used by OLE to identify which server to start.  We are obtaining
	 * the CLSID from the ProgID.
	 */
	if (FAILED(CLSIDFromProgID(L"Excel.Application", &clsExcelApp))) 
	{
		MessageBox(NULL, _T("Cannot obtain CLSID from ProgID"), "Failed", MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// start a new copy of Excel, grab the IDispatch interface
	if (FAILED(CoCreateInstance(clsExcelApp, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (void**)&m_pdispExcelApp))) 
	{
		MessageBox(NULL, _T("Cannot start an instance of Excel for Automation."), "Failed", MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	return TRUE;

}

/*******************************************************************
 *
 *								INVOKE
 *
 *******************************************************************/

/*
 *  INVOKE
 *
 *  Invokes a method or property.  Takes the IDispatch object on which to invoke,
 *  and the name of the method or property as a String.  Arguments, if any,
 *  must have been previously setup using the AddArgumentXxx() functions.
 *
 *  Returns TRUE if the call succeeded.  Returns FALSE if an error occurred.
 *  A messagebox will be displayed explaining the error unless the DISP_NOSHOWEXCEPTIONS
 *  flag is specified.  Errors can be a result of unrecognized method or property
 *  names, bad argument names, invalid types, or runtime-exceptions defined
 *  by the recipient of the Invoke.
 *
 *  The argument list is reset via ClearAllArgs() if the DISP_FREEARGS flag is
 *  specified.  If not specified, it is up to the caller to call ClearAllArgs().
 *
 *  The return value is placed in pvargReturn, which is allocated by the caller.
 *  If no return value is required, pass NULL.  It is up to the caller to free
 *  the return value (ReleaseVariant()).
 *
 *  This function calls IDispatch::GetIDsOfNames for every invoke.  This is not
 *  very efficient if the same method or property is invoked multiple times, since
 *  the DISPIDs for a particular method or property will remain the same during
 *  the lifetime of an IDispatch object.  Modifications could be made to this code
 *  to cache DISPIDs.  If the target application is always the same, a similar
 *  modification is to statically browse and store the DISPIDs at compile-time, since
 *  a given application will return the same DISPIDs in different sessions.
 *  Eliminating the extra cross-process GetIDsOfNames call can result in a
 *  signficant time savings.
 */


BOOL CXLAutomation::ExlInvoke(IDispatch *pdisp, LPOLESTR szMember, VARIANTARG * pvargReturn,
			WORD wInvokeAction, WORD wFlags)
{
	HRESULT hr;
	DISPPARAMS dispparams;
	unsigned int uiArgErr;
	EXCEPINFO excep;
	
	// Get the IDs for the member and its arguments.  GetIDsOfNames expects the
	// member name as the first name, followed by argument names (if any).
	m_alpszArgNames[0] = szMember;
	hr = pdisp->GetIDsOfNames( IID_NULL, m_alpszArgNames,
								1 + m_iNamedArgCount, LOCALE_SYSTEM_DEFAULT, m_aDispIds);
	if (FAILED(hr)) 
	{
		if (!(wFlags & DISP_NOSHOWEXCEPTIONS))
			ShowException(szMember, hr, NULL, 0);
		return FALSE;
	}
	
	if (pvargReturn != NULL)
		ClearVariant(pvargReturn);
	
	// if doing a property put(ref), we need to adjust the first argument to have a
	// named arg of DISPID_PROPERTYPUT.
	if (wInvokeAction & (DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF)) 
	{
		m_iNamedArgCount = 1;
		m_aDispIds[1] = DISPID_PROPERTYPUT;
		pvargReturn = NULL;
	}
	
	dispparams.rgdispidNamedArgs = m_aDispIds + 1;
	dispparams.rgvarg = m_aVargs;
	dispparams.cArgs = m_iArgCount;
	dispparams.cNamedArgs = m_iNamedArgCount;
	
	excep.pfnDeferredFillIn = NULL;
	
	hr = pdisp->Invoke(m_aDispIds[0], IID_NULL, LOCALE_SYSTEM_DEFAULT,
								wInvokeAction, &dispparams, pvargReturn, &excep, &uiArgErr);
	
	if (wFlags & DISP_FREEARGS)
		ClearAllArgs();
	
	if (FAILED(hr)) 
	{
		// display the exception information if appropriate:
		if (!(wFlags & DISP_NOSHOWEXCEPTIONS))
			ShowException(szMember, hr, &excep, uiArgErr);
	
		// free exception structure information
		SysFreeString(excep.bstrSource);
		SysFreeString(excep.bstrDescription);
		SysFreeString(excep.bstrHelpFile);
	
		return FALSE;
	}
	return TRUE;
}

/*
 *  ClearVariant
 *
 *  Zeros a variant structure without regard to current contents
 */
void CXLAutomation::ClearVariant(VARIANTARG *pvarg)
{
	pvarg->vt = VT_EMPTY;
	pvarg->wReserved1 = 0;
	pvarg->wReserved2 = 0;
	pvarg->wReserved3 = 0;
	pvarg->lVal = 0;

}

/*
 *  ClearAllArgs
 *
 *  Clears the existing contents of the arg array in preparation for
 *  a new invocation.  Frees argument memory if so marked.
 */
void CXLAutomation::ClearAllArgs()
{
	int i;
	
	for (i = 0; i < m_iArgCount; i++) 
	{
		if (m_awFlags[i] & DISPARG_NOFREEVARIANT)
			// free the variant's contents based on type
			ClearVariant(&m_aVargs[i]);
		else
			ReleaseVariant(&m_aVargs[i]);
	}

	m_iArgCount = 0;
	m_iNamedArgCount = 0;

}

/*
 *  ReleaseVariant
 *
 *  Clears a particular variant structure and releases any external objects
 *  or memory contained in the variant.  Supports the data types listed above.
 */
int CXLAutomation::ReleaseVariant(VARIANTARG *pvarg)
{
	VARTYPE vt;
	VARIANTARG *pvargArray;
	long lLBound, lUBound, l;
	
	vt = pvarg->vt & 0xfff;		// mask off flags
	
	// check if an array.  If so, free its contents, then the array itself.
	if (V_ISARRAY(pvarg)) 
	{
		// variant arrays are all this routine currently knows about.  Since a
		// variant can contain anything (even other arrays), call ourselves
		// recursively.
		if (vt == VT_VARIANT) 
		{
			SafeArrayGetLBound(pvarg->parray, 1, &lLBound);
			SafeArrayGetUBound(pvarg->parray, 1, &lUBound);
			
			if (lUBound > lLBound) 
			{
				lUBound -= lLBound;
				
				SafeArrayAccessData(pvarg->parray, (void**)&pvargArray);
				
				for (l = 0; l < lUBound; l++) 
				{
					ReleaseVariant(pvargArray);
					pvargArray++;
				}
				
				SafeArrayUnaccessData(pvarg->parray);
			}
		}
		else 
		{
			return 1; //  non-variant type

			// MessageBox(NULL, _T("ReleaseVariant: Array contains non-variant type"), "Failed", MB_OK | MB_ICONSTOP);
		}
		
		// Free the array itself.
		SafeArrayDestroy(pvarg->parray);
	}
	else 
	{
		switch (vt) 
		{
			case VT_DISPATCH:
				//(*(pvarg->pdispVal->lpVtbl->Release))(pvarg->pdispVal);
				pvarg->pdispVal->Release();
				break;
				
			case VT_BSTR:
				SysFreeString(pvarg->bstrVal);
				break;
				
			case VT_I2:
			case VT_BOOL:
			case VT_R8:
			case VT_ERROR:		// to avoid erroring on an error return from Excel
				// no work for these types
				break;
				
			default:

				return 2; //unknonw type
				// MessageBox(NULL, _T("ReleaseVariant: Unknown type"), "Failed", MB_OK | MB_ICONSTOP);
				break;
		}
	}
	
	ClearVariant(pvarg);
	return 0;

}

BOOL CXLAutomation::SetExcelVisible(BOOL bVisible)
{
	if (m_pdispExcelApp == NULL)
		return FALSE;
	
	ClearAllArgs();
	AddArgumentBool(NULL, 0, bVisible);
	return ExlInvoke(m_pdispExcelApp, L"Visible", NULL, DISPATCH_PROPERTYPUT, DISP_FREEARGS);

}

/*******************************************************************
 *
 *					   ARGUMENT CONSTRUCTOR FUNCTIONS
 *
 *  Each function adds a single argument of a specific type to the list
 *  of arguments for the current invoke.  If appropriate, memory may be
 *  allocated to represent the argument.  This memory will be
 *  automatically freed the next time ClearAllArgs() is called unless
 *  the NOFREEVARIANT flag is specified for a particular argument.  If
 *  NOFREEVARIANT is specified it is the responsibility of the caller
 *  to free the memory allocated for or contained within the argument.
 *
 *  Arguments may be named.  The name string must be a C-style string
 *  and it is owned by the caller.  If dynamically allocated, the caller
 *  must free the name string.
 *
 *******************************************************************/

/*
 *  Common code used by all variant types for setting up an argument.
 */

void CXLAutomation::AddArgumentCommon(LPOLESTR lpszArgName, WORD wFlags, VARTYPE vt)
{
	ClearVariant(&m_aVargs[m_iArgCount]);
	
	m_aVargs[m_iArgCount].vt = vt;
	m_awFlags[m_iArgCount] = wFlags;
	
	if (lpszArgName != NULL) 
	{
		m_alpszArgNames[m_iNamedArgCount + 1] = lpszArgName;
		m_iNamedArgCount++;
	}
}	
	

BOOL CXLAutomation::AddArgumentDispatch(LPOLESTR lpszArgName, WORD wFlags, IDispatch * pdisp)
{
	AddArgumentCommon(lpszArgName, wFlags, VT_DISPATCH);
	m_aVargs[m_iArgCount++].pdispVal = pdisp;
	return TRUE;
}


BOOL CXLAutomation::AddArgumentInt2(LPOLESTR lpszArgName, WORD wFlags, int i)
{
	AddArgumentCommon(lpszArgName, wFlags, VT_I2);
	m_aVargs[m_iArgCount++].iVal = i;
	return TRUE;
}


BOOL CXLAutomation::AddArgumentBool(LPOLESTR lpszArgName, WORD wFlags, BOOL b)
{
	AddArgumentCommon(lpszArgName, wFlags, VT_BOOL);
	// Note the variant representation of True as -1
	m_aVargs[m_iArgCount++].boolVal = b ? -1 : 0;
	return TRUE;
}

BOOL CXLAutomation::AddArgumentDouble(LPOLESTR lpszArgName, WORD wFlags, double d)
{
	AddArgumentCommon(lpszArgName, wFlags, VT_R8);
	m_aVargs[m_iArgCount++].dblVal = d;
	return TRUE;
}


BOOL CXLAutomation::ReleaseExcel()
{
	if (m_pdispExcelApp == NULL)
		return TRUE;
	
	// Tell Excel to quit, since for automation simply releasing the IDispatch
	// object isn't enough to get the server to shut down.
	
	// Note that this code will hang if Excel tries to display any message boxes.
	// This can occur if a document is in need of saving.  The CreateChart() code
	// always clears the dirty bit on the documents it creates, avoiding this problem.
	ClearAllArgs();
	bool not_quit_message = false;
	ExlInvoke(m_pdispExcelApp, L"Quit", NULL, DISPATCH_METHOD, 0);
	
	// Even though Excel has been told to Quit, we still need to release the
	// OLE object to account for all memory.
	ReleaseDispatch();
	
	return TRUE;

}

//Create an empty workshet
BOOL CXLAutomation::CreateWorkSheet()
{
	if(NULL == m_pdispExcelApp)
		return FALSE;

	BOOL fResult;
	VARIANTARG varg1, varg2;
	IDispatch *pdispRange = NULL;
	IDispatch *pdispActiveSheet = NULL;
	IDispatch *pdispActiveCell = NULL;
	IDispatch *pdispCrt = NULL;
	

	
	// Set wb = [application].Workbooks.Add(template := xlWorksheet)
	ClearAllArgs();
	if (!ExlInvoke(m_pdispExcelApp, L"Workbooks", &varg1, DISPATCH_PROPERTYGET, 0))
		return FALSE;
	
	
	ClearAllArgs();
	AddArgumentInt2(L"Template", 0, xlWorksheet);
	fResult = ExlInvoke(varg1.pdispVal, L"Add", &varg2, DISPATCH_METHOD, 0);
	ReleaseVariant(&varg1);
	if (!fResult)
		return FALSE;
	m_pdispWorkbook = varg2.pdispVal;
	
	// Set ws = wb.Worksheets(1)
	ClearAllArgs();
	AddArgumentInt2(NULL, 0, 1);
	if (!ExlInvoke(m_pdispWorkbook, L"Worksheets", &varg2, DISPATCH_PROPERTYGET, 0))
		goto CreateWsBail;
	m_pdispWorksheet = varg2.pdispVal;

	fResult = TRUE;

CreateWsExit:
	
	if (pdispRange != NULL)
		pdispRange->Release();
	if (pdispCrt != NULL)
		pdispCrt->Release();
	return fResult;
	
CreateWsBail:
	fResult = FALSE;
	goto CreateWsExit;

}

/*
 *  OLE and IDispatch use a BSTR as the representation of strings.
 *  This constructor automatically copies the passed-in C-style string
 *  into a BSTR.  It is important to not set the NOFREEVARIANT flag
 *  for this function, otherwise the allocated BSTR copy will probably
 *  get lost and cause a memory leak.
 */

BOOL CXLAutomation::AddArgumentOLEString(LPOLESTR lpszArgName, WORD wFlags, LPOLESTR lpsz)
{
	BSTR b;
	
	b = SysAllocString(lpsz);
	if (!b)
		return FALSE;
	AddArgumentCommon(lpszArgName, wFlags, VT_BSTR);
	m_aVargs[m_iArgCount++].bstrVal = b;
	return TRUE;

}

BOOL CXLAutomation::AddArgumentCString(LPOLESTR lpszArgName, WORD wFlags, CString szStr)
{
	BSTR b;
	
	b = szStr.AllocSysString();
	if (!b)
		return FALSE;
	AddArgumentCommon(lpszArgName, wFlags, VT_BSTR);
	m_aVargs[m_iArgCount++].bstrVal = b;
	
	return TRUE;
}

//Perform Worksheets.Cells(x,y).Value = szStr
BOOL CXLAutomation::SetCellsValueToString(double Column, double Row, CString szStr)
{
	if(NULL == m_pdispWorksheet)
		return FALSE;
	if(szStr.IsEmpty())
		return FALSE;
	long nBuffSize = szStr.GetLength();
	

	VARIANTARG vargRng;
	
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, Column);
	AddArgumentDouble(NULL, 0, Row);
	if(!ExlInvoke(m_pdispWorksheet, L"Cells",&vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;

    AddArgumentCString(NULL, 0, szStr );
	if (!ExlInvoke(vargRng.pdispVal, L"Value", NULL, DISPATCH_PROPERTYPUT, 0))
		return FALSE;


	if(ReleaseVariant(&vargRng)>=1)
	{
	
	CString error;
	error.Format("Error in outputing row %d, colume %d, value %s", Row, Column, szStr);
	AfxMessageBox(error);
	}
	
	
	return TRUE;
}
//Create XY chart. Y values are in column nYColumn.
BOOL CXLAutomation::CreateXYChart(int nYColumn)
{
	if(NULL == m_pdispWorksheet)
		return FALSE;

	BOOL fResult;
	VARIANTARG varg1, varg2;
	IDispatch *pdispRange = NULL;
	IDispatch *pdispCrt = NULL;

		
	// set sourceRange = ws.Columns(nYColumn)
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, nYColumn);
	if (!ExlInvoke(m_pdispWorksheet, L"Columns", &varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		goto CreateChartBail;
	pdispRange = varg2.pdispVal;
	
	// set crt = wb.Charts.Add
	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorkbook, L"Charts", &varg1, DISPATCH_PROPERTYGET, 0))
		goto CreateChartBail;
	ClearAllArgs();
	fResult = ExlInvoke(varg1.pdispVal, L"Add", &varg2, DISPATCH_METHOD, 0);
	ReleaseVariant(&varg1);
	if (!fResult)
		goto CreateChartBail;
	pdispCrt = varg2.pdispVal;

	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorkbook, L"ActiveChart", &varg1, DISPATCH_PROPERTYGET, 0))
		goto CreateChartBail;
	m_pdispActiveChart = varg1.pdispVal;

	ClearAllArgs();
	AddArgumentInt2(NULL, 0, xlXYScatterLinesNoMarkers);
	if (!ExlInvoke(m_pdispActiveChart, L"ChartType", &varg1, DISPATCH_PROPERTYPUT, 0))
		goto CreateChartBail;

	//Charts.Add
    //ActiveChart.ChartType = xlXYScatterLinesNoMarkers
    //ActiveChart.SetSourceData Source:=Sheets("Sheet1").Range("A:A, B:B"), PlotBy:= _
    //    xlColumns
	ClearAllArgs();
	AddArgumentInt2(L"PlotBy", 0, xlColumns);
	AddArgumentDispatch(L"Source", 0, pdispRange);	// will auto-free
	pdispRange = NULL;
	if (!ExlInvoke(m_pdispActiveChart, L"SetSourceData", NULL, DISPATCH_METHOD, DISP_FREEARGS))
		goto CreateChartBail;


	fResult = TRUE;

CreateChartExit:
	if (pdispRange != NULL)
		pdispRange->Release();
	if (pdispCrt != NULL)
		pdispCrt->Release();
	return fResult;
	
CreateChartBail:
	fResult = FALSE;
	goto CreateChartExit;

	return TRUE;

}

BOOL CXLAutomation::SetRangeValueDouble(LPOLESTR lpszRef, double d)
{
	if(NULL == m_pdispWorksheet)
		return FALSE;

	VARIANTARG vargRng;
	BOOL fResult;
	
	ClearAllArgs();
	AddArgumentOLEString(NULL, 0, lpszRef);
	if (!ExlInvoke(m_pdispWorksheet, L"Range", &vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	
	AddArgumentDouble(NULL, 0, d);
	fResult = ExlInvoke(vargRng.pdispVal, L"Value", NULL, DISPATCH_PROPERTYPUT, 0);
	ReleaseVariant(&vargRng);
	
	return fResult;

}

/*
 *  Constructs an 1-dimensional array containing variant strings.  The strings
 *  are copied from an incoming array of C-Strings.
 */
BOOL CXLAutomation::AddArgumentCStringArray(LPOLESTR lpszArgName, WORD wFlags, LPOLESTR *paszStrings, int iCount)
{
	SAFEARRAY *psa;
	SAFEARRAYBOUND saBound;
	VARIANTARG *pvargBase;
	VARIANTARG *pvarg;
	int i, j;
	
	saBound.lLbound = 0;
	saBound.cElements = iCount;
	
	psa = SafeArrayCreate(VT_VARIANT, 1, &saBound);
	if (psa == NULL)
		return FALSE;
	
	SafeArrayAccessData(psa, (void**) &pvargBase);
	
	pvarg = pvargBase;
	for (i = 0; i < iCount; i++) 
	{
		// copy each string in the list of strings
		ClearVariant(pvarg);
		pvarg->vt = VT_BSTR;
		if ((pvarg->bstrVal = SysAllocString(*paszStrings++)) == NULL) 
		{
			// memory failure:  back out and free strings alloc'ed up to
			// now, and then the array itself.
			pvarg = pvargBase;
			for (j = 0; j < i; j++) 
			{
				SysFreeString(pvarg->bstrVal);
				pvarg++;
			}
			SafeArrayDestroy(psa);
			return FALSE;
		}
		pvarg++;
	}
	
	SafeArrayUnaccessData(psa);

	// With all memory allocated, setup this argument
	AddArgumentCommon(lpszArgName, wFlags, VT_VARIANT | VT_ARRAY);
	m_aVargs[m_iArgCount++].parray = psa;
	return TRUE;

}
//May be new points have been added to the plot data source. Update plot range
BOOL CXLAutomation::UpdatePlotRange(int nYColumn)
{
	if((NULL == m_pdispWorksheet) || (NULL == m_pdispWorkbook) || (NULL == m_pdispActiveChart))
		return FALSE;

	VARIANTARG varg1;
	IDispatch *pdispRange = NULL;
	IDispatch *pdispActiveChart = NULL;
	BOOL bResult = TRUE;
	
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, nYColumn);
	if (!ExlInvoke(m_pdispWorksheet, L"Columns", &varg1, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	pdispRange = varg1.pdispVal;

   //ActiveChart.SetSourceData Source:=Sheets("Sheet1").Range("A:A, B:B"), PlotBy:= _
   //    xlColumns
	ClearAllArgs();
	AddArgumentInt2(L"PlotBy", 0, xlColumns);
	AddArgumentDispatch(L"Source", 0, pdispRange);	// will auto-free
	pdispRange = NULL;
	if (!ExlInvoke(m_pdispActiveChart, L"SetSourceData", NULL, DISPATCH_METHOD, DISP_FREEARGS))
		bResult = FALSE;

	ClearAllArgs();
	if(NULL != pdispRange)
	{
		pdispRange->Release();
		pdispRange = NULL;
	}
	
	return bResult;
	
}

//Copy string to clipboard and paste it to worksheet
BOOL CXLAutomation::PasteStringToWorksheet(CString pDataBuffer)
{
	if(NULL == m_pdispWorksheet)
		return FALSE;
	if(pDataBuffer.IsEmpty())
		return FALSE;

	long nBuffSize = pDataBuffer.GetLength(); 
	//Nothing to copy
	if(0 == nBuffSize)
		return FALSE;

	HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, 24);
  
	  if (OpenClipboard(NULL) )
	  {
		HGLOBAL hClipMem;
		char* buffer;
		BOOL bResult = TRUE;
		
		EmptyClipboard();
		hClipMem = GlobalAlloc(GMEM_DDESHARE, nBuffSize);
		buffer = (char*)GlobalLock(hClipMem);
		memcpy(buffer, pDataBuffer, nBuffSize);
		GlobalUnlock(hClipMem);
		SetClipboardData(CF_TEXT, hClipMem);
		CloseClipboard();
		
		//Paste data from clipboard
		// set sourceRange = ws.Range("A1")
		IDispatch* pdispRange = NULL;
		VARIANTARG varg1, varg2;

		ClearAllArgs();
		AddArgumentOLEString(NULL, 0, L"A1");
		if (!ExlInvoke(m_pdispWorksheet, L"Range", &varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
			return FALSE;
		pdispRange = varg2.pdispVal;


		//Force past from clipboard
		//set as = sourceRange.Select
		ClearAllArgs();
		if (!ExlInvoke(pdispRange, L"Select", &varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
			goto Failed;

		// set wb.ActiveSheet.Paste
		ClearAllArgs();
		if (!ExlInvoke(m_pdispWorkbook, L"ActiveSheet", &varg1, DISPATCH_PROPERTYGET, 0))
			goto Failed;
		ClearAllArgs();

		// set wb.ActiveSheet.Paste
		ClearAllArgs();
		if (!ExlInvoke(m_pdispWorkbook, L"ActiveSheet", &varg1, DISPATCH_PROPERTYGET, 0))
			goto Failed;
		ClearAllArgs();
		bResult = ExlInvoke(varg1.pdispVal, L"Paste", &varg2, DISPATCH_METHOD, 0);
		ReleaseVariant(&varg1);
	
		goto Success;

Failed:
		bResult = FALSE;
Success:
		if(NULL != pdispRange)
		{
			pdispRange->Release();
			pdispRange = NULL;
		}

		return bResult;
	  }
	  return FALSE;

}
//Clean up: release dipatches
void CXLAutomation::ReleaseDispatch()
{
	if(NULL != m_pdispExcelApp)
	{
		m_pdispExcelApp->Release();
		m_pdispExcelApp = NULL;
	}

	if(NULL != m_pdispWorksheet)
	{
		m_pdispWorksheet->Release();
		m_pdispWorksheet = NULL;
	}

	if(NULL != m_pdispWorkbook)
	{
		m_pdispWorkbook->Release();
		m_pdispWorkbook = NULL;
	}

	if(NULL != m_pdispActiveChart)
	{
		m_pdispActiveChart->Release();
		m_pdispActiveChart = NULL;
	}

}

void CXLAutomation::ShowException(LPOLESTR szMember, HRESULT hr, EXCEPINFO *pexcep, unsigned int uiArgErr)
{
	TCHAR szBuf[512];
	
	switch (GetScode(hr)) 
	{
		case DISP_E_UNKNOWNNAME:
			wsprintf(szBuf, "%s: Unknown name or named argument.", szMember);
			break;
	
		case DISP_E_BADPARAMCOUNT:
			wsprintf(szBuf, "%s: Incorrect number of arguments.", szMember);
			break;
			
		case DISP_E_EXCEPTION:
			wsprintf(szBuf, "%s: Error %d: ", szMember, pexcep->wCode);
			if (pexcep->bstrDescription != NULL)
				lstrcat(szBuf, (char*)pexcep->bstrDescription);
			else
				lstrcat(szBuf, "<<No Description>>");
			break;
			
		case DISP_E_MEMBERNOTFOUND:
			wsprintf(szBuf, "%s: method or property not found.", szMember);
			break;
		
		case DISP_E_OVERFLOW:
			wsprintf(szBuf, "%s: Overflow while coercing argument values.", szMember);
			break;
		
		case DISP_E_NONAMEDARGS:
			wsprintf(szBuf, "%s: Object implementation does not support named arguments.",
						szMember);
		    break;
		    
		case DISP_E_UNKNOWNLCID:
			wsprintf(szBuf, "%s: The locale ID is unknown.", szMember);
			break;
		
		case DISP_E_PARAMNOTOPTIONAL:
			wsprintf(szBuf, "%s: Missing a required parameter.", szMember);
			break;
		
		case DISP_E_PARAMNOTFOUND:
			wsprintf(szBuf, "%s: Argument not found, argument %d.", szMember, uiArgErr);
			break;
			
		case DISP_E_TYPEMISMATCH:
			wsprintf(szBuf, "%s: Type mismatch, argument %d.", szMember, uiArgErr);
			break;

		default:
			wsprintf(szBuf, "%s: Unknown error occured.", szMember);
			break;
	}
	
// comment out in automation	MessageBox(NULL, szBuf, "OLE Error", MB_OK | MB_ICONSTOP);

}
//Delete entire line from the current worksheet
//Worksheet.Rows(nLine).Select
//Selection.Delete Shift:=xlUp
BOOL CXLAutomation::DeleteRow(long nRow)
{
	if(NULL == m_pdispWorksheet)
		return FALSE;

	VARIANTARG varg1;
	
	
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, nRow);
	if (!ExlInvoke(m_pdispWorksheet, L"Rows", &varg1, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;

	ClearAllArgs();
	AddArgumentInt2(L"Shift", 0, xlUp);
	if (!ExlInvoke(varg1.pdispVal, L"Delete", NULL, DISPATCH_METHOD, DISP_FREEARGS))
		return FALSE;
	
		
	return TRUE;

}

//Save current workbook as an Excel file:
//ActiveWorkbook.SaveAs
//FileName:=szFileName, FileFormat:=xlNormal,
//Password:=szPassword,
//WriteResPassword:=szWritePassword,
//ReadOnlyRecommended:= bReadOnly,
//CreateBackup:= bBackup
BOOL CXLAutomation::SaveAs(CString szFileName, int nFileFormat, CString szPassword, CString szWritePassword, BOOL bReadOnly, BOOL bBackUp)
{
	if(NULL == m_pdispWorkbook)
		return FALSE;
	ClearAllArgs();
	AddArgumentBool(L"CreateBackup", 0, bBackUp);
	AddArgumentBool(L"ReadOnlyRecommended", 0, bReadOnly);
	AddArgumentCString(L"WriteResPassword", 0, szWritePassword);
	AddArgumentCString(L"Password", 0, szPassword);
	AddArgumentCString(L"FileName", 0, szFileName);
	if (!ExlInvoke(m_pdispWorkbook, L"SaveAs", NULL, DISPATCH_METHOD, DISP_FREEARGS))
		return FALSE;

	return TRUE;
}

//Get Worksheet.Calls(nColumn, nRow).Value
//This method is not fully tested - see code coments 
CString CXLAutomation::GetCellValueCString(int nColumn, int nRow)
{
	CString szValue =_T("");
	if(NULL == m_pdispWorksheet)
		return szValue;
	
	VARIANTARG vargRng, vargValue;
	
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, nColumn);
	AddArgumentDouble(NULL, 0, nRow);
	if(!ExlInvoke(m_pdispWorksheet, L"Cells",&vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return szValue;
    
	if (!ExlInvoke(vargRng.pdispVal, L"Value", &vargValue, DISPATCH_PROPERTYGET, 0))
		return szValue;

	VARTYPE Type = vargValue.vt;
	switch (Type) 
		{
			case VT_UI1:
				{
					unsigned char nChr = vargValue.bVal;
// comment out					szValue = nChr;
				} 
				break;
			case VT_I4:
				{
					long nVal = vargValue.lVal;
					szValue.Format("%i", nVal);
				}
				break;
			case VT_R4:
				{
					float fVal = vargValue.fltVal;
					szValue.Format("%f", fVal);
				}
				break;
			case VT_R8:
				{
					double dVal = vargValue.dblVal;
					szValue.Format("%f", dVal);
				}
				break;
			case VT_BSTR:
				{
					BSTR b = vargValue.bstrVal;
					szValue = b;
				}
				break;
			case VT_BYREF|VT_UI1:
				{
					//Not tested
					unsigned char* pChr = vargValue.pbVal;
// comment out					szValue = *pChr;
				}
				break;
			case VT_BYREF|VT_BSTR:
				{
					//Not tested
					BSTR* pb = vargValue.pbstrVal;
					szValue = *pb;
				}
			case 0:
				{
					//Empty
					szValue = _T("");
				}

				break;
		}
	
		
	ReleaseVariant(&vargRng);
	ReleaseVariant(&vargValue);
	
	return szValue;

}
//Insert picture from file szFileName to worksheet
//The left top corner of the picture is position in (Column, nRow)  
//on the worksheet
//Size of the picture in % of original size is given by dPicWidth, dPicHeight
//If dPicWidth = 0.0, dPicHeight = 0.0 or dPicWidth = 1.0, dPicHeight = 1.0
//the picture has default (i.e., original) size 
BOOL CXLAutomation::InsertPictureToWorksheet(CString szFileName, int Column, int Row, double dPicWidth, double dPicHeight)
{
	//No file name provided. Leave. 
	if(szFileName.IsEmpty())
		return FALSE;
	if(NULL == m_pdispWorksheet)
		return FALSE;

	//First, select cell where you want copy the picture (i.e., the top left corner of the picture
	//Leave if Column and Row are outside the worksheet
	if((Column < 1) || (Row < 1))
		return FALSE;

	VARIANTARG vargRng, vargActiveCell;
	VARIANTARG varg1, varg2;
	
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, Column);
	AddArgumentDouble(NULL, 0, Row);
	if(!ExlInvoke(m_pdispWorksheet, L"Cells",&vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	ClearAllArgs();
	if(!ExlInvoke(vargRng.pdispVal, L"Select",&vargActiveCell, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	ReleaseVariant(&vargRng);
	ReleaseVariant(&vargActiveCell);

	//Run this macros to incert picture from file 
	//ActiveSheet.Pictures.Insert("c:\mypicture.bmp").Select
	//or
	//ActiveSheet.Pictures.Insert ("C:\mypicture.bmp")
    //ActiveSheet.Pictures.ShapeRange.ScaleWidth 0.31, msoScaleFromTopLeft
	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorksheet, L"Pictures", &varg1, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	ClearAllArgs();
	AddArgumentCString(NULL, 0, szFileName);
	if (!ExlInvoke(varg1.pdispVal, L"Insert", &varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;

	if((dPicWidth != 0.0) && (dPicHeight != 0.0))
	{

		//Run this macros to resize the picture:
		//Selection.ShapeRange.ScaleWidth dPicWidth, msoFalse, msoScaleFromTopLeft
		//Selection.ShapeRange.ScaleHeight dPicWidth, msoFalse, msoScaleFromTopLeft
		VARIANTARG vargImage;
		ClearAllArgs();
		if (!ExlInvoke(varg1.pdispVal, L"ShapeRange", &vargImage, DISPATCH_PROPERTYGET, DISP_FREEARGS))
			return FALSE;

		ClearAllArgs();
		//msoScaleFromTopLeft = 0 - this argument indicate scaling from top left
		AddArgumentInt2(NULL, 0, 0);
		AddArgumentDouble(NULL, 0, dPicWidth);
     	if (!ExlInvoke(vargImage.pdispVal, L"ScaleWidth", NULL, DISPATCH_METHOD, 0)) //DISP_FREEARGS))
			return FALSE;
		if (!ExlInvoke(vargImage.pdispVal, L"ScaleHeight", NULL, DISPATCH_METHOD, 0)) //DISP_FREEARGS))
			return FALSE;
		ClearAllArgs();
		ReleaseVariant(&vargImage);
		
	}
	ReleaseVariant(&varg1);
	ReleaseVariant(&varg2);
	return TRUE;
}
//Paste image to clipborad
BOOL CXLAutomation::PlaceImageToClipboard(BYTE *pImage)
{
	if(NULL == pImage)
		return FALSE;
	LPBITMAPINFOHEADER pBmpHeader = (LPBITMAPINFOHEADER) pImage;
	long nImageBufferSize;
	//Find the size of buffer to store this image
	long nBitCount = pBmpHeader->biBitCount;
	long nWidth = pBmpHeader->biWidth;
	long nHeight = pBmpHeader->biHeight;


	//We will support only 8-bit and 24-bit images
	//8-bit image has 4x256 = 1024-byte palette table followed after the bitmapinfoheader 
	//24-bit image does not have the palette table
	//When calculating buffer size remember that the BMP image is 32-bit alligned
	if((nBitCount != 8) && (nBitCount != 24))
		return FALSE;
	//calculate number of bytes in one image line
	long nBytePerLine = (nBitCount / 8) * nWidth;
	//Here, for instance 72-bit line should be rounded to 96-bit to be 32-bit aligned 
	nBytePerLine =  4 * ((nBytePerLine + 3) / 4); 
	nImageBufferSize = sizeof(BITMAPINFOHEADER) + (nBytePerLine * nHeight);
	//if image is 8-bit, add the size of palette table
	if(8 == nBitCount)
		nImageBufferSize = nImageBufferSize + 256 * sizeof(RGBQUAD);

	  //Place image to clipboard 	 
	  if (OpenClipboard(NULL) )
	  {
		HGLOBAL hClipMem;
		BYTE* buffer;
		BOOL bResult = TRUE;
		EmptyClipboard();
		hClipMem = GlobalAlloc(GMEM_DDESHARE, nImageBufferSize);
		//hClipMem = GlobalAlloc(GMEM_MOVEABLE, nImageBufferSize);
		buffer = (BYTE*)GlobalLock(hClipMem);
		memcpy(buffer, pImage, nImageBufferSize);
		SetClipboardData(CF_DIB, hClipMem);
		GlobalUnlock(hClipMem);
		CloseClipboard();
		return TRUE;
	  }
	return FALSE;
}
//Insert image to worksheet using clipboard
BOOL CXLAutomation::InsertPictureToWorksheet(BYTE *pImage, int Column, int Row, double dPicWidth, double dPicHeight)
{
	//Place image to clipboard
	if(NULL != pImage)
		if(!PlaceImageToClipboard(pImage))
			return FALSE;
	
	//Select cell where you want copy the picture (i.e., the top left corner of the picture
	//Leave if Column and Row are outside the worksheet
	if((Column < 1) || (Row < 1))
		return FALSE;

	VARIANTARG vargCell, vargActiveCell;
	VARIANTARG vargActiveSelection;
	
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, Column);
	AddArgumentDouble(NULL, 0, Row);
	if(!ExlInvoke(m_pdispWorksheet, L"Cells",&vargCell, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	ClearAllArgs();
	if(!ExlInvoke(vargCell.pdispVal, L"Select",&vargActiveCell, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	ReleaseVariant(&vargCell);
	ReleaseVariant(&vargActiveCell);

	//Paste image from clipboard
	//by runing:
	//ActiveSheet.Paste
	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorksheet, L"Paste", &vargActiveSelection, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	ReleaseVariant(&vargActiveSelection);

	//Select image
	if (!ExlInvoke(m_pdispWorksheet, L"Pictures", &vargActiveSelection, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	//Resize image
	if((dPicWidth != 0.0) && (dPicHeight != 0.0))
	{
	
		//Run this macros to resize the picture:
		//Selection.ShapeRange.ScaleWidth dPicWidth, msoFalse, msoScaleFromTopLeft
		//Selection.ShapeRange.ScaleHeight dPicWidth, msoFalse, msoScaleFromTopLeft
		VARIANTARG vargImage;
		ClearAllArgs();
		if (!ExlInvoke(vargActiveSelection.pdispVal, L"ShapeRange", &vargImage, DISPATCH_PROPERTYGET, DISP_FREEARGS))
			return FALSE;

		ClearAllArgs();
		//msoScaleFromTopLeft = 0 - this argument indicate scaling from top left
		AddArgumentInt2(NULL, 0, 0);
		AddArgumentDouble(NULL, 0, dPicWidth);
     	if (!ExlInvoke(vargImage.pdispVal, L"ScaleWidth", NULL, DISPATCH_METHOD, 0)) //DISP_FREEARGS))
			return FALSE;
		if (!ExlInvoke(vargImage.pdispVal, L"ScaleHeight", NULL, DISPATCH_METHOD, 0)) //DISP_FREEARGS))
			return FALSE;
		ClearAllArgs();
		ReleaseVariant(&vargImage);
		
	}
	ReleaseVariant(&vargActiveSelection);
	return TRUE;
}
//Open Microsoft Excel file and switch to the firs available worksheet. 

BOOL CXLAutomation::OpenExcelFile(CString szFileName, CString WorkSheetName, int 	ActiveSheet)
	//	return false;)
{
	//Leave if the file cannot be open
	if(NULL == m_pdispExcelApp)
		return FALSE;
	if(szFileName.IsEmpty())
		return FALSE;

	VARIANTARG varg1, vargWorkbook, vargWorksheet;
	ClearAllArgs();
	if (!ExlInvoke(m_pdispExcelApp, L"Workbooks", &varg1, DISPATCH_PROPERTYGET, 0))
		return FALSE;
		
	ClearAllArgs();
	AddArgumentCString(L"Filename", 0, szFileName);
	if (!ExlInvoke(varg1.pdispVal, L"Open", &vargWorkbook, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;

	
	WorkSheetName.MakeLower();

	int size = GetWorksheetsCount();

	//Now let's get the first worksheet of this workbook
	ClearAllArgs();
	AddArgumentInt2(NULL, 0, ActiveSheet);
	if (!ExlInvoke(vargWorkbook.pdispVal, L"Worksheets", &vargWorksheet, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;


	//Close the empty worksheet
	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorkbook, L"Close", NULL, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	//Remember the newly open worksheet 
	m_pdispWorkbook = vargWorkbook.pdispVal;
	m_pdispWorksheet = vargWorksheet.pdispVal;

	size = GetWorksheetsCount();


	CString Title = GetWorksheetName(ActiveSheet);

	Title.MakeLower();
	WorkSheetName.MakeLower();

	if(Title != WorkSheetName)
	{

		CString msg;
		msg.Format("Please ensure the no.%d work sheet is called %s", ActiveSheet, WorkSheetName);
	AfxMessageBox(msg);

	return false;
	}


	return TRUE;	

}
BOOL CXLAutomation::OpenExcelFile(CString szFileName, int ActiveSheet = 1)
{

	//Leave if the file cannot be open
	if(NULL == m_pdispExcelApp)
		return FALSE;
	if(szFileName.IsEmpty())
		return FALSE;

	VARIANTARG varg1, vargWorkbook, vargWorksheet;
	ClearAllArgs();
	if (!ExlInvoke(m_pdispExcelApp, L"Workbooks", &varg1, DISPATCH_PROPERTYGET, 0))
		return FALSE;
		
	ClearAllArgs();
	AddArgumentCString(L"Filename", 0, szFileName);
	if (!ExlInvoke(varg1.pdispVal, L"Open", &vargWorkbook, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;

	//Now let's get the first worksheet of this workbook
	ClearAllArgs();
	AddArgumentInt2(NULL, 0, ActiveSheet);
	if (!ExlInvoke(vargWorkbook.pdispVal, L"Worksheets", &vargWorksheet, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;

	//Close the empty worksheet
	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorkbook, L"Close", NULL, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	//Remember the newly open worksheet 
	m_pdispWorkbook = vargWorkbook.pdispVal;
	m_pdispWorksheet = vargWorksheet.pdispVal;
	return TRUE;
}


//Create an empty worksheet
int CXLAutomation::AddNewWorkSheet()
{
	if(NULL == m_pdispExcelApp)
		return -1;

	BOOL fResult;
	VARIANTARG varg1, varg2;
	IDispatch *pdispRange = NULL;
	IDispatch *pdispActiveSheet = NULL;
	IDispatch *pdispActiveCell = NULL;
	IDispatch *pdispCrt = NULL;
	

	
	// Set wb = [application].Workbooks.Add(template := xlWorksheet)
	ClearAllArgs();
	if (!ExlInvoke(m_pdispExcelApp, L"Workbooks", &varg1, DISPATCH_PROPERTYGET, 0))
		return -1;
	
	//First time here: no workbook yet. Create new workbook.
	if(NULL == m_pdispWorkbook)
	{
		ClearAllArgs();
		AddArgumentInt2(L"Template", 0, xlWorksheet);
		fResult = ExlInvoke(varg1.pdispVal, L"Add", &varg2, DISPATCH_METHOD, 0);
		ReleaseVariant(&varg1);
		if (!fResult)
			return -1;
		m_pdispWorkbook = varg2.pdispVal;
	
	
		// Set ws = wb.Worksheets(nNextWorksheet)
		ClearAllArgs();
		AddArgumentInt2(NULL, 0, 1);
		if (!ExlInvoke(m_pdispWorkbook, L"Worksheets", &varg2, DISPATCH_PROPERTYGET, 0))
			return -1;
		m_pdispActiveWorksheet = varg2.pdispVal;

		return GetWorksheetsCount();
	}

	//Add a new worksheet to the existing workbook
	ClearAllArgs();
	if(!ExlInvoke(m_pdispWorkbook, L"Sheets",&varg1, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return -1;
	if (!ExlInvoke(varg1.pdispVal, L"Add", &varg2, DISPATCH_PROPERTYGET, 0))
		return -1;
	
	m_pdispActiveWorksheet = varg2.pdispVal;
		
	return GetWorksheetsCount();
}

//Return the number of worksheets in this book by Worksheets.Count
int CXLAutomation::GetWorksheetsCount()
{
	if(NULL == m_pdispWorkbook)
		return -1;

	VARIANTARG varg1, varg2;
	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorkbook, L"Worksheets", &varg1, DISPATCH_PROPERTYGET, 0))
		return -1;
	ClearAllArgs();
	if (!ExlInvoke(varg1.pdispVal, L"Count", &varg2, DISPATCH_PROPERTYGET, 0))
		return -1;

	VARTYPE Type = varg2.vt;
	if(Type != VT_I4)
		return -1;

	int n = varg2.lVal; 
	return varg2.lVal;
}

//Set worksheet for input and output
BOOL CXLAutomation::SetActiveWorksheet(int nWorksheet)
{
	
	// Set ws = Sheets(nNextWorksheet).Select
	//How many worksheets we have?
	int nCount = GetWorksheetsCount();
	if((nWorksheet < 1) || (nWorksheet > nCount))
		return FALSE;
	//Worksheets are counted a strange way: the last added have to be called as 1
	int nActiveWorksheet = nCount - nWorksheet + 1;

	VARIANTARG varg1, varg2;
	ClearAllArgs();
	AddArgumentInt2(NULL, 0, nWorksheet);
	if(!ExlInvoke(m_pdispWorkbook, L"Worksheets",&varg1, DISPATCH_PROPERTYGET, DISP_FREEARGS)) //DISP_FREEARGS))
		return FALSE;
	ClearAllArgs();
	if (!ExlInvoke(varg1.pdispVal, L"Select", &varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
		
	m_pdispActiveWorksheet = varg1.pdispVal;

	return TRUE;
}

//Set chart's X and Y titles by executing this macros:
//ActiveChart.ChartArea.Select
//ActiveChart.Axes(xlCategory, xlPrimary).HasTitle = True
//ActiveChart.Axes(xlCategory, xlPrimary).AxisTitle.Characters.Text = szXTitle
//ActiveChart.Axes(xlValue, xlPrimary).HasTitle = True
// ActiveChart.Axes(xlValue, xlPrimary).AxisTitle.Characters.Text = szYTitle
//xlCategory = 1
//xlPrimary = 1
//xlValue = 2
BOOL CXLAutomation::SetAxesTitle(CString szXTitle, CString szYTitle)
{
	//Leave if there is no chart
	if(NULL == m_pdispActiveChart)
		return FALSE;
	
	int xlCategory = 1;
	int xlPrimary = 1;
	int xlValue = 2;
	VARIANTARG varg1, varg2;

	//Select chart: ActiveChart.ChartArea.Select
	ClearAllArgs();
	if(!ExlInvoke(m_pdispActiveChart, L"ChartArea",&varg1, DISPATCH_PROPERTYGET, DISP_FREEARGS)) //DISP_FREEARGS))
		return FALSE;
	ClearAllArgs();
	if (!ExlInvoke(varg1.pdispVal, L"Select", NULL, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	
	//Set title only if the title has been provided by user
	if(!szXTitle.IsEmpty())
	{
			//ActiveChart.Axes(xlCategory, xlPrimary).HasTitle = True
			ClearAllArgs();
			AddArgumentInt2(NULL, 0, xlPrimary);
			AddArgumentInt2(NULL, 0, xlCategory);
			if(!ExlInvoke(m_pdispActiveChart, L"Axes",&varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
				return FALSE;
			
			ClearAllArgs();
			AddArgumentBool(NULL, 0, TRUE);
			if(!ExlInvoke(varg2.pdispVal, L"HasTitle",NULL, DISPATCH_PROPERTYPUT, DISP_FREEARGS))
				return FALSE;

			//ActiveChart.Axes(xlCategory, xlPrimary).AxisTitle.Characters.Text = szXTitle
			VARIANTARG vargTitle, vargChar;
			ClearAllArgs();
			if (!ExlInvoke(varg2.pdispVal, L"AxisTitle", &vargTitle, DISPATCH_PROPERTYGET, 0))
				return FALSE;
			ClearAllArgs();
			if (!ExlInvoke(vargTitle.pdispVal, L"Characters", &vargChar, DISPATCH_PROPERTYGET, 0))
				return FALSE;
			ClearAllArgs();
			AddArgumentCString(NULL, 0, szXTitle);
			if (!ExlInvoke(vargTitle.pdispVal, L"Text", NULL, DISPATCH_PROPERTYPUT, 0))
				return FALSE;

			ReleaseVariant(&vargChar);
			ReleaseVariant(&vargTitle);
			ReleaseVariant(&varg2);
	}
	
	if(!szYTitle.IsEmpty())
	{
			//ActiveChart.Axes(xlValue, xlPrimary).HasTitle = True
			ClearAllArgs();
			AddArgumentInt2(NULL, 0, xlPrimary);
			AddArgumentInt2(NULL, 0, xlValue);
			if(!ExlInvoke(m_pdispActiveChart, L"Axes",&varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
				return FALSE;
			
			ClearAllArgs();
			AddArgumentBool(NULL, 0, TRUE);
			if(!ExlInvoke(varg2.pdispVal, L"HasTitle",NULL, DISPATCH_PROPERTYPUT, DISP_FREEARGS))
				return FALSE;

			//ActiveChart.Axes(xlValue, xlPrimary).AxisTitle.Characters.Text = szYTitle
			VARIANTARG vargTitle, vargChar;
			ClearAllArgs();
			if (!ExlInvoke(varg2.pdispVal, L"AxisTitle", &vargTitle, DISPATCH_PROPERTYGET, 0))
				return FALSE;
			ClearAllArgs();
			if (!ExlInvoke(vargTitle.pdispVal, L"Characters", &vargChar, DISPATCH_PROPERTYGET, 0))
				return FALSE;
			ClearAllArgs();
			AddArgumentCString(NULL, 0, szYTitle);
			if (!ExlInvoke(vargTitle.pdispVal, L"Text", NULL, DISPATCH_PROPERTYPUT, 0))
				return FALSE;
	}

	return TRUE;
}

BOOL CXLAutomation::SetWorksheetTitle(CString wsTitle)
{
	//Sheets(Sheets.Count).Name = OtherName 
	ClearAllArgs();	
	AddArgumentCString(NULL, 0, wsTitle);
	if (!ExlInvoke(m_pdispActiveWorksheet, L"Name", NULL, DISPATCH_PROPERTYPUT, 0))
		return FALSE;

	return TRUE;
}

BOOL CXLAutomation::SetChartTitle(CString cTitle)
{	
	//Sheets(Sheets.Count).Name = OtherName 
	ClearAllArgs();	
	AddArgumentCString(NULL, 0, cTitle);
	if (!ExlInvoke(m_pdispActiveChart, L"Name", NULL, DISPATCH_PROPERTYPUT, 0))
		return FALSE;
	
	//.ChartTitle.Characters.Text = "My Chart Title"
	VARIANTARG vargTitle, vargChar;
	ClearAllArgs();
	if (!ExlInvoke(m_pdispActiveChart, L"ChartTitle", &vargTitle, DISPATCH_PROPERTYGET, 0))
		return FALSE;
	ClearAllArgs();
	if (!ExlInvoke(vargTitle.pdispVal, L"Characters", &vargChar, DISPATCH_PROPERTYGET, 0))
		return FALSE;
	ClearAllArgs();
	AddArgumentCString(NULL, 0, cTitle);
	if (!ExlInvoke(vargTitle.pdispVal, L"Text", NULL, DISPATCH_PROPERTYPUT, 0))
		return FALSE;

	return TRUE;
}

//Get worksheet name as CString
//The worksheet is defined by number, i.e., 1,2,3 ...
CString CXLAutomation::GetWorksheetName(int nWorksheet)
{

	CString szName = _T("");
	
	VARIANTARG varg1, varg2;
	ClearAllArgs();
	AddArgumentInt2(NULL, 0, nWorksheet);
	if(!ExlInvoke(m_pdispWorkbook, L"Worksheets",&varg1, DISPATCH_PROPERTYGET, DISP_FREEARGS)) //DISP_FREEARGS))
		return szName;

	//Execute macros
	//szName  = Worksheets(nWorksheet).Name
	ClearAllArgs();
	if (!ExlInvoke(varg1.pdispVal, L"Name", &varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return szName;

	//Get the name from varg2
	VARTYPE Type = varg2.vt;
	if(Type == VT_BSTR)
	{
		BSTR b = varg2.bstrVal;
		szName = b;
	}

	return szName;
	
}

//To enable/disable Excel alerts - add this method to CXLAutomation and modify CXLEzAutomation accordingly:

// bShowAlert = FALSE will disable alerts
// bShowAlert = TRUE will enable alerts
BOOL CXLAutomation::EnableAlert(BOOL bShowAlert)
{
    if(NULL == m_pdispExcelApp)
		return FALSE;

	ClearAllArgs();
	AddArgumentBool(NULL, bShowAlert, true);
	if(!ExlInvoke(m_pdispExcelApp,  L"Worksheets", NULL, DISPATCH_METHOD, DISP_FREEARGS))
  		return FALSE; 
	else
		return TRUE;

}
//Before closing server in your application disable alerts by calling:
//XL.EnableAlerts(FALSE);
//XL.ReleaseExcel();
