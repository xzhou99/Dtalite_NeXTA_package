/************************************
Adpated from

How to Read and Write Excel Files in C++ via ADO
By Dmytro Skrypnyk | 10 Nov 2010

*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "adoEOF") rename("BOF", "adoBOF")

const int writeRows = 50000;

inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};

std::string makeConnStr(std::string filename, bool header = true)
{
    std::stringstream stream;

    std::string hdr = header ? "YES" : "NO";

    if(!filename.empty())
        if(*--filename.end() == 'x')
            stream << "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" << filename << ";Extended Properties=\"Excel 12.0 Xml;HDR=" << hdr << "\"";
        else
            stream << "Provider='Microsoft.JET.OLEDB.4.0';Data Source=" << filename << ";Extended Properties=\"Excel 8.0;HDR=" << hdr << "\"";

    return stream.str();
}

std::string sqlSelectSheet(_bstr_t connStr, int sheetIndex)
{
    _ConnectionPtr pCon = NULL;
    _RecordsetPtr pSchema = NULL;

    TESTHR(pCon.CreateInstance(__uuidof(Connection)));
    TESTHR(pCon->Open(connStr, "", "", NULL));

    pSchema = pCon->OpenSchema(adSchemaTables);

    for(int i = 0; i < sheetIndex; ++i) pSchema->MoveNext();

    std::string sheetName = (char*)(_bstr_t)pSchema->Fields->GetItem("TABLE_NAME")->Value.bstrVal;

    std::stringstream stream;
    stream << "SELECT * FROM [" << sheetName << "]";
    return stream.str();
}


void read(std::string excelFile, int sheetIndex, bool header, std::string csvFile)
{
    clock_t t1 = clock();

    std::cout << "reading " << excelFile;

    if(FAILED(::CoInitialize(NULL))) return;

    _RecordsetPtr pSchema = NULL;
    _RecordsetPtr pRec = NULL;

    int cellCount = 0;

    try
    {
        _bstr_t connStr(makeConnStr(excelFile, header).c_str());

        TESTHR(pRec.CreateInstance(__uuidof(Recordset)));
        TESTHR(pRec->Open(sqlSelectSheet(connStr, sheetIndex).c_str(), connStr, adOpenStatic, adLockOptimistic, adCmdText));

        std::ofstream stream(csvFile.c_str());

        while(!pRec->adoEOF)
        {
            for(long i = 0; i < pRec->Fields->GetCount(); ++i)
            {
                if(i > 0) stream << ";";
                _variant_t v = pRec->Fields->GetItem(i)->Value;
                if(v.vt == VT_R8)
                    stream << v.dblVal;
                if(v.vt == VT_BSTR)
                    stream << (char*)(_bstr_t)v.bstrVal;
                ++cellCount;
            }
            stream << std::endl;
            pRec->MoveNext();
        }
    }
    catch(_com_error &e)
    {
        _bstr_t bstrDescription(e.Description());
        CharToOem(bstrDescription, bstrDescription);
        std::cout << bstrDescription << std::endl;
    }

    ::CoUninitialize();

    clock_t t2 = clock();
    double t = (double)(t2 - t1) / CLOCKS_PER_SEC;
    std::cout << ": " << t << " sec; " << cellCount / t << " cells/sec" << "; see " << csvFile << std::endl;
}

void write(std::string filename)
{
    clock_t t1 = clock();

    std::cout << "writing " << filename;

    DeleteFileA(filename.c_str());

    if(FAILED(::CoInitialize(NULL))) return;

    _ConnectionPtr pCon = NULL;
    _CommandPtr pCmd = NULL;
    _RecordsetPtr pRec = NULL;

    try
    {
        _bstr_t connStr(makeConnStr(filename).c_str());

        TESTHR(pCon.CreateInstance(__uuidof(Connection)));
        TESTHR(pCon->Open(connStr, "", "", NULL));

        TESTHR(pCmd.CreateInstance(__uuidof(Command)));
        pCmd->ActiveConnection = pCon;
        pCmd->CommandText = "CREATE TABLE MySheet(A int, B varchar, C int, D int, E int, F int, G int, H int, I int, J varchar)";
        pCmd->Execute(NULL, NULL, adCmdText);

        TESTHR(pRec.CreateInstance(__uuidof(Recordset)));
        pRec->Open("SELECT * FROM MySheet", _variant_t((IDispatch*)pCon), adOpenKeyset, adLockOptimistic, adCmdText);

        for(int i = 0; i < writeRows; ++i)
        {
            TESTHR(pRec->AddNew());

            char str[11] = {0}; for(int j = 0; j < 10; ++j) str[j] = 'a' + (rand() % 26);

            pRec->Fields->GetItem("A")->Value = _variant_t(i);
            pRec->Fields->GetItem("B")->Value = _variant_t(str);
            pRec->Fields->GetItem("C")->Value = _variant_t(i);
            pRec->Fields->GetItem("D")->Value = _variant_t(i);
            pRec->Fields->GetItem("E")->Value = _variant_t(i);
            pRec->Fields->GetItem("F")->Value = _variant_t(i);
            pRec->Fields->GetItem("G")->Value = _variant_t(i);
            pRec->Fields->GetItem("H")->Value = _variant_t(i);
            pRec->Fields->GetItem("I")->Value = _variant_t(i);
            pRec->Fields->GetItem("J")->Value = _variant_t(str);
        }
        TESTHR(pRec->Update());
        TESTHR(pRec->Close());
    }
    catch(_com_error &e)
    {
        _bstr_t bstrDescription(e.Description());
        CharToOem(bstrDescription, bstrDescription);
        std::cout << bstrDescription << std::endl;
    }

    if(pCon != 0 && pCon->State == adStateOpen) pCon->Close();
    ::CoUninitialize();

    clock_t t2 = clock();
    double t = (double)(t2 - t1) / CLOCKS_PER_SEC;
    std::wcout << ": " << t << " sec; " << writeRows * 10 / t << " cells/sec" << std::endl;
}

