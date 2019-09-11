// TLite.cpp : Defines the class behaviors for the application.
//
//  Portions Copyright 2010 Xuesong Zhou (xzhou99@gmail.com)

//   If you help write or modify the code, please also list your names here.
//   The reason of having Copyright info here is to ensure all the modified version, as a whole, under the GPL 
//   and further prevent a violation of the GPL.

// More about "How to use GNU licenses for your own software"
// http://www.gnu.org/licenses/gpl-howto.html


//    This file is part of NeXTA Version 3 (Open-source).

//    NEXTA is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    NEXTA is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with NEXTA.  If not, see <http://www.gnu.org/licenses/>.

#include "stdafx.h"
#include "TLite.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "TLiteDoc.h"
#include "TLiteView.h"
#include "TSView.h"
#include "DlgMOE.h"
#include "DlgPathMOE.h"
#include "DlgMainTemplate.h"
#include "Dlg_VehicleClassification.h"
#include "Dlg_Legend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CDlgMOE *g_LinkMOEDlg;
extern CDlgPathMOE	*g_pPathMOEDlg;
extern std::vector<CDlg_VehicleClassification*>	g_SummaryDialogVector;
extern CDlg_Legend* g_pLegendDlg;

// CTLiteApp

BEGIN_MESSAGE_MAP(CTLiteApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CTLiteApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, &CTLiteApp::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN_NEW_DOC, &CTLiteApp::OnFileOpenNewDoc)
	ON_COMMAND(ID_RESEARCHTOOLS_EXPORTTODTALITESENSORDATAFORMAT, &CTLiteApp::OnResearchtoolsExporttodtalitesensordataformat)
	ON_COMMAND(ID_FILE_OPENMULTIPLETRAFFICDATAPROJECTS, &CTLiteApp::OnFileOpenmultipletrafficdataprojects)
	ON_COMMAND(ID_APP_EXIT, &CTLiteApp::OnAppExit)
	ON_COMMAND(ID_FILE_OPEN_NETWORK_ONLY, &CTLiteApp::OnFileOpenNetworkOnly)
	ON_COMMAND(ID_HELP_USERGUIDE, &CTLiteApp::OnHelpUserguide)
	ON_COMMAND(ID_HELP_CHECKLATESTSOFTWARERELEASE, &CTLiteApp::OnHelpChecklatestsoftwarerelease)
END_MESSAGE_MAP()




// CTLiteApp construction



// The one and only CTLiteApp object

CTLiteApp theApp;


// CTLiteApp initialization
CTLiteApp::CTLiteApp()
{
	m_SimulatorString_32 = "DTALite_32.exe";
	m_SimulatorString_64 = "DTALite.exe";

	m_FreewayColor = RGB(030,144,255);
	m_RampColor = RGB(160,032,240); 
	m_ArterialColor = RGB(034,139,034);
	m_ConnectorColor = RGB(255,165,000);
	m_TransitColor = RGB(255,0,255);
	m_WalkingColor = RGB(127,255,0);

	m_BackgroundColor =  RGB(255,255,255);

	m_NodeColor = RGB(0,0,0);
	m_NodeBrushColor = RGB(0,0,0);

	m_ZoneColor = RGB(000,191,255);


	m_NEXTA_use_flag = 0;
	m_bLoadNetworkOnly = false;
	//m_pTemplateGLView = false;
	m_pTemplateTimeTableView = false;
	m_pDocTemplate2DView = NULL;
	m_pTemplateTimeTableView = NULL;

}
BOOL CTLiteApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	  CWinApp::InitInstance();

        // Standard initialization
        SetRegistryKey(_T("NeXTA Version 3"));
        LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

		char CurrentDirectory[MAX_PATH+1];
        GetCurrentDirectory(MAX_PATH,CurrentDirectory);

		CString NEXTASettingsPath;
		NEXTASettingsPath.Format ("%s\\NEXTA_Settings.ini", CurrentDirectory);
	
		m_NEXTA_use_flag = (int)g_GetPrivateProfileDouble("initialization", "nexta", 0, NEXTASettingsPath);
		WritePrivateProfileString("initialization", "nexta","1",NEXTASettingsPath);

		m_FreewayColor = (DWORD )g_GetPrivateProfileDouble("initialization", "FreewayColor", -1, NEXTASettingsPath);

		if(m_FreewayColor<0)
			m_FreewayColor =  RGB(030,144,255);

		m_RampColor = (DWORD )g_GetPrivateProfileDouble("initialization", "RampColor", -1, NEXTASettingsPath);

		if(m_RampColor<0)
			m_RampColor =  RGB(160,032,240); 

		double value = g_GetPrivateProfileDouble("initialization", "ArterialColor", -1, NEXTASettingsPath);

		if(value<0)
			m_ArterialColor =  RGB(160,032,240); 
		else
			m_ArterialColor = (DWORD)  value;

		value = (DWORD )g_GetPrivateProfileDouble("initialization", "ConnectorColor", -1, NEXTASettingsPath);

		if(value<0)
			m_ConnectorColor =  RGB(160,032,240); 
		else
			m_ConnectorColor = (DWORD)  value;

		value = (DWORD )g_GetPrivateProfileDouble("initialization", "TransitColor", -1, NEXTASettingsPath);

		if(value<0)
			m_TransitColor =  RGB(160,032,240); 
		else
			m_TransitColor = (DWORD)  value;

		value = (DWORD )g_GetPrivateProfileDouble("initialization", "WalkingColor", -1, NEXTASettingsPath);

		if(value<0)
			m_WalkingColor =  RGB(160,032,240); 
		else
			m_WalkingColor = (DWORD)  value;
			

		value = (DWORD )g_GetPrivateProfileDouble("initialization", "BackgroundColor", -1, NEXTASettingsPath);

		if(value<0)
			m_BackgroundColor =   RGB(255,255,255);
		else
			m_BackgroundColor = (DWORD)  value;
			

		value = (DWORD )g_GetPrivateProfileDouble("initialization", "NodeColor", -1, NEXTASettingsPath);

		if(value<0)
			m_NodeColor =   RGB(0,0,0);
		else
			m_NodeColor = (DWORD)  value;
			
		value = (DWORD )g_GetPrivateProfileDouble("initialization", "NodeBrushColor", -1, NEXTASettingsPath);

		if(value<0)
			m_NodeBrushColor =   RGB(0,0,0);
		else
			m_NodeBrushColor = (DWORD)  value;
			

   char lpbuffer[_MAX_PATH];
   if(GetPrivateProfileString("initialization", "UserDefinedSimulator_32","",lpbuffer,sizeof(lpbuffer),NEXTASettingsPath)) 
   {
		m_SimulatorString_32.Format ("%s",lpbuffer);
   }

   if(GetPrivateProfileString("initialization", "UserDefinedSimulator_64","",lpbuffer,sizeof(lpbuffer),NEXTASettingsPath)) 
   {
		m_SimulatorString_64.Format ("%s",lpbuffer);
   }

	int visualization_template = (int)g_GetPrivateProfileDouble("template", "traffic_assignment", 1, NEXTASettingsPath);

	m_VisulizationTemplate = e_traffic_assignment;

		m_LanguageSupport = (eLanguageSupport)g_GetPrivateProfileInt("template", "LanguageSupport", 0, NEXTASettingsPath);


		if( m_VisulizationTemplate == e_traffic_assignment)
		{
		if(m_LanguageSupport  ==LANG_CN_SIMPLIFIED)
		{
        m_pDocTemplate2DView = new CMultiDocTemplate(IDR_TLiteTYPE5,
                RUNTIME_CLASS(CTLiteDoc),
                RUNTIME_CLASS(CChildFrame), // custom MDI child frame
                RUNTIME_CLASS(CTLiteView));

		}else
		{



#ifndef _WIN64
		m_pDocTemplate2DView = new CMultiDocTemplate(IDR_TLiteTYPE1,
			RUNTIME_CLASS(CTLiteDoc),
			RUNTIME_CLASS(CChildFrame), // custom MDI child frame
			RUNTIME_CLASS(CTLiteView)); 
#else
			m_pDocTemplate2DView = new CMultiDocTemplate(IDR_TLiteTYPE1,
				RUNTIME_CLASS(CTLiteDoc),
				RUNTIME_CLASS(CChildFrame), // custom MDI child frame
				RUNTIME_CLASS(CTLiteView));

#endif

		
		
		}
		}

		if( m_VisulizationTemplate == e_train_scheduling)
		{
        m_pDocTemplate2DView = new CMultiDocTemplate(IDR_TLiteTYPE4,
                RUNTIME_CLASS(CTLiteDoc),
                RUNTIME_CLASS(CChildFrame), // custom MDI child frame
                RUNTIME_CLASS(CTLiteView));

		}

        if (!m_pDocTemplate2DView)
                return FALSE;
        AddDocTemplate(m_pDocTemplate2DView);

        // add second Doc template for GLView

    //The AddDocTemplate() call generated by AppWizard established the primary document/frame/view combination for the application that is effective 
        //when the program starts. 
        //The template object below is a secondary template that can be activated in response to the New GLView Window menu item.

			m_pTemplateTimeTableView = new CMultiDocTemplate(
			IDR_TLiteTYPE1,
			RUNTIME_CLASS(CTLiteDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CTimeSpaceView));

        // create main MDI Frame window
        CMainFrame* pMainFrame = new CMainFrame;
		

		if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
        {
                delete pMainFrame;
                return FALSE;
        }
        m_pMainWnd = pMainFrame;
        // call DragAcceptFiles only if there's a suffix
        //  In an MDI app, this should occur immediately after setting m_pMainWnd

        // Parse command line for standard shell commands, DDE, file open
        CCommandLineInfo cmdInfo;
        ParseCommandLine(cmdInfo);


        // Dispatch commands specified on the command line.  Will return FALSE if
        // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
        if (!ProcessShellCommand(cmdInfo))
                return FALSE;

        GetCurrentDirectory(MAX_PATH,pMainFrame->m_CurrentDirectory);
#ifndef _WIN64
	pMainFrame->SetTitle ("NeXTA Version 3 Beta (32-bit)");
#else
	pMainFrame->SetTitle ("NeXTA Version 3 Beta (64-bit)");
#endif

        // The main window has been initialized, so show and update it
        pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
        pMainFrame->UpdateWindow();

        return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CTLiteApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CTLiteApp message handlers


void CTLiteApp::OnFileOpen()
{
	static char BASED_CODE szFilter[] = "NEXTA Data Files (*.dws;*.tnp)|*.dws; *.tnp|DYNASMART Workspace Files (*.dws)|*.dws|Transportation Network Projects (*.tnp)|*.tnp|All Files (*.*)|*.*||";

	 
   CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_ENABLESIZING , szFilter,NULL, 0, true);

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CString NetworkFile = pMainFrame->m_CurrentDirectory;
	if(theApp.m_NEXTA_use_flag == 0)
	{
	dlg.m_ofn.lpstrInitialDir = NetworkFile;
	theApp.m_NEXTA_use_flag = 1;
	}else
	{// elsewise use the last used folder
		dlg.m_ofn.lpstrInitialDir = "";

	}

	

   if(dlg.DoModal() == IDOK)
   {
	  CString PathName =   dlg.GetPathName(); 
      POSITION p = m_pDocManager->GetFirstDocTemplatePosition();
      CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(p);
      CTLiteDoc* pDoc = (CTLiteDoc*)pTemplate->OpenDocumentFile(0);
	  pDoc->m_bLoadNetworkDataOnly = m_bLoadNetworkOnly;
	  pDoc->OnOpenDocument(PathName, m_bLoadNetworkOnly);
   }
}

void CTLiteApp::OnFileOpenNewDoc()
{
	m_bLoadNetworkOnly = false;
	OnFileOpen();
	//reset back
}


int CTLiteApp::ExitInstance()
{

	// Xuesong: potential memory leak
	//if(m_pDocTemplate2DView!=NULL)
	//delete m_pDocTemplate2DView;

	//if(m_pTemplateTimeTableView!=NULL)
	//delete m_pTemplateTimeTableView;
	//collect memory


	return CWinApp::ExitInstance();
}


void CTLiteApp::UpdateAllViews()
	{


	POSITION posTempl;
	POSITION posDoc;

	CMultiDocTemplate *pDocTempl;
	CDocument *pDoc;


	posTempl = GetFirstDocTemplatePosition(); 

	while(posTempl != NULL)
	{
		pDocTempl = (CMultiDocTemplate *) GetNextDocTemplate(posTempl); // first TEMPLATE
		posDoc = pDocTempl->GetFirstDocPosition();

		while(posDoc != NULL)
		{
			pDoc = pDocTempl->GetNextDoc(posDoc); 
			pDoc->UpdateAllViews (0);

		}
	} 


	if(g_LinkMOEDlg  && g_LinkMOEDlg ->GetSafeHwnd ())
	{
		g_LinkMOEDlg->Invalidate (true);
	}


	if(g_pPathMOEDlg  && g_pPathMOEDlg ->GetSafeHwnd ())
	{
		g_pPathMOEDlg->InsertPathMOEItem();
		g_pPathMOEDlg->Invalidate (true);
	}	

	if( g_pLegendDlg!=NULL && g_pLegendDlg->GetSafeHwnd()  && g_pLegendDlg->m_pDoc->m_LinkMOEMode == MOE_impact)
	{
		g_pLegendDlg->Invalidate (true);
	}	

}

void CTLiteApp::OnResearchtoolsExporttodtalitesensordataformat()
{
	// TODO: Add your command handler code here
}

void CTLiteApp::OnFileOpenmultipletrafficdataprojects()
{
   CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		   _T("Project Index File(*.pif)|*.pif|"));
   if(dlg.DoModal() == IDOK)
   {
	  /* read project index
	  // for each project
		  // project type: resolution (macro, micro, micro-scopic)
		  // with/without real-world data
	      // with/without prediction

	   different layers of data (data framework)
	  { import data to a new project
	     network
		 traffic control file
		 demand
		 scenario file (VMS, incident, ramp metering )
		 simulated data
		 observed data
			 detector sensor (multiple days)
		 	 AVI sensor (node to node, AVI)
			 corridor file (node to node file, )
		 probe sensor (local X) including NGSIM data
		 optional CSV files
		 demand
		 vehicle trajectory
	   signal data
	   }	

	   multiple views

	   1) spatial 2d network
			MOE view (color coded)
	   2) spatial 3d network
			color coded + height (MOE (e.g. emissions)
	   3) time series 
			3.1 link MOE
				(speed, travel time, density, flow, queue length)
			3.2 network MOE
			3.3 path 
				(fuel efficiency)
	   4) OD to path view
	   5) time-space view 
			data vs. simulated
			point sensor, AVI, probe, estimated
	   6) quick summary of scenaro
			impacted vehicels by scenarios
			selected OD, selected path

		AMS Tools
		   0) basic GIS viewing tools
		      view, zoom-in, zoom-out, add and delete links, input scenaro and demand data

		   1) exporting data for selected subarea
			Synchro data
			DYNASMART
			DTALite
			TRANSIMS
			CORSIM
			
		   2) on-line GIS	google fusion tables -> KML

		   3)
			generate additional MOE
				emissions
				safety
				reliability
			4) dynamic OD demand estimation

			5) generate capacity using quick analysis tables

			6) communicate with data bus (upload and download dat from central data bus, communicate with simulation clock)
			simulator warpper (call external excutable, exchange data with data bus)
				{
				DYNASMART
				DTALite
				TRANSIMS
				}
	
			7) scenario comparison 
				estimation vs. prediction
				different resolutions
				data vs. estimation results, faulty sensors, imcompartable 

	   // end of each project

	  */


      POSITION p = m_pDocManager->GetFirstDocTemplatePosition();
      CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(p);
      CTLiteDoc* pDoc = (CTLiteDoc*)pTemplate->OpenDocumentFile(0);
      pDoc->OnOpenDocument(dlg.GetPathName());
   }
}


void CTLiteApp::OnAppExit()
{

	exit(0);
}

void CTLiteApp::OnFileOpenNetworkOnly()
{
	m_bLoadNetworkOnly = true;
	OnFileOpen();
	//reset back
	m_bLoadNetworkOnly = false;}


void CTLiteApp::OnHelpUserguide()
{
	g_OpenDocument("https://docs.google.com/document/d/14tUa1I6Xf62zsiWf4lLfngqGqGJlIM_MSehLFMVXass/edit", SW_SHOW);
}

void CTLiteApp::OnHelpChecklatestsoftwarerelease()
{

	g_OpenDocument("http://code.google.com/p/nexta/downloads/list", SW_SHOW);

}

