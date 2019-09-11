// TLiteView.cpp : implementation of the CTLiteView class
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
#include "math.h"

#include "TLiteDoc.h"
#include "TLiteView.h"
#include "MainFrm.h"
#include "DlgMOE.h"
#include "Network.h"
#include "DlgFindALink.h"
#include "DlgPathMOE.h"
#include "GLView.h"
#include "DlgLinkProperties.h"
#include "Dlg_NodeProperties.h"
#include "Dlg_GoogleFusionTable.h"
#include "Dlg_VehicleClassification.h"
#include "Dlg_TravelTimeReliability.h"
#include "Page_Node_Movement.h"
#include "Page_Node_Phase.h"
#include "MyPropertySheet.h"
#include "CSVParser.h"
#include "Dlg_BackgroundImageLocation.h"
#include "Chart_Control\\NSChartCtrl.h"

#include "Dlg_DisplayConfiguration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GDPoint g_Origin;
float g_Resolution;

_cursor_type g_current_cursor_type = _cursor_standard_arrow;


bool compare_IncomingLinkAngleData (IncomingLinkAngle first, IncomingLinkAngle second)
{
	if(first.Angle < second.Angle) return true;
	else 
		return false;
}

void g_SetCursor(_cursor_type cursor_type)
{
	//if(cursor_type == _cursor_movement_network && g_current_cursor_type!=_cursor_movement_network)
	//{	
	//	SetCursor(AfxGetApp()->LoadCursor(IDC_MOVENETWORK));
	//	return;
	//}

	if(cursor_type == _cursor_create_node && g_current_cursor_type!=_cursor_create_node)
	{	
		SetCursor(AfxGetApp()->LoadCursor(IDC_CREATE_NODE_CURSOR));
		return;
	}
	if(cursor_type == _cursor_create_link && g_current_cursor_type!=_cursor_create_link)
	{	
		SetCursor(AfxGetApp()->LoadCursor(IDC_CREATE_LINK_CURSOR));
		return;
	}
	if(cursor_type == _cursor_create_subarea && g_current_cursor_type!= _cursor_create_subarea)
	{	
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_SUBAREA));
		return;
	}

	if(cursor_type == _cursor_create_zone && g_current_cursor_type!= _cursor_create_zone)
	{	
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZONE));
		return;
	}

	if(cursor_type == _cursor_standard_arrow && g_current_cursor_type!=_cursor_standard_arrow)
	{	
		SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW));
		return;
	}
	if(cursor_type == _cursor_standard_arrow && g_current_cursor_type!=_cursor_standard_arrow)
	{	
		SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW));
		return;
	}

	if(cursor_type == _cursor_wait && g_current_cursor_type!=_cursor_standard_arrow)
	{	
		SetCursor(AfxGetApp()->LoadCursor(IDC_WAIT));
		return;
	}

};

extern COLORREF g_MOEDisplayColor[MAX_MOE_DISPLAYCOLOR];
extern float g_Simulation_Time_Stamp;
extern bool g_LinkMOEDlgShowFlag;
int g_ViewID = 0;
IMPLEMENT_DYNCREATE(CTLiteView, CView)

BEGIN_MESSAGE_MAP(CTLiteView, CView)

	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()

	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VIEW_ZOOMIN, &CTLiteView::OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, &CTLiteView::OnViewZoomout)
	ON_COMMAND(ID_VIEW_SHOWNETWORK, &CTLiteView::OnViewShownetwork)
	ON_COMMAND(ID_VIEW_MOVE, &CTLiteView::OnViewMove)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_VIEW_MOVE, &CTLiteView::OnUpdateViewMove)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_VIEW_SELECT, &CTLiteView::OnViewSelect)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECT, &CTLiteView::OnUpdateViewSelect)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_NODE_ORIGIN, &CTLiteView::OnNodeOrigin)
	ON_COMMAND(ID_NODE_DESTINATION, &CTLiteView::OnNodeDestination)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_SEARCH_FINDLINK, &CTLiteView::OnSearchFindlink)
	ON_COMMAND(ID_VIEW_IMAGE, &CTLiteView::OnEditChangebackgroupimagethroughmouse)
	ON_UPDATE_COMMAND_UI(ID_VIEW_IMAGE, &CTLiteView::OnUpdateEditChangebackgroupimagethroughmouse)
	ON_COMMAND(ID_VIEW_BACKGROUNDIMAGE, &CTLiteView::OnViewBackgroundimage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BACKGROUNDIMAGE, &CTLiteView::OnUpdateViewBackgroundimage)
	ON_COMMAND(ID_VIEW_SHOWLINKTYPE, &CTLiteView::OnViewShowlinktype)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWLINKTYPE, &CTLiteView::OnUpdateViewShowlinktype)
	ON_COMMAND(ID_SHOW_SHOWNODE, &CTLiteView::OnShowShownode)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWNODE, &CTLiteView::OnUpdateShowShownode)
	ON_COMMAND(ID_SHOW_SHOWALLPATHS, &CTLiteView::OnShowShowallpaths)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWALLPATHS, &CTLiteView::OnUpdateShowShowallpaths)
	ON_COMMAND(ID_SHOW_SHOWNODENUMBER, &CTLiteView::OnShowShownodenumber)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWNODENUMBER, &CTLiteView::OnUpdateShowShownodenumber)
	ON_COMMAND(ID_Edit_Create1WayLink, &CTLiteView::OnEditCreate1waylink)
	ON_COMMAND(ID_Edit_Create2WayLinks, &CTLiteView::OnEditCreate2waylinks)
	ON_UPDATE_COMMAND_UI(ID_Edit_Create1WayLink, &CTLiteView::OnUpdateEditCreate1waylink)
	ON_UPDATE_COMMAND_UI(ID_Edit_Create2WayLinks, &CTLiteView::OnUpdateEditCreate2waylinks)
	ON_COMMAND(ID_SHOW_GRID, &CTLiteView::OnShowGrid)
	ON_UPDATE_COMMAND_UI(ID_SHOW_GRID, &CTLiteView::OnUpdateShowGrid)
	ON_COMMAND(ID_SHOW_LINKARROW, &CTLiteView::OnShowLinkarrow)
	ON_UPDATE_COMMAND_UI(ID_SHOW_LINKARROW, &CTLiteView::OnUpdateShowLinkarrow)
	ON_COMMAND(ID_IMAGE_LOCKBACKGROUNDIMAGEPOSITION, &CTLiteView::OnImageLockbackgroundimageposition)
	ON_COMMAND(ID_VIEW_TEXTLABEL, &CTLiteView::OnViewTextlabel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TEXTLABEL, &CTLiteView::OnUpdateViewTextlabel)
	ON_COMMAND(ID_LINK_DELETE, &CTLiteView::OnLinkDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETESELECTEDLINK, &CTLiteView::OnUpdateEditDeleteselectedlink)
	ON_COMMAND(ID_LINK_EDITLINK, &CTLiteView::OnLinkEditlink)
	ON_COMMAND(ID_EDIT_CREATENODE, &CTLiteView::OnEditCreatenode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CREATENODE, &CTLiteView::OnUpdateEditCreatenode)
	ON_COMMAND(ID_EDIT_DELETESELECTEDNODE, &CTLiteView::OnEditDeleteselectednode)
	ON_COMMAND(ID_EDIT_SELECTNODE, &CTLiteView::OnEditSelectnode)
	ON_COMMAND(ID_VIEW_SELECT_NODE, &CTLiteView::OnViewSelectNode)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECT_NODE, &CTLiteView::OnUpdateViewSelectNode)
	ON_COMMAND(ID_EDIT_CREATESUBAREA, &CTLiteView::OnEditCreatesubarea)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CREATESUBAREA, &CTLiteView::OnUpdateEditCreatesubarea)
	ON_COMMAND(ID_TOOLS_REMOVENODESANDLINKSOUTSIDESUBAREA, &CTLiteView::OnToolsRemovenodesandlinksoutsidesubarea)
	ON_COMMAND(ID_EDIT_DELETELINKSOUTSIDESUBAREA, &CTLiteView::OnEditDeletelinksoutsidesubarea)
	ON_COMMAND(ID_EDIT_MOVENETWORKCOORDINATES, &CTLiteView::OnEditMovenetworkcoordinates)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVENETWORKCOORDINATES, &CTLiteView::OnUpdateEditMovenetworkcoordinates)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_VIEW_INCREASENODESIZE, &CTLiteView::OnViewIncreasenodesize)
	ON_COMMAND(ID_VIEW_DECREATENODESIZE, &CTLiteView::OnViewDecreatenodesize)
	ON_COMMAND(ID_NODE_CHECKCONNECTIVITYFROMHERE, &CTLiteView::OnNodeCheckconnectivityfromhere)
	ON_COMMAND(ID_NODE_DIRECTIONTOHEREANDRELIABILITYANALYSIS, &CTLiteView::OnNodeDirectiontohereandreliabilityanalysis)
	ON_COMMAND(ID_LINK_INCREASEBANDWIDTH, &CTLiteView::OnLinkIncreasebandwidth)
	ON_COMMAND(ID_LINK_DECREASEBANDWIDTH, &CTLiteView::OnLinkDecreasebandwidth)
	ON_COMMAND(ID_NODE_MOVEMENTPROPERTIES, &CTLiteView::OnNodeMovementproperties)
	ON_COMMAND(ID_LINK_LINEDISPLAYMODE, &CTLiteView::OnLinkLinedisplaymode)
	ON_UPDATE_COMMAND_UI(ID_LINK_LINEDISPLAYMODE, &CTLiteView::OnUpdateLinkLinedisplaymode)
	ON_COMMAND(ID_VIEW_SHOW_CONNECTOR, &CTLiteView::OnViewShowConnector)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_CONNECTOR, &CTLiteView::OnUpdateViewShowConnector)
	ON_COMMAND(ID_VIEW_HIGHLIGHTCENTROIDSANDACTIVITYLOCATIONS, &CTLiteView::OnViewHighlightcentroidsandactivitylocations)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HIGHLIGHTCENTROIDSANDACTIVITYLOCATIONS, &CTLiteView::OnUpdateViewHighlightcentroidsandactivitylocations)
	ON_COMMAND(ID_VIEW_BACKGROUNDCOLOR, &CTLiteView::OnViewBackgroundcolor)
	ON_COMMAND(ID_ACTIVITYLOCATIONMODE_NOLANDUSEACTIVITY, &CTLiteView::OnActivitylocationmodeNolanduseactivity)
	ON_UPDATE_COMMAND_UI(ID_ACTIVITYLOCATIONMODE_NOLANDUSEACTIVITY, &CTLiteView::OnUpdateActivitylocationmodeNolanduseactivity)
	ON_COMMAND(ID_ACTIVITYLOCATIONMODE_LANDUSEACTIVITY, &CTLiteView::OnActivitylocationmodeLanduseactivity)
	ON_UPDATE_COMMAND_UI(ID_ACTIVITYLOCATIONMODE_LANDUSEACTIVITY, &CTLiteView::OnUpdateActivitylocationmodeLanduseactivity)
	ON_COMMAND(ID_ACTIVITYLOCATIONMODE_EXTERNALORIGIN, &CTLiteView::OnActivitylocationmodeExternalorigin)
	ON_UPDATE_COMMAND_UI(ID_ACTIVITYLOCATIONMODE_EXTERNALORIGIN, &CTLiteView::OnUpdateActivitylocationmodeExternalorigin)
	ON_COMMAND(ID_ACTIVITYLOCATIONMODE_EXTERNALDESTINATION, &CTLiteView::OnActivitylocationmodeExternaldestination)
	ON_UPDATE_COMMAND_UI(ID_ACTIVITYLOCATIONMODE_EXTERNALDESTINATION, &CTLiteView::OnUpdateActivitylocationmodeExternaldestination)
	ON_COMMAND(ID_MOE_ODDEMAND, &CTLiteView::OnMoeOddemand)
	ON_UPDATE_COMMAND_UI(ID_MOE_ODDEMAND, &CTLiteView::OnUpdateMoeOddemand)
	ON_UPDATE_COMMAND_UI(ID_LINK_INCREASEBANDWIDTH, &CTLiteView::OnUpdateLinkIncreasebandwidth)
	ON_UPDATE_COMMAND_UI(ID_LINK_DECREASEBANDWIDTH, &CTLiteView::OnUpdateLinkDecreasebandwidth)
	ON_COMMAND(ID_EXPORT_CREATEVISSIMFILES, &CTLiteView::OnExportCreatevissimfiles)
	ON_COMMAND(ID_DEBUG_SHOWVEHICLESWITHINCOMPLETETRIPSONLY, &CTLiteView::OnDebugShowvehicleswithincompletetripsonly)
	ON_UPDATE_COMMAND_UI(ID_DEBUG_SHOWVEHICLESWITHINCOMPLETETRIPSONLY, &CTLiteView::OnUpdateDebugShowvehicleswithincompletetripsonly)
	ON_COMMAND(ID_VEHICLE_VEHICLENUMBER, &CTLiteView::OnVehicleVehiclenumber)
	ON_UPDATE_COMMAND_UI(ID_VEHICLE_VEHICLENUMBER, &CTLiteView::OnUpdateVehicleVehiclenumber)
	ON_COMMAND(ID_VEHICLE_SHOWSELECTEDVEHICLEONLY, &CTLiteView::OnVehicleShowselectedvehicleonly)
	ON_UPDATE_COMMAND_UI(ID_VEHICLE_SHOWSELECTEDVEHICLEONLY, &CTLiteView::OnUpdateVehicleShowselectedvehicleonly)
	ON_COMMAND(ID_NODE_ADDINTERMEDIATEDESTINATIONHERE, &CTLiteView::OnNodeAddintermediatedestinationhere)
	ON_COMMAND(ID_NODE_REMOVEALLINTERMEDIATEDESTINATION, &CTLiteView::OnNodeRemoveallintermediatedestination)
	ON_COMMAND(ID_LINK_AVOIDUSINGTHISLINKINROUTING, &CTLiteView::OnLinkAvoidusingthislinkinrouting)
	ON_BN_CLICKED(IDC_BUTTON_Configuration, &CTLiteView::OnBnClickedButtonConfiguration)
	ON_COMMAND(ID_NODE_NODEPROPERTIES, &CTLiteView::OnNodeNodeproperties)
	ON_COMMAND(ID_NODE_AVOIDTHISNODE, &CTLiteView::OnNodeAvoidthisnode)
	ON_COMMAND(ID_NODE_REMOVENODEAVOIDANCECONSTRAINT, &CTLiteView::OnNodeRemovenodeavoidanceconstraint)
	ON_COMMAND(ID_EDIT_MOVENODE, &CTLiteView::OnEditMovenode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVENODE, &CTLiteView::OnUpdateEditMovenode)
	ON_COMMAND(ID_BACKGROUNDIMAGE_MARKLONG_A, &CTLiteView::OnBackgroundimageMarklongA)
	ON_COMMAND(ID_BACKGROUNDIMAGE_MARKLONG_B, &CTLiteView::OnBackgroundimageMarklongB)
	ON_COMMAND(ID_BACKGROUNDIMAGE_ADDLAT, &CTLiteView::OnBackgroundimageAddlat)
	ON_COMMAND(ID_ZONE_HIGHLIGHTASSOCIATEDACITITYLOCATIONS, &CTLiteView::OnZoneHighlightassociatedacititylocations)
	ON_UPDATE_COMMAND_UI(ID_ZONE_HIGHLIGHTASSOCIATEDACITITYLOCATIONS, &CTLiteView::OnUpdateZoneHighlightassociatedacititylocations)
	ON_COMMAND(ID_ZONE_CREATEZONE, &CTLiteView::OnZoneCreatezone)
	ON_COMMAND(ID_EDIT_CREATEZONE, &CTLiteView::OnEditCreatezone)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CREATEZONE, &CTLiteView::OnUpdateEditCreatezone)
	ON_COMMAND(ID_SUBAREA_HIGHLIGHTLINKSINSIDESUBAREA, &CTLiteView::OnSubareaHighlightlinksinsidesubarea)
	ON_UPDATE_COMMAND_UI(ID_SUBAREA_HIGHLIGHTLINKSINSIDESUBAREA, &CTLiteView::OnUpdateSubareaHighlightlinksinsidesubarea)
	ON_COMMAND(ID_SUBAREA_HIGHLIGHTLINKSACOSSSUBAREA, &CTLiteView::OnSubareaHighlightlinksacosssubarea)
	ON_UPDATE_COMMAND_UI(ID_SUBAREA_HIGHLIGHTLINKSACOSSSUBAREA, &CTLiteView::OnUpdateSubareaHighlightlinksacosssubarea)
	ON_COMMAND(ID_ODMATRIX_VIEWTOP50ODPAIRSONLY, &CTLiteView::OnOdmatrixViewtop50odpairsonly)
	ON_UPDATE_COMMAND_UI(ID_ODMATRIX_VIEWTOP50ODPAIRSONLY, &CTLiteView::OnUpdateOdmatrixViewtop50odpairsonly)
	ON_COMMAND(ID_TRANSIT_SHOWTRANSITACCESSIBILITY, &CTLiteView::OnTransitShowtransitaccessibility)
	ON_UPDATE_COMMAND_UI(ID_TRANSIT_SHOWTRANSITACCESSIBILITY, &CTLiteView::OnUpdateTransitShowtransitaccessibility)
	ON_COMMAND(ID_TRANSIT_CALCULATETRANSITACCESSSIBILITYFROMHERE, &CTLiteView::OnTransitCalculatetransitaccesssibilityfromhere)
	ON_COMMAND(ID_TRANSIT_OUTPUTTRANSITACCESSSIBILITYFROMHERE, &CTLiteView::OnTransitOutputtransitaccesssibilityfromhere)
	ON_COMMAND(ID_MOVEMENT_HIGHLIGHTPROHIBITEDMOVEMENTS, &CTLiteView::OnMovementHighlightprohibitedmovements)
	ON_UPDATE_COMMAND_UI(ID_MOVEMENT_HIGHLIGHTPROHIBITEDMOVEMENTS, &CTLiteView::OnUpdateMovementHighlightprohibitedmovements)
	ON_COMMAND(ID_TRANSIT_SHOWTRANSITLINKSONLY, &CTLiteView::OnTransitShowtransitlinksonly)
	ON_COMMAND(ID_TRANSIT_SHOWWALKLINKSONLY, &CTLiteView::OnTransitShowwalklinksonly)
	ON_UPDATE_COMMAND_UI(ID_TRANSIT_SHOWTRANSITLINKSONLY, &CTLiteView::OnUpdateTransitShowtransitlinksonly)
	ON_UPDATE_COMMAND_UI(ID_TRANSIT_SHOWWALKLINKSONLY, &CTLiteView::OnUpdateTransitShowwalklinksonly)
	ON_COMMAND(ID_FILE_PRINT, &CTLiteView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTLiteView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CTLiteView::OnFilePrintSetup)
	ON_COMMAND(ID_VEHICLEDATA_SHOWCOMPLETETRAJECTORY, &CTLiteView::OnVehicledataShowcompletetrajectory)
	ON_UPDATE_COMMAND_UI(ID_VEHICLEDATA_SHOWCOMPLETETRAJECTORY, &CTLiteView::OnUpdateVehicledataShowcompletetrajectory)
	ON_COMMAND(ID_VEHICLEDATA_SHOW, &CTLiteView::OnVehicledataShow)
	ON_UPDATE_COMMAND_UI(ID_VEHICLEDATA_SHOW, &CTLiteView::OnUpdateVehicledataShow)
	ON_COMMAND(ID_DETECTOR_UNLOCK, &CTLiteView::OnDetectorUnlock)
	ON_UPDATE_COMMAND_UI(ID_DETECTOR_UNLOCK, &CTLiteView::OnUpdateDetectorUnlock)
	ON_COMMAND(ID_DETECTOR_SHOWSENSORDESCRIPTION33769, &CTLiteView::OnDetectorShowsensordescription33769)
	ON_UPDATE_COMMAND_UI(ID_DETECTOR_SHOWSENSORDESCRIPTION33769, &CTLiteView::OnUpdateDetectorShowsensordescription33769)
	ON_COMMAND(ID_DETECTOR_SHOWMATCHEDLINKFORSENSOR, &CTLiteView::OnDetectorShowmatchedlinkforsensor)
	ON_UPDATE_COMMAND_UI(ID_DETECTOR_SHOWMATCHEDLINKFORSENSOR, &CTLiteView::OnUpdateDetectorShowmatchedlinkforsensor)
	ON_COMMAND(ID_GRID_UNLOCK, &CTLiteView::OnGridUnlock)
	ON_UPDATE_COMMAND_UI(ID_GRID_UNLOCK, &CTLiteView::OnUpdateGridUnlock)
	ON_COMMAND(ID_LINK_SHOWHIDEREFERENCENETWORK, &CTLiteView::OnLinkShowhidereferencenetwork)
	ON_UPDATE_COMMAND_UI(ID_LINK_SHOWHIDEREFERENCENETWORK, &CTLiteView::OnUpdateLinkShowhidereferencenetwork)
	ON_COMMAND(ID_EDIT_MOVEReferenceNETWORKCOORDINATES, &CTLiteView::OnEditMovereferencenetworkcoordinates)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MOVEReferenceNETWORKCOORDINATES, &CTLiteView::OnUpdateEditMovereferencenetworkcoordinates)
	ON_COMMAND(ID_DETECTOR_SHOWSPEEDSENSORLOCATION, &CTLiteView::OnDetectorShowspeedsensorlocation)
	ON_UPDATE_COMMAND_UI(ID_DETECTOR_SHOWFIXEDCOUNTDETECTORLOCATION, &CTLiteView::OnUpdateDetectorShowfixedcountdetectorlocation)
	ON_UPDATE_COMMAND_UI(ID_DETECTOR_SHOWSPEEDSENSORLOCATION, &CTLiteView::OnUpdateDetectorShowspeedsensorlocation)
	ON_COMMAND(ID_DETECTOR_SHOWFIXEDCOUNTDETECTORLOCATION, &CTLiteView::OnDetectorShowfixedcountdetectorlocation)
	ON_COMMAND(ID_SHOWALLOWEDLINKSFORDEMANDTYPE_DEMANDTYPE1, &CTLiteView::OnShowallowedlinksfordemandtypeDemandtype1)
	ON_COMMAND(ID_SHOWALLOWEDLINKSFORDEMANDTYPE_DEMANDTYPE2, &CTLiteView::OnShowallowedlinksfordemandtypeDemandtype2)
	ON_COMMAND(ID_SHOWALLOWEDLINKSFORDEMANDTYPE_DEMANDTYPE3, &CTLiteView::OnShowallowedlinksfordemandtypeDemandtype3)
	ON_COMMAND(ID_SHOWALLOWEDLINKSFORDEMANDTYPE_DEMANDTYPE4, &CTLiteView::OnShowallowedlinksfordemandtypeDemandtype4)
	ON_UPDATE_COMMAND_UI(ID_SHOWALLOWEDLINKSFORDEMANDTYPE_DEMANDTYPE1, &CTLiteView::OnUpdateShowallowedlinksfordemandtypeDemandtype1)
	ON_UPDATE_COMMAND_UI(ID_SHOWALLOWEDLINKSFORDEMANDTYPE_DEMANDTYPE2, &CTLiteView::OnUpdateShowallowedlinksfordemandtypeDemandtype2)
	ON_UPDATE_COMMAND_UI(ID_SHOWALLOWEDLINKSFORDEMANDTYPE_DEMANDTYPE3, &CTLiteView::OnUpdateShowallowedlinksfordemandtypeDemandtype3)
	ON_UPDATE_COMMAND_UI(ID_SHOWALLOWEDLINKSFORDEMANDTYPE_DEMANDTYPE4, &CTLiteView::OnUpdateShowallowedlinksfordemandtypeDemandtype4)
	ON_COMMAND(ID_SHOWALLOWEDLINKSFORDEMANDTYPE_SHOWALL, &CTLiteView::OnShowallowedlinksfordemandtypeShowall)
	ON_UPDATE_COMMAND_UI(ID_SHOWALLOWEDLINKSFORDEMANDTYPE_SHOWALL, &CTLiteView::OnUpdateShowallowedlinksfordemandtypeShowall)
	ON_COMMAND(ID_REFERENCELINE_SHOWREFERENCELINENO, &CTLiteView::OnReferencelineShowreferencelineno)
	ON_BN_CLICKED(IDC_BUTTON_Search, &CTLiteView::OnBnClickedButtonSearch)
	END_MESSAGE_MAP()

// CTLiteView construction/destruction
// CTLiteView construction/destruction

CBrush g_BlackBrush(RGB(10,10,10));

CBrush  g_WhiteBrush(RGB(255, 255, 255));
CBrush  g_GreenBrush(RGB(0, 255, 0));

CBrush g_ActivityLocationBrush(RGB(255,0,0));

CPen g_BlackPen(PS_SOLID,1,RGB(0,0,0));

CPen g_TransitPen(PS_SOLID,1,RGB(255,69,0));  // orange red
CBrush g_TransitBrush(RGB(184,134,11));  //DarkGoldenrod	

CPen g_TransitLightPen(PS_SOLID,1,RGB(216,191,216));  // orange red
CBrush g_TransitLightBrush(RGB(255,225,255));  //DarkGoldenrod	


CPen g_TransitAccessibilityPen(PS_SOLID,1,RGB(0,0,255)); //Blue 
CBrush g_TransitAccessibilityBrush(RGB(0,0,255));  //Blue 	

CPen g_LaneMarkingPen(PS_DASH,0,RGB(255,255,255));

CPen g_PenSelectColor(PS_SOLID,5,RGB(255,0,0));
CPen g_PenDetectorColor(PS_SOLID, 1, RGB(0, 0, 255));

CPen g_PenExternalDColor(PS_SOLID,2,RGB(173,255,047)); 
CPen g_PenExternalOColor(PS_SOLID,2,RGB(255,165,0));

CPen g_PenODColor(PS_SOLID,2,RGB(255,255,0));
CPen g_PenSelectPath(PS_SOLID,3,RGB(0,0,255));  // blue

CPen g_PenDisplayColor(PS_SOLID,2,RGB(255,255,0));

CPen g_PenStopSignNodeColor(PS_SOLID,1,RGB(205,200,177));
CPen g_PenSignalNodeColor(PS_SOLID,1,RGB(255,0,0));

CPen g_GPSTrajectoryColor(PS_SOLID, 1, RGB(255,105,180));
CPen g_GPSTrajectoryColor0(PS_SOLID, 0, RGB(0, 0, 0));

CPen g_PenNodeColor(PS_SOLID,1,RGB(0,0,0));

CPen g_PenConnectorColor(PS_DASH,1,RGB(0,0,255));
CPen g_PenCentroidColor(PS_SOLID,1,RGB(0,255,255));

CPen g_PenQueueColor(PS_SOLID,2,RGB(255,0,0));

CPen g_PenQueueBandColor(PS_SOLID,1,RGB(255,0,0));
CBrush g_BrushQueueBandColor(RGB(255,0,0));

CPen g_PenCrashColor(PS_SOLID,1,RGB(255,0,0));
CBrush  g_BrushCrash(HS_VERTICAL,RGB(255,0,255)); //green
CBrush  g_TransitCrash(HS_CROSS,RGB(255,0,255)); //green

CPen g_PenGreen(PS_SOLID,1,RGB(0,255,0));
CBrush  g_BrushGreen(RGB(0,255,0)); //green


CPen g_PenDashBlue(PS_DASH,1,RGB(0,0,255));
CPen g_PenBlue(PS_SOLID,2,RGB(0,0,255));
CPen g_PenReference(PS_SOLID, 2, RGB(255, 0, 255));
CBrush g_BrushReference(RGB(184, 134, 11));


CBrush  g_BrushBlue(RGB(0,0,255));

CPen g_PenSensorColor(PS_SOLID,0,RGB(0,255,0));
CPen g_PenSpeedSensorColor(PS_SOLID,0,RGB(255,0,255));

CBrush g_BrushSensor(RGB(0,255,0));
CBrush g_InvalidBrushSensor(RGB(255, 0, 0));
CBrush g_BrushSpeedSensor(RGB(255,0,255));
CPen g_PenNotMatchedSensorColor(PS_SOLID,1,RGB(255,0,0));

CPen g_PenSelectColor0(PS_SOLID,2,RGB(255,0,0));  // red
CPen g_PenSelectColor1(PS_SOLID,2,RGB(0,0,255));  // blue
CPen g_PenSelectColor2(PS_SOLID,2,RGB(255,0,255)); //magenta
CPen g_PenSelectColor3(PS_SOLID,2,RGB(0,255,255));   // cyan
CPen g_PenSelectColor4(PS_SOLID,2,RGB(0,255,0));  // green
CPen g_PenSelectColor5(PS_SOLID,2,RGB(255,255,0)); // yellow
CPen g_PenPaxSelectColor0(PS_DOT, 1, RGB(255, 0, 0));  // red

CPen g_Pen2SelectColor0(PS_DOT ,1,RGB(255,0,0));  // red
CPen g_Pen2SelectColor1(PS_DOT ,1,RGB(0,0,255));  // blue
CPen g_Pen2SelectColor2(PS_DOT ,1,RGB(255,0,255)); //magenta
CPen g_Pen2SelectColor3(PS_DOT ,1,RGB(0,255,255));   // cyan
CPen g_Pen2SelectColor4(PS_DOT ,1,RGB(0,255,255));  // green
CPen g_Pen2SelectColor5(PS_DOT ,1,RGB(255,255,0)); // yellow


CPen g_PenProhibitedMovement(PS_DASH,6,RGB(255,0,0));  // PS_DASH

CPen g_PenSelectColor0_obs(PS_SOLID,2,RGB(255,0,0));  // red
CPen g_PenSelectColor1_obs(PS_SOLID,2,RGB(0,255,0));  // green
CPen g_PenSelectColor2_obs(PS_SOLID,2,RGB(255,0,255)); //magenta
CPen g_PenSelectColor3_obs(PS_SOLID,2,RGB(0,255,255));   // cyan
CPen g_PenSelectColor4_obs(PS_SOLID,2,RGB(0,0,255));  // blue
CPen g_PenSelectColor5_obs(PS_SOLID,2,RGB(255,255,0)); // yellow

CBrush  g_BrushColor0(HS_BDIAGONAL, RGB(255,0,0));  // red
CBrush  g_BrushColor1(HS_FDIAGONAL,RGB(0,0,255));  // green
CBrush  g_BrushColor2(HS_VERTICAL,RGB(255,0,255)); //magenta
CBrush  g_BrushColor3(HS_HORIZONTAL,RGB(0,255,255));   // cyan
CBrush  g_BrushColor4(HS_CROSS,RGB(0,255,0));  // green
CBrush  g_BrushColor5(HS_DIAGCROSS,RGB(255,255,0)); // yellow

CBrush  g_BrushLinkBand(RGB(125,125,0)); // 
CBrush  g_BrushLinkBandVolume(RGB(0,0,255)); // 

CBrush  g_BrushLinkReference(HS_CROSS, RGB(255,0,255)); //magenta
CPen    g_PenLinkReference(PS_SOLID,2,RGB(255,0,255));  //magenta

CPen g_ThickPenSelectColor0(PS_SOLID,3,RGB(255,0,0));  // red
CPen g_ThickPenSelectColor1(PS_SOLID,3,RGB(0,0,255));  // blue
CPen g_ThickPenSelectColor2(PS_SOLID,3,RGB(255,0,255)); //magenta
CPen g_ThickPenSelectColor3(PS_SOLID,3,RGB(0,255,255));   // cyan
CPen g_ThickPenSelectColor4(PS_SOLID,3,RGB(0,255,0));  // green
CPen g_ThickPenSelectColor5(PS_SOLID,3,RGB(255,255,0)); // yellow

CPen g_SuperThickPenSelectColor0(PS_SOLID,5,RGB(255,0,0));  // red
CPen g_SuperThickPenSelectColor1(PS_SOLID,5,RGB(0,0,255));  // blue
CPen g_SuperThickPenSelectColor2(PS_SOLID,5,RGB(255,0,255)); //magenta
CPen g_SuperThickPenSelectColor3(PS_SOLID,5,RGB(0,255,255));   // cyan
CPen g_SuperThickPenSelectColor4(PS_SOLID,5,RGB(0,255,0));  // green
CPen g_SuperThickPenSelectColor5(PS_SOLID,5,RGB(255,255,0)); // yellow


CPen g_TempLinkPen(PS_DASH,0,RGB(255,255,255));
CPen g_AVILinkPen(PS_DASH,0,RGB(0,255,0));
CPen g_SubareaLinkPen(PS_SOLID,0,RGB(255,255,0));
CPen g_SubareaBoundaryLinkPen(PS_SOLID,2,RGB(255,0,255));
CPen g_SubareaPen(PS_DASH,2,RGB(255,0,0));

CPen g_GridPen(PS_SOLID,1,RGB(190,190,190));

CPen g_PenVehicle(PS_SOLID,1,RGB(0,255,0));  // yellow
CPen g_BrushVehicle(PS_SOLID,1,RGB(0,255,0)); //magenta

CPen g_PenSelectedVehicle(PS_SOLID,2,RGB(255,0,0));  // red
CPen g_PenMatchedSensorLink(PS_DASH, 1, RGB(255, 0, 0));  // red

CPen g_BrushTransitUser(PS_SOLID,1,RGB(255,0,0)); //magenta



void g_SelectColorCode(CDC* pDC, int ColorCount)
{
	switch(ColorCount)
	{

	case 0: pDC->SelectObject(&g_PenSelectColor0); break;
	case 1: pDC->SelectObject(&g_PenSelectColor1); break;
	case 2: pDC->SelectObject(&g_PenSelectColor2); break;
	case 3: pDC->SelectObject(&g_PenSelectColor3); break;
	case 4: pDC->SelectObject(&g_PenSelectColor4); break;
	case 5: pDC->SelectObject(&g_PenSelectColor5); break;
	default:
		pDC->SelectObject(&g_PenSelectColor5);
	}

}
void g_SelectColorCode2(CDC* pDC, int ColorCount)
{
	switch(ColorCount)
	{

	case 0: pDC->SelectObject(&g_Pen2SelectColor0); break;
	case 1: pDC->SelectObject(&g_Pen2SelectColor1); break;
	case 2: pDC->SelectObject(&g_Pen2SelectColor2); break;
	case 3: pDC->SelectObject(&g_Pen2SelectColor3); break;
	case 4: pDC->SelectObject(&g_Pen2SelectColor4); break;
	case 5: pDC->SelectObject(&g_Pen2SelectColor5); break;
	default:
		pDC->SelectObject(&g_Pen2SelectColor5);
	}

}

void g_SelectColorCode3(CDC* pDC, int ColorCount)
{
	switch (ColorCount)
	{

	case 0: pDC->SelectObject(&g_Pen2SelectColor5); break;
	case 1: pDC->SelectObject(&g_Pen2SelectColor1); break;
	case 2: pDC->SelectObject(&g_Pen2SelectColor2); break;
	case 3: pDC->SelectObject(&g_Pen2SelectColor3); break;
	case 4: pDC->SelectObject(&g_Pen2SelectColor4); break;
	default:
		pDC->SelectObject(&g_Pen2SelectColor5);
	}

}

void g_SelectThickColorCode(CDC* pDC, int ColorCount)
{
	switch(ColorCount)
	{

	case 0: pDC->SelectObject(&g_PenSelectColor0_obs); break;
	case 1: pDC->SelectObject(&g_PenSelectColor1_obs); break;
	case 2: pDC->SelectObject(&g_PenSelectColor2_obs); break;
	case 3: pDC->SelectObject(&g_PenSelectColor3_obs); break;
	case 4: pDC->SelectObject(&g_PenSelectColor4_obs); break;
	case 5: pDC->SelectObject(&g_PenSelectColor5_obs); break;
	default:
		pDC->SelectObject(&g_PenSelectColor5_obs);
	}

}


void g_SelectThickPenColor(CDC* pDC, int ColorCount)
{
	switch(ColorCount)
	{
	case 0: pDC->SelectObject(&g_ThickPenSelectColor0); break;
	case 1: pDC->SelectObject(&g_ThickPenSelectColor1); break;
	case 2: pDC->SelectObject(&g_ThickPenSelectColor2); break;
	case 3: pDC->SelectObject(&g_ThickPenSelectColor3); break;
	case 4: pDC->SelectObject(&g_ThickPenSelectColor4); break;
	case 5: pDC->SelectObject(&g_ThickPenSelectColor5); break;
	default:
		pDC->SelectObject(&g_PenSelectColor5);
	}
}

void g_SelectBrushColor(CDC* pDC, int ColorCount)
{
	switch(ColorCount)
	{
	case 0: pDC->SelectObject(&g_BrushColor0); break;
	case 1: pDC->SelectObject(&g_BrushColor1); break;
	case 2: pDC->SelectObject(&g_BrushColor2); break;
	case 3: pDC->SelectObject(&g_BrushColor3); break;
	case 4: pDC->SelectObject(&g_BrushColor4); break;
	case 5: pDC->SelectObject(&g_BrushColor5); break;
	default:
		pDC->SelectObject(&g_BrushColor0);
	}
}


void g_SelectSuperThickPenColor(CDC* pDC, int ColorCount)
{
	switch(ColorCount)
	{

	case 0: pDC->SelectObject(&g_SuperThickPenSelectColor0); break;
	case 1: pDC->SelectObject(&g_SuperThickPenSelectColor1); break;
	case 2: pDC->SelectObject(&g_SuperThickPenSelectColor2); break;
	case 3: pDC->SelectObject(&g_SuperThickPenSelectColor3); break;
	case 4: pDC->SelectObject(&g_SuperThickPenSelectColor4); break;
	case 5: pDC->SelectObject(&g_SuperThickPenSelectColor5); break;
	default:
		pDC->SelectObject(&g_SuperThickPenSelectColor5);
	}

}



CTLiteView::CTLiteView()
{
	m_DislayedDemandType = 0;
	m_bShowFixedDetectorLocation = true;
	m_bShowSpeedSensorLocation = true;
	m_DislayReferenceLineNo = false;

	m_bShowWalkLinksOnly = false;
	m_MoveLayerNo = 0;
	m_bShowCompleteTrajectory = false;
	m_bShowAllCompleteTrajectory = false;
	m_bShowTransitLinksOnly = false;
	m_bShowWalkLinksOnly = false;
	m_bShowProhibitedMovements = false;
	m_bShowTransitAccessibility  = false;
	m_bShowTop10ODOnly = false;

	m_bUpdateLinkAttributeBasedOnType  = false;
	m_LinkTextFontSize = 12;
	m_NodeDisplayBoundarySize = 200;

	m_bNetworkCooridinateHints = false;
	bShowVehiclesWithIncompleteTrips = false;

	m_msStatus = 0;

	if(theApp.m_VisulizationTemplate == e_train_scheduling) 
		m_bLineDisplayConditionalMode = true;

	if(theApp.m_VisulizationTemplate == e_traffic_assignment) 
		m_bLineDisplayConditionalMode = false;

	m_link_display_mode = link_display_mode_band; // 

	m_NodeTypeFaceName      = "Arial";

	m_bShowAVISensor = true;
	m_bShowSensorDecription = false;
	m_bShowSensorMatchedLink = false;
	m_bShowODDemandVolume = false;
	m_bHighlightSubareaLinks = true;
	m_bHighlightSubareaBoundaryLinks = false;
	m_bShowConnector = true;
	m_bHighlightActivityLocation = false;
	isCreatingSubarea = false;
	isFinishSubarea = false;	
	m_ViewID = g_ViewID++;
	m_Resolution = 1;

	m_ShowLinkTextMode  = link_display_none;
	m_ShowMovementTextMode  = movement_display_none;
	m_MovmentTextSize = 1.0;
	m_LinkTextSize = 1.0;

	m_bMouseDownFlag = false;
	m_ShowAllPaths = true;

	m_OriginOnBottomFlag = -1;

	//	RGB(102,204,204);

	m_ToolMode = select_feature_tool;
	m_bMoveDisplay = false;
	m_bMoveImage = false;
	m_bMoveSensorMap = false;
	m_bMoveNetwork = false;
	m_bShowImage = true;
	m_bShowGrid  = false;
	m_bShowLinkArrow = false;
	m_bShowNode = true;
	m_ShowNodeTextMode = node_display_node_number;
	m_ShowGPSTextMode = GPS_display_none;

	m_ShowLinkTextMode = link_display_none;
	m_ShowMovementTextMode  = movement_display_none;

	m_bShowVehicleNumber = false;
	m_bShowSelectedVehicleOnly = false;
	m_bShowLinkType  = true;

	m_Origin.x = 0;
	m_Origin.y = 0;

	m_VehicleSize = 1;
	m_bShowTransit = false;

	m_SelectFromNodeNumber = 0;
	m_SelectToNodeNumber = 0;
	g_ViewList.push_back(this);

}
CTLiteView::~CTLiteView()
{
	std::list<CTLiteView*>::iterator iView = g_ViewList.begin ();
	while (iView != g_ViewList.end())
	{
		if((*iView) == this)
		{
			g_ViewList.erase (iView);  // remove the Viewument to be deleted
			break;

		}
		iView++;
	}

}

BOOL CTLiteView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTLiteView drawing
BOOL CTLiteView::OnPreparePrinting(CPrintInfo* pInfo)
{
   return CView::OnPreparePrinting(pInfo);
}

////////////////////////////////////////////////////////////////////////////////
// OnPrint
////////////////////////////////////////////////////////////////////////////////
void CTLiteView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{

   CView::OnPrint(pDC, pInfo);

   // Reset the resolution and background color
}

void CTLiteView::DrawBitmap(CDC *pDC, CPoint point,UINT nIDResource )
{

	CBitmap bitmapImage;
	//load bitmap from resource
	bitmapImage.LoadBitmap(nIDResource);

	BITMAP bmpInfo;
	CSize bitSize;
	//get size of bitmap
	bitmapImage.GetBitmap(&bmpInfo);
	bitSize = CSize(bmpInfo.bmWidth, bmpInfo.bmHeight);

	CDC* pwholeWndDC;
	pwholeWndDC=pDC;

	CImageList imageList;
	//create ImageList
	imageList.Create(bitSize.cx, bitSize.cy, ILC_COLOR|ILC_MASK, 1, 1);
	//select color yellow is transparent color
	imageList.Add(&bitmapImage, RGB(255,255,0));// Add bitmap to CImageList.

	point.x-=bitSize.cx /2;
	point.y-=bitSize.cy /2;


	//drawing the first bitmap of imagelist Transparently
	//imageList.SetBkColor(RGB(0,0,0));
	imageList.Draw(pwholeWndDC,0, point, ILD_TRANSPARENT );

}
void CTLiteView::OnDraw(CDC* pDC)
{
	//	FollowGlobalViewParameters(this->m_ViewID);

	// Custom draw on top of memDC
	CTLiteDoc* pDoc = GetDocument();
	pDoc->m_Doc_Resolution = m_Resolution;
   
CRect rectClient(0,0,0,0);
	// Get window bounds
   if(pDC->IsPrinting())
   {
      CSize size = pDC->GetWindowExt();
      rectClient.bottom = -size.cy / 2;
      rectClient.left = -size.cx / 2;
      rectClient.top = size.cy / 2;
      rectClient.right = size.cx / 2;

	CBrush brush;

	if (!brush.CreateSolidBrush(theApp.m_BackgroundColor))
		return;

	brush.UnrealizeObject();
	pDC->FillRect(rectClient, &brush);
	DrawObjects(pDC);

   }
   else
   {
   	GetClientRect(&rectClient);
     
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rectClient.Width(),
		rectClient.Height());
	memDC.SelectObject(&bmp);


	CBrush brush;

	if (!brush.CreateSolidBrush(theApp.m_BackgroundColor))
		return;

	brush.UnrealizeObject();
	memDC.FillRect(rectClient, &brush);

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	if(pDoc->m_BackgroundBitmapLoaded && pMainFrame->m_bShowLayerMap[layer_background_image])
	{
		pDoc->m_ImageX2  = pDoc->m_ImageX1+ pDoc->m_ImageWidth * pDoc->m_ImageXResolution;
		pDoc->m_ImageY2  = pDoc->m_ImageY1+ pDoc->m_ImageHeight * pDoc->m_ImageYResolution * pDoc->m_OriginOnBottomFlag ;

		GDPoint IMPoint1;
		IMPoint1.x = pDoc->m_ImageX1;
		IMPoint1.y = pDoc->m_ImageY2;

		GDPoint IMPoint2;
		IMPoint2.x = pDoc->m_ImageX2;
		IMPoint2.y = pDoc->m_ImageY1;


		CPoint point1 = NPtoSP(IMPoint1);
		CPoint point2 = NPtoSP(IMPoint2);

		pDoc->m_BackgroundBitmap.StretchBlt(memDC,point1.x,point1.y,point2.x-point1.x,abs(point2.y-point1.y),SRCCOPY);


		if(pDoc->m_bPointA_Initialized )
		{
			GDPoint pt;
			pt.x = pDoc->m_PointA_x;
			pt.y = pDoc->m_PointA_y;

			CPoint point = NPtoSP(pt);
			int size  = 5;
			memDC.Ellipse (point.x - size, point.y + size,
				point.x + size, point.y - size);

			point.y -= size / 2;
			memDC.TextOut(point.x , point.y , _T("A"));
		}

		if(pDoc->m_bPointB_Initialized )
		{
			GDPoint pt;
			pt.x = pDoc->m_PointB_x;
			pt.y = pDoc->m_PointB_y;

			CPoint point = NPtoSP(pt);
			int size  = 5;
			memDC.Ellipse (point.x - size, point.y + size,
				point.x + size, point.y - size);

			point.y -= size / 2;
			memDC.TextOut(point.x , point.y , _T("B"));
		}

	}


	DrawObjects(&memDC);

	pDC->BitBlt(0, 0, rectClient.Width(), rectClient.Height(), &memDC, 0,
		0, SRCCOPY);

	ReleaseDC(pDC);

   }	// TODO: add draw code for native data here
}


// CTLiteView diagnostics

#ifdef _DEBUG
void CTLiteView::AssertValid() const
{
	CView::AssertValid();
}

void CTLiteView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTLiteDoc* CTLiteView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTLiteDoc)));
	return (CTLiteDoc*)m_pDocument;
}
#endif //_DEBUG


// CTLiteView message handlers
void CTLiteView::DrawObjects(CDC* pDC)
{

	CTLiteDoc* pDoc = GetDocument();
		CPen pen_moe[MAX_LOS_SIZE];
		CBrush brush_moe[MAX_LOS_SIZE];

		for(int los = 0; los < MAX_LOS_SIZE-1; los++)
		{
			pen_moe[los].CreatePen (PS_SOLID, 1, pDoc->m_colorLOS[los]);
			brush_moe[los].CreateSolidBrush (pDoc->m_colorLOS[los]);
		}
	// step 1: set the objects in the network to the screen 

	std::list<DTANode*>::iterator iNode;

	if(!pDoc->m_bFitNetworkInitialized  )
	{
		FitNetworkToScreen();
		pDoc->m_bFitNetworkInitialized = true;

	}

	pDC->SetBkMode(TRANSPARENT);

	CRect ScreenRect;
	GetClientRect(ScreenRect);

	// step 2: draw grids

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	if(pMainFrame->m_bShowLayerMap[layer_grid] == true)
	{
		pDC->SelectObject(&g_GridPen);
		pDC->SetTextColor(RGB(255,228,181));


		// get the closest power 10 number
		m_GridResolution = g_FindClosestYResolution(ScreenRect.Width ()/m_Resolution/10.0f);

		if(pDoc->m_bUseMileVsKMFlag==false && pDoc->m_LongLatFlag==false ) //use km
		{
			m_GridResolution = 1;
		}

		int LeftX  = int(SPtoNP(ScreenRect.TopLeft()).x)-1;

		if(m_GridResolution>1)
			LeftX = LeftX- LeftX%int(m_GridResolution);

		for(double x = LeftX; x<= int(SPtoNP(ScreenRect.BottomRight ()).x)+1; x+=m_GridResolution)
		{
			fromp.x = x;
			fromp.y = SPtoNP(ScreenRect.TopLeft()).y;

			top.x = x;
			top.y = SPtoNP(ScreenRect.BottomRight()).y;

			FromPoint = NPtoSP(fromp);
			ToPoint = NPtoSP(top);

			pDC->SelectObject(&g_GridPen);

			pDC->MoveTo(FromPoint);
			pDC->LineTo(ToPoint);

			CString str;

			if(m_GridResolution<1)
				str.Format ("%.2f",x);
			else
				str.Format ("%.0f",x);

			pDC->TextOut(FromPoint.x,ScreenRect.TopLeft().y+10,str);
		}

		int BottomY  = int(SPtoNP(ScreenRect.BottomRight()).y)-1;

		if(m_GridResolution>1)
			BottomY = BottomY- BottomY%int(m_GridResolution);

		for(double y = BottomY; y<= int(SPtoNP(ScreenRect.TopLeft ()).y)+1; y+=m_GridResolution)
		{

			fromp.x =  SPtoNP(ScreenRect.TopLeft()).x ;
			fromp.y = y;

			top.x = SPtoNP(ScreenRect.BottomRight()).x ;
			top.y = y ;

			FromPoint = NPtoSP(fromp);
			ToPoint = NPtoSP(top);

			pDC->MoveTo(FromPoint);
			pDC->LineTo(ToPoint);

			CString str;
			if(m_GridResolution<1)
				str.Format ("%.2f",y);
			else
				str.Format ("%.0f",y);

			pDC->TextOut(ScreenRect.TopLeft().x+10,FromPoint.y,str);

		}
	}

	if (pMainFrame->m_bShowLayerMap[layer_reference_line])
	{

		// draw generic link layer
		std::list<DTALine*>::iterator iLine;

		if (pDoc->m_DTALineSet.size() > 0)
		{
			pDC->SelectObject(&g_BlackBrush);
			pDC->SetTextColor(RGB(255, 255, 0));
			pDC->SelectObject(&g_PenReference);
			pDC->SetBkColor(RGB(0, 0, 0));

			for (iLine = pDoc->m_DTALineSet.begin(); iLine != pDoc->m_DTALineSet.end(); iLine++)
			{

				for (unsigned int i = 0; i < (*iLine)->m_ShapePoints.size(); i++)
				{
					CPoint point = NPtoSP((*iLine)->m_ShapePoints[i]);
					if (i == 0)
						pDC->MoveTo(point);
					else
						pDC->LineTo(point);

				}

				if (m_ShowLinkTextMode == link_display_link_id_speed_sensorid)
				{

					int node_size = max(2, int(pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution));

					node_size = min(50, node_size);

					int LinkTextFontSize = 10;
					LinkTextFontSize = min(50, m_LinkTextFontSize)*m_LinkTextSize;



					// create rotation font
					CFont link_text_font;

					LOGFONT lf;
					memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
					lf.lfHeight = LinkTextFontSize;        // request a 12-pixel-height font

					int last_shape_point_id = (*iLine)->m_ShapePoints.size() - 1;
					float DeltaX = (*iLine)->m_ShapePoints[last_shape_point_id].x - (*iLine)->m_ShapePoints[0].x;
					float DeltaY = (*iLine)->m_ShapePoints[last_shape_point_id].y - (*iLine)->m_ShapePoints[0].y;

					float sqr_value = (DeltaX)*(DeltaX)+(DeltaY)*(DeltaY);
					float screen_distance = sqrt(sqr_value) * m_Resolution;

					float theta = atan2(DeltaY, DeltaX);

					float theta_deg = fmod(theta / PI * 180, 360);

					if (theta_deg < -90)
						theta_deg += 180;

					if (theta_deg > 90)
						theta_deg -= 180;

					lf.lfEscapement = theta_deg * 10;
					strcpy(lf.lfFaceName, "Arial");

					link_text_font.CreateFontIndirect(&lf);

					pDC->SelectObject(&link_text_font);

					// select text string to be displayed

					bool with_text = false;
					CString str_text, str_reference_text;
					str_text.Format("%s", (*iLine)->m_LineID.c_str());

					if (screen_distance > 50)
						with_text = true;

					// after all the above 3 conditions, show test now. 
					if (with_text)
					{
						GDPoint mid_point = (*iLine)->GetRelativePosition(0.6);
						
						CPoint TextPoint = NPtoSP(mid_point);
						pDC->SetBkColor(RGB(0, 0, 0));

						if (str_text.GetLength() > 0)  // reference text with different color4
						{
							pDC->SetTextColor(RGB(0, 0, 255));
							pDC->TextOut(TextPoint.x, TextPoint.y + m_LinkTextFontSize, str_text);
						}

					}

				}

			}

		}

	}  // reference line layer

	// step 2: select font and color for node drawing, and compute the bandwidth for links
	CFont node_font;  // local font for nodes. dynamically created. it is effective only inside this function. if you want to pass this font to the other function, we need to pass the corresponding font pointer (which has a lot of communication overheads)
	int node_size = max(2,int(pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution));

	node_size = min(50,node_size);

	int NodeTypeSize = pDoc->m_NodeTextDisplayRatio;
	int nFontSize =  max(node_size * NodeTypeSize, 10);


	node_font.CreatePointFont(nFontSize, m_NodeTypeFaceName);

	pDC->SelectObject(&node_font);

	COLORREF oldColor = pDC->SetTextColor(RGB(255,255,0));

	UINT oldAlign = pDC->SetTextAlign(TA_CENTER);

	// Text height
	TEXTMETRIC tm;
	memset(&tm, 0, sizeof TEXTMETRIC);
	pDC->GetOutputTextMetrics(&tm);

	//test the display width of a lane, if greather than 1, then band display mode

	if (pMainFrame->m_bShowLayerMap[layer_link] == true || m_bShowReferenceNetwork == true)  // show link layer either one of 2 conditions are met
	{


		if( pDoc->m_LinkMOEMode == MOE_vehicle || pDoc->m_LinkMOEMode== MOE_queuelength  || pDoc->m_LinkMOEMode == MOE_bottleneck)
				m_link_display_mode = link_display_mode_line;
		else if( pDoc->m_LinkMOEMode == MOE_volume ||pDoc->m_LinkMOEMode ==  MOE_density ||  pDoc->m_LinkMOEMode ==  MOE_speed ||  pDoc->m_LinkMOEMode ==  MOE_emissions || pDoc->m_LinkMOEMode == MOE_impact )
				m_link_display_mode = link_display_mode_band;
		else if(pDoc->m_LinkMOEMode == MOE_none)
		{

			if(m_bLineDisplayConditionalMode || pDoc->m_LaneWidthInFeet * pDoc->m_UnitFeet*m_Resolution * 5 > 0.1f) // 5 lanes
				m_link_display_mode = link_display_mode_line;
			else
				m_link_display_mode = link_display_mode_band;
	
		}
			

		// draw links
		std::list<DTALink*>::iterator iLink;
		std::list<DTALink*>::iterator iLinkDisplay;

		CPoint FromPoint,ToPoint;
		CRect link_rect;

		CPen pen_freeway, pen_ramp, pen_arterial, pen_connector, pen_transit, pen_walking, pen_parking_and_ride;
		CBrush brush_freeway, brush_ramp, brush_arterial, brush_connector, brush_walking, brush_parking_and_ride;

		pen_freeway.CreatePen (PS_SOLID, 1, theApp.m_FreewayColor);
		brush_freeway.CreateSolidBrush (theApp.m_FreewayColor);

		pen_ramp.CreatePen (PS_SOLID, 1, theApp.m_RampColor);
		brush_ramp.CreateSolidBrush (theApp.m_RampColor);

		pen_arterial.CreatePen (PS_SOLID, 1, theApp.m_ArterialColor);
		brush_arterial.CreateSolidBrush (theApp.m_ArterialColor);

		pen_connector.CreatePen (PS_SOLID, 1, theApp.m_ConnectorColor);
		brush_connector.CreateSolidBrush (theApp.m_ConnectorColor);

		pen_transit.CreatePen (PS_SOLID, 3, theApp.m_TransitColor);

		pen_walking.CreatePen (PS_SOLID, 4, theApp.m_WalkingColor);
		pen_parking_and_ride.CreatePen(PS_SOLID, 3, theApp.m_ParkingandRideColor);

		
		brush_walking.CreateSolidBrush (theApp.m_WalkingColor);
		brush_parking_and_ride.CreateSolidBrush(theApp.m_ParkingandRideColor);

		// recongenerate the lind band width offset only when chaning display mode or on volume mode
		if(	pDoc -> m_PrevLinkMOEMode != pDoc -> m_LinkMOEMode || 
			pDoc->m_LinkMOEMode == MOE_density || 
			pDoc->m_LinkMOEMode == MOE_impact ||	
			pDoc->m_LinkMOEMode == MOE_volume || 
			pDoc->m_LinkMOEMode == MOE_speed ||
			pDoc->m_LinkMOEMode == MOE_emissions ||
			pDoc->m_LinkMOEMode == MOE_safety) 
		{
			pDoc->GenerateOffsetLinkBand();
		}

		if (pDoc->m_LinkMOEMode == MOE_density || pDoc->m_LinkMOEMode == MOE_volume  || pDoc->m_LinkMOEMode == MOE_speed || pDoc->m_LinkMOEMode == MOE_emissions)   // under volume mode, dynamically generate volume band
		{
			pDoc->m_LinkBandWidthMode = LBW_link_volume;
		}else if (pDoc->m_LinkMOEMode == MOE_impact) 
		{
		
			pDoc->m_LinkBandWidthMode = LBW_congestion_duration; 
		
		}else
		{
			pDoc->m_LinkBandWidthMode = LBW_number_of_lanes;  // e.g. queue
		}

		if(pDoc->m_LinkSet.size() > 0)  // update for a network with links
		{
			pDoc -> m_PrevLinkMOEMode = pDoc -> m_LinkMOEMode;
		}

		// step 3: draw all links
		int min_x, min_y, max_x, max_y;

		pDC->SelectObject(&g_BrushLinkBand);   //default brush  , then MOE, then apply speical conditions for volume and vehicle mode

		//	TRACE("connector layer: %d\n", pMainFrame->m_bShowLayerMap[layer_connector]);


		for (iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
		{

			if( pMainFrame->m_bShowLayerMap[layer_transit] == true && m_bShowTransitLinksOnly )
			{
				if (pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].IsTransit  ()== false)  //show transit only
					continue; 
			}


			if( pMainFrame->m_bShowLayerMap[layer_transit] == true && m_bShowWalkLinksOnly )
			{
				if (pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].IsWalking  ()== false)  //show walking only
					continue; 
			}

			if( !pMainFrame->m_bShowLayerMap[layer_connector] )
			{
				if (pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].IsConnector ())  //hide connectors
					continue; 
			}

			if( !pMainFrame->m_bShowLayerMap[layer_transit] )
			{
				if (pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].IsTransit  () || pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].IsWalking   ())  //hide transit
					continue; 
			}


			// step 1: decide if a link is included in the screen region
			FromPoint = NPtoSP((*iLink)->m_FromPoint);
			ToPoint = NPtoSP((*iLink)->m_ToPoint);

			min_x = min(FromPoint.x,ToPoint.x);
			max_x = max(FromPoint.x,ToPoint.x);

			min_y = min(FromPoint.y,ToPoint.y);
			max_y = max(FromPoint.y,ToPoint.y);

			int size = 1000;
			link_rect.SetRect(min_x-size,min_y-size,max_x+size,max_y+size);

			if(RectIsInsideScreen(link_rect,ScreenRect) == false)  // not inside the screen boundary
				continue;


			// step 4: select color and brush for MOE mode

			//			continue;
			CPen LinkTypePen;


			// step 5: select color and brush according to link MOE
			float value = -1.0f ;

			if( pDoc->m_LinkMOEMode != MOE_none) 
			{

				if ( (*iLink)->m_FromNodeNumber == 78424 && (*iLink)->m_ToNodeNumber == 61158 && g_Simulation_Time_Stamp>=1320) 
				{

					TRACE("trace");

				}

				// dynamically create LOS pen and brush  
				float power =0 ; int LOS = 1;

				if(pDoc->m_LinkMOEMode == MOE_user_defined)
				{ 
					int current_time  =(int)g_Simulation_Time_Stamp;
					if( current_time < (*iLink)->m_LinkMOEArySize )

					{
						power = (*iLink)->m_LinkMOEAry [current_time].UserDefinedValue ;
						value = (*iLink)->m_LinkMOEAry [current_time].UserDefinedValue ;

					}else
					{
						power = 0;
					}
				}else
				{
					power= pDoc->GetLinkMOE((*iLink), pDoc->m_LinkMOEMode,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);
				}

				if(	pDoc->m_LinkMOEMode == MOE_impact)
				{

				int RelativeStartTime = (*iLink)->GetImpactRelativeStartTime(g_Simulation_Time_Stamp,pDoc->m_PrimaryDataSource);
				power = RelativeStartTime*1.0/g_ImpactStudyPeriodInMin*100;

				if(power>100)
					power = 100;

				}

				LOS = pDoc->GetLOSCode(power);

				//draw link as lines
				if(m_link_display_mode == link_display_mode_line )
				{
					if(pDoc->m_LinkMOEMode != MOE_volume) // not volume display
					{
						pDC->SelectObject(&pen_moe[LOS]);
						pDC->SelectObject(&g_BrushLinkBand);   //default brush  , then MOE, then condition with volume
					}else
					{
						pDC->SelectObject(&g_BlackPen);
						pDC->SelectObject(&g_BlackBrush);

					}
				}
				else 
				{// display link as a band so use black pen
					pDC->SelectObject(&g_BlackPen);
					pDC->SelectObject(&brush_moe[LOS]);
				}

			}
			else if(m_bShowLinkType)  // show link type
			{
				//		COLORREF link_color = pDoc->GetLinkTypeColor((*iLink)->m_link_type );
				//		LinkTypePen.CreatePen(PS_SOLID, 1, link_color);
				//		pDC->SelectObject(&LinkTypePen);

				if ( pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsFreeway ())
				{
					pDC->SelectObject(&pen_freeway);
					pDC->SelectObject(&brush_freeway);
				}else if ( pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsArterial())
				{
					pDC->SelectObject(&pen_arterial);
					pDC->SelectObject(&brush_arterial);
				}else if ( pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsRamp())
				{
					pDC->SelectObject(&pen_ramp);
					pDC->SelectObject(&brush_ramp);
				}else if ( pDoc->m_LinkTypeMap[(*iLink)->m_link_type] .IsConnector())
				{
					pDC->SelectObject(&pen_connector);
					pDC->SelectObject(&brush_connector);
				}else if ( pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsTransit())
				{
					pDC->SelectObject(&pen_transit);
					pDC->SelectObject(&g_TransitCrash);
				}else if ( pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsWalking())
				{
					pDC->SelectObject(&pen_walking);
					pDC->SelectObject(&brush_walking);
				}
				else if (pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsParkingAndRide())
				{
					pDC->SelectObject(&pen_parking_and_ride);
					pDC->SelectObject(&brush_parking_and_ride);
				}
				else
				{
					pDC->SelectObject(&pen_arterial);
					pDC->SelectObject(&g_BrushLinkBand);   //default brush
				}

				// special condition for subarea link
				if(m_bHighlightSubareaLinks &&(*iLink)->m_bIncludedinSubarea && pDoc->m_SubareaShapePoints.size()>=3)
					pDC->SelectObject(&g_SubareaLinkPen);

				if(m_bHighlightSubareaBoundaryLinks &&(*iLink)->m_bIncludedinBoundaryOfSubarea  && pDoc->m_SubareaShapePoints.size()>=3)
					pDC->SelectObject(&g_SubareaBoundaryLinkPen);

			}else  // default arterial model
			{
				pDC->SelectObject(&pen_arterial);
				pDC->SelectObject(&g_BrushLinkBand);   //default brush
			}

			//special condition 1: vehicle mode

			if( pDoc->m_LinkMOEMode == MOE_vehicle)  // when showing vehicles, use black
				pDC->SelectObject(&g_BlackPen);

			//special condition 2:  volume mode
			if (pDoc->m_LinkMOEMode == MOE_volume)   // under volume mode, dynamically generate volume band
				pDC->SelectObject(&g_BrushLinkBandVolume);   //default brush

			// special condition 3: when a link is selected

			//outside subarea
			if((*iLink)->m_DisplayLinkID>=0 && pMainFrame->m_bShowLayerMap[layer_path]==true)
			{
				g_SelectThickPenColor(pDC,(*iLink)->m_DisplayLinkID);
				pDC->SetTextColor(RGB(255,0,0));
			}
			else if(m_bHighlightSubareaLinks &&(*iLink)->m_bIncludedinSubarea)
			{
				g_SelectThickPenColor(pDC,1);
				pDC->SetTextColor(RGB(255,0,0));
			}else if(m_bHighlightSubareaBoundaryLinks &&(*iLink)->m_bIncludedinBoundaryOfSubarea  && pDoc->m_SubareaShapePoints.size()>=3)
			{
				pDC->SelectObject(&g_SubareaBoundaryLinkPen);
			}else if  ((*iLink)->m_LinkNo == pDoc->m_SelectedLinkNo)
			{
				g_SelectThickPenColor(pDC,0);
				pDC->SetTextColor(RGB(255,0,0));
			}else 
				pDC->SetTextColor(RGB(255,228,181));


			// special condition 4: when a link is a reference link
			if (m_bShowReferenceNetwork)
			{
				if((*iLink)->m_LayerNo == 1)
				  pDC->SelectObject(&g_PenReference);
				if ((*iLink)->m_LayerNo == 2)
					pDC->SelectObject(&g_PenDashBlue);

			}


			if (m_bShowReferenceNetwork == true && pMainFrame->m_bShowLayerMap[layer_link] == false && (*iLink)->m_LayerNo ==0)
			{  //skip the baseline link layer under special conditions 
				continue;
			}


			if (m_DislayedDemandType >= 1)  //show the allowed demand type only
			{
				CString number;
				number.Format("%d", m_DislayedDemandType);

				std::string str_number = GetDocument()->CString2StdString(number);
				if ((*iLink)->m_demand_type_code.find(str_number) == std::string::npos)   // do not find this number
					continue; // do not display 

			}

			if (pMainFrame->m_bShowLayerMap[layer_reference_line] == false)
			{
				if ((*iLink)->m_LayerNo == 1)  //skip reference nodes
					return;
			}


			//step 6: draw link as line or band/bar
			if(m_link_display_mode == link_display_mode_line )  
			{
				// calibration mode, do not show volume
				DrawLinkAsLine((*iLink),pDC);
			}
			else 
			{
				
				if(DrawLinkAsBand((*iLink),pDC,false)==false)
					return;
			}

			CPoint ScenarioPoint;

			if (pMainFrame->m_bShowLayerMap[layer_workzone] == true &&
				((*iLink)->GetImpactedFlag(g_Simulation_Time_Stamp, true) >= 0.01 || (g_Simulation_Time_Stamp <= 0.1 && (*iLink)->GetImpactedFlag(-1, true) >= 0.01)))
			{
				ScenarioPoint = NPtoSP((*iLink)->GetRelativePosition(0.6));  // get relative position of a link 
				DrawBitmap(pDC, ScenarioPoint, IDB_WORKZONE);
			}

			if (pMainFrame->m_bShowLayerMap[layer_ramp] == true &&
				((*iLink)->GetRampImpactedFlag(g_Simulation_Time_Stamp) >= 0.01 || (g_Simulation_Time_Stamp <= 0.1 && (*iLink)->GetRampImpactedFlag(-1) >= 0.01)))
			{
				ScenarioPoint = NPtoSP((*iLink)->GetRelativePosition(0.6));  // get relative position of a link 
				DrawBitmap(pDC, ScenarioPoint, IDB_RAMP);
			}

			if (pMainFrame->m_bShowLayerMap[layer_incident] == true &&
				((*iLink)->GetImpactedFlag(g_Simulation_Time_Stamp, false) >= 0.01 || (g_Simulation_Time_Stamp <= 0.1 && (*iLink)->GetImpactedFlag(-1, false) >= 0.01)))
			{
				ScenarioPoint = NPtoSP((*iLink)->GetRelativePosition(0.6));  // get relative position of a link 
				DrawBitmap(pDC, ScenarioPoint, IDB_INCIDENT);
			}

			if (pMainFrame->m_bShowLayerMap[layer_VMS] == true && ((*iLink)->GetMessageSign(g_Simulation_Time_Stamp) >= 0.1 || (g_Simulation_Time_Stamp == 0 && (*iLink)->MessageSignVector.size() > 0)))
			{
				ScenarioPoint = NPtoSP((*iLink)->GetRelativePosition(0.6));  // get relative position of a link 
				DrawBitmap(pDC, ScenarioPoint, IDB_VMS);
			}
			if (pMainFrame->m_bShowLayerMap[layer_toll] == true )
			{
				ScenarioPoint = NPtoSP((*iLink)->GetRelativePosition(0.6));  // get relative position of a link 
				if(pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsParkingAndRide())
					DrawBitmap(pDC, ScenarioPoint, IDB_PRTOLL);
				else if (pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsBRT())
					DrawBitmap(pDC, ScenarioPoint, IDB_BRTTOLL);
			
				if (((*iLink)->GetTollValue(g_Simulation_Time_Stamp) >= 0.1 || (g_Simulation_Time_Stamp == 0 && (*iLink)->TollVector.size() > 0)))
				DrawBitmap(pDC, ScenarioPoint, IDB_TOLL);
			}
			if ((*iLink)->m_AdditionalCost >= 1 && pMainFrame->m_bShowLayerMap[layer_path] == true)
			{
				ScenarioPoint = NPtoSP((*iLink)->GetRelativePosition(0.6));  // get relative position of a link 
				DrawBitmap(pDC, ScenarioPoint, IDB_LINK_CLOSURE);
			}

			if(pMainFrame->m_bShowLayerMap[layer_ramp] == true)
			{
				if( (*iLink)-> m_bOnRampType == true )
					DrawBitmap(pDC, ScenarioPoint, IDB_ONRAMP);

				if( (*iLink)-> m_bOffRampType == true )
					DrawBitmap(pDC, ScenarioPoint, IDB_OFFRAMP);

				if( (*iLink)-> m_FREEVALSegmentCode ==  FREEVAL_ONR)
					DrawBitmap(pDC, ScenarioPoint, IDB_SEGMENT_ONRAMP);

				if( (*iLink)-> m_FREEVALSegmentCode ==  FREEVAL_OFR )
					DrawBitmap(pDC, ScenarioPoint, IDB_SEGMENT_OFFRAMP);
			}



			if(pMainFrame->m_bShowLayerMap[layer_detector] == true  && (*iLink)->m_bSensorData)  // only during non display mode
			{
				if((*iLink)->m_LinkNo == pDoc->m_SelectedLinkNo)
				{
					pDC->SelectObject(&g_PenSelectColor);
				}else
				{

					pDC->SelectObject(&g_PenSensorColor);
				}		
				int size = 5;


				if ((*iLink)->m_bCountSensorData && m_bShowFixedDetectorLocation)
				{
					size = 5;
				CPoint midpoint = NPtoSP((*iLink)->GetRelativePosition(0.3));

				if ((*iLink)->m_b_invalid_sensor == true)
					pDC->SelectObject(g_InvalidBrushSensor);
				else
					pDC->SelectObject(g_BrushSensor);

				pDC->SelectObject(&g_PenSensorColor);
					pDC->Rectangle(midpoint.x-size, midpoint.y-size, midpoint.x+size, midpoint.y+size);
				}

				if ((*iLink)->m_bSpeedSensorData && m_bShowSpeedSensorLocation)
				{
					size = 3;
				CPoint midpoint = NPtoSP((*iLink)->GetRelativePosition(0.5));

					pDC->SetBkMode(TRANSPARENT);
					pDC->SelectObject(g_BrushSpeedSensor);
					pDC->SelectObject(&g_PenSpeedSensorColor);
					pDC->Ellipse (midpoint.x-size, midpoint.y-size, midpoint.x+size, midpoint.y+size);
				}
			}

			if (pDoc->m_LinkMOEMode == MOE_volume && (*iLink)->GetSensorLinkHourlyVolume(g_Simulation_Time_Stamp) > 1)   // draw provided AADT
			{
				pDC->SelectObject(&g_BrushLinkReference);   //reference brush
				pDC->SelectObject(&g_PenLinkReference);   //reference pen
				DrawLinkAsBand((*iLink),pDC,true);  // true: second band as observation
			}

			//step 7: show link labels
			if( m_ShowLinkTextMode != link_display_none )
			{

				int node_size = max(2,int(pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution));

				node_size = min(50,node_size);

				int LinkTextFontSize =   10;
				LinkTextFontSize = min(50,m_LinkTextFontSize)*m_LinkTextSize;


				float sqr_value = (FromPoint.x - ToPoint.x)*(FromPoint.x - ToPoint.x) + (FromPoint.y - ToPoint.y)*(FromPoint.y - ToPoint.y);
				float screen_distance = sqrt( sqr_value) ;

				// create rotation font
				CFont link_text_font;

				LOGFONT lf;
				memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
				lf.lfHeight = LinkTextFontSize;        // request a 12-pixel-height font

				int last_shape_point_id = (*iLink) ->m_ShapePoints .size() -1;
				float DeltaX = (*iLink)->m_ShapePoints[last_shape_point_id].x - (*iLink)->m_ShapePoints[0].x;
				float DeltaY = (*iLink)->m_ShapePoints[last_shape_point_id].y - (*iLink)->m_ShapePoints[0].y;

				float theta = atan2(DeltaY, DeltaX);

				float theta_deg = fmod(theta/PI*180,360);

				if(theta_deg < -90)
					theta_deg += 180;

				if(theta_deg > 90)
					theta_deg -= 180;

				lf.lfEscapement = theta_deg*10;
				strcpy(lf.lfFaceName, "Arial");       

				link_text_font.CreateFontIndirect(&lf);

				pDC->SelectObject(&link_text_font);

				// select text string to be displayed

				bool with_text = false;
				CString str_text, str_reference_text;
				str_text.Format (""); 

				if(screen_distance > 50)
					with_text = true;


				// show text condition 1: street name
				// show text condition 2: crash rates
				if( m_ShowLinkTextMode == link_display_number_of_crashes && (*iLink)->m_number_of_all_crashes >= 0.0001)
				{
					str_text.Format ("%6.2f",(*iLink)->m_number_of_all_crashes );
					with_text = true;
				}


				if(m_ShowLinkTextMode == link_display_street_name)
				{
					if((*iLink)->m_Name.length () > 0 && (*iLink)->m_Name!="(null)"  && screen_distance > 100 )
					{
						str_text = (*iLink)->m_Name.c_str ();
						with_text = true;
					}else
					{
						with_text = false;
					}
				}



				switch (m_ShowLinkTextMode)
				{
				case link_display_from_id_to_id: 
					str_text.Format ("%d->%d", (*iLink)->m_FromNodeNumber , (*iLink)->m_ToNodeNumber ); break;

				case link_display_link_id: 
					str_text.Format ("%d", (*iLink)->m_LinkID  ); break;

				case link_display_speed_sensor_id: 
					str_text.Format ("%s", (*iLink)->m_SpeedSensorID.c_str ()); break;

				case link_display_orientation_code:
					str_text.Format("%s", (*iLink)->m_orientation_code.c_str()); break;

				case link_display_loop_code:
					str_text.Format("%s", (*iLink)->m_loop_code.c_str()); break;
				
				case link_display_demand_type_code:
					str_text.Format("%s", (*iLink)->m_demand_type_code.c_str()); break;

				case  link_display_speed_limit_:
					str_text.Format ("%.1f",(*iLink)->m_SpeedLimit ); break;
				case link_display_length_:
					str_text.Format ("%.3f",(*iLink)->m_Length  ); break;
	
					break;
				case  link_display_free_flow_travel_time_in_min:
					str_text.Format ("%.2f",(*iLink)->m_FreeFlowTravelTime   ); break;
				case  link_display_free_flow_travel_time_in_hour:
					str_text.Format ("%.3f",(*iLink)->m_FreeFlowTravelTime/60.0   ); break;

				case link_display_saturation_flow_rate:
					str_text.Format ("%.0f",(*iLink)->m_Saturation_flow_rate_in_vhc_per_hour_per_lane); break;
				case link_display_number_of_lanes:
					str_text.Format ("%d",(*iLink)->m_NumberOfLanes ); break;						

				case link_display_number_of_left_turn_lanes:

					if((*iLink)->m_NumberOfLeftTurnLanes>=1)
						str_text.Format ("%d",(*iLink)->m_NumberOfLeftTurnLanes  );

					break;						

				case link_display_number_of_right_turn_lanes:

					if((*iLink)->m_NumberOfRightTurnLanes >=1)
					{
						str_text.Format ("%d",(*iLink)->m_NumberOfRightTurnLanes  );
					}
					break;		


				case link_display_length_of_left_turn_lanes:
					if((*iLink)->m_NumberOfLeftTurnLanes>=1)
						str_text.Format ("%.5f",(*iLink)->m_LeftTurnLaneLength  ); 

					break;						

				case link_display_length_of_right_turn_lanes:

					if((*iLink)->m_RightTurnLaneLength>=1)
						str_text.Format ("%.5f",(*iLink)->m_RightTurnLaneLength  ); break;		


				case link_display_lane_capacity_per_hour:

					str_text.Format ("%.0f",(*iLink)->m_LaneCapacity); break;

				case link_display_link_capacity_per_hour:
					str_text.Format ("%.0f",(*iLink)->m_LaneCapacity*(*iLink)->m_NumberOfLanes  ); break;


				case link_display_BPR_alpha_term:
					str_text.Format ("%.4f",(*iLink)->m_BPR_alpha_term   ); break;

				case link_display_BPR_beta_term:
					str_text.Format ("%.4f",(*iLink)->m_BPR_beta_term    ); break;

				case link_display_link_grade:
					str_text.Format ("%.2f",(*iLink)->m_Grade  ); break;

				case link_display_jam_density_in_vhc_pmpl:
					str_text.Format ("%.0f",(*iLink)->m_Kjam   ); break;

				case link_display_wave_speed_in_mph:
					str_text.Format ("%.0f",(*iLink)->m_Wave_speed_in_mph  ); break;

				case link_display_link_type_in_text:
					str_text.Format ("%s",pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].link_type_name.c_str ()  ); break;

				case link_display_link_type_in_number:
					str_text.Format ("%d", (*iLink)->m_link_type  ); break;

				case link_display_internal_link_id:
					str_text.Format ("%d",(*iLink)->m_LinkNo   ); break;

				case link_display_link_id_speed_sensorid:

					if ((*iLink)->m_SpeedSensorID.size() >= 1)
					{
						str_text.Format("%s", (*iLink)->m_SpeedSensorID.c_str());
						with_text = true;
					}
					

					break;

				case link_display_volume_over_capacity_ratio:
					str_text.Format ("%.2f",(*iLink)->m_volume_over_capacity_ratio    ); break;

				case link_display_LevelOfService:
					str_text.Format ("%s",(*iLink)->m_LevelOfService    ); break;

				case link_display_avg_waiting_time_on_loading_buffer:
					if( (*iLink)->m_avg_waiting_time_on_loading_buffer > 0.1)
						str_text.Format ("%.1f",(*iLink)->m_avg_waiting_time_on_loading_buffer   ); break;

				case link_display_avg_simulated_speed_mph:
					str_text.Format ("%.0f",(*iLink)->m_avg_simulated_speed    ); break;

				case link_display_avg_simulated_speed_kmph:
					str_text.Format ("%.0f",(*iLink)->m_avg_simulated_speed*1.6093   ); break;

				case link_display_total_sensor_link_volume:

					if((*iLink)->m_total_sensor_link_volume >= 1)
						str_text.Format ("%.0f",(*iLink)->m_total_sensor_link_volume     );
					break;

				case link_display_total_sensor_vs_simulated_link_volume:
					if((*iLink)->m_total_sensor_link_volume >=1)
						str_text.Format ("%.0f vs. %.0f",(*iLink)->m_total_sensor_link_volume , (*iLink)->m_total_link_volume    ); 

					break;


				case link_display_total_link_volume:
					str_text.Format("%.0f", (*iLink)->m_total_link_volume);

					break;

				case link_display_total_link_count_error:
					if((*iLink)->m_total_sensor_link_volume >=1)
						str_text.Format ("%.0f",(*iLink)->m_total_sensor_link_volume - (*iLink)->m_total_link_volume    ); 

					break;

				case link_display_simulated_AADT:
					if((*iLink)->m_simulated_AADT >=1)
						str_text.Format ("%.0f",(*iLink)->m_simulated_AADT     ); 
					break;


				case link_display_observed_peak_hourly_volume:
					if((*iLink)->m_observed_peak_hourly_volume >=1)
						str_text.Format ("%d",(*iLink)->m_observed_peak_hourly_volume     ); 
					break;

				case link_display_avg_delay:
					if((*iLink)->m_total_delay >=1 && (*iLink)->m_total_delay/ max(1,(*iLink)->m_total_link_volume) >= 0.1 && pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsConnector () == false)
						str_text.Format ("%.1f",(*iLink)->m_total_delay/ max(1,(*iLink)->m_total_link_volume)   );
					break;


				case link_display_total_delay:
					if((*iLink)->m_total_delay>=1 && pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsConnector () == false)
						str_text.Format ("%.0f",(*iLink)->m_total_delay/60.0   );

					break;


				case link_display_total_assigned_link_volume:
					if((*iLink)->m_total_assigned_link_volume >=1)
						str_text.Format ("%.0f",(*iLink)->m_total_assigned_link_volume   );

					break;

				case link_display_total_incomplete_link_volume:
					if((*iLink)->m_total_link_volume_of_incomplete_trips >=1)
						str_text.Format ("%.0f",(*iLink)->m_total_link_volume_of_incomplete_trips   );

					break;
				case link_display_avg_travel_time:
					str_text.Format ("%.1f",(*iLink)->GetAggregatedSimulatedTravelTime(pDoc->m_DemandLoadingStartTimeInMin , pDoc->m_DemandLoadingEndTimeInMin - pDoc->m_DemandLoadingStartTimeInMin)  );
					break;



				case link_display_time_dependent_link_volume:

					pDoc->GetLinkMOE((*iLink), MOE_volume,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);

					value = max(0,value);

					str_text.Format ("%.0f",value);
					break;

				case link_display_time_dependent_lane_volume:

					pDoc->GetLinkMOE((*iLink), MOE_volume,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);
					str_text.Format ("%.0f",value/max(1,(*iLink)->m_NumberOfLanes) );
					break;



				case link_display_time_dependent_speed_mph:
					pDoc->GetLinkMOE((*iLink), MOE_speed,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);
					str_text.Format ("%.0f",value);
					break;
				case link_display_time_dependent_speed_kmph:
					pDoc->GetLinkMOE((*iLink), MOE_speed,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);
					str_text.Format ("%.0f",value*1.6093);
					break;

				case link_display_time_dependent_density:
					pDoc->GetLinkMOE((*iLink), MOE_density,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);

					if(value > 0.1)
					{
						str_text.Format ("%.0f",value);
					}
					break;

				case link_display_time_dependent_queue_length:
					pDoc->GetLinkMOE((*iLink), MOE_queue_length,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);

					if(value>0.01)
					{
						str_text.Format ("%.1f%%",value*100);
					}
					break;


				case link_display_time_dependent_congestion_duration:
					value = (*iLink)->GetImpactDuration ((int)g_Simulation_Time_Stamp, pDoc->m_PrimaryDataSource );

					if(value>0.01)
					{
						str_text.Format ("%.0f min",value);
					}
					break;

					with_text = true;
				case link_display_time_dependent_congestion_start_time:
					value = (*iLink)->GetImpactRelativeStartTime  ((int)g_Simulation_Time_Stamp,pDoc->m_PrimaryDataSource );

					if(value <9999)
					{
						str_text.Format ("%s", pDoc->GetTimeStampString24HourFormat(g_Simulation_Time_Stamp+value));
					}
					break;

					with_text = true;
				}
				if(pDoc->m_bShowCalibrationResults && pDoc->m_LinkMOEMode == MOE_volume)  // special conditions with calibration mode
				{
					if((*iLink)->m_bSensorData )
					{
						str_text.Format ("%.0f",value);
						float percentage_error = (value- (*iLink)->m_ReferenceFlowVolume) / max(1,(*iLink)->m_ReferenceFlowVolume)*100;
						//									str_reference_text.Format ("%.0f, %.0f%%",(*iLink)->m_ReferenceFlowVolume,percentage_error);
						str_reference_text.Format ("%.0f",(*iLink)->m_ReferenceFlowVolume);

						with_text = true;

					}else
					{  // do not show data without sensors
						with_text = false; 
					}

				}


				// after all the above 3 conditions, show test now. 
				if(with_text)
				{
					CPoint TextPoint = NPtoSP((*iLink)->GetRelativePosition(0.3));
					pDC->SetBkColor(RGB(0,0, 0));

					if((*iLink)->m_LinkNo == pDoc->m_SelectedLinkNo)
					{
						pDC->SetTextColor(RGB(255,0,0));
					}else
					{
						pDC->SetTextColor(RGB(0,0,0));
					}


					pDC->TextOut(TextPoint.x,TextPoint.y - m_LinkTextFontSize*1.5, str_text);

					if(str_reference_text.GetLength () > 0 )  // reference text with different color4
					{
						pDC->SetTextColor(RGB(128, 255, 255));
						pDC->TextOut(TextPoint.x,TextPoint.y+m_LinkTextFontSize, str_reference_text);
					}

				}

				// step 8:  show location of scenario/incident /work zone/ toll: link-based objects
				pDC->SetBkColor(RGB(0, 0, 0));

				//************************************

				// step 9: draw sensor (based on a link)
			}
		}
	}

	if(pMainFrame->m_bShowLayerMap[layer_link_MOE] == true && (theApp.m_VisulizationTemplate == e_train_scheduling))
	{	std::list<DTALink*>::iterator iLink;

	for (iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
	{
		DrawLinkTimeTable((*iLink),pDC,0);
	}
	}


	pDC->SelectObject(&node_font);

	// step 10: draw shortest path
	int i;
	/*
	unsigned int iPath;

	if(m_ShowAllPaths)
	{
	for (iPath = 0; iPath < pDoc->m_PathDisplayList.size(); iPath++)
	{
	g_SelectThickPenColor(pDC,0);

	for (i=0 ; i<pDoc->m_PathDisplayList[iPath].m_LinkSize; i++)
	{
	DTALink* pLink = pDoc->m_LinkNoMap[pDoc->m_PathDisplayList[iPath].m_LinkVector[i]];
	if(pLink!=NULL)
	{
	CPoint FromPoint = NPtoSP(pLink->m_FromPoint);
	CPoint ToPoint = NPtoSP(pLink->m_ToPoint);

	if(FromPoint.x==ToPoint.x && FromPoint.y==ToPoint.y)  // same node
	continue; 

	pDC->MoveTo(FromPoint);
	pDC->LineTo(ToPoint);

	}
	}
	}
	}

	*/
	//draw select path
	if (pMainFrame->m_bShowLayerMap[layer_path] && m_PathNodeSequence.GetLength() >0)
	{

		std::vector<int> path_node_sequence;
		std::string str_line = pDoc->CString2StdString(m_PathNodeSequence);
		path_node_sequence = pDoc->ParseLineToIntegers(str_line);

		pDC->SelectObject(&g_WhiteBrush);


		//with vehicle location data at this time stamp
		for (int i = 0; i < path_node_sequence.size() - 1; i++)
		{


			DTANode* pFromNode = pDoc->FindNodeWithNodeNumber(path_node_sequence[i]);
			DTANode* pToNode = pDoc->FindNodeWithNodeNumber(path_node_sequence[i + 1]);
			if (pFromNode != NULL&& pToNode != NULL)
			{

				CPoint from_point = NPtoSP(pFromNode->pt);
				CPoint to_point = NPtoSP(pToNode->pt);

				DTALink* pLink = pDoc->FindLinkWithNodeNumbers(path_node_sequence[i], path_node_sequence[i + 1]);

				if (pLink!=NULL)
					g_SelectColorCode(pDC, 1);
				else
					g_SelectColorCode2(pDC, 1);




				node_size = 5;
				/// starting drawing nodes in normal mode
				pDC->Ellipse(from_point.x - node_size, from_point.y + node_size,
					from_point.x + node_size, from_point.y - node_size);

				pDC->MoveTo(from_point);

				pDC->LineTo(to_point);

				CString str_text;

				GDPoint text_pt;
				text_pt.x = (from_point.x + to_point.x) / 2;
				text_pt.y = (from_point.y + to_point.y) / 2;

				str_text.Format("pn=%d", i+1);

				CPoint screen_point = NPtoSP(text_pt);
				pDC->TextOut(screen_point.x, screen_point.y, str_text);

				CPoint FromPoint = from_point;
				CPoint ToPoint = to_point;

				double slopy = atan2((double)(FromPoint.y - ToPoint.y), (double)(FromPoint.x - ToPoint.x));
				double cosy = cos(slopy);
				double siny = sin(slopy);
				double display_length = sqrt((double)(FromPoint.y - ToPoint.y)*(FromPoint.y - ToPoint.y) + (double)(FromPoint.x - ToPoint.x)*(FromPoint.x - ToPoint.x));
				double arrow_size = min(7, display_length / 5.0);

				if (arrow_size > 0.2)
				{

					m_arrow_pts[0] = ToPoint;
					m_arrow_pts[1].x = ToPoint.x + (int)(arrow_size * cosy - (arrow_size / 2.0 * siny) + 0.5);
					m_arrow_pts[1].y = ToPoint.y + (int)(arrow_size * siny + (arrow_size / 2.0 * cosy) + 0.5);
					m_arrow_pts[2].x = ToPoint.x + (int)(arrow_size * cosy + arrow_size / 2.0 * siny + 0.5);
					m_arrow_pts[2].y = ToPoint.y - (int)(arrow_size / 2.0 * cosy - arrow_size * siny + 0.5);

					pDC->Polygon(m_arrow_pts, 3);
				}

			}



		}
	}



	if(pMainFrame->m_bShowLayerMap[layer_path] && pDoc->m_PathDisplayList.size() > pDoc->m_SelectPathNo && pDoc->m_SelectPathNo!=-1)
	{


		for (i=0 ; i<pDoc->m_PathDisplayList[pDoc->m_SelectPathNo].m_LinkVector.size(); i++)
		{
			DTALink* pLink = pDoc->m_LinkNoMap[pDoc->m_PathDisplayList[pDoc->m_SelectPathNo].m_LinkVector[i]];
			if(pLink!=NULL)
			{
				if(i==0) //
				{
					pDoc->m_OriginNodeID = pLink->m_FromNodeID ;
				}
				if(i== pDoc->m_PathDisplayList[pDoc->m_SelectPathNo].m_LinkVector.size()-1)
				{
					pDoc->m_DestinationNodeID =  pLink->m_ToNodeID ;
				}

				// select link color 
				if(pLink->m_LinkNo == pDoc->m_SelectedLinkNo)
				{
					g_SelectThickPenColor(pDC,0);
					pDC->SetTextColor(RGB(255,0,0));
				}
				else
				{
					pDC->SelectObject(&g_PenSelectPath);
				}

				DrawLinkAsLine(pLink,pDC);

			}
		}
	}

	// step 11: select font for origin and destination  of the shortest path

	// step 11: all all nodes

	if(pMainFrame->m_bShowLayerMap[layer_node] == true)
	{
		CPoint point;
		float feet_size;

		CFont od_font;
		int nODNodeSize = max(node_size,10);


		CPen pen_node;
		CBrush brush_node;

		pen_node.CreatePen (PS_SOLID, 1, theApp.m_NodeColor);
		brush_node.CreateSolidBrush (theApp.m_NodeBrushColor);


		int nODFontSize =  max(nODNodeSize * NodeTypeSize, 10);

		m_NodeTextFontSize = nODFontSize; 

		od_font.CreatePointFont(nODFontSize, m_NodeTypeFaceName);

		for (iNode = pDoc->m_NodeSet.begin(); iNode != pDoc->m_NodeSet.end(); iNode++)
		{

			point = NPtoSP((*iNode)->pt);

			CRect node_rect;
			node_rect.SetRect(point.x-m_NodeDisplayBoundarySize,point.y-m_NodeDisplayBoundarySize,point.x+50,point.y+m_NodeDisplayBoundarySize);

			if(RectIsInsideScreen(node_rect,ScreenRect) == false)  // not inside the screen boundary
				continue;

			pDC->SelectObject(&brush_node);
			pDC->SetTextColor(RGB(255,255,0));
			pDC->SelectObject(&pen_node);
			pDC->SetBkColor(RGB(0,0,0));

			if((*iNode)->m_NodeNo == pDoc->m_SelectedNodeID)
			{
				pDC->SelectObject(&g_PenSelectColor);

			}else if((*iNode)->m_ZoneID ==  pDoc->m_SelectedZoneID  && m_bHighlightActivityLocation)
			{
				pDC->SelectObject(&g_PenCentroidColor);
				pDC->SelectObject(&g_ActivityLocationBrush);

				if((*iNode)->m_External_OD_flag == 1)  // external origin 
				{

					pDC->SelectObject(&g_PenExternalOColor);

					pDC->SetTextColor(RGB(255,0,0));
					pDC->SetBkColor(RGB(0,0,0));

				}

				if((*iNode)->m_External_OD_flag == -1)  // external destination
				{
					pDC->SelectObject(&g_PenExternalDColor);
					pDC->SetTextColor(RGB(255,0,0));
					pDC->SetBkColor(RGB(0,0,0));


				}

			}else
			{

				//default;
				pDC->SelectObject(&pen_node);

				if((*iNode)->m_ControlType == pDoc->m_ControlType_YieldSign || 
					(*iNode)->m_ControlType == pDoc->m_ControlType_2wayStopSign ||
					(*iNode)->m_ControlType == pDoc->m_ControlType_4wayStopSign)
				{			
					pDC->SelectObject(&g_PenStopSignNodeColor);
				}
				if((*iNode)->m_ControlType == pDoc->m_ControlType_PretimedSignal || 
					(*iNode)->m_ControlType == pDoc->m_ControlType_ActuatedSignal)
				{
					pDC->SelectObject(&g_PenSignalNodeColor);
				}

			}

			if((*iNode)->m_NodeNo == pDoc->m_OriginNodeID && pMainFrame->m_bShowLayerMap[layer_path] ==true)
			{

				CFont* oldFont = pDC->SelectObject(&od_font);
				pDC->SelectObject(&g_PenSelectColor);
				pDC->SelectObject(&g_BlackBrush);
				pDC->SetTextColor(RGB(255,0,0));
				pDC->SetBkColor(RGB(0,0,0));

				TEXTMETRIC tmOD;
				memset(&tmOD, 0, sizeof TEXTMETRIC);
				pDC->GetOutputTextMetrics(&tmOD);

				pDC->Rectangle (point.x - nODNodeSize, point.y + nODNodeSize,
					point.x + nODNodeSize, point.y - nODNodeSize);

				point.y -= tmOD.tmHeight / 2;
				pDC->TextOut(point.x , point.y , _T("A"));

				pDC->SelectObject(oldFont);  // restore font



			}else if((*iNode)->m_NodeNo == pDoc->m_DestinationNodeID && pMainFrame->m_bShowLayerMap[layer_path] ==true)
			{
				CFont* oldFont = pDC->SelectObject(&od_font);// these are local font, created inside the function, we do not want to create them in another sub functions to speed up the display efficiency.

				TEXTMETRIC tmOD;
				memset(&tmOD, 0, sizeof TEXTMETRIC);
				pDC->GetOutputTextMetrics(&tmOD);

				pDC->SelectObject(&g_PenSelectColor);
				pDC->SelectObject(&g_BlackBrush);
				pDC->SetTextColor(RGB(255,0,0));
				pDC->SetBkColor(RGB(0,0,0));

				pDC->Rectangle (point.x - nODNodeSize, point.y + nODNodeSize,
					point.x + nODNodeSize, point.y - nODNodeSize);

				point.y -= tmOD.tmHeight / 2;
				pDC->TextOut(point.x , point.y , _T("B"));

				pDC->SelectObject(oldFont);  // restore font


			}else if ((*iNode)->m_IntermediateDestinationNo>=1)
			{
				CFont* oldFont = pDC->SelectObject(&od_font);// these are local font, created inside the function, we do not want to create them in another sub functions to speed up the display efficiency.

				TEXTMETRIC tmOD;
				memset(&tmOD, 0, sizeof TEXTMETRIC);
				pDC->GetOutputTextMetrics(&tmOD);

				pDC->SelectObject(&g_PenSelectColor);
				pDC->SelectObject(&g_BlackBrush);
				pDC->SetTextColor(RGB(255,0,0));
				pDC->SetBkColor(RGB(0,0,0));

				pDC->Rectangle (point.x - nODNodeSize, point.y + nODNodeSize,
					point.x + nODNodeSize, point.y - nODNodeSize);

				point.y -= tmOD.tmHeight / 2;
				CString str;
				str.Format ("I%d",(*iNode)->m_IntermediateDestinationNo);
				pDC->TextOut(point.x , point.y , str);

				pDC->SelectObject(oldFont);  // restore font

			}else
			{
				if(m_bShowNode)
				{

					if(m_ShowNodeTextMode == node_display_cycle_length_in_second_for_signal_only)
					{
						if( (*iNode)->m_ControlType != pDoc->m_ControlType_PretimedSignal &&  (*iNode)->m_ControlType != pDoc-> m_ControlType_ActuatedSignal)
						{

							continue;  // do not draw nodes;

						}

					}

					feet_size = pDoc->m_UnitFeet*m_Resolution;

					if((*iNode)->m_NodeNo == pDoc->m_SelectedNodeID)
					{
						feet_size = max(50,min(100,feet_size*3));  // 

					}


					/// starting drawing nodes in normal mode

					if((*iNode)->m_DistanceToRoot > MAX_SPLABEL-1)
					{
						pDC->SelectObject(&g_PenSelectColor2_obs);

						feet_size = max(2,feet_size);  // show node anyway
					}


					//				if(feet_size*pDoc->m_NodeDisplaySize > 0.2) // add or condition to show all nodes
					if ((*iNode)->m_LayerNo == 1)
					{
						pDC->SelectObject(&g_PenReference);
						pDC->SelectObject(&g_BrushReference);
					}
					if(pDoc->m_LinkMOEMode == MOE_bottleneck)
					{

					int LOS = 0;
					float avg_delay = pDoc->GetNodeTotalDelay( (*iNode)->m_NodeNumber ,g_Simulation_Time_Stamp,LOS);

					if(avg_delay>1)
					{
					float size = avg_delay*pDoc->m_BottleneckDisplaySize;

					//if(size > 200)
					//	size = 200;

					pDC->SelectObject(&g_BlackPen);
					pDC->SelectObject(&brush_moe[LOS]);



					float avg_delay_ratio = avg_delay/max(1,size);

					if(g_bShowRadarChart==true)
						DrawNodeRadarChart(pDC, (*iNode),point, size, avg_delay_ratio, LOS);
					else
						DrawNodePieChart(pDC, (*iNode),point, size, LOS);

					}
					}
					else
					{
						DrawNode(pDC, (*iNode),point, node_size,tm);
					}

				}
			}

		}


	}

	if(pMainFrame->m_bShowLayerMap[layer_movement] == true || m_bShowProhibitedMovements)
	{
		CRect PlotRect;
		for (iNode = pDoc->m_NodeSet.begin(); iNode != pDoc->m_NodeSet.end(); iNode++)
		{
			CPoint point = NPtoSP((*iNode)->pt);

			CRect node_rect;
			node_rect.SetRect(point.x- m_NodeDisplayBoundarySize,point.y-m_NodeDisplayBoundarySize,point.x+ m_NodeDisplayBoundarySize,point.y+ m_NodeDisplayBoundarySize);

			if(RectIsInsideScreen(node_rect,ScreenRect) == false)  // not inside the screen boundary
				continue;



			DrawNodeMovements(pDC, (*iNode), PlotRect);
		}

	}

	//step 12: draw generic point layer , e.g. crashes

	if(pMainFrame->m_bShowLayerMap[layer_incident])
	{
		pDC->SelectObject(&g_BlackBrush);
		pDC->SetTextColor(RGB(255,255,0));
		pDC->SelectObject(&g_PenSelectColor5);
		pDC->SetBkColor(RGB(0,0,0));

		std::list<DTAPoint*>::iterator iCrash;
		for (iCrash = pDoc->m_DTAPointSet.begin(); iCrash != pDoc->m_DTAPointSet.end(); iCrash++)
		{
			CPoint point = NPtoSP((*iCrash)->pt);

			node_size = 3;

			/// starting drawing nodes in normal mode
			pDC->Ellipse(point.x - node_size, point.y + node_size,
				point.x + node_size, point.y - node_size);

		}
	}

	if(pMainFrame->m_bShowLayerMap[layer_vehicle_position])
	{
		pDC->SelectObject(&g_BlackBrush);
		pDC->SetTextColor(RGB(255,255,0));
		pDC->SelectObject(&g_GPSTrajectoryColor);
		pDC->SetBkColor(RGB(0,0,0));

		CFont GPS_font;  // local font for nodes. dynamically created. it is effective only inside this function. if you want to pass this font to the other function, we need to pass the corresponding font pointer (which has a lot of communication overheads)
		int node_size = min(200,max(2,int(pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution)));

		int NodeTypeSize = pDoc->m_NodeTextDisplayRatio;
		int nFontSize =  max(node_size * NodeTypeSize, 10);

		GPS_font.CreatePointFont(nFontSize, m_NodeTypeFaceName);

		CFont* pOldFont = pDC->SelectObject(&node_font);

		
		long timestamp_in_second = (long)(g_Simulation_Time_Stamp*60 + 0.5); // *60 convert min to second, +0.5, round to nearest integer


		if (m_bShowAllCompleteTrajectory || m_bShowCompleteTrajectory)
		{

			pDC->SelectObject(&g_WhiteBrush);

	
					//with vehicle location data at this time stamp

				std::map<string,VehicleLocationTimeIndexedMap>::iterator itr2;

				for(itr2 = pDoc->m_VehicleWithLocationVectorMap.begin();
					itr2 != pDoc->m_VehicleWithLocationVectorMap .end(); itr2++)
				{		//scan all vehicle records at this timestamp

				int agent_no = atoi((*itr2).first.c_str());

				for (int i = 0; i<(*itr2).second.VehicleLocationRecordVector.size()-1; i++)
				{
					std::string agent_type = (*itr2).second.VehicleLocationRecordVector[i].agent_type;

					if (agent_type == "p")  // pax
					{
						if (g_SelectedPassengerID != "All" &&  g_SelectedPassengerID != (*itr2).second.VehicleLocationRecordVector[i].agent_id)
							continue;
					}

					
					if (agent_type == "v")  // vehicle
					{
						if (g_SelectedVehicleID != "All" &&  g_SelectedVehicleID != (*itr2).second.VehicleLocationRecordVector[i].agent_id)
							continue;
					}


					// if day_no is 0 always display 
					if (agent_type == "p")  // pax
						pDC->SelectObject(&g_PenPaxSelectColor0);
					else
						g_SelectColorCode(pDC, (*itr2).second.VehicleLocationRecordVector[i].agent_no % 5);

					CPoint from_point, to_point;

					GDPoint from_pt;
					from_pt.x = (*itr2).second.VehicleLocationRecordVector[i].x;
					from_pt.y = (*itr2).second.VehicleLocationRecordVector[i].y;

					int current_second = (*itr2).second.VehicleLocationRecordVector[i].time_stamp_in_second;
						node_size = 1;
					/// starting drawing nodes in normal mode
						pDC->Ellipse(from_point.x - node_size, from_point.y + node_size,
							from_point.x + node_size, from_point.y - node_size);
			

					GDPoint to_pt;
					to_pt.x = (*itr2).second.VehicleLocationRecordVector[i+1].x;
					to_pt.y = (*itr2).second.VehicleLocationRecordVector[i+1].y;

					from_point = NPtoSP(from_pt);
					 to_point = NPtoSP(to_pt);

						pDC->MoveTo(from_point);

						pDC->LineTo (to_point);

						//CString str_text;

						//GDPoint text_pt;
						//text_pt.x = (from_point.x + to_point.x) / 2;
						//text_pt.y = (from_point.y + to_point.y) / 2;

						//str_text.Format("%s", (*itr2).second.VehicleLocationRecordVector[i].agent_id.c_str());
						//				
						//CPoint screen_point = NPtoSP(text_pt);
						//pDC->TextOut(screen_point.x, screen_point.y, str_text);

						//CPoint FromPoint = from_point;
						//CPoint ToPoint  = to_point;

						//double slopy = atan2((double)(FromPoint.y - ToPoint.y), (double)(FromPoint.x - ToPoint.x));
						//double cosy = cos(slopy);
						//double siny = sin(slopy);   
						//double display_length  = sqrt((double)(FromPoint.y - ToPoint.y)*(FromPoint.y - ToPoint.y)+(double)(FromPoint.x - ToPoint.x)*(FromPoint.x - ToPoint.x));
						//double arrow_size = min(7,display_length/5.0);

						//if(arrow_size>0.2)
						//{
						//	pDC->SelectObject(&g_GPSTrajectoryColor);
						//	m_arrow_pts[0] = ToPoint;
						//	m_arrow_pts[1].x = ToPoint.x + (int)(arrow_size * cosy - (arrow_size / 2.0 * siny) + 0.5);
						//	m_arrow_pts[1].y = ToPoint.y + (int)(arrow_size * siny + (arrow_size / 2.0 * cosy) + 0.5);
						//	m_arrow_pts[2].x = ToPoint.x + (int)(arrow_size * cosy + arrow_size / 2.0 * siny + 0.5);
						//	m_arrow_pts[2].y = ToPoint.y - (int)(arrow_size / 2.0 * cosy - arrow_size * siny + 0.5);

						//	pDC->Polygon(m_arrow_pts, 3);
						//}


				
				}

				}
	
		
		
		}




		pDC->SelectObject(&g_BlackBrush);
		pDC->SelectObject(&g_GPSTrajectoryColor);

			if(pDoc->m_VehicleLocationMap.find(timestamp_in_second) != pDoc->m_VehicleLocationMap.end())
			{
				//with vehicle location data at this time stamp


				std::vector<VehicleLocationRecord>::iterator itr;

				for(itr = pDoc->m_VehicleLocationMap[timestamp_in_second].VehicleLocationRecordVector .begin();
					itr != pDoc->m_VehicleLocationMap[timestamp_in_second].VehicleLocationRecordVector .end(); itr++)
				{		//scan all vehicle records at this timestamp

					GDPoint pt;
					pt.x = (*itr).x ; 
					pt.y = (*itr).y;

					pDC->SelectObject(&g_GPSTrajectoryColor0);

					g_SelectBrushColor(pDC, (*itr).agent_no % 5);
					CPoint point = NPtoSP(pt);

					node_size = 5;
					/// starting drawing nodes in normal mode
						pDC->Ellipse(point.x - node_size, point.y + node_size,
							point.x + node_size, point.y - node_size);

						
					if(itr->b_to_data_flag == true)
					{ 
						pDC->MoveTo (point);
				

					GDPoint to_pt;
					to_pt.x = itr->to_x ; 
					to_pt.y = itr->to_y;

					CPoint to_point = NPtoSP(to_pt);

						pDC->LineTo (to_point);

						CPoint FromPoint = point;
						CPoint ToPoint  = to_point;

						double slopy = atan2((double)(FromPoint.y - ToPoint.y), (double)(FromPoint.x - ToPoint.x));
						double cosy = cos(slopy);
						double siny = sin(slopy);   
						double display_length  = sqrt((double)(FromPoint.y - ToPoint.y)*(FromPoint.y - ToPoint.y)+(double)(FromPoint.x - ToPoint.x)*(FromPoint.x - ToPoint.x));
						double arrow_size = min(7,display_length/5.0);

						if(arrow_size>0.2)
						{

							m_arrow_pts[0] = ToPoint;
							m_arrow_pts[1].x = ToPoint.x + (int)(arrow_size * cosy - (arrow_size / 2.0 * siny) + 0.5);
							m_arrow_pts[1].y = ToPoint.y + (int)(arrow_size * siny + (arrow_size / 2.0 * cosy) + 0.5);
							m_arrow_pts[2].x = ToPoint.x + (int)(arrow_size * cosy + arrow_size / 2.0 * siny + 0.5);
							m_arrow_pts[2].y = ToPoint.y - (int)(arrow_size / 2.0 * cosy - arrow_size * siny + 0.5);

							pDC->Polygon(m_arrow_pts, 3);
						}

					}
					
				
				
				}
			
			}


 //old code
		int vehicle_size  = pDoc->m_VehicleSet.size();

		bool bSamplingDisplay = false;
		int sample_size  = 1;
		if(vehicle_size>100000)
		{ // more than one millon vehicle
			bSamplingDisplay = true;
			sample_size = vehicle_size/100000;
		}

		std::list<DTAVehicle*>::iterator iVehicle;

		for (iVehicle = pDoc->m_VehicleSet.begin(); iVehicle != pDoc ->m_VehicleSet.end(); iVehicle++)
		{
			DTAVehicle* pVehicle = (*iVehicle);

			if(pVehicle->m_VehicleLocationSize> 1)
			{

				if(bSamplingDisplay ==true)
				{
					if(pVehicle->m_VehicleID %sample_size !=0)  // one one of samples will be displayed to speed up display speed
						continue;
				}


				float previous_timestamp = 0;


				previous_timestamp = pVehicle->m_LocationRecordAry [0].time_stamp_in_min ;

				for (int i =0; i < pVehicle->m_VehicleLocationSize ; i++)
				{

					GDPoint pt;
					pt.x = pVehicle->m_LocationRecordAry [i].x ;
					pt.y = pVehicle->m_LocationRecordAry [i].y ;

					CPoint point = NPtoSP(pt);

					node_size = 3;
					if(i==0)
					{ 
						pDC->MoveTo (point);
					}
					else
					{
						pDC->LineTo (point);
					}


					if(m_ShowGPSTextMode != GPS_display_all)
					{
						CString str_text;
						str_text.Format ("");


						switch (m_ShowGPSTextMode)
						{
						case GPS_display_vehicle_id: str_text.Format ("%d", pVehicle->m_VehicleID ); break;
						case GPS_display_timestamp_in_min:   str_text.Format ("%.2f",pVehicle->m_LocationRecordAry [i].time_stamp_in_min  ); break;
						case GPS_display_timegap_in_min:  str_text.Format ("%.2f",pVehicle->m_LocationRecordAry [i].time_stamp_in_min -  previous_timestamp); break;
							//				case GPS_display_speed: 

						}

						CPoint TextPoint =  point;
						pDC->SetBkColor(RGB(0,0, 0));
						pDC->SetTextColor(RGB(0,1,0));
						pDC->TextOut(TextPoint.x,TextPoint.y, str_text);

						/// starting drawing nodes in normal mode
						pDC->Ellipse(point.x - node_size, point.y + node_size,
							point.x + node_size, point.y - node_size);

						previous_timestamp = pVehicle->m_LocationRecordAry [i].time_stamp_in_min ;

					}
				}
			}

		}

		pDC->SelectObject(pOldFont);
	}


	// step 13: draw zone layer

	CPen ZonePen;

	// Create a solid red pen of width 2.
	ZonePen.CreatePen(PS_DASH, 1, theApp.m_ZoneColor);

	if(pMainFrame->m_bShowLayerMap[layer_zone])
	{	

		CFont zone_font;  // local font for nodes. dynamically created. it is effective only inside this function. if you want to pass this font to the other function, we need to pass the corresponding font pointer (which has a lot of communication overheads)
		zone_font.CreatePointFont(nFontSize*4, m_NodeTypeFaceName);

		pDC->SelectObject(&zone_font);

		std::map<int, DTAZone>	:: const_iterator itr;


		pDC->SetTextColor(pDoc->m_ZoneTextColor);


		for(int draw_flag = 0; draw_flag<=1; draw_flag++)
		{
			for(itr = pDoc->m_ZoneMap.begin(); itr != pDoc->m_ZoneMap.end(); itr++)
			{
				if(draw_flag==1 && itr->first != pDoc->m_SelectedZoneID) 
				{  // draw the selected zone only when draw_flag = 1
					continue;
				}



				if(itr->first == pDoc->m_SelectedZoneID )
				{
					pDC->SelectObject(&g_PenSelectColor0);
				}
				else
				{
					pDC->SelectObject(&ZonePen);
				}

				int center_x = 0;
				int center_y = 0;

				if(itr->second.m_ShapePoints .size() > 0)
				{

					int min_x, max_x, min_y, max_y;

					for (int i = 0; i < itr->second.m_ShapePoints.size(); i++)
					{
						CPoint point = NPtoSP(itr->second.m_ShapePoints[i]);

						if (i == 0)
						{
							min_x = max_x = point.x;
							min_y = max_y = point.y;

						}

						min_x = min(point.x, min_x);
						max_x = max(point.x, max_x);

						min_y = min(point.y, min_y);
						max_y = max(point.y, max_y);


					}

					int size = 100;
					CRect zone_rect;
					zone_rect.SetRect(min_x - size, min_y - size, max_x + size, max_y + size);


					if (RectIsInsideScreen(zone_rect, ScreenRect) == false)  // zone not inside the screen boundary: do not draw
						continue;


					for(int i = 0; i< itr->second.m_ShapePoints .size(); i++)
					{

						CPoint point =  NPtoSP(itr->second.m_ShapePoints[i]);

						center_x += point.x;
						center_y += point.y;

						if(i == 0)
							pDC->MoveTo(point);
						else
							pDC->LineTo(point);

					}



					CPoint point_0 =  NPtoSP(itr->second.m_ShapePoints[0]);  // back to the starting point

					pDC->LineTo(point_0);

					center_x  = center_x/max(1,itr->second.m_ShapePoints .size());
					center_y  = center_y/max(1,itr->second.m_ShapePoints .size()) - tm.tmHeight;

					if (zone_rect.Height() >= 10 || zone_rect.Width()>=10)
					{
						CString zone_id_str;
						zone_id_str.Format("%d", itr->second.m_ZoneID);

						pDC->TextOut(center_x, center_y, zone_id_str);
					}
				}

			}
		}

	}	


	// step 13: draw subarea layer
	if(GetDocument()->m_SubareaShapePoints.size() > 0 && (pMainFrame->m_bShowLayerMap[layer_subarea]||pMainFrame->m_bShowLayerMap[layer_zone]))
	{
		CPoint point_0  = NPtoSP(GetDocument()->m_SubareaShapePoints[0]);


		pDC->SelectObject(&g_PenDashBlue);


		pDC->MoveTo(point_0);

		for (int sub_i= 0; sub_i < GetDocument()->m_SubareaShapePoints.size(); sub_i++)
		{
			CPoint point =  NPtoSP(GetDocument()->m_SubareaShapePoints[sub_i]);
			pDC->LineTo(point);

		}
		if(isFinishSubarea)
			pDC->LineTo(point_0);

	}
	// step 14: show not-matched sensors
	if(pMainFrame->m_bShowLayerMap[layer_detector])
	{
		pDC->SelectObject(&g_PenNotMatchedSensorColor);


		int nODNodeSize = max(node_size*0.8, 8);

		CFont od_font;
		int nODFontSize = max(nODNodeSize * NodeTypeSize, 10);
		od_font.CreatePointFont(nODFontSize, m_NodeTypeFaceName);

		std::map<string, DTA_sensor>::iterator iSensor;
		for (iSensor = pDoc->m_SensorMap.begin(); iSensor != pDoc->m_SensorMap.end(); iSensor++)
		{

			GDPoint pt;
			pt.x = pDoc->m_SensorMapX + (*iSensor).second.pt.x;
			pt.y = pDoc->m_SensorMapY + (*iSensor).second.pt.y;

			CPoint point = NPtoSP(pt);

			CFont* oldFont = pDC->SelectObject(&od_font);
			pDC->SelectObject(&g_PenDetectorColor);


			pDC->SelectObject(&g_WhiteBrush);

			DTALink* pLink = GetDocument()->FindLinkWithNodeNumbers((*iSensor).second.matched_from_node_id, (*iSensor).second.matched_to_node_id);

			if (m_bShowSensorMatchedLink && pLink!=NULL)
			{
				pDC->SelectObject(&g_GreenBrush);
			}
			

			pDC->SetTextColor(RGB(255, 0, 0));
			pDC->SetBkColor(RGB(255, 255, 255));

			TEXTMETRIC tmOD;
			memset(&tmOD, 0, sizeof TEXTMETRIC);
			pDC->GetOutputTextMetrics(&tmOD);

			pDC->Rectangle(point.x - nODNodeSize, point.y + nODNodeSize,
				point.x + nODNodeSize, point.y - nODNodeSize);

			point.y -= tmOD.tmHeight / 2;

			if (m_bShowSensorDecription==true)
				pDC->TextOut(point.x, point.y, (*iSensor).second.description.c_str());
			else
				pDC->TextOut(point.x, point.y, (*iSensor).second.SensorID.c_str());

			pDC->SelectObject(oldFont);  // restore font

			if (m_bShowSensorMatchedLink)
			{

				pDC->SelectObject(g_PenMatchedSensorLink);

		
				if (pLink != NULL)
				{
					pDC->MoveTo(point);
					CPoint MatchedPoint = NPtoSP(pLink->GetRelativePosition(0.5));
					pDC->LineTo(MatchedPoint);

				}
					
			
			
			
			}


		}
	}

	// step 15: show vehicles

	if( pDoc->m_LinkMOEMode == MOE_vehicle)
	{

		CFont vehicle_font;  // local font for nodes. dynamically created. it is effective only inside this function. if you want to pass this font to the other function, we need to pass the corresponding font pointer (which has a lot of communication overheads)

		int vehicle_size = min(200,max(3,int(pDoc->m_VehicleDisplaySize *pDoc->m_UnitFeet*m_Resolution)));

		int NodeTypeSize = pDoc->m_NodeTextDisplayRatio;
		int nFontSize =  max(vehicle_size * NodeTypeSize*0.8, 10);

		vehicle_font.CreatePointFont(nFontSize, m_NodeTypeFaceName);

		pDC->SelectObject(&vehicle_font);

		TEXTMETRIC tm_vehicle;
		memset(&tm_vehicle, 0, sizeof TEXTMETRIC);
		pDC->GetOutputTextMetrics(&tm_vehicle);

		pDC->SelectObject(&g_PenVehicle);  //green
		pDC->SelectObject(&g_BrushVehicle); //green



		int vehicle_set_size  = pDoc->m_VehicleSet.size();

		bool bSamplingDisplay = false;
		int sample_size  = 1;
		if(vehicle_set_size>500000)
		{ // more than 0.5 millon vehicles
			bSamplingDisplay = true;
			sample_size = vehicle_set_size/500000;
		}

		std::list<DTAVehicle*>::iterator iVehicle;
		for (iVehicle = pDoc->m_VehicleSet.begin(); iVehicle != pDoc->m_VehicleSet.end(); iVehicle++)
		{
			if(bSamplingDisplay)
			{
				if((*iVehicle)->m_VehicleID %sample_size !=0)
					continue;
			}

			if((*iVehicle)->m_DayNo != g_SimulatedLastDayNo)
				continue;


			if((*iVehicle)->m_DepartureTime <=g_Simulation_Time_Stamp &&
				g_Simulation_Time_Stamp <=(*iVehicle)->m_ArrivalTime && (*iVehicle)->m_NodeSize>=2)
			{

				if(bShowVehiclesWithIncompleteTrips && (*iVehicle)->m_bComplete)  // show incomplete vehicles only
					continue;

				if(m_bShowSelectedVehicleOnly && (*iVehicle)->m_VehicleID != pDoc->m_SelectedVehicleID)  // show selected vehicle only
					continue;


				if((*iVehicle)->m_PricingType ==4)  // transit user
					pDC->SelectObject(&g_BrushTransitUser); //red transit users

				float ratio = 0;

				if((*iVehicle)->m_bGPSVehicle == false) 
				{
					int LinkID = pDoc->GetVehilePosition((*iVehicle), g_Simulation_Time_Stamp,ratio);

					DTALink* pLink = pDoc->m_LinkNoMap[LinkID];
					if(pLink!=NULL)
					{

						CPoint VehPoint= NPtoSP(pLink->GetRelativePosition(ratio));
						if((*iVehicle)->m_VehicleID == pDoc->m_SelectedVehicleID)
						{
							pDC->SelectObject(&g_PenSelectedVehicle); //red transit users
							pDC->Ellipse (VehPoint.x - vehicle_size*3, VehPoint.y - vehicle_size*3,
								VehPoint.x + vehicle_size*3, VehPoint.y + vehicle_size*3);
							pDC->SelectObject(&g_PenVehicle);  //green

						}else
						{ 

							pDC->Ellipse (VehPoint.x - vehicle_size, VehPoint.y - vehicle_size,
								VehPoint.x + vehicle_size, VehPoint.y + vehicle_size);

						}
						if(m_ShowGPSTextMode== GPS_display_vehicle_id)
						{

							CString str_number;
							str_number.Format ("%d",(*iVehicle)->m_VehicleID  );

							VehPoint.y -= tm_vehicle.tmHeight / 2;

							pDC->TextOut(VehPoint.x , VehPoint.y,str_number);
						}
					}

				}else
				{  // show GPS position

					GDPoint pt;
					if(pDoc->GetGPSVehilePosition((*iVehicle), g_Simulation_Time_Stamp, pt) == false)
						continue;

					CPoint VehPoint= NPtoSP(pt);
					if((*iVehicle)->m_VehicleID == pDoc->m_SelectedVehicleID)
					{
						pDC->SelectObject(&g_PenSelectedVehicle); //red transit users
						pDC->Ellipse (VehPoint.x - vehicle_size*3, VehPoint.y - vehicle_size*3,
							VehPoint.x + vehicle_size*3, VehPoint.y + vehicle_size*3);
						pDC->SelectObject(&g_PenVehicle);  //green

					}else
					{ 

						pDC->Ellipse (VehPoint.x - vehicle_size, VehPoint.y - vehicle_size,
							VehPoint.x + vehicle_size, VehPoint.y + vehicle_size);

					}

					if(m_bShowVehicleNumber)
					{

						CString str_number;
						str_number.Format ("%d",(*iVehicle)->m_VehicleID  );

						VehPoint.y -= tm_vehicle.tmHeight / 2;

						pDC->TextOut(VehPoint.x , VehPoint.y,str_number);
					}


				}

			}

		}
	}

		if( pDoc->m_LinkMOEMode == MOE_vehicle)
	{

			CFont pt_font;
			int nODNodeSize = max(node_size*0.8, 8);
			int nODFontSize = max(nODNodeSize * NodeTypeSize, 8);

			pt_font.CreatePointFont(nODFontSize, m_NodeTypeFaceName);

			CFont* oldFont = pDC->SelectObject(&pt_font);

		pDC->SelectObject(&g_PenVehicle);  //green
		pDC->SelectObject(&g_BrushVehicle); //green
		std::map<string, VehicleLocationTimeIndexedMap>::iterator itr2;

		for (itr2 = pDoc->m_VehicleWithLocationVectorMap.begin();
			itr2 != pDoc->m_VehicleWithLocationVectorMap.end(); itr2++)
		{		//scan all vehicle records at this timestamp

			std::vector<VehicleLocationRecord>::iterator itr;

			int agent_no = atoi((*itr2).first.c_str());
			//if (bSamplingDisplay)
			//{
			//	if((*iVehicle)->m_VehicleID %sample_size !=0)
			//		continue;
			//}

			//if((*iVehicle)->m_DayNo != g_SimulatedLastDayNo)
			//	continue;


				//if(bShowVehiclesWithIncompleteTrips && (*iVehicle)->m_bComplete)  // show incomplete vehicles only
				//	continue;

				//if(m_bShowSelectedVehicleOnly && (*iVehicle)->m_VehicleID != pDoc->m_SelectedVehicleID)  // show selected vehicle only
				//	continue;


				//if((*iVehicle)->m_PricingType ==4)  // transit user
				//	pDC->SelectObject(&g_BrushTransitUser); //red transit users

			GDPoint pt;


			if (pDoc->GetAgentPosition((*itr2).first, g_Simulation_Time_Stamp, pt))
			{
						CPoint AgentPoint = NPtoSP(pt);

						//if((*iVehicle)->m_VehicleID == pDoc->m_SelectedVehicleID)
						//{
						//	pDC->SelectObject(&g_PenSelectedVehicle); //red transit users
						//	pDC->Ellipse (VehPoint.x - vehicle_size*3, VehPoint.y - vehicle_size*3,
						//		VehPoint.x + vehicle_size*3, VehPoint.y + vehicle_size*3);
						//	pDC->SelectObject(&g_PenVehicle);  //green

						//}else
						//{ 

						DrawBitmap(pDC, AgentPoint, IDB_BUSMOVING);


							//pDC->Ellipse (VehPoint.x - vehicle_size, VehPoint.y - vehicle_size,
							//	VehPoint.x + vehicle_size, VehPoint.y + vehicle_size);

						//}
							CString str_number;
							str_number.Format("%s", (*itr2).first.c_str());
							AgentPoint.y -=15;
							pDC->TextOut(AgentPoint.x, AgentPoint.y, str_number);
			
				}



		}
	}
	//////////////////////////////////////
	// step 16: draw OD demand

	if(pMainFrame->m_bShowLayerMap[layer_ODMatrix])
	{
		float MaxODDemand = 0;

		int  p = 0; 
		int i,j;


		bool bODDataReady = false;
		if(pDoc->m_OD_data_vector.size()>0)
		{
			bODDataReady = true;
		}

		for(i=0; i < pDoc->m_ZoneNoSize ; i++)
			for(j=0; j< pDoc->m_ZoneNoSize ; j++)
			{
				if( pDoc->m_ODMOEMatrix[p][i][j].TotalVehicleSize > MaxODDemand)
				{
					MaxODDemand = pDoc->m_ODMOEMatrix[p][i][j].TotalVehicleSize;

				}

				if( bODDataReady == false && pDoc->m_ODMOEMatrix[p][i][j].TotalVehicleSize >=10 && i!=j  )
				{
					pDoc->m_OD_data_vector.push_back (pDoc->m_ODMOEMatrix[p][i][j].TotalVehicleSize);
				}

			}

			int threashold_200 = 100;   // width: 1
			int threashold_100 = 100;  //width: 2
			int threashold_50 = 100;  //width: 3
			int threashold_20 = 100;  //width: 4
			int threashold_10 = 100;  //width: 5

			if(bODDataReady == false)
			{
				sort(pDoc->m_OD_data_vector.begin(), pDoc->m_OD_data_vector.end());
			}

			if(pDoc->m_OD_data_vector.size()>200)
			{
				int size  = pDoc->m_OD_data_vector.size();
				threashold_200 = pDoc->m_OD_data_vector[size-200];

				threashold_100 = pDoc->m_OD_data_vector[size- 100];
				threashold_50 = pDoc->m_OD_data_vector[size- 50];
				threashold_20 = pDoc->m_OD_data_vector[size- 20];
				threashold_10 = pDoc->m_OD_data_vector[size- 10];
			}

			// threashold is 100th largest number


			for(i=0; i < pDoc->m_ZoneNoSize ; i++)
				for(j=0; j< pDoc->m_ZoneNoSize ; j++)
				{
					if( pDoc->m_ODMOEMatrix[p][i][j].TotalVehicleSize >=5 && i!=j)
					{
						CPoint FromPoint = NPtoSP(pDoc->m_ZoneMap[pDoc->m_ZoneNumberVector [i]].GetCenter());
						CPoint ToPoint = NPtoSP(pDoc->m_ZoneMap[pDoc->m_ZoneNumberVector [j]].GetCenter());

						CPen penmoe;
						double value = pDoc->m_ODMOEMatrix[p][i][j].TotalVehicleSize;
						float Width = value/MaxODDemand*10;

						if(m_bShowTop10ODOnly == false)
						{
							if(value >= threashold_200)
								Width = max(1.1,Width);  // show the line for 200th largest value

							if(value >= threashold_100)
								Width = max(Width,2);  

							if(value >= threashold_50)
								Width = max(Width,3);  // show the line for 50th largest value

							if(value >= threashold_20)
								Width = max(Width,4);  // show the line for 50th largest value
						}
						if(value >= threashold_10)
							Width = max(Width,5);  // show the line for 50th largest value

						if(Width>=1)  //draw critical OD demand only
						{

							penmoe.CreatePen (PS_SOLID, (int)(Width), RGB(0,255,255));
							pDC->SelectObject(&penmoe);
							pDC->MoveTo(FromPoint);
							pDC->LineTo(ToPoint);
						}

					}
				}

	}


	if(pDoc->m_ZoneMap.find(pDoc->m_CriticalOriginZone)!= pDoc->m_ZoneMap.end() 
		&& pDoc->m_ZoneMap.find(pDoc->m_CriticalDestinationZone)!= pDoc->m_ZoneMap.end())
	{
		CPoint FromPoint = NPtoSP(pDoc->m_ZoneMap[pDoc->m_CriticalOriginZone].GetCenter());
		CPoint ToPoint = NPtoSP(pDoc->m_ZoneMap[pDoc->m_CriticalDestinationZone].GetCenter());

		CPen penmoe;
		float Width = 5;

		if(Width>=0.2)  //draw critical OD demand only
		{
			penmoe.CreatePen (PS_SOLID, (int)(Width), RGB(0,255,255));
			pDC->SelectObject(&penmoe);
			pDC->MoveTo(FromPoint);
			pDC->LineTo(ToPoint);
		}

	}


	// step 17: draw Public Transit Layer

	if(pMainFrame->m_bShowLayerMap[layer_transit])
	{
		CFont pt_font;
		int nODNodeSize = max(node_size*0.8,8);
		int nODFontSize =  max(nODNodeSize * NodeTypeSize, 8);

		pt_font.CreatePointFont(nODFontSize, m_NodeTypeFaceName);

		CFont* oldFont = pDC->SelectObject(&pt_font);

		pDC->SetTextColor(RGB(255,0,0));
		pDC->SetBkColor(RGB(0,0,0));

		if(m_bShowTransitAccessibility)
		{
			pDC->SelectObject(&g_TransitLightBrush);
			pDC->SelectObject(&g_TransitLightPen);

		}
		else
		{
			pDC->SelectObject(&g_TransitBrush);
			pDC->SelectObject(&g_TransitPen);
		}
		DrawPublicTransitLayer(pDC);
		pDC->SelectObject(oldFont);  // restore font

	}

	if(m_bShowTransitAccessibility)
	{
		CFont pt_font;
		int nODNodeSize = max(node_size*0.8,8);
		int nODFontSize =  max(nODNodeSize * NodeTypeSize, 8);

		pt_font.CreatePointFont(nODFontSize, m_NodeTypeFaceName);

		CFont* oldFont = pDC->SelectObject(&pt_font);

		pDC->SetTextColor(RGB(255,0,0));
		pDC->SetBkColor(RGB(0,0,0));

		pDC->SelectObject(&g_TransitAccessibilityPen);
		pDC->SelectObject(&g_TransitAccessibilityBrush);

		DrawPublicTransitAccessibilityLayer(pDC);
		pDC->SelectObject(oldFont);  // restore font

	}

	// show node and bottleneck
	if(pMainFrame->m_bShowLayerMap[layer_ramp] == true && pDoc->m_LinkMOEMode == MOE_bottleneck)
	{

		if(m_bLineDisplayConditionalMode)  // special condition
		{
			m_link_display_mode = link_display_mode_line;

		}else
		{
			if((pDoc->m_LinkMOEMode !=MOE_bottleneck) &&(pDoc->m_LinkMOEMode == MOE_none || pDoc->m_LaneWidthInFeet * pDoc->m_UnitFeet*m_Resolution * 5 > 0.1f)) // 5 lanes
				m_link_display_mode = link_display_mode_band;
			else 
				m_link_display_mode = link_display_mode_line;
		}


		// draw links
		std::list<DTALink*>::iterator iLink;
		std::list<DTALink*>::iterator iLinkDisplay;

		CPoint FromPoint,ToPoint;
		CRect link_rect;




		CPen pen_freeway, pen_ramp, pen_arterial, pen_connector,pen_transit,pen_walking;
		CBrush brush_freeway, brush_ramp, brush_arterial, brush_connector,brush_walking;

		pen_freeway.CreatePen (PS_SOLID, 1, theApp.m_FreewayColor);
		brush_freeway.CreateSolidBrush (theApp.m_FreewayColor);

		pen_ramp.CreatePen (PS_SOLID, 1, theApp.m_RampColor);
		brush_ramp.CreateSolidBrush (theApp.m_RampColor);

		pen_arterial.CreatePen (PS_SOLID, 1, theApp.m_ArterialColor);
		brush_arterial.CreateSolidBrush (theApp.m_ArterialColor);

		pen_connector.CreatePen (PS_SOLID, 1, theApp.m_ConnectorColor);
		brush_connector.CreateSolidBrush (theApp.m_ConnectorColor);

		pen_transit.CreatePen (PS_SOLID, 1, theApp.m_TransitColor);

		pen_walking.CreatePen (PS_SOLID, 1, theApp.m_WalkingColor);
		brush_walking.CreateSolidBrush (theApp.m_WalkingColor);

		// recongenerate the lind band width offset only when chaning display mode or on volume mode
		if(	pDoc -> m_PrevLinkMOEMode != pDoc -> m_LinkMOEMode || 
			pDoc->m_LinkMOEMode == MOE_density || 
			pDoc->m_LinkMOEMode == MOE_impact ||	
			pDoc->m_LinkMOEMode == MOE_bottleneck ||	
			pDoc->m_LinkMOEMode == MOE_volume || 
			pDoc->m_LinkMOEMode == MOE_speed ||
			pDoc->m_LinkMOEMode == MOE_emissions ||
			pDoc->m_LinkMOEMode == MOE_safety) 
		{
			pDoc->GenerateOffsetLinkBand();
		}

		if (pDoc->m_LinkMOEMode == MOE_density || pDoc->m_LinkMOEMode == MOE_volume || pDoc->m_LinkMOEMode == MOE_bottleneck || pDoc->m_LinkMOEMode == MOE_speed || pDoc->m_LinkMOEMode == MOE_emissions)   // under volume mode, dynamically generate volume band
		{
			pDoc->m_LinkBandWidthMode = LBW_link_volume;
		}else if (pDoc->m_LinkMOEMode == MOE_impact) 
		{
		
			pDoc->m_LinkBandWidthMode = LBW_congestion_duration; 
		
		}else
		{
			pDoc->m_LinkBandWidthMode = LBW_number_of_lanes;  // e.g. queue
		}

		if(pDoc->m_LinkSet.size() > 0)  // update for a network with links
		{
			pDoc -> m_PrevLinkMOEMode = pDoc -> m_LinkMOEMode;
		}

		// step 3: draw all links
		int min_x, min_y, max_x, max_y;

		pDC->SelectObject(&g_BrushLinkBand);   //default brush  , then MOE, then apply speical conditions for volume and vehicle mode

		//	TRACE("connector layer: %d\n", pMainFrame->m_bShowLayerMap[layer_connector]);

		for (iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
		{


			if( pMainFrame->m_bShowLayerMap[layer_transit] == true && m_bShowTransitLinksOnly )
			{
				if (pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].IsTransit  ()== false)  //show transit only
					continue; 
			}


			if( pMainFrame->m_bShowLayerMap[layer_transit] == true && m_bShowWalkLinksOnly )
			{
				if (pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].IsWalking  ()== false)  //show walking only
					continue; 
			}

			if( !pMainFrame->m_bShowLayerMap[layer_connector] )
			{
				if (pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].IsConnector ())  //hide connectors
					continue; 
			}

			if( !pMainFrame->m_bShowLayerMap[layer_transit] )
			{
				if (pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].IsTransit  () || pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].IsWalking   ())  //hide transit
					continue; 
			}


			// step 1: decide if a link is included in the screen region
			FromPoint = NPtoSP((*iLink)->m_FromPoint);
			ToPoint = NPtoSP((*iLink)->m_ToPoint);

			min_x = min(FromPoint.x,ToPoint.x);
			max_x = max(FromPoint.x,ToPoint.x);

			min_y = min(FromPoint.y,ToPoint.y);
			max_y = max(FromPoint.y,ToPoint.y);

			int size = 1000;
			link_rect.SetRect(min_x-size,min_y-size,max_x+size,max_y+size);

			if(RectIsInsideScreen(link_rect,ScreenRect) == false)  // not inside the screen boundary
				continue;


			// step 4: select color and brush for MOE mode

			//			continue;
			CPen LinkTypePen;


			// step 5: select color and brush according to link MOE
			float value = -1.0f ;

			if( pDoc->m_LinkMOEMode != MOE_none) 
			{

				if ( (*iLink)->m_FromNodeNumber == 78424 && (*iLink)->m_ToNodeNumber == 61158 && g_Simulation_Time_Stamp>=1320) 
				{

					TRACE("trace");

				}

				// dynamically create LOS pen and brush  
				float power =0 ; int LOS = 1;

				if(pDoc->m_LinkMOEMode == MOE_user_defined)
				{ 
					int current_time  =(int)g_Simulation_Time_Stamp;
					if( current_time < (*iLink)->m_LinkMOEArySize )

					{
						power = (*iLink)->m_LinkMOEAry [current_time].UserDefinedValue ;
						value = (*iLink)->m_LinkMOEAry [current_time].UserDefinedValue ;

					}else
					{
						power = 0;
					}
				}else
				{
					power= pDoc->GetLinkMOE((*iLink), pDoc->m_LinkMOEMode,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);
				}

				if(	pDoc->m_LinkMOEMode == MOE_impact)
				{

				int RelativeStartTime = (*iLink)->GetImpactRelativeStartTime(g_Simulation_Time_Stamp,pDoc->m_PrimaryDataSource);
				power = RelativeStartTime*1.0/g_ImpactStudyPeriodInMin*100;

				if(power>100)
					power = 100;

				}

				LOS = pDoc->GetLOSCode(power);

				//draw link as lines
				if(m_link_display_mode == link_display_mode_line )
				{
					if(pDoc->m_LinkMOEMode != MOE_volume) // not volume display
					{
						pDC->SelectObject(&pen_moe[LOS]);
						pDC->SelectObject(&g_BrushLinkBand);   //default brush  , then MOE, then condition with volume
					}else
					{
						pDC->SelectObject(&g_BlackPen);
						pDC->SelectObject(&g_BlackBrush);

					}
				}
				else 
				{// display link as a band so use black pen
					pDC->SelectObject(&g_BlackPen);
					pDC->SelectObject(&brush_moe[LOS]);
				}

			}
			else if(m_bShowLinkType)  // show link type
			{
				//		COLORREF link_color = pDoc->GetLinkTypeColor((*iLink)->m_link_type );
				//		LinkTypePen.CreatePen(PS_SOLID, 1, link_color);
				//		pDC->SelectObject(&LinkTypePen);

				if ( pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsFreeway ())
				{
					pDC->SelectObject(&pen_freeway);
					pDC->SelectObject(&brush_freeway);
				}else if ( pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsArterial())
				{
					pDC->SelectObject(&pen_arterial);
					pDC->SelectObject(&brush_arterial);
				}else if ( pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsRamp())
				{
					pDC->SelectObject(&pen_ramp);
					pDC->SelectObject(&brush_ramp);
				}else if ( pDoc->m_LinkTypeMap[(*iLink)->m_link_type] .IsConnector())
				{
					pDC->SelectObject(&pen_connector);
					pDC->SelectObject(&brush_connector);
				}else if ( pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsTransit())
				{
					pDC->SelectObject(&pen_transit);
					pDC->SelectObject(&g_TransitCrash);
				}else if ( pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsWalking())
				{
					pDC->SelectObject(&pen_walking);
					pDC->SelectObject(&brush_walking);
				}else
				{
					pDC->SelectObject(&pen_arterial);
					pDC->SelectObject(&g_BrushLinkBand);   //default brush
				}

				// special condition for subarea link
				if(m_bHighlightSubareaLinks &&(*iLink)->m_bIncludedinSubarea && pDoc->m_SubareaShapePoints.size()>=3)
					pDC->SelectObject(&g_SubareaLinkPen);

				if(m_bHighlightSubareaBoundaryLinks &&(*iLink)->m_bIncludedinBoundaryOfSubarea  && pDoc->m_SubareaShapePoints.size()>=3)
					pDC->SelectObject(&g_SubareaBoundaryLinkPen);

			}else  // default arterial model
			{
				pDC->SelectObject(&pen_arterial);
				pDC->SelectObject(&g_BrushLinkBand);   //default brush
			}

			//special condition 1: vehicle mode

			if( pDoc->m_LinkMOEMode == MOE_vehicle)  // when showing vehicles, use black
				pDC->SelectObject(&g_BlackPen);

			//special condition 2:  volume mode
			if (pDoc->m_LinkMOEMode == MOE_volume)   // under volume mode, dynamically generate volume band
				pDC->SelectObject(&g_BrushLinkBandVolume);   //default brush

			// special condition 3: when a link is selected

			//outside subarea
			if((*iLink)->m_DisplayLinkID>=0 && pMainFrame->m_bShowLayerMap[layer_path]==true)
			{
				g_SelectThickPenColor(pDC,(*iLink)->m_DisplayLinkID);
				pDC->SetTextColor(RGB(255,0,0));
			}
			else if(m_bHighlightSubareaLinks &&(*iLink)->m_bIncludedinSubarea)
			{
				g_SelectThickPenColor(pDC,1);
				pDC->SetTextColor(RGB(255,0,0));
			}else if(m_bHighlightSubareaBoundaryLinks &&(*iLink)->m_bIncludedinBoundaryOfSubarea  && pDoc->m_SubareaShapePoints.size()>=3)
			{
				pDC->SelectObject(&g_SubareaBoundaryLinkPen);
			}else if  ((*iLink)->m_LinkNo == pDoc->m_SelectedLinkNo)
			{
				g_SelectThickPenColor(pDC,0);
				pDC->SetTextColor(RGB(255,0,0));
			}else 
				pDC->SetTextColor(RGB(255,228,181));

			//step 6: draw link as line or band/bar
			if(m_link_display_mode == link_display_mode_line )  
			{
				// calibration mode, do not show volume
				DrawLinkAsLine((*iLink),pDC);
			}else  // as lane group
			{
				if(DrawLinkAsBand((*iLink),pDC,false)==false)
					return;
			}

			CPoint ScenarioPoint = NPtoSP((*iLink)->GetRelativePosition(0.6));  // get relative position of a link 

			if( pMainFrame->m_bShowLayerMap[layer_workzone] == true && 
				((*iLink) ->GetImpactedFlag(g_Simulation_Time_Stamp,true)>=0.01 || (g_Simulation_Time_Stamp <=0.1 && (*iLink) ->GetImpactedFlag(-1,true)>=0.01)))
				DrawBitmap(pDC, ScenarioPoint, IDB_WORKZONE);

			if( pMainFrame->m_bShowLayerMap[layer_ramp] == true && 
				((*iLink) ->GetRampImpactedFlag(g_Simulation_Time_Stamp)>=0.01 || (g_Simulation_Time_Stamp <=0.1 && (*iLink) ->GetRampImpactedFlag(-1)>=0.01)))
				DrawBitmap(pDC, ScenarioPoint, IDB_RAMP);

			if( pMainFrame->m_bShowLayerMap[layer_incident] == true && 
				((*iLink) ->GetImpactedFlag(g_Simulation_Time_Stamp,false)>=0.01 || (g_Simulation_Time_Stamp <=0.1 &&(*iLink) ->GetImpactedFlag(-1,false)>=0.01)))
				DrawBitmap(pDC, ScenarioPoint, IDB_INCIDENT);

			if(  pMainFrame->m_bShowLayerMap[layer_VMS] == true && ((*iLink) ->GetMessageSign(g_Simulation_Time_Stamp)>=0.1 || (g_Simulation_Time_Stamp ==0 && (*iLink) ->MessageSignVector.size()>0)))
				DrawBitmap(pDC, ScenarioPoint, IDB_VMS);

			if (pMainFrame->m_bShowLayerMap[layer_toll] == true && ((*iLink)->GetTollValue(g_Simulation_Time_Stamp) >= 0.1 || (g_Simulation_Time_Stamp == 0 && (*iLink)->TollVector.size() > 0)))
			{

				DrawBitmap(pDC, ScenarioPoint, IDB_TOLL);
//						pDC->SelectObject(&g_BlackPen);
////						pDC->SelectObject(&g_BlackBrush);
//	
//				pDC->Ellipse(ScenarioPoint.x - node_size, ScenarioPoint.y + node_size,
//					ScenarioPoint.x + node_size, ScenarioPoint.y - node_size);

			}

			if( (*iLink)->m_AdditionalCost>=1 && pMainFrame->m_bShowLayerMap[layer_path] == true )
				DrawBitmap(pDC, ScenarioPoint, IDB_LINK_CLOSURE);


			if(pMainFrame->m_bShowLayerMap[layer_ramp] == true)
			{
				if( (*iLink)-> m_bOnRampType == true )
					DrawBitmap(pDC, ScenarioPoint, IDB_ONRAMP);

				if( (*iLink)-> m_bOffRampType == true )
					DrawBitmap(pDC, ScenarioPoint, IDB_OFFRAMP);

				if( (*iLink)-> m_FREEVALSegmentCode ==  FREEVAL_ONR)
					DrawBitmap(pDC, ScenarioPoint, IDB_SEGMENT_ONRAMP);

				if( (*iLink)-> m_FREEVALSegmentCode ==  FREEVAL_OFR )
					DrawBitmap(pDC, ScenarioPoint, IDB_SEGMENT_OFFRAMP);
			}



			if(pMainFrame->m_bShowLayerMap[layer_detector] == true  && (*iLink)->m_bSensorData)  // only during non display mode
			{
				if((*iLink)->m_LinkNo == pDoc->m_SelectedLinkNo)
				{
					pDC->SelectObject(&g_PenSelectColor);
				}else
				{

					pDC->SelectObject(&g_PenSensorColor);
				}		
				CPoint midpoint = NPtoSP((*iLink)->GetRelativePosition(0.5));
				int size = 6;

				if ((*iLink)->m_b_invalid_sensor == true)
					pDC->SelectObject(g_InvalidBrushSensor);
				else
					pDC->SelectObject(g_BrushSensor);

				pDC->Rectangle(midpoint.x-size, midpoint.y-size, midpoint.x+size, midpoint.y+size);

			}

			if (pDoc->m_LinkMOEMode == MOE_volume && (*iLink)->GetSensorLinkHourlyVolume(g_Simulation_Time_Stamp) > 1)   // draw provided AADT
			{
				pDC->SelectObject(&g_BrushLinkReference);   //reference brush
				pDC->SelectObject(&g_PenLinkReference);   //reference pen
				DrawLinkAsBand((*iLink),pDC,true);  // true: second band as observation
			}

			//step 7: show link labels
			if( m_ShowLinkTextMode != link_display_none )
			{

				int node_size = max(2,int(pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution));

				node_size = min(50,node_size);

				int LinkTextFontSize =   10;
				LinkTextFontSize = min(50,m_LinkTextFontSize)*m_LinkTextSize;


				float sqr_value = (FromPoint.x - ToPoint.x)*(FromPoint.x - ToPoint.x) + (FromPoint.y - ToPoint.y)*(FromPoint.y - ToPoint.y);
				float screen_distance = sqrt( sqr_value) ;

				// create rotation font
				CFont link_text_font;

				LOGFONT lf;
				memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
				lf.lfHeight = LinkTextFontSize;        // request a 12-pixel-height font

				int last_shape_point_id = (*iLink) ->m_ShapePoints .size() -1;
				float DeltaX = (*iLink)->m_ShapePoints[last_shape_point_id].x - (*iLink)->m_ShapePoints[0].x;
				float DeltaY = (*iLink)->m_ShapePoints[last_shape_point_id].y - (*iLink)->m_ShapePoints[0].y;

				float theta = atan2(DeltaY, DeltaX);

				float theta_deg = fmod(theta/PI*180,360);

				if(theta_deg < -90)
					theta_deg += 180;

				if(theta_deg > 90)
					theta_deg -= 180;

				lf.lfEscapement = theta_deg*10;
				strcpy(lf.lfFaceName, "Arial");       

				link_text_font.CreateFontIndirect(&lf);

				pDC->SelectObject(&link_text_font);

				// select text string to be displayed

				bool with_text = false;
				CString str_text, str_reference_text;
				str_text.Format (""); 

				if(screen_distance > 50)
					with_text = true;


				// show text condition 1: street name
				// show text condition 2: crash rates
				if( m_ShowLinkTextMode == link_display_number_of_crashes && (*iLink)->m_number_of_all_crashes >= 0.0001)
				{
					str_text.Format ("%6.2f",(*iLink)->m_number_of_all_crashes );
					with_text = true;
				}


				if(m_ShowLinkTextMode == link_display_street_name)
				{
					if((*iLink)->m_Name.length () > 0 && (*iLink)->m_Name!="(null)"  && screen_distance > 100 )
					{
						str_text = (*iLink)->m_Name.c_str ();
						with_text = true;
					}else
					{
						with_text = false;
					}
				}


				if((*iLink)->m_FromNodeNumber == 85 &&  (*iLink)->m_ToNodeNumber == 799)
				{

					TRACE("");
				}

				switch (m_ShowLinkTextMode)
				{
				case link_display_from_id_to_id: 
					str_text.Format ("%d->%d", (*iLink)->m_FromNodeNumber , (*iLink)->m_ToNodeNumber ); break;

				case link_display_link_id: 
					str_text.Format ("%d", (*iLink)->m_LinkID  ); break;

				case link_display_speed_sensor_id: 
					str_text.Format ("%s", (*iLink)->m_SpeedSensorID.c_str ()); break;

				case link_display_count_sensor_id: 
					str_text.Format ("%s", (*iLink)->m_CountSensorID.c_str ()); break;

			case  link_display_speed_limit_:
					str_text.Format ("%.1f",(*iLink)->m_SpeedLimit ); break;
				case link_display_length_:
					str_text.Format ("%.3f",(*iLink)->m_Length  ); break;
	
				case  link_display_free_flow_travel_time_in_min:
					str_text.Format ("%.2f",(*iLink)->m_FreeFlowTravelTime   ); break;
				case  link_display_free_flow_travel_time_in_hour:
					str_text.Format ("%.3f",(*iLink)->m_FreeFlowTravelTime/60.0   ); break;

				case link_display_saturation_flow_rate:
					str_text.Format ("%.0f",(*iLink)->m_Saturation_flow_rate_in_vhc_per_hour_per_lane); break;
				case link_display_number_of_lanes:
					str_text.Format ("%d",(*iLink)->m_NumberOfLanes ); break;						

				case link_display_number_of_left_turn_lanes:

					if((*iLink)->m_NumberOfLeftTurnLanes>=1)
						str_text.Format ("%d",(*iLink)->m_NumberOfLeftTurnLanes  );

					break;						

				case link_display_number_of_right_turn_lanes:

					if((*iLink)->m_NumberOfRightTurnLanes >=1)
					{
						str_text.Format ("%d",(*iLink)->m_NumberOfRightTurnLanes  );
					}
					break;		


				case link_display_length_of_left_turn_lanes:
					if((*iLink)->m_NumberOfLeftTurnLanes>=1)
						str_text.Format ("%.5f",(*iLink)->m_LeftTurnLaneLength  ); 

					break;						

				case link_display_length_of_right_turn_lanes:

					if((*iLink)->m_RightTurnLaneLength>=1)
						str_text.Format ("%.5f",(*iLink)->m_RightTurnLaneLength  ); break;		


				case link_display_lane_capacity_per_hour:

					str_text.Format ("%.0f",(*iLink)->m_LaneCapacity); break;

				case link_display_link_capacity_per_hour:
					str_text.Format ("%.0f",(*iLink)->m_LaneCapacity*(*iLink)->m_NumberOfLanes  ); break;


				case link_display_BPR_alpha_term:
					str_text.Format ("%.4f",(*iLink)->m_BPR_alpha_term   ); break;

				case link_display_BPR_beta_term:
					str_text.Format ("%.4f",(*iLink)->m_BPR_beta_term    ); break;

				case link_display_link_grade:
					str_text.Format ("%.2f",(*iLink)->m_Grade  ); break;

				case link_display_jam_density_in_vhc_pmpl:
					str_text.Format ("%.0f",(*iLink)->m_Kjam   ); break;

				case link_display_wave_speed_in_mph:
					str_text.Format ("%.0f",(*iLink)->m_Wave_speed_in_mph  ); break;

				case link_display_link_type_in_text:
					str_text.Format ("%s",pDoc->m_LinkTypeMap[(*iLink)->m_link_type ].link_type_name.c_str ()  ); break;

				case link_display_link_type_in_number:
					str_text.Format ("%d", (*iLink)->m_link_type  ); break;

				case link_display_internal_link_id:
					str_text.Format ("%d",(*iLink)->m_LinkNo   ); break;

				case link_display_link_id_speed_sensorid:

					if ((*iLink)->m_SpeedSensorID.size() >= 1)
					{
						str_text.Format("%s", (*iLink)->m_SpeedSensorID.c_str());
						with_text = true;
					}
					
					break;
				case link_display_volume_over_capacity_ratio:
					str_text.Format ("%.2f",(*iLink)->m_volume_over_capacity_ratio    ); break;

				case link_display_LevelOfService:
					str_text.Format ("%c",(*iLink)->m_LevelOfService    ); break;

				case link_display_avg_waiting_time_on_loading_buffer:
					if( (*iLink)->m_avg_waiting_time_on_loading_buffer > 0.1)
						str_text.Format ("%.1f",(*iLink)->m_avg_waiting_time_on_loading_buffer   ); break;

				case link_display_avg_simulated_speed_mph:
					str_text.Format ("%.0f",(*iLink)->m_avg_simulated_speed    ); break;

				case link_display_avg_simulated_speed_kmph:
					str_text.Format ("%.0f",(*iLink)->m_avg_simulated_speed*1.6093   ); break;

				case link_display_total_sensor_link_volume:

					if((*iLink)->m_total_sensor_link_volume >= 1)
						str_text.Format ("%.0f",(*iLink)->m_total_sensor_link_volume     );
					break;

				case link_display_total_sensor_vs_simulated_link_volume:
					if((*iLink)->m_total_sensor_link_volume >=1)
						str_text.Format ("%.0f vs. %.0f",(*iLink)->m_total_sensor_link_volume , (*iLink)->m_total_link_volume    ); 

					break;


				case link_display_total_link_volume:
					str_text.Format ("%.0f", (*iLink)->m_total_link_volume    ); 

					break;

				case link_display_total_link_count_error:
					if((*iLink)->m_total_sensor_link_volume >=1)
						str_text.Format ("%.0f",(*iLink)->m_total_sensor_link_volume - (*iLink)->m_total_link_volume    ); 

					break;

				case link_display_simulated_AADT:
					if((*iLink)->m_simulated_AADT >=1)
						str_text.Format ("%.0f",(*iLink)->m_simulated_AADT     ); 
					break;


				case link_display_observed_peak_hourly_volume:
					if((*iLink)->m_observed_peak_hourly_volume >=1)
						str_text.Format ("%d",(*iLink)->m_observed_peak_hourly_volume     ); 
					break;

				case link_display_avg_delay:
					if((*iLink)->m_total_delay >=1 && (*iLink)->m_total_delay/ max(1,(*iLink)->m_total_link_volume) >= 0.1 && pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsConnector () == false)
						str_text.Format ("%.1f",(*iLink)->m_total_delay/ max(1,(*iLink)->m_total_link_volume)   );
					break;


				case link_display_total_delay:
					if((*iLink)->m_total_delay>=1 && pDoc->m_LinkTypeMap[(*iLink)->m_link_type].IsConnector () == false)
						str_text.Format ("%.0f",(*iLink)->m_total_delay/60.0   );

					break;


				case link_display_total_assigned_link_volume:
					if((*iLink)->m_total_assigned_link_volume >=1)
						str_text.Format ("%.0f",(*iLink)->m_total_assigned_link_volume   );

					break;

				case link_display_total_incomplete_link_volume:
					if((*iLink)->m_total_link_volume_of_incomplete_trips >=1)
						str_text.Format ("%.0f",(*iLink)->m_total_link_volume_of_incomplete_trips   );

					break;
				case link_display_avg_travel_time:
					str_text.Format ("%.1f",(*iLink)->GetAggregatedSimulatedTravelTime(pDoc->m_DemandLoadingStartTimeInMin , pDoc->m_DemandLoadingEndTimeInMin - pDoc->m_DemandLoadingStartTimeInMin)  );
					break;



				case link_display_time_dependent_link_volume:

					pDoc->GetLinkMOE((*iLink), MOE_volume,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);

					value = max(0,value);

					str_text.Format ("%.0f",value);
					break;

				case link_display_time_dependent_lane_volume:

					pDoc->GetLinkMOE((*iLink), MOE_volume,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);
					str_text.Format ("%.0f",value/max(1,(*iLink)->m_NumberOfLanes) );
					break;



				case link_display_time_dependent_speed_mph:
					pDoc->GetLinkMOE((*iLink), MOE_speed,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);
					str_text.Format ("%.0f",value);
					break;
				case link_display_time_dependent_speed_kmph:
					pDoc->GetLinkMOE((*iLink), MOE_speed,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);
					str_text.Format ("%.0f",value*1.6093);
					break;

				case link_display_time_dependent_density:
					pDoc->GetLinkMOE((*iLink), MOE_density,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);

					if(value > 0.1)
					{
						str_text.Format ("%.0f",value);
					}
					break;

				case link_display_time_dependent_queue_length:
					pDoc->GetLinkMOE((*iLink), MOE_queue_length,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);

					if(value>0.01)
					{
						str_text.Format ("%.1f%%",value*100);
					}
					break;


				case link_display_time_dependent_congestion_duration:
					value = (*iLink)->GetImpactDuration ((int)g_Simulation_Time_Stamp,pDoc->m_PrimaryDataSource);

					if(value>0.01)
					{
						str_text.Format ("%.0f min",value);
					}
					break;

					with_text = true;
				case link_display_time_dependent_congestion_start_time:
					value = (*iLink)->GetImpactRelativeStartTime  ((int)g_Simulation_Time_Stamp,pDoc->m_PrimaryDataSource);

					if(value <9999)
					{
						str_text.Format ("%s", pDoc->GetTimeStampString24HourFormat(g_Simulation_Time_Stamp+value));
					}
					break;

					with_text = true;
				}
				if(pDoc->m_bShowCalibrationResults && pDoc->m_LinkMOEMode == MOE_volume)  // special conditions with calibration mode
				{
					if((*iLink)->m_bSensorData )
					{
						str_text.Format ("%.0f",value);
						float percentage_error = (value- (*iLink)->m_ReferenceFlowVolume) / max(1,(*iLink)->m_ReferenceFlowVolume)*100;
						//									str_reference_text.Format ("%.0f, %.0f%%",(*iLink)->m_ReferenceFlowVolume,percentage_error);
						str_reference_text.Format ("%.0f",(*iLink)->m_ReferenceFlowVolume);

						with_text = true;

					}else
					{  // do not show data without sensors
						with_text = false; 
					}

				}


				// after all the above 3 conditions, show test now. 
				if(with_text)
				{
					CPoint TextPoint = NPtoSP((*iLink)->GetRelativePosition(0.3));
					pDC->SetBkColor(RGB(0,0, 0));

					if((*iLink)->m_LinkNo == pDoc->m_SelectedLinkNo)
					{
						pDC->SetTextColor(RGB(255,0,0));
					}else
					{
						pDC->SetTextColor(RGB(0,0,0));
					}


					pDC->TextOut(TextPoint.x,TextPoint.y - m_LinkTextFontSize*1.5, str_text);

					if(str_reference_text.GetLength () > 0 )  // reference text with different color4
					{
						pDC->SetTextColor(RGB(128, 255, 255));
						pDC->TextOut(TextPoint.x,TextPoint.y+m_LinkTextFontSize, str_reference_text);
					}

				}

				// step 8:  show location of scenario/incident /work zone/ toll: link-based objects
				pDC->SetBkColor(RGB(0, 0, 0));

				//************************************

				// step 9: draw sensor (based on a link)
			}
		}


	}


}

void CTLiteView::FitNetworkToScreen()
{
	CRect ScreenRect;
	GetClientRect(ScreenRect);
	m_ScreenOrigin = ScreenRect.CenterPoint ();

	CTLiteDoc* pDoc = GetDocument();

	if(pDoc==NULL)
		return;

	double res_wid = ScreenRect.Width()/(pDoc->m_NetworkRect.Width()+0.000001);
	double res_height = ScreenRect.Height()/(pDoc->m_NetworkRect.Height()+0.000001);

	m_Resolution = min(res_wid, res_height);  

	//+1 to avide devided by zero error;

	m_Resolution*=0.8f;

	m_Origin.x = pDoc->m_NetworkRect.Center ().x ;
	m_Origin.y = pDoc->m_NetworkRect.Center ().y ;

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	pMainFrame->UpdateLegendView();

	//	TRACE("Wid: %d, %f, m_Resolution=%f,m_Origin: x = %f\n",ScreenRect.Width(),pDoc->m_NetworkRect.Width(), m_Resolution, m_Origin.x);

}


// CTLiteView message handlers

void CTLiteView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	OnViewShownetwork();
}


BOOL CTLiteView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// change image size
	CTLiteDoc* pDoc = GetDocument();

	if(m_ToolMode != backgroundimage_tool && m_ToolMode !=  network_coordinate_tool)  //select, move
	{
		if(zDelta > 0)
		{
			if(m_Resolution*1.6 * max(pDoc->m_NetworkRect.Width () , pDoc->m_NetworkRect.Height  () ) <  INT_MAX) 
			{
				m_Resolution*=1.5f;
			}
		}
		else
		{
			m_Resolution/=1.5f;
		}
	}else
	{

		if(m_ToolMode == network_coordinate_tool)
		{

			int LayerNo = 1;

			// change image size
			CTLiteDoc* pDoc = GetDocument();
			float increase_ratio = 1.005f;
			float decrease_ratio = 1.0f / increase_ratio;

			if(zDelta > 0)
			{

				// control -> Y only
				// shift -> X only



				if(nFlags != MK_CONTROL)  // shift or nothing
				{
					pDoc->ChangeNetworkCoordinates(LayerNo,increase_ratio,1.0f,0.0f,0.0f);
				}

				if(nFlags != MK_SHIFT)  // control or nothing
				{
					pDoc->ChangeNetworkCoordinates(LayerNo,1.0f,increase_ratio,0.0f,0.0f);
				}
			}
			else
			{
				if(nFlags != MK_CONTROL)  // shift or nothing
				{
					pDoc->ChangeNetworkCoordinates(LayerNo,decrease_ratio,1.0f,0.0f,0.0f);
				}

				if(nFlags != MK_SHIFT)  // control or nothing
				{
					pDoc->ChangeNetworkCoordinates(LayerNo,1.0f,decrease_ratio,0.0f,0.0f);
				}
			}
		}


		if(m_ToolMode == backgroundimage_tool)
		{

			if(zDelta > 0)
			{

				// control -> Y only
				// shift -> X only

				if(nFlags != MK_CONTROL)  // shift or nothing
					pDoc->m_ImageXResolution*=1.02f;

				if(nFlags != MK_SHIFT)  // control or nothing
					pDoc->m_ImageYResolution*=1.02f;
			}
			else
			{
				if(nFlags != MK_CONTROL)  // shift or nothing
					pDoc->m_ImageXResolution/=1.02f;

				if(nFlags != MK_SHIFT)  // control or nothing
					pDoc->m_ImageYResolution/=1.02f;
			}
		}
	}
	SetGlobalViewParameters();
	CRect ScreenRect;
	GetClientRect(ScreenRect);

	CPoint LeftTop, RightBottom;
	LeftTop.x = ScreenRect.left ;
	LeftTop.y = ScreenRect.top  ;

	RightBottom.x = ScreenRect.right ;
	RightBottom.y = ScreenRect.bottom ;

	GDPoint  gdpt_lt  = SPtoNP(LeftTop);
	GDPoint  gdpt_rb  = SPtoNP(RightBottom);

	double width = fabs(gdpt_lt.x - gdpt_rb.x )/ pDoc->m_UnitMile;
	double height = fabs(gdpt_lt.y - gdpt_rb.y )/ pDoc->m_UnitMile;

	CString str;

	GDPoint  gdpt = SPtoNP(pt);
	str.Format("%.5f,%.5f", gdpt.x, gdpt.y);
	pDoc->SendTexttoStatusBar(str);

	if(pDoc->m_bUseMileVsKMFlag)
		str.Format("width: %.1f mi; height: %.1f mi",  width, height);
	else
		str.Format("width: %.1f km; height: %.1f km",  width, height);

	pDoc->m_ScreenWidth_InMile = width;

	pDoc->SendTexttoStatusBar(str,1);
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	pMainFrame->UpdateLegendView();
	Invalidate();

	return TRUE;
}

BOOL CTLiteView::OnEraseBkgnd(CDC* pDC)
{

	return TRUE;
}

void CTLiteView::OnViewZoomin()
{

	CTLiteDoc* pDoc = GetDocument();

	if(m_Resolution*1.2 * max(pDoc->m_NetworkRect.Width () , pDoc->m_NetworkRect.Height  () ) <  INT_MAX) 
	{
		m_Resolution*=1.1f;

	}

	SetGlobalViewParameters();

	Invalidate();
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	pMainFrame->UpdateLegendView();

}

void CTLiteView::OnViewZoomout()
{
	m_Resolution/=1.1f;

	SetGlobalViewParameters();
	Invalidate();
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	pMainFrame->UpdateLegendView();
}

void CTLiteView::OnViewShownetwork()
{
	FitNetworkToScreen();

	Invalidate();
}

void CTLiteView::OnViewMove()
{
	m_ToolMode = move_tool;
}

int CTLiteView::FindClosestNode(CPoint point, float Min_distance)
{
	int SelectedNodeID = -1;
	CTLiteDoc* pDoc = GetDocument();

	std::list<DTANode*>::iterator iNode;

	for (iNode = pDoc->m_NodeSet.begin(); iNode != pDoc->m_NodeSet.end(); iNode++)
	{
		CPoint NodePoint = NPtoSP((*iNode)->pt);

		CSize size = NodePoint - point;
		double distance = pow((size.cx*size.cx + size.cy*size.cy),0.5);
		if( distance < Min_distance)
		{
			SelectedNodeID = (*iNode)->m_NodeNo ;
			Min_distance = distance;
		}

	}

	//	if(Min_distance > pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution*20)
	//	{
	//		SelectedNodeID = -1;
	//	}
	return SelectedNodeID;
}

int CTLiteView::FindClosestZone(CPoint point, float Min_distance)
{
	CTLiteDoc* pDoc = GetDocument();
	if(pDoc !=NULL)
	{
		GDPoint pt = SPtoNP (point);
		int ZoneID = pDoc->GetZoneID(pt);

		if(ZoneID > 0)
			return ZoneID;

	}
	return -1;
}
void CTLiteView::OnLButtonDown(UINT nFlags, CPoint point)
{

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	//speical condition first



	CTLiteDoc* pDoc = GetDocument();

	if (m_ToolMode == move_tool || (m_ToolMode == move_node_tool && pDoc->m_SelectedNodeID <= -1))
	{
		m_last_cpoint = point;
		m_last_left_down_point  = point;
		g_SetCursor(_cursor_movement_network);
		m_bMoveDisplay = true;
	}



	if(m_ToolMode == select_feature_tool || m_ToolMode == select_link_tool  )
	{
		m_last_cpoint = point;
		m_last_left_down_point  = point;
		m_bMoveDisplay = true;
	}

	if(m_ToolMode == backgroundimage_tool)
	{
		m_last_cpoint = point;
		g_SetCursor(_cursor_movement_network);
		m_bMoveImage = true;
	}

	if (m_ToolMode == sensor_location_move_tool)
	{
		m_last_cpoint = point;
		g_SetCursor(_cursor_movement_network);
		m_bMoveSensorMap = true;
	}
	if(m_ToolMode == network_coordinate_tool)
	{
		m_last_cpoint = point;
		g_SetCursor(_cursor_movement_network);
		m_bMoveNetwork = true;
	}


	if(m_ToolMode == create_1waylink_tool || m_ToolMode == create_2waylinks_tool)
	{
		m_TempLinkStartPoint = point;
		m_TempLinkEndPoint = point;
		m_bMouseDownFlag = true;


		g_SetCursor(_cursor_create_link);


	}

	if(m_ToolMode == move_node_tool)
	{
		pDoc->m_SelectedNodeID = FindClosestNode(point, pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution*2.0f);
		m_bMouseDownFlag = true;

	}


	if(m_ToolMode == subarea_tool || m_ToolMode == add_zone_tool)
	{
		if(m_ToolMode == subarea_tool)
			g_SetCursor(_cursor_create_subarea);

		if(m_ToolMode == add_zone_tool)
			g_SetCursor(_cursor_create_zone);

		m_TempZoneStartPoint = point;
		m_TempZoneEndPoint = point;

		m_bMouseDownFlag = true;

		if(!isCreatingSubarea && GetDocument()->m_SubareaShapePoints.size()==0)
		{
			// Record the start location and find the closest feature point
			m_FirstSubareaPoints = point;
			GetDocument()->m_SubareaShapePoints.push_back(SPtoNP(point));

			isCreatingSubarea = true;
			SetCapture();
			m_last_left_down_point = point;
		}
		else
		{
			if(bFindCloseSubareaPoint(point) && GetDocument()->m_SubareaShapePoints.size()>= 3)
		 {
			 GetDocument()->m_SubareaShapePoints.push_back(SPtoNP(point));
			 GetDocument()->m_SubareaShapePoints.push_back(GetDocument()->m_SubareaShapePoints[0]);
			 isCreatingSubarea = false;
			 CopyLinkSetInSubarea();
			 isFinishSubarea = true;
			 if(m_ToolMode == add_zone_tool)
			 {
				 GetDocument()-> OnSubareaCreatezonefromsubarea();

			 }else
			 {

				 GetDocument()->PushBackNetworkState ();
			 }
			 m_ToolMode = move_tool;
			 ReleaseCapture();
			 m_last_left_down_point = point;

			 m_last_cpoint = point;


			 Invalidate();



			 //FitNetworkToScreen();


		 }
			else
		 {
			 GetDocument()->m_SubareaShapePoints.push_back(SPtoNP(point));
		 }
		}
	}

	CView::OnLButtonDown(nFlags, point);
}


void CTLiteView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CTLiteDoc* pDoc = GetDocument();
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	if (m_ToolMode == move_tool || (m_ToolMode == move_node_tool && pDoc->m_SelectedNodeID <= -1))
	{
		CSize OffSet = point - m_last_cpoint;
		m_Origin.x -= OffSet.cx/m_Resolution;
		m_Origin.y -= OffSet.cy*m_OriginOnBottomFlag/m_Resolution;

		g_SetCursor(_cursor_standard_arrow);
		SetGlobalViewParameters();

		m_bMoveDisplay = false;
	}

	if(m_ToolMode == move_node_tool && pDoc->m_SelectedNodeID>=0)  //  a node has been selected
	{

		MoveNode(pDoc->m_SelectedNodeID,point);
		pDoc->m_bLinkToBeShifted = true;
		pDoc->OffsetLink();
		pDoc->GenerateOffsetLinkBand();
		m_bMouseDownFlag = false;

	}


	if(m_ToolMode == select_link_tool)
	{
		CSize OffSet_ButtonDownUp = point - m_last_left_down_point;
		CSize OffSet = point - m_last_cpoint;

		// use move tool mode when offset is positive

		if((abs(OffSet_ButtonDownUp.cx) + abs(OffSet_ButtonDownUp.cx)) >=2) 
		{
			m_Origin.x -= OffSet.cx/m_Resolution;
			m_Origin.y -= OffSet.cy*m_OriginOnBottomFlag/m_Resolution;

			g_SetCursor(_cursor_standard_arrow);

			SetGlobalViewParameters();

			m_bMoveDisplay = false;
		}else
		{	// same point for LButton down and up messages

		}
	}

	if(m_ToolMode == select_feature_tool)
	{
		CSize OffSet_ButtonDownUp = point - m_last_left_down_point;
		CSize OffSet = point - m_last_cpoint;
		// use move tool mode when offset is positive
		if((abs(OffSet_ButtonDownUp.cx) + abs(OffSet_ButtonDownUp.cx)) >=2) 
		{
			m_Origin.x -= OffSet.cx/m_Resolution;
			m_Origin.y -= OffSet.cy*m_OriginOnBottomFlag/m_Resolution;

			g_SetCursor(_cursor_standard_arrow);

			SetGlobalViewParameters();

			m_bMoveDisplay = false;
		}else
		{ // same point for LButton down and up messages

			switch (pMainFrame->m_iSelectedLayer )
			{
			case layer_node:

				pDoc->m_SelectedLinkNo = -1;
				for (std::list<DTALink*>::iterator iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
				{
					(*iLink)->m_DisplayLinkID = -1;
				}

				pDoc->m_SelectedNodeID = FindClosestNode(point, pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution*2.0f);

				if(pDoc->m_SelectedNodeID >= 0)
				{

					pMainFrame->m_FeatureInfoVector.clear();

					DTANode* pNode = pDoc->m_NodeNoMap [pDoc->m_SelectedNodeID ];
					CFeatureInfo element;
					element.Attribute = "Node ID";
					element.Data.Format ("%d",pNode->m_NodeNumber  );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					element.Attribute = "Control Type";

					if(pNode->m_ControlType == pDoc->m_ControlType_UnknownControl) element.Data.Format ("Unknown Control" );
					if(pNode->m_ControlType == pDoc->m_ControlType_ExternalNode) element.Data.Format ("External Node" );
					if(pNode->m_ControlType ==  pDoc->m_ControlType_NoControl) element.Data.Format ("No Control" );
					if(pNode->m_ControlType ==  pDoc->m_ControlType_YieldSign) element.Data.Format ("Yield Sign" );
					if(pNode->m_ControlType ==  pDoc->m_ControlType_2wayStopSign) element.Data.Format ("2 Way Stop" );
					if(pNode->m_ControlType ==  pDoc->m_ControlType_4wayStopSign) element.Data.Format ("4 Way Stop" );
					if(pNode->m_ControlType ==  pDoc->m_ControlType_PretimedSignal) element.Data.Format ("Pretimed Signal" );
					if(pNode->m_ControlType ==  pDoc->m_ControlType_ActuatedSignal) element.Data.Format ("actuated Signal" );
					if(pNode->m_ControlType ==  pDoc->m_ControlType_Roundabout) element.Data.Format ("Roundabout" );

					pMainFrame->m_FeatureInfoVector.push_back (element);

					element.Attribute = "x";
					element.Data.Format ("%f",pNode->pt.x   );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					element.Attribute = "y";
					element.Data.Format ("%f",pNode->pt.y   );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					element.Attribute = "Associated Zone ID";
					element.Data.Format ("%d",pNode->m_ZoneID   );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					if(pNode->m_ControlType ==  pDoc->m_ControlType_PretimedSignal ||
						pNode->m_ControlType ==  pDoc->m_ControlType_ActuatedSignal)
					{
						element.Attribute = "Cycle Length";
						element.Data.Format ("%d sec",pNode->m_CycleLengthInSecond );
						pMainFrame->m_FeatureInfoVector.push_back (element);
					}

					if(pNode->m_Name .size()>0 && pNode->m_Name!="(null)")
					{
						element.Attribute = "Street Name";
						element.Data.Format ("%s",pNode->m_Name.c_str ()   );

						pMainFrame->m_FeatureInfoVector.push_back (element);
					}


					pMainFrame->FillFeatureInfo ();

				}else
				{
					pMainFrame->m_FeatureInfoVector.clear();
					pMainFrame->FillFeatureInfo ();
				}


				break;

			case layer_zone:

				pDoc->m_SelectedZoneID = FindClosestZone(point,0);

				pMainFrame->m_FeatureInfoVector.clear();

				if(pDoc->m_SelectedZoneID>=0)
				{
					CFeatureInfo element;
					element.Attribute = "Zone ID";
					element.Data.Format ("%d", pDoc->m_SelectedZoneID );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					pDoc->m_SelectedLinkNo = -1;
					for (std::list<DTALink*>::iterator iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
					{
						(*iLink)->m_DisplayLinkID = -1;
					}

					pDoc->m_SelectedNodeID = -1;


					pMainFrame->FillFeatureInfo ();
				}

				break;
			case layer_link:
			case layer_connector:
			case layer_link_MOE:
			case layer_detector:
			case layer_workzone:
			case layer_ramp:
			case layer_incident:


			case layer_VMS:
			case layer_toll:
			case layer_path:


				pDoc->m_SelectedNodeID = -1;
				OnClickLink(nFlags, point);

				if(pDoc->m_SelectedLinkNo >= 0)
				{

					pMainFrame->m_FeatureInfoVector.clear();

					DTALink* pLink = pDoc->m_LinkNoMap [pDoc->m_SelectedLinkNo ];
					CFeatureInfo element;
					element.Attribute = "Link ID";
					element.Data.Format ("%d",pLink->m_LinkID  );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					element.Attribute = "name";
					element.Data.Format ("%s",pLink->m_Name.c_str ()  );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					element.Attribute = "From Node ID";
					element.Data.Format ("%d",pLink->m_FromNodeNumber  );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					element.Attribute = "To Node ID";
					element.Data.Format ("%d",pLink->m_ToNodeNumber   );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					if(pDoc->m_LinkTypeMap.find(pLink->m_link_type) != pDoc->m_LinkTypeMap.end())
					{
						element.Attribute = "Type";
						element.Data.Format ("%s",pDoc->m_LinkTypeMap  [ pLink->m_link_type ].link_type_name.c_str () );
						pMainFrame->m_FeatureInfoVector.push_back (element);
					}

					if(pDoc->m_bUseMileVsKMFlag)
						element.Attribute = "Speed Limit";
					else		
						element.Attribute = "Speed Limit (kmph)";

					element.Data.Format ("%3.0f",pLink->m_SpeedLimit );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					if(pDoc->m_bUseMileVsKMFlag)
						element.Attribute = "Length ";
					else
						element.Attribute = "Length (km)";

					element.Data.Format ("%4.3f",pLink->m_Length   );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					element.Attribute = "FFTT (min)";
					element.Data.Format ("%4.3f",pLink->m_FreeFlowTravelTime    );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					element.Attribute = "# of Lanes";
					element.Data.Format ("%d",pLink->m_NumberOfLanes );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					element.Attribute = "Lane Capacity";
					element.Data.Format ("%4.0f",pLink->m_MaximumServiceFlowRatePHPL  );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					element.Attribute = "Link Capacity";
					element.Data.Format ("%4.0f",pLink->m_MaximumServiceFlowRatePHPL*pLink->m_NumberOfLanes   );
					pMainFrame->m_FeatureInfoVector.push_back (element);

					//element.Attribute = "Mode Code";
					//element.Data.Format ("%s",pLink->m_Mode_code   );
					//pMainFrame->m_FeatureInfoVector.push_back (element);


					pMainFrame->FillFeatureInfo ();


				}else
				{
					pMainFrame->m_FeatureInfoVector.clear();	
					pMainFrame->FillFeatureInfo ();
				}

				if(pMainFrame->m_iSelectedLayer == layer_link_MOE)
				{
					g_LinkMOEDlgShowFlag = false;
					pDoc->OnViewShowmoe();
				}

				break;

			}
			m_bMoveDisplay = false;


		}
	}

	if(m_ToolMode == backgroundimage_tool)
	{

		CSize OffSet = point - m_last_cpoint;
		CTLiteDoc* pDoc = GetDocument();
		pDoc->m_ImageX1  += OffSet.cx*pDoc->m_ImageMoveSize;
		pDoc->m_ImageY1  += OffSet.cy*m_OriginOnBottomFlag*pDoc->m_ImageMoveSize;

		g_SetCursor(_cursor_standard_arrow);

		m_bMoveImage = false;
	}

	if (m_ToolMode == sensor_location_move_tool)
	{

		CSize OffSet = point - m_last_cpoint;
		CTLiteDoc* pDoc = GetDocument();
		pDoc->m_SensorMapX += OffSet.cx*m_Resolution;
		pDoc->m_SensorMapY += OffSet.cy*m_OriginOnBottomFlag*m_Resolution;

		g_SetCursor(_cursor_standard_arrow);

		m_bMoveSensorMap = false;
	}
	if(m_ToolMode == network_coordinate_tool)
	{
		g_SetCursor(_cursor_standard_arrow);
		m_bMoveNetwork = false;
	}



	if(m_ToolMode == create_1waylink_tool || m_ToolMode == create_2waylinks_tool)
	{
		m_bMouseDownFlag = false;
		CTLiteDoc* pDoc = GetDocument();

		m_TempLinkEndPoint = point;

		CSize OffSet = m_TempLinkStartPoint - m_TempLinkEndPoint;

		if(abs(OffSet.cx) +  abs(OffSet.cy) < 3)  // clicking on the same point, do not create links
			return;

		pDoc->Modify();
		pDoc->PushBackNetworkState();


		DTANode* pFromNode = 0;// create from node if there is no overlapping node
		float min_selection_distance = max(10,int(pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution));

		min_selection_distance = min(50,min_selection_distance);  // 50 PiX FOR SELECTION 

		int FromNodeID = FindClosestNode(m_TempLinkStartPoint, min_selection_distance);
		if(FromNodeID ==-1)
		{
			pFromNode = pDoc->AddNewNode(SPtoNP(m_TempLinkStartPoint),0,0,false,true);
		}else
		{
			pFromNode = pDoc-> m_NodeNoMap[FromNodeID];
		}

		DTANode* pToNode = 0;// create from node if there is no overlapping node
		int ToNodeID = FindClosestNode(m_TempLinkEndPoint, min_selection_distance);
		if(ToNodeID ==-1)
		{
			pToNode = pDoc->AddNewNode(SPtoNP(m_TempLinkEndPoint),0,0,false,true);
		}else
		{
			pToNode = pDoc-> m_NodeNoMap[ToNodeID];
		}

		// create one way link

		if(m_ToolMode == create_1waylink_tool)
		{
			pDoc->AddNewLink(pFromNode->m_NodeNo, pToNode->m_NodeNo,false);
		}

		// create 2 way links with opposite direction
		if(m_ToolMode == create_2waylinks_tool)
		{
			pDoc->AddNewLink(pFromNode->m_NodeNo, pToNode->m_NodeNo,true);
			pDoc->AddNewLink(pToNode->m_NodeNo,pFromNode->m_NodeNo,true);

		}

		pDoc->m_bFitNetworkInitialized = true; // do not reflesh after adding links

	}

	if(m_ToolMode == create_node_tool)
	{
		CTLiteDoc* pDoc = GetDocument();

		DTANode* pFromNode = 0;// create from node if there is no overlapping node

		float min_selection_distance = max(20,int(pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution));
		int FromNodeID = FindClosestNode(point, min_selection_distance);
		if(FromNodeID ==-1)
		{
			pFromNode = pDoc->AddNewNode(SPtoNP(point),0,0,false,true);
		}

		pDoc->m_bFitNetworkInitialized = true; // do not reflesh after adding links

	}

	Invalidate();
	CView::OnLButtonUp(nFlags, point);
}

void CTLiteView::OnUpdateViewMove(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == move_tool ? 1 : 0);
}

void CTLiteView::OnMouseMove(UINT nFlags, CPoint point)
{
	CTLiteDoc* pDoc = GetDocument();

	if ((m_ToolMode == move_tool || (m_ToolMode == move_node_tool && pDoc->m_SelectedNodeID <= -1) || m_ToolMode == select_link_tool || m_ToolMode == select_feature_tool) && m_bMoveDisplay)
	{
		CSize OffSet = point - m_last_cpoint;
		//		if(!(OffSet.cx !=0 && OffSet.cy !=0))
		{
			m_Origin.x -= OffSet.cx/m_Resolution;
			m_Origin.y -= OffSet.cy*m_OriginOnBottomFlag/m_Resolution;
			m_last_cpoint = point;

		}

		if(m_bMoveDisplay)
			g_SetCursor(_cursor_movement_network);

		SetGlobalViewParameters();

		Invalidate();

	}
	if(m_ToolMode == move_node_tool && pDoc->m_SelectedNodeID>=0 && m_bMouseDownFlag)  //  a node has been selected
	{

		MoveNode(pDoc->m_SelectedNodeID,point);

		pDoc->m_bLinkToBeShifted = true;
		pDoc->OffsetLink();
		pDoc->GenerateOffsetLinkBand();

		Invalidate();

	}

	if(m_ToolMode == backgroundimage_tool && m_bMoveImage)
	{
		CSize OffSet = point - m_last_cpoint;
		//		if(!(OffSet.cx !=0 && OffSet.cy !=0))
		{
			pDoc->m_ImageX1  += OffSet.cx*pDoc->m_ImageMoveSize;
			pDoc->m_ImageY1  += OffSet.cy*m_OriginOnBottomFlag*pDoc->m_ImageMoveSize;

			m_last_cpoint = point;

		}
		if(m_bMoveDisplay)
			g_SetCursor(_cursor_movement_network);

		Invalidate();

	}

	if (m_ToolMode == sensor_location_move_tool && m_bMoveSensorMap)
	{
		CSize OffSet = point - m_last_cpoint;
		//		if(!(OffSet.cx !=0 && OffSet.cy !=0))
		{
			pDoc->m_SensorMapX += OffSet.cx*pDoc->m_SensorMapMoveSize;
			pDoc->m_SensorMapY += OffSet.cy*m_OriginOnBottomFlag*pDoc->m_SensorMapMoveSize;

			m_last_cpoint = point;

		}
		if (m_bMoveSensorMap)
			g_SetCursor(_cursor_movement_network);

		Invalidate();

	}


	if(m_ToolMode == network_coordinate_tool && m_bMoveNetwork)
	{
		CSize OffSet = point - m_last_cpoint;
		//		if(!(OffSet.cx !=0 && OffSet.cy !=0))
		{
			CTLiteDoc* pDoc = GetDocument();
			float delta_x = OffSet.cx/m_Resolution;
			float delta_y = OffSet.cy*m_OriginOnBottomFlag/m_Resolution;

			int LayerNo = m_MoveLayerNo; // for imported network
			pDoc->ChangeNetworkCoordinates(LayerNo,1.0f,1.0f,delta_x,delta_y);

			m_last_cpoint = point;

		}
		Invalidate();

	}

	if(m_ToolMode == create_node_tool )
	{
		g_SetCursor(_cursor_create_node);
	}

	if(m_ToolMode == create_1waylink_tool || m_ToolMode == create_2waylinks_tool)
	{
		g_SetCursor(_cursor_create_link);

		if(m_bMouseDownFlag)
		{
			// if it is the first moving operation, erase the previous temporal link
			if(m_TempLinkStartPoint!=m_TempLinkEndPoint)
				DrawTemporalLink(m_TempLinkStartPoint,m_TempLinkEndPoint);

			// update m_TempLinkEndPoint from the current mouse point
			m_TempLinkEndPoint = point;

			// draw a new temporal link
			DrawTemporalLink(m_TempLinkStartPoint,m_TempLinkEndPoint);
		}



	}
	if(m_ToolMode == subarea_tool  || m_ToolMode == add_zone_tool)
	{
		if(m_ToolMode == subarea_tool )
			g_SetCursor(_cursor_create_subarea);

		if(m_ToolMode == add_zone_tool )
			g_SetCursor(_cursor_create_zone);

		if(isCreatingSubarea)
		{
			// if it is the first moving operation, erase the previous temporal link
			if(m_TempZoneStartPoint!=m_TempZoneEndPoint)
				DrawTemporalLink(m_TempZoneStartPoint,m_TempZoneEndPoint);

			// update m_TempLinkEndPoint from the current mouse point
			m_TempZoneEndPoint = point;

			// draw a new temporal link
			DrawTemporalLink(m_TempZoneStartPoint,m_TempZoneEndPoint);
		}

	}


	CString str;
	GDPoint  gdpt  = SPtoNP(point);
	str.Format("%.5f,%.5f", gdpt.x, gdpt.y);
	GetDocument()->SendTexttoStatusBar(str);

	CRect ScreenRect;
	GetClientRect(ScreenRect);

	CPoint LeftTop, RightBottom;
	LeftTop.x = ScreenRect.left ;
	LeftTop.y = ScreenRect.top  ;

	RightBottom.x = ScreenRect.right ;
	RightBottom.y = ScreenRect.bottom ;

	GDPoint  gdpt_lt  = SPtoNP(LeftTop);
	GDPoint  gdpt_rb  = SPtoNP(RightBottom);

	double width = fabs(gdpt_lt.x - gdpt_rb.x )/ pDoc->m_UnitMile;
	double height = fabs(gdpt_lt.y - gdpt_rb.y )/ pDoc->m_UnitMile;

	str.Format("%.5f,%.5f", gdpt.x, gdpt.y);
	GetDocument()->SendTexttoStatusBar(str);

	if(pDoc->m_bUseMileVsKMFlag)
		str.Format("width: %.1f mi; height: %.1f mi",  width, height);
	else
		str.Format("width: %.1f km; height: %.1f km",  width/1.60934, height/1.60934);

	pDoc->m_ScreenWidth_InMile = width;
	pDoc->SendTexttoStatusBar(str,1);
	CView::OnMouseMove(nFlags, point);
}

void CTLiteView::OnViewSelect()
{
	m_ToolMode = select_link_tool;
}

void CTLiteView::OnUpdateViewSelect(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == select_link_tool ? 1 : 0);
}

void CTLiteView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnRButtonDown(nFlags, point);

	if((m_ToolMode == subarea_tool || m_ToolMode == add_zone_tool)&& GetDocument()->m_SubareaShapePoints.size()>= 3)
	{
		CWaitCursor wait;
		GetDocument()->m_SubareaShapePoints.push_back(SPtoNP(point));
		GetDocument()->m_SubareaShapePoints.push_back(GetDocument()->m_SubareaShapePoints[0]);
		isCreatingSubarea = false;
		CopyLinkSetInSubarea();
		isFinishSubarea = true;
		m_ToolMode = select_link_tool;
		ReleaseCapture();

		m_last_left_down_point = point;

		Invalidate();
	}
}

void CTLiteView::OnNodeOrigin()
{
	CTLiteDoc* pDoc = GetDocument();

	pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

	pDoc->m_OriginNodeID = pDoc->m_SelectedNodeID;
	if(pDoc->m_SelectedNodeID>=0)
		TRACE("ONode %s selected.\n", pDoc->m_NodeNoMap [pDoc->m_OriginNodeID]->m_Name );
	pDoc->Routing(false);

	m_ShowAllPaths = true;
	Invalidate();

}



void CTLiteView::OnNodeDestination()
{
	CTLiteDoc* pDoc = GetDocument();
	pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

	pDoc->m_DestinationNodeID = pDoc->m_SelectedNodeID;
	if(pDoc->m_SelectedNodeID>=0)
		TRACE("ONode %s selected.\n", pDoc->m_NodeNoMap [pDoc->m_DestinationNodeID]->m_Name );

	m_ShowAllPaths = true;
	if(pDoc->Routing(false)==0)
	{
		AfxMessageBox("The selected OD pair does not have a connected path. Please check input_link.csv for details.");

	}

	Invalidate();

}

void CTLiteView::FindAccessibleTripIDWithCurrentMousePoint()
{
	GDPoint pt = SPtoNP(m_CurrentMousePoint);
	CTLiteDoc* pDoc = GetDocument();

	pDoc->FindAccessibleTripID(pt,g_Simulation_Time_Stamp);
}


void CTLiteView::OnContextMenu(CWnd* pWnd, CPoint point)
{

	Invalidate();

	CPoint MenuPoint = point;
	CTLiteDoc* pDoc = GetDocument();
	ScreenToClient(&point);

	m_CurrentMousePoint = point;



	CClientDC dc(this);


	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	//speical condition first


	//if(pMainFrame-> m_iSelectedLayer == layer_zone)
	//{
	//	CMenu cm;
	//	cm.LoadMenu(IDR_MENU1);

	//	if(pDoc->m_SubareaShapePoints .size()>=3)
	//	{
	//		cm.GetSubMenu(layer_subarea)->TrackPopupMenu(
	//		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
	//		MenuPoint.x, MenuPoint.y, AfxGetMainWnd());
	//	}else
	//	{
	//		cm.GetSubMenu(layer_zone)->TrackPopupMenu(
	//		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
	//		MenuPoint.x, MenuPoint.y, AfxGetMainWnd());
	//	
	//	}
	//	

	//}else
	{
		CMenu cm;
		if(theApp.m_LanguageSupport  ==LANG_CN_SIMPLIFIED)
		{
			cm.LoadMenu(IDR_MENU5);
		}
		else
		{
			cm.LoadMenu(IDR_MENU1);
		}

		int layer_no = (int)(pMainFrame-> m_iSelectedLayer);
		cm.GetSubMenu(layer_no)->TrackPopupMenu(
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
			MenuPoint.x, MenuPoint.y, AfxGetMainWnd());

	}


}

void CTLiteView::OnClickLink(UINT nFlags, CPoint point)
{

	CTLiteDoc* pDoc = GetDocument();
	double Min_distance = 100;  // in screen points


	for (std::list<DTALink*>::iterator iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
	{
		if((*iLink) ->m_ShapePoints.size() <2)
			continue;

		for(int si = 0; si < (*iLink) ->m_ShapePoints .size()-1; si++)
		{

			FromPoint = NPtoSP((*iLink)->m_ShapePoints[si]);
			ToPoint = NPtoSP((*iLink)->m_ShapePoints[si+1]);

			GDPoint p0, pfrom, pto;
			p0.x  = point.x; p0.y  = point.y;
			pfrom.x  = FromPoint.x; pfrom.y  = FromPoint.y;
			pto.x  = ToPoint.x; pto.y  = ToPoint.y;

			float distance = g_GetPoint2LineDistance(p0, pfrom, pto, pDoc->m_UnitMile);

			if(distance >0 && distance < Min_distance)
			{
				pDoc->m_SelectedLinkNo = (*iLink)->m_LinkNo ;

				Min_distance = distance;
			}
		}
	}

	//if(Min_distance > pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution*20)
	//{
	//	pDoc->m_SelectedLinkNo = -1;
	//	g_ClearLinkSelectionList();
	//}else
	{
		// if the control key is not pressed 
		if ( (nFlags & MK_CONTROL) ==false)
		{
			g_ClearLinkSelectionList();
		}

		int bFoundFlag = false;

		for (std::list<s_link_selection>::iterator iLink = g_LinkDisplayList.begin(); iLink != g_LinkDisplayList.end(); iLink++)
		{
			if( (*iLink).link_no == pDoc->m_SelectedLinkNo &&  (*iLink).document_no == pDoc->m_DocumentNo )
			{ 
				bFoundFlag = true; 
				break;
			}

		}
		if(!bFoundFlag)  //has not been selected;
		{
			GDPoint pt  = SPtoNP(point);

			DTALink* pLink = pDoc->m_LinkNoMap [pDoc->m_SelectedLinkNo];

			g_AddLinkIntoSelectionList(pLink,pDoc->m_SelectedLinkNo, pDoc->m_DocumentNo, true, pt.x, pt.y);
		}
	}



	// reset
	for (std::list<DTALink*>::iterator iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
	{
		(*iLink)->m_DisplayLinkID = -1;
	}

	int LinkCount=0;
	for (std::list<s_link_selection>::iterator iLinkDisplay = g_LinkDisplayList.begin(); iLinkDisplay != g_LinkDisplayList.end(); iLinkDisplay++, LinkCount++)
	{

		if((*iLinkDisplay).pLink  !=NULL && pDoc->m_DocumentNo == (*iLinkDisplay).document_no )
		{
			pDoc->m_LinkNoMap[(*iLinkDisplay).link_no ]->m_DisplayLinkID = LinkCount;
		}

	}

}
void CTLiteView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CTLiteDoc* pDoc = GetDocument();

	/*	if( pDoc->m_TrafficFlowModelFlag > 0 || m_ToolMode == select_link_tool)
	{
	// create MOE Dlg when double clicking

	OnClickLink(nFlags, point);
	g_LinkMOEDlgShowFlag = false;
	pDoc->OnViewShowmoe();
	}
	*/
	/*		
	std::list<DTANode*>::iterator iNode;


	for (iNode = pDoc->m_NodeSet.begin(); iNode != pDoc->m_NodeSet.end(); iNode++)
	{
	CPoint NodePoint = NPtoSP((*iNode)->pt);

	CSize size = NodePoint - point;
	double distance = pow((size.cx*size.cx + size.cy*size.cy),0.5);
	if( distance < Min_distance)
	{
	m_SelectedNodeID = (*iNode)->m_NodeNo ;
	Min_distance = distance;
	return;
	}

	}

	if(Min_distance > m_NodeSize*2)
	{
	m_SelectedNodeID = -1;
	}

	*/

	if((m_ToolMode == subarea_tool || m_ToolMode == add_zone_tool) && GetDocument()->m_SubareaShapePoints.size()>= 3)
	{
		CWaitCursor wait;
		GetDocument()->m_SubareaShapePoints.push_back(SPtoNP(point));
		GetDocument()->m_SubareaShapePoints.push_back(GetDocument()->m_SubareaShapePoints[0]);
		isCreatingSubarea = false;
		CopyLinkSetInSubarea();
		isFinishSubarea = true;


		//add a zone
		if(m_ToolMode == add_zone_tool)
		{
			GetDocument()-> OnSubareaCreatezonefromsubarea();

		}
		m_ToolMode = select_link_tool;
		ReleaseCapture();
		m_last_left_down_point = point;

	}
	m_bMoveDisplay = false;

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	if(pMainFrame-> m_iSelectedLayer == layer_vehicle_position)
	{

		FILE* st = NULL;
		fopen_s(&st,pDoc->m_ProjectDirectory + "GPS_log.csv","a");
		if(st!=NULL)
		{
			DWORD Time = GetTickCount();

			int node_id = FindClosestNode(point, 300);  // 300 is screen unit

			fprintf(st, "%d,%ld\n", pDoc->m_NodeNotoNumberMap[node_id], Time);
			fclose(st);
		}
	}
	CView::OnLButtonDblClk(nFlags, point);
}

void CTLiteView::OnSearchFindlink()
{
	CDlgFindALink dlg;

	CTLiteDoc* pDoc = GetDocument();

	dlg.m_SearchHistoryVector = pDoc->m_SearchHistoryVector;
	dlg.m_SearchMode = pDoc->m_SearchMode;

	if(dlg.DoModal () == IDOK)
	{

		m_PathNodeSequence = "";

		if(dlg.m_SearchMode == efind_link)
		{

			DTALink* pLink = pDoc->FindLinkWithNodeNumbers(dlg.m_FromNodeNumber ,dlg.m_ToNodeNumber );

			if(pLink !=NULL)
			{
				pDoc->m_SelectedLinkNo = pLink->m_LinkNo ;
				pDoc->m_SelectedNodeID = -1;

				pDoc->ZoomToSelectedLink(pLink->m_LinkNo);

				m_SelectFromNodeNumber = dlg.m_FromNodeNumber;
				m_SelectToNodeNumber = dlg.m_ToNodeNumber;
				Invalidate();

			}
		}

		if(dlg.m_SearchMode == efind_node)
		{
			DTANode* pNode = pDoc->FindNodeWithNodeNumber (dlg.m_NodeNumber);
			if(pNode !=NULL)
			{
				pDoc->m_SelectedLinkNo = -1;
				pDoc->m_SelectedNodeID = pNode->m_NodeNo ;

				m_Origin = pNode->pt ;

				m_bShowNode = true;
				Invalidate();
			}else
			{
				CString str;

				if(dlg.m_NodeNumber != 0)  // 0 is the default node number in this dialog
				{
					str.Format ("Node %d cannot be found.",dlg.m_NodeNumber);
					AfxMessageBox(str);
				}
			}
		}

		if(dlg.m_SearchMode == efind_path)
		{
			m_SelectFromNodeNumber = dlg.m_FromNodeNumber;
			m_SelectToNodeNumber = dlg.m_ToNodeNumber;

			m_ShowAllPaths = true;

			DTANode* pFromNode = pDoc->FindNodeWithNodeNumber (dlg.m_FromNodeNumber);
			if(pFromNode ==NULL)
			{
				CString str;
				str.Format ("From Node %d cannot be found.",dlg.m_FromNodeNumber);
				AfxMessageBox(str);
				return;
			}else
			{
				pDoc->m_OriginNodeID = pFromNode->m_NodeNo;

			}
			DTANode* pToNode = pDoc->FindNodeWithNodeNumber (dlg.m_ToNodeNumber);
			if(pToNode ==NULL)
			{
				CString str;
				str.Format ("To Node %d cannot be found.",dlg.m_ToNodeNumber);
				AfxMessageBox(str);
				return;
			}else
			{
				pDoc->m_DestinationNodeID = pToNode->m_NodeNo;
			}

			pDoc->Routing(false);

		}

		if (dlg.m_SearchMode == efind_sensor)
		{

			pDoc->m_SelectedLinkNo = -1;
			pDoc->m_SelectedNodeID = -1;

			CString value_str;
			value_str.Format("%d", dlg.m_NodeNumber);

			pDoc->ZoomToSelectedSensor(pDoc->CString2StdString(value_str));

			return;
		}

		if (dlg.m_SearchMode == efind_path_node_sequence)
		{

			m_PathNodeSequence = dlg.m_PathNodeSequence;
			Invalidate();

			return;
		}

		if(dlg.m_SearchMode == efind_vehicle)
		{
			int SelectedVehicleID = dlg.m_VehicleNumber; // internal vehicle index starts from zero
			std::vector<int> LinkVector;

			if(pDoc->m_VehicleIDMap.find(SelectedVehicleID) == pDoc->m_VehicleIDMap.end())
			{
				if(SelectedVehicleID>=0)
				{
					CString str_message;
					str_message.Format ("Vehicle Id %d cannot be found.", SelectedVehicleID+1);
					AfxMessageBox(str_message);
				}

				pDoc->m_SelectedVehicleID = -1;
				pDoc->HighlightPath(LinkVector,1);

				return;
			}


			



			pDoc->m_SelectedVehicleID = SelectedVehicleID;

			DTAVehicle* pVehicle = pDoc->m_VehicleIDMap[SelectedVehicleID];

			CPlayerSeekBar m_wndPlayerSeekBar;


			// set departure time to the current time of display
			g_Simulation_Time_Stamp = pVehicle->m_DepartureTime +1;

			CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

			pMainFrame->m_wndPlayerSeekBar.SetPos(g_Simulation_Time_Stamp);

			pDoc->m_LinkMOEMode = MOE_vehicle;

			for(int link= 1; link<pVehicle->m_NodeSize; link++)
			{
				LinkVector.push_back (pVehicle->m_NodeAry[link].LinkNo);
			}

			pDoc->HighlightPath(LinkVector,1);

			pDoc->m_HighlightGDPointVector.clear();

			pDoc->m_HighlightGDPointVector = pVehicle->m_GPSLocationVector ; // assign the GPS points to be highlighted

		}

		pDoc->m_SearchHistoryVector = dlg.m_SearchHistoryVector;
		pDoc->m_SearchMode = dlg.m_SearchMode;


	}

	Invalidate();
}

void CTLiteView::OnEditChangebackgroupimagethroughmouse()
{
	if(m_ToolMode!= backgroundimage_tool)
		m_ToolMode = backgroundimage_tool;
	else
		m_ToolMode = move_tool;

}

void CTLiteView::OnUpdateEditChangebackgroupimagethroughmouse(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == backgroundimage_tool ? 1 : 0);
}

void CTLiteView::OnViewBackgroundimage()
{
	m_bShowImage = !m_bShowImage;
	Invalidate();
}

void CTLiteView::OnUpdateViewBackgroundimage(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowImage);
}

void CTLiteView::OnViewShowlinktype()
{
	m_bShowLinkType = !m_bShowLinkType;
	Invalidate();
}

void CTLiteView::OnUpdateViewShowlinktype(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowLinkType);

}


void CTLiteView::OnShowShownode()
{
	m_bShowNode = !m_bShowNode;
	Invalidate();
}

void CTLiteView::OnUpdateShowShownode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowNode);
}

void CTLiteView::OnShowShowallpaths()
{
	//	m_ShowAllPaths = !m_ShowAllPaths;

	Invalidate();
}

void CTLiteView::OnUpdateShowShowallpaths(CCmdUI *pCmdUI)
{
}



void CTLiteView::OnShowShownodenumber()
{
	if(m_ShowNodeTextMode != node_display_none)
		m_ShowNodeTextMode = node_display_none;
	else 
		m_ShowNodeTextMode = node_display_node_number;

	if(m_ShowNodeTextMode != node_display_none)
		m_bShowNode = true;

	Invalidate();

}

void CTLiteView::OnUpdateShowShownodenumber(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ShowNodeTextMode != node_display_none);

}

void  CTLiteView::DrawTemporalLink(CPoint start_point, CPoint end_point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);

	dc.SelectObject(&g_TempLinkPen);
	// Select drawing mode

	int oldROP2 = dc.SetROP2(R2_XORPEN);

	// Draw the line
	dc.MoveTo(start_point);
	dc.LineTo(end_point);

	// Reset drawing mode
	dc.SetROP2(oldROP2);

}

void CTLiteView::OnEditCreate1waylink()
{
	if(m_ToolMode != create_1waylink_tool)
	{

		CTLiteDoc* pDoc = GetDocument();

		m_bLineDisplayConditionalMode  =true;
		m_link_display_mode = link_display_mode_line;
		m_bShowLinkArrow = true;

		m_ToolMode = create_1waylink_tool;
		Invalidate();

	}
	else   //reset to default selection tool
		m_ToolMode = select_link_tool;

}

void CTLiteView::OnEditCreate2waylinks()
{
	if(m_ToolMode != create_2waylinks_tool)
	{
			CTLiteDoc* pDoc = GetDocument();



		m_bLineDisplayConditionalMode  =true;
		m_link_display_mode = link_display_mode_line;
		m_bShowLinkArrow = true;

		m_ToolMode = create_2waylinks_tool;
			Invalidate();

	}
	else   //reset to default selection tool
		m_ToolMode = select_link_tool;

}

void CTLiteView::OnUpdateEditCreate1waylink(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == create_1waylink_tool);

}

void CTLiteView::OnUpdateEditCreate2waylinks(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == create_2waylinks_tool);
}

void CTLiteView::OnShowGrid()
{
	m_bShowGrid = !m_bShowGrid;
	Invalidate();
}

void CTLiteView::OnUpdateShowGrid(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowGrid);
}

void CTLiteView::OnShowLinkarrow()
{
	m_bShowLinkArrow = !m_bShowLinkArrow;

	if(m_bShowLinkArrow)
	{
		m_link_display_mode = link_display_mode_line;
		m_bLineDisplayConditionalMode = true;
	}
	Invalidate();
}

void CTLiteView::OnUpdateShowLinkarrow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowLinkArrow);
}


void CTLiteView::OnImageLockbackgroundimageposition()
{
	m_ToolMode = move_tool;  // swtich m_ToolMode from backgroundimage_tool to other tool
}

void CTLiteView::OnViewTextlabel()
{

}

void CTLiteView::OnUpdateViewTextlabel(CCmdUI *pCmdUI)
{
}


void CTLiteView::OnLinkDelete()
{

	CTLiteDoc* pDoc = GetDocument();

	if(pDoc->m_SelectedLinkNo == -1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}
	pDoc->Modify ();
	pDoc->PushBackNetworkState();

	pDoc->DeleteLink(pDoc->m_SelectedLinkNo);
	pDoc->m_SelectedLinkNo = -1;
	Invalidate();
}

void CTLiteView::OnUpdateEditDeleteselectedlink(CCmdUI *pCmdUI)
{
	pCmdUI->Enable (GetDocument()->m_SelectedLinkNo>=0);
}

void CTLiteView::OnLinkEditlink()
{
	CTLiteDoc* pDoc = GetDocument();

	if(pDoc->m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink= pDoc->m_LinkNoMap [pDoc->m_SelectedLinkNo];
	if(pLink!=NULL)
	{
		CDlgLinkProperties dlg;


		GDPoint pt = SPtoNP(m_CurrentMousePoint);
		dlg.m_LinkLocationX = pt.x;
		dlg.m_LinkLocationY = pt.y;

		dlg.m_bUpdateLinkAttributeBasedOnType = m_bUpdateLinkAttributeBasedOnType;


		dlg.m_pDoc = pDoc;

		dlg.m_LinkID = pLink->m_LinkID ;
		dlg.StreetName  = pLink->m_Name.c_str () ;
		dlg.FromNode = pLink->m_FromNodeNumber ;
		dlg.ToNode = pLink->m_ToNodeNumber ;
		dlg.m_ModeCode = pLink->m_Mode_code .c_str ();

		dlg.m_prohibited_u_turn = (BOOL)(pLink->m_prohibited_u_turn) ;

		dlg.m_NumLeftTurnLanes  = pLink-> m_NumberOfLeftTurnLanes;
		dlg.m_NumRightTurnLanes  = pLink-> m_NumberOfRightTurnLanes;
		
		dlg.m_CountSensorID  = pLink->m_CountSensorID.c_str();
		dlg.m_SpeedSensorID = pLink->m_SpeedSensorID.c_str();


		dlg.m_LeftTurnLength   = pLink->m_LeftTurnLaneLength ;
		dlg.m_RightTurnLength  = pLink-> m_RightTurnLaneLength;
		dlg.m_KJam  = pLink->m_Kjam ;
		dlg.m_Grade  = pLink->m_Grade ;

		if(pDoc->m_bUseMileVsKMFlag)
		{
			dlg.LinkLength = pLink->m_Length ;
			dlg.SpeedLimit = pLink->m_SpeedLimit ;
		}else
		{ // km
			dlg.LinkLength = pLink->m_Length*1.60934 ;
			dlg.SpeedLimit = pLink->m_SpeedLimit*1.60934 ;
		}
		dlg.FreeFlowTravelTime = pLink->m_FreeFlowTravelTime ;
		dlg.LaneCapacity  = pLink->m_LaneCapacity ;
		dlg.nLane = pLink->m_NumberOfLanes ;
		dlg.LinkType = pLink->m_link_type;

		dlg.m_BPR_Alpha  = pLink->m_BPR_alpha_term  ;
		dlg.m_BPR_Beta  = pLink->m_BPR_beta_term  ;

		dlg.DefaultSpeedLimit = pDoc->m_DefaultSpeedLimit ;
		dlg.DefaultCapacity = pDoc->m_DefaultCapacity ;
		dlg.DefaultnLane = pDoc->m_DefaultNumLanes;

		if(dlg.DoModal() == IDOK)
		{
			if (pLink->m_LinkID != dlg.m_LinkID)  // different link id
			{
				if (pDoc->m_LinkIDtoLinkMap.find(dlg.m_LinkID) != pDoc->m_LinkIDtoLinkMap.end())
				{
					CString msg;
					msg.Format("Warning: Link ID %d is updated to %d but the new ID already exists. Please check carefully.",
						pLink->m_LinkID, dlg.m_LinkID);
				}

				pDoc->m_LinkIDtoLinkMap[dlg.m_LinkID] = pDoc->m_LinkIDtoLinkMap[pLink->m_LinkID]; // update pointer to the link
				pDoc->m_LinkIDtoLinkMap[pLink->m_LinkID] = NULL;
				pLink->m_LinkID = dlg.m_LinkID;
			}

			m_bUpdateLinkAttributeBasedOnType = dlg.m_bUpdateLinkAttributeBasedOnType;

			if(pDoc->m_bUseMileVsKMFlag)
			{

				pLink->m_Length = dlg.LinkLength;
				pLink->m_SpeedLimit = dlg.SpeedLimit;

			}else
			{
				pLink->m_Length = dlg.LinkLength/1.60934;
				pLink->m_SpeedLimit = dlg.SpeedLimit/1.60934;
			}


			pLink->m_FreeFlowTravelTime = pLink->m_Length /pLink->m_SpeedLimit*60.0f; 

			pLink->m_Name  = pDoc->CString2StdString(dlg.StreetName);

			pLink->m_LaneCapacity  = dlg.LaneCapacity;


			pLink->m_Mode_code  = pDoc->CString2StdString(dlg.m_ModeCode) ;
			pLink->m_CountSensorID = pDoc->CString2StdString(dlg.m_CountSensorID);
			pLink->m_SpeedSensorID = pDoc->CString2StdString(dlg.m_SpeedSensorID);

			pLink->m_BPR_alpha_term  = dlg.m_BPR_Alpha  ;
			pLink->m_BPR_beta_term   = dlg.m_BPR_Beta;

			if(fabs(pLink-> m_Kjam - dlg.m_KJam) >=1)
				dlg.m_bEditChange = true; 

			if(fabs(pLink-> m_Grade - dlg.m_Grade) >=1)
				dlg.m_bEditChange = true; 

			pLink->m_Kjam = dlg.m_KJam;
			pLink->m_Grade = dlg.m_Grade;



			if(pLink->m_NumberOfLanes  != dlg.nLane)
			{
				pLink->m_NumberOfLanes  = dlg.nLane;
				pDoc->GenerateOffsetLinkBand();  // update width of band
			}

			pLink-> m_NumberOfLeftTurnLanes = dlg.m_NumLeftTurnLanes ;


			std::list<DTANode*>::iterator iNode;

			if(pLink-> m_NumberOfLeftTurnLanes!= dlg.m_NumLeftTurnLanes)
			{
				pLink-> m_NumberOfLeftTurnLanes = dlg.m_NumLeftTurnLanes ;
				// change movement's left-turn's # of number of lanes accordingly

				for (iNode = pDoc->m_NodeSet.begin(); iNode != pDoc->m_NodeSet.end(); iNode++)
				{
					for(int tp = 0; tp< pDoc->m_TimingPlanVector.size(); tp++)  // first loop for each timing plan
					{

						std::string timing_plan_name = pDoc->m_TimingPlanVector[tp].timing_plan_name;  // fetch timing_plan (unique) name

						if((*iNode)->m_MovementDataMap.find (timing_plan_name)!=(*iNode)->m_MovementDataMap.end())
						{
							for(unsigned int m = 0; m< (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector .size(); m++)
							{
								DTANodeMovement* pMovement = &((*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m]);
								DTALink* pLink0 = pDoc->m_LinkNoMap[pMovement->IncomingLinkNo  ];
								if(pLink0->m_LinkNo  == pLink->m_LinkNo && (pMovement->movement_turn == DTA_LeftTurn || pMovement->movement_turn == DTA_LeftTurn2))
								{
									pMovement->QEM_Lanes = dlg.m_NumLeftTurnLanes;
								}
							}

						}
					}
				}
			}

			if(pLink-> m_NumberOfRightTurnLanes!= dlg.m_NumRightTurnLanes)
			{
				pLink-> m_NumberOfRightTurnLanes = dlg.m_NumRightTurnLanes ;
				// change movement's right-turn's # of number of  accordingly

				for (iNode = pDoc->m_NodeSet.begin(); iNode != pDoc->m_NodeSet.end(); iNode++)
				{

					for(int tp = 0; tp< pDoc->m_TimingPlanVector.size(); tp++)  // first loop for each timing plan
					{

						std::string timing_plan_name = pDoc->m_TimingPlanVector[tp].timing_plan_name;  // fetch timing_plan (unique) name


					for(unsigned int m = 0; m< (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector .size(); m++)
					{
						DTANodeMovement* pMovement = &((*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m]);
						DTALink* pLink0 = pDoc->m_LinkNoMap[pMovement->IncomingLinkNo  ];
						if(pLink0->m_LinkNo  == pLink->m_LinkNo && (pMovement->movement_turn == DTA_RightTurn || pMovement->movement_turn == DTA_RightTurn2))
						{
							pMovement->QEM_Lanes = dlg.m_NumRightTurnLanes;
						}

					}
					}
				}
			}




			pLink->m_prohibited_u_turn = dlg.m_prohibited_u_turn; 


			if(pLink->m_link_type != dlg.LinkType)
			{
				dlg.m_bEditChange = true;
				pLink->m_link_type = dlg.LinkType ;
			}

			pLink->m_observed_peak_hourly_volume = dlg.m_PeakHourlyVolume;

			if( pDoc->m_LinkTypeMap[pLink->m_link_type].IsFreeway () ||  pDoc->m_LinkTypeMap[pLink->m_link_type].IsRamp  ())
			{
				pDoc->m_NodeNoMap[pLink->m_FromNodeID ]->m_bConnectedToFreewayORRamp = true;
				pDoc->m_NodeNoMap[pLink->m_ToNodeID ]->m_bConnectedToFreewayORRamp = true;
			}


			pDoc->m_DefaultSpeedLimit = dlg.DefaultSpeedLimit;
			pDoc->m_DefaultCapacity = dlg.DefaultCapacity;
			pDoc->m_DefaultNumLanes = dlg.DefaultnLane;


			if(pLink->m_LeftTurnLaneLength != dlg.m_LeftTurnLength)
				dlg.m_bEditChange = true; 

			if(pLink-> m_RightTurnLaneLength != dlg.m_RightTurnLength)
				dlg.m_bEditChange = true; 

			if(pLink-> m_RightTurnLaneLength != dlg.m_RightTurnLength)
				dlg.m_bEditChange = true; 


			pLink->m_LeftTurnLaneLength = dlg.m_LeftTurnLength; 
			pLink-> m_RightTurnLaneLength = dlg.m_RightTurnLength;

			// error checking here


			if(dlg.m_bEditChange)
			{
				pDoc->Modify();
				pDoc->PushBackNetworkState();
			}

		}
		Invalidate();
	}

}


void CTLiteView::OnEditCreatenode()
{
	if(m_ToolMode != create_node_tool)
	{
		m_ToolMode = create_node_tool;
		m_bShowNode = true;
		m_ShowNodeTextMode = node_display_node_number;

	}
	else   //reset to default selection tool
		m_ToolMode = select_link_tool;
}

void CTLiteView::OnUpdateEditCreatenode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == create_node_tool ? 1 : 0);
}

void CTLiteView::OnEditDeleteselectednode()
{
	CTLiteDoc* pDoc = GetDocument();
	pDoc->Modify ();
	pDoc->PushBackNetworkState();

	if(pDoc->m_SelectedNodeID == -1)
	{
		AfxMessageBox("Please select a node first.");
		return;
	}
	if(pDoc->DeleteNode(pDoc->m_SelectedNodeID) ==false)
	{
		AfxMessageBox("Only an isolated node can be deleted. Please delete adjacent links first.");
		return;
	}
	pDoc->m_SelectedNodeID = -1;
	Invalidate();

}

void CTLiteView::OnEditSelectnode()
{
	// TODO: Add your command handler code here
}

void CTLiteView::OnViewSelectNode()
{
	m_ToolMode = select_feature_tool;
}

void CTLiteView::OnUpdateViewSelectNode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == select_feature_tool ? 1 : 0);
}



void CTLiteView::OnEditCreatesubarea()
{
	m_ToolMode = subarea_tool;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	pMainFrame->m_bShowLayerMap[layer_subarea] = true;
	pMainFrame-> m_iSelectedLayer = layer_subarea;
	GetDocument()->m_SubareaShapePoints.clear();

	CListCtrl * pGISLayerList = (CListCtrl *)(pMainFrame->m_GISLayerBar).GetDlgItem(IDC_LIST_GISLAYER);
	pGISLayerList->Invalidate(1);  // update display of selected layer

}

void CTLiteView::OnUpdateEditCreatesubarea(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == subarea_tool ? 1 : 0);
}


void CTLiteView::CopyLinkSetInSubarea()
{
	CTLiteDoc* pDoc = GetDocument();

	if( pDoc->m_SubareaShapePoints.size() < 3)
		return;

	LPPOINT m_subarea_points = new POINT[pDoc->m_SubareaShapePoints.size()];
	for (int sub_i= 0; sub_i < pDoc->m_SubareaShapePoints.size(); sub_i++)
	{
		CPoint point =  NPtoSP(pDoc->m_SubareaShapePoints[sub_i]);
		m_subarea_points[sub_i].x = point.x;
		m_subarea_points[sub_i].y = point.y;
	}

	// Create a polygonal region
	m_polygonal_region = CreatePolygonRgn(m_subarea_points, pDoc->m_SubareaShapePoints.size(), WINDING);

	pDoc->m_SubareaNodeSet.clear();
	pDoc->m_SubareaLinkSet.clear();
	pDoc->m_SubareaNodeIDMap.clear();

	std::list<DTANode*>::iterator iNode = pDoc->m_NodeSet.begin ();
	while (iNode != pDoc->m_NodeSet.end())
	{
		CPoint point = NPtoSP((*iNode)->pt);
		if(PtInRegion(m_polygonal_region, point.x, point.y))  //outside subarea
		{
			pDoc->m_SubareaNodeSet .push_back ((*iNode));
			pDoc->m_SubareaNodeIDMap[(*iNode)->m_NodeNo ]=  (*iNode);
			//inside subarea
		}
		iNode++;
	}

	std::list<DTALink*>::iterator iLink;

	iLink = pDoc->m_LinkSet.begin(); 

	while (iLink != pDoc->m_LinkSet.end())
	{
		(*iLink)->m_bIncludedinSubarea = false;
		if(pDoc->m_SubareaNodeIDMap.find((*iLink)->m_FromNodeID ) != pDoc->m_SubareaNodeIDMap.end() || pDoc->m_SubareaNodeIDMap.find((*iLink)->m_ToNodeID ) != pDoc->m_SubareaNodeIDMap.end()) 
		{
			pDoc->m_SubareaLinkSet.push_back (*iLink);

			(*iLink)->m_bIncludedinSubarea = true;


		}

		(*iLink)->m_bIncludedinBoundaryOfSubarea = false;
		if( (pDoc->m_SubareaNodeIDMap.find((*iLink)->m_FromNodeID ) != pDoc->m_SubareaNodeIDMap.end() && pDoc->m_SubareaNodeIDMap.find((*iLink)->m_ToNodeID ) == pDoc->m_SubareaNodeIDMap.end())
			|| (pDoc->m_SubareaNodeIDMap.find((*iLink)->m_ToNodeID ) != pDoc->m_SubareaNodeIDMap.end() && pDoc->m_SubareaNodeIDMap.find((*iLink)->m_FromNodeID ) == pDoc->m_SubareaNodeIDMap.end()))
		{
			(*iLink)->m_bIncludedinBoundaryOfSubarea = true;
		}

		iLink++;
	}


	DeleteObject(m_polygonal_region);
	delete [] m_subarea_points;

}

void CTLiteView::OnToolsRemovenodesandlinksoutsidesubarea()
{
	CWaitCursor wait;

	CTLiteDoc* pDoc = GetDocument();

	pDoc->m_bDYNASMARTDataSet = false; // after performing subarea cut, we will use AMS tnp file format

	pDoc->Modify ();
	pDoc->PushBackNetworkState();


	LPPOINT m_subarea_points = new POINT[pDoc->m_SubareaShapePoints.size()];
	for (unsigned int sub_i= 0; sub_i < pDoc->m_SubareaShapePoints.size(); sub_i++)
	{
		CPoint point =  NPtoSP(pDoc->m_SubareaShapePoints[sub_i]);
		m_subarea_points[sub_i].x = point.x;
		m_subarea_points[sub_i].y = point.y;
	}

	// Create a polygonal region
	m_polygonal_region = CreatePolygonRgn(m_subarea_points, pDoc->m_SubareaShapePoints.size(), WINDING);

	// step 01
	std::list<DTANode*>::iterator iNode;
	iNode = pDoc->m_NodeSet.begin ();


	while (iNode != pDoc->m_NodeSet.end())
	{
		CPoint point = NPtoSP((*iNode)->pt);
		if(PtInRegion(m_polygonal_region, point.x, point.y) == false)  //outside subarea
		{
			(*iNode)->m_bSubareaFlag = 0;
		}
		else
		{
			(*iNode)->m_bSubareaFlag = 1;	//inside subarea
		}

		(*iNode)->m_tobeRemoved = true;  // mark all to be removed first

		++iNode;
	}



	//step 02
	//remove links
	pDoc->m_LinkNoMap.clear();

	std::list<DTALink*>::iterator iLink;

	iLink = pDoc->m_LinkSet.begin(); 

	while (iLink != pDoc->m_LinkSet.end())
	{
		DTANode* pFromNode = pDoc->m_NodeNoMap[(*iLink)->m_FromNodeID];
		DTANode* pToNode = pDoc->m_NodeNoMap[(*iLink)->m_ToNodeID];

		if(pFromNode->m_bSubareaFlag == 0 &&  pToNode ->m_bSubareaFlag == 0 ) 
		{
			iLink = pDoc->m_LinkSet.erase(iLink);  // remove when one of end points are covered by the subarea

		}else
		{
			pDoc->m_LinkNoMap[(*iLink)->m_LinkNo] = (*iLink);

			pFromNode->m_tobeRemoved = false;  // no need to remove as long as one adjacent link is kept
			pToNode->m_tobeRemoved = false; 

			++iLink;
		}
	}


	// step 03: remove nodes
	pDoc->m_NodeNumberMap.clear();
	pDoc->m_NodeNoMap.clear();

	iNode = pDoc->m_NodeSet.begin ();
	while (iNode != pDoc->m_NodeSet.end())
	{
		if((*iNode)->m_NodeNumber ==511)
		{
			TRACE("..");
		}

		if((*iNode)->m_tobeRemoved == true )  //outside subarea
		{
			iNode = pDoc->m_NodeSet.erase (iNode);
		}
		else
		{
			pDoc->m_NodeNumberMap[(*iNode)->m_NodeNumber  ] = (*iNode);
			pDoc->m_NodeNoMap[(*iNode)->m_NodeNo  ] = (*iNode);

			++iNode;
			//inside subarea
		}
	}



	// step 04: mark zones to be removed. 

	std::map<int, DTAZone>	:: iterator itr;

	for(itr = pDoc->m_ZoneMap.begin(); itr != pDoc->m_ZoneMap.end(); ++itr)
	{
		TRACE("Zone ID %d: \n", itr->first );
		int activty_node_size = itr->second.m_ActivityLocationVector.size();
		int outofsubarea_count  = 0;
		// copy to another array first		
		std::vector<int> activity_node_vector;
		for(int i = 0; i< itr->second.m_ActivityLocationVector .size(); i++)
		{
			DTAActivityLocation element = itr->second.m_ActivityLocationVector[i];

			activity_node_vector.push_back (element.NodeNumber);
		}
		// remove nodes from actvity location vector of a zone
		for(unsigned int k = 0; k < activity_node_vector.size(); k++)
		{
			if( pDoc->m_NodeNumberMap.find(activity_node_vector[k]) == pDoc->m_NodeNumberMap.end())
				itr->second.RemoveNodeActivityMode(activity_node_vector[k]);
		}

		if(itr->first == 551)
		{
			TRACE("");
		}
		if( itr->second.m_ActivityLocationVector.size() ==0 )
		{
			itr->second.m_bWithinSubarea = false;
		}
	}

	// step 05: add new zones if the boundary has zone --

	iNode = pDoc->m_NodeSet.begin ();
	int TAZ = 1000;  // mark for subarea

	int boundary_zone_count = 0;
	while (iNode != pDoc->m_NodeSet.end())
	{

		if((*iNode)->m_bSubareaFlag ==0 && (*iNode)->m_ZoneID == 0)  // remaining boundary-and-not-activity-location nodes after out of subarea links are removed. 
		{
			// create new zone number and add this node as activity center
			// try to get TAZ as the node number

			TAZ = (*iNode)->m_NodeNumber ;

			while(pDoc->m_ZoneMap.find (TAZ) != pDoc->m_ZoneMap.end ())  // find unused TAZ as new TAZ
			{
				TAZ ++;
			}

			// now we have an unused TAZ  
			(*iNode)->m_bZoneActivityLocationFlag = true;

			pDoc->m_ZoneMap [TAZ].m_ZoneID = TAZ;
			(*iNode)->m_ZoneID = TAZ;

			DTAActivityLocation element;
			element.ZoneID  = TAZ;
			element.NodeNumber = (*iNode)->m_NodeNumber  ;
			pDoc->m_ZoneMap [TAZ].m_ActivityLocationVector .push_back (element );
			boundary_zone_count++;
			TAZ ++;

		}
		++iNode;
	}



	// step 06: generate route file
	std::list<DTAVehicle*>::iterator iVehicle;
	pDoc->m_PathMap.clear();
	pDoc->m_ODMatrixMap.clear();


	for (iVehicle = pDoc->m_VehicleSet.begin(); iVehicle != pDoc->m_VehicleSet.end(); iVehicle++)
	{
		DTAVehicle* pVehicle = (*iVehicle);

		if(pVehicle->m_NodeSize >= 2 && pVehicle->m_bComplete)  // with physical path in the network
		{

			// update node sequence
			std::vector<int> LinkVector;
			std::vector<int> NodeVector;

			int StartFlag = 0; // 0: not start yet, 1: start node chain, 2: end node chain
			for(int link= 1; link<pVehicle->m_NodeSize; link++)
			{
				int OrgLinkNO = pVehicle->m_NodeAry[link].LinkNo;


				if( pDoc->m_LinkNoMap.find(OrgLinkNO)!= pDoc->m_LinkNoMap.end()) // the link exists in subarea 
				{

					if(StartFlag <=1)
						LinkVector.push_back(OrgLinkNO);

					if(StartFlag ==0)
						StartFlag = 1; // start node chain
				}else  // link has been removed
				{
					if(StartFlag==1) // node chain has been started 
					{
						StartFlag = 2;  // terminate
						break;
					} else 
					{
						//continue // StartFlag ==0
					}
				}

			}
			// LinkVector for remaining links in subarea 

			if(LinkVector.size() >=2)
			{

				DTALink* pLinkOrg = pDoc->m_LinkNoMap[LinkVector[0]];
				DTALink* pLinkDes = pDoc->m_LinkNoMap[LinkVector[LinkVector.size()-1]];

				int NewOriginZoneID = pDoc->m_NodeNoMap[pLinkOrg->m_FromNodeID ]->m_ZoneID  ;
				int NewDestinationZoneID = pDoc->m_NodeNoMap[pLinkDes->m_ToNodeID ]->m_ZoneID  ;;
				int NewNodeNumberSum = 0;

				for(unsigned li = 0; li < LinkVector.size(); li++)
				{
					DTALink* pLink = pDoc->m_LinkNoMap[LinkVector[li]];
					if(li==0) // first link
					{
						NewNodeNumberSum += pLink->m_FromNodeNumber;
					}

					NewNodeNumberSum += pLink->m_ToNodeNumber;
				}

				int Newm_NodeSize = LinkVector.size() +1;
				CString label;
				label.Format("%d,%d,%d,%d", NewOriginZoneID  , NewDestinationZoneID , NewNodeNumberSum , Newm_NodeSize );

				//existing path
				pDoc->m_PathMap[label].Origin = NewOriginZoneID;
				pDoc->m_PathMap[label].Destination  = NewDestinationZoneID;
				pDoc->m_PathMap[label].TotalVehicleSize+=1;

				if(pDoc->m_PathMap[label].TotalVehicleSize == 1)  // new path
				{

					for(unsigned li = 0; li < LinkVector.size(); li++)
					{
						DTALink* pLink = pDoc->m_LinkNoMap[LinkVector[li]];
						if(li==0) // first link
						{
							pDoc->m_PathMap[label].m_NodeVector.push_back(pLink->m_FromNodeNumber );
						}

						pDoc->m_PathMap[label].m_NodeVector.push_back(pLink->m_ToNodeNumber);

					}
				}

				//
				// reuse label as OD label
				label.Format("%d,%d", pVehicle->m_OriginZoneID  , pVehicle->m_DestinationZoneID);
				pDoc->m_ODMatrixMap[label].Origin = NewOriginZoneID;
				pDoc->m_ODMatrixMap[label].Destination  = NewDestinationZoneID;
				pDoc->m_ODMatrixMap[label].TotalVehicleSize+=1;


			}

		}
	}

	DeleteObject(m_polygonal_region);
	delete [] m_subarea_points;

	int zone_count = 0;

	std::map<int, DTAZone>	:: const_iterator itr_zone;

	for(itr_zone = pDoc->m_ZoneMap.begin(); itr_zone != pDoc->m_ZoneMap.end(); ++itr_zone)
	{
		if(itr_zone->second.m_ActivityLocationVector .size() > 0)
			zone_count++;

	}

	pDoc->m_bSaveProjectFromSubareaCut =true;  // mark subarea cut

	CString SubareaCutMessage;
	SubareaCutMessage.Format ("The subarea includes %d nodes, %d links;\n%d zones, %d newly generated boundary zones;\n%d subarea OD pairs, %d subarea path records.",
		pDoc->m_NodeSet.size(), pDoc->m_LinkSet.size(), zone_count,boundary_zone_count,pDoc->m_ODMatrixMap.size(),
		pDoc->m_PathMap.size());

	AfxMessageBox(SubareaCutMessage, MB_ICONINFORMATION);

	Invalidate();
}

void CTLiteView::OnViewShowAVISensor()
{
	m_bShowAVISensor = !m_bShowAVISensor;
	Invalidate();
}

void CTLiteView::OnUpdateViewShowAVISensor(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowAVISensor ? 1 : 0);
}

void CTLiteView::OnFileDataexchangewithgooglefusiontables()
{
	//	CopyLinkSetInSubarea();
	CDlg_GoogleFusionTable dlg;
	dlg.m_pDoc= GetDocument();
	dlg.DoModal ();
}


void CTLiteView::OnEditDeletelinksoutsidesubarea()
{

	OnToolsRemovenodesandlinksoutsidesubarea();
}

void CTLiteView::OnEditMovenetworkcoordinates()
{
	m_MoveLayerNo = 0; //baseline layer
	m_ToolMode = network_coordinate_tool;
	m_bShowGrid = true;
	if(m_bNetworkCooridinateHints==false)
	{
		AfxMessageBox("Hints:\n1. Please use mouse to move the reference network and shift its coordinates;\n2. Use mouse wheel to adjust the size of the reference network (zoom);\n3. Click shift or control key together to change the horizonal or vertical size, respectively;\n4. Click on the other toolbar button to turn on disable this moving-reference-network mode.", MB_ICONINFORMATION);
		m_bNetworkCooridinateHints = true;
	}

	Invalidate();

}

void CTLiteView::OnUpdateEditMovenetworkcoordinates(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == network_coordinate_tool ? 1 : 0);
}


void CTLiteView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	switch (nChar)
	{
	case 27:  //ESCAPE

		m_ToolMode= select_feature_tool;

		break;
	case 90 :   //'Z'

		if( GetKeyState(VK_CONTROL) < 0)
		{
			CTLiteDoc* pDoc =GetDocument();
			pDoc->Undo();
		}

	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTLiteView::OnViewIncreasenodesize()
{
	CTLiteDoc* pDoc = GetDocument();

	if(pDoc->m_LinkMOEMode != MOE_bottleneck)
	{
		pDoc->m_NodeDisplaySize = max(pDoc->m_NodeDisplaySize *1.2, pDoc->m_NodeDisplaySize+1);

	}
	else if(pDoc->m_LinkMOEMode != MOE_vehicle)
	{

		pDoc->m_BottleneckDisplaySize = max(pDoc->m_BottleneckDisplaySize *1.2, pDoc->m_BottleneckDisplaySize+1);
		//pDoc->m_VehicleDisplaySize*=1.2;
	}


	Invalidate();
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	pMainFrame->UpdateLegendView();

}

void CTLiteView::OnViewDecreatenodesize()
{
	CTLiteDoc* pDoc = GetDocument();

	if(pDoc->m_LinkMOEMode != MOE_bottleneck)
	{
		pDoc->m_NodeDisplaySize /=1.2;
		pDoc->m_NodeDisplaySize = max(0.00001,pDoc->m_NodeDisplaySize);
	}
	else
	{
		pDoc->m_BottleneckDisplaySize/=1.2;
		pDoc->m_BottleneckDisplaySize = max(0.00001,pDoc->m_BottleneckDisplaySize);
	}


	Invalidate();
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	pMainFrame->UpdateLegendView();


}


void CTLiteView::DrawLinkAsLine(DTALink* pLink, CDC* pDC)
{
	// 

	if(pLink->m_ShapePoints.size() == 0)
		return;

	// normal line
	CTLiteDoc* pDoc = GetDocument();


	FromPoint = NPtoSP(pLink->m_ShapePoints[0]);
	ToPoint = NPtoSP(pLink->m_ShapePoints[pLink->m_ShapePoints.size() - 1]);

	if (FromPoint.x == ToPoint.x && FromPoint.y == ToPoint.y)  // same node
		return;

	if (abs(FromPoint.x - ToPoint.x < 10) && abs(FromPoint.y - ToPoint.y) < 10)  // almost same locations 
	{
		DrawLinkAsStraightLine(pLink, pDC);
		return;
	}

	for(int si = 0; si < pLink->m_ShapePoints.size()-1; si++)
	{
		FromPoint = NPtoSP(pLink->m_ShapePoints[si]);
		ToPoint = NPtoSP(pLink->m_ShapePoints[si+1]);

		if(FromPoint.x==ToPoint.x && FromPoint.y==ToPoint.y)  // same node
			continue; 


		if(pDoc->m_LinkMOEMode == MOE_queue_length)  // green color as background
		{

			if(pLink->m_LinkNo == pDoc->m_SelectedLinkNo)
				pDC->SelectObject(&g_PenBlue); 
			else
				pDC->SelectObject(&g_PenGreen); 
		}



		pDC->MoveTo(FromPoint);
		pDC->LineTo(ToPoint);

		if(m_bShowLinkArrow && si ==0)
		{
			double slopy = atan2((double)(FromPoint.y - ToPoint.y), (double)(FromPoint.x - ToPoint.x));
			double cosy = cos(slopy);
			double siny = sin(slopy);   
			double display_length  = sqrt((double)(FromPoint.y - ToPoint.y)*(FromPoint.y - ToPoint.y)+(double)(FromPoint.x - ToPoint.x)*(FromPoint.x - ToPoint.x));
			double arrow_size = min(7,display_length/5.0);

			if(arrow_size>0.2)
			{

				m_arrow_pts[0] = ToPoint;
				m_arrow_pts[1].x = ToPoint.x + (int)(arrow_size * cosy - (arrow_size / 2.0 * siny) + 0.5);
				m_arrow_pts[1].y = ToPoint.y + (int)(arrow_size * siny + (arrow_size / 2.0 * cosy) + 0.5);
				m_arrow_pts[2].x = ToPoint.x + (int)(arrow_size * cosy + arrow_size / 2.0 * siny + 0.5);
				m_arrow_pts[2].y = ToPoint.y - (int)(arrow_size / 2.0 * cosy - arrow_size * siny + 0.5);

				pDC->Polygon(m_arrow_pts, 3);
			}

		}

	}

	if(pDoc->m_LinkMOEMode == MOE_queue_length)   // queue length mode
	{
		CPen * pOldPen  = pDC->SelectObject(&g_PenQueueColor);
		float value;
		float queue_ratio = pDoc->GetLinkMOE(pLink, pDoc->m_LinkMOEMode,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value);

		if(queue_ratio> 1)
			queue_ratio = 1;

		if(queue_ratio<0)
			queue_ratio = 0;


		if(pLink->m_FromNodeNumber == 11 && pLink->m_ToNodeNumber == 12)
		{
			TRACE("");

		}
		for(int si = 0; si < pLink->m_ShapePoints.size()-1; si++)
		{
			bool bDrawQueueCell = false;

			FromPoint = NPtoSP(pLink->m_ShapePoints[si]);
			ToPoint = NPtoSP(pLink->m_ShapePoints[si+1]);

			if(FromPoint.x==ToPoint.x && FromPoint.y==ToPoint.y)  // same node
				continue; 


			if(queue_ratio < 0.005f)
				break;


			GDPoint pt;

			if(pLink->m_ShapePoints.size() == 2)
			{  // simple straight line



				pt.x =  pLink->m_ShapePoints[0].x + (1-queue_ratio) * (pLink->m_ShapePoints[1].x - pLink->m_ShapePoints[0].x);
				pt.y =  pLink->m_ShapePoints[0].y + (1-queue_ratio) * (pLink->m_ShapePoints[1].y - pLink->m_ShapePoints[0].y);

				FromPoint = NPtoSP(pt);  // new to point as the end of queue line
				bDrawQueueCell = true;

			}else  // more than 2 feature points
			{
				if( pLink->m_ShapePointRatios[si] > 1-queue_ratio) //  1- queue_ratio) is the starting point to draw queue: e.g. queue_ratio = 0.5, then we should draw queeus from 0.5 to 1.0
				{
					bDrawQueueCell = true;

					if (si >=1 && pLink->m_ShapePointRatios[si-1] < (1-queue_ratio)) // first segment to draw the link
					{
						float ratio  = ((pLink->m_ShapePointRatios[si] - (1-queue_ratio))/max(0.00001,pLink->m_ShapePointRatios[si] -pLink-> m_ShapePointRatios[si-1] ));

						if(ratio >=1.1  || ratio <-0.1)
							TRACE("");

						pt.x =  pLink->m_ShapePoints[si-1].x + (1-ratio) * (pLink->m_ShapePoints[si].x - pLink->m_ShapePoints[si-1].x);
						pt.y =  pLink->m_ShapePoints[si-1].y + (1-ratio) * (pLink->m_ShapePoints[si].y - pLink->m_ShapePoints[si-1].y);

						FromPoint = NPtoSP(pt);  // new from point as the end of queue line
						ToPoint = NPtoSP(pLink->m_ShapePoints[si]);  // to point as the beginning of queue line

					}


				}


			}
			if(bDrawQueueCell)
			{
				pDC->MoveTo(FromPoint);
				pDC->LineTo(ToPoint);
			}

		}



		pDC->SelectObject(pOldPen);
	}

}

void CTLiteView::DrawLinkAsStraightLine(DTALink* pLink, CDC* pDC)
{
	// 

	if (pLink->m_ShapePoints.size() == 0)
		return;

	// normal line
	CTLiteDoc* pDoc = GetDocument();

		FromPoint = NPtoSP(pLink->m_ShapePoints[0]);
		ToPoint = NPtoSP(pLink->m_ShapePoints[pLink->m_ShapePoints.size() - 1]);


		if (pDoc->m_LinkMOEMode == MOE_queue_length)  // green color as background
		{

			if (pLink->m_LinkNo == pDoc->m_SelectedLinkNo)
				pDC->SelectObject(&g_PenBlue);
			else
				pDC->SelectObject(&g_PenGreen);
		}

		pDC->MoveTo(FromPoint);
		pDC->LineTo(ToPoint);

		if (m_bShowLinkArrow)
		{
			double slopy = atan2((double)(FromPoint.y - ToPoint.y), (double)(FromPoint.x - ToPoint.x));
			double cosy = cos(slopy);
			double siny = sin(slopy);
			double display_length = sqrt((double)(FromPoint.y - ToPoint.y)*(FromPoint.y - ToPoint.y) + (double)(FromPoint.x - ToPoint.x)*(FromPoint.x - ToPoint.x));
			double arrow_size = min(7, display_length / 5.0);

			if (arrow_size>0.2)
			{

				m_arrow_pts[0] = ToPoint;
				m_arrow_pts[1].x = ToPoint.x + (int)(arrow_size * cosy - (arrow_size / 2.0 * siny) + 0.5);
				m_arrow_pts[1].y = ToPoint.y + (int)(arrow_size * siny + (arrow_size / 2.0 * cosy) + 0.5);
				m_arrow_pts[2].x = ToPoint.x + (int)(arrow_size * cosy + arrow_size / 2.0 * siny + 0.5);
				m_arrow_pts[2].y = ToPoint.y - (int)(arrow_size / 2.0 * cosy - arrow_size * siny + 0.5);

				pDC->Polygon(m_arrow_pts, 3);
			}

		}

	

	if (pDoc->m_LinkMOEMode == MOE_queue_length)   // queue length mode
	{
		CPen * pOldPen = pDC->SelectObject(&g_PenQueueColor);
		float value;
		float queue_ratio = pDoc->GetLinkMOE(pLink, pDoc->m_LinkMOEMode, (int)g_Simulation_Time_Stamp, g_MOEAggregationIntervalInMin, value);

		if (queue_ratio> 1)
			queue_ratio = 1;

		if (queue_ratio<0)
			queue_ratio = 0;


			bool bDrawQueueCell = false;


			if (queue_ratio < 0.005f)
				return;


			GDPoint pt;

			pt.x = pLink->m_ShapePoints[0].x + (1 - queue_ratio) * (pLink->m_ShapePoints[pLink->m_ShapePoints.size()-1].x - pLink->m_ShapePoints[0].x);
			pt.y = pLink->m_ShapePoints[0].y + (1 - queue_ratio) * (pLink->m_ShapePoints[pLink->m_ShapePoints.size() - 1].y - pLink->m_ShapePoints[0].y);

				FromPoint = NPtoSP(pt);  // new to point as the end of queue line
				bDrawQueueCell = true;

			if (bDrawQueueCell)
			{
				pDC->MoveTo(FromPoint);
				pDC->LineTo(ToPoint);
			}


		pDC->SelectObject(pOldPen);
	}


}

bool CTLiteView::DrawLinkAsBand(DTALink* pLink, CDC* pDC, bool bObservationFlag =false)
{
	// draw queue length
	CTLiteDoc* pDoc = GetDocument();


	if(pLink->m_BandLeftShapePoints.size() == 0)
		return false;

	int band_point_index = 0;  

	if(pLink ->m_ShapePoints.size() > 900)
	{
		AfxMessageBox("Too many shape points...");
		return false;
	}
	int si; // we should not use unsigned integer here as si-- 
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();


	FromPoint = NPtoSP(pLink->m_ShapePoints[0]);
	ToPoint = NPtoSP(pLink->m_ShapePoints[pLink->m_ShapePoints.size() - 1]);

	if (FromPoint.x == ToPoint.x && FromPoint.y == ToPoint.y)  // same node
		return true;

	if (abs(FromPoint.x - ToPoint.x < 10) && abs(FromPoint.y - ToPoint.y) < 10)  // almost same locations 
	{
		DrawLinkAsStraightBand(pLink, pDC, bObservationFlag);
		return true;
	}

	if(bObservationFlag == false)
	{  // simulated data

		if( pLink ->m_BandLeftShapePoints.size() > 0)
		{
			for(si = 0; si < pLink ->m_BandLeftShapePoints .size(); si++)
			{
				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_BandLeftShapePoints[si]);
			}

			for(si = pLink ->m_BandRightShapePoints .size()-1; si >=0 ; si--)
			{
				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_BandRightShapePoints[si]);
			}

			m_BandPoint[band_point_index++]= NPtoSP(pLink->m_BandLeftShapePoints[0]);
		}

	}else if (pMainFrame->m_bShowLayerMap[layer_detector])
	{  //observed data
		if(pLink ->m_ReferenceBandLeftShapePoints.size() > 0)  // m_ReferenceBandLeftShapePoints has been initialized
		{

			for(si = 0; si < pLink ->m_ShapePoints .size(); si++)
			{
				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_ReferenceBandLeftShapePoints[si]);
			}

			for(si = pLink ->m_ShapePoints .size()-1; si >=0 ; si--)
			{
				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_ReferenceBandRightShapePoints[si]);
			}

			m_BandPoint[band_point_index++]= NPtoSP(pLink->m_ReferenceBandLeftShapePoints[0]);
		}

	}

	if(pDoc->m_LinkMOEMode == MOE_queue_length)  // green color as background
	{

		if(pLink->m_LinkNo == pDoc->m_SelectedLinkNo)
		{
			pDC->SelectObject(&g_PenBlue); 
			pDC->SelectObject(&g_BrushBlue); 


		}else
		{
			pDC->SelectObject(&g_PenGreen); 
			pDC->SelectObject(&g_BrushGreen); 
		}

	}

	pDC->Polygon(m_BandPoint, band_point_index);

	// ****************************************/

	if(pDoc->m_LinkMOEMode == MOE_queue_length)   // queue length mode
	{
		CPen * pOldPen  = pDC->SelectObject(&g_PenQueueColor);
		float value;

		if(pLink->m_FromNodeNumber == 11 && pLink->m_ToNodeNumber == 12)
		{
			TRACE("");

		}
		float queue_ratio = pDoc->GetLinkMOE(pLink, pDoc->m_LinkMOEMode,(int)g_Simulation_Time_Stamp,g_MOEAggregationIntervalInMin, value)/100.0;



		if(queue_ratio> 1)
			queue_ratio = 1;

		if(queue_ratio<0)
			queue_ratio = 0;



		for(int si = 0; si < pLink->m_ShapePoints.size()-1; si++)
		{
			bool bDrawQueueCell = false;

			FromPoint = NPtoSP(pLink->m_ShapePoints[si]);
			ToPoint = NPtoSP(pLink->m_ShapePoints[si+1]);

			if(FromPoint.x==ToPoint.x && FromPoint.y==ToPoint.y)  // same node
				continue; 


			GDPoint pt;

			if(pLink->m_ShapePoints.size() == 2)
			{  // simple straight line


				if(queue_ratio < 0.01f)
					break;

				pt.x =  pLink->m_ShapePoints[0].x + (1-queue_ratio) * (pLink->m_ShapePoints[1].x - pLink->m_ShapePoints[0].x);
				pt.y =  pLink->m_ShapePoints[0].y + (1-queue_ratio) * (pLink->m_ShapePoints[1].y - pLink->m_ShapePoints[0].y);

				FromPoint = NPtoSP(pt);  // new to point as the end of queue line
				bDrawQueueCell = true;

			}else  // more than 2 feature points
			{
				if( pLink->m_ShapePointRatios[si] > 1-queue_ratio) //  1- queue_ratio) is the starting point to draw queue: e.g. queue_ratio = 0.5, then we should draw queeus from 0.5 to 1.0
				{
					bDrawQueueCell = true;

					if (si >=1 && pLink->m_ShapePointRatios[si-1] < (1-queue_ratio)) // first segment to draw the link
					{
						float ratio  = ((pLink->m_ShapePointRatios[si] - (1-queue_ratio))/max(0.00001,pLink->m_ShapePointRatios[si] -pLink-> m_ShapePointRatios[si-1] ));

						if(ratio >=1.1  || ratio <-0.1)
							TRACE("");

						pt.x =  pLink->m_ShapePoints[si-1].x + (1-ratio) * (pLink->m_ShapePoints[si].x - pLink->m_ShapePoints[si-1].x);
						pt.y =  pLink->m_ShapePoints[si-1].y + (1-ratio) * (pLink->m_ShapePoints[si].y - pLink->m_ShapePoints[si-1].y);

						FromPoint = NPtoSP(pt);  // new to point as the end of queue line

						ToPoint = NPtoSP(pLink->m_ShapePoints[si]);  // end of queue line

					}


				}


			}
			if(bDrawQueueCell)
			{
				pDC->MoveTo(FromPoint);
				pDC->LineTo(ToPoint);
			}

		}



		pDC->SelectObject(pOldPen);
	}


	return true;
}


bool CTLiteView::DrawLinkAsStraightBand(DTALink* pLink, CDC* pDC, bool bObservationFlag = false)
{
	// draw queue length
	CTLiteDoc* pDoc = GetDocument();


	if (pLink->m_BandLeftShapePoints.size() == 0)
		return false;

	int band_point_index = 0;

	if (pLink->m_ShapePoints.size() > 900)
	{
		AfxMessageBox("Too many shape points...");
		return false;
	}
	int si; // we should not use unsigned integer here as si-- 
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();


	FromPoint = NPtoSP(pLink->m_ShapePoints[0]);
	ToPoint = NPtoSP(pLink->m_ShapePoints[pLink->m_ShapePoints.size() - 1]);

	if (FromPoint.x == ToPoint.x && FromPoint.y == ToPoint.y)  // same node
		return true;

	if (bObservationFlag == false)
	{  // simulated data

		if (pLink->m_BandLeftShapePoints.size() > 0)
		{

				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_BandLeftShapePoints[0]);
				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_BandLeftShapePoints[pLink->m_BandRightShapePoints.size() - 1]);

				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_BandRightShapePoints[pLink->m_BandRightShapePoints.size() - 1]);
				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_BandRightShapePoints[0]);

				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_BandLeftShapePoints[0]);
		}

	}
	else if (pMainFrame->m_bShowLayerMap[layer_detector])
	{  //observed data
		if (pLink->m_ReferenceBandLeftShapePoints.size() > 0)  // m_ReferenceBandLeftShapePoints has been initialized
		{

				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_ReferenceBandLeftShapePoints[0]);
				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_ReferenceBandLeftShapePoints[pLink->m_ReferenceBandLeftShapePoints.size()-1]);


				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_ReferenceBandRightShapePoints[pLink->m_ReferenceBandLeftShapePoints.size() - 1]);
				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_ReferenceBandRightShapePoints[0]);

				m_BandPoint[band_point_index++] = NPtoSP(pLink->m_ReferenceBandLeftShapePoints[0]);
		}

	}

	if (pDoc->m_LinkMOEMode == MOE_queue_length)  // green color as background
	{

		if (pLink->m_LinkNo == pDoc->m_SelectedLinkNo)
		{
			pDC->SelectObject(&g_PenBlue);
			pDC->SelectObject(&g_BrushBlue);


		}
		else
		{
			pDC->SelectObject(&g_PenGreen);
			pDC->SelectObject(&g_BrushGreen);
		}

	}

	pDC->Polygon(m_BandPoint, band_point_index);

	// ****************************************/

	if (pDoc->m_LinkMOEMode == MOE_queue_length)   // queue length mode
	{
		CPen * pOldPen = pDC->SelectObject(&g_PenQueueColor);
		float value;

		if (pLink->m_FromNodeNumber == 11 && pLink->m_ToNodeNumber == 12)
		{
			TRACE("");

		}
		float queue_ratio = pDoc->GetLinkMOE(pLink, pDoc->m_LinkMOEMode, (int)g_Simulation_Time_Stamp, g_MOEAggregationIntervalInMin, value) / 100.0;



		if (queue_ratio> 1)
			queue_ratio = 1;

		if (queue_ratio<0)
			queue_ratio = 0;



			bool bDrawQueueCell = false;

			if (FromPoint.x == ToPoint.x && FromPoint.y == ToPoint.y)  // same node
				return true;


			GDPoint pt;

				if (queue_ratio < 0.01f)
					return true;

				pt.x = pLink->m_ShapePoints[0].x + (1 - queue_ratio) * (pLink->m_ShapePoints[pLink->m_ShapePoints.size()-1].x - pLink->m_ShapePoints[0].x);
				pt.y = pLink->m_ShapePoints[0].y + (1 - queue_ratio) * (pLink->m_ShapePoints[pLink->m_ShapePoints.size() - 1].y - pLink->m_ShapePoints[0].y);

				FromPoint = NPtoSP(pt);  // new to point as the end of queue line
				bDrawQueueCell = true;

			if (bDrawQueueCell)
			{
				pDC->MoveTo(FromPoint);
				pDC->LineTo(ToPoint);
			}

				pDC->SelectObject(pOldPen);
	}


	return true;
}


bool CTLiteView::DrawLinkAsLaneGroup(DTALink* pLink, CDC* pDC)
{
	CTLiteDoc* pDoc = GetDocument();


	pLink->m_SetBackStart = min (pDoc->m_NodeDisplaySize, pLink->m_Length*0.3);  // restrict set back to 30% of link length
	pLink->m_SetBackEnd = min (pDoc->m_NodeDisplaySize, pLink->m_Length*0.3);

	pLink->AdjustLinkEndpointsWithSetBack();

	if(pLink ->m_ShapePoints.size() > 900)
	{
		AfxMessageBox("Two many shape points...");
		return false;
	}


	// generate m_BandLeftShapePoints, m_BandRightShapePoints for the whole link with setback points
	pLink->m_BandLeftShapePoints.clear();
	pLink->m_BandRightShapePoints.clear();


	double lane_offset = pDoc->m_UnitFeet*pDoc->m_LaneWidthInFeet;  // 20 feet per lane

	int last_shape_point_id = pLink->m_ShapePoints .size() -1;
	double DeltaX = pLink->m_ShapePoints[last_shape_point_id].x - pLink->m_ShapePoints[0].x;
	double DeltaY = pLink->m_ShapePoints[last_shape_point_id].y - pLink->m_ShapePoints[0].y;
	double theta = atan2(DeltaY, DeltaX);

	int si;  // we cannot use unsigned int here as there is a conditoin below
	// 	for(si = pLink ->m_ShapePoints .size()-1; si >=0 ; si--)
	for(si = 0; si < pLink ->m_ShapePoints .size(); si++)
	{
		GDPoint pt;

		GDPoint OrgShapepoint = pLink->m_ShapePoints[si];

		if(si == 0)
			OrgShapepoint = pLink->m_FromPointWithSetback;

		if(si == last_shape_point_id)
			OrgShapepoint = pLink->m_ToPointWithSetback;

		pt.x = OrgShapepoint.x - lane_offset* cos(theta-PI/2.0f);
		pt.y = OrgShapepoint.y - lane_offset* sin(theta-PI/2.0f);

		pLink->m_BandLeftShapePoints.push_back (pt);

		pt.x  = OrgShapepoint.x + pLink->m_NumberOfLanes *lane_offset* cos(theta-PI/2.0f);
		pt.y = OrgShapepoint.y + pLink->m_NumberOfLanes*lane_offset* sin(theta-PI/2.0f);

		pLink->m_BandRightShapePoints.push_back (pt);
	}



	int band_point_index = 0;
	// close the loop to construct

	for(si = 0; si < pLink ->m_ShapePoints .size(); si++)
	{
		m_BandPoint[band_point_index++] = NPtoSP(pLink->m_BandLeftShapePoints[si]);
	}

	for(si = pLink ->m_ShapePoints .size()-1; si >=0 ; si--)
	{
		m_BandPoint[band_point_index++] = NPtoSP(pLink->m_BandRightShapePoints[si]);
	}

	m_BandPoint[band_point_index++]= NPtoSP(pLink->m_BandLeftShapePoints[0]);


	pDC->Polygon(m_BandPoint, band_point_index);

	/*
	// draw lane markings

	CPen* OldPen = pDC->SelectObject(&g_LaneMarkingPen);

	for(int lane = 1; lane < pLink->m_NumberOfLanes ; lane++)
	{
	for(si = 0; si < pLink ->m_ShapePoints .size(); si++)
	{
	GDPoint pt;
	GDPoint OrgShapepoint = pLink->m_ShapePoints[si];

	if(si == 0)
	{
	OrgShapepoint = pLink->m_FromPointWithSetback;
	}

	if(si == last_shape_point_id)
	OrgShapepoint = pLink->m_ToPointWithSetback;

	pt.x = OrgShapepoint.x + lane*lane_offset* cos(theta-PI/2.0f);
	pt.y = OrgShapepoint.y + lane*lane_offset* sin(theta-PI/2.0f);

	if(si == 0)
	pDC->MoveTo(NPtoSP(pt));
	else
	pDC->LineTo(NPtoSP(pt));

	}

	}
	pDC->SelectObject(OldPen);  // restore pen
	*/
	return true;
}

void CTLiteView::OnViewDisplaylanewidth()
{
	if(m_link_display_mode ==link_display_mode_line )
		m_link_display_mode = link_display_mode_band;
	else
		m_link_display_mode = link_display_mode_line;

	Invalidate();
}


void CTLiteView::OnNodeCheckconnectivityfromhere()
{
	CTLiteDoc* pDoc = GetDocument();
	pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit
	pDoc->m_OriginNodeID = pDoc->m_SelectedNodeID;
	pDoc->Routing(true);
	m_ShowAllPaths = true;
	Invalidate();
}



extern void g_RandomCapacity(float* ptr, int num, float mean, float COV,int seed);
#define MAX_SAMPLE_SIZE 200

void CTLiteView::OnNodeDirectiontohereandreliabilityanalysis()
{   
	OnNodeDestination();


}
void CTLiteView::OnLinkIncreasebandwidth()
{
	CTLiteDoc* pDoc = GetDocument();

	// 
	if(pDoc->m_LinkBandWidthMode == LBW_link_volume || pDoc->m_LinkBandWidthMode == LBW_congestion_duration || pDoc->m_LinkBandWidthMode == LBW_number_of_lanes)
	{
		pDoc->m_MaxLinkWidthAsLinkVolume  /=1.2;
	}

	pDoc->GenerateOffsetLinkBand();

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	pMainFrame->UpdateLegendView();

	Invalidate();
}

void CTLiteView::OnLinkDecreasebandwidth()
{
	CTLiteDoc* pDoc = GetDocument();


	// 
	if(pDoc->m_LinkBandWidthMode == LBW_link_volume ||pDoc->m_LinkBandWidthMode == LBW_congestion_duration ||  pDoc->m_LinkBandWidthMode == LBW_number_of_lanes)
	{
		pDoc->m_MaxLinkWidthAsLinkVolume  *=1.2;
	}


	pDoc->GenerateOffsetLinkBand();
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	pMainFrame->UpdateLegendView();
	Invalidate();

}

void CTLiteView::OnLinkSwichtolineBandwidthMode()
{

	if(m_link_display_mode == link_display_mode_band)
		m_link_display_mode = link_display_mode_line;
	else
		m_link_display_mode = link_display_mode_band;

	CTLiteDoc* pDoc = GetDocument();
	pDoc->GenerateOffsetLinkBand();


	Invalidate();
}

void CTLiteView::OnViewTransitlayer()
{
	m_bShowTransit = !m_bShowTransit;
	Invalidate();
}

void CTLiteView::OnUpdateViewTransitlayer(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowTransit);
}

void CTLiteView::DrawPublicTransitLayer(CDC *pDC)
{

	CTLiteDoc* pDoc = GetDocument();
	std::map<int, int> RouteMap;

	int stop_size = min(20,max(2,int(pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution)));

	TEXTMETRIC tmOD;
	memset(&tmOD, 0, sizeof TEXTMETRIC);
	pDC->GetOutputTextMetrics(&tmOD);

	CRect ScreenRect;
	GetClientRect(ScreenRect);

	std::map<int, PT_Trip>::iterator iPT_TripMap;
	for ( iPT_TripMap= pDoc->m_PT_network.m_PT_TripMap.begin() ; iPT_TripMap != pDoc->m_PT_network.m_PT_TripMap.end(); iPT_TripMap++ )
	{


		if(RouteMap.find((*iPT_TripMap).second.route_id )!= RouteMap.end())
		{
			// the route has been drawed
			continue;
		}


		RouteMap[(*iPT_TripMap).second.route_id] = 1;

		CPoint ScreenPoint, Prev_ScreenPoint;

		for(int si = 0; si < (*iPT_TripMap).second .m_PT_StopTimeVector.size(); si++)
		{

			GDPoint shape_point = (*iPT_TripMap).second .m_PT_StopTimeVector[si].pt ;

			ScreenPoint = NPtoSP(shape_point);

			CRect node_rect;
			node_rect.SetRect(ScreenPoint.x-50,ScreenPoint.y-50,ScreenPoint.x+50,ScreenPoint.y+50);

			//if(RectIsInsideScreen(node_rect,ScreenRect) == false)  // not inside the screen boundary
			//	continue;

			pDC->Rectangle (ScreenPoint.x - stop_size, ScreenPoint.y + stop_size,
				ScreenPoint.x + stop_size, ScreenPoint.y - stop_size);

			if(stop_size>=1)
			{
				CString station_str;
				station_str.Format ("S%d",(*iPT_TripMap).second .m_PT_StopTimeVector[si].stop_id);
				pDC->TextOut(ScreenPoint.x , ScreenPoint.y , station_str);

				ScreenPoint.y -= tmOD.tmHeight *2/ 3;
				station_str.Format ("R%d",(*iPT_TripMap).second.route_id);
				pDC->TextOut(ScreenPoint.x , ScreenPoint.y , station_str);
			}

			if(si==0)
			{
				pDC->MoveTo (ScreenPoint);
			}else
				//					if( ((Prev_ScreenPoint.x == Prev_ScreenPoint.x)&& (Prev_ScreenPoint.y == Prev_ScreenPoint.y)) == false) // no overlapping points
			{
				pDC->LineTo  (ScreenPoint);
				Prev_ScreenPoint = ScreenPoint;

			}



		} // for each stop
	} // for each trip

}

void CTLiteView::DrawPublicTransitAccessibilityLayer(CDC *pDC)
{

	//CWaitCursor wait;
	CTLiteDoc* pDoc = GetDocument();
	std::map<int, int> RouteMap;

	int stop_size = min(20,max(2,int(pDoc->m_NodeDisplaySize*pDoc->m_UnitFeet*m_Resolution)));

	TEXTMETRIC tmOD;
	memset(&tmOD, 0, sizeof TEXTMETRIC);
	pDC->GetOutputTextMetrics(&tmOD);

	CRect ScreenRect;
	GetClientRect(ScreenRect);

	std::map<int, PT_Trip>::iterator iPT_TripMap;
	for ( iPT_TripMap= pDoc->m_PT_network.m_PT_TripMap.begin() ; iPT_TripMap != pDoc->m_PT_network.m_PT_TripMap.end(); iPT_TripMap++ )
	{


		if(pDoc->m_AccessibleTripIDMap.find((*iPT_TripMap).second.trip_id  )== pDoc->m_AccessibleTripIDMap.end())
		{
			// the trip is not included in origin's accessible list
			continue;
		}


		CPoint ScreenPoint, Prev_ScreenPoint;

		bool ini_flag = true;

		for(int si = 0; si < (*iPT_TripMap).second .m_PT_StopTimeVector.size(); si++)
		{

			int stop_time_in_min = (*iPT_TripMap).second .m_PT_StopTimeVector[si].departure_time ;

			if(stop_time_in_min>= g_Simulation_Time_Stamp && stop_time_in_min< g_Simulation_Time_Stamp+  pDoc->m_max_accessible_transit_time_in_min)
		 {

			 GDPoint shape_point = (*iPT_TripMap).second .m_PT_StopTimeVector[si].pt ;

			 ScreenPoint = NPtoSP(shape_point);

			 CRect node_rect;
			 node_rect.SetRect(ScreenPoint.x-50,ScreenPoint.y-50,ScreenPoint.x+50,ScreenPoint.y+50);

			 //if(RectIsInsideScreen(node_rect,ScreenRect) == false)  // not inside the screen boundary
			 //	continue;

			 pDC->Rectangle (ScreenPoint.x - stop_size, ScreenPoint.y + stop_size,
				 ScreenPoint.x + stop_size, ScreenPoint.y - stop_size);

			 if(stop_size>=1)
			 {
				 CString station_str;
				 station_str.Format ("S%d",(*iPT_TripMap).second .m_PT_StopTimeVector[si].stop_id);
				 pDC->TextOut(ScreenPoint.x , ScreenPoint.y , station_str);

				 ScreenPoint.y -= tmOD.tmHeight *2/ 3;
				 station_str.Format ("R%d",(*iPT_TripMap).second.route_id);
				 pDC->TextOut(ScreenPoint.x , ScreenPoint.y , station_str);
			 }

			 if(ini_flag)
				{
					pDC->MoveTo (ScreenPoint);
					ini_flag = false;
			 }else
				 //					if( ((Prev_ScreenPoint.x == Prev_ScreenPoint.x)&& (Prev_ScreenPoint.y == Prev_ScreenPoint.y)) == false) // no overlapping points
				{
					pDC->LineTo  (ScreenPoint);
					Prev_ScreenPoint = ScreenPoint;

				}


		 }

		} // for each stop
	} // for each trip

}



void CTLiteView::DrawNodeRadarChart(CDC *pDC, DTANode* pNode, CPoint point, int chart_size, float delay_ratio, int LOS)
{

	CTLiteDoc* pDoc = GetDocument();

 	CRect ChartRect(point.x - chart_size, point.y + chart_size,
			point.x + chart_size, point.y - chart_size);


	int theSliceCount = 0;

    CPen pen(PS_SOLID, 1, RGB(0,0,255));
    pDC->SelectObject(&pen);

	if(pNode->m_IncomingLinkVector.size()==1)
	{
    pDC->Ellipse(ChartRect);
	
		return;
	}


   
	
	 std::vector<IncomingLinkAngle> IncomingLinkVector;


		for(unsigned int i = 0; i< pNode->m_IncomingLinkVector.size(); i++)
		{
	
			DTALink* pLink = pDoc->m_LinkNoMap[pNode->m_IncomingLinkVector[i]];
			int Angle = pDoc->Find_P2P_Angle(  pDoc->m_NodeNoMap[pLink->m_FromNodeID]->pt, pDoc->m_NodeNoMap[pLink->m_ToNodeID]->pt);
		
		
			IncomingLinkAngle element;
			element.pLink = pLink;
			element.Angle = Angle;

		
			IncomingLinkVector.push_back(element);
		}


		std::sort(IncomingLinkVector.begin(), IncomingLinkVector.end(), compare_IncomingLinkAngleData);


		CPoint PtVector[10];
		int vector_size = min(10, IncomingLinkVector.size());
		for(unsigned int i = 0; i< vector_size ; i++)
		{
			
			DTALink* pLink = IncomingLinkVector[i].pLink ;

			float link_delay = pLink->GetDynamicNodeDelay(g_Simulation_Time_Stamp, pDoc->m_PrimaryDataSource );
			float radius_onscreen = link_delay *pDoc->m_BottleneckDisplaySize;

			radius_onscreen = max(2,radius_onscreen);
			

			int point_size = pLink->m_ShapePoints.size();


			CPoint pt0 =  NPtoSP(pLink->m_ShapePoints[0]); 
			CPoint pt1 =  NPtoSP(pLink->m_ShapePoints[point_size - 1]); 

			double delta_x = pt0.x- pt1.x;
			double delta_y = pt0.y- pt1.y;

			double link_screen_size = sqrt(pow(delta_x,2) +  pow(delta_y,2) );

			double ratio = 1.0f - radius_onscreen/max(1,link_screen_size);
			CPoint pt;

			pt.x =  (1-ratio)*pt0.x + ratio * pt1.x;
			pt.y =  (1-ratio)*pt0.y + ratio * pt1.y;

				PtVector[i]= pt;
			
		}

		   pDC->Polygon(PtVector, vector_size);


//		for(unsigned int i = 0; i< theSliceCount; i++)
//		{
//			
//			DTALink* pLink = pDoc->m_LinkNoMap[pNode->m_IncomingLinkVector[i]];
//			
//			SlicePercentage[i] = link_delay[i]/ max(1,total_delay)*100;
//
//				// method 1: identify movement approach from scratch 
//			DTA_Direction direction =  pDoc->Find_Closest_Angle_to_Approach( pDoc->Find_P2P_Angle(pLink->m_FromPoint,pLink->m_ToPoint));
//
//			color_vector[i] = pDoc->m_ColorDirectionVector[direction];
//
//		}
///*
//		for(unsigned int i = 0; i< theSliceCount; i++)
//		{
//			
//			DTALink* pLink = pDoc->m_LinkNoMap[pNode->m_IncomingLinkVector[i]];
//			SlicePercentage[i] = link_delay[i]/ max(1,total_delay)*100;
//
//			thePieInfo[i].color = 
//			thePieInfo[i].desc.Format("  ");
//			thePieInfo[i].percentage = SlicePercentage[i];
//
//		}
//		*/
//	thePieInfo[0] = PieInfo("Stocks", SlicePercentage[0], color_vector[0]);
//    thePieInfo[1] = PieInfo("Bonds", SlicePercentage[1], color_vector[1]);
//    thePieInfo[2] = PieInfo("Bonds", SlicePercentage[2], color_vector[2]);
//    thePieInfo[3] = PieInfo("Bonds", SlicePercentage[3], color_vector[3]);
//    thePieInfo[4] = PieInfo("Bonds", SlicePercentage[4], color_vector[4]);
// 
//
//
//    double percent = 0;
//
//    for(int i = 0; i < theSliceCount; i++)
//    {
//        percent += thePieInfo[i].percentage;
//        if(percent > 100)
//            percent = 100;
//        double angle = percent / 100 * 360;
//        double x = radius * cos(angle * PI / 180);
//        double y = radius * sin(angle * PI / 180);
//
//		SlicePt[i].x = (int)(xCenter + x + 0.5);
//		SlicePt[i].y = (int) (yCenter + y + 0.5);
//    }
// 
//
//    for(int i = 0; i < theSliceCount; i++)
//    {
//        CBrush br;
//        br.CreateSolidBrush(thePieInfo[i].color);
//        pDC->SelectObject(&br);
//
//		int index = i+1;
//
//		if(index == theSliceCount)
//			index =0;
//
//		pDC->Pie(rect,SlicePt[i], SlicePt[index]);
//		
//	}

}
void CTLiteView::DrawNodePieChart(CDC *pDC, DTANode* pNode, CPoint point, int chart_size, int LOS)
{
	PieInfo thePieInfo[32];
	CPoint SlicePt[32];
	float SlicePercentage[32];
	float link_delay[32];
	COLORREF color_vector[32];

	CTLiteDoc* pDoc = GetDocument();
	CRect ChartRect(point.x - chart_size, point.y + chart_size,
			point.x + chart_size, point.y - chart_size);


	int theSliceCount = 0;

	CRect rect = ChartRect;
  
    CPen pen(PS_SOLID, 1, RGB(0,0,255));
    pDC->SelectObject(&pen);
    pDC->Ellipse(rect);
 
  //  double xCenter = (rect.left + rect.right) / 2;
  //  double yCenter = (rect.top + rect.bottom) / 2;
 
  //  double radius = rect.Height() / 2;

		//theSliceCount = min(2,pNode->m_IncomingLinkVector.size());
 
		//float total_delay = 0;
		//
		//for(unsigned int i = 0; i< theSliceCount; i++)
		//{
		//	
		//	DTALink* pLink = pDoc->m_LinkNoMap[pNode->m_IncomingLinkVector[i]];

		//	link_delay[i]= pLink->GetSimulatedNodeDelay(g_Simulation_Time_Stamp);
		//	total_delay+= link_delay[i];
		//}


//		for(unsigned int i = 0; i< theSliceCount; i++)
//		{
//			
//			DTALink* pLink = pDoc->m_LinkNoMap[pNode->m_IncomingLinkVector[i]];
//			
//			SlicePercentage[i] = link_delay[i]/ max(1,total_delay)*100;
//
//				// method 1: identify movement approach from scratch 
//			DTA_Direction direction =  pDoc->Find_Closest_Angle_to_Approach( pDoc->Find_P2P_Angle(pLink->m_FromPoint,pLink->m_ToPoint));
//
//			color_vector[i] = pDoc->m_ColorDirectionVector[direction];
//
//		}
///*
//		for(unsigned int i = 0; i< theSliceCount; i++)
//		{
//			
//			DTALink* pLink = pDoc->m_LinkNoMap[pNode->m_IncomingLinkVector[i]];
//			SlicePercentage[i] = link_delay[i]/ max(1,total_delay)*100;
//
//			thePieInfo[i].color = 
//			thePieInfo[i].desc.Format("  ");
//			thePieInfo[i].percentage = SlicePercentage[i];
//
//		}
//		*/
//	thePieInfo[0] = PieInfo("Stocks", SlicePercentage[0], color_vector[0]);
//    thePieInfo[1] = PieInfo("Bonds", SlicePercentage[1], color_vector[1]);
//    thePieInfo[2] = PieInfo("Bonds", SlicePercentage[2], color_vector[2]);
//    thePieInfo[3] = PieInfo("Bonds", SlicePercentage[3], color_vector[3]);
//    thePieInfo[4] = PieInfo("Bonds", SlicePercentage[4], color_vector[4]);
// 
//
//
//    double percent = 0;
//
//    for(int i = 0; i < theSliceCount; i++)
//    {
//        percent += thePieInfo[i].percentage;
//        if(percent > 100)
//            percent = 100;
//        double angle = percent / 100 * 360;
//        double x = radius * cos(angle * PI / 180);
//        double y = radius * sin(angle * PI / 180);
//
//		SlicePt[i].x = (int)(xCenter + x + 0.5);
//		SlicePt[i].y = (int) (yCenter + y + 0.5);
//    }
// 
//
//    for(int i = 0; i < theSliceCount; i++)
//    {
//        CBrush br;
//        br.CreateSolidBrush(thePieInfo[i].color);
//        pDC->SelectObject(&br);
//
//		int index = i+1;
//
//		if(index == theSliceCount)
//			index =0;
//
//		pDC->Pie(rect,SlicePt[i], SlicePt[index]);
//		
//	}

}

void CTLiteView::DrawNode(CDC *pDC, DTANode* pNode, CPoint point, int node_size,TEXTMETRIC tm)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	if (pMainFrame->m_bShowLayerMap[layer_reference_line]==false)
	{
		if (pNode->m_LayerNo == 1)  //skip reference nodes
			return; 
	}

	//if(pNode->m_ZoneID >0)  // if destination node associated with zones
	//{
	//	float zone_multipler = 1.2;
	//	pDC->Ellipse(point.x - node_size*zone_multipler, point.y + node_size*zone_multipler,
	//		point.x + node_size*zone_multipler, point.y - node_size*zone_multipler);
	//}
	CTLiteDoc* pDoc = GetDocument();

	if (node_size < 2)  // node display size is too small to  show node labels 
	{
	
		pDC->MoveTo(point.x, point.y);
		pDC->LineTo(point.x, point.y);

		return;
	}
else
{


	if(pNode->m_ControlType == pDoc->m_ControlType_PretimedSignal || 
		pNode->m_ControlType == pDoc->m_ControlType_ActuatedSignal)  // traffic signal control
	{
		pDC->Rectangle (point.x - node_size, point.y + node_size,
			point.x + node_size, point.y - node_size);
	}else if (pNode->m_ControlType == pDoc->m_ControlType_YieldSign || 
		pNode->m_ControlType == pDoc->m_ControlType_2wayStopSign ||
		pNode->m_ControlType == pDoc->m_ControlType_4wayStopSign)
	{
		int  width_of_ellipse = node_size*4/5;
		pDC->RoundRect (point.x - node_size, point.y + node_size,
			point.x + node_size, point.y - node_size,width_of_ellipse,width_of_ellipse);

	}else
	{
		pDC->Ellipse(point.x - node_size, point.y + node_size,
			point.x + node_size, point.y - node_size);
	}
}

	// 
	if (node_size < 4)  // node display size is too small to  show node labels 
		return; 

	if(m_ShowNodeTextMode != node_display_none)
	{

		CString str_node_label;
		str_node_label.Format ("%d",pNode->m_NodeNumber );

		if(m_ShowNodeTextMode == node_display_sequential_node_number)
		{
			str_node_label.Format ("%d",pNode->m_NodeNo +1 );

		}



		if(m_ShowNodeTextMode == node_display_zone_number)
		{

			if(pNode->m_ZoneID > 0)  // external origin
				str_node_label.Format ("%d",pNode->m_ZoneID );
			else
				str_node_label.Format ("");
		}

		if(m_ShowNodeTextMode == node_display_cycle_length_in_second || m_ShowNodeTextMode == node_display_cycle_length_in_second_for_signal_only)
		{
			if(pNode->m_CycleLengthInSecond >0)
				str_node_label.Format ("%d",pNode->m_CycleLengthInSecond  );
			else
				str_node_label.Format ("");

		}

		if(m_ShowNodeTextMode == node_display_offset_in_second_for_signal_only)
		{

			if(
				pNode->m_ControlType == pDoc->m_ControlType_PretimedSignal || 
				pNode->m_ControlType == pDoc->m_ControlType_ActuatedSignal)  // traffic signal control
				str_node_label.Format ("%d",pNode->m_SignalOffsetInSecond   );
			else
				str_node_label.Format ("");

		}



		if(m_ShowNodeTextMode == node_display_intersection_name)
		{
			if(pNode->m_Name .size() >0 && pNode->m_Name!="(null)")
				str_node_label.Format ("%s",pNode->m_Name.c_str()  );
			else
				str_node_label.Format ("");

		}

		if(m_ShowNodeTextMode == node_display_control_type)
		{

			if( pDoc->m_NodeTypeMap.find(pNode->m_ControlType)!= pDoc->m_NodeTypeMap.end())
			{
				str_node_label = pDoc->m_NodeTypeMap[pNode->m_ControlType].c_str() ;
			}

		}




		if(m_ShowNodeTextMode == node_display_travel_time_from_origin && pNode->m_DistanceToRoot > 0.00001 && pNode->m_DistanceToRoot < MAX_SPLABEL-1)  // check connectivity, overwrite with distance to the root
			str_node_label.Format ("%4.1f",pNode->m_DistanceToRoot );


		point.y -= tm.tmHeight / 2;

		pDC->TextOut(point.x , point.y,str_node_label);
	}

	if(pNode->m_DistanceToRoot > MAX_SPLABEL-1)  //restore pen
		pDC->SelectObject(&g_PenNodeColor);





}



void CTLiteView::OnNodeMovementproperties()
{
	CTLiteDoc* pDoc = GetDocument();

	if(pDoc == NULL)
		return;

	if(pDoc->m_ProjectDirectory.GetLength () ==0)
	{

		AfxMessageBox("Please first save and then reload the data set to show movement data.");
		return;
	}

	pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

	if(pDoc->m_SelectedNodeID >=0)
	{
		DTANode* pNode = pDoc->m_NodeNoMap [pDoc->m_SelectedNodeID ];

		if(pNode==NULL)
			return;

		//if(pNode->m_ControlType !=  pDoc->m_ControlType_PretimedSignal &&
		//		pNode->m_ControlType !=  pDoc->m_ControlType_ActuatedSignal )  //this movement vector is the same as the current node
		//	{
		//
		//		CString str;
		//		str.Format("The selected node %d does not have signal information.", pNode->m_NodeNumber );
		//		AfxMessageBox(str,MB_ICONINFORMATION);
		//		return;
		//	}
		//	

		//CWaitCursor wait;
		//pDoc->GenerateMovementCountFromVehicleFile();
		//pDoc->ExportQEMData(pNode->m_NodeNumber );

		CPropertySheet sheet("Node Movement Data");

		CPage_Node_Movement MovementPage;
		MovementPage.m_psp.pszTitle = _T("Movemnent");
		MovementPage.m_psp.dwFlags |= PSP_USETITLE;

		MovementPage.m_pDoc = pDoc;
		MovementPage.m_PeakHourFactor = pDoc->m_PeakHourFactor ;
		MovementPage.m_CurrentNodeNumber = pNode->m_NodeNumber;
		MovementPage.m_CurrentNode_Name = pNode->m_Name.c_str () ;
		MovementPage.m_Offset = pNode->m_SignalOffsetInSecond;

		if(pNode->m_ControlType == pDoc->m_ControlType_PretimedSignal
			|| pNode->m_ControlType == pDoc->m_ControlType_ActuatedSignal)
			MovementPage.m_bSigalizedNode  = true;
		else
			MovementPage.m_bSigalizedNode  = false;

		sheet.AddPage(&MovementPage);  // 0



		CPage_Node_Phase PhasePage;

		if(pNode->m_ControlType == pDoc->m_ControlType_PretimedSignal
			|| pNode->m_ControlType == pDoc->m_ControlType_ActuatedSignal)
		{
			PhasePage.m_psp.pszTitle = _T("Phase");
			PhasePage.m_psp.dwFlags |= PSP_USETITLE;
			PhasePage.m_pDoc = pDoc;
			PhasePage.m_PeakHourFactor = pDoc->m_PeakHourFactor ;
			PhasePage.m_CurrentNodeNumber = pNode->m_NodeNumber;
			PhasePage.m_CurrentNode_Name = pNode->m_Name.c_str () ;
			PhasePage.m_Offset = pNode->m_SignalOffsetInSecond;
			sheet.AddPage(&PhasePage);  // 0
		}

		sheet.SetActivePage (0);
		if(sheet.DoModal() == IDOK)
		{


		}

		//if ( 0 == m_msStatus )
		//{
		//	m_ms.m_pDoc = pDoc;
		//	m_ms.PrepareData4Editing();
		//	m_msStatus = 1;
		//}

		//CString str;
		//str.Format("Node %d",pDoc->m_NodeNotoNumberMap[pDoc->m_SelectedNodeID]);

		//CMyPropertySheet sheet(str);
		//sheet.SetTitle(str);
		//CPage_Node_Movement MovementPage;
		//MovementPage.m_pDoc = pDoc;
		//MovementPage.m_pView= this;
		//MovementPage.m_psp.dwFlags |= PSP_USETITLE;
		//MovementPage.m_psp.pszTitle = _T("Movement");
		//sheet.AddPage(&MovementPage);  // 0


		////if(pDoc->m_NodeNoMap.find(pDoc->m_SelectedNodeID) ==pDoc->m_NodeNoMap.end())
		////	return;

		////DTANode*  pNode = pDoc->m_NodeNoMap[pDoc->m_SelectedNodeID];

		//CPage_Node_Lane LanePage;
		//LanePage.m_pDoc = pDoc;
		//LanePage.m_pView= this;
		//LanePage.m_psp.dwFlags |= PSP_USETITLE;
		//LanePage.m_psp.pszTitle = _T("Lane");
		//sheet.AddPage(&LanePage);  // 2

		//CPage_Node_LaneTurn LaneTurnPage;
		//LaneTurnPage.m_pDoc = pDoc;
		//LaneTurnPage.m_pView= this;
		//LaneTurnPage.m_psp.dwFlags |= PSP_USETITLE;
		//LaneTurnPage.m_psp.pszTitle = _T("LaneTurn");
		//sheet.AddPage(&LaneTurnPage);  // 3



		//sheet.DoModal();
	}
}

void CTLiteView::OnLinkLinedisplaymode()
{
	if(m_bLineDisplayConditionalMode)  // user control now
		m_bLineDisplayConditionalMode = false;

	CTLiteDoc* pDoc = GetDocument();
	if(!m_bLineDisplayConditionalMode)
		pDoc->GenerateOffsetLinkBand();

	Invalidate();
}

void CTLiteView::OnUpdateLinkLinedisplaymode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bLineDisplayConditionalMode);
}



void CTLiteView::OnViewShowConnector()
{
	m_bShowConnector = ! m_bShowConnector;
	Invalidate();
}

void CTLiteView::OnUpdateViewShowConnector(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowConnector);
}


void CTLiteView::OnViewHighlightcentroidsandactivitylocations()
{
	m_bHighlightActivityLocation = !m_bHighlightActivityLocation;
	Invalidate();
}

void CTLiteView::OnUpdateViewHighlightcentroidsandactivitylocations(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bHighlightActivityLocation);
}

void CTLiteView::OnViewBackgroundcolor()
{
	CTLiteDoc* pDoc = GetDocument();

	CColorDialog dlg(theApp.m_BackgroundColor, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		theApp.m_BackgroundColor= dlg.GetColor();
		pDoc->UpdateAllViews(0);
	}
	Invalidate();
}

void CTLiteView::OnActivitylocationmodeNolanduseactivity()
{

	if(m_ShowNodeTextMode != node_display_zone_number)
	{
		m_ShowNodeTextMode = node_display_zone_number;
		AfxMessageBox("Node text display mode is now set to display the zone ID of activity location.", MB_ICONINFORMATION);
	}

	CTLiteDoc* pDoc = GetDocument();

	if(pDoc != NULL)
	{
		int ZoneID = FindClosestZone(m_CurrentMousePoint,0);

		pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

		if(pDoc->m_SelectedNodeID >=0 && ZoneID>0)
		{
			pDoc->m_ZoneMap [ZoneID].RemoveNodeActivityMode(pDoc->m_NodeNotoNumberMap [pDoc->m_SelectedNodeID]);

			// remove zone attributes
			pDoc->m_NodeNoMap [pDoc->m_SelectedNodeID]->m_ZoneID = 0;

		}
	}
	Invalidate();
}

void CTLiteView::OnUpdateActivitylocationmodeNolanduseactivity(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CTLiteView::OnActivitylocationmodeLanduseactivity()
{
	if(m_ShowNodeTextMode != node_display_zone_number)
	{
		m_ShowNodeTextMode = node_display_zone_number;
		AfxMessageBox("Node text display mode is now set to display the zone ID of activity location.", MB_ICONINFORMATION);
	}
	CTLiteDoc* pDoc = GetDocument();

	if(pDoc != NULL)
	{
		int ZoneID = FindClosestZone(m_CurrentMousePoint,0);

		pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

		if(pDoc->m_SelectedNodeID >=0 && ZoneID>0)
		{
			pDoc->m_ZoneMap [ZoneID].SetNodeActivityMode (pDoc->m_NodeNotoNumberMap [pDoc->m_SelectedNodeID],0);  // 0 for land use activity
			// set zone attributes
			pDoc->m_NodeNoMap [pDoc->m_SelectedNodeID]->m_ZoneID = ZoneID;
			pDoc->m_NodeNoMap [pDoc->m_SelectedNodeID]->m_External_OD_flag = 0;

			pDoc->m_SelectedNodeID = -1;

		}
	}
	m_bHighlightActivityLocation  = true;
	Invalidate();
}

void CTLiteView::OnUpdateActivitylocationmodeLanduseactivity(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CTLiteView::OnActivitylocationmodeExternalorigin()
{
	if(m_ShowNodeTextMode != node_display_zone_number)
	{
		m_ShowNodeTextMode = node_display_zone_number;
		AfxMessageBox("Node text display mode is now set to display the zone ID of activity location.", MB_ICONINFORMATION);
	}

	CTLiteDoc* pDoc = GetDocument();

	if(pDoc != NULL)
	{
		int ZoneID = FindClosestZone(m_CurrentMousePoint,0);

		pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

		if(pDoc->m_SelectedNodeID >=0 && ZoneID>0)
		{
			pDoc->m_ZoneMap [ZoneID].SetNodeActivityMode (pDoc->m_NodeNotoNumberMap [pDoc->m_SelectedNodeID],1);  // 1 for external origin
			// set zone attributes
			pDoc->m_NodeNoMap [pDoc->m_SelectedNodeID]->m_ZoneID = ZoneID;
			pDoc->m_NodeNoMap [pDoc->m_SelectedNodeID]->m_External_OD_flag = 1;
			pDoc->m_SelectedNodeID = -1;
		}
	}
	m_bHighlightActivityLocation  = true;
	Invalidate();
}

void CTLiteView::OnUpdateActivitylocationmodeExternalorigin(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CTLiteView::OnActivitylocationmodeExternaldestination()
{
	if(m_ShowNodeTextMode != node_display_zone_number)
	{
		m_ShowNodeTextMode = node_display_zone_number;
		AfxMessageBox("Node text display mode is now set to display the zone ID of activity location.", MB_ICONINFORMATION);
	}

	CTLiteDoc* pDoc = GetDocument();

	if(pDoc != NULL)
	{
		int ZoneID = FindClosestZone(m_CurrentMousePoint,0);

		pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

		if(pDoc->m_SelectedNodeID >=0 && ZoneID>0)
		{
			pDoc->m_ZoneMap [ZoneID].SetNodeActivityMode (pDoc->m_NodeNotoNumberMap [pDoc->m_SelectedNodeID],-1);  // -1 for external destination
			// set zone attributes
			pDoc->m_NodeNoMap [pDoc->m_SelectedNodeID]->m_ZoneID = ZoneID;
			pDoc->m_NodeNoMap [pDoc->m_SelectedNodeID]->m_External_OD_flag = -1;
			pDoc->m_SelectedNodeID = -1;

		}
	}
	m_bHighlightActivityLocation  = true;
	Invalidate();
}

void CTLiteView::OnUpdateActivitylocationmodeExternaldestination(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CTLiteView::OnMoeOddemand()
{
	m_bShowODDemandVolume = !m_bShowODDemandVolume;
	Invalidate();

}

void CTLiteView::OnUpdateMoeOddemand(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowODDemandVolume);
}

void CTLiteView::SetGlobalViewParameters()
{

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	if(pMainFrame->m_bSynchronizedDisplay)
	{
		std::list<CTLiteView*>::iterator iView = g_ViewList.begin ();
		while (iView != g_ViewList.end())
		{
			if((*iView) != this)  // not this window
			{
				(*iView)->m_Resolution  = m_Resolution;
				(*iView)->m_ScreenOrigin = m_ScreenOrigin;
				(*iView)->m_Origin  = m_Origin;

				CTLiteDoc * pDoc = (*iView)->GetDocument();
				pDoc->m_MaxLinkWidthAsLinkVolume = GetDocument()->m_MaxLinkWidthAsLinkVolume;
				(*iView)->Invalidate ();

			}
			iView++;
		}
	}
}
void CTLiteView::OnUpdateLinkIncreasebandwidth(CCmdUI *pCmdUI)
{
	CTLiteDoc* pDoc = GetDocument();
	pCmdUI->Enable ((pDoc->m_LinkMOEMode == MOE_volume || pDoc->m_LinkMOEMode == MOE_bottleneck ||pDoc->m_LinkMOEMode == MOE_impact || pDoc->m_LinkMOEMode == MOE_user_defined || pDoc->m_LinkMOEMode == MOE_speed || pDoc->m_LinkMOEMode == MOE_density || pDoc->m_LinkMOEMode == MOE_emissions|| pDoc->m_LinkMOEMode == MOE_none) && m_bLineDisplayConditionalMode  == false);

}

void CTLiteView::OnUpdateLinkDecreasebandwidth(CCmdUI *pCmdUI)
{
	CTLiteDoc* pDoc = GetDocument();

	pCmdUI->Enable ((pDoc->m_LinkMOEMode == MOE_volume || pDoc->m_LinkMOEMode == MOE_bottleneck ||pDoc->m_LinkMOEMode == MOE_impact || pDoc->m_LinkMOEMode == MOE_user_defined || pDoc->m_LinkMOEMode == MOE_speed || pDoc->m_LinkMOEMode == MOE_density || pDoc->m_LinkMOEMode == MOE_emissions|| pDoc->m_LinkMOEMode == MOE_none)&& m_bLineDisplayConditionalMode  == false);
}


void CTLiteView::CreateDefaultJunction()
{
	CTLiteDoc* pDoc = GetDocument();

	if(pDoc == NULL)
		return;

	if ( 0 == m_msStatus )
	{
		m_ms.m_pDoc = pDoc;
		m_ms.PrepareData4Editing();
		m_msStatus = 1;
		AfxMessageBox("Junctions are created with default information!",MB_OK);
	}
}


void CTLiteView::OnExportCreatevissimfiles()
{
	CTLiteDoc* pDoc = GetDocument();

	if(pDoc == NULL)
		return;

	if ( 0 == m_msStatus )
	{
		m_ms.m_pDoc = pDoc;
		m_ms.PrepareData4Editing();
		m_msStatus = 1;
		AfxMessageBox("Junctions are created with default information!",MB_OK);
	}
	if ( 1 == m_msStatus )
	{
		m_ms.Create2Files();
		AfxMessageBox("VISSIM files are created!",MB_OK);
	}
}


void CTLiteView::OnDebugShowvehicleswithincompletetripsonly()
{
	bShowVehiclesWithIncompleteTrips = !bShowVehiclesWithIncompleteTrips;
	Invalidate();
}

void CTLiteView::OnUpdateDebugShowvehicleswithincompletetripsonly(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(bShowVehiclesWithIncompleteTrips);
}

void CTLiteView::OnVehicleVehiclenumber()
{
	m_bShowVehicleNumber = !m_bShowVehicleNumber;
	Invalidate();

}

void CTLiteView::OnUpdateVehicleVehiclenumber(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowVehicleNumber);
}

void CTLiteView::OnVehicleShowselectedvehicleonly()
{
	m_bShowSelectedVehicleOnly = ! m_bShowSelectedVehicleOnly;
	Invalidate();

}

void CTLiteView::OnUpdateVehicleShowselectedvehicleonly(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowSelectedVehicleOnly);
}

void CTLiteView::OnNodeAddintermediatedestinationhere()
{
	CTLiteDoc* pDoc = GetDocument();

	int SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

	if(SelectedNodeID>=0)
	{

		pDoc->m_IntermediateDestinationVector.push_back(SelectedNodeID);

		pDoc->m_NodeNoMap[SelectedNodeID]->m_IntermediateDestinationNo = pDoc->m_IntermediateDestinationVector.size();

	}



	pDoc->Routing(false);


	m_ShowAllPaths = true;
	Invalidate();
}

void CTLiteView::OnNodeRemoveallintermediatedestination()
{
	CTLiteDoc* pDoc = GetDocument();

	pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

	pDoc->m_IntermediateDestinationVector.clear();
	std::list<DTANode*>::iterator iNode;

	for (iNode = pDoc->m_NodeSet.begin(); iNode != pDoc->m_NodeSet.end(); iNode++)
	{
		(*iNode)->m_IntermediateDestinationNo = 0;

	}

	std::list<DTALink*>::iterator iLink;

	for (iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
	{
		(*iLink)->m_AdditionalCost = 0;
	}

	pDoc->Routing(false);


	m_ShowAllPaths = true;
	Invalidate();
}

void CTLiteView::OnLinkAvoidusingthislinkinrouting()
{
	CTLiteDoc* pDoc = GetDocument();

	if(pDoc->m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink = pDoc->m_LinkNoMap [pDoc->m_SelectedLinkNo ];
	pLink -> m_AdditionalCost = 10000;
	pDoc->Routing(false,true);


	m_ShowAllPaths = true;
	Invalidate();

}


void CTLiteView::OnBnClickedButtonConfiguration()
{

	CDlg_DisplayConfiguration* m_pDlg = new CDlg_DisplayConfiguration;
	m_pDlg->pView = this;

	CTLiteDoc* pDoc = GetDocument();

	m_pDlg->m_bShowSignalNodeMovementOnly = 	pDoc->m_bShowSignalNodeMovementOnly ;

	m_pDlg->m_ShowNodeTextMode = this ->m_ShowNodeTextMode;
	m_pDlg->SetModelessFlag(true); // voila! this is all it takes to make your dlg modeless!
	m_pDlg->Create(IDD_DIALOG_DISPLAY_CONFIG); 
	m_pDlg->ShowWindow(SW_SHOW); 

}

void CTLiteView::OnNodeNodeproperties()
{
	CTLiteDoc* pDoc = GetDocument();

	if(pDoc == NULL)
		return;

	pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

	if(pDoc->m_SelectedNodeID >=0)
	{
		DTANode* pNode = pDoc->m_NodeNoMap [pDoc->m_SelectedNodeID ];

		if(pNode==NULL)
			return;

		CDlg_NodeProperties dlg;

		dlg.m_pDoc = pDoc;

		dlg.NodeID  = pNode->m_NodeNumber  ;
		dlg.NodeName   = pNode->m_Name .c_str ();
		dlg.ZoneID  = pNode->m_ZoneID  ;
		dlg.ControlType = pNode->m_ControlType ;
		dlg.m_CycleLength = pNode->m_CycleLengthInSecond;

		if(dlg.DoModal() == IDOK)
		{

			CT2CA pszConvertedAnsiString (dlg.NodeName);
			// construct a std::string using the LPCSTR input
			std::string strStd (pszConvertedAnsiString);

			if ( pNode ->m_ControlType != dlg.ControlType 
				|| pNode->m_Name.compare (strStd)!=0
				|| pNode->m_ZoneID != dlg.ZoneID
				)
			{
				pDoc->Modify();
				pDoc->PushBackNetworkState();
				pNode->m_Name  = strStd;
				pNode->m_ZoneID = dlg.ZoneID;
				pNode->m_CycleLengthInSecond = dlg.m_CycleLength;
				pNode ->m_ControlType = dlg.ControlType;

			}

			// if node id is changed by the users
			
			if (dlg.NodeID != pNode->m_NodeNumber)
			{
				if (pDoc->m_NodeNumberMap.find(dlg.NodeID) != pDoc->m_NodeNumberMap.end())
				{
					CString msg;
					msg.Format("Node ID: %d that you just input has been used. Please select a new node ID.", dlg.NodeID);
					AfxMessageBox(msg);
				}
				else
				{
					pDoc->Modify();
					pDoc->PushBackNetworkState();


					int old_node_id = pNode->m_NodeNumber;
					int new_node_id = dlg.NodeID;
					
					pNode->m_NodeNumber = dlg.NodeID;
					
					//search related links,

					DTANode* pNode = pDoc->m_NodeNumberMap[old_node_id];

					for (int i = 0; i < pNode->m_OutgoingLinkVector.size(); i++)
					{
						DTALink* pLink = pDoc->m_LinkNoMap[pNode->m_OutgoingLinkVector[i]];
						pLink->m_FromNodeNumber = new_node_id;
					}
					for (int i = 0; i < pNode->m_IncomingLinkVector.size(); i++)
					{
						DTALink* pLink = pDoc->m_LinkNoMap[pNode->m_IncomingLinkVector[i]];
						pLink->m_ToNodeNumber = new_node_id;

					}

					// search activity locations for associated zone id
				
					if (pNode->m_ZoneID >= 1)  // with associated zone
					{
					
					pDoc->m_ZoneMap[pNode->m_ZoneID].RemoveNodeActivityMode(old_node_id);

					DTAActivityLocation element;
					element.ZoneID = pNode->m_ZoneID;
					element.NodeNumber = new_node_id;

					pDoc->m_ZoneMap[pNode->m_ZoneID].m_ActivityLocationVector.push_back(element);
				
					}
				
				}

			}
			


			// dlg.ZoneID // handing here
		}
		Invalidate();
	}
}

void CTLiteView::DrawMovementLink(CDC* pDC,GDPoint pt_from, GDPoint pt_to,int NumberOfLanes, double theta, int lane_width)
{

}

void CTLiteView::DrawNodeMovements(CDC* pDC, DTANode* pNode, CRect PlotRect)
{
	CPen NormalPen(PS_SOLID,2,RGB(0,0,0));
	CPen TimePen(PS_DOT,1,RGB(0,0,0));
	CPen DataPen(PS_SOLID,0,RGB(0,0,0));
	CPen SelectedPen(PS_SOLID,4,RGB(255,0,0));
	CPen SelectedPhasePen(PS_SOLID,4,RGB(0,0,255));

	CBrush  WhiteBrush(RGB(255,255,255)); 

	pDC->SetBkMode(TRANSPARENT);

	pDC->SelectObject(&DataPen);
	pDC->SelectObject(&WhiteBrush);

	CBrush  BrushLinkBand(RGB(152,245,255)); 
	pDC->SelectObject(&BrushLinkBand);

	CTLiteDoc*  pDoc = GetDocument();

	double size_ratio= pDoc->m_MovementTextBoxSizeInFeet/250;

	double node_set_back = size_ratio*150 * pDoc->m_UnitFeet;

	double link_length = size_ratio*250 * pDoc->m_UnitFeet;
	double movement_length = size_ratio*100 * pDoc->m_UnitFeet;
	double lane_width = size_ratio*10 * pDoc->m_UnitFeet;


	double text_length =  pDoc->m_MovementTextBoxSizeInFeet * 0.15 *  pDoc->m_UnitFeet ;

	std::map<CString, double> Turn_Degree_map;

	std::string timing_plan_name = "0";

	for (unsigned int i = 0; i< pNode->m_MovementDataMap[timing_plan_name].m_MovementVector.size(); i++)
	{
		DTATimingPlan timing_plan = GetDocument()->GetTimingPlanInfo(timing_plan_name);
		DTANodeMovement movement = pNode->m_MovementDataMap[timing_plan_name].m_MovementVector[i];

		if( pDoc->m_hide_non_specified_movement_on_freeway_and_ramp && movement.bNonspecifiedTurnDirectionOnFreewayAndRamps )
			continue;

		if(pDoc->m_bShowSignalNodeMovementOnly == true && (
			(pNode->m_ControlType != pDoc->m_ControlType_PretimedSignal && 
			pNode->m_ControlType != pDoc->m_ControlType_ActuatedSignal))

			)
			continue;


		if(pDoc->m_LinkNoMap.find(movement.IncomingLinkNo) ==  pDoc->m_LinkNoMap.end())
			return;

		if(pDoc->m_LinkNoMap.find(movement.OutgoingLinkNo) ==  pDoc->m_LinkNoMap.end())
			return;

		DTALink* pInLink  = pDoc->m_LinkNoMap [movement.IncomingLinkNo];
		DTALink* pOutLink  = pDoc->m_LinkNoMap [movement.OutgoingLinkNo ];

		int destination_node  = pOutLink->m_ToNodeNumber; 

		if(movement.turning_prohibition_flag ==1 && m_bShowProhibitedMovements)
		{
			// select thick red pen: 
			pDC->SelectObject(&g_PenProhibitedMovement);

		}

		if(m_bShowProhibitedMovements && movement.turning_prohibition_flag == 0)
		{
			continue;// no drawing

		}


		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

		if( !pMainFrame->m_bShowLayerMap[layer_connector] &&  pDoc->m_LinkTypeMap[pInLink->m_link_type  ].IsConnector ())
			continue;

		//if( pDoc->m_LinkTypeMap[pInLink->m_link_type  ].IsRamp  ())  //always one way
		//	continue;

		GDPoint p1, p2, p3, p_text;
		// 1: fetch all data
		p1  = pDoc->m_NodeNoMap[movement.in_link_from_node_id ]->pt;
		p2  = pDoc->m_NodeNoMap[movement.in_link_to_node_id ]->pt;
		p3  = pDoc->m_NodeNoMap[movement.out_link_to_node_id]->pt;

		// reserved direction
		double DeltaX = p2.x - p1.x ;
		double DeltaY = p2.y - p1.y ;
		double theta = atan2(DeltaY, DeltaX);

		double movement_approach_turnection_theta = atan2(p3.y - p1.y, p3.x - p1.x);


		GDPoint p1_new, p2_new, p3_new;
		GDPoint p1_text, p3_text;
		GDPoint pt_movement[3];


		// 2. set new origin
		p2_new.x = pNode->pt.x +(-1)*node_set_back*cos(theta);  
		p2_new.y = pNode->pt.y + (-1)*node_set_back*sin(theta);

		p1_new.x = pNode->pt.x +(-1)*link_length*cos(theta);
		p1_new.y = pNode->pt.y + (-1)*link_length*sin(theta);

		p_text.x = p1_new.x - text_length*cos(theta);
		p_text.y = p1_new.y - text_length*sin(theta);


		// 3 determine the control point for  PolyBezier
		double lane_width =  size_ratio*3*pDoc->m_LaneWidthInFeet * pDoc->m_UnitFeet;
		double movement_offset = 0;
		double text_offset = 0.7*lane_width;
		int mid_lane_number = 4+ pInLink->m_NumberOfLanes / 2;
		float control_point_ratio = 0;

		float MovementTextFontSize = 8;

		CString movement_approach_turnection_label;
		movement_approach_turnection_label.Format ("%d,%s", movement.in_link_from_node_id , pDoc->GetTurnString(movement.movement_turn));

		if(movement.movement_turn == DTA_Through ) 
		{

			if(Turn_Degree_map.find (movement_approach_turnection_label ) != Turn_Degree_map.end())
			{  // this direction has been used/defined.

				if( Turn_Degree_map[movement_approach_turnection_label ] <  movement_approach_turnection_theta)
					movement_offset = lane_width * (mid_lane_number - 0.7);
				else
					movement_offset = lane_width * (mid_lane_number + 0.7);

			}else
			{
				movement_offset = lane_width * mid_lane_number;

			}

		}else if(movement.movement_turn == DTA_LeftTurn || movement.movement_turn == DTA_LeftTurn2 ) 
		{
			movement_offset = lane_width * (mid_lane_number-1);
		}else
		{
			movement_offset = lane_width * (mid_lane_number+1);
		}

		// keep a record
		Turn_Degree_map[movement_approach_turnection_label] = movement_approach_turnection_theta;

		int movement_size = max(2,int(text_length*m_Resolution));

		movement_size = min(50,movement_size);

		int PointSize =  max(movement_size, 10)*m_MovmentTextSize;

		GDPoint pt_from, pt_to, pt_text;
		pt_from.x = p1_new.x + movement_offset* cos(theta-PI/2.0f);
		pt_from.y = p1_new.y + movement_offset* sin(theta-PI/2.0f);


		double alpha  = 0.3;

		pt_to.x  = p2_new.x + movement_offset* cos(theta-PI/2.0f);
		pt_to.y  =  p2_new.y + movement_offset* sin(theta-PI/2.0f);

		// direction

		DeltaX = p2.x - p3.x ;
		DeltaY = p2.y - p3.y ;
		theta = atan2(DeltaY, DeltaX);


		if(movement.movement_turn == DTA_Through ) 
		{
			p3_new.x = pt_to.x - movement_length * cos(theta);
			p3_new.y = pt_to.y - movement_length * sin(theta);

		}else if(movement.movement_turn == DTA_LeftTurn ) 
		{
			p3_new.x = pt_to.x - movement_length * cos(theta -PI/4.0f);
			p3_new.y = pt_to.y - movement_length * sin(theta -PI/4.0f);
		}else
		{
			p3_new.x = pt_to.x - movement_length * cos(theta +PI/4.0f);
			p3_new.y = pt_to.y - movement_length * sin(theta +PI/4.0f);
		}

		CPoint DrawPoint[3];
		DrawPoint[0] = NPtoSP(pt_from);
		DrawPoint[1] = NPtoSP(pt_to);

		pDC->MoveTo(DrawPoint[0]);
		pDC->LineTo(DrawPoint[1]);
		DrawPoint[2] = NPtoSP(p3_new);


		pDC->LineTo(DrawPoint[2]);

		//restore pen
		pDC->SelectObject(&DataPen);


		//text 

		// create rotation font
		CFont movement_text_font;



		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
		lf.lfHeight = PointSize;

		DeltaX = p2.x - p1.x ;
		DeltaY = p2.y - p1.y ;
		theta = atan2(DeltaY, DeltaX);

		float theta_deg = fmod(theta/PI*180,360);

		if(theta_deg < -90)
			theta_deg += 180;

		if(theta_deg > 90)
			theta_deg -= 180;

		lf.lfEscapement = theta_deg*10;


		strcpy(lf.lfFaceName, "Arial");       

		movement_text_font.CreateFontIndirect(&lf);

		CFont* oldFont = pDC->SelectObject(&movement_text_font);

		pDC->SelectObject(&movement_text_font);

		// select text string to be displayed

		CString str_text;


		float number_of_hours = max(0.01, (pDoc->m_DemandLoadingEndTimeInMin - pDoc->m_DemandLoadingStartTimeInMin) / 60.0);
		float sim_turn_hourly_count = movement.sim_turn_count / number_of_hours;

		//empty label
		str_text.Format("");

		switch (m_ShowMovementTextMode)
		{
		case movement_display_turn_up_node_number: str_text.Format("%d", pInLink->m_FromNodeNumber); break;
		case movement_display_turn_dest_node_number: str_text.Format("%d", pOutLink->m_ToNodeNumber); break;
		case movement_display_turn_three_node_numbers: str_text.Format("%d,%d,%d", pInLink->m_FromNodeNumber,pInLink->m_ToNodeNumber,pOutLink->m_ToNodeNumber); break;

		case movement_display_turn_type: str_text.Format("%s", pDoc->GetTurnString(movement.movement_turn)); break;
		case movement_display_turn_direction: str_text.Format("%s", pDoc->GetTurnDirectionString(movement.movement_approach_turn)); break;
		case movement_display_turn_protected_permited_prohibited: str_text.Format("%d", movement.turning_prohibition_flag); break;


		case movement_display_sim_turn_count: 
			//					if(movement.sim_turn_count>=1)
			str_text.Format("%.0f", movement.sim_turn_count );

			break;

		case movement_display_sim_turn_hourly_count: 
			str_text.Format("%.0f", sim_turn_hourly_count ) ; 

			break;

		case movement_display_sim_turn_percentage: 

			//					if(movement.sim_turn_count>=1)
			str_text.Format("%.1f%%", movement.turning_percentage  ); 

			break;

		case movement_display_sim_turn_delay: 

			//					if(movement.sim_turn_count>=1)
			str_text.Format("%.1f", movement.sim_turn_delay*60  ); // min to second

			break;


		case movement_display_sim_turn_delay_in_min: 

			str_text.Format("%.1f", movement.sim_turn_delay  ); 

			break;



		case movement_display_obs_turn_hourly_count: 
			//					if(movement.sim_turn_count>=1)
			str_text.Format("%d", movement.obs_turn_hourly_count); 

			break;

		case movement_display_obs_turn_percentage: 

			if(movement.obs_turn_hourly_count>=1 && pInLink->m_observed_peak_hourly_volume >=1)
				str_text.Format("%.1f%%", movement.obs_turn_hourly_count*100.0/ max (1, pInLink->m_observed_peak_hourly_volume)); 

			break;

		case movement_display_obs_turn_delay: 

			//					if(movement.sim_turn_count>=1)
			str_text.Format("%d", movement.obs_turn_delay  ); 

			break;

		case movement_display_QEM_Lanes: str_text.Format("%d",movement.QEM_Lanes ); break;
		case movement_display_QEM_Shared: str_text.Format("%d",movement.QEM_Shared ); break;
		case movement_display_QEM_Width:  str_text.Format("%d",movement.QEM_Width ); break;
		case movement_display_QEM_Storage: str_text.Format("%d",movement.QEM_Storage ); break;
		case movement_display_QEM_StLanes: str_text.Format("%d",movement.QEM_StLanes ); break;
		case movement_display_QEM_Grade:  str_text.Format("%.1f",movement.QEM_Grade ); break;
		case movement_display_QEM_Speed: str_text.Format("%.1f mph",movement.QEM_Speed ); break;
		case movement_display_QEM_IdealFlow: str_text.Format("%.1f",movement.QEM_IdealFlow ); break;
		case movement_display_QEM_LostTime: str_text.Format("%.1f",movement.QEM_LostTime ); break;
		case movement_display_QEM_Phase1: str_text.Format("%d",movement.QEM_Phase1 ); break;
		case movement_display_QEM_PermPhase1: str_text.Format("%d",movement.QEM_PermPhase1 ); break;
		case movement_display_QEM_DetectPhase1: str_text.Format("%d",movement.QEM_DetectPhase1 ); break;

		case movement_display_QEM_TurnVolume: str_text.Format("%d",movement.QEM_TurnVolume  ); break;
		case movement_display_QEM_TurnPercentage: str_text.Format("%.0f%%",movement.QEM_TurnPercentage  ); break;

		case movement_display_QEM_EffectiveGreen: str_text.Format("%.0f",movement.QEM_EffectiveGreen ); break;
		case movement_display_QEM_Capacity: str_text.Format("%.0f",movement.QEM_Capacity ); break;
		case movement_display_QEM_VOC: str_text.Format("%.2f",movement.QEM_VOC ); break;
		case movement_display_QEM_SatFlow: str_text.Format("%.0f",movement.QEM_SatFlow ); break;
		case movement_display_QEM_Delay: str_text.Format("%.1f s",movement.QEM_Delay ); break;
		case movement_display_QEM_LOS: str_text.Format("%s",movement.QEM_LOS ); break;


		default: str_text.Format("");
		}

		CPoint TextPoint;

		double alpah = 1.5;


		TextPoint.x = alpah * DrawPoint[0].x + (1-alpah)* DrawPoint[1].x  ;
		TextPoint.y =  alpah * DrawPoint[0].y + (1-alpah)* DrawPoint[1].y ;
		CRect rt(TextPoint.x -1,TextPoint.y -1, TextPoint.x +1, TextPoint.y+1);

		pDC->SetBkColor(RGB(0,0, 0));

		pDC->SetTextColor(RGB(0,1,0));
		pDC->TextOut(TextPoint.x,TextPoint.y - PointSize*0.5, str_text);	


		pDC->SelectObject(oldFont);



	}


}

void CTLiteView::OnNodeAvoidthisnode()
{
	CTLiteDoc* pDoc = GetDocument();

	pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

	std::list<DTALink*>::iterator iLink;

	for (iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
	{
		if((*iLink)->m_ToNodeID == pDoc->m_SelectedNodeID)
		{
			(*iLink)->m_AdditionalCost = 10000;
		}
	}

	pDoc->Routing(false, true);

	m_ShowAllPaths = true;
	Invalidate();
}

void CTLiteView::OnNodeRemovenodeavoidanceconstraint()
{
	CTLiteDoc* pDoc = GetDocument();

	pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

	std::list<DTALink*>::iterator iLink;

	for (iLink = pDoc->m_LinkSet.begin(); iLink != pDoc->m_LinkSet.end(); iLink++)
	{
		(*iLink)->m_AdditionalCost = 0;
	}

	pDoc->Routing(false, true);

	m_ShowAllPaths = true;
	Invalidate();

}

void CTLiteView::OnEditMovenode()
{
	m_ToolMode = move_node_tool;

	
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	pMainFrame->m_iSelectedLayer = layer_node;

	CListCtrl * pGISLayerList = (CListCtrl *)(pMainFrame->m_GISLayerBar).GetDlgItem(IDC_LIST_GISLAYER);
	pGISLayerList->Invalidate(1);  // update display of selected layer

	m_bMouseDownFlag = false;
}

void CTLiteView::OnUpdateEditMovenode(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == move_node_tool ? 1 : 0);
}



void CTLiteView::OnBackgroundimageMarklongA()
{
	CTLiteDoc* pDoc = GetDocument();

	GDPoint pt = SPtoNP(m_CurrentMousePoint);

	pDoc->m_PointA_x = pt.x;
	pDoc->m_PointA_y = pt.y;


	pDoc->m_bPointA_Initialized = true;

	Invalidate();
}

void CTLiteView::OnBackgroundimageMarklongB()
{

	CTLiteDoc* pDoc = GetDocument();
	GDPoint pt = SPtoNP(m_CurrentMousePoint);

	pDoc->m_PointB_x = pt.x;
	pDoc->m_PointB_y = pt.y;

	pDoc->m_bPointB_Initialized = true;

	Invalidate();

}

void CTLiteView::OnBackgroundimageAddlat()
{
	CTLiteDoc* pDoc = GetDocument();

	CDlg_BackgroundImageLocation dlg;

	if(pDoc->m_bPointB_Initialized)
	{
		dlg.m_bPointA_Initialized = true;
		dlg.m_PointB_x = pDoc->m_PointB_x;
		dlg.m_PointB_y = pDoc->m_PointB_y;
	}

	if(pDoc->m_bPointA_Initialized)
	{
		dlg.m_bPointB_Initialized = true;
		dlg.m_PointA_x = pDoc->m_PointA_x;
		dlg.m_PointA_y = pDoc->m_PointA_y;
	}

	if(dlg.DoModal ()==IDOK)
	{
		pDoc->m_bPointB_Initialized  = false;
		pDoc->m_bPointA_Initialized = false;

		pDoc->m_PointA_long = dlg.m_PointA_long ;
		pDoc->m_PointB_long = dlg.m_PointB_long ;

		pDoc->m_PointA_lat = dlg.m_PointA_lat ;
		pDoc->m_PointB_lat = dlg.m_PointB_lat ;


		pDoc->ResetBackgroundImageCoordinate();

		Invalidate();
	}
}

void CTLiteView::OnZoneHighlightassociatedacititylocations()
{
	m_bHighlightActivityLocation = !m_bHighlightActivityLocation;
	Invalidate();

}

void CTLiteView::OnUpdateZoneHighlightassociatedacititylocations(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bHighlightActivityLocation);
}

void CTLiteView::OnZoneCreatezone()
{
	OnEditCreatesubarea();
	Invalidate();

}

void CTLiteView::OnEditCreatezone()
{
	m_ToolMode = add_zone_tool;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	pMainFrame->m_bShowLayerMap[layer_zone] = true;
	pMainFrame-> m_iSelectedLayer = layer_zone;
	GetDocument()->m_SubareaShapePoints.clear();

	CListCtrl * pGISLayerList = (CListCtrl *)(pMainFrame->m_GISLayerBar).GetDlgItem(IDC_LIST_GISLAYER);
	pGISLayerList->Invalidate(1);  // update display of selected layer
}

void CTLiteView::OnUpdateEditCreatezone(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == add_zone_tool ? 1 : 0);
	Invalidate();

}

void CTLiteView::OnSubareaHighlightlinksinsidesubarea()
{
	m_bHighlightSubareaLinks = !m_bHighlightSubareaLinks;

	if(m_bHighlightSubareaLinks)
		m_bHighlightSubareaBoundaryLinks = false;

	Invalidate();
}

void CTLiteView::OnUpdateSubareaHighlightlinksinsidesubarea(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bHighlightSubareaLinks);
}

void CTLiteView::OnSubareaHighlightlinksacosssubarea()
{
	m_bHighlightSubareaBoundaryLinks = !m_bHighlightSubareaBoundaryLinks;

	if(m_bHighlightSubareaBoundaryLinks)
		m_bHighlightSubareaLinks = false;

	Invalidate();
}

void CTLiteView::OnUpdateSubareaHighlightlinksacosssubarea(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bHighlightSubareaBoundaryLinks);
}

void CTLiteView::OnOdmatrixViewtop50odpairsonly()
{
	m_bShowTop10ODOnly = !m_bShowTop10ODOnly;
	Invalidate();

}

void CTLiteView::OnUpdateOdmatrixViewtop50odpairsonly(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowTop10ODOnly);
}

void CTLiteView::OnTransitShowtransitaccessibility()
{
	m_bShowTransitAccessibility = !m_bShowTransitAccessibility;
	Invalidate();
}

void CTLiteView::OnUpdateTransitShowtransitaccessibility(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowTransitAccessibility);
}

void CTLiteView::OnTransitCalculatetransitaccesssibilityfromhere()
{
	CTLiteDoc* pDoc = GetDocument();

	pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

	pDoc->m_OriginNodeID = pDoc->m_SelectedNodeID;

	GDPoint pt = SPtoNP(m_CurrentMousePoint);

	if( g_Simulation_Time_Stamp < 300 || g_Simulation_Time_Stamp >= 22*60)
	{  // reset to morning 7AM
		g_Simulation_Time_Stamp = 420;

	}
	pDoc->FindAccessibleTripID(pt,g_Simulation_Time_Stamp);

	m_bShowTransitAccessibility = true;

	Invalidate();

}

void CTLiteView::OnTransitOutputtransitaccesssibilityfromhere()
{
	CTLiteDoc* pDoc = GetDocument();

	pDoc->m_SelectedNodeID = FindClosestNode(m_CurrentMousePoint, 300);  // 300 is screen unit

	pDoc->m_OriginNodeID = pDoc->m_SelectedNodeID;

	GDPoint pt = SPtoNP(m_CurrentMousePoint);

	if( g_Simulation_Time_Stamp < 300 || g_Simulation_Time_Stamp >= 22*60)
	{  // reset to morning 7AM
		g_Simulation_Time_Stamp = 420;

	}
	pDoc->PrintOutAccessibleMap(pt, false);


}

void CTLiteView::OnMovementHighlightprohibitedmovements()
{
	m_bShowProhibitedMovements = !m_bShowProhibitedMovements;
	Invalidate();
}

void CTLiteView::OnUpdateMovementHighlightprohibitedmovements(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowProhibitedMovements);
}

void CTLiteView::OnTransitShowtransitlinksonly()
{
	m_bShowTransitLinksOnly = !m_bShowTransitLinksOnly;
	Invalidate();

}

void CTLiteView::OnTransitShowwalklinksonly()
{
	m_bShowWalkLinksOnly = !m_bShowWalkLinksOnly;
	Invalidate();

}

void CTLiteView::OnUpdateTransitShowtransitlinksonly(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowTransitLinksOnly);
}

void CTLiteView::OnUpdateTransitShowwalklinksonly(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowWalkLinksOnly);
}

void CTLiteView::OnFilePrint()
{
	// TODO: Add your command handler code here
}

void CTLiteView::OnFilePrintPreview()
{
	// TODO: Add your command handler code here
}

void CTLiteView::OnFilePrintSetup()
{
	// TODO: Add your command handler code here
}

void CTLiteView::OnVehicledataShowcompletetrajectory()
{
		m_bShowCompleteTrajectory = !m_bShowCompleteTrajectory;
		Invalidate();

}

void CTLiteView::OnUpdateVehicledataShowcompletetrajectory(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowCompleteTrajectory);
}

void CTLiteView::OnVehicledataShow()
{
	m_bShowAllCompleteTrajectory = !m_bShowAllCompleteTrajectory;
}

void CTLiteView::OnUpdateVehicledataShow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowAllCompleteTrajectory);
}


void CTLiteView::OnDetectorUnlock()
{
	if (m_ToolMode != sensor_location_move_tool)
		m_ToolMode = sensor_location_move_tool;
	else
		m_ToolMode = move_tool;

}


void CTLiteView::OnUpdateDetectorUnlock(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == sensor_location_move_tool ? 1 : 0);

}


void CTLiteView::OnDetectorShowsensordescription33769()
{
	m_bShowSensorDecription = !m_bShowSensorDecription;
	Invalidate();
}


void CTLiteView::OnUpdateDetectorShowsensordescription33769(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowSensorDecription == true ? 1 : 0);
}


void CTLiteView::OnDetectorShowmatchedlinkforsensor()
{
	m_bShowSensorMatchedLink = !m_bShowSensorMatchedLink;
	Invalidate();

}


void CTLiteView::OnUpdateDetectorShowmatchedlinkforsensor(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowSensorMatchedLink == true ? 1 : 0);
}


void CTLiteView::OnGridUnlock()
{


	if (m_ToolMode != network_coordinate_tool)
		m_ToolMode = network_coordinate_tool;
	else
		m_ToolMode = move_tool;

	m_bShowGrid = true;
	
	if (m_bNetworkCooridinateHints == false)
	{
		AfxMessageBox("Hints:\n1. Please use mouse to move the reference network and shift its coordinates;\n2. Use mouse wheel to adjust the size of the reference network (zoom);\n3. Click shift or control key together to change the horizonal or vertical size, respectively;\n4. Click on the other toolbar button to turn on disable this moving-reference-network mode.", MB_ICONINFORMATION);
		m_bNetworkCooridinateHints = false;
	}

	Invalidate();

}


void CTLiteView::OnUpdateGridUnlock(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == network_coordinate_tool ? 1 : 0);
}


void CTLiteView::OnLinkShowhidereferencenetwork()
{
	m_bShowReferenceNetwork = !m_bShowReferenceNetwork;
}


void CTLiteView::OnUpdateLinkShowhidereferencenetwork(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowReferenceNetwork ? 1 : 0);
}


void CTLiteView::OnEditMovereferencenetworkcoordinates()
{
	if (m_ToolMode != network_coordinate_tool)
	{
		m_MoveLayerNo = 1; //reference layer
		m_bShowGrid = true;
		if (m_bNetworkCooridinateHints == false)
		{
			AfxMessageBox("Hints:\n1. Please use mouse to move the reference network and shift its coordinates;\n2. Use mouse wheel to adjust the size of the reference network (zoom);\n3. Click shift or control key together to change the horizonal or vertical size, respectively;\n4. Click on the other toolbar button to turn on disable this moving-reference-network mode.", MB_ICONINFORMATION);
			m_bNetworkCooridinateHints = true;
		}
		m_ToolMode = network_coordinate_tool;
	}
	else
		m_ToolMode = move_tool;

	Invalidate();
}


void CTLiteView::OnUpdateEditMovereferencenetworkcoordinates(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ToolMode == network_coordinate_tool ? 1 : 0);
}

void CTLiteView::OnDetectorShowfixedcountdetectorlocation()
{
	m_bShowFixedDetectorLocation = !m_bShowFixedDetectorLocation;
	Invalidate();

}


void CTLiteView::OnDetectorShowspeedsensorlocation()
{
	m_bShowSpeedSensorLocation = !m_bShowSpeedSensorLocation;
	Invalidate();
}


void CTLiteView::OnUpdateDetectorShowfixedcountdetectorlocation(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowFixedDetectorLocation ? 1 : 0);
}


void CTLiteView::OnUpdateDetectorShowspeedsensorlocation(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowSpeedSensorLocation ? 1 : 0);
}




void CTLiteView::OnShowallowedlinksfordemandtypeDemandtype1()
{
	m_DislayedDemandType = 1;
	Invalidate();
}



void CTLiteView::OnShowallowedlinksfordemandtypeDemandtype2()
{
	m_DislayedDemandType = 2;
	Invalidate();
}


void CTLiteView::OnShowallowedlinksfordemandtypeDemandtype3()
{
	m_DislayedDemandType = 3;
	Invalidate();
}


void CTLiteView::OnShowallowedlinksfordemandtypeDemandtype4()
{
	m_DislayedDemandType = 4;
	Invalidate();
}


void CTLiteView::OnUpdateShowallowedlinksfordemandtypeDemandtype1(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_DislayedDemandType ==1);
}


void CTLiteView::OnUpdateShowallowedlinksfordemandtypeDemandtype2(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_DislayedDemandType == 2);
}


void CTLiteView::OnUpdateShowallowedlinksfordemandtypeDemandtype3(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_DislayedDemandType == 3);
}


void CTLiteView::OnUpdateShowallowedlinksfordemandtypeDemandtype4(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_DislayedDemandType == 4);
}


void CTLiteView::OnShowallowedlinksfordemandtypeShowall()
{
	m_DislayedDemandType = 0;
	Invalidate();
}


void CTLiteView::OnUpdateShowallowedlinksfordemandtypeShowall(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_DislayedDemandType == 0);
}


void CTLiteView::OnReferencelineShowreferencelineno()
{
	m_DislayReferenceLineNo = true;
	Invalidate();

}


void CTLiteView::OnBnClickedButtonSearch()
{
	OnSearchFindlink();
}
