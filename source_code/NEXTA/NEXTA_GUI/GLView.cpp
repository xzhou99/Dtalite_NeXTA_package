// GLView.cpp : implementation file
// Original author: Wei Chen, cw7788@gmail.com
// Enhanced by Xuesong Zhou (xzhou99@gmail.com)

#include "stdafx.h"
#include "TLite.h"
#include "TLiteDoc.h"
#include "GLView.h"
#include "Network.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLView
extern float g_Simulation_Time_Stamp;
extern CString g_time_to_string(long timestamp);
std::list<CGLView*>	g_GLViewList;


IMPLEMENT_DYNCREATE(CGLView, CView)

CGLView::CGLView()
{
	g_GLViewList.push_back(this);
	m_WindowText = "3D";

	m_bAnimation = false;

	m_bLockZ=false;
	m_bShowGrass=true;
	m_bFollowCar=false;
	m_pDC = NULL;
	m_ObservePoint.x=0.0;
	m_ObservePoint.y=0.0;
	m_ObservePoint.z=0.0;
	m_Normal.x=0.0;
	m_Normal.y=1.0;
	m_Normal.z=0.0;
	m_zAng=2.809;//3.1415926/8.0;
	m_xyAng=PI/2.0;;
	m_EyeLength=1000.0;
	m_State=glNone;
	m_bShowBackgroundImage = true;
	m_bLoadBackgroundImage = false;
	m_ShowSpeedVariability = true;
	m_ShowAllPaths = false;
	m_ShowAllTrains = true;

	m_BackgroundMapHeight = -5.0;
	m_BackgroundMapHeight_UpperLayer = 300.0;
	m_BackgroundMapHeight_MedLayer = 150.0;
	m_bShowDualLayer = false;
	m_bShowSecurityCube = false;
	m_PredictionHorizon  = 120;

}

CGLView::~CGLView()
{
}


BEGIN_MESSAGE_MAP(CGLView, CView)
	//{{AFX_MSG_MAP(CGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_3DZoom, On3DZoom)
	ON_UPDATE_COMMAND_UI(ID_3DZoom, OnUpdate3DZoom)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_3DPam, On3DPam)
	ON_UPDATE_COMMAND_UI(ID_3DPam, OnUpdate3DPam)
	ON_COMMAND(ID_3DRotate, On3DRotate)
	ON_UPDATE_COMMAND_UI(ID_3DRotate, OnUpdate3DRotate)
	ON_COMMAND(ID_3DLockZ, On3DLockZ)
	ON_UPDATE_COMMAND_UI(ID_3DLockZ, OnUpdate3DLockZ)
	ON_COMMAND(ID_3DReset, On3DReset)
	ON_UPDATE_COMMAND_UI(ID_3DReset, OnUpdate3DReset)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_3DDISPLAY_TRAVELTIMEVARIABILITY, &CGLView::On3ddisplayTraveltimevariability)
	ON_COMMAND(ID_3DDISPLAY_TIME, &CGLView::On3ddisplayTimeDependentPaths)
	ON_COMMAND(ID_3DDISPLAY_ANIMATION, &CGLView::On3ddisplayAnimation)
	ON_UPDATE_COMMAND_UI(ID_3DDISPLAY_TRAVELTIMEVARIABILITY, &CGLView::OnUpdate3ddisplayTraveltimevariability)
	ON_UPDATE_COMMAND_UI(ID_3DDISPLAY_TIME, &CGLView::OnUpdate3ddisplayTimeDependentPaths)
	ON_UPDATE_COMMAND_UI(ID_3DDISPLAY_ANIMATION, &CGLView::OnUpdate3ddisplayAnimation)
	ON_COMMAND(ID_3DDISPLAY_BASICVIEW, &CGLView::On3ddisplayBasicview)
	ON_WM_CLOSE()
	ON_COMMAND(ID_3D_CARFOL, &CGLView::On3dCarfol)
	ON_UPDATE_COMMAND_UI(ID_3D_CARFOL, &CGLView::OnUpdate3dCarfol)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLView drawing

void CGLView::OnDraw(CDC* pDC)
{
	Render();

}

/////////////////////////////////////////////////////////////////////////////
// CGLView diagnostics

#ifdef _DEBUG
void CGLView::AssertValid() const
{
	CView::AssertValid();
}

void CGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGLView message handlers

int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	int         n;
	HGLRC       hrc;
	GLfloat      fAspect;
	GLfloat     fNearPlane, fFarPlane;

	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);

	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |          // support OpenGL
		PFD_DOUBLEBUFFER|PFD_GENERIC_ACCELERATED|PFD_SWAP_LAYER_BUFFERS ,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		MessageBox("ChoosePixelFormat failed");
		return -1;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return -1;
	}


	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);


	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);

	fAspect = 1.0f;
	fNearPlane = 1.0f;
	fFarPlane = 7000.0f;

	glViewport(0 , 0 , 1024, 768);   //default View Port
	m_ViewPortCX = 1024;
	m_ViewPortCY = 768;


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, fAspect, fNearPlane, fFarPlane);
	glMatrixMode(GL_MODELVIEW);	
	return 0;
}

void CGLView::OnDestroy() 
{
	HGLRC   hrc;


	hrc = ::wglGetCurrentContext();

	::wglMakeCurrent(NULL,  NULL);

	if (hrc)
		::wglDeleteContext(hrc);


	if (m_pDC)
		CView::OnDestroy();	
}

void CGLView::OnSize(UINT nType, int cx, int cy) 
{

	CView::OnSize(nType, cx, cy);

	if(cy > 0)
	{
		glViewport(0, 0, cx, cy);

		m_ViewPortCX = cx;
		m_ViewPortCY = cy;


		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLdouble)cx/cy, 1.0f, 7000.0f);
		glMatrixMode(GL_MODELVIEW);
	}	

}

BOOL CGLView::OnEraseBkgnd(CDC* pDC) 
{
	return true;
}

void CGLView::Render()
{
    SetWindowText(m_WindowText);

	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_ACCUM_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//////////////////////
	ComputeEyePosition();
	glLoadIdentity();

	double Length;
	Length=30.0;
	doublePointStruct EyePosition;

	// fly over animation


	//
	if(m_bFollowCar)
	{
	double Length;
	Length=100.0;

		// m_CarPosition has been pre-set in vehicle drawing

		//EyePosition.z=car.m_Position.z;//10.0;
//		m_EyePoint.x = m_CarPosition.x+Length*cos(m_EyeAng);
//		m_EyePoint.y = m_CarPosition.y+Length*sin(m_EyeAng);

		// keep the z position

		gluLookAt(m_EyePoint.x,m_EyePoint.y,m_EyePoint.z,m_CarPosition.x,m_CarPosition.y,m_CarPosition.z+10.0,0.0,0.0,1.0);
	}
	else
		gluLookAt(m_EyePoint.x,m_EyePoint.y,m_EyePoint.z,m_ObservePoint.x,m_ObservePoint.y,m_ObservePoint.z,0.0,0.0,1.0);
	glColor3d(0.6,0.6,0.6);

	float Height0 = 0.0f;

	CTLiteDoc* pDoc = GetDocument();

	if(m_bShowBackgroundImage && pDoc->m_BackgroundBitmapLoaded) //  // 2D background map is loaded
	{	
		if(m_bLoadBackgroundImage==false) // and 3D map is not loaded yet
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT,1);
			m_pBackgroundImage = auxDIBImageLoad(pDoc->m_ProjectDirectory+"background.bmp");
			glPixelStorei(GL_UNPACK_ALIGNMENT,1);
			m_bLoadBackgroundImage = true;
		}

		if(m_pBackgroundImage!=NULL)
		{
			glTexImage2D(GL_TEXTURE_2D,0,3,m_pBackgroundImage->sizeX,m_pBackgroundImage->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,m_pBackgroundImage->data );
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//  GL_CLAMP); 
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);////GL_CLAMP); 
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST); // GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST); // GL_LINEAR);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); 
			glEnable(GL_TEXTURE_2D);

// lower layer
			Vector vector;
			glColor3d(0.0,0.5,0.0);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0,0.0);  //2D binding
			vector.v.x=NXtoSX_org(pDoc->m_ImageX1);
			vector.v.y=NYtoSY_org(pDoc->m_ImageY2);
			vector.v.z=m_BackgroundMapHeight;
			glVertex3dv(vector.d);
			glTexCoord2d(1.0,0.0); //2D binding
			vector.v.x=NXtoSX_org(pDoc->m_ImageX2);
			vector.v.y=NYtoSY_org(pDoc->m_ImageY2);
			vector.v.z=m_BackgroundMapHeight;
			glVertex3dv(vector.d); 
			glTexCoord2d(1.0,1.0); //2D binding
			vector.v.x=NXtoSX_org(pDoc->m_ImageX2);
			vector.v.y=NYtoSY_org(pDoc->m_ImageY1);
			vector.v.z=m_BackgroundMapHeight;
			glVertex3dv(vector.d);
			glTexCoord2d(0.0,1.0); //2D binding
			vector.v.x=NXtoSX_org(pDoc->m_ImageX1);
			vector.v.y=NYtoSY_org(pDoc->m_ImageY1);
			vector.v.z=m_BackgroundMapHeight;
			glVertex3dv(vector.d);
			glEnd();

			if(m_bShowDualLayer)
			{
/// upper layer

			glBegin(GL_QUADS);
			glTexCoord2d(0.0,0.0);  //2D binding
			vector.v.x=NXtoSX_org(pDoc->m_ImageX1);
			vector.v.y=NYtoSY_org(pDoc->m_ImageY2);
			vector.v.z=m_BackgroundMapHeight_UpperLayer;
			glVertex3dv(vector.d);
			glTexCoord2d(1.0,0.0); //2D binding
			vector.v.x=NXtoSX_org(pDoc->m_ImageX2);
			vector.v.y=NYtoSY_org(pDoc->m_ImageY2);
			vector.v.z=m_BackgroundMapHeight_UpperLayer;
			glVertex3dv(vector.d); 
			glTexCoord2d(1.0,1.0); //2D binding
			vector.v.x=NXtoSX_org(pDoc->m_ImageX2);
			vector.v.y=NYtoSY_org(pDoc->m_ImageY1);
			vector.v.z=m_BackgroundMapHeight_MedLayer;
			glVertex3dv(vector.d);
			glTexCoord2d(0.0,1.0); //2D binding
			vector.v.x=NXtoSX_org(pDoc->m_ImageX1);
			vector.v.y=NYtoSY_org(pDoc->m_ImageY1);
			vector.v.z=m_BackgroundMapHeight_MedLayer;
			glVertex3dv(vector.d);
			glEnd();

			}
		}
		glDisable(GL_TEXTURE_2D);

	}
	CRect ScreenRect;
	GetClientRect(ScreenRect);

	float res_wid = ScreenRect.Width()/(pDoc->m_NetworkRect.Width()+1);
	float res_height = ScreenRect.Height()/(pDoc->m_NetworkRect.Height()+1);
	m_XYResolution = min(res_wid, res_height); 
	m_MinX = pDoc->m_NetworkRect.left;
	m_MinY = pDoc->m_NetworkRect.top;
	m_OrgX = (pDoc->m_NetworkRect.left+pDoc->m_NetworkRect.right)/2;
	m_OrgY = (pDoc->m_NetworkRect.top+pDoc->m_NetworkRect.bottom)/2;



	DrawAllObjects();


	/* test code
	double x0 =  NXtoSX_org(pDoc->m_NetworkRect.left);
	double x1 =  NXtoSX_org(pDoc->m_NetworkRect.right);
	double y0 =  NXtoSX_org(pDoc->m_NetworkRect.top);
	double y1 =  NXtoSX_org(pDoc->m_NetworkRect.bottom);
	glBegin(GL_QUADS);
	glColor3f(0.9f,0.9f,0.9f);
	glVertex3f(x0,y0,Height0);
	glVertex3f(x0,y1,Height0);
	glVertex3f(x1,y1,Height0);
	glVertex3f(x1,y0,Height0);
	glEnd();

	*/


	if(m_bShowGrass){

		Vector vector;
		glColor3d(0.0,0.5,0.0);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0,0.0);
		vector.v.x=-10000.0;
		vector.v.y=-10000.0;
		vector.v.z=-20.0;
		glVertex3dv(vector.d);
		glTexCoord2d(90000.0,0.0);
		vector.v.x=100000.0;
		vector.v.y=-10000.0;
		vector.v.z=-20.0;
		glVertex3dv(vector.d);
		glTexCoord2d(90000.0,900000.0);
		vector.v.x=1000000.0;
		vector.v.y=1000000.0;
		vector.v.z=-20.0;
		glVertex3dv(vector.d);
		glTexCoord2d(0.0,90000.0);
		vector.v.x=-10000.0;
		vector.v.y=1000000.0;
		vector.v.z=-20.0;
		glVertex3dv(vector.d);
		glEnd();
	}

	glFinish();
	SwapBuffers(wglGetCurrentDC());	
}

void CGLView::DrawAllObjects()
{
	//draw background links
	double Height0 = m_BackgroundMapHeight+2.0;

	CTLiteDoc* pDoc = GetDocument();

	glBegin(GL_LINES);
	if(m_bShowBackgroundImage && pDoc->m_BackgroundBitmapLoaded) //  // 2D background map is loaded
		glColor3f(0.0f,0.5f,0.5f);   // dark blue
	else
		glColor3f(1.0f,1.0f,0.5f);  //yellow

	std::list<DTALink*>::iterator iLink;
	for (iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
	{
		float fromX = NXtoSX_org((*iLink)->m_FromPoint.x);
		float fromY = NYtoSY_org((*iLink)->m_FromPoint.y);
		float toX = NXtoSX_org((*iLink)->m_ToPoint.x);
		float toY = NYtoSY_org((*iLink)->m_ToPoint.y);

		glVertex3f(fromX,fromY,Height0);
		glVertex3f(toX,toY,Height0);

	}

	//draw time-dependent path

	if(m_ShowAllPaths)
	{

		unsigned int iPath;

		float TimeStep = 2.0f;


		for(int Time= 0; Time<72; Time+=60)  // draw time-dependent shortest every 1 hour
		{

			for (iPath = 0; iPath < pDoc->m_PathDisplayList.size(); iPath++)
			{

				float CurrentTime = Time;

				SelectNumuericalColor(iPath);

				for (int i=0 ; i<pDoc->m_PathDisplayList[iPath].m_LinkSize; i++)
				{
					DTALink* pLink = pDoc->m_LinkNoMap[pDoc->m_PathDisplayList[iPath].m_LinkVector[i]];
					if(pLink!=NULL)
					{

						float fromX = NXtoSX_org(pLink->m_FromPoint.x);
						float fromY = NYtoSY_org(pLink->m_FromPoint.y);
						float toX = NXtoSX_org(pLink->m_ToPoint.x);
						float toY = NYtoSY_org(pLink->m_ToPoint.y);

						float TimeZFrom = CurrentTime*TimeStep;

						CurrentTime += pLink->ObtainHistTravelTime((int)(CurrentTime));

						float TimeZTo = CurrentTime*TimeStep;

						glVertex3f(fromX,fromY,TimeZFrom);
						glVertex3f(toX,toY,TimeZTo);

					}
				}
			}
		}

	}


	//draw time-dependent train path

	if(m_ShowAllTrains)
	{

		// find the maximum timesamp
		float max_time = 0;
		unsigned int v ;

		for(v = 0; v<pDoc->m_TrainVector.size(); v++)
		{

			DTA_Train* pTrain = pDoc->m_TrainVector[v];

			if((pTrain->m_DepartureTime + pTrain->m_ActualTripTime) > max_time)
				max_time = pTrain->m_DepartureTime + pTrain->m_ActualTripTime;
		}

		float m_ZResolution = 300.0f/max_time;

		for(v = 0; v<pDoc->m_TrainVector.size(); v++)
		{

			DTA_Train* pTrain = pDoc->m_TrainVector[v];

			SelectNumuericalColor(pTrain->m_TrainType-1);

			for(int n = 1; n< pTrain->m_NodeSize; n++)
			{
				DTALink* pLink = pDoc->m_LinkNoMap[pTrain->m_aryTN[n].RailLinkID];

				ASSERT(pLink!=NULL);

				float fromX = NXtoSX_org(pLink->m_FromPoint.x);
				float fromY = NYtoSY_org(pLink->m_FromPoint.y);
				float toX = NXtoSX_org(pLink->m_ToPoint.x);
				float toY = NYtoSY_org(pLink->m_ToPoint.y);

				float TimeZFrom = pTrain->m_aryTN[n-1].NodeTimestamp*m_ZResolution;
				float TimeZTo = pTrain->m_aryTN[n].NodeTimestamp*m_ZResolution;


				glVertex3f(fromX,fromY,TimeZFrom);
				glVertex3f(toX,toY,TimeZTo);


			}


		}
	}
	glEnd();

	//show travel time variability
	if(pDoc->m_LinkMOEMode == MOE_volume || pDoc->m_LinkMOEMode == MOE_speed || pDoc->m_LinkMOEMode == MOE_emissions)
	{
		glBegin(GL_QUADS);

		float MaxVariability = 0;


		std::list<DTALink*>::iterator iLink;
		for (iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
		{

			if(MaxVariability < ((*iLink)->m_MaxSpeed-(*iLink)->m_MinSpeed) )
				MaxVariability = (*iLink)->m_MaxSpeed-(*iLink)->m_MinSpeed;

		}

		for (iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
		{
			float fromX = NXtoSX_org((*iLink)->m_FromPoint.x);
			float fromY = NYtoSY_org((*iLink)->m_FromPoint.y);
			float toX = NXtoSX_org((*iLink)->m_ToPoint.x);
			float toY = NYtoSY_org((*iLink)->m_ToPoint.y);

			
			float VariabilityRatio = pDoc->GetLinkMOE((*iLink), MOE_volume , (int)g_Simulation_Time_Stamp);

			if(VariabilityRatio>=1) VariabilityRatio=1;
			if(VariabilityRatio<=0) VariabilityRatio=0;

//			GetColorFromPower(VolumeRatio);
			GetRGBColorFromPower(0.3);

			//		float ZTop =  (*iLink)->m_MeanVolume/10;  // assume maximum is 2000
			int current_time  = g_Simulation_Time_Stamp;

			float color_power = pDoc->GetLinkMOE((*iLink), pDoc->m_LinkMOEMode , (int)g_Simulation_Time_Stamp);

			float value = 0;
//			float color_power = pDoc->GetLinkMOE((*iLink), speed, (int)g_Simulation_Time_Stamp+ m_PredictionHorizon,value);

			float maximum_link_volume = 8000.0f;
			float max_density = 200.0f;

			float link_volume = (*iLink)->m_StaticLinkVolume ;

			if( pDoc->m_LinkMOEMode != MOE_none && g_Simulation_Time_Stamp >=1 && g_Simulation_Time_Stamp + m_PredictionHorizon< (*iLink)->m_SimulationHorizon)
			{
				link_volume = max(0,(*iLink)->m_LinkMOEAry[current_time + m_PredictionHorizon].ObsLinkFlow)* (*iLink)->m_NumLanes;
			}
//			float ZTop =  (*iLink)->m_SpeedLimit/(*iLink)->m_MeanSpeed*5;  // convert it to travel time index
			
			GetRGBColorFromPower(VariabilityRatio);
			
			float ZBottom =  0;


			//top use volume as default
			float ZTop = link_volume/maximum_link_volume*10;  // convert to 100 scale

			if(ZTop<=0)
				ZTop = 0;

			glVertex3f(fromX,fromY,ZBottom);
			glVertex3f(toX,toY,ZBottom);

			glVertex3f(toX,toY,ZTop);
			glVertex3f(fromX,fromY,ZTop);

		}
	}


		//show travel time variability
	if(m_bShowSecurityCube )
	{
		glBegin(GL_LINES);

                /*      This is the top face*/
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.0f, 0.0f, -1.0f);
                glVertex3f(-1.0f, 0.0f, -1.0f);
                glVertex3f(-1.0f, 0.0f, 0.0f);

                /*      This is the front face*/
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(-1.0f, 0.0f, 0.0f);
                glVertex3f(-1.0f, -1.0f, 0.0f);
                glVertex3f(0.0f, -1.0f, 0.0f);

                /*      This is the right face*/
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.0f, -1.0f, 0.0f);
                glVertex3f(0.0f, -1.0f, -1.0f);
                glVertex3f(0.0f, 0.0f, -1.0f);

                /*      This is the left face*/
                glVertex3f(-1.0f, 0.0f, 0.0f);
                glVertex3f(-1.0f, 0.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, 0.0f);

                /*      This is the bottom face*/
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, 0.0f);

                /*      This is the back face*/
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(-1.0f, 0.0f, -1.0f);
                glVertex3f(-1.0f, -1.0f, -1.0f);
                glVertex3f(0.0f, -1.0f, -1.0f);
	}
	glEnd();

	// draw vehicles


	if(pDoc->m_LinkMOEMode == MOE_vehicle)
	{
	bool bSelectVehicle = false;
	std::list<DTAVehicle*>::iterator iVehicle;

	int VehicleFilteringMode = 1;
	
	if (pDoc->m_VehicleSet.size() > 50000)
		VehicleFilteringMode = int(pDoc->m_VehicleSet.size()/50000)+1;

	for (iVehicle = pDoc->m_VehicleSet.begin(); iVehicle != pDoc->m_VehicleSet.end(); iVehicle++)
	{
		if((*iVehicle)->m_VehicleID %VehicleFilteringMode==0)
		{

			if((*iVehicle)->m_bComplete && (*iVehicle)->m_DepartureTime <=g_Simulation_Time_Stamp &&
				g_Simulation_Time_Stamp <=(*iVehicle)->m_ArrivalTime && (*iVehicle)->m_NodeSize>=2)
			{

				float ratio = 0;
				int LinkID = pDoc->GetVehilePosition((*iVehicle), g_Simulation_Time_Stamp,ratio);

				DTALink* pLink = pDoc->m_LinkNoMap[LinkID];
				if(pLink!=NULL)
				{
					float fromX = NXtoSX_org(pLink->m_FromPoint.x);
					float fromY = NYtoSY_org(pLink->m_FromPoint.y);
					float toX = NXtoSX_org(pLink->m_ToPoint.x);
					float toY = NYtoSY_org(pLink->m_ToPoint.y);

					m_EyeAng = atan2(toY-fromY,toX-fromX);
					float VehPointX = ratio*fromX + (1-ratio)*toX;
					float VehPointY = ratio*fromY + (1-ratio)*toY;

		

					//							glTranslatef(VehPointX,VehPointY,4);
					//							auxWireCube(2.0f); 
					//							glFlush();

					glPushMatrix();
					glTranslated(VehPointX,VehPointY,4);
					double HuAng;
					double Ang;
					doublePointStruct m_Normal;
					m_Normal.x=0.0;
					m_Normal.y=0.0;
					m_Normal.z=1.0;

					Ang=atan2(m_Normal.y,m_Normal.x);

					HuAng=Ang*180.0/3.1415926;
					glRotated(HuAng+270.0,0.0,0.0,1.0);
					//for z angle
					Ang=asin(m_Normal.z);
					HuAng=Ang*180.0/3.1415926;
					glRotated(HuAng,1.0,0.0,0.0);

					float m_Height = 4;
					float m_Width = 4;
					glColor3d(1,1,0);
					glTranslated(0.0,0.0,m_Height/2.0);//0.0);
					auxSolidBox(m_Height,m_Width,m_Height);
			
					if(!bSelectVehicle)  // not select vehicle yet
					{
						m_CarPosition.x = VehPointX;
						m_CarPosition.y = VehPointY;
						m_CarPosition.z = 4;
					
						glColor3d(1.0,0.0,0.0);  //red
						bSelectVehicle = true;
						m_Height = 8;
						m_Width = 6;
					}else
					{
						glColor3d(0.0,0.0,0.0);
					}
					glTranslated(0.0,m_Width/2.0+1.0,0.0);
					auxSolidBox(m_Height,2.0,m_Height);
					glPopMatrix();



				}

			}
		}

	}

	}
}

void CGLView::ComputeEyePosition()
{
	double x,y,z,t;
	z=m_EyeLength*sin(m_zAng);
	t=m_EyeLength*cos(m_zAng);
	if(fabs(t)<0.00000001)t=0.0;

	x=m_EyeLength*cos(m_xyAng);
	y=-m_EyeLength*sin(m_xyAng);

	if(fabs(x)<0.0001)x=0.0;
	if(fabs(y)<0.0001)y=0.0;
	if(fabs(z)<0.0001)z=0.0;

	m_EyePoint.x=m_ObservePoint.x+x;
	m_EyePoint.y=m_ObservePoint.y+y;
	m_EyePoint.z=m_ObservePoint.z+z;

	if(m_EyePoint.z<0)
		m_EyePoint.z = 0;
	///////////////////

	m_Normal.x=0.0;
	m_Normal.y=0.0;
	m_Normal.z=1.0;

}

void CGLView::On3DZoom() 
{
	m_State=glZoom;	
}

void CGLView::OnUpdate3DZoom(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_State==glZoom);	
}

void CGLView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_StartPoint=point;
	CView::OnLButtonDown(nFlags, point);
}

void CGLView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CSize size;
	size=point-m_StartPoint;
	m_StartPoint=point;
	if(nFlags&MK_LBUTTON){
		switch(m_State){
	case glMove:  // move XY

		m_ObservePoint.x-=(double)size.cx;
		m_ObservePoint.y+=(double)size.cy;

		Render();
		break;
	case glRotate:
		m_xyAng+=(double)size.cx/120.0;
		m_zAng+=(double)size.cy/120.0;

		if( m_zAng < 0)  //ensure m_zAng is always >=0
			m_zAng = 0;

		Render();
		break;
	case glZoom:

		m_EyeLength-=(double)size.cy*1.5;

		Render();
		break;
	default:
		break;
		}
	}
	CView::OnMouseMove(nFlags, point);
}

void CGLView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_State=glNone;
	CView::OnRButtonDown(nFlags, point);
}

void CGLView::On3DPam() 
{
	m_State=glMove;

}

void CGLView::OnUpdate3DPam(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_State==glMove);	

}

void CGLView::On3DRotate() 
{
	m_State=glRotate;

}

void CGLView::OnUpdate3DRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_State==glRotate);	

}

void CGLView::On3DLockZ() 
{
	m_bLockZ = true;
	Render();
}

void CGLView::OnUpdate3DLockZ(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bLockZ);	
}

void CGLView::On3DReset() 
{
	m_ObservePoint.x=0.0;
	m_ObservePoint.y=0.0;
	m_ObservePoint.z=0.0;
	m_Normal.x=0.0;
	m_Normal.y=1.0;
	m_Normal.z=0.0;
	m_zAng=2.809;//3.1415926/8.0;
	m_xyAng=PI/2.0;;
	m_EyeLength=800.0;
	m_State=glNone;
	Render();

}

void CGLView::OnUpdate3DReset(CCmdUI* pCmdUI) 
{
}

void CGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	double Step = 10;

    CString title_str;


	switch(nChar)
	{
	case VK_UP :
		m_ObservePoint.y-=Step;
		break;

	case VK_DOWN :
		m_ObservePoint.y+=Step;
		break;

	case VK_LEFT :
		m_ObservePoint.x+=Step;
		break;

	case VK_RIGHT :
		m_ObservePoint.x-=Step;
		break;

	case VK_PRIOR: 
		m_PredictionHorizon -=5;

		if(m_PredictionHorizon < 0)
			m_PredictionHorizon = 0;

	  title_str.Format("prediction: +%d min", m_PredictionHorizon);
      GetParentFrame()->SetWindowText(title_str);

        break;
    case VK_NEXT:
		m_PredictionHorizon +=5;

	   title_str.Format("prediction: +%d min", m_PredictionHorizon);
      GetParentFrame()->SetWindowText(title_str);

	  break;
	}
	Render();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(nFlags != MK_CONTROL)  // shift or nothing
	{
		m_EyeLength+=zDelta*1.0;
	}else  //control + mouse wheel
	{
		m_BackgroundMapHeight_MedLayer+=zDelta*0.10f;
	}

	Render();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CGLView::On3ddisplayTraveltimevariability()
{
	m_ShowSpeedVariability = !m_ShowSpeedVariability;
	Render();
}

void CGLView::On3ddisplayTimeDependentPaths()
{
	m_ShowAllPaths = !m_ShowAllPaths;
	Render();
}

void CGLView::On3ddisplayAnimation()
{
	// TODO: Add your command handler code here
}

void CGLView::OnUpdate3ddisplayTraveltimevariability(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ShowSpeedVariability);

}

void CGLView::OnUpdate3ddisplayTimeDependentPaths(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ShowAllPaths);
}

void CGLView::OnUpdate3ddisplayAnimation(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CGLView::On3ddisplayBasicview()
{
	m_ShowSpeedVariability = false;
	m_ShowAllPaths = false;
	Render();
}


void CGLView::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);

	//	CView::OnClose();
}


void CGLView::On3dCarfol()
{
	m_bFollowCar = !m_bFollowCar;
	Render();

}

void CGLView::OnUpdate3dCarfol(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bFollowCar);
}
