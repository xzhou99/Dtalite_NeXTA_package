// Dlg_GoogleFusionTable.cpp : implementation file
//

#include "stdafx.h"
#include <windows.h>

#include "TLite.h"
#include "MainFrm.h"

#include "Dlg_GoogleFusionTable.h"
#include "CSVParser.h"

#include "Strsafe.h"

#include <string>
#include <fstream>

using std::string;
using std::ifstream;


// CDlg_GoogleFusionTable dialog

IMPLEMENT_DYNAMIC(CDlg_GoogleFusionTable, CDialog)

CDlg_GoogleFusionTable::CDlg_GoogleFusionTable(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_GoogleFusionTable::IDD, pParent)
	, m_ProjectFolder(_T(""))
	, m_NodeTableID(_T(""))
	, m_LinkTableID(_T(""))
{
	m_NodeTableID = _T("2299886");
	m_LinkTableID = _T("2410212");

	m_UploadNodeTableID = _T("2310833");
	m_UploadLinkTableID = _T("2409893");
	m_SubareaLinkSize = 0;

}

CDlg_GoogleFusionTable::~CDlg_GoogleFusionTable()
{
}

void CDlg_GoogleFusionTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ExcelFile5, m_ProjectFolder);
	DDX_Text(pDX, IDC_EDIT_ExcelFile, m_NodeTableID);
	DDX_Text(pDX, IDC_EDIT_ExcelFile2, m_LinkTableID);
	DDX_Text(pDX, IDC_SUBAREA_LINK_SIZE, m_SubareaLinkSize);
}


BEGIN_MESSAGE_MAP(CDlg_GoogleFusionTable, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_Download, &CDlg_GoogleFusionTable::OnBnClickedButtonDownload)
	ON_BN_CLICKED(IDC_BUTTON_Upload, &CDlg_GoogleFusionTable::OnBnClickedButtonUpload)
	ON_EN_CHANGE(IDC_EDIT_ExcelFile5, &CDlg_GoogleFusionTable::OnEnChangeEditExcelfile5)
	ON_BN_CLICKED(IDC_BUTTON_LINK_WEB, &CDlg_GoogleFusionTable::OnBnClickedButtonLinkWeb)
	ON_BN_CLICKED(IDC_BUTTON_LINK_WEB_Subarea, &CDlg_GoogleFusionTable::OnBnClickedButtonLinkWebSubarea)
	ON_EN_CHANGE(IDC_EDIT_ExcelFile, &CDlg_GoogleFusionTable::OnEnChangeEditExcelfile)
	ON_EN_CHANGE(IDC_EDIT_ExcelFile2, &CDlg_GoogleFusionTable::OnEnChangeEditExcelfile2)
END_MESSAGE_MAP()


// CDlg_GoogleFusionTable message handlers

BOOL CDlg_GoogleFusionTable::OnInitDialog()
{
	if(m_pDoc->m_ProjectDirectory.GetLength () == 0)
	{
			CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

			CString Sample_Google_Fusion_Tables_Project = "\\Sample_Google_Fusion_Tables_Project\\";

			m_pDoc->m_ProjectDirectory = pMainFrame->m_CurrentDirectory + Sample_Google_Fusion_Tables_Project;
	}
	
	m_ProjectFolder = m_pDoc->m_ProjectDirectory;
	m_SubareaLinkSize = m_pDoc->m_SubareaLinkSet .size();

	UpdateData(false);
	return true;
}

string GetGoogleAuthorizationCode(string fileName)
{
	ifstream file(fileName.c_str());

	string s;
	string AuthCode;

	while (getline(file,s))
	{
		if (s.find("Auth=") != string::npos)
		{
			AuthCode = s.substr(string("Auth=").length());
			break;
		}
	}

	return AuthCode;
}

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

void ErrorExit(PTSTR lpszFunction) 

// Format a readable error message, display a message box, 
// and exit from the application.
{ 
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(1);
}

void CreateChildProcess(TCHAR* cmd);

string RunQuery(TCHAR* cmd)
{
	string ret = "";

	SECURITY_ATTRIBUTES saAttr; 
 
// Set the bInheritHandle flag so pipe handles are inherited. 
 
   saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
   saAttr.bInheritHandle = TRUE; 
   saAttr.lpSecurityDescriptor = NULL; 

// Create a pipe for the child process's STDOUT. 
 
   if ( ! CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0) ) 
      ErrorExit(TEXT("StdoutRd CreatePipe")); 

// Ensure the read handle to the pipe for STDOUT is not inherited.

   if ( ! SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) )
      ErrorExit(TEXT("Stdout SetHandleInformation")); 

// Create a pipe for the child process's STDIN. 
 
   if (! CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) 
      ErrorExit(TEXT("Stdin CreatePipe")); 

// Ensure the write handle to the pipe for STDIN is not inherited. 
 
   if ( ! SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
      ErrorExit(TEXT("Stdin SetHandleInformation")); 


   CreateChildProcess(cmd);

   DWORD dwRead; 
   CHAR chBuf[4096]; 
   BOOL bSuccess = FALSE;
   HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

// Close the write end of the pipe before reading from the 
// read end of the pipe, to control child process execution.
// The pipe is assumed to have enough buffer space to hold the
// data the child process has already written to it.
 
   if (!CloseHandle(g_hChildStd_OUT_Wr)) 
      ErrorExit(TEXT("StdOutWr CloseHandle")); 
 
   for (;;) 
   { 
      bSuccess = ReadFile( g_hChildStd_OUT_Rd, chBuf, 4095, &dwRead, NULL);
      if( ! bSuccess || dwRead == 0 ) break; 

	  chBuf[dwRead] = 0;
	  ret += chBuf;

	  //MessageBoxA(NULL,chBuf,NULL,MB_OK);

      /*bSuccess = WriteFile(hParentStdOut, chBuf, 
                           dwRead, &dwWritten, NULL);
      if (! bSuccess ) break; */
   } 

   return ret;
}

void CreateChildProcess(TCHAR* cmd)
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{ 
   TCHAR* szCmdline=cmd;
   PROCESS_INFORMATION piProcInfo; 
   STARTUPINFO siStartInfo;
   BOOL bSuccess = FALSE; 
 
// Set up members of the PROCESS_INFORMATION structure. 
 
   ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
 
// Set up members of the STARTUPINFO structure. 
// This structure specifies the STDIN and STDOUT handles for redirection.
 
   ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
   siStartInfo.cb = sizeof(STARTUPINFO); 
   siStartInfo.hStdError = g_hChildStd_OUT_Wr;
   siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
   siStartInfo.hStdInput = g_hChildStd_IN_Rd;
   siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
 
// Create the child process. 
    
   bSuccess = CreateProcess(NULL, 
      szCmdline,     // command line 
      NULL,          // process security attributes 
      NULL,          // primary thread security attributes 
      TRUE,          // handles are inherited 
      0,             // creation flags 
      NULL,          // use parent's environment 
      NULL,          // use parent's current directory 
      &siStartInfo,  // STARTUPINFO pointer 
      &piProcInfo);  // receives PROCESS_INFORMATION 
   
   // If an error occurs, exit the application. 
   if ( ! bSuccess ) 
      ErrorExit(TEXT("CreateProcess"));
   else 
   {
      // Close handles to the child process and its primary thread.
      // Some applications might keep these handles to monitor the status
      // of the child process, for example. 

      CloseHandle(piProcInfo.hProcess);
      CloseHandle(piProcInfo.hThread);
   }
}

void CDlg_GoogleFusionTable::OnBnClickedButtonDownload()
{


	UpdateData(true);

	// m_NodeTableID //m_LinkTableID

	STARTUPINFO si = { 0 };  
	PROCESS_INFORMATION pi = { 0 };  

	si.cb = sizeof(si); 

	CWaitCursor curs;
	if(m_pDoc->m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project directory has not been specified.");
		return;
	}

	// clean the exiting data
	m_pDoc->m_NodeSet .clear();
	m_pDoc->m_LinkSet .clear();


	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	CString sCommand;
	CString strParam;
	CTime ExeStartTime = CTime::GetCurrentTime();

//	sCommand.Format("%s\\curl.exe - transfer a URL",pMainFrame->m_CurrentDirectory, m_pDoc->m_ProjectDirectory);


	string s = (string(pMainFrame->m_CurrentDirectory)) + "\\curl.exe -s -d Email=fusiontabletestutah@gmail.com -d Passwd=utah123456 -d service=fusiontables -k \"https://www.google.com/accounts/ClientLogin\"";
	TCHAR cmd[1024];
	strcpy(cmd,s.c_str());
	string query_result = RunQuery(cmd);

	string tmp = query_result.substr(query_result.find("Auth="));
	if(tmp.length ()  == 0)
		return;

	string AuthCode = tmp.substr(0,tmp.length()-1);
	
	//int ret = m_pDoc->ProcessExecute(sCommand, strParam, m_pDoc->m_ProjectDirectory, true);

	//sCommand.Format("curl.exe \"http://www.google.com/fusiontables/api/query?sql=SELECT+*+FROM+%s\" >input_link_1.csv",this->m_LinkTableID);
	//sCommand.Format("curl.exe -L -k -s -H \"Authorization: GoogleLogin auth=%s\" --data-urlencode sql=\"SELECT * FROM %s\" https://www.google.com/fusiontables/api/query  > input_link_2.csv",AuthCode.c_str(),m_LinkTableID);

	
	s = (string(pMainFrame->m_CurrentDirectory)) 
		+ "\\curl.exe -L -s -k -H \"Authorization: GoogleLogin " 
		+ AuthCode + "\" --data-urlencode sql=\"SELECT * FROM " 
		+ m_LinkTableID.GetBuffer(0) 
		+ "\" \"https://www.google.com/fusiontables/api/query\"";

	//ofstream out1((string(m_pDoc->m_ProjectDirectory.GetBuffer(0)) + "\\out.txt").c_str());
	//out1 << s << std::endl;
	//out1.close();

	strcpy(cmd,s.c_str());
	query_result = RunQuery(cmd);


	ofstream out1((string(m_pDoc->m_ProjectDirectory.GetBuffer(0)) + "\\input_link.csv").c_str());
	out1 << query_result << std::endl;
	out1.close();


	s = (string(pMainFrame->m_CurrentDirectory)) 
		+ "\\curl.exe -L -s -k -H \"Authorization: GoogleLogin " 
		+ AuthCode + "\" --data-urlencode sql=\"SELECT * FROM " 
		+ m_NodeTableID.GetBuffer(0) + "\" \"https://www.google.com/fusiontables/api/query\"";

	strcpy(cmd,s.c_str());
	query_result = RunQuery(cmd);

	ofstream out2((string(m_pDoc->m_ProjectDirectory.GetBuffer(0)) + "\\input_node.csv").c_str());
	out2 << query_result << std::endl;
	out2.close();

	CWaitCursor wait;

	if(m_pDoc->m_ProjectFile.GetLength () ==0)
	{
		m_pDoc->m_ProjectFile.Format ("%s\\temp.ini",pMainFrame->m_CurrentDirectory );  // create temp project
	}

	m_pDoc->OnOpenTrafficNetworkDocument (m_pDoc->m_ProjectFile, true );
	m_pDoc->UpdateAllViews (0);
}

BOOL GenerateInsertStrings(string fileName, std::vector<std::vector<std::string>>& value_vector, int record_index)
{
	std::vector<std::string> header;

	CCSVParser csvParser;
	csvParser.OpenCSVFile(fileName);

	csvParser.GetHeaderList();

	int n=0;
	while(csvParser.ReadRecord() && n >= record_index && n< (record_index+100))
	{
		value_vector.push_back(csvParser.GetLineRecord());
		n++;
	}

	csvParser.CloseCSVFile();

	return TRUE;
}

void CDlg_GoogleFusionTable::OnBnClickedButtonUpload()
{
// http://skipperkongen.dk/2011/01/05/google-fusion-tables-cheat-sheet/
	// TODO: Add your control notification handler code here
	// read local sql files
	// login with default gmail account
	// insert into the specified table id

	if(m_SubareaLinkSize == 0)
	{
	AfxMessageBox("Please first select a subarea to be uploaded to the cloud computing server.", MB_ICONINFORMATION);
	
	}

	CWaitCursor wait;

	m_pDoc->WriteSubareaFiles();
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	string s = (string(pMainFrame->m_CurrentDirectory)) + "\\curl.exe -s -d Email=fusiontabletestutah@gmail.com -d Passwd=utah123456 -d service=fusiontables -k \"https://www.google.com/accounts/ClientLogin\"";
	TCHAR* cmd = new TCHAR[1024*40];
	if (cmd == NULL)
	{
		return;
	}

	strcpy(cmd,s.c_str());
	string query_result = RunQuery(cmd);

	string tmp = query_result.substr(query_result.find("Auth="));
	//Remove the last LF char
	string AuthCode = tmp.substr(0,tmp.length()-1);

	std::vector<std::vector<std::string>> value_vector;

	//Upload Link
	s = (string(pMainFrame->m_CurrentDirectory)) 
		+ "\\curl.exe -L -s -k -H \"Authorization: GoogleLogin " 
		+ AuthCode + "\" --data-urlencode sql=\"DELETE FROM " 
		+ m_UploadLinkTableID.GetBuffer(0) + "\" \"https://www.google.com/fusiontables/api/query\"";
	strcpy(cmd,s.c_str());
	query_result = RunQuery(cmd);

	value_vector.clear();

	int record;
	for( record = 0; record< m_pDoc->m_LinkSet.size(); record+=100)
	{
	GenerateInsertStrings(string(m_pDoc->m_ProjectDirectory.GetBuffer(0)) + "\\optional_subarea_link.csv",value_vector,record);
	size_t n = 0;
	string sql_str = "";
	while(n < value_vector.size())
	{
		s = (string(pMainFrame->m_CurrentDirectory)) + "\\curl.exe -L -s -k -H \"Authorization: GoogleLogin " + AuthCode + "\" --data-urlencode sql=\"";
		for (;n<value_vector.size();n++)
		{
			sql_str += string("INSERT INTO ") + m_UploadLinkTableID.GetBuffer(0) 
				+ " (congestion_index,link_id,from_node_id,to_node_id,direction,length_in_mile,"
				+ "number_of_lanes,speed_limit_in_mph,capacity_in_vhc_per_hour_per_lane,link_type,"
				+ "jam_density_in_vhc_pmpl,wave_speed_in_mph,mode_code,grade,geometry)" 
				+ "VALUES (";
			for (size_t m=0;m<value_vector.at(n).size();m++)
			{
				string str = value_vector.at(n).at(m);
				if (m == 0)
				{
					sql_str += "'" + str + "',";
					continue;
				}

				if (m == value_vector.at(n).size() - 3)
				{
					sql_str += "'" + str + "',";
					continue;
				}

				if (m == value_vector.at(n).size() - 1)
				{
					sql_str += "'" + str + "'";
					continue;
				}

				sql_str += str + ",";
			}
			sql_str += ");";

			if (s.length() + sql_str.length() < 32000)
			{
				s += sql_str;
				sql_str = "";
			}
			else
			{
				n++;
				break;
			}
		}

		s += "\" \"https://www.google.com/fusiontables/api/query\"";

		strcpy(cmd,s.c_str());
		query_result = RunQuery(cmd);
		Sleep(5000);
		
	}
	
	if(record >=0)
		break;

	}


	//Upload Node
	s = (string(pMainFrame->m_CurrentDirectory)) 
		+ "\\curl.exe -L -s -k -H \"Authorization: GoogleLogin " 
		+ AuthCode + "\" --data-urlencode sql=\"DELETE FROM " 
		+ m_UploadNodeTableID.GetBuffer(0) 
		+ "\" \"https://www.google.com/fusiontables/api/query\"";

	strcpy(cmd,s.c_str());
	query_result = RunQuery(cmd);

	value_vector.clear();

	for( record = 0; record< m_pDoc->m_NodeSet  .size(); record+=100)
	{
	GenerateInsertStrings(string(m_pDoc->m_ProjectDirectory.GetBuffer(0)) + "\\" + "optional_subarea_node.csv",value_vector,record);
	int n = 0;
	string sql_str="";

	while(n < value_vector.size())
	{
		s = (string(pMainFrame->m_CurrentDirectory)) + "\\" + "curl.exe -L -s -k -H \"Authorization: GoogleLogin " + AuthCode + "\" --data-urlencode sql=\"";
		for (;n<value_vector.size();n++)
		{
			sql_str += string("INSERT INTO ") 
				+ m_UploadNodeTableID.GetBuffer(0) 
				+ " (name,node_id,control_type,geometry) VALUES (";
			for (size_t m=0;m<value_vector.at(n).size();m++)
			{
				string str = value_vector.at(n).at(m);
				if (m == 0)
				{
					sql_str += "'" + str + "',";
					continue;
				}

				if (m == value_vector.at(n).size() - 1)
				{
					sql_str += "'" + str + "'";
					continue;
				}

				sql_str += str + ",";
			}
			sql_str += ");";

			if (s.length() + sql_str.length() < 32000)
			{
				s += sql_str;
				sql_str = "";
			}
			else
			{
				n++;
				break;
			}
		}

		s += "\" \"https://www.google.com/fusiontables/api/query\"";

		strcpy(cmd,s.c_str());
		query_result = RunQuery(cmd);
		Sleep(5000);
	}

	if(record >=0)
		break;
	}
	delete [] cmd;
}

void CDlg_GoogleFusionTable::OnEnChangeEditExcelfile5()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CDlg_GoogleFusionTable::OnBnClickedButtonLinkWeb()
{
	CString on_line_address;
	on_line_address.Format ("http://www.google.com/fusiontables/DataSource?dsrcid=%s",m_LinkTableID);

   HINSTANCE result = ShellExecute(NULL, _T("open"), on_line_address, NULL,NULL, SW_SHOW);;
}

void CDlg_GoogleFusionTable::OnBnClickedButtonLinkWebSubarea()
{
	CString on_line_address;
	on_line_address.Format ("http://www.google.com/fusiontables/DataSource?dsrcid=%s",m_UploadLinkTableID);

   HINSTANCE result = ShellExecute(NULL, _T("open"), on_line_address, NULL,NULL, SW_SHOW);;
}

void CDlg_GoogleFusionTable::OnEnChangeEditExcelfile()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CDlg_GoogleFusionTable::OnEnChangeEditExcelfile2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
