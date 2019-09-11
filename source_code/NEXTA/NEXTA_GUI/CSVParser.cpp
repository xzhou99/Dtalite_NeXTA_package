#include "StdAfx.h"
#include "CSVParser.h"

#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;


CCSVParser::CCSVParser(void)
{
	Delimiter = ',';
	IsFirstLineHeader = true;
	m_bSkipFirstLine = false;
	m_bSynchroSingleCSVFile = false;
	m_bLastSectionRead = false;
	m_EmptyLineCount++;
}

CCSVParser::~CCSVParser(void)
{
	if (inFile.is_open()) inFile.close();
}


bool CCSVParser::OpenCSVFile(string fileName, bool bIsFirstLineHeader)
{
	inFile.clear();
	inFile.open(fileName.c_str());

	IsFirstLineHeader = bIsFirstLineHeader;
	if (inFile.is_open())
	{
		if(m_bSkipFirstLine)
		{
			string s;
			std::getline(inFile,s);
		}
		if (IsFirstLineHeader)
		{
			string s;
			std::getline(inFile,s);

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
				Headers.push_back(name);
				FieldsIndices[name] = (int) i;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool CCSVParser::ReadSectionHeader()
{
	//skip // data 

	Headers.clear();
	FieldsIndices.clear();

	string s;   // skip one line
	std::getline(inFile,s);

	if(s.length () ==0)
		return true; 

	std::getline(inFile,s);  // read the line for the header

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
void CCSVParser::CloseCSVFile(void)
{
	inFile.close();
}

vector<string> CCSVParser::GetLineRecord()
{
	return LineFieldsValue;
}

vector<string> CCSVParser::GetHeaderList()
{
	return Headers;
}

bool CCSVParser::ReadRecord()
{
	LineFieldsValue.clear();

	if (inFile.is_open())
	{
		string s;
		std::getline(inFile,s);
		if (s.length() > 0)
		{
			if(m_bSynchroSingleCSVFile && s.find("[") != string::npos)  // synchro single csv file
			{
				LineFieldsValue = ParseLine(s);

				if(LineFieldsValue.size()>=1) 
				{
					m_SynchroSectionName = LineFieldsValue[0];

					if(m_SynchroSectionName.find("Phases") != string::npos)
					{
						m_bLastSectionRead  = true;
					}
				}

				//re-read section header
				ReadSectionHeader();
				std::getline(inFile,s);  // read the line for field values



			}
			LineFieldsValue = ParseLine(s);
			return true;
		}
		else
		{

			if(m_bSynchroSingleCSVFile)
			{
				if(m_bLastSectionRead)  // reach the last section
					return false;
				else
				{
					if(inFile.eof ())
						return false;
					else
						return true;
				}
			}else //non - synchro
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
}

vector<string> CCSVParser::ParseLine(string line)
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