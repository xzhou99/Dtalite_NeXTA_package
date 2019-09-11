// Microsoft Excel Automation Class by Val Golovlev
//http://www.codeguru.com/cpp/data/mfc_database/microsoftexcel/article.php/c11745
// EzAutomation.h: interface for the CXLEzAutomation class.
//
//enhanced by Xuesong Zhou for reading Excel file as a CSV parser

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EzAutomation_H__D140B9A3_1995_40AC_8E6D_8F23A95A63A2__INCLUDED_)
#define AFX_EzAutomation_H__D140B9A3_1995_40AC_8E6D_8F23A95A63A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XLAutomation.h"
#define xlNormal -4143

#include "XLAutomation.h"
#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
using namespace std;
using std::string;
using std::ifstream;
using std::vector;
using std::map;
using std::istringstream;

class CXLEzAutomation  
{

private:
	char Delimiter;
	bool IsFirstLineHeader;
	vector<string> LineFieldsValue;
	vector<string> Headers;
	map<string,int> FieldsIndices;

	int m_RowNo;
	int m_ColumnNo;


public:

	vector<string> GetHeaderVector()
	{
		return Headers;
	}

	int m_EmptyLineCount ;
	bool m_bSynchroSingleCSVFile;
	string m_SynchroSectionName;
	bool m_bLastSectionRead;

	bool m_bSkipFirstLine;  // for synchro CSV files

	bool PhasePage(string fileName, bool bIsFirstLineHeader = true);

bool ReadRecord()
{
	LineFieldsValue.clear();

		string s;
		if(ExcelGetline(s) == false)
			return false;

		if (s.length() > 0)
		{

			LineFieldsValue = ParseLine(s);
			return true;
		}else
			return false;
		
		
		
}


	template <class T> bool GetValueBySectionKeyFieldName(string file_name, string section_name, string key_name, string field_name, T& value)
	{
		OpenFile(file_name);
		while(ReadRecord())
		{
			if(LineFieldsValue[0]!=section_name || LineFieldsValue[1] != key_name)
			continue;
	
		if (FieldsIndices.find(field_name) == FieldsIndices.end())
		{
			CloseCSVFile();
			return false;
		}
		else
		{
			if (LineFieldsValue.size() == 0)
			{
				CloseCSVFile();
				return false;
			}

			int size = (int)(LineFieldsValue.size());
			if(FieldsIndices[field_name]>= size)
			{
				CloseCSVFile();
				return false;
			}

			string str_value = LineFieldsValue[FieldsIndices[field_name]];

			if (str_value.length() <= 0)
			{
			CloseCSVFile();
				return false;
			}

			istringstream ss(str_value);

			T converted_value;
			ss >> converted_value;

			if (/*!ss.eof() || */ ss.fail())
			{

			CloseCSVFile();
			return false;
			}

			value = converted_value;
			CloseCSVFile();
			return true;
		}
		}
			CloseCSVFile();

			return false;
	}
	template <class T> bool GetValueByFieldName(string field_name, T& value)
	{
		if (FieldsIndices.find(field_name) == FieldsIndices.end())
		{
			return false;
		}
		else
		{
			if (LineFieldsValue.size() == 0)
			{
				return false;
			}

			int size = (int)(LineFieldsValue.size());
			if(FieldsIndices[field_name]>= size)
			{
				return false;
			}

			string str_value = LineFieldsValue[FieldsIndices[field_name]];

			if (str_value.length() <= 0)
			{
				return false;
			}

			istringstream ss(str_value);

			T converted_value;
			ss >> converted_value;

			if (/*!ss.eof() || */ ss.fail())
			{
				return false;
			}

			value = converted_value;
			return true;
		}
	}

	bool GetValueByFieldName(string field_name, string& value)
	{
		if (FieldsIndices.find(field_name) == FieldsIndices.end())
		{
			return false;
		}
		else
		{
			if (LineFieldsValue.size() == 0)
			{
				return false;
			}

			unsigned int index  = FieldsIndices[field_name];
			if(index >=LineFieldsValue.size())
			{
				return false;
			}
			string str_value = LineFieldsValue[index];

			if (str_value.length() <= 0)
			{
				return false;
			}

			value = str_value;
			return true;
		}
	}


BOOL ExcelGetline(string &s, string &substring, char delim)
{


	while(true)
	{
	std::string Value = GetCellValue(m_RowNo, m_ColumnNo);


	if(m_RowNo==0)
	{
	if(Value.size()==0)
	{
		m_RowNo++;
		m_ColumnNo = 0;

		return FALSE;
	}
	else
	{

		FieldsIndices[Value] = m_ColumnNo;
	}
	
	}else
	{

		if(m_ColumnNo>=FieldsIndices.size() || Value.find_first_of (Value)!= string::npos)
		{  // return a line
		m_RowNo++;
		m_ColumnNo = 0;

		return FALSE;
		}
	
	}
	s += Value;
	s += ",";
	
	m_ColumnNo++;

	}
	return TRUE;

}
bool ExcelGetline(string &s)
{
	while(true)
	{
	std::string Value = GetCellValue(m_ColumnNo, m_RowNo);

	if(m_RowNo==1)
	{
	if(Value.size()==0) // reach end the first line
	{
		m_RowNo++;
		m_ColumnNo = 1;

		return true;
	}
	else
	{
		if(m_ColumnNo==1 && Value.size () ==0)  //first column with empty string
			return false; 


		FieldsIndices[Value] = m_ColumnNo;
	}
	
	}else
	{

		if(m_ColumnNo>=FieldsIndices.size())
		{  // return a line
		m_RowNo++;
		m_ColumnNo = 1;

		return true;
		}
	
	}
	s += Value;
	s += ",";
	
	m_ColumnNo++;
	}

	return true;
}
bool OpenFile(CString fileName,CString WorksheetName, int ActiveSheet)
{
	m_RowNo = 1;
	m_ColumnNo = 1;

	if(OpenExcelFile(fileName,WorksheetName, ActiveSheet) == FALSE)
		return false;

	bool IsFirstLineHeader = true;
		if(m_bSkipFirstLine)
		{
			string s;
			ExcelGetline(s);
		}
		if (IsFirstLineHeader)
		{
			string s;
			ExcelGetline(s);

			if(s.length () ==0)
				return true; 

			vector<string> FieldNames = ParseLine(s);

			for (size_t i=0;i<FieldNames.size();i++)
			{
				string tmp_str = FieldNames.at(i);
				size_t start = tmp_str.find_first_not_of(" ");

				string name;
				if (start == string::npos)
				{
					name = "";
				}
				else
				{
					name = tmp_str.substr(start);
					TRACE("%s,",name.c_str ());
				}

				if (std::string::npos != name.find_first_of("."))
				{
				  //digit(s)found in header
					// we convert 2.0000 in header to 2 here
						
						unsigned found = name.find_last_of(".");
						std::string str = name.substr(0,found);
							name = str;
				//
				}

				Headers.push_back(name);
				FieldsIndices[name] = (int) i;
			}
		}

		return true;
	
}

void Close()
{
	ReleaseExcel();
}
bool ReadSectionHeader()
{
	//skip // data 

	Headers.clear();
	FieldsIndices.clear();

	string s;   // skip one line
	ExcelGetline(s);

	if(s.length () ==0)
		return true; 

	ExcelGetline(s);  // read the line for the header

	vector<string> FieldNames = ParseLine(s);

	for (size_t i=0;i<FieldNames.size();i++)
	{
		string tmp_str = FieldNames.at(i);
		size_t start = tmp_str.find_first_not_of(" ");

		string name;
		if (start == string::npos)
		{
			name = "";
		}
		else
		{
			name = tmp_str.substr(start);
			TRACE("%s,",name.c_str ());
		}
		Headers.push_back(name);
		FieldsIndices[name] = (int) i;
	}


	return true;

}


vector<string> GetLineRecord()
{
	return LineFieldsValue;
}

vector<string> GetHeaderList()
{
	return Headers;
}

vector<string> ParseLine(string line)
{
	vector<string> SeperatedStrings;
	string subStr;
	istringstream ss(line);


	if (line.find_first_of('"') == string::npos)
	{

		while (std::getline(ss,subStr,Delimiter))
		{
			SeperatedStrings.push_back(subStr);
		}

		if (line.at(line.length()-1) == ',')
		{
			SeperatedStrings.push_back("");
		}
	}
	else
	{
		while (line.length() > 0)
		{
			size_t n1 = line.find_first_of(',');
			size_t n2 = line.find_first_of('"');

			if (n1 == string::npos && n2 == string::npos) //last field without double quotes
			{
				subStr = line;
				SeperatedStrings.push_back(subStr);
				break;
			}

			if (n1 == string::npos && n2 != string::npos) //last field with double quotes
			{
				size_t n3 = line.find_first_of('"',n2+1); // second double quote

				//extract content from double quotes
				subStr = line.substr(n2+1, n3-n2-1);
				SeperatedStrings.push_back(subStr);

				break;
			}

			if (n1 != string::npos && (n1 < n2 || n2 == string::npos))
			{
				subStr = line.substr(0,n1);
				SeperatedStrings.push_back(subStr);
				if (n1 < line.length()-1)
				{
					line = line.substr(n1+1);
				}
				else // comma is the last char in the line string, push an empty string to the back of vector
				{
					SeperatedStrings.push_back("");
					break;
				}
			}

			if (n1 != string::npos && n2 != string::npos && n2 < n1)
			{
				size_t n3 = line.find_first_of('"',n2+1); // second double quote
				subStr = line.substr(n2+1, n3-n2-1);
				SeperatedStrings.push_back(subStr);
				size_t idx = line.find_first_of(',',n3+1);

				if (idx != string::npos)
				{
					line = line.substr(idx+1);
				}
				else
				{
					break;
				}
			}
		}

	}

	return SeperatedStrings;
}

CString GetCString(std::string ColumnName)
{

	std::string Holder;
	GetValueByFieldName(ColumnName, Holder);

   return Holder.c_str();
};

long GetLong(std::string ColumnName, bool &bExist, bool bRequired=false )
{
	long Holder = 0;
	GetValueByFieldName(ColumnName, Holder);


   return Holder;
};

double  GetDouble(std::string ColumnName, bool &bExist,bool bRequired=false)
{
	double Holder = 0;
	GetValueByFieldName(ColumnName, Holder);

	return Holder;
}
float GetFloat(std::string ColumnName)
{
	float Holder = 0;
	GetValueByFieldName(ColumnName, Holder);

}


public:


	CString m_CurrentFileName;
	BOOL InsertPictureFromBuffer(BYTE* pImage, int Column, int Row, double dXScale, double dYScale);
	BOOL OpenExcelFile(CString szFileName, CString WorkSheetName, int ActiveSheet);
	BOOL OpenExcelFile(CString szFileName, int WorkSheet);
	BOOL PlacePictureToClipboard(BYTE* pImage);
	BOOL InsertPictureFromClipboard(int Column, int Row);
	BOOL InsertPictureFromFile(CString szFileName, int Column, int Row);
	std::string GetCellValue(int nColumn, int nRow);
	BOOL SaveFileAs(CString szFileName);

	BOOL SaveFile()
	{
	return SaveFileAs(m_CurrentFileName);
	}

	BOOL DeleteRow(int nRow);
	BOOL ReleaseExcel();
	BOOL SetCellValue(int nColumn, int nRow, CString szValue);
	BOOL SetCellValue(int nColumn, int nRow, int number)
	{
	CString szValue;
	szValue.Format("%d",number);
	SetCellValue(nColumn, nRow,szValue);
	return true;
	}

	BOOL SetCellValue(int nColumn, int nRow, float number)
	{
	CString szValue;
	szValue.Format("%f",number);
	SetCellValue(nColumn, nRow,szValue);
	return true;

	}

	BOOL SetCellValue(int nColumn, int nRow, double number)
	{
	CString szValue;
	szValue.Format("%f",number);
	SetCellValue(nColumn, nRow,szValue);
	return true;

	}


	BOOL ExportCString(CString szDataCollection);
	BOOL UpdateXYChart(int nYColumn);
	BOOL CreateXYChart(int nYColumn);

	BOOL SetActiveWorksheet(int nWorksheet)
	{
	m_pXLServer->SetActiveWorksheet(nWorksheet);
	return true;
	}

	BOOL SetActiveWorksheet(CString name)
	{
	 for (int i = 1; i <= m_pXLServer->GetWorksheetsCount (); i++)
	 {
		 CString worksheet_name = m_pXLServer->GetWorksheetName(i);
		if(worksheet_name.Compare (name) ==0)
		{
			m_pXLServer->SetActiveWorksheet(i);
			return true;
		}
	 }
	 return false;
	}



	int AddNewWorkSheet()
	{
	return m_pXLServer->AddNewWorkSheet();
	}
   
	BOOL SetWorksheetTitle(CString wsTitle)
	{
		return m_pXLServer->SetWorksheetTitle(wsTitle);
	}
	CXLEzAutomation();
	CXLEzAutomation(BOOL bVisible);
	virtual ~CXLEzAutomation();

protected:
	CXLAutomation* m_pXLServer;
};

#endif // !defined(AFX_EzAutomation_H__D140B9A3_1995_40AC_8E6D_8F23A95A63A2__INCLUDED_)
