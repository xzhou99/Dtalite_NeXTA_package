// RecordsetExt.cpp : implementation file
//

#include "StdAfx.h"
#ifndef _WIN64
#include "RecordSetExt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordsetExt

IMPLEMENT_DYNAMIC(CRecordsetExt, CDaoRecordset )

   CRecordsetExt::CRecordsetExt(CDaoDatabase* pdb)
   : CDaoRecordset(pdb)
{
   //{{AFX_FIELD_INIT(CRecordsetExt)
   //}}AFX_FIELD_INIT

}


CString CRecordsetExt::GetDefaultDBName()
{
   return _T("");
}

CString CRecordsetExt::GetDefaultSQL()
{
   return _T("");
}

void CRecordsetExt::DoFieldExchange(CFieldExchange* pFX)
{
   //{{AFX_FIELD_MAP(CRecordsetExt)
   pFX->SetFieldType(CFieldExchange::outputColumn);
   //}}AFX_FIELD_MAP
}


////////////////////////////////////////////////////////////////
// CRecordsetExt class

// Get by Column Name
CString CRecordsetExt::GetCString(CString ColumnName, bool bErrorMessage)
{
   COleVariant covFieldValue;
   CString Holder;

   try
   {
      GetFieldValue(ColumnName, covFieldValue);
   }
   catch(CDaoException* e)
   {
	   if(bErrorMessage)
	   {
      CString error = "Field not found:" + ColumnName + "\n";
      AfxMessageBox(error ,MB_OK|MB_ICONEXCLAMATION);

	for(int i = 0; i< GetFieldCount(); i++)
	{
		CDaoFieldInfo fieldinfo;

		GetFieldInfo(i,fieldinfo);
		TRACE("index %d: fieldinfo %s\n",i,fieldinfo.m_strName);
	}
   
	   }
      return "";
   }

   Holder.Format("%s",covFieldValue.pbstrVal);
   return Holder;
};

long CRecordsetExt::GetLong(CString ColumnName, bool &bExist, bool bRequired=false )
{
   COleVariant covFieldValue;
   VARIANT *vFieldValue;
   bExist=true;
   try
   {
      GetFieldValue(ColumnName, covFieldValue);
   }
   catch(CDaoException* e)
   {
      if(bRequired)
      {		CString error = "Field not found:" + ColumnName + "\n";
	 AfxMessageBox(error ,MB_OK|MB_ICONEXCLAMATION);
      }
      bExist=false;
      return -999;
   }

   vFieldValue = (LPVARIANT)covFieldValue;
   VARIANTARG   vargDest={0};
   VariantChangeType(&vargDest,vFieldValue,0, VT_I4);
   return vargDest.lVal;
};

double CRecordsetExt:: GetDouble(CString ColumnName, bool &bExist,bool bRequired=false)
{
   COleVariant covFieldValue;
   VARIANT *vFieldValue;
   bExist=true;

   try
   {
      GetFieldValue(ColumnName, covFieldValue);
   }
   catch(CDaoException* e)
   {
      if(bRequired)
      {
	 CString error = "Field " + ColumnName + "  not found.";
	 AfxMessageBox(error ,MB_OK|MB_ICONEXCLAMATION);
      }
      bExist=false;
	for(int i = 0; i< GetFieldCount(); i++)
	{
		CDaoFieldInfo fieldinfo;

		GetFieldInfo(i,fieldinfo);
		TRACE("index %d: fieldinfo %s\n",i,fieldinfo.m_strName);
	}
   
      return -999.0;
   }

   vFieldValue = (LPVARIANT)covFieldValue;
   VARIANTARG   vargDest={0};
   VariantChangeType(&vargDest,vFieldValue,0, VT_R8);
   return vargDest.dblVal;
}
double CRecordsetExt::GetDouble(int index, bool &bExist,bool bRequired)
{
   COleVariant covFieldValue;
   VARIANT *vFieldValue;
   bExist=true;

   try
   {
      GetFieldValue(index, covFieldValue);
   }
   catch(CDaoException* e)
   {
      if(bRequired)
      {
	 CString error;
	 error.Format("Field index %d not found.",index);
	 AfxMessageBox(error ,MB_OK|MB_ICONEXCLAMATION);
      }
      bExist=false;
	for(int i = 0; i< GetFieldCount(); i++)
	{
		CDaoFieldInfo fieldinfo;

		GetFieldInfo(i,fieldinfo);
		TRACE("index %d: fieldinfo %s\n",i,fieldinfo.m_strName);
	}
   
      return -999.0;
   }

   vFieldValue = (LPVARIANT)covFieldValue;
   VARIANTARG   vargDest={0};
   VariantChangeType(&vargDest,vFieldValue,0, VT_R8);
   return vargDest.dblVal;
}

float CRecordsetExt::GetFloat(CString ColumnName)
{
   COleVariant covFieldValue;
   VARIANT *vFieldValue;

   try
   {
      GetFieldValue(ColumnName, covFieldValue);
   }
   catch(CDaoException* e)
   {
      CString error = "Field not found:" + ColumnName + "\n";
      AfxMessageBox(error ,MB_OK|MB_ICONEXCLAMATION);
      return -999.0;
   }

   vFieldValue = (LPVARIANT)covFieldValue;
   VARIANTARG   vargDest={0};
   VariantChangeType(&vargDest,vFieldValue,0, VT_R4);
   return vargDest.fltVal;
}


#endif