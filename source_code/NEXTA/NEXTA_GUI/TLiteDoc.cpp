//TLiteDoc.cpp : implementation of the CTLiteDoc class
//

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

#include <atlbase.h>
#include <atlconv.h>
#include <map>
#include "stdafx.h"
#include "Geometry.h"
#include "CSVParser.h"
#include "RecordSetExt.h"
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "TLiteView.h"
#include "DlgMOE.h"
#include "DlgPathMOE.h"
#include "DlgTrainInfo.h"
#include "DlgFileLoading.h"
#include "MainFrm.h"
#include "Shellapi.h"
#include "DlgDefaultLinkProperties.h"
#include "DlgAssignmentSettings.h"
#include "CGridListCtrlEx\\CGridListCtrlEx.h"
#include "Dlg_ImportGISDataSet.h"
#include "Zip_utility\\zip.h"
#include "Zip_utility\\unzip.h"
#include <stdlib.h> 

#include "DlgLinkList.h"
#include "DlgPathList.h"
#include "Dlg_UnitTestingList.h"
#include "DlgGridCtrl.h"
#include "Dlg_ImageSettings.h"
#include "Shellapi.h"
#include "DlgSensorDataLoading.h"
#include "Dlg_ImportODDemand.h"
#include "DlgNetworkAlignment.h"
#include "Dlg_VehEmissions.h"
#include "DlgScenario.h"
#include "DlgMOETabView.h"
#include "Dlg_ImportShapeFiles.h"
#include "Dlg_ImportPlanningModel.h"
#include "Dlg_ImportNetwork.h"
#include "Dlg_ImportPointSensor.h"
#include "Dlg_SignalDataExchange.h"

#include "Dlg_PricingConfiguration.h"
#include "Dlg_LinkVisualizationConfig.h"

#include "Data-Interface\\XLEzAutomation.h"
#include "Data-Interface\\XLTestDataSource.h"
#include "Dlg_VehicleClassification.h"
#include "Dlg_Find_Vehicle.h"
#include "Dlg_TravelTimeReliability.h"
#include "Dlg_GISDataExchange.h"
#include "Dlg_GISDataExport.h"

#include "Dlg_Legend.h"

#include "LinePlot\\LinePlotTest.h"
#include "LinePlot\\LinePlotTestDlg.h"
#include "Page_Node_Movement.h"

#include "AssignmentSimulationSettingDlg.h"
#include "NetworkDataDlg.h"
#include "SensorNetworkDataDlg.h"
#include "CorridorDataDlg.h"
#include "GIS_Import_DataDlg.h"
#include "Dlg_RealWorldWidth.h"
#include "Dlg_UserInput.h"

#include "Dlg_Information.h"
#include "Dlg_TDMOE_Configuration.h"

#include "Dlg_KML_Configuration.h"
#include "Dlg_KML_Configuration_Zone.h"
#include "Dlg_LeftTurnLaneSetup.h"
#include "Dlg_ImportNetwork.h"
#include "Dlg_GIS_Setting_Config.h"
#include "Dlg_DataImportWizard.h"
#include "Dlg_NEXTA_Configuration.h"
#include "DlgScenario.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define _max_number_of_movements  32

extern std::vector<float> g_link_volume_vector; 
extern std::vector<float> g_link_speed_vector;
extern int AgentLite_main(float Alpha);
extern std::string g_AgentLiteProjectDirectory;

extern void g_SetCursor(_cursor_type cursor_type);
extern void g_ProgramStop_agentlite();


extern int g_ParserIntSequence(std::string str, std::vector<int> &vect); 
extern int g_ParserFloatSequence(std::string str, std::vector<float> &vect); 

CDlgMOE *g_LinkMOEDlg = NULL;
CDlgPathMOE	*g_pPathMOEDlg = NULL;
CDlg_Legend* g_pLegendDlg = NULL;
CDlgLinkList* g_pLinkListDlg = NULL;
CDlg_VehPathAnalysis* g_pVehiclePathDlg = NULL;
CDlgPathList* g_pPathListDlg = NULL;
CDlg_UnitTestingList* g_pUnitTestingListDlg = NULL;

bool g_bShowLinkList = false;
bool g_bShowVehiclePathDialog = false;



extern float g_Simulation_Time_Stamp;
bool g_LinkMOEDlgShowFlag = false;

std::list<s_link_selection>	g_LinkDisplayList;
std::list<CTLiteDoc*>	g_DocumentList;

std::vector<CDlg_VehicleClassification*>	g_SummaryDialogVector;

bool g_bValidDocumentChanged = false;
std::list<CTLiteView*>	g_ViewList;

void g_ClearLinkSelectionList()
{
	//std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
	//while (iDoc != g_DocumentList.end())
	//{

	//	if((*iDoc)!=NULL)
	//	{
	//	for (std::list<DTALink*>::iterator iLink = (*iDoc)->m_LinkSet.begin(); iLink != (*iDoc)->m_LinkSet.end(); iLink++)
	//	{
	//		if((*iLink)!=NULL)
	//		{
	//		(*iLink)->m_DisplayLinkID = -1;
	//		}
	//	}
	//	}
	//	iDoc++;
	//}

	for (std::list<s_link_selection>::iterator iLinkDisplay = g_LinkDisplayList.begin(); iLinkDisplay != g_LinkDisplayList.end();iLinkDisplay++ )
	{

		if((*iLinkDisplay).pLink !=NULL)
			(*iLinkDisplay).pLink -> m_DisplayLinkID =  -1;
	}


	g_LinkDisplayList.clear ();
}

void g_AddLinkIntoSelectionList(DTALink* pLink, int link_no, int document_no, bool b_SelectOtherDocuments, double x, double y)
{
	s_link_selection element;
	element.link_no = link_no;
	element.document_no = document_no;
	element.pLink = pLink;

	g_LinkDisplayList.push_back(element);

	std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
	while (iDoc != g_DocumentList.end())
	{
		if((*iDoc)->m_DocumentNo != document_no)  // not the current document
		{
			GDPoint point;
			point.x = x;
			point.y = y;
			double matching_distance = 0;
			int sel_link_no = (*iDoc)->SelectLink(point, matching_distance);
			if(sel_link_no>=0)  //select link
			{
				element.link_no = sel_link_no;
				element.document_no = (*iDoc)->m_DocumentNo ;
				g_LinkDisplayList.push_back(element);
			}
			//update anyway
			(*iDoc)->UpdateAllViews (0);

		}

		iDoc++;
	}

}
// CTLiteDoc

IMPLEMENT_DYNCREATE(CTLiteDoc, CDocument)

BEGIN_MESSAGE_MAP(CTLiteDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CTLiteDoc::OnFileOpen)
	ON_COMMAND(ID_SHOW_SHOWPATHMOE, &CTLiteDoc::OnShowShowpathmoe)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SHOWPATHMOE, &CTLiteDoc::OnUpdateShowShowpathmoe)
	ON_COMMAND(ID_FILE_SAVE_PROJECT, &CTLiteDoc::OnFileSaveProject)
	ON_COMMAND(ID_FILE_SAVE_PROJECT_AS, &CTLiteDoc::OnFileSaveProjectAs)
	ON_COMMAND(ID_IMAGE_IMPORTBACKGROUNDIMAGE, &CTLiteDoc::OnImageImportbackgroundimage)
	ON_COMMAND(ID_FILE_DATALOADINGSTATUS, &CTLiteDoc::OnFileDataloadingstatus)
	ON_COMMAND(ID_MOE_VOLUME, &CTLiteDoc::OnMoeVolume)
	ON_COMMAND(ID_MOE_SPEED, &CTLiteDoc::OnMoeSpeed)
	ON_UPDATE_COMMAND_UI(ID_MOE_VOLUME, &CTLiteDoc::OnUpdateMoeVolume)
	ON_UPDATE_COMMAND_UI(ID_MOE_SPEED, &CTLiteDoc::OnUpdateMoeSpeed)
	ON_COMMAND(ID_MOE_NONE, &CTLiteDoc::OnMoeNone)
	ON_UPDATE_COMMAND_UI(ID_MOE_NONE, &CTLiteDoc::OnUpdateMoeNone)
	ON_COMMAND(ID_TOOLS_CARFOLLOWINGSIMULATION, &CTLiteDoc::OnToolsCarfollowingsimulation)
	ON_COMMAND(ID_MOE_VC_Ratio, &CTLiteDoc::OnMoeVcRatio)
	ON_UPDATE_COMMAND_UI(ID_MOE_VC_Ratio, &CTLiteDoc::OnUpdateMoeVcRatio)
	ON_COMMAND(ID_MOE_TRAVELTIME, &CTLiteDoc::OnMoeTraveltime)
	ON_UPDATE_COMMAND_UI(ID_MOE_TRAVELTIME, &CTLiteDoc::OnUpdateMoeTraveltime)
	ON_COMMAND(ID_MOE_CAPACITY, &CTLiteDoc::OnMoeCapacity)
	ON_UPDATE_COMMAND_UI(ID_MOE_CAPACITY, &CTLiteDoc::OnUpdateMoeCapacity)
	ON_COMMAND(ID_MOE_SPEEDLIMIT, &CTLiteDoc::OnMoeSpeedlimit)
	ON_UPDATE_COMMAND_UI(ID_MOE_SPEEDLIMIT, &CTLiteDoc::OnUpdateMoeSpeedlimit)
	ON_COMMAND(ID_MOE_FREEFLOWTRAVLETIME, &CTLiteDoc::OnMoeFreeflowtravletime)
	ON_UPDATE_COMMAND_UI(ID_MOE_FREEFLOWTRAVLETIME, &CTLiteDoc::OnUpdateMoeFreeflowtravletime)
	ON_COMMAND(ID_EDIT_DELETESELECTEDLINK, &CTLiteDoc::OnEditDeleteselectedlink)
	ON_COMMAND(ID_MOE_LENGTH, &CTLiteDoc::OnMoeLength)
	ON_UPDATE_COMMAND_UI(ID_MOE_LENGTH, &CTLiteDoc::OnUpdateMoeLength)
	ON_COMMAND(ID_EDIT_SETDEFAULTLINKPROPERTIESFORNEWLINKS, &CTLiteDoc::OnEditSetdefaultlinkpropertiesfornewlinks)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SETDEFAULTLINKPROPERTIESFORNEWLINKS, &CTLiteDoc::OnUpdateEditSetdefaultlinkpropertiesfornewlinks)
	ON_COMMAND(ID_TOOLS_PROJECTFOLDER, &CTLiteDoc::OnToolsProjectfolder)
	ON_COMMAND(ID_TOOLS_OPENNEXTAPROGRAMFOLDER, &CTLiteDoc::OnToolsOpennextaprogramfolder)
	ON_COMMAND(ID_MOE_NOODMOE, &CTLiteDoc::OnMoeNoodmoe)
	ON_UPDATE_COMMAND_UI(ID_MOE_NOODMOE, &CTLiteDoc::OnUpdateMoeNoodmoe)
	ON_COMMAND(ID_ODTABLE_IMPORT_OD_TRIP_FILE, &CTLiteDoc::OnOdtableImportOdTripFile)
	ON_COMMAND(ID_TOOLS_EDITODDEMANDTABLE, &CTLiteDoc::OnToolsEditoddemandtable)
	ON_COMMAND(ID_SEARCH_LINKLIST, &CTLiteDoc::OnSearchLinklist)
	ON_COMMAND(ID_MOE_VEHICLE, &CTLiteDoc::OnMoeVehicle)
	ON_UPDATE_COMMAND_UI(ID_MOE_VEHICLE, &CTLiteDoc::OnUpdateMoeVehicle)
	ON_COMMAND(ID_TOOLS_VIEWSIMULATIONSUMMARY, &CTLiteDoc::OnToolsViewsimulationsummary)
	ON_COMMAND(ID_TOOLS_VIEWASSIGNMENTSUMMARYLOG, &CTLiteDoc::OnToolsViewassignmentsummarylog)
	ON_COMMAND(ID_HELP_VISITDEVELOPMENTWEBSITE, &CTLiteDoc::OnHelpVisitdevelopmentwebsite)
	ON_COMMAND(ID_TOOLS_RUNTRAFFICASSIGNMENT, &CTLiteDoc::OnToolsRuntrafficassignment)
	ON_COMMAND(ID_IMPORTODTRIPFILE_3COLUMNFORMAT, &CTLiteDoc::OnImportodtripfile3columnformat)
	ON_COMMAND(ID_TOOLS_PERFORMSCHEDULING, &CTLiteDoc::OnToolsPerformscheduling)
	ON_COMMAND(ID_FILE_CHANGECOORDINATESTOLONG, &CTLiteDoc::OnFileChangecoordinatestolong)
	ON_COMMAND(ID_TOOLS_EXPORTOPMODEDISTRIBUTION, &CTLiteDoc::OnToolsExportopmodedistribution)
	ON_COMMAND(ID_TOOLS_EXPORTTOGAMSFILE, &CTLiteDoc::OnToolsExporttoHistDatabase)
	ON_COMMAND(ID_RESEARCHTOOLS_EXPORTTODTALITESENSORDATAFORMAT, &CTLiteDoc::OnResearchtoolsExporttodtalitesensordataformat)
	ON_COMMAND(ID_SCENARIO_CONFIGURATION, &CTLiteDoc::OnScenarioConfiguration)
	ON_COMMAND(ID_IMPORT_SINGLEEXCELFILE, &CTLiteDoc::OnImportdataImportExcelFile)
	ON_COMMAND(ID_MOE_VEHICLEPATHANALAYSIS, &CTLiteDoc::OnMoeVehiclepathanalaysis)
	ON_COMMAND(ID_FILE_CONSTRUCTANDEXPORTSIGNALDATA, &CTLiteDoc::OnFileConstructandexportsignaldata)
	ON_COMMAND(ID_FILE_IMPORT_DEMAND_FROM_CSV, &CTLiteDoc::OnFileImportDemandFromCsv)
	ON_COMMAND(ID_IMPORT_SENSOR_DATA, &CTLiteDoc::OnImportSensorData)
	ON_COMMAND(ID_IMPORT_LINKMOE, &CTLiteDoc::OnImportLinkmoe)
	ON_COMMAND(ID_IMPORT_VEHICLEFILE, &CTLiteDoc::OnImportVehiclefile)
	ON_COMMAND(ID_LINKMOE_EMISSIONS, &CTLiteDoc::OnLinkmoeEmissions)
	ON_UPDATE_COMMAND_UI(ID_LINKMOE_EMISSIONS, &CTLiteDoc::OnUpdateLinkmoeEmissions)
	ON_COMMAND(ID_LINKMOE_SAFETY, &CTLiteDoc::OnLinkUserDefinedMOE)
	ON_UPDATE_COMMAND_UI(ID_LINKMOE_SAFETY, &CTLiteDoc::OnUpdateUserDefinedMOE)
	ON_COMMAND(ID_IMPORT_AGENT_FILE, &CTLiteDoc::OnImportAgentFile)
	ON_COMMAND(ID_IMPORT_NGSIM_FILE, &CTLiteDoc::OnImportNgsimFile)
	ON_COMMAND(ID_EXPORT_AMS, &CTLiteDoc::OnExportAms)
	ON_COMMAND(ID_IMPORT_GPS33185, &CTLiteDoc::OnImportGps33185)
	ON_COMMAND(ID_IMPORT_WEATHER33188, &CTLiteDoc::OnImportWeather33188)
	ON_COMMAND(ID_IMPORT_GPS, &CTLiteDoc::OnImportGps)
	ON_COMMAND(ID_LINK_ADD_WORKZONE, &CTLiteDoc::OnLinkAddWorkzone)
	ON_COMMAND(ID_LINK_ADD_INCIDENT, &CTLiteDoc::OnLinkAddIncident)
	ON_COMMAND(ID_LINK_ADDVMS, &CTLiteDoc::OnLinkAddvms)
	ON_COMMAND(ID_IMPORT_SUBAREALAYERFORMAPMATCHING, &CTLiteDoc::OnImportSubarealayerformapmatching)
	ON_COMMAND(ID_LINK_ADDLINK, &CTLiteDoc::OnLinkAddlink)
	ON_COMMAND(ID_LINK_ADDHOVTOLL, &CTLiteDoc::OnLinkAddhovtoll)
	ON_COMMAND(ID_LINK_ADDHOTTOLL, &CTLiteDoc::OnLinkAddhottoll)
	ON_COMMAND(ID_LINK_ADDTOLLEDEXPRESSLANE, &CTLiteDoc::OnLinkAddtolledexpresslane)
	ON_COMMAND(ID_LINK_CONVERTGENERALPURPOSELANETOTOLLEDLANE, &CTLiteDoc::OnLinkConvertgeneralpurposelanetotolledlane)
	ON_COMMAND(ID_LINK_VEHICLESTATISTICSANALAYSIS, &CTLiteDoc::OnLinkVehiclestatisticsanalaysis)
	ON_COMMAND(ID_SUBAREA_DELETESUBAREA, &CTLiteDoc::OnSubareaDeletesubarea)
	ON_COMMAND(ID_TOOLS_TRAVELTIMERELIABILITYANALYSIS, &CTLiteDoc::OnToolsTraveltimereliabilityanalysis)
	ON_COMMAND(ID_LINK_LINKBAR, &CTLiteDoc::OnLinkLinkbar)
	ON_COMMAND(ID_IMPORT_ARCGISSHAPEFILE, &CTLiteDoc::OnImportArcgisshapefile)
	ON_COMMAND(ID_LINK_INCREASEOFFSETFORTWO, &CTLiteDoc::OnLinkIncreaseoffsetfortwo)
	ON_COMMAND(ID_LINK_DECREASEOFFSETFORTWO, &CTLiteDoc::OnLinkDecreaseoffsetfortwo)
	ON_COMMAND(ID_VIEW_SHOWHIDE_LEGEND, &CTLiteDoc::OnViewShowhideLegend)
	ON_COMMAND(ID_MOE_VIEWLINKMOESUMMARYFILE, &CTLiteDoc::OnMoeViewlinkmoesummaryfile)
	ON_COMMAND(ID_VIEW_CALIBRATIONVIEW, &CTLiteDoc::OnViewCalibrationview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CALIBRATIONVIEW, &CTLiteDoc::OnUpdateViewCalibrationview)
	ON_COMMAND(ID_MOE_VIEWODDEMANDESTIMATIONSUMMARYPLOT, &CTLiteDoc::OnMoeViewoddemandestimationsummaryplot)
	ON_COMMAND(ID_LINK_VIEWLINK, &CTLiteDoc::OnLinkViewlink)
	ON_COMMAND(ID_DELETE_SELECTED_LINK, &CTLiteDoc::OnDeleteSelectedLink)
	ON_COMMAND(ID_EXPORT_GENERATEZONE, &CTLiteDoc::OnExportGenerateTravelTimeMatrix)
	ON_COMMAND(ID_EXPORT_GENERATESHAPEFILES, &CTLiteDoc::OnExportGenerateshapefiles)
	ON_COMMAND(ID_LINKMOEDISPLAY_QUEUELENGTH, &CTLiteDoc::OnLinkmoedisplayQueuelength)
	ON_UPDATE_COMMAND_UI(ID_LINKMOEDISPLAY_QUEUELENGTH, &CTLiteDoc::OnUpdateLinkmoedisplayQueuelength)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TRAVELTIMERELIABILITYANALYSIS, &CTLiteDoc::OnUpdateLinkmoeTraveltimereliability)

	ON_COMMAND(ID_MOE_PATHLIST, &CTLiteDoc::OnMoePathlist)
	ON_COMMAND(ID_VIEW_SHOWMOE, &CTLiteDoc::OnViewShowmoe)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWMOE, &CTLiteDoc::OnUpdateViewShowmoe)
	ON_COMMAND(ID_FILE_UPLOADLINKDATATOGOOGLEFUSIONTABLE, &CTLiteDoc::OnFileUploadlinkdatatogooglefusiontable)
	ON_COMMAND(ID_3_VIEWDATAINEXCEL, &CTLiteDoc::On3Viewdatainexcel)
	ON_COMMAND(ID_5_VIEWDATAINEXCEL, &CTLiteDoc::On5Viewdatainexcel)
	ON_COMMAND(ID_MOE_VIEWNETWORKTIMEDEPENDENTMOE, &CTLiteDoc::OnMoeViewnetworktimedependentmoe)
	ON_COMMAND(ID_2_VIEWDATAINEXCEL33398, &CTLiteDoc::On2Viewdatainexcel33398)
	ON_COMMAND(ID_2_VIEWNETWORKDATA, &CTLiteDoc::On2Viewnetworkdata)
	ON_COMMAND(ID_3_VIEWODDATAINEXCEL, &CTLiteDoc::On3Viewoddatainexcel)
	ON_COMMAND(ID_MOE_OPENALLMOETABLES, &CTLiteDoc::OnMoeOpenallmoetables)
	ON_COMMAND(ID_FILE_OPEN_NEW_RAIL_DOC, &CTLiteDoc::OnFileOpenNewRailDoc)
	ON_BN_CLICKED(IDC_BUTTON_Database, &CTLiteDoc::OnBnClickedButtonDatabase)
	ON_COMMAND(ID_TOOLS_UNITTESTING, &CTLiteDoc::OnToolsUnittesting)
	ON_COMMAND(ID_VIEW_TRAININFO, &CTLiteDoc::OnViewTraininfo)
	ON_COMMAND(ID_IMPORT_AMSDATASET, &CTLiteDoc::OnImportAmsdataset)
	ON_COMMAND(ID_DEMANDFILE_ODDEMANDMATRIX, &CTLiteDoc::OnDemandfileOddemandmatrix)
	ON_COMMAND(ID_DEMANDFILE_HOVODDEMANDMATRIX, &CTLiteDoc::OnDemandfileHovoddemandmatrix)
	ON_COMMAND(ID_DEMANDFILE_TRUCKODDEMANDMATRIX, &CTLiteDoc::OnDemandfileTruckoddemandmatrix)
	ON_COMMAND(ID_DEMANDFILE_INTERMODALODDEMANDMATRIX, &CTLiteDoc::OnDemandfileIntermodaloddemandmatrix)
	ON_COMMAND(ID_LINK_ADDINCIDENT, &CTLiteDoc::OnLinkAddIncident)
	ON_COMMAND(ID_TOOLS_GENERATEPHYSICALZONECENTROIDSONROADNETWORK, &CTLiteDoc::OnToolsGeneratephysicalzonecentroidsonroadnetwork)
	ON_COMMAND(ID_IMPORT_DEMANDDATASET, &CTLiteDoc::OnImportDemanddataset)
	ON_COMMAND(ID_NODE_INCREASENODETEXTSIZE, &CTLiteDoc::OnNodeIncreasenodetextsize)
	ON_COMMAND(ID_NODE_DECREASENODETEXTSIZE, &CTLiteDoc::OnNodeDecreasenodetextsize)
	ON_COMMAND(ID_TOOLS_CHECKINGFEASIBILITY, &CTLiteDoc::OnToolsCheckingfeasibility)
	ON_COMMAND(ID_TOOLS_GPSMAPMATCHING, &CTLiteDoc::OnToolsGpsmapmatching)
	ON_COMMAND(ID_IMPORT_SYNCHROUTDFCSVFILES, &CTLiteDoc::OnImportSynchroutdfcsvfiles)
	ON_COMMAND(ID_OUTPUT_MOVEMENT_STRING, &CTLiteDoc::OnOutputMovementcsvfiles)
	ON_COMMAND(ID_PROJECT_EDITMOESETTINGS, &CTLiteDoc::OnProjectEditmoesettings)
	ON_COMMAND(ID_PROJECT_Multi_Scenario_Results, &CTLiteDoc::OnProjectMultiScenarioResults)
	ON_COMMAND(ID_PROJECT_12, &CTLiteDoc::OnProject12)
	ON_COMMAND(ID_VIEW_MOVEMENT_MOE, &CTLiteDoc::OnViewMovementMoe)
	ON_COMMAND(ID_PROJECT_TIME_DEPENDENT_LINK_MOE, &CTLiteDoc::OnProjectTimeDependentLinkMoe)
	ON_COMMAND(ID_VIEW_ODME_Result, &CTLiteDoc::OnViewOdmeResult)
	ON_COMMAND(ID_PROJECT_VIEW_AGENT_MOE, &CTLiteDoc::OnProjectViewAgentMoe)
	ON_COMMAND(ID_PROJECT_ODMATRIXESTIMATIONINPUT, &CTLiteDoc::OnProjectOdmatrixestimationinput)
	ON_COMMAND(ID_PROJECT_INPUTSENSORDATAFORODME, &CTLiteDoc::OnProjectInputsensordataforodme)
	ON_COMMAND(ID_TOOLS_GENERATEODMATRIXGRAVITYMODEL, &CTLiteDoc::OnToolsGenerateodmatrixgravitymodel)
	ON_COMMAND(ID_LINKATTRIBUTEDISPLAY_LINKNAME, &CTLiteDoc::OnLinkattributedisplayLinkname)
	ON_UPDATE_COMMAND_UI(ID_LINKATTRIBUTEDISPLAY_LINKNAME, &CTLiteDoc::OnUpdateLinkattributedisplayLinkname)
	ON_COMMAND(ID_TOOLS_GENERATESIGNALCONTROLLOCATIONS, &CTLiteDoc::OnToolsGeneratesignalcontrollocations)
	ON_COMMAND(ID_AssigngmentSimulationSettings, &CTLiteDoc::OnAssignmentSimulatinSettinsClicked)
	ON_COMMAND(ID_PROJECT_1_NETWORK, &CTLiteDoc::OnProjectNetworkData)
	ON_COMMAND(ID_LINK_ADDSENSOR, &CTLiteDoc::OnLinkAddsensor)
	ON_COMMAND(ID_IMPORT_SYNCHROCOMBINEDCSVFILE, &CTLiteDoc::OnImportSynchrocombinedcsvfile)
	ON_COMMAND(ID_TOOLS_OBTAINCYCLELENGTHFROM, &CTLiteDoc::OnToolsObtainCyclelengthfromNearbySignals)
	ON_COMMAND(ID_TOOLS_SAVEPROJECTFOREXTERNALLAYER, &CTLiteDoc::OnToolsSaveprojectforexternallayer)
	ON_COMMAND(ID_MOE_MOE, &CTLiteDoc::OnMoeTableDialog)
	ON_COMMAND(ID_TOOLS_REVERSEVERTICALCOORDINATE, &CTLiteDoc::OnToolsReverseverticalcoordinate)
	ON_COMMAND(ID_GENERATEGISSHAPEFILES_LOADLINKCSVFILE, &CTLiteDoc::OnGenerategisshapefilesLoadlinkcsvfile)
	ON_COMMAND(ID_SENSORTOOLS_CONVERTTOHOURCOUNT, &CTLiteDoc::OnSensortoolsConverttoHourlyVolume)
	ON_COMMAND(ID_IMPORT_INRIXSHAPEFILEANDSPEEDDATA, &CTLiteDoc::OnImportInrixshapefileandspeeddata)
	ON_COMMAND(ID_TRAFFICCONTROLTOOLS_TRANSFERMOVEMENTDATAFROMREFERENCENETWORKTOCURRENTNETWORK, &CTLiteDoc::OnTrafficcontroltoolsTransfermovementdatafromreferencenetworktocurrentnetwork)
	ON_COMMAND(ID_DEMANDTOOLS_GENERATEINPUT, &CTLiteDoc::OnDemandtoolsGenerateinput)
	ON_COMMAND(ID_DEMAND_RECONSTRUCTLINKMOETH, &CTLiteDoc::OnDemandReconstructlinkmoeth)
	ON_COMMAND(ID_NETWORKTOOLS_RESETLINKLENGTH, &CTLiteDoc::OnNetworktoolsResetlinklength)
	ON_COMMAND(ID_SUBAREA_CREATEZONEFROMSUBAREA, &CTLiteDoc::OnSubareaCreatezonefromsubarea)
	ON_COMMAND(ID_DEMAND_REGENERATEACTIVITYLOCATIONS, &CTLiteDoc::OnDemandRegenerateactivitylocations)
	ON_COMMAND(ID_DEMAND_CONVERT, &CTLiteDoc::OnDemandConvert)
	ON_COMMAND(ID_TRAFFICCONTROLTOOLS_TRANSFERSIGNALDATAFROMREFERENCENETWORKTOCURRENTNETWORK, &CTLiteDoc::OnTrafficcontroltoolsTransfersignaldatafromreferencenetworktocurrentnetwork)
	ON_COMMAND(ID_IMPORT_BACKGROUNDIMAGE, &CTLiteDoc::OnImportBackgroundimage)
	ON_COMMAND(ID_ZONE_DELETEZONE, &CTLiteDoc::OnZoneDeletezone)
	ON_COMMAND(ID_ZONE_VIEWZONEDATA, &CTLiteDoc::OnZoneViewzonedata)
	ON_COMMAND(ID_NODE_VIEWNODEDATA, &CTLiteDoc::OnNodeViewnodedata)
	ON_COMMAND(ID_LINK_VIEWLINKDATA, &CTLiteDoc::OnLinkViewlinkdata)
	ON_COMMAND(ID_MOVEMENT_VIEWMOVEMENTDATATABLE, &CTLiteDoc::OnMovementViewmovementdatatable)
	ON_COMMAND(ID_ODMATRIX_ODDEMANDMATRIX, &CTLiteDoc::OnOdmatrixOddemandmatrix)
	ON_COMMAND(ID_WORKZONE_VIEWWORKZONEDATA, &CTLiteDoc::OnWorkzoneViewworkzonedata)
	ON_COMMAND(ID_VMS_VIEWVMSDATATABLE, &CTLiteDoc::OnVmsViewvmsdatatable)
	ON_COMMAND(ID_TOLL_VIEWTOLLDATATABLE, &CTLiteDoc::OnTollViewtolldatatable)
	ON_COMMAND(ID_DETECTOR_VIEWSENSORDATATABLE, &CTLiteDoc::OnDetectorViewsensordatatable)
	ON_COMMAND(ID_CONNECTOR_VIEWACTIVITYLOCATIONDATATABLE, &CTLiteDoc::OnConnectorViewactivitylocationdatatable)
	ON_COMMAND(ID_DETECTOR_VIEWCALIBRATION, &CTLiteDoc::OnDetectorViewcalibration)
	ON_COMMAND(ID_LINKMOE_EXPORTLINKMOEDATATOSHAPEFILE, &CTLiteDoc::OnLinkmoeExportlinkmoedatatoshapefile)
	ON_COMMAND(ID_NODE_EXPORTNODELAYERTOGISSHAPEFILE, &CTLiteDoc::OnNodeExportnodelayertogisshapefile)
	ON_COMMAND(ID_ZONE_EXPORTZONELAYERTOGISSHAPEFILE, &CTLiteDoc::OnZoneExportzonelayertogisshapefile)
	ON_COMMAND(ID_GRID_USEMILEASUNITOFLENGTH, &CTLiteDoc::OnGridUsemileasunitoflength)
	ON_UPDATE_COMMAND_UI(ID_GRID_USEMILEASUNITOFLENGTH, &CTLiteDoc::OnUpdateGridUsemileasunitoflength)
	ON_COMMAND(ID_GRID_USEKMASUNITOFLENGTH, &CTLiteDoc::OnGridUsekmasunitoflength)
	ON_UPDATE_COMMAND_UI(ID_GRID_USEKMASUNITOFLENGTH, &CTLiteDoc::OnUpdateGridUsekmasunitoflength)
	ON_COMMAND(ID_GRID_USELONG, &CTLiteDoc::OnGridUselong)
	ON_UPDATE_COMMAND_UI(ID_GRID_USELONG, &CTLiteDoc::OnUpdateGridUselong)
	ON_COMMAND(ID_CRASH_VIEWINCIDENTDATATABLE, &CTLiteDoc::OnCrashViewincidentdatatable)
	ON_COMMAND(ID_ZONE_REMOVEACTIVITYLOCATIONSOFSELECTEDZONE, &CTLiteDoc::OnZoneRemoveactivitylocationsofselectedzone)
	ON_COMMAND(ID_ZONE_REGENERATEACTIVITYLOCATIONSFORSELECTEDZONE, &CTLiteDoc::OnZoneRegenerateactivitylocationsforselectedzone)
	ON_COMMAND(ID_SHOW_MOE_PATHLIST, &CTLiteDoc::OnShowMoePathlist)
	ON_COMMAND(ID_EXPORT_EXPORTAGGREGATEDLINKMOEFILE, &CTLiteDoc::OnExportExportaggregatedlinkmoefile)
	ON_COMMAND(ID_HELP_REPORTBUG, &CTLiteDoc::OnHelpReportbug)
	ON_COMMAND(ID_FREEWAYTOOLS_VIEW, &CTLiteDoc::OnFreewaytoolsView)
	ON_COMMAND(ID_EXPORT_GENERATESHAPEFILES_PATH_DATA, &CTLiteDoc::OnExportGenerateshapefilesPathData)
	ON_COMMAND(ID_DETECTOR_EXPORTLINKFLOWPROPORTIONMATRIXTOCSVFILE, &CTLiteDoc::OnDetectorExportlinkflowproportionmatrixtocsvfile)
	ON_COMMAND(ID_DELETE_WORKZONE, &CTLiteDoc::OnDeleteWorkzone)
	ON_COMMAND(ID_CRASH_DELETEINCIDENTONSELECTEDLINK, &CTLiteDoc::OnCrashDeleteincidentonselectedlink)
	ON_COMMAND(ID_SUBAREA_GENERATEEVACUATIONZONESCENARIOFILE, &CTLiteDoc::OnSubareaGenerateevacuationzonescenariofile)
	ON_COMMAND(ID_SUBAREA_GENERATEWEATHERSCENARIOFILE, &CTLiteDoc::OnSubareaGenerateweatherscenariofile)
	ON_COMMAND(ID_SUBAREA_GENERATEWORKZONESCENARIOFILEFROMLINKSINSIDESUBAREA, &CTLiteDoc::OnSubareaGenerateworkzonescenariofilefromlinksinsidesubarea)
	ON_COMMAND(ID_IMPORT_OPENSTREETMAPSHAPEFILE, &CTLiteDoc::OnImportShapefile)
	ON_COMMAND(ID_FILE_OPENTESTSETS, &CTLiteDoc::OnFileOpentestsets)
	ON_COMMAND(ID_FILE_OPENSAMPLEDATASETFOLDER, &CTLiteDoc::OnFileOpensampledatasetfolder)
	ON_COMMAND(ID_LINK_ADD_RADIO_MESSAGE, &CTLiteDoc::OnLinkAddRadioMessage)
	ON_COMMAND(ID_SENSORTOOLS_SENSORDATA, &CTLiteDoc::OnSensortoolsSensordata)
	ON_COMMAND(ID_EXPORT_EXPORTZONE3DKMLFILE, &CTLiteDoc::OnExportExportzone3dkmlfile)
	ON_COMMAND(ID_LINK_APPLYDEFAULTLINKATTRIBUTESTOALLLINKS, &CTLiteDoc::OnLinkApplydefaultlinkattributestoalllinks)
	ON_COMMAND(ID_LINK_APPLYDEFAULTLANECAPACITYTOALLLINKS, &CTLiteDoc::OnLinkApplydefaultlanecapacitytoalllinks)
	ON_COMMAND(ID_LINK_APPLYDEFAULTNUMBEROFLANESTOALLLINKS, &CTLiteDoc::OnLinkApplydefaultnumberoflanestoalllinks)
	ON_COMMAND(ID_EXPORT_EXPORTLINK2DKMLFILE, &CTLiteDoc::OnExportExportlink2dkmlfile)
	ON_COMMAND(ID_EXPORT_EXPORTSIGNALNODEKMLFILE, &CTLiteDoc::OnExportExportsignalnodekmlfile)
	ON_COMMAND(ID_EXPORT_EXPORTNON, &CTLiteDoc::OnExportExportnonsignalnodekmlfile)
	ON_COMMAND(ID_SENSORTOOLS_IMPORTSENSORDATAFROMIMPORT, &CTLiteDoc::OnSensortoolsImportsensordatafromimport)
	ON_COMMAND(ID_TRAFFICCAPACITY_SETDEFAULT, &CTLiteDoc::OnTrafficcapacitySetdefault)
	ON_COMMAND(ID_FILE_REMOVENONESSENTIALFILESTOREDUCEFOLDERSIZE, &CTLiteDoc::OnFileRemovenonessentialfilestoreducefoldersize)
	ON_COMMAND(ID_MOE_VIEWODDEMANDESTIMATIONSUMMARYPLOT_HOURLY, &CTLiteDoc::OnMoeViewoddemandestimationsummaryplotHourly)
	ON_COMMAND(ID_MOE_VIEWODDEMANDESTIMATIONSUMMARYPLOT_LANE_HOURLY, &CTLiteDoc::OnMoeViewoddemandestimationsummaryplotLaneHourly)
	ON_COMMAND(ID_TRANSIT_OUTPUTTRANSITACCESSSIBILITYFROMALLACTIVITYLOCATIONS, &CTLiteDoc::OnTransitOutputtransitaccesssibilityfromallactivitylocations)
	ON_COMMAND(ID_MOVEMENT_HIDENON, &CTLiteDoc::OnMovementHidenon)
	ON_UPDATE_COMMAND_UI(ID_MOVEMENT_HIDENON, &CTLiteDoc::OnUpdateMovementHidenon)
	ON_COMMAND(ID_MOVEMENT_SETPEAKHOURFACTOR, &CTLiteDoc::OnMovementSetpeakhourfactor)
	ON_COMMAND(ID_ZONE_CHANGEZONENUMBER, &CTLiteDoc::OnZoneChangezonenumber)
	ON_UPDATE_COMMAND_UI(ID_ZONE_DELETEZONE, &CTLiteDoc::OnUpdateZoneDeletezone)
	ON_UPDATE_COMMAND_UI(ID_ZONE_CHANGEZONENUMBER, &CTLiteDoc::OnUpdateZoneChangezonenumber)
	ON_COMMAND(ID_CHANGELINKTYPECOLOR_FREEWAY, &CTLiteDoc::OnChangelinktypecolorFreeway)
	ON_COMMAND(ID_CHANGELINKTYPECOLOR_RAMP, &CTLiteDoc::OnChangelinktypecolorRamp)
	ON_COMMAND(ID_CHANGELINKTYPECOLOR_ARTERIAL, &CTLiteDoc::OnChangelinktypecolorArterial)
	ON_COMMAND(ID_CHANGELINKTYPECOLOR_CONNECTOR, &CTLiteDoc::OnChangelinktypecolorConnector)
	ON_COMMAND(ID_CHANGELINKTYPECOLOR_TRANSIT, &CTLiteDoc::OnChangelinktypecolorTransit)
	ON_COMMAND(ID_CHANGELINKTYPECOLOR_WALKINGMODE, &CTLiteDoc::OnChangelinktypecolorWalkingmode)
	ON_COMMAND(ID_CHANGELINKTYPECOLOR_RESETTODEFAULTCOLORSCHEMA, &CTLiteDoc::OnChangelinktypecolorResettodefaultcolorschema)
	ON_COMMAND(ID_NODE_CHANGENODECOLOR, &CTLiteDoc::OnNodeChangenodecolor)
	ON_COMMAND(ID_NODE_CHANGENODEBACKGROUNDCOLOR, &CTLiteDoc::OnNodeChangenodebackgroundcolor)
	ON_COMMAND(ID_ZONE_CHANGEZONECOLOR, &CTLiteDoc::OnZoneChangezonecolor)
	ON_COMMAND(ID_EDIT_UNDO33707, &CTLiteDoc::OnEditUndo33707)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO33707, &CTLiteDoc::OnUpdateEditUndo33707)
	ON_COMMAND(ID_EDIT_REDO33709, &CTLiteDoc::OnEditRedo33709)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO33709, &CTLiteDoc::OnUpdateEditRedo33709)
	ON_COMMAND(ID_MOVEMENT_SETUPNUMBEROFLEFTTURNLANESFORSIGNALIZEDNODES, &CTLiteDoc::OnMovementSetupnumberofleftturnlanesforsignalizednodes)
	ON_COMMAND(ID_MOVEMENT_OPTIMIZEPHASINGANDTIMINGDATAFORALLTRAFFICSIGNALSTHROUGHQEM, &CTLiteDoc::OnMovementOptimizephasingandtimingdataforalltrafficsignalsthroughqem)
	ON_COMMAND(ID_LINKMOEDISPLAY_IMPACT, &CTLiteDoc::OnLinkmoedisplayImpact)
	ON_UPDATE_COMMAND_UI(ID_LINKMOEDISPLAY_IMPACT, &CTLiteDoc::OnUpdateLinkmoedisplayImpact)
	ON_COMMAND(ID_LINKMOEDISPLAY_BOTTLENECK, &CTLiteDoc::OnLinkmoedisplayBottleneck)
	ON_UPDATE_COMMAND_UI(ID_LINKMOEDISPLAY_BOTTLENECK, &CTLiteDoc::OnUpdateLinkmoedisplayBottleneck)
	ON_COMMAND(ID_FILE_TRAFFICDATASETTING, &CTLiteDoc::OnFileTrafficdatasetting)
	ON_COMMAND(ID_EXPORT_DYNASMART, &CTLiteDoc::OnExportDynasmart)
	ON_COMMAND(ID_DEMAND_USEROADCAPACITYTOGENERATEDEFAULTPRODUCTIONANDATTRACTIONFOREACHZONE, &CTLiteDoc::OnDemandUseroadcapacitytogeneratedefaultproductionandattractionforeachzone)
	ON_COMMAND(ID_GISPLANNINGDATASET_CONFIGUREIMPORTINGSETTINGFILE, &CTLiteDoc::OnGisplanningdatasetConfigureimportingsettingfile)
	ON_COMMAND(ID_SUBAREA_EXPORTTOTALNUMBEROFVEHICLESINSUBAREA, &CTLiteDoc::OnSubareaExporttotalnumberofvehiclesinsubarea)
	ON_COMMAND(ID_RAMP_RAMPDATA, &CTLiteDoc::OnRampRampdata)
	ON_COMMAND(ID_LINK_ADD_RAMPMETER, &CTLiteDoc::OnLinkAddRampmeter)
	ON_COMMAND(ID_DELETE_RAMPMETER, &CTLiteDoc::OnDeleteRampmeter)
	ON_COMMAND(ID_MOE_VIEWODDEMANDESTIMATIONSUMMARYPLOT_LaneDensity, &CTLiteDoc::OnMoeViewoddemandestimationsummaryplotLanedensity)
	ON_COMMAND(ID_TOOLS_CONFIGURATION, &CTLiteDoc::OnToolsConfiguration)
	ON_COMMAND(ID_DETECTOR_OVERWRITESENSORLOCATIONDATA, &CTLiteDoc::OnDetectorOverwritesensorlocationdata)
	ON_COMMAND(ID_SENSORTOOLS_MAPSENSORLOCATIONTOMODELLINKS, &CTLiteDoc::OnSensortoolsMapsensorlocationtomodellinks)
	ON_COMMAND(ID_NETWORKTOOLS_GENERATELOOPCODEANDDIRECTIONCODE, &CTLiteDoc::OnNetworktoolsGenerateloopcodeanddirectioncode)
	ON_COMMAND(ID_SENSORTOOLS_CLEANSENSORDATAWITHREASONABLERANGE, &CTLiteDoc::OnSensortoolsCleansensordatawithreasonablerange)
	ON_COMMAND(ID_REFERENCE_CREATESPEEDSENSORMAPPINGFORBASELINENETWORK, &CTLiteDoc::OnReferenceCreatespeedsensormappingforbaselinenetwork)
	ON_COMMAND(ID_DETECTOR_OVERWRITESENSORLOCATIONANDSENSORCOUNTDATA, &CTLiteDoc::OnDetectorOverwritesensorlocationandsensorcountdata)
	ON_COMMAND(ID_SENSORTOOLS_VIEWVALIDATIONPLOTFORLINKSPEED, &CTLiteDoc::OnSensortoolsViewvalidationplotforlinkspeed)
	ON_COMMAND(ID_TOLL_ADDBUSTOLL, &CTLiteDoc::OnTollAddbustoll)
	ON_COMMAND(ID_TOLL_ADDBRTTOLL, &CTLiteDoc::OnTollAddbrttoll)
	ON_COMMAND(ID_TOLL_ADDMETROTOLL, &CTLiteDoc::OnTollAddmetrotoll)
	ON_COMMAND(ID_TRANSIT_GENERATETRANSITSPACE, &CTLiteDoc::OnTransitGeneratetransitspace)
	ON_COMMAND(ID_PATH_CLEARALLPATHDISPLAY, &CTLiteDoc::OnPathClearallpathdisplay)
	ON_COMMAND(ID_TOOLS_GENERATEGISSHAPEFILES, &CTLiteDoc::OnToolsGeneratedefaultinputsignalfile)
		ON_COMMAND(ID_TOOLS_IMPORTSYNCHRONCOMBINEDFILE, &CTLiteDoc::OnToolsImportsynchroncombinedfile)
		ON_COMMAND(ID_ODMATRIX_RAPID, &CTLiteDoc::OnOdmatrixRapid)
		ON_COMMAND(ID_ODMATRIX_RAPID_Decrease, &CTLiteDoc::OnOdmatrixRapidDecrease)
		ON_COMMAND(ID_BUTTON_ABM, &CTLiteDoc::OnButtonAbm)
		END_MESSAGE_MAP()


// CTLiteDoc construction/destruction

CTLiteDoc::CTLiteDoc()
{
	m_DemandAlpha = 1.0;
	bSynchroImportReadyToSaveFlag = false;

	m_calibration_data_start_time_in_min = 0;
	m_calibration_data_end_time_in_min = 1440;

	m_ImageMoveSize = 0.0001;
	m_SensorMapX = 0;
	m_SensorMapY = 0;
	
	m_SensorMapXResolution = 1;
	m_SensorMapYResolution = 1;
	m_SensorMapMoveSize = 0.0001;

	m_PrimaryDataSource = eSimulationData;

	AddNameIntoTimingPlanVector(0, 1440,"0");

	m_CurrentDisplayTimingPlanName = "0";

	m_EmissionType = DTA_CO2;


	m_MovementTextBoxSizeInFeet = 50;
	m_bShowSignalNodeMovementOnly = true;

	m_hide_non_specified_movement_on_freeway_and_ramp = true;

	m_sensor_data_aggregation_type = 0;  // no processing, 1:  hourly data: 2: hourly and lane-based data 

	m_GridMatrix = Allocate3DDynamicArray<GridNodeSet>(_MAX_TRANSIT_GRID_SIZE,_MAX_TRANSIT_GRID_SIZE ,_MAX_TRANSIT_TIME_SIZE );

	m_TurnDirectionStringMap["NBL2"] =  DTA_NBL2;
	m_TurnDirectionStringMap["NBL"] =  DTA_NBL;
	m_TurnDirectionStringMap["NBT"] =  DTA_NBT;
	m_TurnDirectionStringMap["NBR"] =  DTA_NBR;
	m_TurnDirectionStringMap["NBR2"] =  DTA_NBR2;

	m_TurnDirectionStringMap["SBL2"] =  DTA_SBL2;
	m_TurnDirectionStringMap["SBL"] =  DTA_SBL;
	m_TurnDirectionStringMap["SBT"] =  DTA_SBT;
	m_TurnDirectionStringMap["SBR"] =  DTA_SBR;
	m_TurnDirectionStringMap["SBR2"] =  DTA_SBR2;

	m_TurnDirectionStringMap["EBL2"] =  DTA_EBL2;
	m_TurnDirectionStringMap["EBL"] =  DTA_EBL;
	m_TurnDirectionStringMap["EBT"] =  DTA_EBT;
	m_TurnDirectionStringMap["EBR"] =  DTA_EBR;
	m_TurnDirectionStringMap["EBR2"] =  DTA_EBR2;

	m_TurnDirectionStringMap["WBL2"] =  DTA_WBL2;
	m_TurnDirectionStringMap["WBL"] =  DTA_WBL;
	m_TurnDirectionStringMap["WBT"] =  DTA_WBT;
	m_TurnDirectionStringMap["WBR"] =  DTA_WBR;
	m_TurnDirectionStringMap["WBR2"] =  DTA_WBR2;

	m_PeakHourFactor = 1.0;
	m_bIdentifyBottleneckAndOnOffRamps = false;
	m_ScreenWidth_InMile = 10;

	m_bUseMileVsKMFlag = true;
	m_bRightHandTrafficFlag = 1;
	m_ImageWidthInMile = 1;
	m_PointA_x = m_PointA_y = m_PointB_x = m_PointB_y = 0;
	m_PointA_long = m_PointA_lat = m_PointB_long = m_PointB_lat= 0;
	m_bPointA_Initialized =  m_bPointB_Initialized = false;

	m_bSummaryDialog = false;
	m_StartNodeNumberForNewNodes = 1;
	m_demand_multiplier = 1;

	m_number_of_iterations = 20;
	m_number_of_reporting_days = 1;
	m_traffic_flow_model = 1;
	m_signal_reresentation_model = 0;
	m_traffic_analysis_method = 0;

	m_ActivityLocationCount = 0;
	m_SearchMode= efind_node;
	m_LongLatFlag = false; // default, we do not know if the coordinate system is long or lat
	m_bUnitMileInitialized = false;
	g_bValidDocumentChanged = true;

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	m_DefaultDataFolder.Format ("%s\\default_data_folder\\",pMainFrame->m_CurrentDirectory);

	m_ZoneNoSize  = 0;
	m_bRunCrashPredictionModel = false;
	m_ZoomToSelectedObject = true;
	m_max_walking_distance = 0.25;
	m_max_accessible_transit_time_in_min = 15;

	m_DemandTypeSize = 1; // just one demand trip type:
	m_ODMOEMatrix = NULL;

	m_bMovementAvailableFlag = false;

	m_ImportNetworkAlready = false;
	m_DemandLoadingStartTimeInMin = 420;
	m_DemandLoadingEndTimeInMin = 480;

	m_bSaveProjectFromSubareaCut = false;
	m_bSaveProjectFromImporting = false;
	m_bExport_Link_MOE_in_input_link_CSF_File = false;
	m_bBezierCurveFlag = true;

	m_CriticalOriginZone = -1;
	m_CriticalDestinationZone = -1;

	m_ControlType_UnknownControl = 0;
	m_ControlType_NoControl = 1;
	m_ControlType_YieldSign = 2;
	m_ControlType_2wayStopSign = 3;
	m_ControlType_4wayStopSign = 4;
	m_ControlType_PretimedSignal = 5;
	m_ControlType_ActuatedSignal = 6;
	m_ControlType_Roundabout = 7;
	m_ControlType_ExternalNode = 100;


	m_LinkTypeMap[1].link_type = 1;
	m_LinkTypeMap[1].link_type_name = "Freeway";
	m_LinkTypeMap[1].type_code = "f";

	m_LinkTypeMap[2].link_type = 2;
	m_LinkTypeMap[2].link_type_name = "Highway";
	m_LinkTypeMap[2].type_code = "h";

	m_LinkTypeMap[3].link_type = 3;
	m_LinkTypeMap[3].link_type_name = "Principal arterial";
	m_LinkTypeMap[3].type_code = "a";

	m_LinkTypeMap[4].link_type = 4;
	m_LinkTypeMap[4].link_type_name = "Major arterial";
	m_LinkTypeMap[4].type_code = "a";

	m_LinkTypeMap[5].link_type = 5;
	m_LinkTypeMap[5].link_type_name = "Minor arterial";
	m_LinkTypeMap[5].type_code = "a";

	m_LinkTypeMap[6].link_type = 6;
	m_LinkTypeMap[6].link_type_name = "Collector";
	m_LinkTypeMap[6].type_code = "a";

	m_LinkTypeMap[7].link_type = 7;
	m_LinkTypeMap[7].link_type_name = "Local";
	m_LinkTypeMap[7].type_code = "a";

	m_LinkTypeMap[8].link_type = 8;
	m_LinkTypeMap[8].link_type_name = "Frontage road";
	m_LinkTypeMap[8].type_code = "a";

	m_LinkTypeMap[9].link_type = 9;
	m_LinkTypeMap[9].link_type_name = "Ramp";
	m_LinkTypeMap[9].type_code = "r";

	m_LinkTypeMap[10].link_type = 10;
	m_LinkTypeMap[10].link_type_name = "Zonal connector";
	m_LinkTypeMap[10].type_code = "c";

	m_LinkTypeMap[100].link_type = 100;
	m_LinkTypeMap[100].link_type_name = "Transit link";
	m_LinkTypeMap[100].type_code = "t";

	m_LinkTypeMap[200].link_type = 200;
	m_LinkTypeMap[200].link_type_name = "Walking link";
	m_LinkTypeMap[200].type_code = "w";

	m_LinkTypeFreeway = 1;
	m_LinkTypeArterial = 3;
	m_LinkTypeHighway = 2;


	// blue style
	m_KML_style_map["green"] = "7d00ff00";
	m_KML_style_map["red"] = "7d0000ff";
	m_KML_style_map["blue"] = "7dff0000";
	m_KML_style_map["yellow"] = "7d00ffff";


	m_OriginOnBottomFlag = 1;

	m_DocumentNo = g_DocumentList.size();
	g_DocumentList.push_back (this);

	m_RandomRoutingCoefficient = 0.0f;
	m_bDYNASMARTDataSet = false;
	m_bGPSDataSet = false;
	m_bEmissionDataAvailable = false;
	m_AdjLinkSize = 20;  // initial value
	m_ColorDirection = -1;
	m_colorLOS[0] = RGB(190,190,190);
	m_colorLOS[1] = RGB(0,255,0);
	m_colorLOS[2] = RGB(255,250,117);
	m_colorLOS[3] = RGB(255,250,0);
	m_colorLOS[4] = RGB(255,216,0);
	m_colorLOS[5] = RGB(255,153,0);
	m_colorLOS[6] = RGB(255,0,0);


	m_ColorDirectionVector[DTA_South] = RGB(255,69,0 );  // orange red
	m_ColorDirectionVector[DTA_North] = RGB(255,255,255); // white
	m_ColorDirectionVector[DTA_East] = RGB(192,192,192 ); // Silver
	m_ColorDirectionVector[DTA_West] = RGB(169,169,169); // gray

	m_ColorDirectionVector[DTA_NorthEast] = RGB(255,69,0);  //orange red
	m_ColorDirectionVector[DTA_NorthWest] = RGB(255,69,0); //orange red
	m_ColorDirectionVector[DTA_SouthEast] = RGB(255,255,255 ); // white
	m_ColorDirectionVector[DTA_SouthWest] = RGB(255,255,255); // white
	m_ColorDirectionVector[DTA_NotDefined] = RGB(169,169,169); // grey



	m_MaxLinkWidthAsNumberOfLanes = 5;
	m_MaxLinkWidthAsLinkVolume = 1000;

	m_VehicleSelectionMode = CLS_network;
	m_LinkBandWidthMode = LBW_number_of_lanes;
	m_bLoadNetworkDataOnly = false;
	m_SamplingTimeInterval = 1;
	m_AVISamplingTimeInterval = 5;

	m_bSimulationDataLoaded  = false;
	m_EmissionDataFlag = false;
	m_bLinkToBeShifted = true;
	m_SimulationStartTime_in_min = 0;  // 6 AM
	m_SimulationEndTime_in_min = 1440;

	m_NumberOfDays = 0;
	m_LinkMOEMode = MOE_none;
	m_PrevLinkMOEMode = MOE_fftt;  // make sure the document gets a change to initialize the display view as the first mode is MOE_none
	m_ODMOEMode = odnone;

	MaxNodeKey = 60000;  // max: unsigned short 65,535;
	m_BackgroundBitmapLoaded  = false;
	m_LongLatCoordinateFlag = false;
	m_ColorFreeway = RGB(198,226,255);
	m_ColorHighway = RGB(100,149,237);
	m_ColorArterial = RGB(0,0,0);
	m_pNetwork = NULL;
	m_OriginNodeID = -1;

	m_DestinationNodeID = -1;
	m_NodeSizeSP = 0;


	m_PathMOEDlgShowFlag = false;
	m_SelectPathNo = -1;

	m_ImageX1 = 0;
	m_ImageX2 = 1000;
	m_ImageY1 = 0;
	m_ImageY2 = 1000;

	m_ZoneTextColor = RGB(0,191,255);

	m_bLoadMovementData = true;

	if(theApp.m_VisulizationTemplate == e_traffic_assignment)
	{

		m_NodeDisplaySize = 100;  // in feet
		m_BottleneckDisplaySize = 1;
		m_VehicleDisplaySize = 10; // in feet
		theApp.m_BackgroundColor =  RGB(255,255,255);  //white
		m_NodeTextDisplayRatio = 4;

	}

	if(theApp.m_VisulizationTemplate == e_train_scheduling)
	{
		m_NodeDisplaySize = 2000;  // in feet
		m_VehicleDisplaySize= 1000; // in feet
		theApp.m_BackgroundColor =  RGB(255,255,255);
		m_NodeTextDisplayRatio = 8;
	}



	m_NetworkRect.top  = 50;
	m_NetworkRect.bottom = 0;

	m_NetworkRect.left   = 0;
	m_NetworkRect.right = 100;

	m_UnitMile = 1;
	m_UnitFeet = m_UnitMile/5280.0;


	m_OffsetInFeet = 15;
	m_LaneWidthInFeet = 10;
	m_bFitNetworkInitialized = false; 

	m_DefaultNumLanes = 1;
	m_DefaultSpeedLimit = 65.0f;
	m_DefaultCapacity = 1900.0f;
	m_DefaultLinkType = 1;

	m_ODSize = 0;
	m_PreviousODSize = -1;
	m_SelectedLinkNo = -1;
	m_SelectedNodeID = -1;
	m_SelectedZoneID = -1;
	m_SelectedVehicleID = -1;

	m_bSetView = false;
	m_bShowLegend = false;
	m_bShowPathList = true;

	for(int i=0; i<40;i++)
	{
		for(int los= 0; los < MAX_LOS_SIZE; los++)
		{
			m_LOSBound[i][los] = 0;
		}
	}

	// speed LOS bound
	m_LOSBound[MOE_speed][1] = 100;
	m_LOSBound[MOE_speed][2] = 80;
	m_LOSBound[MOE_speed][3] = 65;
	m_LOSBound[MOE_speed][4] = 50;
	m_LOSBound[MOE_speed][5] = 40;
	m_LOSBound[MOE_speed][6] = 33;
	m_LOSBound[MOE_speed][7] = 0;

	m_LOSBound[MOE_reliability][1] = 0;
	m_LOSBound[MOE_reliability][2] = 0.1f;
	m_LOSBound[MOE_reliability][3] = 0.2f;
	m_LOSBound[MOE_reliability][4] = 0.3f;
	m_LOSBound[MOE_reliability][5] = 0.5f;
	m_LOSBound[MOE_reliability][6] = 0.7f;
	m_LOSBound[MOE_reliability][7] = 999;

	m_LOSBound[MOE_density][1] = 0;
	m_LOSBound[MOE_density][2] = 11;
	m_LOSBound[MOE_density][3] = 17;
	m_LOSBound[MOE_density][4] = 25;
	m_LOSBound[MOE_density][5] = 35;
	m_LOSBound[MOE_density][6] = 45;
	m_LOSBound[MOE_density][7] = 999;

	m_LOSBound[MOE_impact][7] = 100;
	m_LOSBound[MOE_impact][6] = 83.3;
	m_LOSBound[MOE_impact][5] = 66.7;
	m_LOSBound[MOE_impact][4] = 50.0;
	m_LOSBound[MOE_impact][3] = 33.3;
	m_LOSBound[MOE_impact][2] = 16.7;
	m_LOSBound[MOE_impact][1] = 0;


	m_LOSBound[MOE_bottleneck][7] = 100;
	m_LOSBound[MOE_bottleneck][6] = 83.3;
	m_LOSBound[MOE_bottleneck][5] = 66.7;
	m_LOSBound[MOE_bottleneck][4] = 50.0;
	m_LOSBound[MOE_bottleneck][3] = 33.3;
	m_LOSBound[MOE_bottleneck][2] = 16.7;
	m_LOSBound[MOE_bottleneck][1] = 0;



	// http://www.epa.gov/otaq/consumer/420f08024.pdf
	m_LOSBound[MOE_emissions ][0] = 100.0;
	m_LOSBound[MOE_emissions ][1] = 40.0;
	m_LOSBound[MOE_emissions][2] =  35.0;
	m_LOSBound[MOE_emissions ][3] = 30.0;;
	m_LOSBound[MOE_emissions][4] = 25.0;;
	m_LOSBound[MOE_emissions][5] = 20.0;;
	m_LOSBound[MOE_emissions][6] = 15.0;;
	m_LOSBound[MOE_emissions][7] =  10.0;


	float max_value[10];
	max_value[1]= 2.0000;
	max_value[2]= 5.0000;
	max_value[3]= 5.0000;
	max_value[4]= 0.6;

	for(int type =1; type<=4; type++)
	{
		for(int los = 0; los<=7; los++)
		{
			m_LOSBound[MOE_emissions + type][los] = max_value[type]*(los-1)/7.0;
		}
	}
	m_TrafficFlowModelFlag = 1;  // static traffic assignment as default
	m_Doc_Resolution = 1;
	m_bShowCalibrationResults = false;

	m_SampleExcelNetworkFile = "\\sample_data_sets\\2. Importing_Creating_Traffic_Modeling_Data_Sets\\0. Import_Excel_files\\";
	//	m_SampleExcelSensorFile = "\\Sample_Import_Excel_Files\\input_Portland_sensor_data.xls";

	TDDemandSOVMatrix = NULL;
	TDDemandHOVMatrix = NULL;
	TDDemandTruckMatrix = NULL;

}

static bool DeleteLinkPointer( DTALink * theElement ) { delete theElement; return true; }

void CTLiteDoc::ClearNetworkData()
{
	m_LinkKeyMap.clear ();
	m_SpeedSensorIDMap.clear ();
	m_CountSensorIDMap.clear ();

	m_NodeSet.clear ();
	m_LinkSet.clear ();

	m_NodeNotoZoneNameMap.clear();
	m_NodeNotoLinkMap.clear();

	m_NodeNumbertoNodeNoMap.clear();
	m_RailMOW_vector.clear();
	m_train_schedule_vector.clear();
	m_ODSize = 0;
	m_PathDisplayList.clear();
	m_LinkIDRecordVector.clear();
	m_MessageStringVector.clear();

	m_DTAPointSet.clear();
	m_DTALineSet.clear();


	m_SubareaNodeSet.clear();
	m_SubareaLinkSet.clear();
	m_ZoneMap.clear();

	m_DemandFileVector.clear();

	m_SensorMap.clear();



}

CTLiteDoc::~CTLiteDoc()
{
	if(m_GridMatrix)
		Deallocate3DDynamicArray<GridNodeSet>(m_GridMatrix, _MAX_TRANSIT_GRID_SIZE, _MAX_TRANSIT_GRID_SIZE);


	g_bValidDocumentChanged = true;
	CWaitCursor wait;
	if(m_ODMOEMatrix !=NULL)
		Deallocate3DDynamicArray<VehicleStatistics>(m_ODMOEMatrix,	m_PreviousDemandTypeSize, m_PreviousZoneNoSize);


	if(g_LinkMOEDlg!=NULL && g_LinkMOEDlg->m_pDoc == this)
	{
		delete g_LinkMOEDlg;
		g_LinkMOEDlg= NULL;

	}

	if(g_pPathMOEDlg!=NULL &&  g_pPathMOEDlg->m_pDoc == this)
	{
		delete g_pPathMOEDlg;
		g_pPathMOEDlg= NULL;

	}

	if(g_pLegendDlg!=NULL &&  g_pLegendDlg->m_pDoc == this)
	{
		delete g_pLegendDlg;
		g_pLegendDlg= NULL;

	}

	if(g_pLinkListDlg!=NULL && g_pLinkListDlg->m_pDoc == this)
	{
		delete g_pLinkListDlg;
		g_pLinkListDlg= NULL;

	}

	if(g_pVehiclePathDlg!=NULL && g_pVehiclePathDlg->m_pDoc == this)
	{
		delete g_pVehiclePathDlg;
		g_pVehiclePathDlg= NULL;

	}
	if(g_pPathListDlg!=NULL && g_pPathListDlg->m_pDoc == this)
	{
		delete g_pPathListDlg;
		g_pPathListDlg= NULL;

	}

	if(g_pUnitTestingListDlg!=NULL && g_pUnitTestingListDlg->m_pDoc == this)
	{
		delete g_pUnitTestingListDlg;
		g_pUnitTestingListDlg= NULL;

	}

	std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
	while (iDoc != g_DocumentList.end())
	{
		if((*iDoc) == this)
		{
			g_DocumentList.erase (iDoc);  // remove the document to be deleted
			break;

		}
		iDoc++;
	}

	m_NEXTALOGFile.close();

	if(m_pNetwork!=NULL)
		delete m_pNetwork;


	//	m_LinkSet.remove_if (DeleteLinkPointer);

	/*
	std::list<DTANode*>::iterator iNode;
	for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{
	DTANode* pNode = (*iNode);

	if(pNode!=NULL)
	delete pNode;

	}
	*/
}

BOOL CTLiteDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTLiteDoc serialization

void CTLiteDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CTLiteDoc diagnostics

#ifdef _DEBUG
void CTLiteDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTLiteDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG



void CTLiteDoc::SetStatusText(CString StatusText)
{
	CTLiteView* pView = 0;
	POSITION pos = GetFirstViewPosition();
	if(pos != NULL)
	{
		pView = (CTLiteView*) GetNextView(pos);
		if(pView!=NULL)
			pView->SetStatusText(StatusText);
	}


}

void CTLiteDoc::ReadSimulationLinkMOEData_Parser(LPCTSTR lpszFileName)
{
	CCSVParser parser;
	int i= 0;
	if (parser.OpenCSVFile(lpszFileName))
	{

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->ResetMOEAry(g_Simulation_Time_Horizon);  // use one day horizon as the default value
		}

		while(parser.ReadRecord())
		{

			int from_node_number;
			if(parser.GetValueByFieldName("from_node_id",from_node_number) == false)
				break;
			int to_node_number ;

			if(parser.GetValueByFieldName("to_node_id",to_node_number) == false)
				break;

			int t;

			if(parser.GetValueByFieldName("timestamp_in_min",t) == false)
				break;

			DTALink* pLink = FindLinkWithNodeNumbers(from_node_number , to_node_number, lpszFileName );

			if(pLink!=NULL)
			{
				if(t < g_Simulation_Time_Horizon)
				{
					//travel_time_in_min, delay_in_min, link_volume_in_veh, link_volume_in_vehphpl,
					//density_in_veh_per_mile_per_lane, speed_in_mph, queue_length_in_, cumulative_arrival_count, cumulative_departure_count
					parser.GetValueByFieldName("travel_time_in_min",pLink->m_LinkMOEAry[t].TravelTime);
					parser.GetValueByFieldName("link_volume_in_veh_per_hour_for_all_lanes",pLink->m_LinkMOEAry[t].LinkFlow);
					parser.GetValueByFieldName("density_in_veh_per_distance_per_lane",pLink->m_LinkMOEAry[t].Density );
					parser.GetValueByFieldName("speed",pLink->m_LinkMOEAry[t].Speed);
					parser.GetValueByFieldName("exit_queue_length",pLink->m_LinkMOEAry[t].QueueLength );
					parser.GetValueByFieldName("number_of_queued_vehicles", pLink->m_LinkMOEAry[t].number_of_queued_vehicles);
					parser.GetValueByFieldName("cumulative_arrival_count", pLink->m_LinkMOEAry[t].ArrivalCumulativeFlow);
					parser.GetValueByFieldName("cumulative_departure_count",pLink->m_LinkMOEAry[t].TravelTime);
					
				}
				i++;
			}else
			{
				CString msg;
				msg.Format ("Please check if link %d->%d at file %s still exists in input_link.csv.", from_node_number , to_node_number, lpszFileName);  // +2 for the first field name line
				AfxMessageBox(msg);
				break;
			}

		}

		m_bSimulationDataLoaded = true;

		g_Simulation_Time_Stamp = 0; // reset starting time
		g_SimulationStartTime_in_min = 0;

		m_SimulationLinkTDMOEDataLoadingStatus.Format ("%d link time-dependent MOE records are loaded from file %s.",i,lpszFileName);
	}
}

void CTLiteDoc::ReadModelLinkMOEData_Parser(LPCTSTR lpszFileName)
{
	CCSVParser parser;
	int i= 0;
	if (parser.OpenCSVFile(lpszFileName))
	{

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->ResetMOEAry(g_Simulation_Time_Horizon);  // use one day horizon as the default value
		}

		while(parser.ReadRecord())
		{

			std::string link_key;
			if(parser.GetValueByFieldName("link_key",link_key) == false)
				break;

			int start_time_in_min,end_time_in_min;

			if(parser.GetValueByFieldName("start_time_in_min",start_time_in_min) == false)
				break;

			if(parser.GetValueByFieldName("end_time_in_min",end_time_in_min) == false)
				break;

				if(start_time_in_min > end_time_in_min)
				{

				CString str_msg;
				str_msg.Format ( "Error: simulation link MOE record No.%d, start_time_in_min > end_time_in_min", i+1);
				AfxMessageBox(str_msg);
				break;
				
				}

			if(start_time_in_min <  m_DemandLoadingStartTimeInMin)
				m_DemandLoadingStartTimeInMin = start_time_in_min;

			if(end_time_in_min >  m_DemandLoadingEndTimeInMin)
				m_DemandLoadingEndTimeInMin = end_time_in_min ;


			if(m_LinkKeyMap.find(link_key.c_str())!=m_LinkKeyMap.end())
			{
						
				DTALink* pLink =  m_LinkKeyMap[link_key.c_str()];


					float link_hourly_volume = 0;
					float density = 0;
					float speed_per_hour = 0;
					float queue_length_percentage = 0;

					parser.GetValueByFieldName("link_hourly_volume",link_hourly_volume);
					parser.GetValueByFieldName("density",density);
					parser.GetValueByFieldName("speed_per_hour",speed_per_hour);
					parser.GetValueByFieldName("queue_length_percentage",queue_length_percentage);

					float inflow_count = 0; 
					float outflow_count = 0;
					parser.GetValueByFieldName("inflow_count",inflow_count);
					parser.GetValueByFieldName("outflow_count",outflow_count);

					float energy = 0;
					float CO2 = 0;
					float NOX = 0;
					float CO = 0;
					float HC = 0;

					parser.GetValueByFieldName("energy", energy);
					parser.GetValueByFieldName("CO2", CO2);
					parser.GetValueByFieldName("NOx", NOX);
					parser.GetValueByFieldName("CO", CO);
					parser.GetValueByFieldName("HC", HC);

					for(int t = start_time_in_min; t < min(end_time_in_min,g_Simulation_Time_Horizon-1); t++)
				{
					pLink->m_LinkMOEAry[t].LinkFlow  = link_hourly_volume;
					pLink->m_LinkMOEAry[t].Density  = density;
					pLink->m_LinkMOEAry[t].Speed = speed_per_hour;
					pLink->m_LinkMOEAry[t].QueueLength = queue_length_percentage;
					pLink->m_LinkMOEAry[t].TravelTime = pLink->m_Length / max(0.01,speed_per_hour);
	
					float incount = inflow_count/max(1,end_time_in_min-start_time_in_min);  // min count
					float outcount = outflow_count/max(1,end_time_in_min-start_time_in_min);  // min count

					if(t>=1)
					{
					pLink->m_LinkMOEAry[t].ArrivalCumulativeFlow = pLink->m_LinkMOEAry[t-1].ArrivalCumulativeFlow + incount;
					pLink->m_LinkMOEAry[t].DepartureCumulativeFlow = pLink->m_LinkMOEAry[t-1].DepartureCumulativeFlow + outflow_count;
					}


					pLink->m_LinkMOEAry[t].CO2 = CO2;
					pLink->m_LinkMOEAry[t].NOX = NOX;
					pLink->m_LinkMOEAry[t].CO = CO;
					pLink->m_LinkMOEAry[t].CO2 = CO2;
					pLink->m_LinkMOEAry[t].HC = HC;
				
				}


				i++;
			}else
			{
				CString msg;
				msg.Format ("Please check if link %s at file %s still exists in input_link.csv.", link_key.c_str (), lpszFileName);  // +2 for the first field name line
				AfxMessageBox(msg);
				break;
			}

		}

		m_bSimulationDataLoaded = true;

		g_Simulation_Time_Stamp = 0; // reset starting time
		g_SimulationStartTime_in_min = 0;

		m_SimulationLinkTDMOEDataLoadingStatus.Format ("%d link model MOE records are loaded from file %s.",i,lpszFileName);
	}
}
bool CTLiteDoc::ReadSimulationLinkMOEData_Bin(LPCTSTR lpszFileName)
{

	typedef struct 
	{
		int from_node_id;
		int to_node_id;
		int timestamp_in_min;
		float travel_time_in_min;
		float delay_in_min;
		float link_volume_in_veh_per_hour_per_lane;
		float link_volume_in_veh_per_hour_for_all_lanes;
		float density_in_veh_per_mile_per_lane;
		float speed_in_mph;
		float exit_queue_length;
		int cumulative_arrival_count;
		int cumulative_departure_count;

		int time_dependent_left_arrival_count;
		int time_dependent_left_departure_count;
		int number_of_queued_vehicles;
		int number_of_left_queued_vehicles;
		
		int cumulative_SOV_revenue;
		int cumulative_HOV_revenue;
		int cumulative_truck_revenue;
		int day_no;
		float Energy;		
		float CO2;
		float NOX;
		float CO;
		float HC;

	} struct_TDMOE;

	int i= 0;
	FILE* pFile;


	fopen_s(&pFile,lpszFileName,"rb");
	if(pFile!=NULL)
	{

		fseek(pFile, 0, SEEK_END );
		int Length = ftell(pFile);
		fclose(pFile);
		float LengthinMB= Length*1.0/1024/1024;
		if(LengthinMB>100)
		{
			CString msg;
			msg.Format("The time-dependent link MOE file is %5.1f MB in size.\nIt could take quite a while to load this file.\nWould you like to load this file?",LengthinMB);
			if(AfxMessageBox(msg,MB_YESNO|MB_ICONINFORMATION)==IDNO)
				return false;
		}
		fclose(pFile);
	}

	int error_count = 0;
	/////////////////
	fopen_s(&pFile,lpszFileName,"rb");
	if(pFile!=NULL)
	{

		m_SimulationStartTime_in_min = 1440;
		m_SimulationEndTime_in_min = 0;


		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->ResetMOEAry(g_Simulation_Time_Horizon);  // use one day horizon as the default value
		}

		int prev_from_node_number = -1;
		int prev_to_node_number = -1;
		DTALink* pLink = NULL;
		while(!feof(pFile))
		{
			struct_TDMOE element;
			size_t result = fread(&element,sizeof(element),1,pFile);
			if(result!= 1)
				break;

			int from_node_number = element.from_node_id;
			int to_node_number = element.to_node_id;


			element.day_no = 0;
			g_SimulatedDayDataMap[ element.day_no] = true;

			g_SimulatedLastDayNo= max(g_SimulatedLastDayNo,  element.day_no);
			g_SimulationDayNo = g_SimulatedLastDayNo;



			int t = element.timestamp_in_min + element.day_no *1440;

			if(t < m_SimulationStartTime_in_min)
				m_SimulationStartTime_in_min =t;


			if(t > m_SimulationEndTime_in_min)
				m_SimulationEndTime_in_min =t;

			m_SimulationEndTime_in_min = min (m_SimulationEndTime_in_min,1440);

			if (pLink == NULL || from_node_number != prev_from_node_number || to_node_number != prev_to_node_number)
			{
				bool bWarningFlag = true;

				if (error_count >= 1)
				{
					bWarningFlag = false;
				}

				pLink = FindLinkWithNodeNumbers(from_node_number, to_node_number, lpszFileName, bWarningFlag);
			}
			if(pLink!=NULL  )
			{	

				prev_from_node_number = from_node_number;
				prev_to_node_number = to_node_number;


				if (t <= g_Simulation_Time_Horizon - 1 && t<pLink->m_LinkMOEArySize )
				{

				//travel_time_in_min, delay_in_min, link_volume_in_veh, link_volume_in_vehphpl,
				//density_in_veh_per_mile_per_lane, speed_in_mph, queue_length_in_, cumulative_arrival_count, cumulative_departure_count
				pLink->m_LinkMOEAry[t].TravelTime = element.travel_time_in_min;
				pLink->m_LinkMOEAry[t].LinkFlow = max(0, element.link_volume_in_veh_per_hour_for_all_lanes);

				if (pLink->m_LinkMOEAry[t].LinkFlow <= -1)
				{
					AfxMessageBox("Negative link count error!");
					break;
				
				}
				pLink->m_LinkMOEAry[t].Density  = element.density_in_veh_per_mile_per_lane;

				pLink->m_LinkMOEAry[t].Speed = element.speed_in_mph;
				pLink->m_LinkMOEAry[t].QueueLength = element.exit_queue_length;

				pLink->m_LinkMOEAry[t].ArrivalCumulativeFlow = element.cumulative_arrival_count;
				pLink->m_LinkMOEAry[t].DepartureCumulativeFlow = element.cumulative_departure_count;

				//pLink->m_LinkMOEAry[t].time_dependent_left_arrival_count = element.time_dependent_left_arrival_count;
				//pLink->m_LinkMOEAry[t].time_dependent_left_departure_count = element.time_dependent_left_departure_count;
				pLink->m_LinkMOEAry[t].number_of_queued_vehicles = element.number_of_queued_vehicles;
				//pLink->m_LinkMOEAry[t].number_of_left_queued_vehicles = element.number_of_left_queued_vehicles;


				pLink->m_LinkMOEAry[t].Energy = min(60,element.Energy);

				pLink->m_LinkMOEAry[t].CO2 = element.CO2;
				pLink->m_LinkMOEAry[t].NOX = element.NOX;
				pLink->m_LinkMOEAry[t].CO = element.CO;
				pLink->m_LinkMOEAry[t].HC = element.HC;
				i++;
				}

			}else
			{
				//CString msg;
				//msg.Format ("Please check if link %d->%d at file %s still exists in input_link.csv.", from_node_number , to_node_number, lpszFileName);  // +2 for the first field name line
				//AfxMessageBox(msg);
				error_count++;
				continue;
			}




		}


		fclose(pFile);

		m_bSimulationDataLoaded = true;

		g_Simulation_Time_Stamp = 0; // reset starting time
		g_SimulationStartTime_in_min = 0;


		m_SimulationLinkTDMOEDataLoadingStatus.Format ("%d link time-dependent MOE records are loaded from file %s.",i,lpszFileName);
		return true;
	}
	return true;
}

bool CTLiteDoc::ReadSimulationLinkMOEData_SimpleBin(LPCTSTR lpszFileName)
{

	typedef struct
	{
		int version_no;
		int emission_data;
		int revenue_data;
		int turn_volume_data;
		int simulation_starttime_in_min;
		int simulation_endtime_in_min;
		int aggregation_time_interval;
		int number_of_links;
		int reserve_data1;
		int reserve_data2;
		int reserve_data3;
		int reserve_data4;
		int reserve_data5;


	} struct_TDSimpleMOE_HEADER;


	typedef struct
	{
		int from_node_id;
		int to_node_id;
		int number_of_data_elements;
		int day_no;
	} struct_TDLink_HEADER;

	typedef struct
	{

		int timestamp_in_min;
		int aggregation_time_interval; // new data 
		float travel_time_in_min;
		float link_volume_in_veh_per_hour_for_all_lanes;
		float density_in_veh_per_mile_per_lane;
		float speed_in_mph;
		float exit_queue_length;
		int cumulative_arrival_count;
		int cumulative_departure_count;

	} struct_TDSimpleMOE;

	int count = 0;
	FILE* pFile;


	fopen_s(&pFile, lpszFileName, "rb");
	if (pFile != NULL)
	{

		fseek(pFile, 0, SEEK_END);
		int Length = ftell(pFile);
		fclose(pFile);
		float LengthinMB = Length*1.0 / 1024 / 1024;
		if (LengthinMB>100)
		{
			CString msg;
			msg.Format("The time-dependent link MOE file is %5.1f MB in size.\nIt could take quite a while to load this file.\nWould you like to load this file?", LengthinMB);
			if (AfxMessageBox(msg, MB_YESNO | MB_ICONINFORMATION) == IDNO)
				return false;
		}
		fclose(pFile);
	}

	/////////////////
	fopen_s(&pFile, lpszFileName, "rb");
	if (pFile != NULL)
	{
		// level 1: header
		struct_TDSimpleMOE_HEADER header;

		size_t result = fread(&header, sizeof(struct_TDSimpleMOE_HEADER), 1, pFile);
		if (result != 1)
		{
		
			fclose(pFile);
			return true;
		};



		m_SimulationStartTime_in_min = header.simulation_starttime_in_min;
		m_SimulationEndTime_in_min = header.simulation_endtime_in_min  ;


		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->ResetMOEAry(header.simulation_endtime_in_min);  // use one day horizon as the default value
		}
		CString str_loading;


		while (!feof(pFile))
		{
			if (count % 1000 == 0)
			{
				str_loading.Format("loading %.0f%% of %d links", count*100 / max(1,m_LinkSet.size()), m_LinkSet.size());
				SetStatusText(str_loading);
			}
			// level 2: link
			struct_TDLink_HEADER link_header;
			size_t result_link = fread(&link_header, sizeof(link_header), 1, pFile);

			int from_node_number = link_header.from_node_id;
			int to_node_number = link_header.to_node_id;

			DTALink* pLink = FindLinkWithNodeNumbers(from_node_number, to_node_number, lpszFileName);
			if (pLink == NULL)
				break;

			link_header.day_no = 0;

			g_SimulatedDayDataMap[link_header.day_no ] = true;
			
			for (int i = 0; i < link_header.number_of_data_elements; i++)
			{
				count++;
	
			struct_TDSimpleMOE element;
			size_t result = fread(&element, sizeof(element), 1, pFile);
			if (result != 1)
				break;

	
			g_SimulatedLastDayNo = 0;
			g_SimulationDayNo = g_SimulatedLastDayNo;

			int t_start =min(0,element.timestamp_in_min);

			if (t_start < m_SimulationStartTime_in_min)
				m_SimulationStartTime_in_min = t_start;


			if (t_start + header.aggregation_time_interval> m_SimulationEndTime_in_min)
				m_SimulationEndTime_in_min = header.aggregation_time_interval;

				m_SimulationEndTime_in_min = min(m_SimulationEndTime_in_min, 1440);
				// level 3
				for (int t = t_start; t < t_start + element.aggregation_time_interval; t++)
				{

					if (t < pLink->m_LinkMOEArySize)
					{
						pLink->m_LinkMOEAry[t].TravelTime = element.travel_time_in_min;
						pLink->m_LinkMOEAry[t].LinkFlow = max(0, element.link_volume_in_veh_per_hour_for_all_lanes);

						if (element.link_volume_in_veh_per_hour_for_all_lanes <= -1)
						{
							AfxMessageBox("Negative link count error!");
							fclose(pFile);
							return true;
						}


						pLink->m_LinkMOEAry[t].Density = element.density_in_veh_per_mile_per_lane;
						pLink->m_LinkMOEAry[t].Speed = element.speed_in_mph;
						pLink->m_LinkMOEAry[t].QueueLength = element.exit_queue_length;
						pLink->m_LinkMOEAry[t].ArrivalCumulativeFlow = element.cumulative_arrival_count;
						pLink->m_LinkMOEAry[t].DepartureCumulativeFlow = element.cumulative_departure_count;
					}
					t++;
				}

			}
			

		}


		fclose(pFile);

		m_bSimulationDataLoaded = true;

		g_Simulation_Time_Stamp = 0; // reset starting time
		g_SimulationStartTime_in_min = 0;


		m_SimulationLinkTDMOEDataLoadingStatus.Format("%d link time-dependent MOE records are loaded from file %s.", count, lpszFileName);
		return true;
	}
	return false;
}

void CTLiteDoc::ReadSimulationLinkMOEData(LPCTSTR lpszFileName)
{

}

bool CTLiteDoc::ReadSimulationLinkOvarvallMOEData(LPCTSTR lpszFileName)
{
	double total_number_of_crashes = 0;

	CCSVParser parser;
	int i= 0;
	if (parser.OpenCSVFile(lpszFileName))
	{

		while(parser.ReadRecord())
		{

			int from_node_number;
			if(parser.GetValueByFieldName("from_node_id",from_node_number) == false)
				break;
			int to_node_number ;

			if(parser.GetValueByFieldName("to_node_id",to_node_number) == false)
				break;



			DTALink* pLink = FindLinkWithNodeNumbers(from_node_number , to_node_number, lpszFileName );
			// add reading log


			if(pLink!=NULL)
			{

				if(!parser.GetValueByFieldName("speed",pLink->m_avg_simulated_speed ))
				{
					parser.GetValueByFieldName("speed_in_mph",pLink->m_avg_simulated_speed );
				}
				parser.GetValueByFieldName("total_link_volume",pLink->m_total_link_volume );
				parser.GetValueByFieldName("volume_over_capacity_ratio",pLink->m_volume_over_capacity_ratio );
				parser.GetValueByFieldName("level_of_service",pLink->m_LevelOfService);

				parser.GetValueByFieldName("avg_waiting_time_on_loading_buffer_(min)",pLink->m_avg_waiting_time_on_loading_buffer);
				parser.GetValueByFieldName("sensor_link_volume",pLink->m_total_sensor_link_volume);
				parser.GetValueByFieldName("measurement_error",pLink->m_total_link_count_error);
				//				parser.GetValueByFieldName("simulated_AADT",pLink->m_simulated_AADT);

				// we have to re-calculate this value as the users might change the AADT conversion factor in input_link.csv
				pLink->m_simulated_AADT = pLink->m_total_link_volume / max(0.01,pLink->m_AADT_conversion_factor);
				i++;
			}else
			{
				//reading error;
				return false;
			}


		}
		m_SimulationLinkMOEDataLoadingStatus.Format ("%d link records are loaded from file %s.",i,lpszFileName);
	}else
	{
		//	AfxMessageBox("Error: File output_LinkMOE.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
	}

	return true;
}

void CTLiteDoc::ReadHistoricalData(CString directory)
{
}



BOOL CTLiteDoc::OnOpenTrafficNetworkDocument(CString ProjectFileName, bool bNetworkOnly, bool bImportShapeFiles)
{


	m_NodeDisplaySize = 50;  // in feet


	CTime LoadingStartTime = CTime::GetCurrentTime();

	m_bLoadNetworkDataOnly = bNetworkOnly;
	FILE* st = NULL;
	//	cout << "Reading file node.csv..."<< endl;

	CString directory;
	m_ProjectFile = ProjectFileName;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

	m_ProjectDirectory = directory;
	m_ProjectTitle = GetWorkspaceTitleName(ProjectFileName);
	SetTitle(m_ProjectTitle);

	//CopyDefaultFiles();

	ReadModelAgentTrajectory(m_ProjectDirectory + "model_trajectory.csv");

	ReadGPSTrajectory(m_ProjectDirectory + "CAR_GPS_data.csv");

	//CString file_name;
	//for (int day = 1; day < 31; day += 2)
	//{
	//	file_name.Format("CAR_GPS_10.%02d - 10.%02d.csv", day, day + 1);
	//	ReadGPSTrajectory(m_ProjectDirectory + file_name);
	//}

	// read users' prespecified control type
	ReadNodeControlTypeCSVFile(directory+"input_node_control_type.csv");
	ReadLinkTypeCSVFile(directory+"input_link_type.csv");

	if(bNetworkOnly ==false)
	{  // we need to check the data consistency here
		ReadDemandTypeCSVFile(directory+"input_demand_type.csv");
	}

	CWaitCursor wc;
	OpenWarningLogFile(directory);

	ClearNetworkData();
	m_ODSize = 0;

	// test if input_node.csv can be opened.
	/*	ifstream input_node_file(directory+"input_node.csv");

	*/


	//m_bUseMileVsKMFlag = (bool)(g_GetPrivateProfileDouble("coordinate_info", "mile_as_unit_of_length", 1, ProjectFileName));

	m_bRightHandTrafficFlag = g_GetPrivateProfileDouble("coordinate_info", "right_hand_traffic_flag", 1, ProjectFileName);

	if (m_bRightHandTrafficFlag == 0)
		m_bRightHandTrafficFlag = -1;
	ReadBackgroundImageFile(ProjectFileName);


	if (ReadNodeCSVFile(directory + "input_node.csv") || m_BackgroundBitmapLoaded == true)
	{
	
	if(!ReadLinkCSVFile(directory+"input_link.csv",false,false)) 
		return false;
	}

	if (ReadNodeCSVFile(directory + "input_node_2.csv",1))
	{

		if (!ReadLinkCSVFile(directory + "input_link_2.csv", false, 1))
			return false;

		if (!ReadLinkCSVFile(directory + "input_mapping_link.csv", false, 2))
			return false;
	}

	ReadReferenceLineCSVFile(directory + "optional_reference_line.csv" );
	if(ReadZoneCSVFile(directory+"input_zone.csv"))
	{
		ReadActivityLocationCSVFile(directory+"input_activity_location.csv");
	}

	CalculateZoneCapacity();
	//ConstructMovementVector();
	ConstructMovementVector1();
	ReadAMSMovementCSVFile(directory + "input_movement.csv", -1);
	
	// jun added 11/29/18
	// this should be done before ReadAMSSignalControlCSVFile()
	ReadOutputMovementMOECSVFile(directory + "output_MovementMOE.csv");
	// end 

	ReadAMSSignalControlCSVFile(directory + "input_timing.csv");
	//// try to read DSP control file, if it exists
	//ReadDYNASMART_ControlFile_ForAMSHub();



	// here load the simulation output, where include the output_linkMOE
	if (bNetworkOnly == false)
	{
		LoadSimulationOutput();
	}

	CalculateDrawingRectangle(false);
	m_bFitNetworkInitialized  = false;


	CTime LoadingEndTime = CTime::GetCurrentTime();

	CTimeSpan ts = LoadingEndTime  - LoadingStartTime;
	CString str_running_time;

	str_running_time.Format ("Network loading time: %d min(s) %d sec(s)...",ts.GetMinutes(), ts.GetSeconds());

	SetStatusText(str_running_time);



	//	OffsetLink(); we do not need to offset here as the importing function has done so. 
	ReadScenarioData();

	//: comment out now, it uses alternative format	

	//	ReadHistoricalData(directory);


	//ReadObservationLinkVolumeData(directory+"input_static_obs_link_volume.csv");

	ReadBackgroundImageFile(ProjectFileName);
	ReadSubareaCSVFile(directory+"optional_subarea.csv");


	m_AMSLogFile.close();
	return true;
}
BOOL CTLiteDoc::OnOpenRailNetworkDocument(LPCTSTR lpszPathName)
{
	FILE* st = NULL;
	//	cout << "Reading file node.csv..."<< endl;

	CString directory;
	m_ProjectFile = lpszPathName;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);


	m_ProjectDirectory = directory;
	m_ProjectTitle = GetWorkspaceTitleName(lpszPathName);
	SetTitle(m_ProjectTitle);

	CWaitCursor wc;

	OpenWarningLogFile(directory);

	if(!ReadNodeCSVFile(directory+"input_node.csv")) return false;
	if(!ReadLinkCSVFile(directory+"input_link.csv",false,false)) return false;

	ReadScenarioData();

	CalculateDrawingRectangle();

	m_AdjLinkSize +=2;  // add two more elements to be safe

	std::list<DTANode*>::iterator iNode;
	//adjust XY coordinates if the corrdinate system is not consistenty
	if(fabs(m_UnitMile-1.00)>0.10)  // ask users if we need to adjust the XY coordinates
	{
		if(AfxMessageBox("The link length information in link.csv is not consistent with the X/Y coordinates in node.csv.\nDo you want to adjust the the X/Y coordinates in node.csv?", MB_YESNO) == IDYES)

			for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
			{
				(*iNode)->pt.x = ((*iNode)->pt.x - min(m_NetworkRect.left,m_NetworkRect.right))*m_UnitMile;
				(*iNode)->pt.y = ((*iNode)->pt.y - min(m_NetworkRect.top, m_NetworkRect.bottom))*m_UnitMile;
			}

			std::list<DTALink*>::iterator iLink;
			for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			{
				(*iLink)->m_FromPoint = m_NodeNoMap[(*iLink)->m_FromNodeID]->pt;
				(*iLink)->m_ToPoint = m_NodeNoMap[(*iLink)->m_ToNodeID]->pt;
			}

			//			m_UnitMile  = 1.0;
			//			m_UnitFeet = 1/5280.0;
			CalculateDrawingRectangle();

			UpdateAllViews(0);

	}

	if(m_bLinkToBeShifted)
	{

		std::list<DTALink*>::iterator iLink;

		double minimum_link_length = 999999;
		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			if((*iLink) -> m_bToBeShifted)
			{
				if((*iLink)->m_Length <  minimum_link_length)
					minimum_link_length = (*iLink)->m_Length ;
			}
		}
		double link_offset = max(minimum_link_length*0.025*m_UnitMile, m_UnitFeet*m_OffsetInFeet);  // 80 feet


		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			if((*iLink) -> m_bToBeShifted)
			{
				double DeltaX = (*iLink)->m_ToPoint.x - (*iLink)->m_FromPoint.x ;
				double DeltaY = (*iLink)->m_ToPoint.y - (*iLink)->m_FromPoint.y ;
				double theta = atan2(DeltaY, DeltaX);

				(*iLink)->m_FromPoint.x += link_offset* cos(theta-PI/2.0f);
				(*iLink)->m_ToPoint.x += link_offset* cos(theta-PI/2.0f);

				(*iLink)->m_FromPoint.y += link_offset* sin(theta-PI/2.0f);
				(*iLink)->m_ToPoint.y += link_offset* sin(theta-PI/2.0f);
			}
		}
	}

	// for train timetabling
	ReadTrainProfileCSVFile(directory+"input_train_link_running_time.csv");
	ReadTimetableCVSFile(directory+"output_timetable.csv");

	return true;
}

BOOL CTLiteDoc::OnOpenDocument(CString ProjectFileName, bool bLoadNetworkOnly )
{
	CWaitCursor wait;

	m_NodeDisplaySize = max(1, g_GetPrivateProfileDouble("GUI", "node_display_size", 50, ProjectFileName));

	m_NodeTextDisplayRatio = max(1, g_GetPrivateProfileDouble("GUI", "node_text_display_ratio", 4, ProjectFileName));

	m_bLoadMovementData = (bool)(g_GetPrivateProfileInt("GUI", "load_movement_data", 1, ProjectFileName));


	CTime LoadingStartTime = CTime::GetCurrentTime();

	if(ProjectFileName.Find("tnp")>=0)  //Transportation network project format
	{

		OnOpenTrafficNetworkDocument(ProjectFileName,bLoadNetworkOnly);
	}else if(ProjectFileName.Find("dws")>=0)  //DYNASMART-P format
	{
		OnOpenDYNASMARTProject(ProjectFileName,bLoadNetworkOnly);
	}else if(ProjectFileName.Find("rnp")>=0)  //rail network project format
	{
		OnOpenRailNetworkDocument(ProjectFileName,bLoadNetworkOnly);

	}else
	{
		AfxMessageBox("The selected file type is not selected.");
		return false;	

	}


	CTime LoadingEndTime = CTime::GetCurrentTime();

	CTimeSpan ts = LoadingEndTime  - LoadingStartTime;

	m_StrLoadingTime.Format ("Overall loading time: %d min(s) %d sec(s)...",ts.GetMinutes(), ts.GetSeconds());

	CDlgFileLoading dlg;
	dlg.m_pDoc = this;
	dlg.DoModal ();

	UpdateAllViews(0);
	return true;

}

bool CTLiteDoc::ReadBackgroundImageFile(LPCTSTR lpszFileName, bool bAskForInput)
{
	//read impage file Background.bmp

	if(m_BackgroundBitmapLoaded)
		m_BackgroundBitmap.Detach ();

	m_BackgroundBitmap.Load(m_ProjectDirectory + "background_image.bmp");

	m_BackgroundBitmapLoaded = !(m_BackgroundBitmap.IsNull ());
	//	m_BackgroundBitmapLoaded = true;

	m_ImageXResolution = 1;
	m_ImageYResolution = 1;


	if(m_BackgroundBitmapLoaded)
	{

		if(bAskForInput)
		{

			m_ImageX1 = 0;
			m_ImageY1 = 0;
			m_ImageX2 = 20;
			m_ImageY2 = 10;

			m_OriginOnBottomFlag = 1;
		}else
		{
			m_OriginOnBottomFlag = (int)(g_GetPrivateProfileDouble("coordinate_info", "origin_on_bottom_flag", m_OriginOnBottomFlag, lpszFileName));
			m_ImageX1 = g_GetPrivateProfileDouble("background_image_coordinate_info", "left", m_NetworkRect.left, lpszFileName);
			m_ImageY1 = g_GetPrivateProfileDouble("background_image_coordinate_info", "bottom", m_NetworkRect.bottom, lpszFileName);
			m_ImageX2 = g_GetPrivateProfileDouble("background_image_coordinate_info", "right", m_NetworkRect.right, lpszFileName);
			m_ImageY2 = g_GetPrivateProfileDouble("background_image_coordinate_info", "top", m_NetworkRect.top, lpszFileName);
			m_ImageWidthInMile = g_GetPrivateProfileDouble("background_image_coordinate_info", "width",1, lpszFileName);

			if(m_LinkSet.size()==0)  // no link yet, need to initialize the map unit
			{

				//m_UnitMile =fabs(m_ImageX2-m_ImageX1)/ max(0.00001,m_ImageWidthInMile);
				//m_UnitFeet = m_UnitMile/5280;
			}

		}

		m_ImageWidth = fabs(m_ImageX2 - m_ImageX1);
		m_ImageHeight = fabs(m_ImageY2 - m_ImageY1);

		m_ImageMoveSize = m_ImageWidth/2000.0f;

		m_BackgroundImageFileLoadingStatus.Format ("Optional background image file is loaded.");

	}
	else
		m_BackgroundImageFileLoadingStatus.Format ("Optional background image file is not loaded.");

	return m_BackgroundBitmapLoaded;
}
// CTLiteDoc commands

void CTLiteDoc::OnFileOpen()
{

	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_LONGNAMES|OFN_ENABLESIZING,
		_T("Transportation Network Project (*.tnp)|*.tnp|"),NULL,0,true);
	if(dlg.DoModal() == IDOK)
	{
		OnOpenDocument(dlg.GetPathName());
	}
}

void CTLiteDoc::OnFileSaveimagelocation()
{

	TCHAR IniFilePath[_MAX_PATH];
	sprintf_s(IniFilePath,"%s", m_ProjectFile);

	char lpbuffer[64];

	sprintf_s(lpbuffer,"%f",m_ImageX1);
	WritePrivateProfileString("background_image_coordinate_info","left",lpbuffer,IniFilePath);
	sprintf_s(lpbuffer,"%f",m_ImageY1);
	WritePrivateProfileString("background_image_coordinate_info","bottom",lpbuffer,IniFilePath);

	sprintf_s(lpbuffer,"%f",m_ImageX2);
	WritePrivateProfileString("background_image_coordinate_info","right",lpbuffer,IniFilePath);
	sprintf_s(lpbuffer,"%f",m_ImageY2);
	WritePrivateProfileString("background_image_coordinate_info","top",lpbuffer,IniFilePath);

	sprintf_s(lpbuffer,"%f",m_ImageWidthInMile);
	WritePrivateProfileString("background_image_coordinate_info","width",lpbuffer,IniFilePath);


}

COLORREF CTLiteDoc::GetLinkTypeColor(int LinkType)
{
	COLORREF color;
	switch (LinkType)
	{
	case 1: color = m_ColorFreeway; break;
	case 2: color = m_ColorHighway; break;
	default: color = m_ColorArterial;

	}
	return color;
}

void CTLiteDoc::OnShowShowpathmoe()
{
	m_PathMOEDlgShowFlag = !m_PathMOEDlgShowFlag;

	if(m_PathMOEDlgShowFlag)
	{
		if(g_pPathMOEDlg==NULL)
		{
			g_pPathMOEDlg = new 

				CDlgPathMOE();

			g_pPathMOEDlg->m_pDoc = this;
			g_pPathMOEDlg->SetModelessFlag(TRUE);
			g_pPathMOEDlg->Create(IDD_DIALOG_PATHMOE);
		}

		if(g_pPathMOEDlg->GetSafeHwnd())
			g_pPathMOEDlg->ShowWindow(SW_SHOW);

	}else
	{
		if(g_pPathMOEDlg->GetSafeHwnd())
		{
			g_pPathMOEDlg->ShowWindow(SW_HIDE);
		}
	}

}

void CTLiteDoc::OnUpdateShowShowpathmoe(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(g_LinkMOEDlgShowFlag);
}



void CTLiteDoc::OnSearchListtrains()
{
	CDlgTrainInfo train_dlg;

	train_dlg.m_pDoc = this;

	train_dlg.DoModal ();


}

bool CTLiteDoc::ReadNodeControlTypeCSVFile(LPCTSTR lpszFileName)
{

	m_NodeTypeMap[0] = "unknown_control";
	m_NodeTypeMap[1] = "no_control";
	m_NodeTypeMap[2] = "yield_sign";
	m_NodeTypeMap[3] = "2way_stop_sign";
	m_NodeTypeMap[4] = "4way_stop_sign";
	m_NodeTypeMap[5] = "pretimed_signal";
	m_NodeTypeMap[6] = "actuated_signal";
	m_NodeTypeMap[7] = "roundabout";


	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{
		int control_type_code;
		int i=0;
		while(parser.ReadRecord())
		{
			control_type_code = 0;
			parser.GetValueByFieldName("unknown_control",control_type_code);
			m_NodeTypeMap[control_type_code] = "unknown_control";
			m_ControlType_UnknownControl = control_type_code;

			control_type_code = 1;
			parser.GetValueByFieldName("no_control",control_type_code);
			m_NodeTypeMap[control_type_code] = "no_control";
			m_ControlType_NoControl = control_type_code;

			control_type_code = 2;
			parser.GetValueByFieldName("yield_sign",control_type_code);
			m_NodeTypeMap[control_type_code] = "yield_sign";
			m_ControlType_YieldSign = control_type_code;

			control_type_code = 3;
			parser.GetValueByFieldName("2way_stop_sign",control_type_code);
			m_NodeTypeMap[control_type_code] = "2way_stop_sign";
			m_ControlType_2wayStopSign = control_type_code;

			control_type_code = 4;
			parser.GetValueByFieldName("4way_stop_sign",control_type_code);
			m_NodeTypeMap[control_type_code] = "4way_stop_sign";
			m_ControlType_4wayStopSign = control_type_code;

			control_type_code = 5;
			parser.GetValueByFieldName("pretimed_signal",control_type_code);
			m_NodeTypeMap[control_type_code] = "pretimed_signal";
			m_ControlType_PretimedSignal = control_type_code;

			control_type_code = 6;
			parser.GetValueByFieldName("actuated_signal",control_type_code);
			m_NodeTypeMap[control_type_code] = "actuated_signal";
			m_ControlType_ActuatedSignal = control_type_code;

			control_type_code = 7;
			parser.GetValueByFieldName("roundabout",control_type_code);
			m_NodeTypeMap[control_type_code] = "roundabout";
			m_ControlType_Roundabout = control_type_code;


			control_type_code = 100;
			parser.GetValueByFieldName("roundabout",m_ControlType_ExternalNode);
			m_NodeTypeMap[control_type_code] = "roundabout";
			m_ControlType_Roundabout = control_type_code;

			break;  // just one line
		}



		return true;
	}

	return false;
}

bool CTLiteDoc::ReadNodeCSVFile(LPCTSTR lpszFileName, int LayerNo)
{


	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{
		int i= m_NodeSet.size();
		while(parser.ReadRecord())
		{
			int node_id;
			string name;
			int CycleLengthInSecond = 0;
			DTANode* pNode = 0;

			int control_type;
			double X;
			double Y;

			node_id =1;
			if(parser.GetValueByFieldName("node_id",node_id) == false)
				break;



			//	TRACE("Node ID: %d\n", node_id);

			if(LayerNo==1) // additional layer
				node_id = node_id*(-1);


			string address_string;

			address_string = "A-001.B-002.C-01";
			
			vector<string> AddressStrings;
			vector<string> LayerStrings;
					//	if (parser.GetValueByFieldName("tp_address", address_string))
			{
				string subStr;
				istringstream ss(address_string);
				char Delimiter = '.';

				if (address_string.find_first_of('"') == string::npos)
				{
					while (std::getline(ss, subStr, Delimiter))
					{
						string subStr2;
						istringstream ss2(subStr);
						char Delimiter2 = '-';

						std::getline(ss2, subStr2, Delimiter2);
						LayerStrings.push_back(subStr2);

						std::getline(ss2, subStr2, Delimiter2);
						AddressStrings.push_back(subStr2);
					}
				}

			}



			if(!parser.GetValueByFieldName("name",name))
				name = "";

			if(!parser.GetValueByFieldName("control_type",control_type))
				control_type = 0;

			parser.GetValueByFieldName("cycle_length_in_second", CycleLengthInSecond);

			int offset_in_second = 0;
			parser.GetValueByFieldName("offset_in_second", offset_in_second);

			// use the X and Y as default values first

			bool bFieldX_Exist = parser.GetValueByFieldName("x",X,false);
			parser.GetValueByFieldName("y",Y,false);

			string geo_string;

			if(parser.GetValueByFieldName("geometry",geo_string))
			{
				// overwrite when the field "geometry" exists
				CGeometry geometry(geo_string);

				std::vector<CCoordinate> CoordinateVector = geometry.GetCoordinateList();

				if(CoordinateVector.size()>0)
				{
					X = CoordinateVector[0].X;
					Y = CoordinateVector[0].Y;
				}

			}

			if(m_NodeNumbertoNodeNoMap.size()!= 0 && m_NodeNumbertoNodeNoMap.find(node_id) != m_NodeNumbertoNodeNoMap.end())
			{
				CString error_message;
				error_message.Format ("Node %d in input_node.csv has been defined twice. Please check.", node_id);
				AfxMessageBox(error_message);
				return 0;
			}

			pNode = new DTANode;
			if(LayerNo ==0)
				pNode->m_NodeNumber = node_id;
			else
				pNode->m_NodeNumber = node_id*(-1);

			pNode->m_ControlType = control_type;
			pNode->m_SignalOffsetInSecond = offset_in_second;
			pNode->m_LayerNo = LayerNo;

			pNode->pt.x = X;
			pNode->pt.y = Y;

			pNode->m_NodeNo = i;
			pNode->m_ZoneID = 0;

			pNode->m_CycleLengthInSecond = CycleLengthInSecond;

			m_NodeSet.push_back(pNode);
			m_NodeNoMap[i] = pNode;
			m_NodeNumberMap[node_id] = pNode;
			m_NodeNotoNumberMap[i] = node_id;
			m_NodeNumbertoNodeNoMap[node_id] = i;
			i++;

			//			cout << "node = " << node << ", X= " << X << ", Y = " << Y << endl;

		}

		m_NodeDataLoadingStatus.Format ("%d nodes are loaded from file %s.",m_NodeSet.size(),lpszFileName);
		return true;
	}else
	{
		if (LayerNo == 0)
		{
			AfxMessageBox("Error: File input_node.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		}return false;
		//		g_ProgramStop();
	}

}
void CTLiteDoc::BuildGridSystem()
{

	bool bRectInitialized = false;
	m_AdjLinkSize = 0;

	for (std::list<DTANode*>::iterator iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{
		if ((*iNode)->m_LayerNo == 0)  //base line network only
		{
			if (!bRectInitialized)
			{
				m_GridRect.left = (*iNode)->pt.x;
				m_GridRect.right = (*iNode)->pt.x;
				m_GridRect.top = (*iNode)->pt.y;
				m_GridRect.bottom = (*iNode)->pt.y;
				bRectInitialized = true;
			}

			m_GridRect.Expand((*iNode)->pt);
		}
	}

	m_GridXStep = max(0.0001,m_GridRect.Width () /_MAX_TRANSIT_GRID_SIZE);

	m_GridYStep = max(0.0001,m_GridRect.Height ()/_MAX_TRANSIT_GRID_SIZE);

	int time_interval_no = 0;
	for (std::list<DTANode*>::iterator iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{
		if ((*iNode)->m_LayerNo == 0)  //base line network only
		{
			int x_key = ((*iNode)->pt.x - m_GridRect.left) / m_GridXStep;
			int y_key = ((*iNode)->pt.y - m_GridRect.bottom) / m_GridYStep;

			//feasible region
			x_key = max(0, x_key);
			x_key = min(_MAX_TRANSIT_GRID_SIZE - 1, x_key);

			y_key = max(0, y_key);
			y_key = min(_MAX_TRANSIT_GRID_SIZE - 1, y_key);

			m_GridMatrix[x_key][y_key][0].m_NodeVector.push_back((*iNode)->m_NodeNo);
			m_GridMatrix[x_key][y_key][0].m_NodeX.push_back((*iNode)->pt.x);
			m_GridMatrix[x_key][y_key][0].m_NodeY.push_back((*iNode)->pt.y);
		}
	}

	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		if ((*iLink)->m_LayerNo == 0)  //base line network only
		{
			int x_key = ((*iLink)->m_FromPoint.x  - m_GridRect.left) / m_GridXStep;
			int y_key = ((*iLink)->m_FromPoint.y - m_GridRect.bottom) / m_GridYStep;

			//feasible region
			x_key = max(0, x_key);
			x_key = min(_MAX_TRANSIT_GRID_SIZE - 1, x_key);

			y_key = max(0, y_key);
			y_key = min(_MAX_TRANSIT_GRID_SIZE - 1, y_key);

			m_GridMatrix[x_key][y_key][0].m_LinkNoVector.push_back((*iLink)->m_LinkNo);

		
			int from_x_key = x_key;
			int from_y_key = y_key;


			x_key = ((*iLink)->m_ToPoint.x - m_GridRect.left) / m_GridXStep;
			y_key = ((*iLink)->m_ToPoint.y - m_GridRect.bottom) / m_GridYStep;

			//feasible region
			x_key = max(0, x_key);
			x_key = min(_MAX_TRANSIT_GRID_SIZE - 1, x_key);

			y_key = max(0, y_key);
			y_key = min(_MAX_TRANSIT_GRID_SIZE - 1, y_key);

			if (from_x_key != x_key || from_y_key != y_key)
			{
				m_GridMatrix[x_key][y_key][0].m_LinkNoVector.push_back((*iLink)->m_LinkNo);
			}

		}
	}
}

void CTLiteDoc::PrintOutAccessibleMap(GDPoint pt, bool bAllLocations)
{

	CWaitCursor wait;

	int search_start_time = 7*60;
	int search_end_time = 8*60; 


	//if(bAllLocations)
	//{
	//search_start_time = 8*60;
	//search_end_time = 22*60;

	//}else
	//{
	//search_start_time = 8*60;
	//int search_end_time = 8*60; 

	//
	//
	//}

	FILE* pFile;
	fopen_s(&pFile,m_ProjectDirectory + "AMS_transit_accessibility.csv","w");
	if(pFile==NULL)
	{
		AfxMessageBox("File AMS_transit_accessibility.csv cannot be opened. Please check." );
		return;
	}

	fprintf(pFile,",,,,,,");  // max_accessible_transit_time_in_min= ,%d,

	for(int current_time = search_start_time; current_time <search_end_time; current_time +=15)   //every 15 min current_time is the time for starting to board the bus
	{
		fprintf(pFile,"min %d,", current_time);

	}

	fprintf(pFile,"\n");
	fprintf(pFile,",,,,,,");  // max_accessible_transit_time_in_min= ,%d,

	for(int current_time = search_start_time; current_time < search_end_time; current_time +=15)   //every 15 min current_time is the time for starting to board the bus
	{
		fprintf(pFile,"%s,", GetTimeStampString24HourFormat (current_time));

	}


	fprintf(pFile, ",");
	for(int current_time = search_start_time; current_time <search_end_time; current_time +=15)   //every 15 min current_time is the time for starting to board the bus
	{
		fprintf(pFile,"%s,", GetTimeStampString24HourFormat (current_time));

	}

	fprintf(pFile,"\n");



	std::map<int, DTAZone>	:: iterator itr;

	for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); ++itr)
	{
		std::map<int, int > UsedActivityLocationMap;

		for(int i = 0; i< itr->second.m_ActivityLocationVector .size(); i++)
		{
			DTAActivityLocation element = itr->second.m_ActivityLocationVector[i];

			if(bAllLocations)
				pt = element.pt;
			else
			{
				// use pt from function input
			}


			if(element.External_OD_flag != 0)  // external zones
				continue;

			{
				for(double max_walking_distance = 0.05; max_walking_distance <=0.25; max_walking_distance+=0.001)
				{
					for(int max_accessible_transit_time_in_min = 45; max_accessible_transit_time_in_min<=45; max_accessible_transit_time_in_min+=5)
					{

						if(bAllLocations)
						{
							fprintf(pFile,"node number = %d,zone id = %d,walking_distance =, %5.4f, max_accessible_transit_time_in_min= ,%d,", 
								element.NodeNumber, 
								element.ZoneID, 
								max_walking_distance, 
								max_accessible_transit_time_in_min);
						}else
						{
							fprintf(pFile,"single point,,walking_distance =, %5.2f, max_accessible_transit_time_in_min= ,%d,", 

								max_walking_distance, 
								max_accessible_transit_time_in_min);

						}

						// 
						for(int current_time = search_start_time; current_time < search_end_time; current_time +=15)   //every 15 min current_time is the time for starting to board the bus
						{

							// stage I: build m_AccessibleTrip


							int stop_time_record_count  = 0; 

							m_AccessibleTripIDMap.clear();

							int time_interval_size = 15; // min
							int time_interval_no = current_time /time_interval_size;  // 15 min


							std::map<int, PT_Trip>::iterator iPT_TripMap;
							for ( iPT_TripMap= m_PT_network.m_PT_TripMap.begin() ; iPT_TripMap != m_PT_network.m_PT_TripMap.end(); iPT_TripMap++ )
							{

								for(int si = 0; si < (*iPT_TripMap).second .m_PT_StopTimeVector.size(); si++)
								{

									int station_time_interval_no = (*iPT_TripMap).second .m_PT_StopTimeVector[si].arrival_time/time_interval_size; // 30 min


									if(station_time_interval_no == time_interval_no)
									{


										double distance_in_mile = g_CalculateP2PDistanceInMileFromLatitudeLongitude(pt, (*iPT_TripMap).second .m_PT_StopTimeVector[si].pt);

										if(distance_in_mile < max_walking_distance) // default: 0.25 miles
										{
											m_AccessibleTripIDMap[(*iPT_TripMap).first ]= 1;
											break;
										}
									}
								}
							}


							//////////////////////////////////////
							// stage II: count number of stop time records;

							for ( iPT_TripMap= m_PT_network.m_PT_TripMap.begin() ; iPT_TripMap != m_PT_network.m_PT_TripMap.end(); iPT_TripMap++ )
							{


								if(m_AccessibleTripIDMap.find((*iPT_TripMap).second.trip_id  )== m_AccessibleTripIDMap.end())
								{
									// the trip is not included in origin's accessible list
									continue;
								}


								CPoint ScreenPoint, Prev_ScreenPoint;

								bool ini_flag = true;

								for(int si = 0; si < (*iPT_TripMap).second .m_PT_StopTimeVector.size(); si++)
								{

									int stop_time_in_min = (*iPT_TripMap).second .m_PT_StopTimeVector[si].departure_time ;



									if(stop_time_in_min>= current_time && stop_time_in_min< current_time +  max_accessible_transit_time_in_min)
									{

										GDPoint shape_point = (*iPT_TripMap).second .m_PT_StopTimeVector[si].pt ;
										stop_time_record_count++;

									}

								}

							}     // end of stage II

							fprintf(pFile,"%d,", stop_time_record_count);



						}  // max_accessible_transit_time_in_min


						///////////////////////////////////////////////////
						fprintf(pFile, "|# of accessible transfer stop times|,"); 

						bool b_accessible_transfer_station = false;
						if(b_accessible_transfer_station)
						{
							for(int current_time = search_start_time; current_time <search_end_time; current_time +=15)   //every 15 min current_time is the time for starting to board the bus
							{

								// stage I: build m_AccessibleTrip


								int stop_time_record_count  = 0; 

								m_AccessibleTripIDMap.clear();

								int time_interval_size = 15; // min
								int time_interval_no = current_time /time_interval_size;  // 15 min


								std::map<int, PT_Trip>::iterator iPT_TripMap;
								for ( iPT_TripMap= m_PT_network.m_PT_TripMap.begin() ; iPT_TripMap != m_PT_network.m_PT_TripMap.end(); iPT_TripMap++ )
								{


									for(int si = 0; si < (*iPT_TripMap).second .m_PT_StopTimeVector.size(); si++)
									{

										int station_time_interval_no = (*iPT_TripMap).second .m_PT_StopTimeVector[si].arrival_time/time_interval_size; // 30 min


										if(station_time_interval_no == time_interval_no)
										{

											double distance_in_mile = g_CalculateP2PDistanceInMileFromLatitudeLongitude(pt, (*iPT_TripMap).second .m_PT_StopTimeVector[si].pt);

											if(distance_in_mile < max_walking_distance) // default: 0.25 miles
											{
												m_AccessibleTripIDMap[(*iPT_TripMap).first ]= 1;
												break;
											}
										}
									}
								}


								//////////////////////////////////////
								// stage II: count number of stop time records;

								int transfer_stop_time_count = 0;


								for ( iPT_TripMap= m_PT_network.m_PT_TripMap.begin() ; iPT_TripMap != m_PT_network.m_PT_TripMap.end(); iPT_TripMap++ )
								{


									if(m_AccessibleTripIDMap.find((*iPT_TripMap).second.trip_id  )== m_AccessibleTripIDMap.end())
									{
										// the trip is not included in origin's accessible list
										continue;
									}

									if((*iPT_TripMap).second.m_PT_StopTimeVector.size() <=2)
										break;

									int start_time = (*iPT_TripMap).second.m_PT_StopTimeVector[0].departure_time; 
									int end_time =  (*iPT_TripMap).second.m_PT_StopTimeVector[(*iPT_TripMap).second.m_PT_StopTimeVector.size()-1].departure_time ;

									CPoint ScreenPoint, Prev_ScreenPoint;

									bool ini_flag = true;

									for(int si = 0; si < (*iPT_TripMap).second .m_PT_StopTimeVector.size(); si++)
									{

										int stop_time_in_min = (*iPT_TripMap).second .m_PT_StopTimeVector[si].departure_time ;

										if(stop_time_in_min>= current_time && stop_time_in_min< current_time +  max_accessible_transit_time_in_min)
										{

											PT_StopTime first = (*iPT_TripMap).second .m_PT_StopTimeVector[si];


											std::map<int, PT_Trip>::iterator iPT_TripMap2;
											for ( iPT_TripMap2= m_PT_network.m_PT_TripMap.begin() ; iPT_TripMap2 != m_PT_network.m_PT_TripMap.end(); iPT_TripMap2++ )
											{

												if((*iPT_TripMap2).second.m_PT_StopTimeVector.size() <=2)
													break;

												int start_time2 = (*iPT_TripMap2).second.m_PT_StopTimeVector[0].departure_time ;
												int end_time2 =  (*iPT_TripMap2).second.m_PT_StopTimeVector[(*iPT_TripMap2).second.m_PT_StopTimeVector.size()-1].departure_time ;
												if(start_time2 > end_time || end_time2 < start_time )
													break;

												for(int sj = 0; sj < (*iPT_TripMap2).second .m_PT_StopTimeVector.size(); sj++)
												{
													PT_StopTime second =  (*iPT_TripMap2).second.m_PT_StopTimeVector[sj];  // global vector of stop times

													if( 
														m_PT_network.m_PT_TripMap [first.trip_id ].route_id  == 37175 
														&&  m_PT_network.m_PT_TripMap [second .trip_id].route_id ==37163)
													{

														if(first. stop_id == 6587 && second.stop_id == 16374)
														{
															TRACE("station");
														}
														/* not the same route id*/
														if(first.departure_time >=  (second .departure_time - 60 ) /* 15 min waiting time*/
															&& first .departure_time <   second .departure_time )
														{ 

															double transfer_walking_distance  = 
																g_CalculateP2PDistanceInMileFromLatitudeLongitude(first .pt  , second .pt  );

															TRACE("dist = %f\n",transfer_walking_distance);

															if(transfer_walking_distance <= 0.25 )
															{
																transfer_stop_time_count ++;

																break; // break from this trip loop
															}


														}
													}  // /* not the same route id*/
												}


											}

										}





									}

								}     // end of stage II

								fprintf(pFile,"%d,", transfer_stop_time_count);


							}
						}



						fprintf(pFile,"\n");

					}  // current time 
				} // walking distance
			} 

		} // origin activity location

		if(bAllLocations == false)
			break;
	}  // zone
	fclose(pFile);
}


void CTLiteDoc::FindAccessibleTripID(GDPoint pt, int timestamp_in_min)
{
	m_AccessibleTripIDMap.clear();

	int time_interval_size = 15; // min
	int time_interval_no = timestamp_in_min/time_interval_size;  // 15 min


	std::map<int, PT_Trip>::iterator iPT_TripMap;
	for ( iPT_TripMap= m_PT_network.m_PT_TripMap.begin() ; iPT_TripMap != m_PT_network.m_PT_TripMap.end(); iPT_TripMap++ )
	{

		for(int si = 0; si < (*iPT_TripMap).second .m_PT_StopTimeVector.size(); si++)
		{

			int station_time_interval_no = (*iPT_TripMap).second .m_PT_StopTimeVector[si].arrival_time/time_interval_size; // 30 min


			if(station_time_interval_no == time_interval_no)
			{

				double distance_in_mile = g_CalculateP2PDistanceInMileFromLatitudeLongitude(pt, (*iPT_TripMap).second .m_PT_StopTimeVector[si].pt);

				if(distance_in_mile < m_max_walking_distance) // default: 0.25 miles
				{
					m_AccessibleTripIDMap[(*iPT_TripMap).first ]= 1;
					break;
				}
			}
		}
	}
}
int CTLiteDoc::FindClosestNode(double x, double y, double min_distance,  int step_size, double time_stamp_in_min)
{

	step_size = int(min_distance/m_GridXStep+1);

	int x_key = (x - m_GridRect.left)/ m_GridXStep;
	int y_key = (y -m_GridRect.bottom)/ m_GridYStep;

	//feasible region
	x_key = max(0,x_key);
	x_key = min(99,x_key);

	y_key = max(0,y_key);
	y_key = min(99,y_key);

	int NodeId = -1;


	for(int x_i = max(0,x_key- step_size); x_i <= min(99,x_key+ step_size); x_i++)
		for(int y_i = max(0,y_key-step_size); y_i <= min(99,y_key+step_size ); y_i++)
		{

			GridNodeSet element = m_GridMatrix[x_i][y_i][0];

			for(unsigned int i = 0; i < element.m_NodeVector.size(); i++)
			{

				double distance = sqrt( (x-element.m_NodeX[i])*(x-element.m_NodeX[i]) + (y-element.m_NodeY[i])*(y-element.m_NodeY[i]))/m_UnitMile;

				if(distance < min_distance)
				{

					min_distance = distance;

					NodeId =  element.m_NodeVector[i];

				}

				// this is for GPS map matching

				double distance_in_mile = distance / m_UnitMile;
				if(distance_in_mile < m_NodeNoMap[element.m_NodeVector[i]] ->m_min_distance_from_GPS_point)
				{
					m_NodeNoMap[element.m_NodeVector[i]] ->m_min_distance_from_GPS_point = distance_in_mile;
					m_NodeNoMap[element.m_NodeVector[i]] ->m_GPS_arrival_time = time_stamp_in_min;

					m_NodeNoMap[element.m_NodeVector[i]] ->m_DistanceToRoot = distance_in_mile;

				}


			}	// per node in a grid cell

		} // for nearby cell

		return NodeId;
}

int CTLiteDoc::FindClosestZone(double x, double y, double min_distance,  int step_size)
{

	step_size = int(min_distance/m_GridXStep+1);

	int x_key = (x - m_GridRect.left)/ m_GridXStep;
	int y_key = (y -m_GridRect.bottom)/ m_GridYStep;

	//feasible region
	x_key = max(0,x_key);
	x_key = min(99,x_key);

	y_key = max(0,y_key);
	y_key = min(99,y_key);

	int NodeId = -1;
	int ZoneNumber = -1;


	for(int x_i = max(0,x_key- step_size); x_i <= min(99,x_key+ step_size); x_i++)
		for(int y_i = max(0,y_key-step_size); y_i <= min(99,y_key+step_size ); y_i++)
		{

			GridNodeSet element = m_GridMatrix[x_i][y_i][0];

			for(unsigned int i = 0; i < element.m_NodeVector.size(); i++)
			{
				if( m_NodeNoMap[element.m_NodeVector[i]]->m_ZoneID >0)

				{
					double distance = sqrt( (x-element.m_NodeX[i])*(x-element.m_NodeX[i]) + (y-element.m_NodeY[i])*(y-element.m_NodeY[i]));

					if(distance < min_distance)
					{

						min_distance = distance;

						NodeId =  element.m_NodeVector[i];
						ZoneNumber  = m_NodeNoMap[NodeId]->m_ZoneID;
					}
				}



			}	// per node in a grid cell

		} // for nearby cell

		return ZoneNumber;
}


float CTLiteDoc::GetLinkBandWidth(float Value)
{ //output: m_BandWidthValue for each link
	if(m_MaxLinkWidthAsLinkVolume < 10)
		m_MaxLinkWidthAsLinkVolume = 10;

	float VolumeRatio = 1/m_MaxLinkWidthAsLinkVolume;  // 1000 vehicles flow rate as 1 lanes

	float LaneVolumeEquivalent = 200;
	float DurationInMinEquivalent = 20;

	float link_volume = 0;
	// default mode
	float BandWidthValue = 0;

	BandWidthValue =  max(0,Value*LaneVolumeEquivalent*VolumeRatio);

	if(m_LinkBandWidthMode == LBW_number_of_lanes)
	{
		BandWidthValue =  min(7,max(1,Value*LaneVolumeEquivalent*VolumeRatio));


	}else if(m_LinkBandWidthMode == LBW_link_volume)
	{
		BandWidthValue = Value*VolumeRatio; 
	}else if (m_LinkBandWidthMode == LBW_number_of_marked_vehicles)
	{
		//			pLink->m_BandWidthValue =  pLink->m_NumberOfMarkedVehicles *VolumeRatio;
	}else if (m_LinkBandWidthMode == LBW_congestion_duration)
	{
		BandWidthValue =  Value *DurationInMinEquivalent*VolumeRatio;
	}
	else
		// default value
	{
		BandWidthValue =  5*LaneVolumeEquivalent*VolumeRatio;
	}

	return BandWidthValue;

}

void CTLiteDoc::ReCalculateLinkBandWidth()
{ //output: m_BandWidthValue for each link
	std::list<DTALink*>::iterator iLink;

	if(m_MaxLinkWidthAsLinkVolume < 10)
		m_MaxLinkWidthAsLinkVolume = 10;

	float VolumeRatio = 1/m_MaxLinkWidthAsLinkVolume;  // 1000 vehicles flow rate as 1 lanes

	float max_total_volume = 1;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		if((*iLink)->m_total_link_volume > max_total_volume)
		{
			max_total_volume = (*iLink)->m_total_link_volume;
		}
	}

	float TotalVolumeRatio = VolumeRatio;

	float LaneVolumeEquivalent = 200;
	float DurationInMinEquivalent = 20;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		DTALink* pLink = (*iLink);
		if (pLink->m_FromNodeNumber == 31 && pLink->m_ToNodeNumber == 30)
			TRACE("");

		float link_volume = 0;
		// default mode
		pLink->m_BandWidthValue =  max(0,pLink->m_NumberOfLanes*LaneVolumeEquivalent*VolumeRatio);

		if(m_LinkBandWidthMode == LBW_number_of_lanes)
		{
			if(m_LinkTypeMap[pLink->m_link_type ].IsConnector ())  // 1 lane as connector
				pLink->m_BandWidthValue =  min(1,pLink->m_NumberOfLanes)*LaneVolumeEquivalent*VolumeRatio;
			else
				pLink->m_BandWidthValue =  min(7, max(1,pLink->m_NumberOfLanes)*LaneVolumeEquivalent*VolumeRatio);


		}else if(m_LinkBandWidthMode == LBW_link_volume)
		{
			if(m_LinkMOEMode == MOE_safety)  // safety
			{
				pLink->m_BandWidthValue = pLink->m_number_of_all_crashes *10*VolumeRatio;   // 10 crashes as 5 lanes. 
			}else
			{
				if(g_Simulation_Time_Stamp>=1) // dynamic traffic assignment mode
				{

					GetLinkMOE((*iLink), MOE_volume,g_Simulation_Time_Stamp, g_MOEAggregationIntervalInMin, link_volume);

					pLink->m_BandWidthValue = link_volume*VolumeRatio; 
				}else  // total volume
				{

					GetLinkMOE((*iLink), MOE_volume,m_DemandLoadingStartTimeInMin, m_DemandLoadingEndTimeInMin-m_DemandLoadingStartTimeInMin, link_volume);

					pLink->m_BandWidthValue = link_volume*VolumeRatio;
				}
			}

			if(m_LinkMOEMode == MOE_volume && pLink->m_bSensorData)  // reference volume
			{
				float sensor_volume = pLink->GetSensorLinkHourlyVolume(g_Simulation_Time_Stamp);
				GetLinkMOE((*iLink), MOE_volume,g_Simulation_Time_Stamp, g_MOEAggregationIntervalInMin, link_volume);
				pLink->m_ReferenceBandWidthValue = sensor_volume*VolumeRatio; 
			}

		}else if (m_LinkBandWidthMode == LBW_number_of_marked_vehicles)
		{
			pLink->m_BandWidthValue =  pLink->m_NumberOfMarkedVehicles *VolumeRatio;
		}else if (m_LinkBandWidthMode == LBW_congestion_duration)
		{
			pLink->m_BandWidthValue =  pLink->GetImpactDuration (g_Simulation_Time_Stamp, this->m_PrimaryDataSource ) *DurationInMinEquivalent*VolumeRatio;
		}
		else
			// default value
		{
			pLink->m_BandWidthValue =  pLink->m_NumberOfLanes*LaneVolumeEquivalent*VolumeRatio;
		}

	}

}
void CTLiteDoc::GenerateOffsetLinkBand()
{


	ReCalculateLinkBandWidth();

	std::list<DTALink*>::iterator iLink;

	m_UnitFeet = max(0.00001,m_UnitFeet);

	m_LaneWidthInFeet = 12;
	double lane_offset = m_UnitFeet*m_LaneWidthInFeet;  // 20 feet per lane

	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		(*iLink)->m_BandLeftShapePoints.clear();
		(*iLink)->m_BandRightShapePoints.clear();

		(*iLink)->m_ReferenceBandLeftShapePoints.clear();
		(*iLink)->m_ReferenceBandRightShapePoints.clear();

		if ((*iLink)->m_FromNodeNumber == 31 && (*iLink)->m_ToNodeNumber == 30)
			TRACE("");

		if((*iLink)->m_ShapePoints.size() ==0)
			continue;

		int last_shape_point_id = (*iLink) ->m_ShapePoints .size() -1;
		double DeltaX = (*iLink)->m_ShapePoints[last_shape_point_id].x - (*iLink)->m_ShapePoints[0].x;
		double DeltaY = (*iLink)->m_ShapePoints[last_shape_point_id].y - (*iLink)->m_ShapePoints[0].y;
		double theta = 0;

		if(fabs(DeltaY)>0.0000001)
			theta = atan2(DeltaY, DeltaX);


		for(unsigned int si = 0; si < (*iLink) ->m_ShapePoints .size(); si++)
		{

			// calculate theta for each feature point segment
			// longer than 0.5 miles
			if(si>= 1 && (*iLink) ->m_Length > 0.5 &&((*iLink) ->m_ShapePoints .size() >4 || m_LinkTypeMap[(*iLink)->m_link_type].IsRamp ()))  // ramp or >4 feature points
			{
				last_shape_point_id = si;
				DeltaX = (*iLink)->m_ShapePoints[last_shape_point_id].x - (*iLink)->m_ShapePoints[si-1].x;
				DeltaY = (*iLink)->m_ShapePoints[last_shape_point_id].y - (*iLink)->m_ShapePoints[si-1].y;

				if(fabs(DeltaY)>0.00001)
					theta = atan2(DeltaY, DeltaX);
				else {
					if (DeltaX > 0)
						theta = 0;
					else 
						theta = PI;
					}
			}

			GDPoint pt;

			pt.x = (*iLink)->m_ShapePoints[si].x ;
			pt.y = (*iLink)->m_ShapePoints[si].y ;

			(*iLink)->m_BandLeftShapePoints.push_back (pt);

			double x_offset = (*iLink)->m_BandWidthValue*lane_offset* cos(theta - PI / 2.0f);
			double y_offset = (*iLink)->m_BandWidthValue*lane_offset* sin(theta - PI / 2.0f);
			pt.x = (*iLink)->m_ShapePoints[si].x + x_offset;
			pt.y = (*iLink)->m_ShapePoints[si].y + y_offset;

			(*iLink)->m_BandRightShapePoints.push_back (pt);

			if((*iLink)->m_bSensorData) // refernece band
			{

				pt.x = (*iLink)->m_ShapePoints[si].x ;
				pt.y = (*iLink)->m_ShapePoints[si].y ;

				(*iLink)->m_ReferenceBandLeftShapePoints.push_back (pt);

				pt.x  = (*iLink)->m_ShapePoints[si].x + (*iLink)->m_ReferenceBandWidthValue*lane_offset* cos(theta-PI/2.0f);
				pt.y = (*iLink)->m_ShapePoints[si].y + (*iLink)->m_ReferenceBandWidthValue*lane_offset* sin(theta-PI/2.0f);
				(*iLink)->m_ReferenceBandRightShapePoints.push_back (pt);
			}

		}

	}

}

void CTLiteDoc::OffsetLink()
{
	CWaitCursor wait;
	std::list<DTALink*>::iterator iLink;

	double length_sum_in_mile = 0;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		length_sum_in_mile += (*iLink) ->m_Length;

		if( (*iLink) ->m_Original_ShapePoints .size() ==0)// no original shape points
		{

			(*iLink) ->m_Original_ShapePoints = (*iLink) ->m_ShapePoints;
		}

		// comment it out for future examination
		//if( (*iLink) ->m_Original_ShapePoints .size() ==2)// two shape points only, overwite them using upsteram node and downstream node number 
		//{

		//		if((*iLink)->m_FromNodeNumber == 53718 && (*iLink)->m_ToNodeNumber == 53810)
		//		{
		//		TRACE("");
		//		}

		//	(*iLink) ->m_Original_ShapePoints.clear ();
		//	(*iLink) ->m_Original_ShapePoints.push_back (m_NodeNoMap[(*iLink) ->m_FromNodeID]->pt);
		//	(*iLink) ->m_Original_ShapePoints.push_back (m_NodeNoMap[(*iLink) ->m_ToNodeID]->pt );

		//}
	}

		double AvgLinkLengthInFeet = length_sum_in_mile/max(1,m_LinkSet.size()) * 5280.0;

		m_NodeDisplaySize = max(100, AvgLinkLengthInFeet*0.05);  // in feet


	if(m_bLinkToBeShifted)
	{
		std::list<DTALink*>::iterator iLink;

		double minimum_link_length = 999999;
		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			if((*iLink) -> m_bToBeShifted)
			{
				if((*iLink)->m_Length <  minimum_link_length)
					minimum_link_length = (*iLink)->m_Length ;
			}
		}
		double link_offset = max(minimum_link_length*0.025*m_UnitMile, m_UnitFeet*m_OffsetInFeet);  // 80 feet

		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			if ((*iLink)->m_FromNodeNumber == 31 && (*iLink)->m_ToNodeNumber == 30)
				TRACE("");


			if((*iLink) -> m_bToBeShifted)
			{
				//Test if an opposite link exits
				unsigned long OppositeLinkKey = GetLinkKey((*iLink)->m_ToNodeID,(*iLink)->m_FromNodeID);
				if ( m_NodeNotoLinkMap.find(OppositeLinkKey) != m_NodeNotoLinkMap.end())
				{
					// set two links as two-way links
					(*iLink)->m_bOneWayLink = false;

					DTALink* pLink = m_NodeNotoLinkMap[OppositeLinkKey];
					if(pLink!=NULL)
						pLink->m_bOneWayLink = false;

				}
			}
		}



		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			if ((*iLink)->m_FromNodeNumber == 31 && (*iLink)->m_ToNodeNumber == 30)
			{

				TRACE("");
			}

			if( (*iLink)->m_bOneWayLink == false)// apply link split to two way links
			{


				int last_shape_point_id = (*iLink) ->m_Original_ShapePoints .size() -1;

				ASSERT(last_shape_point_id>=1);

				double DeltaX = (*iLink)->m_Original_ShapePoints[last_shape_point_id].x - (*iLink)->m_Original_ShapePoints[0].x;
				double DeltaY = (*iLink)->m_Original_ShapePoints[last_shape_point_id].y - (*iLink)->m_Original_ShapePoints[0].y;
				double theta = 0;			
				if(fabs(DeltaY)>0.00001)
					theta= atan2(DeltaY, DeltaX);
				else {
					if (DeltaX > 0)
						theta = 0;
					else
						theta = PI;
					}

				for(unsigned int si = 0; si < (*iLink) ->m_Original_ShapePoints .size(); si++)
				{
					// calculate theta for each feature point segment
					if(si>= 1 && ( (*iLink) ->m_Original_ShapePoints .size() >4 || m_LinkTypeMap[(*iLink)->m_link_type].IsRamp ()))  // ramp or >4 feature points
					{
						last_shape_point_id = si;
						DeltaX = (*iLink)->m_Original_ShapePoints[last_shape_point_id].x - (*iLink)->m_Original_ShapePoints[si-1].x;
						DeltaY = (*iLink)->m_Original_ShapePoints[last_shape_point_id].y - (*iLink)->m_Original_ShapePoints[si-1].y;


						//if(fabs(DeltaY)>0.001)
						//	theta= atan2(DeltaY, DeltaX);
					}
					double cos_offset =  cos(theta-PI/2.0);
					double sin_offset = sin(theta-PI/2.0);
					(*iLink)->m_ShapePoints[si].x = (*iLink)->m_Original_ShapePoints[si].x + m_bRightHandTrafficFlag*link_offset* cos_offset;
					(*iLink)->m_ShapePoints[si].y = (*iLink)->m_Original_ShapePoints[si].y + m_bRightHandTrafficFlag*link_offset* sin_offset;
				}
			}


		}
	}

	m_bBezierCurveFlag = false;

	if(m_bBezierCurveFlag)  //do not apply m_bBezierCurveFlag 
	{
		BezierCurve bezier_smoothing;

		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			// bezier smoothing
			std::vector<GDPoint> NewShapePoints;

			if((*iLink) ->m_ShapePoints .size() >2)
			{
				// general smooth curve
				bezier_smoothing.Bezier2D((*iLink) ->m_ShapePoints, NewShapePoints);

				// re-write to data
				(*iLink) ->m_ShapePoints = NewShapePoints;

			}
		}

	}
}

bool CTLiteDoc::ReadLinkCSVFile(LPCTSTR lpszFileName, bool bCreateNewNodeFlag = false,  int LayerNo = 0)
{

	long i = m_LinkSet.size();
	DTALink* pLink = 0;
	float default_coordinate_distance_sum=0;
	float length_sum_in_mile = 0;

	CString error_message;
	CString warning_message = "";
	int warning_message_no = 0;

	bool bTwoWayLinkFlag = false;

	float AADT_conversion_factor = 0.1;	


	bool length_field_warning = false;
	bool number_of_lanes_field_warning = false;
	bool speed_limit_field_warning = false;
	bool capacity_field_warning = false;
	bool type_field_warning = false;


	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{
		bool bNodeNonExistError = false;
		while(parser.ReadRecord())
		{

			long link_id = 0;
			int from_node_id;
			int to_node_id;
			int direction = 0;
			double length_in_mile;
			int number_of_lanes= 1;
			int speed_limit_in_mph= 60;
			double capacity_in_pcphpl= 2000;
			int type = 1;
			string name;
			float k_jam = 180;
			int network_design_flag = 0;

			string loop_code, orientation_code;


			float wave_speed_in_mph = 12;
			string mode_code = "";
			string demand_type_code = "";

			float grade = 0;

			if(!parser.GetValueByFieldName("name",name))
				name = "";

			if(!parser.GetValueByFieldName("from_node_id",from_node_id)) 
			{
				AfxMessageBox("Field from_node_id has not been defined in file input_link.csv. Please check.");
				break;
			}
			if(!parser.GetValueByFieldName("to_node_id",to_node_id))
			{
				AfxMessageBox("Field to_node_id has not been defined in file input_link.csv. Please check.");
				break;
			}


			if(LayerNo == 1)  // for subarea mapmatching, all the node ids are converted to negative numbers. 
			{
				from_node_id = from_node_id *(-1);
				to_node_id = to_node_id *(-1);
			}

			if (LayerNo == 2) // for mapping 
			{
				from_node_id = from_node_id;  // from layer 0
				to_node_id = to_node_id *(-1); // from layer 1
			}

			if(m_NodeNumbertoNodeNoMap.find(from_node_id)== m_NodeNumbertoNodeNoMap.end())
			{
				if(bCreateNewNodeFlag == false)  // not create new node
				{
					CString warning;
					warning.Format ("from_node_id %d in input_link.csv has not been defined in input_node.csv.\n",from_node_id);
					bNodeNonExistError = true;

					if(warning_message.GetLength () < 3000)  // not adding and showing too many links
					{
						warning_message += warning;
					}

					continue;
				}else  // need to create a new node with geometry information
				{
					string geo_string;
					std::vector<CCoordinate> CoordinateVector;
					if(parser.GetValueByFieldName("geometry",geo_string))
					{
						// overwrite when the field "geometry" exists
						CGeometry geometry(geo_string);
						CoordinateVector = geometry.GetCoordinateList();
						if(CoordinateVector.size()>=2)
						{
							GDPoint	pt; 
							pt.x = CoordinateVector[0].X;
							pt.y = CoordinateVector[0].Y;
							AddNewNode(pt, from_node_id, LayerNo);

						}else
						{

							error_message.Format ("Field geometry in Link %s has less than 2 feature points, coordinate information of upstream node cannot be extracted. Please check.",name.c_str ());
							AfxMessageBox(error_message);
							return false;
						}

					}else
					{
						AfxMessageBox("Field geometry cannot be found in input_link.csv. Please check.");
						return false;
					}

				}	
			}

			if(m_NodeNumbertoNodeNoMap.find(to_node_id)== m_NodeNumbertoNodeNoMap.end())
			{
				if(bCreateNewNodeFlag == false)  // not create new node
				{
					CString warning;
					warning.Format ("to_node_id %d in input_link.csv has not been defined in input_node.csv\n",to_node_id);
					bNodeNonExistError = true;

					if(warning_message.GetLength () < 3000)  // not adding and showing too many links
					{
						warning_message += warning;
					}
					continue;
				}else  // create new node
				{
					string geo_string;
					std::vector<CCoordinate> CoordinateVector;
					if(parser.GetValueByFieldName("geometry",geo_string))
					{
						// overwrite when the field "geometry" exists
						CGeometry geometry(geo_string);
						CoordinateVector = geometry.GetCoordinateList();
						if(CoordinateVector.size()>=2)
						{
							GDPoint	pt; 
							pt.x = CoordinateVector[CoordinateVector.size()-1].X;
							pt.y = CoordinateVector[CoordinateVector.size()-1].Y;
							AddNewNode(pt, to_node_id, LayerNo);

						}else
						{
							error_message.Format ("Field geometry in Link %s has less than 2 feature points, coordinate information of upstream node cannot be extracted. Please check.",name.c_str ());
							AfxMessageBox(error_message);
							return false;
						}

					}else
					{
						AfxMessageBox("Field geometry cannot be found in input_link.csv. Please check.");
						return false;
					}


				}

			}


			DTALink* pExistingLink =  FindLinkWithNodeNumbers(from_node_id,to_node_id);
			bool bDonotAllowDuplicatedLinks = false;
			if (bDonotAllowDuplicatedLinks && pExistingLink)
			{

				CString warning;
				warning.Format ("Link %d-> %d is duplicated.\n", from_node_id,to_node_id);
				if(warning_message.GetLength () < 3000)  // not adding and showing too many links
				{
					warning_message += warning;
				}
				continue;
			}


			if(!parser.GetValueByFieldName("length_in_mile",length_in_mile))
			{
				if(!parser.GetValueByFieldName("length",length_in_mile))
				{
					if(!length_field_warning)
					{					
						AfxMessageBox("Field length_in_mile or length has not been defined in file input_link.csv. Please check.");
						length_field_warning = true;
					}
				
					length_in_mile = 1;
				}

			}


			if(!parser.GetValueByFieldName("direction",direction))
				direction = 1;



			if(!parser.GetValueByFieldName("number_of_lanes",number_of_lanes))
			{
					if(!number_of_lanes_field_warning)
					{
						AfxMessageBox("Field number_of_lanes has not been defined in file input_link.csv. Please check.");
						number_of_lanes_field_warning = true;
					}
					number_of_lanes = 1;

			}

			if (m_LinkTypeMap[type].type_code.find('c') != string::npos && number_of_lanes == 0)
			{
				number_of_lanes = 7; // reset # of lanes for connectors to a positive value
			}

			int NumberOfLeftTurnLanes = 0;
			int NumberOfRightTurnLanes = 0;
			double LeftTurnLaneLength = 0;	
			double RightTurnLaneLength = 0;	


			parser.GetValueByFieldName("number_of_left_turn_lanes",NumberOfLeftTurnLanes);
			parser.GetValueByFieldName("number_of_right_turn_lanes",NumberOfRightTurnLanes);
			parser.GetValueByFieldName("length_of_left_turn_lanes",LeftTurnLaneLength);
			parser.GetValueByFieldName("length_of_right_turn_lanes",RightTurnLaneLength);


			if(!parser.GetValueByFieldName("speed_limit_in_mph",speed_limit_in_mph))
			{
				if(!parser.GetValueByFieldName("speed_limit",speed_limit_in_mph))
				{
					if(!speed_limit_field_warning)
					{
					error_message.Format ("Link %s: Field speed_limit has not been defined in file input_link.csv. Please check.",name.c_str ());
					AfxMessageBox(error_message);
					speed_limit_field_warning = true;
					}
					speed_limit_in_mph = 60;
				}
			}

			float speed_at_capacity = speed_limit_in_mph;
			parser.GetValueByFieldName("speed_at_capacity", speed_at_capacity);

			float color_value = 0;
			parser.GetValueByFieldName("KML_color_value",color_value);

			int green_height = 0;
			parser.GetValueByFieldName("KML_green_height",green_height);
			int red_height = 0;
			parser.GetValueByFieldName("KML_red_height",red_height);

			int blue_height = 0;
			parser.GetValueByFieldName("KML_blue_height",blue_height);
			int yellow_height = 0;
			parser.GetValueByFieldName("KML_yellow_height",yellow_height);


			if(!parser.GetValueByFieldName("lane_cap",capacity_in_pcphpl))
			{
				if(!capacity_field_warning)
				{
				error_message.Format("Link %s: Field lane_cap has not been defined in file input_link.csv. Please check.",name.c_str ());
				AfxMessageBox(error_message);
				capacity_field_warning = true;
				}

				capacity_in_pcphpl = 1800;
			}



			if(!parser.GetValueByFieldName("link_type",type))
			{
				if(!type_field_warning)
				{
				AfxMessageBox("Field link_type has not been defined in file input_link.csv. Please check.");
				type_field_warning = true;
				}

				type = 1;
			}


			int observed_AADT = 0;
			int observed_peak_hourly_volume = 0;

			parser.GetValueByFieldName("observed_AADT",observed_AADT);
			parser.GetValueByFieldName("observed_peak_hourly_volume",observed_peak_hourly_volume);

			if(!parser.GetValueByFieldName("jam_density",k_jam))
			{
				if(!parser.GetValueByFieldName("jam_density_in_vhc_pmpl",k_jam))
					k_jam = 180;
			}
			if(!parser.GetValueByFieldName("AADT_conversion_factor",AADT_conversion_factor))
				AADT_conversion_factor = 0.1;

			if(!parser.GetValueByFieldName("grade",grade))
				grade = 0;

			if(!parser.GetValueByFieldName("wave_speed",wave_speed_in_mph))
			{
				if(!parser.GetValueByFieldName("wave_speed_in_mph",wave_speed_in_mph))
					wave_speed_in_mph = 12;
			}

			if (!parser.GetValueByFieldName("demand_type_code", demand_type_code))
				demand_type_code = "";


			if(!parser.GetValueByFieldName("mode_code",mode_code))
				mode_code = ""; 

			if (!parser.GetValueByFieldName("map_matching_loop_code", loop_code))
				loop_code = "";
			if (!parser.GetValueByFieldName("map_matching_orientation_code", orientation_code))
				orientation_code = "";

			std::replace( mode_code.begin(), mode_code.end(), ',', ';'); 


			if(mode_code.find ('"') !=  string::npos)
			{
				mode_code = '"' + mode_code + '"' ;
			}


			int prohibited_u_turn  = 0;

			parser.GetValueByFieldName("prohibited_u_turn",prohibited_u_turn);


			float BPR_alpha_term = 0.15;
			float BPR_beta_term = 4;

			parser.GetValueByFieldName("BPR_alpha_term",BPR_alpha_term);
			parser.GetValueByFieldName("BPR_beta_term",BPR_beta_term);




			if(!parser.GetValueByFieldName("link_id",link_id))  // no value
			{
				// mark it as 0 first, and we then find a new unique link id after reading all links
				link_id = 0;
			}

			string link_key;
			parser.GetValueByFieldName("link_key",link_key);

			string geo_string;

			std::vector<CCoordinate> Original_CoordinateVector;
			if(parser.GetValueByFieldName("original_geometry",geo_string))
			{
				// overwrite when the field "geometry" exists
				CGeometry geometry(geo_string);
				Original_CoordinateVector = geometry.GetCoordinateList();
			}

			bool bToBeShifted = true;
			bool bWithCoordinateVector = false;
			std::vector<CCoordinate> CoordinateVector;
			if(parser.GetValueByFieldName("geometry",geo_string))
			{
				// overwrite when the field "geometry" exists
				CGeometry geometry(geo_string);
				CoordinateVector = geometry.GetCoordinateList();
				if(CoordinateVector.size()>=2)
				{
					m_bLinkToBeShifted = false;
					bToBeShifted = false;
					bWithCoordinateVector = true;
				}
			}
			if(bWithCoordinateVector == false)
			{
				// no geometry information
				CCoordinate cc_from, cc_to; 
				cc_from.X = m_NodeNoMap[m_NodeNumbertoNodeNoMap[from_node_id]]->pt.x;
				cc_from.Y = m_NodeNoMap[m_NodeNumbertoNodeNoMap[from_node_id]]->pt.y;

				cc_to.X = m_NodeNoMap[m_NodeNumbertoNodeNoMap[to_node_id]]->pt.x;
				cc_to.Y = m_NodeNoMap[m_NodeNumbertoNodeNoMap[to_node_id]]->pt.y;

				CoordinateVector.push_back(cc_from);
				CoordinateVector.push_back(cc_to);
			}



			int link_code_start = 1;
			int link_code_end = 1;

			if (direction == -1) // reversed
			{
				link_code_start = 2; link_code_end = 2;
			}

			if (direction == 0) // two-directional link
			{
				link_code_start = 1; link_code_end = 2;
				bTwoWayLinkFlag = true;
			}



			string SpeedSensorID;
			parser.GetValueByFieldName("speed_sensor_id",SpeedSensorID);

			if(SpeedSensorID.size() ==0)
			{
			parser.GetValueByFieldName("TMC",SpeedSensorID);
			}

			string CountSensorID_str;
			parser.GetValueByFieldName("count_sensor_id",CountSensorID_str);

			parser.GetValueByFieldName("network_design_flag", network_design_flag);
			

			for(int link_code = link_code_start; link_code <=link_code_end; link_code++)
			{

				bool bNodeNonExistError = false;
				int m_SimulationHorizon = 1;
				pLink = new DTALink(m_SimulationHorizon);

				pLink->m_LayerNo = LayerNo;
				pLink->m_LinkNo = i;
				pLink->m_Name  = name;
				pLink->m_OrgDir = direction;
				pLink->m_LinkID = link_id;
				pLink->m_LinkKey = link_key.c_str ();

				m_LinkKeyMap[pLink->m_LinkKey] = pLink;

				pLink->m_Mode_code = mode_code;
				pLink->m_demand_type_code = demand_type_code;


				pLink->m_NumberOfLeftTurnLanes = NumberOfLeftTurnLanes;
				pLink->m_NumberOfRightTurnLanes = NumberOfRightTurnLanes;
				pLink->m_LeftTurnLaneLength = LeftTurnLaneLength;	
				pLink->m_RightTurnLaneLength = RightTurnLaneLength;	

				pLink->m_network_design_flag = network_design_flag;
				pLink->color_value = color_value;
				pLink->green_height = green_height;
				pLink->red_height = red_height;
				pLink->blue_height = blue_height;
				pLink->yellow_height = yellow_height;

				pLink->m_orientation_code = orientation_code;
				pLink->m_loop_code = loop_code;

				pLink->m_geo_string  = geo_string;

				if(SpeedSensorID.size() >=1)  // TMC exists 
				{
					pLink->m_SpeedSensorID = SpeedSensorID;
					m_SpeedSensorIDMap [SpeedSensorID] = pLink;
				}

				if(CountSensorID_str.size() >=1)
				{
					pLink->m_CountSensorID = CountSensorID_str;
					m_CountSensorIDMap [CountSensorID_str.c_str ()] = pLink;
				
				}




				if(link_code == 1)  //AB link
				{
					pLink->m_FromNodeNumber = from_node_id;

					pLink->m_ToNodeNumber = to_node_id;
					pLink->m_Direction  = 1;

					pLink->m_FromNodeID = m_NodeNumbertoNodeNoMap[from_node_id];
					pLink->m_ToNodeID= m_NodeNumbertoNodeNoMap[to_node_id];

					int si;

					if (LayerNo <= 1)  // not the mapping layer 
					{

						for (si = 0; si < CoordinateVector.size(); si++)
						{
							GDPoint	pt;
							pt.x = CoordinateVector[si].X;
							pt.y = CoordinateVector[si].Y;
							pLink->m_ShapePoints.push_back(pt);

						}
					}

				}

				if(link_code == 2)  //BA link
				{
					pLink->m_FromNodeNumber = to_node_id;
					pLink->m_ToNodeNumber = from_node_id;
					pLink->m_Direction  = 1;

					pLink->m_FromNodeID = m_NodeNumbertoNodeNoMap[to_node_id];
					pLink->m_ToNodeID= m_NodeNumbertoNodeNoMap[from_node_id];

					for(int si = CoordinateVector.size()-1; si >=0; si--)
					{
						GDPoint	pt;
						pt.x = CoordinateVector[si].X;
						pt.y = CoordinateVector[si].Y;
						pLink->m_ShapePoints .push_back (pt);
					}

					pLink->m_bToBeShifted = bToBeShifted; 
				}
				// original geometry

				if(pLink->m_ShapePoints.size() ==0)
				{
					TRACE("");
				}


				if(Original_CoordinateVector.size()>0) // data available
				{
					for(unsigned int si = 0; si < Original_CoordinateVector.size(); si++)
					{
						GDPoint	pt;
						pt.x = Original_CoordinateVector[si].X;
						pt.y = Original_CoordinateVector[si].Y;
						pLink->m_Original_ShapePoints .push_back (pt);

					}
				}else
				{  //   data not available, we might use the previously offet shape points to construct the "base-line" shape points
					pLink->m_Original_ShapePoints =  pLink->m_ShapePoints;

					m_OffsetInFeet = 0;

				}
				pLink->m_NumberOfLanes= number_of_lanes;

				if (pLink->m_FromNodeNumber == 2056 )
					TRACE("");

				pLink->m_SpeedLimit= max(10,speed_limit_in_mph);  // minimum speed limit is 1 mph
				pLink->m_SpeedAtCapacity = speed_at_capacity;
				
				pLink->m_avg_simulated_speed = pLink->m_SpeedLimit;

				//				pLink->m_Length= max(length_in_mile, pLink->m_SpeedLimit*0.1f/60.0f);  // minimum distance, special note: we do not consider the minimum constraint here, but a vehicle cannot travel longer then 0.1 seconds
				pLink->m_Length= length_in_mile;
				pLink->m_FreeFlowTravelTime = pLink->m_Length/pLink->m_SpeedLimit*60.0f;  // convert from hour to min
				pLink->m_StaticTravelTime = pLink->m_FreeFlowTravelTime;

				pLink->m_BPR_alpha_term =  BPR_alpha_term;
				pLink->m_BPR_beta_term  = BPR_beta_term;


				pLink->m_MaximumServiceFlowRatePHPL= capacity_in_pcphpl;
				pLink->m_LaneCapacity  = pLink->m_MaximumServiceFlowRatePHPL;
				pLink->m_link_type= type;
				pLink->m_Grade = grade;
				pLink->m_prohibited_u_turn  = prohibited_u_turn;

				pLink->m_Kjam = k_jam;
				pLink->m_AADT_conversion_factor = AADT_conversion_factor;
				pLink->m_Wave_speed_in_mph  = wave_speed_in_mph;

				m_NodeNoMap[pLink->m_FromNodeID ]->m_Connections+=1;
				m_NodeNoMap[pLink->m_ToNodeID ]->m_Connections+=1;


				if( m_LinkTypeMap[pLink->m_link_type].IsFreeway () ||  m_LinkTypeMap[pLink->m_link_type].IsRamp  ())
				{
					m_NodeNoMap[pLink->m_FromNodeID ]->m_bConnectedToFreewayORRamp = true;
					m_NodeNoMap[pLink->m_ToNodeID ]->m_bConnectedToFreewayORRamp = true;

				}

				m_NodeNoMap[pLink->m_FromNodeID ]->m_OutgoingLinkVector.push_back(pLink->m_LinkNo);
				m_NodeNoMap[pLink->m_ToNodeID ]->m_IncomingLinkVector.push_back(pLink->m_LinkNo);

				if(m_LinkTypeMap[pLink->m_link_type].IsConnector  () == false )
					m_NodeNoMap[pLink->m_ToNodeID ]->m_IncomingNonConnectors++;

				//estimation node level production and attraction
				m_NodeNoMap[pLink->m_FromNodeID ]->m_NodeProduction += min(pLink->m_LaneCapacity,pLink->m_Saturation_flow_rate_in_vhc_per_hour_per_lane) * min(4,pLink->m_NumberOfLanes )/2.0f; // min() is used to avoid artefial large capacity and lare number of lanes
				m_NodeNoMap[pLink->m_ToNodeID ]->m_NodeAttraction += min(pLink->m_LaneCapacity,pLink->m_Saturation_flow_rate_in_vhc_per_hour_per_lane) * min(4,pLink->m_NumberOfLanes )/2.0f;  // /2 is used to get reasonable demand



				unsigned long LinkKey = GetLinkKey( pLink->m_FromNodeID, pLink->m_ToNodeID);

				m_NodeNotoLinkMap[LinkKey] = pLink;

				__int64  LinkKey2 = GetLink64Key(pLink-> m_FromNodeNumber,pLink->m_ToNodeNumber);
				m_NodeNumbertoLinkMap[LinkKey2] = pLink;

				m_LinkNotoLinkMap[i] = pLink;
				m_LinkIDtoLinkMap[link_id] = pLink;


				m_NodeNoMap[pLink->m_FromNodeID ]->m_TotalCapacity += (pLink->m_MaximumServiceFlowRatePHPL* pLink->m_NumberOfLanes);

				pLink->m_FromPoint = m_NodeNoMap[pLink->m_FromNodeID]->pt;
				pLink->m_ToPoint = m_NodeNoMap[pLink->m_ToNodeID]->pt;

				if(pLink->m_Length>=0.00001)
				{
					default_coordinate_distance_sum+= pLink->DefaultDistance();
					length_sum_in_mile += pLink ->m_Length;
				}
				//			pLink->SetupMOE();



				if(!bNodeNonExistError)
				{
					//TRACE("\nAdd link no.%d,  %d -> %d",i,pLink->m_FromNodeNumber, pLink->m_ToNodeNumber );
					m_LinkSet.push_back (pLink);
					m_LinkNoMap[i]  = pLink;
					i++;
				}
			}

		}

		if(bNodeNonExistError)
		{
			ofstream m_WarningLogFile;

			m_WarningLogFile.open ( m_ProjectDirectory + "NeXTA.log", ios::out);
			if (m_WarningLogFile.is_open())
			{
				m_WarningLogFile.width(12);
				m_WarningLogFile.precision(3) ;
				m_WarningLogFile.setf(ios::fixed);
				m_WarningLogFile << warning_message << endl;

			}


			m_WarningLogFile.close ();

			AfxMessageBox("Some nodes in input_link.csv have not been defined in input_node.csv. Please check file NeXTA.log.");
		}

		m_UnitMile  = 1.0f;

		if(length_sum_in_mile>0.000001f)
		{
			m_UnitMile= default_coordinate_distance_sum / length_sum_in_mile ;

			m_bUnitMileInitialized = true;  // the unit mile has been reset according to the actual distance 
		}

		m_UnitFeet = m_UnitMile/5280.0f;  


		double AvgLinkLengthInFeet = length_sum_in_mile/max(1,m_LinkSet.size()) * 5280.0;

		m_NodeDisplaySize = min(10, AvgLinkLengthInFeet*0.05);  // in feet



		m_MovementTextBoxSizeInFeet = AvgLinkLengthInFeet * 0.2; // 20% of average link length

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			if((*iLink)-> m_Length <=0.00001)  //reset zero length link
			{
				(*iLink)-> m_Length = pLink->DefaultDistance()/m_UnitMile;

			}

		}

		//CString SettingsFile;
		//SettingsFile.Format ("%sDTASettings.txt",m_ProjectDirectory);
		//int long_lat_coordinate_flag = (int)(g_GetPrivateProfileDouble("GUI", "long_lat_coordinate_flag", 1, SettingsFile));	

		/*		if(m_UnitMile<1/50)  // long/lat must be very large and greater than 1/62!
		{

		m_LongLatCoordinateFlag = true;
		m_UnitFeet = m_UnitMile/62/5280.0f;  // 62 is 1 long = 62 miles
		WritePrivateProfileString("GUI","long_lat_coordinate_flag","0",SettingsFile);

		}
		else
		{
		WritePrivateProfileString("GUI","long_lat_coordinate_flag","0",SettingsFile);

		}
		*/
		// count of # of transit and walking links

		int transit_link_count = 0;
		int walking_link_count = 0;
		std::list<DTALink*>::iterator iLink;

		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{


			if (m_LinkTypeMap[(*iLink)->m_link_type ].IsTransit  ())
				transit_link_count++;

			if (m_LinkTypeMap[(*iLink)->m_link_type ].IsWalking  ())
				walking_link_count++;

		}

		if( walking_link_count >0 || transit_link_count >0)
		{
			m_LinkDataLoadingStatus.Format ("%d links (%d transit links, %d walking links) are loaded from file %s.",
				m_LinkSet.size(),transit_link_count, walking_link_count,lpszFileName);

		}else
		{
			m_LinkDataLoadingStatus.Format ("%d links are loaded from file %s.",m_LinkSet.size(),lpszFileName);
		}


		AssignUniqueLinkIDForEachLink();

		if(bTwoWayLinkFlag == true)
			m_bLinkToBeShifted = true;

		GenerateOffsetLinkBand();

		BuildGridSystem();

		if(warning_message.GetLength () >=1)
		{
			CString final_message; 
			final_message = "Warning messages:\n" + warning_message + "\n\nIf you save the network in *.tnp again, then duplicated links and links with non-existings nodes will be automatically removed.\n";
			AfxMessageBox(final_message);
		}
		return true;
	}else
	{
		if (LayerNo == 0)
		{
			AfxMessageBox("Error: File input_link.csv cannot be opened.\n It might be currently used and locked by EXCEL.");
		}
		return false;
		//		g_ProgramStop();
	}


}


bool CTLiteDoc::ReadReferenceLineCSVFile(LPCTSTR lpszFileName)
{
	long i = 0;

	CString error_message;
	CString warning_message = "";
	int warning_message_no = 0;


	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{
		bool bNodeNonExistError = false;
		while (parser.ReadRecord())
		{
			std::string name;
			int line_type = 0;
			int direction = 1;  // default: single direction 
			std::string line_id;

			if (!parser.GetValueByFieldName("name", name))
				name = "";
			if (!parser.GetValueByFieldName("line_id", line_id))
				line_id = "";

			if (!parser.GetValueByFieldName("line_type", line_type))
			{
				line_type = 0;
			}


			string geo_string;

			std::vector<CCoordinate> Original_CoordinateVector;
			if (parser.GetValueByFieldName("original_geometry", geo_string))
			{
				// overwrite when the field "geometry" exists
				CGeometry geometry(geo_string);
				Original_CoordinateVector = geometry.GetCoordinateList();
			}

			bool bToBeShifted = true;
			bool bWithCoordinateVector = false;
			std::vector<CCoordinate> CoordinateVector;
			if (parser.GetValueByFieldName("geometry", geo_string))
			{
				// overwrite when the field "geometry" existsD
				CGeometry geometry(geo_string);
				CoordinateVector = geometry.GetCoordinateList();
				if (CoordinateVector.size() >= 2)
				{
					m_bLinkToBeShifted = false;
					bToBeShifted = false;
					bWithCoordinateVector = true;
				}
			}


			int link_code_start = 1;
			int link_code_end = 1;

			if (direction == -1) // reversed
			{
				link_code_start = 2; link_code_end = 2;
			}

			if (direction == 0) // two-directional link
			{
				link_code_start = 1; link_code_end = 2;
			}


			DTALine* pLine = NULL;

			for (int link_code = link_code_start; link_code <= link_code_end; link_code++)
			{

				pLine = new DTALine();
				pLine->direction = direction;
				pLine->m_LineID = line_id;
				pLine->m_geo_string = geo_string;

				if (link_code == 1)  //AB link
				{
					pLine->direction = 1;

					int si;

					for (si = 0; si < CoordinateVector.size(); si++)
					{
						GDPoint	pt;
						pt.x = CoordinateVector[si].X;
						pt.y = CoordinateVector[si].Y;
						pLine->m_ShapePoints.push_back(pt);

					}

				}

				if (link_code == 2)  //BA link
				{
					pLine->direction = 1;

					for (int si = CoordinateVector.size() - 1; si >= 0; si--)
					{
						GDPoint	pt;
						pt.x = CoordinateVector[si].X;
						pt.y = CoordinateVector[si].Y;
						pLine->m_ShapePoints.push_back(pt);
					}

				}
				m_DTALineSet.push_back(pLine);

				i++;
			}
		}

	}

	std::list<DTALine*>::iterator iLine;

	for (iLine = m_DTALineSet.begin(); iLine != m_DTALineSet.end(); iLine++)
	{
		(*iLine)->CalculateShapePointRatios();

	}


	if (m_DTALineSet.size() > 0)
	{
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		pMainFrame->m_bShowLayerMap[layer_vehicle_position] = true;
	}

	return true;

}

void CTLiteDoc::GenerateSubareaStatistics()
{
	//	
	//if(m_SubareaShapePoints.size()>0)
	//{
	//std::list<DTALink*>::iterator iLink;

	//iLink = m_LinkSet.begin(); 

	//int subarea_link_count = 0;

	//while (iLink != m_LinkSet.end())
	//{
	//	if((*iLink)->m_bIncludedinSubarea ==true)
	//	{
	//	subarea_link_count++;
	//	}
	//	iLink++;
	//}

	//if(subarea_link_count>=1)
	//	return; // do nothing

	//	CTLiteView* pView = 0;
	//	POSITION pos = GetFirstViewPosition();
	//	if(pos != NULL)
	//	{
	//		pView = (CTLiteView*) GetNextView(pos);
	//		if(pView!=NULL )
	//		{
	//			pView->CopyLinkSetInSubarea();
	//		}

	//	}
	//}

}
bool CTLiteDoc::ReadSubareaCSVFile(LPCTSTR lpszFileName)
{

	int lineno = 0 ;
	m_SubareaShapePoints.clear ();

	CCSVParser parser;

	if (parser.OpenCSVFile(lpszFileName))
	{
		int i=0;
		while(parser.ReadRecord())
		{
			GDPoint point;

			if(parser.GetValueByFieldName("x",point.x) == false)
				break;

			if(parser.GetValueByFieldName("y",point.y) == false)
				break;

			m_SubareaShapePoints.push_back(point);
			lineno++;
		}

		if(m_SubareaShapePoints.size()>0)
		{
			CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
			pMainFrame->m_bShowLayerMap[layer_subarea] = true;


		}
	}

	return true;
}

bool CTLiteDoc::ReadActivityLocationCSVFile(LPCTSTR lpszFileName)
{
	CCSVParser parser;

	bool bWarningFlag = false; 
	if (parser.OpenCSVFile(lpszFileName))
	{
		int lineno =0;
		m_ActivityLocationCount = 0;
		while(parser.ReadRecord())
		{
			int zone_number;

			if(parser.GetValueByFieldName("zone_id",zone_number) == false)
			{
				if(lineno == 0)
				{
					AfxMessageBox("Field zone_id has not been defined in file input_activity_location.csv. Please check.");
				}
				continue; 
			}

			int node_name;
			if(parser.GetValueByFieldName("node_id",node_name) == false)
			{
				AfxMessageBox("Field node_id has not been defined in file input_activity_location.csv. Please check.");
				continue;
			}

			map <int, int> :: const_iterator m_Iter = m_NodeNumbertoNodeNoMap.find(node_name);

			if (m_Iter == m_NodeNumbertoNodeNoMap.end() && !bWarningFlag)
			{
				m_NEXTALOGFile<< "Node Number "  << node_name << " in input_activity_location.csv has not been defined in input_node.csv. Please check."  << endl; 

				CString str;
				str.Format("Node Number %d in input_activity_location.csv has not been defined in input_node.csv", node_name );
				AfxMessageBox(str);
				bWarningFlag = true;
				continue;

			}

			if (m_ZoneMap.find(zone_number) == m_ZoneMap.end() && !bWarningFlag)
			{

				CString str;
				str.Format("Zone Number %d in input_activity_location.csv has not been defined in input_zone.csv. Please check.", zone_number );
				AfxMessageBox(str);
				bWarningFlag = true;

				continue;
			}
			// if there are multiple nodes for a zone, the last node id is recorded.
			DTAActivityLocation element;
			element.ZoneID  = zone_number;
			element.NodeNumber = node_name;
			element.pt = m_NodeNoMap [m_NodeNumbertoNodeNoMap[node_name] ] ->pt;

			int External_OD_Flag  = 0;
			parser.GetValueByFieldName("external_OD_flag",External_OD_Flag);
			element.External_OD_flag = External_OD_Flag;

			m_NodeNotoZoneNameMap[m_NodeNumbertoNodeNoMap[node_name]] = zone_number;
			m_NodeNoMap [m_NodeNumbertoNodeNoMap[node_name] ] -> m_ZoneID = zone_number;
			m_NodeNoMap [m_NodeNumbertoNodeNoMap[node_name] ] ->m_External_OD_flag = element.External_OD_flag;

			m_ZoneMap [zone_number].m_ActivityLocationVector .push_back (element);

			if(m_ODSize < zone_number)
				m_ODSize = zone_number;

			lineno++;
			m_ActivityLocationCount++;
		}

		m_ActivityLocationDataLoadingStatus.Format ("%d activity location entries are loaded from file %s.",lineno,lpszFileName);
		return true;
	}else
	{
		m_ActivityLocationDataLoadingStatus.Format ("File input_activity_location.csv does not exist. Use default node-zone mapping table.");

		return false;
	}

}

bool CTLiteDoc::ReadZoneCSVFile(LPCTSTR lpszFileName)
{
	bool bNodeNonExistError = false;
	m_NodeNotoZoneNameMap.clear ();
	int lineno = 0 ;

	CCSVParser parser;

	m_ZoneMap.clear ();

	if (parser.OpenCSVFile(lpszFileName))
	{
		int i=0;
		while(parser.ReadRecord())
		{
			int zone_number;
			float production  = 0;
			float attraction  = 0;

			if(parser.GetValueByFieldName("zone_id",zone_number) == false)
			{
				if(i==0) // first line
				{
					AfxMessageBox("Field zone_id has not been defined in file input_activity_location.csv. Please check.");
				}
				return false;
			}

			int TAZ = zone_number;
			parser.GetValueByFieldName("TAZ",TAZ);


			parser.GetValueByFieldName("production",production);
			parser.GetValueByFieldName("attraction",attraction);

			m_ZoneMap [zone_number].m_ZoneID = TAZ;
			m_ZoneMap [zone_number].m_Production  = production;
			m_ZoneMap [zone_number].m_Attraction  = attraction;

			string notes; 

			if(parser.GetValueByFieldName("notes",notes))
				m_ZoneMap [zone_number].notes  = notes;

			std::vector<CCoordinate> CoordinateVector;
			string geo_string;

			if(parser.GetValueByFieldName("geometry",geo_string))
			{
				// overwrite when the field "geometry" exists
				CGeometry geometry(geo_string);
				CoordinateVector = geometry.GetCoordinateList();

				int si;
				for(si = 0; si < CoordinateVector.size(); si++)
				{
					GDPoint	pt;
					pt.x = CoordinateVector[si].X;
					pt.y = CoordinateVector[si].Y;
					m_ZoneMap [zone_number].m_ShapePoints .push_back (pt);

				}

			}
			if(m_ODSize < zone_number)
				m_ODSize = zone_number;

			lineno++;
		}

		if(bNodeNonExistError)
			AfxMessageBox("Some nodes in input_zone.csv have not been defined in input_node.csv. Please check NeXTA.log in the project folder.");

		m_ZoneDataLoadingStatus.Format ("%d zone info records are loaded from file %s.",lineno,lpszFileName);
		return true;
	}else
	{
		m_ZoneDataLoadingStatus.Format ("File input_zone.csv does not exist. Use default zone-TAZ mapping table.");

		return false;
	}

}

bool CTLiteDoc::ReadScenarioSettingCSVFile(LPCTSTR lpszFileName)
{
	CCSVParser parser_scenario;
	if (parser_scenario.OpenCSVFile(lpszFileName))
	{

		m_NumberOfScenarioSettings = 0;
		while(parser_scenario.ReadRecord())
		{
			parser_scenario.GetValueByFieldName("number_of_iterations",m_number_of_iterations);
			parser_scenario.GetValueByFieldName("traffic_flow_model",m_traffic_flow_model);
			parser_scenario.GetValueByFieldName("signal_representation_model",m_signal_reresentation_model);

			parser_scenario.GetValueByFieldName("traffic_analysis_method",m_traffic_analysis_method);
			
			parser_scenario.GetValueByFieldName("calibration_data_start_time_in_min", m_calibration_data_start_time_in_min);
			parser_scenario.GetValueByFieldName("calibration_data_end_time_in_min", m_calibration_data_end_time_in_min);



			m_NumberOfScenarioSettings++;
		}

		if(m_NumberOfScenarioSettings >=1)
			return true;

	}
	return false;
}


bool CTLiteDoc::WriteScenarioSettingCSVFile(LPCTSTR lpszFileName)
{
	int scenario_no;
	string scenario_name;
	int random_seed;
	int ODME_start_iteration;

	float ODME_max_percentage_deviation_wrt_hist_demand;
	float ODME_step_size;
	int ue_gap_calculation_method;
	int agent_demand_input_mode;
	int calibration_data_start_time_in_min;
	int calibration_data_end_time_in_min;
	int routing_movement_delay_mode;

	int accessibility_mode;
	int LearningRate[400] = {5};

	CCSVParser parser_scenario;
	if (parser_scenario.OpenCSVFile(lpszFileName))
	{
		m_NumberOfScenarioSettings = 0;

		while(parser_scenario.ReadRecord())
		{
			parser_scenario.GetValueByFieldName("scenario_no",scenario_no);
			parser_scenario.GetValueByFieldName("scenario_name",scenario_name);

			// the following parameters can be changed through dialog
			//parser_scenario.GetValueByFieldName("number_of_iterations",m_number_of_iterations);//
			//parser_scenario.GetValueByFieldName("traffic_flow_model",m_traffic_flow_model);
			//parser_scenario.GetValueByFieldName("traffic_analysis_method",m_traffic_analysis_method);
			//parser_scenario.GetValueByFieldName("demand_multiplier",m_demand_multiplier);
			//parser_scenario.GetValueByFieldName("emission_data_output",m_emission_data_output);

			parser_scenario.GetValueByFieldName("random_seed",random_seed);

			parser_scenario.GetValueByFieldName("ODME_start_iteration",ODME_start_iteration);
			parser_scenario.GetValueByFieldName("ODME_max_percentage_deviation_wrt_hist_demand",ODME_max_percentage_deviation_wrt_hist_demand);
			parser_scenario.GetValueByFieldName("ODME_step_size",ODME_step_size);
			parser_scenario.GetValueByFieldName("ue_gap_calculation_method",ue_gap_calculation_method);
			parser_scenario.GetValueByFieldName("calibration_data_start_time_in_min",calibration_data_start_time_in_min);
			parser_scenario.GetValueByFieldName("calibration_data_end_time_in_min",calibration_data_end_time_in_min);
			parser_scenario.GetValueByFieldName("routing_movement_delay_mode",routing_movement_delay_mode);


			m_NumberOfScenarioSettings++;
		}

		parser_scenario.CloseCSVFile ();


	}


	if(m_NumberOfScenarioSettings==1)
	{
		CCSVWriter ScenarioFile;

		if(ScenarioFile.Open(lpszFileName))
		{


			ScenarioFile.SetFieldNameAndValue("scenario_no",scenario_no);
			ScenarioFile.SetFieldNameAndValue("scenario_name",scenario_name);
			ScenarioFile.SetFieldNameAndValue("number_of_iterations",m_number_of_iterations);//
			ScenarioFile.SetFieldNameAndValue("traffic_flow_model",m_traffic_flow_model);
			ScenarioFile.SetFieldNameAndValue("signal_representation_model",m_signal_reresentation_model);
			ScenarioFile.SetFieldNameAndValue("traffic_analysis_method",m_traffic_analysis_method);
			ScenarioFile.SetFieldNameAndValue("random_seed",random_seed);
			ScenarioFile.SetFieldNameAndValue("ODME_start_iteration",ODME_start_iteration);
			ScenarioFile.SetFieldNameAndValue("ODME_max_percentage_deviation_wrt_hist_demand",ODME_max_percentage_deviation_wrt_hist_demand);
			ScenarioFile.SetFieldNameAndValue("ODME_step_size",ODME_step_size);
			ScenarioFile.SetFieldNameAndValue("calibration_data_start_time_in_min",calibration_data_start_time_in_min);
			ScenarioFile.SetFieldNameAndValue("calibration_data_end_time_in_min",calibration_data_end_time_in_min);

			ScenarioFile.WriteHeader ();
			ScenarioFile.WriteRecord ();

		}
	}else if (m_NumberOfScenarioSettings >=2)
	{
		AfxMessageBox("There are multiple scenarios in input_scenario_settings.csv. Please use Excel to make changes.", MB_ICONINFORMATION);

		OpenCSVFileInExcel(lpszFileName);

	}

	return true;
}


bool CTLiteDoc::ReadMetaDemandCSVFile(LPCTSTR lpszFileName)
{
	CString directory;

	// reset
	m_DemandLoadingStartTimeInMin = 1440;
	m_DemandLoadingEndTimeInMin = 0;

	m_DemandFileVector.clear ();

	if(m_ProjectFile.GetLength () ==0 )
	{
		AfxMessageBox("The project directory has not been specified. Please save the project to a new folder first.");
		return false;
	}
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);


	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{

		while(parser.ReadRecord())
		{
			string file_name;
			string format_type;
			int demand_type= 1;
			int start_time_in_min,end_time_in_min;
			float subtotal_demand_volume;

			parser.GetValueByFieldName("file_name",file_name);

			m_DemandFileVector.push_back (file_name.c_str ());
			parser.GetValueByFieldName("format_type",format_type);
			parser.GetValueByFieldName("demand_type",demand_type);

			if(demand_type >= m_DemandTypeVector.size())  // demand type out of bound
				break;

			parser.GetValueByFieldName("start_time_in_min",start_time_in_min);
			parser.GetValueByFieldName("end_time_in_min",end_time_in_min);
			parser.GetValueByFieldName("subtotal_demand_volume",subtotal_demand_volume);

			if(m_DemandLoadingStartTimeInMin > start_time_in_min)
				m_DemandLoadingStartTimeInMin = start_time_in_min;

			if(m_DemandLoadingEndTimeInMin < end_time_in_min)
				m_DemandLoadingEndTimeInMin = end_time_in_min;


			CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

			if(pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingStartTimeInMin==-1)
			{
				pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingStartTimeInMin = m_DemandLoadingStartTimeInMin;
			}else if ( pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingStartTimeInMin > m_DemandLoadingStartTimeInMin)
			{
				pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingStartTimeInMin = m_DemandLoadingStartTimeInMin;

			}

			if(pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingEndTimeInMin==-1)
			{
				pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingEndTimeInMin = m_DemandLoadingEndTimeInMin;
			}else if ( pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingEndTimeInMin < m_DemandLoadingEndTimeInMin)
			{
				pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingEndTimeInMin = m_DemandLoadingEndTimeInMin;

			}

			pMainFrame->m_wndPlayerSeekBar.Invalidate ();

		}

	}  // for each record
	return true;

}

bool CTLiteDoc::ReadMetaSignalCSVFile(LPCTSTR lpszFileName)
{
	CString directory;


	m_DemandFileVector.clear ();

	if(m_ProjectFile.GetLength () ==0 )
	{
		AfxMessageBox("The project directory has not been specified. Please save the project to a new folder first.");
		return false;
	}
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);


	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{

		while(parser.ReadRecord())
		{
			string file_name;
			string format_type;
			int demand_type= 1;
			int start_time_in_min,end_time_in_min;
			float subtotal_demand_volume;

			parser.GetValueByFieldName("file_name",file_name);

			m_DemandFileVector.push_back (file_name.c_str ());
			parser.GetValueByFieldName("format_type",format_type);
			parser.GetValueByFieldName("demand_type",demand_type);

			if(demand_type >= m_DemandTypeVector.size())  // demand type out of bound
				break;

			parser.GetValueByFieldName("start_time_in_min",start_time_in_min);
			parser.GetValueByFieldName("end_time_in_min",end_time_in_min);
			parser.GetValueByFieldName("subtotal_demand_volume",subtotal_demand_volume);

			if(m_DemandLoadingStartTimeInMin > start_time_in_min)
				m_DemandLoadingStartTimeInMin = start_time_in_min;

			if(m_DemandLoadingEndTimeInMin < end_time_in_min)
				m_DemandLoadingEndTimeInMin = end_time_in_min;


			CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

			if(pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingStartTimeInMin==-1)
			{
				pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingStartTimeInMin = m_DemandLoadingStartTimeInMin;
			}else if ( pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingStartTimeInMin > m_DemandLoadingStartTimeInMin)
			{
				pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingStartTimeInMin = m_DemandLoadingStartTimeInMin;

			}

			if(pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingEndTimeInMin==-1)
			{
				pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingEndTimeInMin = m_DemandLoadingEndTimeInMin;
			}else if ( pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingEndTimeInMin < m_DemandLoadingEndTimeInMin)
			{
				pMainFrame->m_wndPlayerSeekBar.m_DemandLoadingEndTimeInMin = m_DemandLoadingEndTimeInMin;

			}

			pMainFrame->m_wndPlayerSeekBar.Invalidate ();

		}

	}  // for each record
	return true;

}


bool CTLiteDoc::ReadDemandCSVFile(LPCTSTR lpszFileName)
{
	float LengthinMB;
	FILE* pFile;
	long Length;
	fopen_s(&pFile,lpszFileName,"rb");
	if(pFile!=NULL)
	{
		fseek(pFile, 0, SEEK_END );
		Length = ftell(pFile);
		fclose(pFile);

		LengthinMB= Length*1.0/1024/1024;
		if(LengthinMB > 100)
		{
			CString msg;
			msg.Format("The input_demand.csv file is %5.1f MB in size.\nIt could take quite a while to load this file.\nWould you like to load the input_demand file?",LengthinMB);
			if(AfxMessageBox(msg,MB_YESNO|MB_ICONINFORMATION)==IDNO)
				return true;
		}
	}

	//  // step 1: unzip
	//
	//CString directory;

	//if(m_ProjectFile.GetLength () ==0 )
	//{
	//	AfxMessageBox("The project directory has not been specified. Please save the project to a new folder first.");
	//	return false;
	//}
	//directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

	//HZIP hz = OpenZip(directory+"TNP_data.zip",0);
	//   
	//SetCurrentDirectory(directory);

	//   ZIPENTRY ze; GetZipItem(hz,-1,&ze); int numitems=ze.index;
	// 	for (int zi=0; zi<numitems; zi++)
	//  { 
	//GetZipItem(hz,zi,&ze);
	//   UnzipItem(hz,zi,ze.name);
	//  }
	//   CloseZip(hz);


	//// step 2: read bin file

	// // obtain file size:
	//// obtain file size:
	// pFile = fopen ( directory+"input_demand.bin" , "rb" );
	// if (pFile!=NULL)
	// {
	//  float total_demand = 0;
	// fseek (pFile , 0 , SEEK_END);
	// long lSize = ftell (pFile);
	// rewind (pFile);
	// int RecordCount = lSize/sizeof(DemandRecordData);
	// if(RecordCount >=1)
	// {
	// // allocate memory to contain the whole file:
	// DemandRecordData* pDemandData = new DemandRecordData [RecordCount];
	// // copy the file into the buffer:
	// fread (pDemandData,1,sizeof(DemandRecordData)*RecordCount,pFile);
	// int demand_type;
	// int demand_type_size = min(5,m_DemandTypeVector.size());
	// for(int i = 0; i < RecordCount; i++)
	// {
	//for(demand_type = 0; demand_type < demand_type_size; demand_type++)
	//{
	//	total_demand +=  pDemandData[i].number_of_vehicles[demand_type];
	//	m_ZoneMap[pDemandData[i].origin_zone ].m_ODDemandMatrix [pDemandData[i].destination_zone].SetValue (demand_type+1,pDemandData[i].number_of_vehicles[demand_type]);
	//}
	// 
	// }
	// 
	// /* the whole file is now loaded in the memory buffer. */

	// // terminate
	// fclose (pFile);

	//	m_AMSLogFile << RecordCount << "demand entries are loaded from file input_demand.bin "  ". Total demand =  " 
	//		<< total_demand << endl;
	//	m_DemandDataLoadingStatus.Format ("%d demand entries are loaded from file input_demand.bin. Total demand = %f",RecordCount,total_demand);


	// if(pDemandData!=NULL)
	//  delete pDemandData;

	//
	//  return true;
	// }
	// }

	////


	long lineno = 0;
	float total_demand = 0;
	FILE* st;
	fopen_s(&st,lpszFileName, "r");
	if (st!=NULL)
	{
		char  str_line[2000]; // input string
		int str_line_size;
		g_read_a_line(st,str_line, str_line_size); //  skip the first line

		int origin_zone, destination_zone;
		float number_of_vehicles ;
		float starting_time_in_min;
		float ending_time_in_min;


		while( fscanf_s(st,"%d,%d,%f,%f,",&origin_zone,&destination_zone,&starting_time_in_min, &ending_time_in_min) >0)
		{
			// static traffic assignment, set the demand loading horizon to [0, 60 min]
			for(unsigned int demand_type = 1; demand_type <= m_DemandTypeVector.size(); demand_type++)
			{
				number_of_vehicles = 0;
				if( fscanf_s(st,"%f,",&number_of_vehicles) == 0)
				{

					CString msg;
					msg.Format ("Demand type %d in line %d of file %s cannot be found. ",demand_type,lineno+1,lpszFileName);
					AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
					return false;
				}

				if(number_of_vehicles < -0.0001)
					number_of_vehicles = 0;

				if(origin_zone <= m_ODSize && destination_zone <= m_ODSize)
				{
					m_ZoneMap[origin_zone].m_ODDemandMatrix [destination_zone].SetValue (demand_type,number_of_vehicles);
					total_demand += number_of_vehicles;
				}
				else
				{
					CString msg;
					msg.Format ("Line %d in file %s has a zone number greater than the size of zones (%d). ",lineno,lpszFileName, m_ODSize);
					AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
					return false;
				}
			}

			lineno++;
		}

		fclose(st);
		m_AMSLogFile << lineno << "demand entries are loaded from file " << lpszFileName << ". Total demand =  " << total_demand << endl;
		m_DemandDataLoadingStatus.Format ("%d demand entries are loaded from file %s. Total demand = %f",lineno,lpszFileName,total_demand);
		return true;
	}else
	{
		//		AfxMessageBox("Error: File input_demand.csv cannot be found or opened.\n It might be currently used and locked by EXCEL.");
		return false;
		//		g_ProgramStop();
	}

}


bool CTLiteDoc::ReadDemandTypeCSVFile(LPCTSTR lpszFileName)
{
	long lineno = 1;
	CCSVParser parser;

	if (parser.OpenCSVFile(lpszFileName))
	{
		m_DemandTypeVector.clear();

		while(parser.ReadRecord())
		{
			int demand_type;
			float averageVOT;

			if(parser.GetValueByFieldName("demand_type",demand_type) == false)
				break;
			string demand_type_name, pricing_type_name;
			parser.GetValueByFieldName("demand_type_name",demand_type_name);
			parser.GetValueByFieldName("avg_VOT", averageVOT);

			float ratio_pretrip, ratio_enroute;

			parser.GetValueByFieldName("percentage_of_pretrip_info",ratio_pretrip);
			parser.GetValueByFieldName("percentage_of_enroute_info",ratio_enroute);

			DTADemandType element;
			element.demand_type = demand_type;
			element.demand_type_name  = demand_type_name.c_str ();
			element.average_VOT = max(0,min(averageVOT, 1000));
			element.info_class_percentage[1] = ratio_pretrip;
			element.info_class_percentage[2] = ratio_enroute;
			element.info_class_percentage[0] = 1 - ratio_enroute - ratio_pretrip;

			for( int i=0; i< m_VehicleTypeVector.size(); i++)
			{
				std::ostringstream  str_percentage_of_vehicle_type;
				str_percentage_of_vehicle_type << "percentage_of_vehicle_type" << i+1;

				float percentage_vehicle_type = 0;
				if(parser.GetValueByFieldName(str_percentage_of_vehicle_type.str(),percentage_vehicle_type))
					element.vehicle_type_percentage[i]= percentage_vehicle_type;
			}

			m_DemandTypeVector.push_back(element);

			lineno++;
		}
		m_AMSLogFile << "Read " << m_DemandTypeVector.size() << " demand types from file "  << lpszFileName << endl; 

		CString msg;
		msg.Format("Imported %d demand types from file %s",m_DemandTypeVector.size(),lpszFileName);
		m_MessageStringVector.push_back (msg);

		return true;
	}else
	{
		CString msg;
		msg.Format("Imported 0 demand types from file %s",lpszFileName);
		m_MessageStringVector.push_back (msg);

		return false;
	}

}

bool CTLiteDoc::ReadVehicleTypeCSVFile(LPCTSTR lpszFileName)
{
	long lineno = 1;
	CCSVParser parser;

	if (parser.OpenCSVFile(lpszFileName))
	{
		m_VehicleTypeVector.clear();

		while(parser.ReadRecord())
		{
			int demand_type;

			if(parser.GetValueByFieldName("vehicle_type",demand_type) == false)
				break;

			string vehicle_type_name;
			parser.GetValueByFieldName("vehicle_type_name",vehicle_type_name);

			DTAVehicleType element;
			element.vehicle_type = demand_type;
			element.vehicle_type_name  = vehicle_type_name.c_str ();

			m_VehicleTypeVector.push_back(element);

			lineno++;
		}

		m_AMSLogFile << "Read " << m_VehicleTypeVector.size() << " vehicle types from file "  << lpszFileName << endl; 

		return true;
	}else
	{
	//	AfxMessageBox("Error: File input_vehicle_type.csv cannot be found or opened.\n It might be currently used and locked by EXCEL.");
		return false;
		//		g_ProgramStop();
	}

}

bool CTLiteDoc::ReadLinkTypeCSVFile(LPCTSTR lpszFileName)
{
	long lineno = 0;
	CCSVParser parser;

	if (parser.OpenCSVFile(lpszFileName))
	{
		m_LinkTypeMap.clear();
		while(parser.ReadRecord())
		{
			DTALinkType element;

			if(parser.GetValueByFieldName("link_type",element.link_type ) == false)
			{
				if(lineno==0)
				{
					AfxMessageBox("Field link_type cannot be found in input_link_type.csv.");
				}
				break;
			}


			if(element.link_type<0)
			{
				CString str;
				str.Format ("Field link_type in file input_link_type.csv has an invalid value of %d at line %d. Please check.",element.link_type, lineno);
				AfxMessageBox(str);
				break;
			}

			if(parser.GetValueByFieldName("link_type_name",element.link_type_name ) == false)
			{
				AfxMessageBox("Field link_type_name cannot be found in input_link_type.csv.");
				break;
			}



			if(parser.GetValueByFieldName("type_code",element.type_code   ) == false)
			{
				AfxMessageBox("Field type_code cannot be found in input_link_type.csv.");
				break;
			}

			parser.GetValueByFieldName("travel_time_bias_factor",element.travel_time_bias_factor   ) ;
			parser.GetValueByFieldName("approximate_cycle_length_in_second",element.approximate_cycle_length_in_second   ) ;
			parser.GetValueByFieldName("saturation_flow_rate_in_vhc_per_hour_per_lane",element.saturation_flow_rate_in_vhc_per_hour_per_lane   ) ;
			parser.GetValueByFieldName("capacity_adjustment_factor",element.capacity_adjustment_factor   ) ;

			if(parser.GetValueByFieldName("default_lane_capacity",element.default_lane_capacity   ) == false && lineno==0)
			{
				//				AfxMessageBox("Field default_lane_capacity cannot be found in input_link_type.csv.");
			}

			if(parser.GetValueByFieldName("default_speed_limit",element.default_speed) == false  && lineno==0)
			{
				//				AfxMessageBox("Field default_speed cannot be found in input_link_type.csv.");
			}

			if(parser.GetValueByFieldName("default_number_of_lanes",element.default_number_of_lanes) == false  && lineno==0)
			{
				//				AfxMessageBox("Field default_number_of_lanes cannot be found in input_link_type.csv.");
			}

			if(element.type_code .find('f') != string::npos)
			{
				m_LinkTypeFreeway = element.link_type;
			}

			if(element.type_code .find('a') != string::npos)
			{
				m_LinkTypeArterial = element.link_type;
			}

			if(element.type_code .find('h') != string::npos)
			{
				m_LinkTypeHighway = element.link_type;
			}


			m_LinkTypeMap[element.link_type] = element;

			lineno++;
		}

		CString msg;
		msg.Format("Imported %d link types from file %s",m_LinkTypeMap.size(),lpszFileName);
		m_MessageStringVector.push_back (msg);
		m_AMSLogFile << "Read " << m_LinkTypeMap.size() << " link types from file "  << lpszFileName << endl; 

		return true;
	}else
	{
		return false;

	}

}
bool CTLiteDoc::ReadTemporalDemandProfileCSVFile(LPCTSTR lpszFileName)
{
	long lineno = 1;
	CCSVParser parser;

	if (parser.OpenCSVFile(lpszFileName))
	{
		while(parser.ReadRecord())
		{

			int from_zone_id, to_zone_id;
			int demand_type;

			if(parser.GetValueByFieldName("from_zone_id",from_zone_id) == false)
				break;

			if(parser.GetValueByFieldName("to_zone_id",to_zone_id) == false)
				break;

			if(parser.GetValueByFieldName("demand_type",demand_type) == false)
				break;

			string time_series_label;
			parser.GetValueByFieldName("time_series_label",time_series_label);


			DTADemandProfile element; 

			element.demand_type = demand_type;
			element.from_zone_id = from_zone_id;
			element.to_zone_id = to_zone_id;

			element.series_name = time_series_label.c_str ();

			for(int t = 0; t< MAX_TIME_INTERVAL_SIZE; t++)
			{
				CString time_stamp_str = GetTimeStampStrFromIntervalNo (t,true);

				// Convert a TCHAR string to a LPCSTR
				CT2CA pszConvertedAnsiString (time_stamp_str);

				// construct a std::string using the LPCSTR input
				std::string strStd (pszConvertedAnsiString);

				float percentage = 0.0f;
				parser.GetValueByFieldName(strStd,percentage);

				if( percentage>0.00001)
					element.time_dependent_ratio[t] = percentage;

			}

			m_DemandProfileVector.push_back (element);

			lineno++;
		}
	}
	m_AMSLogFile << "Read " << m_DemandProfileVector.size() << " temporal demand elements from file "  << lpszFileName << endl; 

	CString msg;
	msg.Format("Imported %d temporal demand elements from file %s",m_DemandProfileVector.size(),lpszFileName);
	m_MessageStringVector.push_back (msg);

	if(m_DemandProfileVector.size()>=1)
		return true;
	else
		return false;

	return true;
}

bool CTLiteDoc::Read3ColumnTripTxtFile(LPCTSTR lpszFileName)
{

	FILE* st = NULL;
	fopen_s(&st,lpszFileName,"r");

	long lineno = 1;
	if(st!=NULL)
	{


		while(!feof(st))
		{
			int origin_zone	       = g_read_integer(st);
			if(origin_zone == -1)  // reach end of file
				break;

			int destination_zone	   = g_read_integer(st);  // we might read "Origin" here, so if we find "O", we return 0 as a key to jump to the next origin.

			if(origin_zone > m_ODSize)
			{
				CString msg;
				msg.Format ("Error at line %d: origin zone number = %d, which is greater than the number of OD zones: %d.",lineno,origin_zone, m_ODSize);
				break;
			}

			if(destination_zone > m_ODSize)
			{
				CString msg;
				msg.Format ("Error at line %d: destination zone number = %d, which is greater than the number of OD zones: %d.",lineno,origin_zone, m_ODSize);
				break;
			}

			float number_of_vehicles = g_read_float(st);

			m_ZoneMap[origin_zone].m_ODDemandMatrix [destination_zone].SetValue (1, number_of_vehicles);

			lineno++;
		}
		fclose(st);
		m_DemandDataLoadingStatus.Format ("%d demand entries are loaded from file %s.",lineno,lpszFileName);
		return true;
	}else
	{
		AfxMessageBox("Error: OD Trip file cannot be opened.");
		return false;
		//		g_ProgramStop();
	}
}
bool CTLiteDoc::ReadTripTxtFile(LPCTSTR lpszFileName)
{
	FILE* st = NULL;
	fopen_s(&st,lpszFileName,"r");

	long lineno = 0;
	if(st!=NULL)
	{

		while(!feof(st))
		{
			for(int i = 1; i <=m_ODSize; i++)
			{
				int origin_zone	       = g_read_integer(st);
				if(origin_zone == -1)  // reach end of file
					break;

				for(int j = 1; j <=m_ODSize; j++)
				{
					int destination_zone	   = g_read_integer_with_char_O(st);  // we might read "Origin" here, so if we find "O", we return 0 as a key to jump to the next origin.

					if(destination_zone == 0) // we reach "O" of string Origin 
					{
						break;
					}

					destination_zone = j;  // use destination that we read to override j

					float number_of_vehicles = g_read_float(st);

					m_ZoneMap[origin_zone].m_ODDemandMatrix [destination_zone].SetValue (1, number_of_vehicles);
					lineno++;
				}
			}
		}
		fclose(st);
		m_DemandDataLoadingStatus.Format ("%d demand entries are loaded from file %s.",lineno,lpszFileName);
		return true;
	}else
	{
		AfxMessageBox("Error: OD Trip file cannot be opened.");
		return false;
		//		g_ProgramStop();
	}
}

bool CTLiteDoc::ReadScenarioData()
{

	// clean up all scenario data first
	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->CapacityReductionVector .clear();
		(*iLink)->TollVector .clear();
		(*iLink)->MessageSignVector.clear ();

	}

	//  Dynamic Message Sign

	// clean up all scenario data first
	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->CapacityReductionVector .clear();

	}
	int i =0;
	i+=ReadIncidentScenarioData();
	i+=ReadWorkZoneScenarioData();
	i+=ReadTrafficControlScenarioData();
	i+=ReadVMSScenarioData();
	i+=ReadLink_basedTollScenarioData();

	if(i > 0)
	{
		m_ScenarioDataLoadingStatus.Format ("%d scenario records are loaded.",i);
	}

	return true;
}


void CTLiteDoc::SendTexttoStatusBar(CString str, int index)
{
	POSITION pos = GetFirstViewPosition();
	CView *p = (CView *)this->GetNextView(pos);
	CStatusBar* bar = (CStatusBar*)(p->GetParentFrame()->GetMessageBar());
	bar->SetPaneText(index, str);
}


bool  CTLiteDoc::SaveSubareaDemandFile()
{
	FILE* st = NULL;
	FILE* st_meta_data = NULL;
	CString directory;

	if(m_ProjectFile.GetLength () ==0 )
	{
		AfxMessageBox("The project directory has not been specified. Please save the project to a new folder first.");
		return false;
	}

	if(m_bSaveProjectFromSubareaCut == true)  // after subarea cut, we need to regenerate demand matrix
	{
		// export path flow file 
		directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

		ExportPathflowToCSVFiles();

		fopen_s(&st_meta_data,directory+"input_demand_file_list.csv","w");
		if(st_meta_data!=NULL)
		{
			fprintf(st_meta_data,"scenario_no,file_sequence_no,file_name,format_type,number_of_lines_to_be_skipped,loading_multiplier,start_time_in_min,end_time_in_min,apply_additional_time_dependent_profile,subtotal_in_last_column,number_of_demand_types,demand_type_1,demand_type_2,demand_type_3,demand_type_4,'00:00,'00:15,'00:30,'00:45,'01:00,'01:15,'01:30,'01:45,'02:00,'02:15,'02:30,'02:45,'03:00,'03:15,'03:30,'03:45,'04:00,'04:15,'04:30,'04:45,'05:00,'05:15,'05:30,'05:45,'06:00,'06:15,'06:30,'06:45,'07:00,'07:15,'07:30,'07:45,'08:00,'08:15,'08:30,'08:45,'09:00,'09:15,'09:30,'09:45,'10:00,'10:15,'10:30,'10:45,'11:00,'11:15,'11:30,'11:45,'12:00,'12:15,'12:30,'12:45,'13:00,'13:15,'13:30,'13:45,'14:00,'14:15,'14:30,'14:45,'15:00,'15:15,'15:30,'15:45,'16:00,'16:15,'16:30,'16:45,'17:00,'17:15,'17:30,'17:45,'18:00,'18:15,'18:30,'18:45,'19:00,'19:15,'19:30,'19:45,'20:00,'20:15,'20:30,'20:45,'21:00,'21:15,'21:30,'21:45,'22:00,'22:15,'22:30,'22:45,'23:00,'23:15,'23:30,'23:45\n");
			fprintf(st_meta_data,"0,1,AMS_OD_table.csv,column,1,1,%d,%d,0,0,1,1\n",m_DemandLoadingStartTimeInMin,m_DemandLoadingEndTimeInMin);
			// m_DemandLoadingStartTimeInMin,m_DemandLoadingEndTimeInMin are read from the original project
			fclose(st_meta_data);

			AfxMessageBox("File input_demand_file_list.csv is reset with AMS_OD_table.csv as a new demand input file, as a result of the subarea cut.", MB_ICONINFORMATION);
		}else
		{
			AfxMessageBox("Error: File input_demand_file_list.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
			return false;
		}


		m_bSaveProjectFromSubareaCut = false;
	}
	return true;
}


bool  CTLiteDoc::SaveNewDemandMatrixFile()
{
	FILE* st = NULL;
	FILE* st_meta_data = NULL;
	CString directory;

	if(m_ProjectFile.GetLength () ==0 )
	{
		AfxMessageBox("The project directory has not been specified. Please save the project to a new folder first.");
		return false;
	}

	if(m_DemandMatrixMap.size() > 0 )  // after subarea cut, we need to regenerate demand matrix
	{
		// export path flow file 
		directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

		ExportPathflowToCSVFiles();

		fopen_s(&st_meta_data,directory+"input_demand_file_list.csv","w");
		if(st_meta_data!=NULL)
		{
			fprintf(st_meta_data,"scenario_no,file_sequence_no,file_name,format_type,number_of_lines_to_be_skipped,loading_multiplier,start_time_in_min,end_time_in_min,apply_additional_time_dependent_profile,subtotal_in_last_column,number_of_demand_types,demand_type_1,demand_type_2,demand_type_3,demand_type_4,'00:00,'00:15,'00:30,'00:45,'01:00,'01:15,'01:30,'01:45,'02:00,'02:15,'02:30,'02:45,'03:00,'03:15,'03:30,'03:45,'04:00,'04:15,'04:30,'04:45,'05:00,'05:15,'05:30,'05:45,'06:00,'06:15,'06:30,'06:45,'07:00,'07:15,'07:30,'07:45,'08:00,'08:15,'08:30,'08:45,'09:00,'09:15,'09:30,'09:45,'10:00,'10:15,'10:30,'10:45,'11:00,'11:15,'11:30,'11:45,'12:00,'12:15,'12:30,'12:45,'13:00,'13:15,'13:30,'13:45,'14:00,'14:15,'14:30,'14:45,'15:00,'15:15,'15:30,'15:45,'16:00,'16:15,'16:30,'16:45,'17:00,'17:15,'17:30,'17:45,'18:00,'18:15,'18:30,'18:45,'19:00,'19:15,'19:30,'19:45,'20:00,'20:15,'20:30,'20:45,'21:00,'21:15,'21:30,'21:45,'22:00,'22:15,'22:30,'22:45,'23:00,'23:15,'23:30,'23:45\n");
			fprintf(st_meta_data,"0,1,input_demand.csv,matrix,1,1,%d,%d,0,0,1,1\n",m_DemandLoadingStartTimeInMin,m_DemandLoadingEndTimeInMin);
			// m_DemandLoadingStartTimeInMin,m_DemandLoadingEndTimeInMin are read from the original project
			fclose(st_meta_data);
		}else
		{
			AfxMessageBox("Error: File input_demand_file_list.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
			return false;
		}


		
		fopen_s(&st,directory+"input_demand.csv","w");
			if(st!=NULL)
			{
				fprintf(st,"zone_id");
				std::map<int, DTAZone>	:: const_iterator itr;

				for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
				{
					fprintf(st,",%d",itr->first );
				}


				fprintf(st,"\n");

				std::map<int, DTAZone>	:: const_iterator itr_to_zone_id;

				int index = 0;
				for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
				{
					fprintf(st,"%d,",itr->first );

					for(itr_to_zone_id = m_ZoneMap.begin(); itr_to_zone_id != m_ZoneMap.end(); itr_to_zone_id++)
					{
						float value = GetODDemandValue(1,itr->first,itr_to_zone_id->first );
						{
							fprintf(st,"%f,", value);
						}
					}

					fprintf(st,"\n");

				}

				fclose(st);

			}else
			{
				AfxMessageBox("Error: File input_demand.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
				return false;

			}
		m_bSaveProjectFromImporting = false;
	}
	return true;
}

void  CTLiteDoc::CopyDefaultFiles(CString directory)
{

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	CString DefaultDataFolder;

	DefaultDataFolder.Format ("%s\\default_data_folder\\",pMainFrame->m_CurrentDirectory);

	if(directory.GetLength () == 0)
		directory = m_ProjectDirectory;


	if(m_bDYNASMARTDataSet)
	{
		CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"input_node_control_type_dsp.csv","input_node_control_type.csv",true);
		CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"input_link_type_dsp.csv","input_link_type.csv",true);
		WriteInputDemandMetaDataForDSP(directory);

	}else
	{
		//CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"AMS_traffic_data_settings.csv");
		CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"input_node_control_type.csv");
		CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"input_link_type.csv");

		if(m_DemandMatrixMap.size() >0)  // we have imported data, so we delete the file first
		{
			DeleteFile(m_ProjectDirectory+"input_demand_file_list.csv");
		}

		CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"input_demand_meta_data_matrix.csv","input_demand_file_list.csv");

	}

	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"optional_vehicle_emission_rate.csv");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"input_scenario_settings.csv");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"input_vehicle_type.csv");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"input_zone.csv");
	CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,directory,"input_activity_location.csv");



}

BOOL CTLiteDoc::SaveLinkData(LPCTSTR lpszPathName,bool bExport_Link_MOE_in_input_link_CSF_File, int SelectedLayNo)
{
	CWaitCursor wait;

	// reidentify bottlenecks;

	int StartHour = m_DemandLoadingStartTimeInMin/60;
	int EndHour = int((m_DemandLoadingEndTimeInMin+59)/60);

	int Start15Min =  (m_DemandLoadingStartTimeInMin/15)*15;
	int End15Min =  int((m_DemandLoadingEndTimeInMin+14)/15)*15;


	FILE* st;
	fopen_s(&st,lpszPathName,"w");
	if(st!=NULL)
	{
		std::list<DTALink*>::iterator iLink;
		fprintf(st,"name,link_id,link_key,speed_sensor_id,count_sensor_id,from_node_id,to_node_id,link_type_name,direction,length,number_of_lanes,speed_limit,lane_cap,link_type,jam_density,wave_speed,demand_type_code,mode_code,geometry,original_geometry,");
		fprintf(st,"map_matching_orientation_code,map_matching_loop_code,");

		if(bExport_Link_MOE_in_input_link_CSF_File)  // save time-dependent MOE
		{
			int hour;
			for(hour =StartHour; hour <= EndHour; hour++)
			{
				CString str_MOE_hour;
				str_MOE_hour.Format ("h%d_link_volume,",hour);
				fprintf(st,str_MOE_hour);
			}

			for(hour = StartHour; hour <= EndHour; hour++)
			{
				CString str_MOE_hour;
				str_MOE_hour.Format ("h%d_speed,",hour);
				fprintf(st,str_MOE_hour);
			}

			for(hour =StartHour; hour <= EndHour; hour++)
			{
				CString str_MOE_hour;
				str_MOE_hour.Format ("h%d_travel_time,",hour);
				fprintf(st,str_MOE_hour);
			}

			int min;
			CString str_MOE_15Min;

			for(min = Start15Min; min <= End15Min; min+=15)
			{
				str_MOE_15Min.Format ("%s_link_volume,",GetTimeStampString(min));
				fprintf(st,str_MOE_15Min);
			}

			for(min = Start15Min; min <= End15Min; min+=15)
			{
				str_MOE_15Min.Format ("%s_speed,",GetTimeStampString(min));
				fprintf(st,str_MOE_15Min);
			}

			for(min = Start15Min; min <= End15Min; min+=15)
			{

				str_MOE_15Min.Format ("%s_travel_time,",GetTimeStampString(min));
				fprintf(st,str_MOE_15Min);
			}
		}

		fprintf(st,"\n");	

		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			if((*iLink)->m_LayerNo == SelectedLayNo && (*iLink)->m_FromNodeID != (*iLink)->m_ToNodeID)
			{
				int ToNodeID = (*iLink)->m_ToNodeID ;
				DTANode* pNode = m_NodeNoMap[ToNodeID];
				//set default green time 
				if(pNode->m_ControlType == m_ControlType_PretimedSignal || 
					pNode->m_ControlType == m_ControlType_ActuatedSignal)
				{

				}


				CString link_type_name = " ";

				if(m_LinkTypeMap.find((*iLink)->m_link_type) != m_LinkTypeMap.end())
				{
					link_type_name = m_LinkTypeMap[(*iLink)->m_link_type].link_type_name.c_str ();
				}

				int sign_flag = 1;
				if(SelectedLayNo == 1)  // external layer
					sign_flag = -1;

				std::replace( (*iLink)->m_Name.begin(), (*iLink)->m_Name.end(), ',', ' '); 

				fprintf(st,"%s,%d,%s,%s,%s,%d,%d,%s,%d,%.5f,%d,%.4f,%.4f,%d,%.1f,%.1f,\"%s\",\"%s\",",
					(*iLink)->m_Name.c_str (),
					(*iLink)->m_LinkID, 
					(*iLink)->m_LinkKey , 
					(*iLink)->m_SpeedSensorID .c_str (),
					(*iLink)->m_CountSensorID .c_str (),
					

					(*iLink)->m_FromNodeNumber*sign_flag, 
					(*iLink)->m_ToNodeNumber*sign_flag, 
					link_type_name,
					(*iLink)->m_Direction,
					(*iLink)->m_Length ,
					(*iLink)->m_NumberOfLanes ,
					(*iLink)->m_SpeedLimit,
					(*iLink)->m_LaneCapacity ,(*iLink)->m_link_type,(*iLink)->m_Kjam, (*iLink)->m_Wave_speed_in_mph, 
					(*iLink)->m_demand_type_code.c_str(),

					(*iLink)->m_Mode_code.c_str ());

				// geometry
				fprintf(st,"\"<LineString><coordinates>");

				for(unsigned int si = 0; si< (*iLink)->m_ShapePoints.size(); si++)
				{
					fprintf(st,"%f,%f,0.0",(*iLink)->m_ShapePoints[si].x, (*iLink)->m_ShapePoints[si].y);

					if(si!=(*iLink)->m_ShapePoints.size()-1)
						fprintf(st," ");
				}

				fprintf(st,"</coordinates></LineString>\",");

				// original_geometry
				fprintf(st,"\"<LineString><coordinates>");

				for(unsigned int si = 0; si< (*iLink)->m_Original_ShapePoints.size(); si++)
				{
					fprintf(st,"%f,%f,0.0",(*iLink)->m_Original_ShapePoints[si].x, (*iLink)->m_Original_ShapePoints[si].y);

					if(si!=(*iLink)->m_Original_ShapePoints.size()-1)
						fprintf(st," ");
				}

				fprintf(st,"</coordinates></LineString>\",");

				fprintf(st,"%s,%s,",				(*iLink)->m_orientation_code.c_str(),
					(*iLink)->m_loop_code.c_str());

				if(bExport_Link_MOE_in_input_link_CSF_File)  // save time-dependent MOE
				{ 
					int hour;
					for(hour = StartHour; hour <= EndHour; hour++)
					{ 
						fprintf(st,"%f,", (*iLink)->GetAvgLinkHourlyVolume(hour*60,(hour+1)*60));
					}

					for(hour = StartHour; hour <= EndHour; hour++)
					{ 
						float avg_travel_time = (*iLink)->m_Length *60/max (1,(*iLink)->GetAvgLinkSpeed (hour*60,(hour+1)*60));
						fprintf(st,"%.2f,", (*iLink)->GetAvgLinkSpeed (hour*60,(hour+1)*60));
					}
					for(hour =StartHour; hour <= EndHour; hour++)
					{ 
						float avg_travel_time = (*iLink)->m_Length *60/max (1,(*iLink)->GetAvgLinkSpeed (hour*60,(hour+1)*60));
						fprintf(st,"%.2f,", avg_travel_time );
					}

					int min;


					for(min = Start15Min; min <= End15Min; min+=15)
					{ 
						fprintf(st,"%f,", (*iLink)->GetAvgLinkHourlyVolume(min,min+15));
					}

					for(min = Start15Min; min <= End15Min; min+=15)
					{ 
						fprintf(st,"%.2f,", (*iLink)->GetAvgLinkSpeed (min,min+15));
					}
					for(min = Start15Min; min <= End15Min; min+=15)
					{ 
						float avg_travel_time = (*iLink)->m_Length *60/max (1,(*iLink)->GetAvgLinkSpeed (min,min+15));
						fprintf(st,"%.2f,", avg_travel_time );
					}

				}

				fprintf(st,"\n");
			}  // layer no ==0
		}

		fclose(st);
	}else
	{
		CString message;
		message.Format("Error: File %s cannot be opened.\nIt might be currently used and locked by EXCEL.", lpszPathName);
		AfxMessageBox(message);
		return false;
	}
}

BOOL CTLiteDoc::SaveNodeFile()
{

	CString  directory = m_ProjectDirectory;
	FILE* st = NULL;


	fopen_s(&st,directory+"input_node.csv","w");
	if(st!=NULL)
	{
		std::list<DTANode*>::iterator iNode;
		fprintf(st, "name,node_id,zone_id,control_type,control_type_name,cycle_length_in_second,x,y,geometry\n");
		for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{
					if((*iNode)->m_LayerNo == 0) 
			{

				//if((*iNode)->m_IncomingLinkVector.size()>=3)  // more than 2 movements
				//	(*iNode)->m_ControlType = m_ControlType_PretimedSignal;

				CString control_type_name="";

				if( m_NodeTypeMap.find((*iNode)->m_ControlType)!= m_NodeTypeMap.end())				{
					control_type_name = m_NodeTypeMap[(*iNode)->m_ControlType].c_str() ;
				}

				if((*iNode)->m_ControlType != m_ControlType_PretimedSignal && (*iNode)->m_ControlType != m_ControlType_ActuatedSignal)
				{
					(*iNode)->m_CycleLengthInSecond = 0;
					(*iNode)->m_SignalOffsetInSecond = 0;
				}

				std::replace( (*iNode)->m_Name.begin(), (*iNode)->m_Name.end(), ',', ' '); 

				//replace with original node number in case synchro exporting function has changed the node number
				if((*iNode)->m_NodeOriginalNumber >=1) 
				{
					(*iNode)->m_NodeNumber = (*iNode)->m_NodeOriginalNumber;

				}
				fprintf(st, "%s,%d,%d,%d,%s,%d,%f,%f,\"<Point><coordinates>%f,%f</coordinates></Point>\"\n", 
					(*iNode)->m_Name.c_str (), (*iNode)->m_NodeNumber , 
					(*iNode)->m_ZoneID,
					(*iNode)->m_ControlType,   control_type_name, 
					(*iNode)->m_CycleLengthInSecond,
					(*iNode)->pt .x, (*iNode)->pt .y,(*iNode)->pt .x, (*iNode)->pt .y);
			}
		}

		fclose(st);
	}else
	{
		AfxMessageBox("Error: File input_node.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}
}

bool CTLiteDoc::SaveSensorCountData(bool CheckInValidData)
{
	CString  directory = m_ProjectDirectory;
	FILE* st = NULL;

	FILE* st_invalid = NULL;

	fopen_s(&st, directory + "sensor_count.csv", "w");

	if (CheckInValidData == true)
	{
		fopen_s(&st_invalid, directory + "sensor_count_invalid.csv", "w");

		if (st_invalid == NULL)
		{
			AfxMessageBox("Error: File sensor_count_invalid.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
			return false;
		}
	}

	if (st != NULL)
	{
		fprintf(st, "count_sensor_id,day_no,start_time_in_min,end_time_in_min,direction,link_count,derived_lane_hourly_volume,link_name,matched_link_name,matched_link_from_node_id,matched_link_to_node_id,matched_link_type,matched_link_number_of_lanes\n");

		if (st_invalid != NULL)
		{
			fprintf(st_invalid, "count_sensor_id,day_no,start_time_in_min,end_time_in_min,direction,link_count,derived_lane_hourly_volume,link_name, derived_lane_hourly_volume_max,derived_lane_hourly_volume_min,matched_link_name,matched_link_from_node_id,matched_link_to_node_id,matched_link_type,matched_link_number_of_lanes\n");
		}
		for (int i = 0; i< m_SensorCountVector.size(); i++)
		{
			DTASensorData element = m_SensorCountVector[i];

			DTALink* pLink = FindLinkWithNodeNumbers(element.link_from_node_id, element.link_to_node_id);

			if (pLink != NULL && element.b_valid_data == true && element.count_sensor_id.size()>0)
			{
				fprintf(st, "%s,%d,%d,%d,%s,%f,%f,%s,%d,%d,%s,%d\n",
					element.count_sensor_id.c_str(),
					element.day_no,
					element.start_time_in_min,
					element.end_time_in_min,
					element.direction.c_str(),
					element.count,
					element.derived_lane_hourly_volume,
					pLink->m_Name .c_str(),
					pLink->m_FromNodeNumber,
					pLink->m_ToNodeNumber,
					m_LinkTypeMap[pLink->m_link_type].link_type_name.c_str(),
					pLink->m_NumberOfLanes

					);
			}
			else if (CheckInValidData == true && st_invalid!=NULL)
			{
				fprintf(st_invalid, "%s,%d,%d,%d,%s,%f,%f,%s,%f,%f,%d,%d,%s,%d\n",
					element.count_sensor_id.c_str(),
					element.day_no,
					element.start_time_in_min,
					element.end_time_in_min,
					element.direction.c_str(),
					element.count,
					element.derived_lane_hourly_volume,
					pLink->m_Name.c_str(),
					pLink->m_sensor_hourly_lane_volume_max,
					pLink->m_sensor_hourly_lane_volume_min,
					pLink->m_FromNodeNumber,
					pLink->m_ToNodeNumber,
					m_LinkTypeMap[pLink->m_link_type].link_type_name.c_str(),
					pLink->m_NumberOfLanes

					);
				

			}
			
		}

		fclose(st);
		if (CheckInValidData == true && st_invalid!=NULL)
		{
			fclose(st_invalid);
		}
	}
	else
	{
		AfxMessageBox("Error: File sensor_count.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}
	return true;
}

BOOL CTLiteDoc::SaveProject(LPCTSTR lpszPathName, int SelectedLayNo)
{
	Modify(false);
	FILE* st = NULL;
	CString directory;
	CString prj_file = lpszPathName;
	directory = prj_file.Left(prj_file.ReverseFind('\\') + 1);

	CWaitCursor wc;

	CString OldDirectory = m_ProjectDirectory;

	if(OldDirectory!=directory)
	{

		// copy output files
		CopyFile(OldDirectory+"output_LinkMOE.csv", directory+"output_LinkMOE.csv", FALSE);
		CopyFile(OldDirectory+"LinkStaticMOE.csv", directory+"LinkStaticMOE.csv", FALSE);
		CopyFile(OldDirectory+"agent.bin", directory+"agent.bin", FALSE);
		CopyFile(OldDirectory+"output_LinkTDMOE.bin", directory+"output_LinkTDMOE.bin", FALSE);

		//		CopyFile(OldDirectory+"NetworkMOE_1min.csv", directory+"NetworkMOE_1min.csv", FALSE);

		CopyFile(OldDirectory+"demand.dat", directory+"demand.dat", FALSE);
		CopyFile(OldDirectory+"demand_HOV.dat", directory+"demand_HOV.dat", FALSE);
		CopyFile(OldDirectory+"demand_truck.dat", directory+"demand_truck.dat", FALSE);

	}

	// update m_ProjectDirectory
	CopyDefaultFiles(directory);
	m_ProjectDirectory = directory;

	char lpbuffer[64];

	sprintf_s(lpbuffer,"%f",m_NodeDisplaySize);

	WritePrivateProfileString("GUI", "node_display_size",lpbuffer,lpszPathName);


	if(m_LongLatFlag==true)
		WritePrivateProfileString("coordinate_info", "long_lat_flag","1",lpszPathName);
	else  // use km
		WritePrivateProfileString("coordinate_info", "long_lat_flag","0",lpszPathName);


	fopen_s(&st,directory+"input_node_control_type.csv","w");
	if(st!=NULL)
	{
		fprintf(st, "control_type_name,unknown_control,no_control,yield_sign,2way_stop_sign,4way_stop_sign,pretimed_signal,actuated_signal,roundabout\n");
		fprintf(st,"control_type");
		fprintf(st,",%d",m_ControlType_UnknownControl);
		fprintf(st,",%d",m_ControlType_NoControl);
		fprintf(st,",%d",m_ControlType_YieldSign);
		fprintf(st,",%d",m_ControlType_2wayStopSign);
		fprintf(st,",%d",m_ControlType_4wayStopSign);
		fprintf(st,",%d",m_ControlType_PretimedSignal);
		fprintf(st,",%d",m_ControlType_ActuatedSignal);
		fprintf(st,",%d",m_ControlType_Roundabout);
		fclose(st);
	}else
	{
		AfxMessageBox("Error: File input_node_control_type.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}




	sprintf_s(lpbuffer,"%f",m_NodeDisplaySize);
	WritePrivateProfileString("GUI","node_display_size",lpbuffer,lpszPathName);

	sprintf_s(lpbuffer,"%f",m_NodeTextDisplayRatio);
	WritePrivateProfileString("GUI","node_text_display_ratio",lpbuffer,lpszPathName);


	SaveNodeFile();

	SaveLinkData(directory+"input_link.csv",false,SelectedLayNo);

	//	SaveInputLanesFile(directory+"input_lanes.csv");

	if (m_LinkSet.size() <= _MAX_LINK_FOR_LOAD_MOVEMENT_DATA && m_bLoadMovementData == true)
	{
		SaveMovementData(directory + "input_movement.csv", -1, true);
	}

	if (bSynchroImportReadyToSaveFlag)
	{
		SaveTimingData();
		bSynchroImportReadyToSaveFlag = false;
	}

	//fopen_s(&st,directory+"input_phase.csv","w");
	//if(st!=NULL)
	//{
	//	std::list<DTANode*>::iterator iNode;
	//	fprintf(st, "node_name,node_id,phase_index,incoming_link_from_node_id,outgoing_link_to_node_id,turning_direction\n");
	//	for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	//	{
	//		if((*iNode)->m_LayerNo == SelectedLayNo) 
	//		{
	//			for(unsigned int p = 0; p< (*iNode)->m_PhaseVector .size(); p++)
	//			{
	//				for(unsigned int m = 0; m< (*iNode)->m_PhaseVector[p].movement_index_vector.size() ; m++)
	//				{
	//					int movement_index = (*iNode)->m_PhaseVector[p].movement_index_vector[m];;
	//					DTANodeMovement movement = (*iNode)->m_MovementDataMap["0"].m_MovementVector[movement_index];


	//					fprintf(st,"%s,%d,%d,%d,%d,%s\n",(*iNode)->m_Name.c_str (), 
	//						(*iNode)->m_NodeNumber ,
	//						p+1,
	//						m_NodeNotoNumberMap[movement.in_link_from_node_id],
	//						m_NodeNotoNumberMap[movement.out_link_to_node_id ],
	//						GetTurnString(movement.movement_turn));
	//				}
	//			}
	//		}
	//	}

	//	fclose(st);
	//}else
	//{
	//	AfxMessageBox("Error: File input_phase.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
	//	return false;
	//}
	if(m_bLoadNetworkDataOnly)  // only network data are loaded, no need to save the other data.
		return true; 

	// save zone here

	fopen_s(&st,directory+"input_zone.csv","w");
	if(st!=NULL)
	{
		fprintf(st,"zone_id,geometry\n");

		std::map<int, DTAZone>	:: const_iterator itr;

		for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
		{
			if(itr->second.m_bWithinSubarea && itr->first>=1)
			{
				//create activity location for empty zones

				fprintf(st, "%d,", 
					itr->first
					//itr->second .m_OriginTotalNumberOfVehicles , itr->second .m_OriginTotalTravelDistance /max(1,itr->second .m_OriginTotalNumberOfVehicles),
					//itr->second .m_DestinationTotalNumberOfVehicles , itr->second .m_DestinationTotalTravelDistance /max(1,itr->second .m_DestinationTotalNumberOfVehicles),
					//itr->second .color_code.c_str () , 
					//itr->second.m_Height,
					//itr->second .notes.c_str ()
					);

				fprintf(st,"\"<Polygon><outerBoundaryIs><LinearRing><coordinates>");
				for(unsigned int si = 0; si< itr->second.m_ShapePoints.size(); si++)
				{
					fprintf(st,"%f,%f,0.0",itr->second.m_ShapePoints[si].x, itr->second.m_ShapePoints[si].y);

					if(si!=itr->second.m_ShapePoints.size()-1)
						fprintf(st," ");
				}
				fprintf(st,"\</coordinates></LinearRing></outerBoundaryIs></Polygon>\"");
				fprintf(st, "\n");
			}
		}

		fclose(st);

	}else
	{
		AfxMessageBox("Error: File input_zone.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}

	fopen_s(&st,directory+"output_zone.csv","w");
	if(st!=NULL)
	{
		fprintf(st,"zone_id,total_production_capacity,total_production,avg_production_distance,total_attraction_capacity,total_attraction,avg_attraction_distance,geometry\n");

		std::map<int, DTAZone>	:: const_iterator itr;

		for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
		{
			if(itr->second.m_bWithinSubarea && itr->first>=1)
			{
				//create activity location for empty zones

				if(itr->first == 1065)
				{
					TRACE("");
				}

				fprintf(st, "%d,%f,%d,%f,%f,%d,%f,", 
					itr->first,
					itr->second .m_ProductionCapacity,
					itr->second .m_OriginTotalNumberOfVehicles , 
					itr->second .m_OriginTotalTravelDistance /max(1,itr->second .m_OriginTotalNumberOfVehicles),
					itr->second .m_AttractionCapacity,
					itr->second .m_DestinationTotalNumberOfVehicles , 
					itr->second .m_DestinationTotalTravelDistance /max(1,itr->second .m_DestinationTotalNumberOfVehicles)
					);

				fprintf(st,"\"<Polygon><outerBoundaryIs><LinearRing><coordinates>");
				for(unsigned int si = 0; si< itr->second.m_ShapePoints.size(); si++)
				{
					fprintf(st,"%f,%f,0.0",itr->second.m_ShapePoints[si].x, itr->second.m_ShapePoints[si].y);

					if(si!=itr->second.m_ShapePoints.size()-1)
						fprintf(st," ");
				}
				fprintf(st,"\"</coordinates></LinearRing></outerBoundaryIs></Polygon>\"");
				fprintf(st, "\n");
			}
		}

		fclose(st);

	}else
	{
		AfxMessageBox("Error: File output_zone.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}

	fopen_s(&st,directory+"input_activity_location.csv","w");
	if(st!=NULL)
	{
		fprintf(st,"zone_id,node_id,external_OD_flag\n");

		std::map<int, DTAZone>	:: iterator itr;

		for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); ++itr)
		{
			std::map<int, int > UsedActivityLocationMap;

			for(int i = 0; i< itr->second.m_ActivityLocationVector .size(); i++)
			{
				DTAActivityLocation element = itr->second.m_ActivityLocationVector[i];

				if(UsedActivityLocationMap.find(element.NodeNumber) == UsedActivityLocationMap.end ())  // has not been used
				{
					fprintf(st, "%d,%d,%d\n", itr->first , element.NodeNumber , element.External_OD_flag );
					UsedActivityLocationMap[element.NodeNumber] = 1;
				}
			}

		}
		fclose(st);

	}else
	{
		AfxMessageBox("Error: File input_activity_location.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}

	if(m_SubareaShapePoints.size() >0) // with subarea data only
	{
		fopen_s(&st,directory+"AMS_od_pairs.csv","w");
		if(st!=NULL)
		{
			fprintf(st,"from_zone_id,to_zone_id,from_node_id,to_node_id\n");

			std::map<int, DTAZone>	:: iterator itr;
			std::map<int, DTAZone>	:: iterator itr2;

			for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)  // from zone
			{

				for(itr2 = m_ZoneMap.begin(); itr2 != m_ZoneMap.end(); itr2++)  // to zone
				{

					if(itr->second.m_ActivityLocationVector .size() >0  && itr2->second.m_ActivityLocationVector .size() >0)
					{
						DTAActivityLocation element_from = itr->second.m_ActivityLocationVector[0];
						DTAActivityLocation element_to = itr2->second.m_ActivityLocationVector[0];
						fprintf(st, "%d,%d,%d,%d\n", element_from.ZoneID,element_to.ZoneID, element_from.NodeNumber,element_to.NodeNumber);
					}

				}			



			}
			fclose(st);

		}else
		{
			AfxMessageBox("Error: File input_od_pairs.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
			return false;
		}
		// save demand here
		fopen_s(&st, directory + "optional_subarea.csv", "w");
		if (st != NULL)
		{
			fprintf(st, "feature_id,x,y\n");
			for (unsigned int sub_i = 0; sub_i < m_SubareaShapePoints.size(); sub_i++)
			{
				fprintf(st, "%d,%f,%f\n", sub_i, m_SubareaShapePoints[sub_i].x, m_SubareaShapePoints[sub_i].y);
			}


			fclose(st);

		}
		else
		{
			AfxMessageBox("Error: File optional_subarea.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
			return false;
		}
	
	}



	if(m_VehicleTypeVector.size()==0)  // no data available, use default values
	{

		DTAVehicleType element;
		element.vehicle_type = 1; element.vehicle_type_name = "passenger car";  m_VehicleTypeVector.push_back (element);
		element.vehicle_type = 2; element.vehicle_type_name = "passenger truck";  m_VehicleTypeVector.push_back (element);
		element.vehicle_type = 3; element.vehicle_type_name = "light commercial truck";  m_VehicleTypeVector.push_back (element);
		element.vehicle_type = 4; element.vehicle_type_name = "single unit long-haul truck";  m_VehicleTypeVector.push_back (element);
		element.vehicle_type = 5; element.vehicle_type_name = "combination long-haul truck";  m_VehicleTypeVector.push_back (element);

	}

	//// save vehicle type info here  : xuesong: do not overwrite the vehicle type information, it is editable by users now
	//fopen_s(&st,directory+"input_vehicle_type.csv","w");
	//if(st!=NULL)
	//{

	//	fprintf(st,"vehicle_type,vehicle_type_name\n");
	//	for(std::vector<DTAVehicleType>::iterator itr = m_VehicleTypeVector.begin(); itr != m_VehicleTypeVector.end(); ++itr)
	//	{
	//		(*itr).vehicle_type_name.Replace (",", " ");
	//		{
	//			fprintf(st, "%d,%s\n", (*itr).vehicle_type , (*itr).vehicle_type_name);
	//		}

	//	}

	//	fclose(st);
	//}else
	//{
	//	AfxMessageBox("Error: File input_vehicle_type.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
	//	return false;
	//}


	// save demand type info here
	fopen_s(&st,directory+"input_demand_type.csv","w");
	if(st!=NULL)
	{
		fprintf(st,"demand_type,demand_type_name,avg_VOT,percentage_of_pretrip_info,percentage_of_enroute_info,");


		unsigned int i;
		for( i=0; i< m_VehicleTypeVector.size(); i++)
		{
			CString str_percentage_of_vehicle_type; 
			fprintf (st,"percentage_of_vehicle_type%d,", i+1);
		}
		fprintf (st,"\n");

		if(m_DemandTypeVector.size()==0)  // no data available, use default values
		{
			DTADemandType element;
			element.demand_type =1;
			element.demand_type_name = "SOV";
			element.vehicle_type_percentage[0] = 100;
			m_DemandTypeVector.push_back(element);

			element.demand_type =2;
			element.demand_type_name = "HOV";
			m_DemandTypeVector.push_back(element);

			element.demand_type =3;
			element.demand_type_name = "truck";
			m_DemandTypeVector.push_back(element);

			element.demand_type =4;
			element.demand_type_name = "intermodal";
			m_DemandTypeVector.push_back(element);

		}


		for(std::vector<DTADemandType>::iterator itr = m_DemandTypeVector.begin(); itr != m_DemandTypeVector.end(); ++itr)
		{
			(*itr).demand_type_name.Replace (",", " ");
			{
				fprintf(st, "%d,%s,%5.3f,%5.3f,%5.3f,%5.3f,", (*itr).demand_type, (*itr).demand_type_name, (*itr).average_VOT,
					
					(*itr).info_class_percentage[1], (*itr).info_class_percentage[2]);
			}

			for(i=0; i< m_VehicleTypeVector.size(); i++)
			{
				fprintf (st,"%5.3f,", (*itr).vehicle_type_percentage [i]);
			}
			fprintf (st,"\n");
		}

		fclose(st);
	}else
	{
		AfxMessageBox("Error: File input_demand_type.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}
	// save link type info here
	fopen_s(&st,directory+"input_link_type.csv","w");
	if(st!=NULL)
	{
		fprintf(st,"link_type,link_type_name,type_code,default_lane_capacity,default_speed_limit,default_number_of_lanes,travel_time_bias_factor,approximate_cycle_length_in_second,saturation_flow_rate_in_vhc_per_hour_per_lane\n");
		for(std::map<int, DTALinkType>::iterator itr = m_LinkTypeMap.begin(); itr != m_LinkTypeMap.end(); itr++)
		{
			std::replace( itr->second .link_type_name.begin(), itr->second .link_type_name.end(), ',', ' '); 

			if(itr->second .link_type_name.length () > 0 && itr->second .type_code.length () > 0)
			{

				fprintf(st, "%d,%s,%s,%f,%d,%.4f,%.4f\n", itr->first , 
					itr->second .link_type_name.c_str () , 
					itr->second .type_code .c_str (),
					itr->second .default_lane_capacity,
					itr->second .default_speed,
					itr->second .default_number_of_lanes,
					itr->second .travel_time_bias_factor);
			}
		}

		fclose(st);
	}else
	{
		AfxMessageBox("Error: File input_link_type.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}



	SaveSubareaDemandFile();

if(m_DemandMatrixMap.size()>0)
{
	SaveNewDemandMatrixFile();
}

	//// save demand if a matrix has been imported 
	//if(m_DemandMatrixMap.size()>0)
	//{

	//	CopyDefaultFiles(); // include input_demand_meta-database, input_vehicle type... 
		//// 3 column format
		//if(m_ImportDemandColumnFormat == true)
		//{
		//	fopen_s(&st,directory+"input_demand.csv","w");
		//	if(st!=NULL)
		//	{
		//		fprintf(st,"from_zone_id,to_zone_id,number_of_vehicles\n");


		//		std::map<CString, float>	:: const_iterator itr;

		//		for(itr = m_DemandMatrixMap.begin(); itr != m_DemandMatrixMap.end(); itr++)
		//		{
		//			int FileNo, OriginZone, DestinationZone;

		//			ParseODDemandKey((*itr).first ,FileNo, OriginZone, DestinationZone);

		//			if(FileNo==1)
		//			{
		//				fprintf(st,"%d,%d,%f\n",OriginZone, DestinationZone ,  (*itr).second  );
		//			}

		//		}
		//		fclose(st);

		//	}else
		//	{
		//		AfxMessageBox("Error: File input_demand.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		//		return false;
		//	}
		//	fclose(st);
		//}else
		//{


	OnFileSaveimagelocation();

	WriteLink_basedTollScenarioData();
	WriteVMSScenarioData();
	WriteWorkZoneScenarioData();

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	CString NEXTASettingsPath;
	NEXTASettingsPath.Format ("%s\\NEXTA_Settings.ini", pMainFrame->m_CurrentDirectory);

	CString color_str;
	color_str.Format("%d", theApp.m_FreewayColor);
	WritePrivateProfileString("initialization", "FreewayColor",color_str,NEXTASettingsPath);

	color_str.Format("%d", theApp.m_RampColor);
	WritePrivateProfileString("initialization", "RampColor",color_str,NEXTASettingsPath);

	color_str.Format("%d", theApp.m_ArterialColor );
	WritePrivateProfileString("initialization", "ArterialColor",color_str,NEXTASettingsPath);

	color_str.Format("%d", theApp.m_ConnectorColor  );
	WritePrivateProfileString("initialization", "ConnectorColor",color_str,NEXTASettingsPath);

	color_str.Format("%d", theApp.m_TransitColor  );
	WritePrivateProfileString("initialization", "TransitColor",color_str,NEXTASettingsPath);

	color_str.Format("%d", theApp.m_WalkingColor   );
	WritePrivateProfileString("initialization", "WalkingColor",color_str,NEXTASettingsPath);

	color_str.Format("%d", theApp.m_BackgroundColor   );
	WritePrivateProfileString("initialization", "BackgroundColor",color_str,NEXTASettingsPath);

	color_str.Format("%d", theApp.m_NodeColor    );
	WritePrivateProfileString("initialization", "NodeColor",color_str,NEXTASettingsPath);

	color_str.Format("%d", theApp.m_NodeBrushColor    );
	WritePrivateProfileString("initialization", "NodeBackgroundColor",color_str,NEXTASettingsPath);

	color_str.Format("%d", theApp.m_ZoneColor   );
	WritePrivateProfileString("initialization", "ZoneColor",color_str,NEXTASettingsPath);

	return true;
}
void CTLiteDoc::OnFileSaveProject()
{
	if(theApp.m_VisulizationTemplate == e_train_scheduling)
		return;

	if(m_ProjectDirectory.GetLength ()>3 && m_ProjectFile.GetLength()>3)
		SaveProject(m_ProjectFile);
	else
		OnFileSaveProjectAs();
}

void CTLiteDoc::OnFileSaveProjectAs()
{
	Modify(false);

	int number_of_signals  = 0;

	for (std::list<DTANode*>::iterator iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{


		if((*iNode)->m_ControlType == m_ControlType_PretimedSignal || (*iNode)->m_ControlType == m_ControlType_ActuatedSignal)
		{
			number_of_signals++;
		}

	}

	try{
		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

		CFileDialog fdlg (FALSE, "*.tnp", "*.tnp",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ENABLESIZING,
			"Transportation Network Project (*.tnp)|*.tnp|DYNASMART Workspace Files (*.dws)|*.dws|",pMainFrame,0,true);


		if(fdlg.DoModal()==IDOK)
		{
			CString path = fdlg.GetPathName  ();
			CWaitCursor wait;
			m_ProjectFile = path;

			m_ProjectTitle = GetWorkspaceTitleName(m_ProjectFile);

			bool bSavedFlag = false;
			if(path.Find("tnp")>=0)  //Transportation network project format
			{
				bSavedFlag = SaveProject(path);
			}else if(path.Find("dws")>=0)  //DYNASMART-P format
			{
				bSavedFlag = SaveDYNASMARTProject(path,false);
			}

			if(bSavedFlag)
			{
				CString msg;
				if(m_NodeSet.size()>0)
				{
					msg.Format ("Files input_node.csv, input_link.csv and input_zone.csv have been successfully saved with %d nodes, %d signals, %d links, %d zones.",
						m_NodeSet.size(), 
						number_of_signals,
						m_LinkSet.size(), 
						m_ZoneMap.size()
						);
					AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
				}/*else
				 {
				 msg.Format ("Files input_node.csv, input_link.csv and input_zone.csv have been successfully saved.");
				 AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
				 }*/
				SetTitle(m_ProjectTitle);


			}
		}

	} catch (char *str  ) 
	{
		AfxMessageBox(str,MB_ICONINFORMATION);
	}
	// TODO: Add your command handler code here
}

void CTLiteDoc::CalculateDrawingRectangle(bool NodeLayerOnly)
{   

	CWaitCursor wait;

	std::list<DTALink*>::iterator iLink;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->CalculateShapePointRatios();

	}

	bool bRectInitialized = false;
	m_AdjLinkSize = 0;

	for (std::list<DTANode*>::iterator iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{
		if(NodeLayerOnly || (*iNode)->m_Connections >0 )  // we might try import node layer only from shape file, so all nodes have no connected links. 
		{
			if(!bRectInitialized)
			{
				m_NetworkRect.left = (*iNode)->pt.x ;
				m_NetworkRect.right = (*iNode)->pt.x;
				m_NetworkRect.top = (*iNode)->pt.y;
				m_NetworkRect.bottom = (*iNode)->pt.y;
				bRectInitialized = true;
			}

			if((*iNode)->m_Connections > m_AdjLinkSize)
				m_AdjLinkSize = (*iNode)->m_Connections;

			m_NetworkRect.Expand((*iNode)->pt);
		}

	}

	for (std::list<DTAPoint*>::iterator iPoint = m_DTAPointSet.begin(); iPoint != m_DTAPointSet.end(); iPoint++)
	{
		if(!bRectInitialized)
		{
			m_NetworkRect.left = (*iPoint)->pt.x ;
			m_NetworkRect.right = (*iPoint)->pt.x;
			m_NetworkRect.top = (*iPoint)->pt.y;
			m_NetworkRect.bottom = (*iPoint)->pt.y;
			bRectInitialized = true;
		}

		m_NetworkRect.Expand((*iPoint)->pt);

	}

	if(m_BackgroundBitmapLoaded)  // if there is a bitmap background
	{

		if(!bRectInitialized)  // there is no node or link layer
		{
			m_NetworkRect.left = m_ImageX1;
			m_NetworkRect.right = m_ImageX2;
			m_NetworkRect.top = m_ImageY2;
			m_NetworkRect.bottom = m_ImageY1;
			bRectInitialized = true;

		}

	}

	for (std::list<DTALine*>::iterator iLine = m_DTALineSet.begin(); iLine != m_DTALineSet.end(); iLine++)
	{
		for(unsigned int i = 0; i< (*iLine)->m_ShapePoints .size(); i++)
		{
			if(!bRectInitialized)
			{
				m_NetworkRect.left = (*iLine)->m_ShapePoints[i].x ;
				m_NetworkRect.right = (*iLine)->m_ShapePoints[i].x;
				m_NetworkRect.top = (*iLine)->m_ShapePoints[i].y;
				m_NetworkRect.bottom = (*iLine)->m_ShapePoints[i].y;

				bRectInitialized = true;
			}else
			{
				m_NetworkRect.Expand((*iLine)->m_ShapePoints[i]);
			}
		}

	}



	if(theApp.m_VisulizationTemplate == e_train_scheduling)
	{

		CString str;
		str.Format("%d nodes and %d links are loaded.", m_NodeSet.size(), m_LinkSet.size());
		AfxMessageBox(str,MB_ICONINFORMATION);
	}


	IdentifyBottleNeckAndOnOffRamps();
}


bool CTLiteDoc::WriteSelectVehicleDataToCSVFile(LPCTSTR lpszFileName, std::vector<DTAVehicle*> VehicleVector)
{

	// vehicle_id,  origin_zone_id, destination_zone_id, departure_time,
	//	arrival_time, complete_flag, trip_time, demand_type, occupancy, information_type,
	//	value_of_time, path_min_cost,distance_in_mile, number_of_nodes,
	//	node id, node arrival time

	ofstream VehicleDataFile;
	VehicleDataFile.open (lpszFileName, ios::out);
	if (VehicleDataFile.is_open())
	{
		VehicleDataFile.width(12);
		VehicleDataFile.precision(4) ;
		VehicleDataFile.setf(ios::fixed);
		VehicleDataFile << "vehicle_id,origin_zone_id,destination_zone_id,departure_time, trip_time, complete_flag, demand_type,pricing_type,vehicle_type, information_class,value_of_time, toll_dollar_cost,emissions,distance_in_mile, number_of_nodes, <node id;node arrival time>" << endl;

		for(int v = 0; v < VehicleVector.size(); v++)
		{

			DTAVehicle* pVehicle = VehicleVector[v] ;

			if(pVehicle!=NULL)
			{
				VehicleDataFile << pVehicle->m_VehicleID << ",";
				VehicleDataFile << pVehicle->m_OriginZoneID << ",";
				VehicleDataFile << pVehicle->m_DestinationZoneID << ",";
				VehicleDataFile << pVehicle->m_DepartureTime << ",";
				VehicleDataFile << pVehicle->m_TripTime << ",";
				if(pVehicle->m_bComplete == 0)
					VehicleDataFile << "0,";
				else
					VehicleDataFile << "1,";

				VehicleDataFile << pVehicle->m_DemandType << ",";
				VehicleDataFile << pVehicle->m_PricingType << ",";
				VehicleDataFile << pVehicle->m_VehicleType << ",";
				VehicleDataFile << pVehicle->m_InformationClass << ",";
				VehicleDataFile << pVehicle->m_VOT << ",";
				VehicleDataFile << pVehicle->m_TollDollarCost << ",";
				VehicleDataFile << pVehicle->m_PM << ",";
				VehicleDataFile << pVehicle->m_Distance << ",";
				VehicleDataFile << pVehicle->m_NodeSize << ",\"";

				//for(int i=0; i< pVehicle->m_NodeSize-1; i++)
				//{
				//	VehicleDataFile << "<"; 

				//	if(pVehicle->m_NodeAry[i].LinkNo!=-1 && m_LinkNoMap.find (pVehicle->m_NodeAry[i].LinkNo) != m_LinkNoMap.end())
				//	{
				//	DTALink* pLink = m_LinkNoMap[pVehicle->m_NodeAry[i].LinkNo];
				//
				//	VehicleDataFile << pLink->m_FromNodeNumber << ";";
				//	VehicleDataFile << pVehicle->m_NodeAry[i].ArrivalTimeOnDSN << ">";
				//	}
				//}
				//VehicleDataFile << pVehicle->m_NodeSize << "\"";
				VehicleDataFile << endl;
			}
		}
		VehicleDataFile.close();
		return true;
	}

	return false;
}
void CTLiteDoc::ReadVehicleCSVFile_Parser(LPCTSTR lpszFileName)
{

	//   cout << "Read vehicle file... "  << endl;
	// vehicle_id,  origin_zone_id, destination_zone_id, departure_time,
	//	arrival_time, complete_flag, trip_time, demand_type, occupancy, information_type,
	//	value_of_time, path_min_cost,distance_in_mile, number_of_nodes,
	//	node id, node arrival time

	float LengthinMB;
	FILE* pFile;
	fopen_s(&pFile,lpszFileName,"rb");
	if(pFile!=NULL)
	{
		fseek(pFile, 0, SEEK_END );
		int Length = ftell(pFile);
		fclose(pFile);
		LengthinMB= Length*1.0/1024/1024;
		if(LengthinMB>50)
		{
			CString msg;
			msg.Format("The %s file is %5.1f MB in size.\nIt could take quite a while to load this file.\nWould you like to load the vehicle file?",lpszFileName,LengthinMB);
			if(AfxMessageBox(msg,MB_YESNO|MB_ICONINFORMATION)==IDNO)
				return;
		}
	}

	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{
		m_VehicleSet.clear();
		int count = 0;
		while(parser.ReadRecord())
		{


			int m_VehicleID= -1;
			parser.GetValueByFieldName("agent_id",m_VehicleID);
			if(m_VehicleID == -1)
				break;

			DTAVehicle* pVehicle = 0;
			pVehicle = new DTAVehicle;
			pVehicle->m_VehicleID		= m_VehicleID;
			parser.GetValueByFieldName("from_zone_id",pVehicle->m_OriginZoneID);
			parser.GetValueByFieldName("to_zone_id",pVehicle->m_DestinationZoneID);
			parser.GetValueByFieldName("origin_node_id", pVehicle->m_OriginNodeID);
			parser.GetValueByFieldName("destination_node_id", pVehicle->m_DestinationNodeID);

			pVehicle->m_bComplete = true;

			parser.GetValueByFieldName("departure_time_in_min",pVehicle->m_DepartureTime);
			parser.GetValueByFieldName("arrival_time_in_min",pVehicle->m_ArrivalTime);


			if(g_Simulation_Time_Horizon < pVehicle->m_ArrivalTime)
				g_Simulation_Time_Horizon = pVehicle->m_ArrivalTime;

			parser.GetValueByFieldName("travel_time_in_min",pVehicle->m_TripTime );

			float distance_in_mile = 0;

			parser.GetValueByFieldName("distance",pVehicle->m_Distance  );

			parser.GetValueByFieldName("demand_type",pVehicle->m_DemandType );
			parser.GetValueByFieldName("pricing_type",pVehicle->m_PricingType );
			parser.GetValueByFieldName("vehicle_type",pVehicle->m_VehicleType );
			parser.GetValueByFieldName("information_type",pVehicle->m_InformationClass );
			parser.GetValueByFieldName("VOT",pVehicle->m_VOT );
			parser.GetValueByFieldName("toll_cost",pVehicle->m_TollDollarCost );

			pVehicle->m_Distance = distance_in_mile;

			parser.GetValueByFieldName("number_of_nodes",pVehicle->m_NodeSize );

			std::string path_node_sequence,path_time_sequence;
			parser.GetValueByFieldName("path_node_sequence",path_node_sequence );
			parser.GetValueByFieldName("path_time_sequence",path_time_sequence );
			

			parser.GetValueByFieldName("TotalEnergy_(KJ)", pVehicle->m_EmissionData.Energy);
			parser.GetValueByFieldName("CO2_(g)", pVehicle->m_EmissionData.CO2);
			parser.GetValueByFieldName("NOX_(g)", pVehicle->m_EmissionData.NOX);
			parser.GetValueByFieldName("CO_(g)", pVehicle->m_EmissionData.CO);
			parser.GetValueByFieldName("HC_(g)", pVehicle->m_EmissionData.HC);
			parser.GetValueByFieldName("PM_(g)", pVehicle->m_EmissionData.PM);
			parser.GetValueByFieldName("PM_2.5(g)", pVehicle->m_EmissionData.PM2_5);


			std::vector<int> node_sequence;
			std::vector<float> time_sequence;

			g_ParserIntSequence(path_node_sequence, node_sequence);
			g_ParserFloatSequence(path_time_sequence, time_sequence);


			std::string node_sequence_node_timestamp;
			std::vector<float> node_time_sequence;

			if(parser.GetValueByFieldName("node_sequence_node:timestamp",node_sequence_node_timestamp ) && node_sequence_node_timestamp.size() >0)
			{


			g_ParserFloatSequence(node_sequence_node_timestamp, node_time_sequence);

			for(unsigned s = 0; s < node_time_sequence.size(); s+=2)
			{


				int node_number = node_time_sequence[s];
				node_sequence.push_back (node_number);
				time_sequence.push_back (node_time_sequence[s+1]);

			
			
			
			}


			}



			if(node_sequence.size() != pVehicle->m_NodeSize )
			{
				//CString str;
				//str.Format("path_node_sequence for trip_id %d should have %d nodes, but only %d nodes are provided. Please check.", 
				//	m_VehicleID, pVehicle->m_NodeSize,node_sequence.size()  );

				//AfxMessageBox(str);
				//break;

				continue;
			}

			if(time_sequence.size() != pVehicle->m_NodeSize )
			{
				//CString str;
				//str.Format("path_time_sequence for trip_id %d should have %d elements, but only %d elements are provided. Please check.", 
				//	m_VehicleID, pVehicle->m_NodeSize,time_sequence.size()  );

				//AfxMessageBox(str);
				continue;

			}
			if(pVehicle->m_NodeSize>=1)  // in case reading error
			{
				pVehicle->m_NodeAry = new SVehicleLink[pVehicle->m_NodeSize];

				pVehicle->m_NodeNumberSum = 0;
				for(int i=0; i< pVehicle->m_NodeSize; i++)
				{
					m_PathNodeVectorSP[i] = node_sequence[i];
					pVehicle->m_NodeNumberSum += m_PathNodeVectorSP[i];
					if(i>=1)
					{
						DTALink* pLink = FindLinkWithNodeNumbers(m_PathNodeVectorSP[i-1],m_PathNodeVectorSP[i]);
						if(pLink==NULL)
						{

							CString str;
							str.Format("path node sequence error for trip_id %d: link %d ->%d does not exist. Please check.", 
							m_VehicleID, m_PathNodeVectorSP[i-1],m_PathNodeVectorSP[i]  );

							continue;
						}
						pVehicle->m_NodeAry[i].LinkNo  = pLink->m_LinkNo ;
						pLink->m_TotalTravelTime +=  pVehicle->m_NodeAry[i].ArrivalTimeOnDSN - pVehicle->m_NodeAry[i-1].ArrivalTimeOnDSN;
					}

					// random error beyond 6 seconds for better ainimation

					float random_value = g_RNNOF()*0.01; // 0.1 min = 6 seconds


					pVehicle->m_NodeAry[i].ArrivalTimeOnDSN = time_sequence[i]+random_value;
				}

				m_VehicleSet.push_back (pVehicle);
				m_VehicleIDMap[pVehicle->m_VehicleID]  = pVehicle;


				count++;
			} 
		}

		UpdateMovementDataFromVehicleTrajector();

		m_SimulationVehicleDataLoadingStatus.Format ("%d vehicles are loaded from file %s.",count,lpszFileName);

	}
}



bool CTLiteDoc::SaveInputPathCSVFile(LPCTSTR lpszFileName)
{
	FILE * st;
	st = fopen(lpszFileName,"w");

	if(st!=NULL)
	{
		// header
		fprintf(st,"path_no,path_name,node_sequence,");

		for(int min  = 0; min < 1440; min +=15)
		{
			CString str = 		GetTimeStampStrFromIntervalNo (min/15,true);
			fprintf(st,"%s,",str);
		}

		fprintf(st,"\n");

		// 

		int path_count  = 1;
		int empty_path_count  = 0;

		for(unsigned int p = 0; p < m_PathDisplayList.size(); p++) // for each path
		{
			DTAPath path_element = m_PathDisplayList[p];

			if(path_element.m_LinkVector.size() > 1)  // do not save empty path
			{
				if(path_element.m_path_name.compare ("New Path") ==0)
				{
					CString PathTitle;
					PathTitle.Format(" ");

					path_element.m_path_name = PathTitle;
				}

				fprintf(st,"%d,%s,", path_count++,path_element.m_path_name.c_str ());

				for (int i=0 ; i < path_element.m_LinkVector.size(); i++)  // for each pass link
				{
					DTALink* pLink = m_LinkNoMap[m_PathDisplayList[p].m_LinkVector[i]];
					if(pLink != NULL)
					{
						fprintf(st,"%d;",
							pLink->m_FromNodeNumber);

					}

					if(i==path_element.m_LinkVector.size()-1)
					{
						if(pLink != NULL)
						{
							fprintf(st,"%d",
								pLink->m_ToNodeNumber);

						}

					}
				}


				// for time stame
				if(m_PathDisplayList[p].m_bWithSensorTravelTime)
				{
					for(int t = 0 ; t< 1440; t+= 15)  // for each starting time
					{
						if(m_PathDisplayList[p].m_SensorTimeDependentTravelTime[t] >=0.1)
							fprintf(st,"%.2f,", m_PathDisplayList[p].m_SensorTimeDependentTravelTime[t] );
						else
							fprintf(st,",");

					}
				}


				fprintf(st,"\n");

			}else
			{
				empty_path_count ++;
			}

		} //for each path

		if(empty_path_count >=1)
		{


		}
		fclose(st);
		return true;
	}else
	{
		AfxMessageBox("File optional_path.csv cannot be opened.");
		return false;

	}
}
void CTLiteDoc::ReadInputPathCSVFile(LPCTSTR lpszFileName)
{

	CString ErrorMessageVector;

	for(unsigned int p = 0; p < m_PathDisplayList.size(); p++) // for each path
	{

		m_PathDisplayList[p].m_LinkVector.clear();
	}

	m_PathDisplayList.clear();
	m_SelectPathNo=0;

	bool bEmptyFile = true;
	float LengthinMB;
	FILE* pFile;
	fopen_s(&pFile,lpszFileName,"rb");
	if(pFile!=NULL)
	{
		fseek(pFile, 0, SEEK_END );
		int Length = ftell(pFile);
		fclose(pFile);
		if(Length>0)
			bEmptyFile = false;

	}

	if(bEmptyFile)  // do not read empty file
	{
		// add a new path
		DTAPath path_element;

		path_element.m_path_name  = "New Path";

		m_PathDisplayList.push_back (path_element);

		m_SelectPathNo = m_PathDisplayList.size() -1;
		return;
	}
	int prev_path_id = -1;
	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{
		bool bNodeNonExistError = false;

		m_OriginNodeID = -1;
		m_DestinationNodeID = -1;

		int count = 0;
		DTALink* pLink =NULL;
		while(parser.ReadRecord())
		{
			int link_id = 0;
			int from_node_id;
			int to_node_id;

			int path_id = count;
			string path_name;
			parser.GetValueByFieldName("path_name",path_name);


			string node_sequence; 
			parser.GetValueByFieldName("node_sequence",node_sequence);

			if(path_name.size() ==0 && node_sequence.size() ==0)  // empty line
				break;
			//find reference travel time
			DTAPath path_element;

			path_element.m_path_name  = path_name;

			m_PathDisplayList.push_back (path_element);



			std::vector<int> node_sequence_vector;

			read_multiple_integers_from_a_string(node_sequence.c_str (), node_sequence_vector);

			int route_no = m_PathDisplayList.size()-1;

			if(node_sequence_vector.size() <=1)
				continue;

			for(unsigned int i = 0; i < node_sequence_vector.size()-1; i++)
			{

				pLink = FindLinkWithNodeNumbers(node_sequence_vector[i],node_sequence_vector[i+1],lpszFileName);


				if(pLink==NULL)
				{
					CString ErrorMessage;
					ErrorMessage.Format("Path Name %s: %d->%d does not exist.\n", path_name,node_sequence_vector[i],node_sequence_vector[i+1]);

					ErrorMessageVector+= ErrorMessage;
					return;
				}

				//if(count==0)
				//{
				//	m_OriginNodeID  = pLink->m_FromNodeID ;
				//}

				m_PathDisplayList[route_no].m_LinkVector.push_back (pLink->m_LinkNo );

			}



			if(node_sequence_vector.size()>=2)
			{
				for(int t = 0 ; t< 1440; t+= 15)  // for each starting time
				{
					CString str = 		GetTimeStampStrFromIntervalNo (t,true);

					std::string str_time = CString2StdString (str);

					float travel_time = 0;
					parser.GetValueByFieldName(str_time,travel_time);


					if(travel_time >0.1f)
					{
						m_PathDisplayList[route_no].m_bWithSensorTravelTime = true;
						for(int s = 0; s<15; s++)
						{

							m_PathDisplayList[route_no].m_SensorTimeDependentTravelTime[t+s] = travel_time;
						}
					}

				}

			}


			CString c_path_name;
			c_path_name.Format("%d",route_no+1);


			count++;
		}

		//if(pLink!=NULL)  // last link
		//{
		//	m_DestinationNodeID  = pLink->m_ToNodeID ;	
		//}

		// add a new path
		DTAPath path_element;

		path_element.m_path_name  = "New Path";

		m_PathDisplayList.push_back (path_element);

		m_SelectPathNo = m_PathDisplayList.size() -1;

	}else
	{
		AfxMessageBox("File optional_path.csv cannot be opened.");
	}
	m_PathDataLoadingStatus.Format ("%d paths are loaded from file %s.",m_PathDisplayList.size(),lpszFileName);

}
void CTLiteDoc::ReadAMSPathCSVFile(LPCTSTR lpszFileName)
{

	//   cout << "Read path file... "  << endl;
	// vehicle_id,  origin_zone_id, destination_zone_id, departure_time,
	//	arrival_time, complete_flag, trip_time, demand_type, occupancy, information_type,
	//	value_of_time, path_min_cost,distance_in_mile, number_of_nodes,
	//	node id, node arrival time

	float LengthinMB;
	FILE* pFile;
	fopen_s(&pFile,lpszFileName,"rb");
	if(pFile!=NULL)
	{
		fseek(pFile, 0, SEEK_END );
		int Length = ftell(pFile);
		fclose(pFile);
		LengthinMB= Length*1.0/1024/1024;
		if(LengthinMB>250)
		{
			CString msg;
			msg.Format("The AMS_path_flow.csv file is %5.1f MB in size.\nIt could take quite a while to load this file.\nWould you like to load the file?",LengthinMB);
			if(AfxMessageBox(msg,MB_YESNO|MB_ICONINFORMATION)==IDNO)
				return;
		}
	}


	int count = 0;

	FILE* st = NULL;
	fopen_s(&st,lpszFileName,"r");
	if(st!=NULL)
	{
		m_PathMap.clear();

		while(!feof(st))
		{
			int path_id = g_read_integer(st);
			if(path_id < 0)
				break;
			int vehicle_count = g_read_integer(st);
			int number_of_nodes = g_read_integer(st);

			CString label;
			label.Format("%d", path_id);

			//existing path
			m_PathMap[label].TotalVehicleSize  = vehicle_count;


			for(int i = 0; i< number_of_nodes; i++)
			{
				int node_number = g_read_integer(st);
				m_PathMap[label].m_NodeVector.push_back(node_number);

				if(i >=1)  // to be used when node number changes after generating physical links from connectors
				{
					DTALink* pLink = FindLinkWithNodeNumbers(m_PathMap[label].m_NodeVector[i-1],node_number);
					m_PathMap[label].m_LinkPointerVector .push_back(pLink);
				}
			}

			count++;
		} 
		fclose(st);
	}

	m_PathDataLoadingStatus.Format ("%d AMS paths are loaded from file %s.",count,lpszFileName);

}

void CTLiteDoc::ScanAMSTimingPlanCSVFile(LPCTSTR lpszFileName, int scenario_no = 0)
{

	//reset
	m_TimingPlanVector.clear();

	AddNameIntoTimingPlanVector(m_DemandLoadingStartTimeInMin , m_DemandLoadingEndTimeInMin,"0");

	CCSVParser parser;

	int count = 0;
	if (!parser.OpenCSVFile(lpszFileName))  //no such file exists.
	{
		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

		CString DefaultDataFolder;

		DefaultDataFolder.Format ("%s\\default_data_folder\\",pMainFrame->m_CurrentDirectory);

		//CopyDefaultFile(DefaultDataFolder,m_ProjectDirectory,m_ProjectDirectory,"AMS_timing_plan.csv","AMS_timing_plan.csv");

	}

	parser.CloseCSVFile ();

	if (parser.OpenCSVFile(lpszFileName))
	{
		while(parser.ReadRecord())
		{
			int scenario_no;
			int start_day_no;
			int end_day_no;


			if(parser.GetValueByFieldName("scenario_no",scenario_no) == false)
				break;
			if(parser.GetValueByFieldName("start_day_no",start_day_no) == false)
				break;
			if(parser.GetValueByFieldName("end_day_no",end_day_no) == false)
				break;

			for(int time = m_DemandLoadingStartTimeInMin ; time <m_DemandLoadingEndTimeInMin; time+=15)
			{

				CString str;
				str.Format("'%s", GetTimeStampString24HourFormat (time));

				std::string str_time =  CString2StdString(str);


				std::string timing_plan_name;

				parser.GetValueByFieldName(str_time,timing_plan_name);

				if(timing_plan_name.size () > 0 && timing_plan_name != "0")
				{
					AddNameIntoTimingPlanVector(time, time+15,timing_plan_name);
				}

			}


		}

	}


}
int CTLiteDoc::ReadAMSMovementCSVFile(LPCTSTR lpszFileName, int NodeNumber = -1)
{

	int number_of_nodes = 0;

	m_MovementPointerMap.clear();
	m_Node2MovementStr.clear();

	std::list<DTALink*>::iterator iLink;

	std::list<DTANode*>::iterator iNode;
	for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{
		if ((*iNode)->m_NodeNumber == 1) {
			int test = 1;
		}

		if((*iNode)->m_LayerNo == 0) 
		{
			for(int tp = 0; tp< m_TimingPlanVector.size(); tp++)  // first loop for each timing plan
			{
				std::string timing_plan_name = m_TimingPlanVector[tp].timing_plan_name;  // fetch timing_plan (unique) name

				for(unsigned int m = 0; m< (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector.size(); m++)
				{

					DTANodeMovement movement = (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m];

					CString label;
					int up_node_id = m_NodeNoMap[movement.in_link_from_node_id]->m_NodeNumber  ;
					int dest_node_id = m_NodeNoMap[movement.out_link_to_node_id ]->m_NodeNumber ;
					label.Format("%d;%d;%d:%s", up_node_id,(*iNode)->m_NodeNumber,dest_node_id, timing_plan_name.c_str ());

					m_MovementPointerMap[label] = &((*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m]); // store pointer
					m_Node2MovementStr[(*iNode)->m_NodeNumber].push_back(label);

					// set default value for through turns

					DTALink* pLink = FindLinkWithNodeNumbers(up_node_id,(*iNode)->m_NodeNumber, lpszFileName );
					if(pLink !=NULL)
					{

						switch ((*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m].movement_turn)
						{
						case DTA_Through: (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m].QEM_Lanes = pLink->m_NumberOfLanes; break;
						case DTA_LeftTurn: 
						case DTA_LeftTurn2:
							(*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m].QEM_Lanes = max((*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m].QEM_Lanes,
								pLink->m_NumberOfLeftTurnLanes) ; break;


						case DTA_RightTurn:
						case DTA_RightTurn2: 
							(*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m].QEM_Lanes = max( pLink->m_NumberOfRightTurnLanes,
								(*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m].QEM_Lanes); break;
						}

						(*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m].QEM_Speed  = pLink->m_SpeedLimit ;
					}else
					{
						return 0;
					}
				}

			}


		}
	}

	CCSVParser parser_movement;

	// here some function change the movement_approach_turn

	int count = 0;

	int current_node_id = -1;
	if (parser_movement.OpenCSVFile(lpszFileName))
	{
		while(parser_movement.ReadRecord())
		{
			int up_node_id, node_id, dest_node_id;

			std::string timing_plan_name;


			if(parser_movement.GetValueByFieldName("node_id",node_id) == false)
				break;

//			parser_movement.GetValueByFieldName("timing_plan_name",timing_plan_name);


			if(timing_plan_name.size() == 0)
				timing_plan_name = "0";

			if(current_node_id != node_id)
			{
				current_node_id = node_id;
				number_of_nodes++;
			}


			parser_movement.GetValueByFieldName("up_node_id",up_node_id);
			parser_movement.GetValueByFieldName("dest_node_id",dest_node_id);

			DTANode* pNode = FindNodeWithNodeNumber (node_id);


			if(m_NodeNumbertoNodeNoMap.find(up_node_id)== m_NodeNumbertoNodeNoMap.end() 
				|| m_NodeNumbertoNodeNoMap.find(node_id)== m_NodeNumbertoNodeNoMap.end() 
				|| m_NodeNumbertoNodeNoMap.find(dest_node_id)== m_NodeNumbertoNodeNoMap.end() )
			{
			continue;
			
			}
			CString movement_index = pNode->GetMovementIndex(m_NodeNumbertoNodeNoMap[up_node_id],
				m_NodeNumbertoNodeNoMap[node_id], m_NodeNumbertoNodeNoMap[dest_node_id]);


			CString label;
			label.Format("%d;%d;%d:%s", up_node_id,node_id,dest_node_id,timing_plan_name.c_str ());

			if(m_MovementPointerMap.find(label) != m_MovementPointerMap.end())
			{
				DTANodeMovement* pMovement = m_MovementPointerMap[label];

				parser_movement.GetValueByFieldName ("prohibited_flag",pMovement->turning_prohibition_flag);
				parser_movement.GetValueByFieldName ("protected_flag",pMovement->turning_protected_flag );
				parser_movement.GetValueByFieldName ("permitted_flag",pMovement->turning_permitted_flag);


				DTALink* pLink0 = FindLinkWithNodeNumbers(up_node_id, node_id);

				if(pLink0!=NULL)
				{
					if(pMovement->obs_turn_hourly_count >=1)
					{
						pLink0->m_bSensorData = true;
					}

				}else
				{
					return 0;
				}

				DTALink* pLink1 = FindLinkWithNodeNumbers(node_id, dest_node_id);

				if(pLink1!=NULL)
				{
				}else
				{
					return 0;
				}



				std::string turn_type;

				std::string QEM_dir_string;
				parser_movement.GetValueByFieldName ("turn_type",turn_type );
				// parser_movement.GetValueByFieldName ("turn_direction", QEM_dir_string);

				// pMovement->movement_approach_turn  = GetTurnDirectionFromString(QEM_dir_string.c_str ());

				parser_movement.GetValueByFieldName ("QEM_TurnVolume",pMovement->QEM_TurnVolume );
				if(turn_type.find("Through") == string::npos )  // the # of lanes and speed for through movements are determined by link attribute
				{
					parser_movement.GetValueByFieldName ("Lanes",pMovement->QEM_Lanes );
					parser_movement.GetValueByFieldName ("Speed",pMovement->QEM_Speed );
				}

	
				count++;

			}else
			{
				// if the movement has not been defined in the construction part, ignore the data. 
			}
		}
	}


	// overwrite green time if a node is a signalized node
	for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{
		if ((*iNode)->m_NodeNumber == 2) {
			int test = 1;
		}

		if((*iNode)->m_LayerNo == 0  && ((*iNode)->m_ControlType == m_ControlType_PretimedSignal || (*iNode)->m_ControlType == m_ControlType_ActuatedSignal)) 
		{
			for(unsigned int m = 0; m< (*iNode)->m_MovementDataMap["0"].m_MovementVector.size(); m++)
			{
				if((*iNode)->m_MovementDataMap["0"].m_MovementVector[m].QEM_Lanes >= 1 && (*iNode)->m_MovementDataMap["0"].m_MovementVector[m].QEM_EffectiveGreen  <= 6 )
				{  // use default value
					DTALink* pLink0 = m_LinkNoMap[(*iNode)->m_MovementDataMap["0"].m_MovementVector[m].IncomingLinkNo];

					(*iNode)->m_MovementDataMap["0"].m_MovementVector[m].QEM_EffectiveGreen =
					(int)((*iNode)->m_CycleLengthInSecond* pLink0->m_LaneCapacity
						/ pLink0->m_Saturation_flow_rate_in_vhc_per_hour_per_lane);

				}
			}
		}
	}


	//	m_MovementDataLoadingStatus.Format ("%d AMS movements are loaded from file %s.",count,lpszFileName);
	return number_of_nodes;

}

///////////////////////////////// Jun's edit: 11/29/2018   /////////////////////////////////

void CTLiteDoc::ReadOutputMovementMOECSVFile(LPCTSTR lpszFileName) {
	// save the movement capacity and volume to the m_MovementPointerMap[label]
	map<CString, int> map_movement_label2total_volume_count;
	
	CCSVParser parser;
	if (parser.OpenCSVFile(lpszFileName))
	{

		// read the output_movementMOE.csv file to get the movement capacity and total vehicle 

		while (parser.ReadRecord())
		{		
			int up_node_id = -1;
			int curr_node_id = -1;
			int dest_node_id = -1;
			int total_vehicle_count = 0;

			if (parser.GetValueByFieldName("incoming_link_from_node_id", up_node_id) == false)
			{
				//AfxMessageBox("Field incoming_link_from_node_id cannot be found in output_movementMOE.csv.");
				continue;
			}

			if (parser.GetValueByFieldName("node_id", curr_node_id) == false)
			{
				//AfxMessageBox("Field node_id cannot be found in output_movementMOE.csv.");
				continue;
			}

			if (parser.GetValueByFieldName("outgoing_link_to_node_id", dest_node_id) == false)
			{
				//AfxMessageBox("Field outgoing_link_to_node_id cannot be found in output_movementMOE.csv.");
				continue;
			}

			if (parser.GetValueByFieldName("total_vehicle_count", total_vehicle_count) == false)
			{
				//AfxMessageBox("Field total_vehicle_count cannot be found in output_movementMOE.csv.");
				continue;
			}

			CString label;

			label.Format("%d;%d;%d:%s", up_node_id, curr_node_id, dest_node_id, "0"); // the default timing plan is '0'

			map_movement_label2total_volume_count[label] = total_vehicle_count;
			cout << label << endl;
		}

		for (std::list<DTANode*>::iterator iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{
			if ((*iNode)->m_LayerNo == 0)
			{
				for (int tp = 0; tp < m_TimingPlanVector.size(); tp++)  // first loop for each timing plan
				{
					std::string timing_plan_name = m_TimingPlanVector[tp].timing_plan_name;  // fetch timing_plan (unique) name

					for (unsigned int m = 0; m < (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector.size(); m++)
					{

						DTANodeMovement movement = (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m];

						CString label;
						int up_node_id = m_NodeNoMap[movement.in_link_from_node_id]->m_NodeNumber;
						int dest_node_id = m_NodeNoMap[movement.out_link_to_node_id]->m_NodeNumber;
						label.Format("%d;%d;%d:%s", up_node_id, (*iNode)->m_NodeNumber, dest_node_id, timing_plan_name.c_str());

						if (map_movement_label2total_volume_count.find(label) != map_movement_label2total_volume_count.end()) {
							m_MovementPointerMap[label]->QEM_TurnVolume = map_movement_label2total_volume_count[label];
						}

					}
				}
			}
		}
	}


};

double CalEquivalentTab(
	string movement_str,
	vector<vector<double>> through_veh_equivalents, 
	map<string, vector<double>> map_movement_str2vec, 
	map<string, string> map_movestr2movestr) {
	double equivalent_tab = 1.0;
	// if there exists the through traffic
	if (map_movement_str2vec.find(map_movestr2movestr[movement_str]) != map_movement_str2vec.end()) {
		double number_of_oppo_lanes = map_movement_str2vec[map_movestr2movestr[movement_str]][1]; // opposite lane numbers
		double opposite_vol = map_movement_str2vec[map_movestr2movestr[movement_str]][0]; // opposite volumes

		int number_of_oppo_lanes_index = max(0, min(int(number_of_oppo_lanes - 1), 2));
		int opppsite_flow_index = 0;
		if (opposite_vol >= 200 && opposite_vol < 400) opppsite_flow_index = 1;
		if (opposite_vol >= 400 && opposite_vol < 600) opppsite_flow_index = 2;
		if (opposite_vol >= 600 && opposite_vol < 800) opppsite_flow_index = 3;
		if (opposite_vol >= 800 && opposite_vol < 1000) opppsite_flow_index = 4;
		if (opposite_vol >= 1000 && opposite_vol < 1200) opppsite_flow_index = 5;
		if (opposite_vol >= 1200) opppsite_flow_index = 6;

		equivalent_tab = through_veh_equivalents[opppsite_flow_index][number_of_oppo_lanes_index];
	}

	return equivalent_tab;
}

int NumberOfProtectedLeftTurns(map<string, vector<double>> map_movement_str2vec) {
	int number_of_protected_left_turns = 0;
	for (auto map_movement_str : map_movement_str2vec) {
		vector<double> tmp = map_movement_str.second;
		number_of_protected_left_turns += map_movement_str2vec[map_movement_str.first][2];
	}

	return number_of_protected_left_turns;
}

void AssignPhaseNum(int number_of_protected_left_truns, map<string, vector<double>>& map_movement_str2vec) {
	int protected_EBL = map_movement_str2vec.find("EBL") != map_movement_str2vec.end() && map_movement_str2vec["EBL"][2];
	int protected_WBL = map_movement_str2vec.find("WBL") != map_movement_str2vec.end() && map_movement_str2vec["WBL"][2];
	int protected_SBL = map_movement_str2vec.find("SBL") != map_movement_str2vec.end() && map_movement_str2vec["SBL"][2];
	int protected_NBL = map_movement_str2vec.find("NBL") != map_movement_str2vec.end() && map_movement_str2vec["NBL"][2];
	bool four_phase_left_turn = (protected_EBL || protected_WBL) && (protected_SBL || protected_NBL);
	bool E_W_protected_left_turn = protected_EBL || protected_WBL;
	bool N_S_protected_left_turn = protected_NBL || protected_SBL;
	bool no_protected_left_turn = !(protected_EBL || protected_WBL || protected_NBL || protected_SBL);

	if (four_phase_left_turn) {
		// the total number of protected left turn is larger than 3, 
		// that means we will assign protecte phases to all left turns
		map_movement_str2vec["EBL"].push_back(1);
		map_movement_str2vec["WBT"].push_back(2);
		map_movement_str2vec["WBR"].push_back(2);
		map_movement_str2vec["NBL"].push_back(3);
		map_movement_str2vec["SBT"].push_back(4);
		map_movement_str2vec["SBR"].push_back(4);
		map_movement_str2vec["WBL"].push_back(5);
		map_movement_str2vec["EBT"].push_back(6);
		map_movement_str2vec["EBR"].push_back(6);
		map_movement_str2vec["SBL"].push_back(7);
		map_movement_str2vec["NBT"].push_back(8);
		map_movement_str2vec["NBR"].push_back(8);
	} else if (number_of_protected_left_truns >= 1) {
		// the total number of protected left turn is larger than 1, 
		if (E_W_protected_left_turn) {
			map_movement_str2vec["EBL"].push_back(1);
			map_movement_str2vec["WBT"].push_back(2);
			map_movement_str2vec["WBR"].push_back(2);
			map_movement_str2vec["NBL"].push_back(8);
			map_movement_str2vec["SBT"].push_back(4);
			map_movement_str2vec["SBR"].push_back(4);
			map_movement_str2vec["WBL"].push_back(5);
			map_movement_str2vec["EBT"].push_back(6);
			map_movement_str2vec["EBR"].push_back(6);
			map_movement_str2vec["SBL"].push_back(4);
			map_movement_str2vec["NBT"].push_back(8);
			map_movement_str2vec["NBR"].push_back(8);
		}
		else if (N_S_protected_left_turn) {
			map_movement_str2vec["EBL"].push_back(6);
			map_movement_str2vec["WBT"].push_back(2);
			map_movement_str2vec["WBR"].push_back(2);
			map_movement_str2vec["NBL"].push_back(3);
			map_movement_str2vec["SBT"].push_back(4);
			map_movement_str2vec["SBR"].push_back(4);
			map_movement_str2vec["WBL"].push_back(2);
			map_movement_str2vec["EBT"].push_back(6);
			map_movement_str2vec["EBR"].push_back(6);
			map_movement_str2vec["SBL"].push_back(7);
			map_movement_str2vec["NBT"].push_back(8);
			map_movement_str2vec["NBR"].push_back(8);
		}
	} 
	if (no_protected_left_turn) {
		map_movement_str2vec["WBL"].push_back(2);
		map_movement_str2vec["WBT"].push_back(2);
		map_movement_str2vec["WBR"].push_back(2);
		map_movement_str2vec["SBL"].push_back(4);
		map_movement_str2vec["SBT"].push_back(4);
		map_movement_str2vec["SBR"].push_back(4);
		map_movement_str2vec["EBL"].push_back(6);
		map_movement_str2vec["EBT"].push_back(6);
		map_movement_str2vec["EBR"].push_back(6);
		map_movement_str2vec["NBL"].push_back(8);
		map_movement_str2vec["NBT"].push_back(8);
		map_movement_str2vec["NBR"].push_back(8);
	}
}

vector<double> CriticalLaneVolumes(map<string, vector<double>> map_movement_str2vec) {
	double critical_volume_1_5 = 0.0;
	double critical_volume_2_6 = 0.0;
	double critical_volume_3_7 = 0.0;
	double critical_volume_4_8 = 0.0;

	for (auto movement_str : map_movement_str2vec) {
		if (movement_str.second[7] == 1 || movement_str.second[7] == 5) {
			critical_volume_1_5 = max(critical_volume_1_5, movement_str.second[6]);
		}
		if (movement_str.second[7] == 2 || movement_str.second[7] == 6) {
			critical_volume_2_6 = max(critical_volume_2_6, movement_str.second[6]);
		}
		if (movement_str.second[7] == 3 || movement_str.second[7] == 7) {
			critical_volume_3_7 = max(critical_volume_3_7, movement_str.second[6]);
		}
		if (movement_str.second[7] == 4 || movement_str.second[7] == 8) {
			critical_volume_4_8 = max(critical_volume_4_8, movement_str.second[6]);
		}
	}

	return { critical_volume_1_5 , critical_volume_2_6 , critical_volume_3_7 , critical_volume_4_8 , 
		critical_volume_1_5 + critical_volume_2_6 + critical_volume_3_7 + critical_volume_4_8 };
}


int GetTotalPhaseNumber(map<string, vector<double>> map_movement_str2vec){
	set<int> phase_number_set;
	for (auto movement_str : map_movement_str2vec) {
		// the phase number that is not assigned to the movements
		if (phase_number_set.find(movement_str.second[7]) == phase_number_set.end()) {
			phase_number_set.insert(movement_str.second[7]);
		}
	}

	return phase_number_set.size();
}


void AllocateEffectiveGreen(
	double desired_cycle_length, 
	vector<double> critical_lane_vol_vec, 
	map<string, vector<double>>& map_movement_str2vec) {
	double effective_green_time_1_5 = max(desired_cycle_length * critical_lane_vol_vec[0] / critical_lane_vol_vec[4], rand() % 10 + 5);
	double effective_green_time_2_6 = max(desired_cycle_length * critical_lane_vol_vec[1] / critical_lane_vol_vec[4], rand() % 10 + 5);
	double effective_green_time_3_7 = max(desired_cycle_length * critical_lane_vol_vec[2] / critical_lane_vol_vec[4], rand() % 10 + 5);
	double effective_green_time_4_8 = max(desired_cycle_length * critical_lane_vol_vec[3] / critical_lane_vol_vec[4], rand() % 10 + 5);

	for (auto movement_str : map_movement_str2vec) {
		int phase_num = movement_str.second[7];
		if (phase_num == 1 || phase_num == 5) map_movement_str2vec[movement_str.first].push_back(effective_green_time_1_5);
		if (phase_num == 2 || phase_num == 6) map_movement_str2vec[movement_str.first].push_back(effective_green_time_2_6);
		if (phase_num == 3 || phase_num == 7) map_movement_str2vec[movement_str.first].push_back(effective_green_time_3_7);
		if (phase_num == 4 || phase_num == 8) map_movement_str2vec[movement_str.first].push_back(effective_green_time_4_8);
	}
}

vector<int> GetUpAndDesNode(string STDStr) {
	vector<int> res = { 0, 0 };
	vector<int> loc;
	if (STDStr.size() == 0) return res;
	for (int i = 0; i < STDStr.size(); i++){
		if (STDStr[i] - '9' > 0 || STDStr[i] - '0' < 0) {
			loc.push_back(i);
		}
	}

	res[0] = atoi(STDStr.substr(0, loc[0] + 1).c_str());
	res[1] = atoi(STDStr.substr(loc[1] + 1, loc[2] - loc[1] - 1).c_str());
	return res;
}

void InitializeMap_movement_str2vec(map<string, vector<double>>& map_movement_str2vec) {
	vector<string> movement_str_vec = { "EBL", "EBT", "EBR", "WBL", "WBT", "WBR", "NBL", "NBT", "NBR", "SBL", "SBT", "SBR" };
	for (auto str : movement_str_vec) {
		map_movement_str2vec[str] = { 0.0, 0.0 };
	}
}

void CalThrouthVelEquVol(map<string, vector<double>>& map_movement_str2vec) {
	// the map_movement_str is the movement_str with the related volumn
	// !!!!!column names: volumn(vehicle), lanes, is_protected_left_turn, equivalent_tab, volume(tvu/h), lane group, vol/lan

	// table for the computation of through vehicle equivalent volumn
	// oppo flow				# of lanes
	//				1			2			3
	// 0			1.1			1.1			1.1
	// 200			2.5			2.0			1.8
	// 400			5.0			3.0			2.5
	// 600			10.0		5.0			4.0
	// 800			13.0		8.0			6.0
	// 1000			15.0		13.0		10.0
	// 1200			15.0		15.0		15.0
	vector<vector<double>> through_veh_equivalents;
	through_veh_equivalents.push_back({ 1.1, 1.1, 1.1 });
	through_veh_equivalents.push_back({ 2.5, 2.0, 1.8 });
	through_veh_equivalents.push_back({ 5.0, 3.0, 2.5 });
	through_veh_equivalents.push_back({ 10.0, 5.0, 4.0 });
	through_veh_equivalents.push_back({ 13.0, 8.0, 6.0 });
	through_veh_equivalents.push_back({ 15.0, 13.0, 10.0 });
	through_veh_equivalents.push_back({ 15.0, 15.0, 15.0 });

	// the right turn vehicle equivalents considering the pedestrian vol peds/h
	// None(0)		= 1.18
	// Low(50)		= 1.21
	// Moderate(200)= 1.32
	// High(400)	= 1.52
	// Extreme(800)	= 2.14
	vector<double> right_veh_equivalents = { 1.18, 1.21, 1.32, 1.52, 2.14 };

	map<string, string> map_movestr2movestr;
	map_movestr2movestr["EBL"] = "WBT";
	map_movestr2movestr["WBL"] = "EBT";
	map_movestr2movestr["NBL"] = "SBT";
	map_movestr2movestr["SBL"] = "NBT";

	// is left_turn protected??
	for (auto map_movement_str : map_movement_str2vec) {
		string movement_str = map_movement_str.first;
		// check if the left turn need protected or not
		if (movement_str[2] == 'L' && map_movement_str.second[0] >= 200) {
			// it is left turn and the left turn volumn >= 200, then we will use protected left turn
			map_movement_str2vec[movement_str].push_back(1); // it is a protected left turn
		}
		else {
			map_movement_str2vec[movement_str].push_back(0);
		}
	}

	for (auto map_movement_str : map_movement_str2vec) {
		string movement_str = map_movement_str.first;
		// check if the left turn need protected or not
		double equivalent_tab = 1.0;
		if (movement_str[2] == 'L' && map_movement_str.second[2] == 0) {
			// the left turn is not protected
			equivalent_tab = CalEquivalentTab(movement_str, through_veh_equivalents, map_movement_str2vec, map_movestr2movestr);
		}
		else if (movement_str[2] == 'R') {
			// as we are not considering the pedestrian here, we will use the moderate number as default
			equivalent_tab = right_veh_equivalents[2];
		}

		map_movement_str2vec[movement_str].push_back(equivalent_tab);
	}

	for (auto map_movement_str : map_movement_str2vec) {
		vector<double> tmp = map_movement_str.second;
		map_movement_str2vec[map_movement_str.first].push_back(tmp[0] * tmp[3]); // the actual vehicle * the equivalent_tab
	}

	// lane_group_volume
	for (auto map_movement_str : map_movement_str2vec) {
		string movement_str = map_movement_str.first;
		vector<double> tmp = map_movement_str.second;
		if (movement_str[2] == 'L') map_movement_str2vec[map_movement_str.first].push_back(tmp[4]);
		if (movement_str[2] == 'R') map_movement_str2vec[map_movement_str.first].push_back(0);
		if (movement_str[2] == 'T') {
			string new_str = movement_str;
			new_str[2] = 'R';
			map_movement_str2vec[map_movement_str.first].push_back(tmp[4] + map_movement_str2vec[new_str][4]);
			// the right and through altogether
		}
	}

	// vol/lane (tvu/h/ln)
	for (auto map_movement_str : map_movement_str2vec) {
		string movement_str = map_movement_str.first;
		vector<double> tmp = map_movement_str.second;
		map_movement_str2vec[map_movement_str.first].push_back(tmp[5] / 2.0);
	}
}


int CTLiteDoc::ReadAMSSignalControlCSVFile(LPCTSTR lpszFileName)
{

	CCSVParser parser_signal;

	if (parser_signal.OpenCSVFile(lpszFileName, true))  // not required
	{
		int up_node_id = 0;
		int dest_node_id = 0;

		while (parser_signal.ReadRecord())
		{
			int node_id = -1;
			int phase_id = -1;
			int next_phase_id = -1;
			int green_duration = -1;
			std::string timing_plan_name, movement_str, movement_dir_str;

			parser_signal.GetValueByFieldName("int_id", node_id);

			DTANode* pNode = FindNodeWithNodeNumber(node_id);

			if (pNode != NULL)
			{
				pNode->m_withSignalPlans = true;
				parser_signal.GetValueByFieldName("timing_plan_no", timing_plan_name);
				parser_signal.GetValueByFieldName("movement_str", movement_str);
				parser_signal.GetValueByFieldName("green_duration", green_duration);
				parser_signal.GetValueByFieldName("next_phase_id", next_phase_id, false);
				parser_signal.GetValueByFieldName("phase_id", phase_id);

				SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_GreenDuration, green_duration);
				SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_NextPhaseNo, next_phase_id);
				SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_MOVEMENT_VECTOR, movement_str);

				parser_signal.GetValueByFieldName("movement_dir_str", movement_dir_str);
				SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_MOVEMENT_DIR_VECTOR, movement_dir_str);


				if (movement_str.size() == 0 && movement_dir_str.size() > 0)
				{
					for (unsigned int m = 0; m < pNode->m_MovementDataMap["0"].m_MovementVector.size(); m++)
					{
						bool bMovementIncluded = IfMovementDirIncludedInPhase(node_id,
							timing_plan_name, phase_id, m);

						if (bMovementIncluded)
						{

							CString sub_movement_str;

							sub_movement_str.Format(";%d_%d_%s", m_NodeNoMap[pNode->m_MovementDataMap["0"].m_MovementVector[m].in_link_from_node_id]->m_NodeNumber,
								m_NodeNoMap[pNode->m_MovementDataMap["0"].m_MovementVector[m].out_link_to_node_id]->m_NodeNumber,
								GetTurnShortString(pNode->m_MovementDataMap["0"].m_MovementVector[m].movement_turn));
							//select
							movement_str += sub_movement_str;
						}

					}

					SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_MOVEMENT_VECTOR, movement_str);

				}
			}
		}
	}


	///////////////////////////////// Jun's edit: 11/13/2018   /////////////////////////////////
	// here compute the through vehicle equivalent volumns

	// m_MovementPointerMap
	// label.Format("%d;%d;%d:%s", up_node_id, node_id, dest_node_id, timing_plan_name.c_str());


	/// auto generate the phasing data
	std::list<DTANode*>::iterator iNode;
	for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{
		DTANode* pNode = (*iNode);
		if (pNode != NULL && pNode->m_withSignalPlans!=true && pNode->m_ControlType == 5)
		{
			int node_id = pNode->m_NodeNumber;
			int phase_id = -1;
			int next_phase_id = -1;
			int green_duration = 0;

			vector<CString> movement_label = m_Node2MovementStr[node_id];
			map<string, vector<int>> map_movement_str2from_to_node;
			map<string, vector<double>> map_movement_str2vec;
			map<string, vector<string>> map_movement_str2label;


			InitializeMap_movement_str2vec (map_movement_str2vec);

			// column names : volumn, lanes, equivalent tab, volume, lane group, vol / lan
			int hour = 0; // the current hour
			int up_node_id = 0;
			int curr_node_id = 0;
			int dest_node_id = 0;
			// calculate the phase number and effective green time of each movement

			// step 0: get the volume and # of lanes data
			for (int i = 0; i < movement_label.size(); i++) {
				CString movement_str_tmp = movement_label[i];
				std::string STDStr((LPCTSTR)movement_str_tmp);
				// Reads data from s and stores them according to parameter format into the locations given by the additional arguments
				// as if scanf was used, but reading from s instead of the standard input (stdin).
				vector<int> node_vec = GetUpAndDesNode(STDStr);
				up_node_id = node_vec[0];
				dest_node_id = node_vec[1];

				DTANodeMovement* movement = m_MovementPointerMap[movement_label[i]];
				// label.Format("%d;%d;%d:%s", up_node_id, node_id, dest_node_id, timing_plan_name.c_str());
				// get the count of the peak hour ()

				int movement_app_turn = movement->movement_approach_turn;
				vector<string> movement_str_vec = { "EBL", "EBT", "EBR", "WBL", "WBT", "WBR", "NBL", "NBT", "NBR", "SBL", "SBT", "SBR" };
				
				// there are more than one movement of the same movement string
				// east bound 
				if (movement_app_turn == DTA_EBL) {
					// read the volumn
					map_movement_str2vec["EBL"][0] = movement->QEM_TurnVolume;
					map_movement_str2vec["EBL"][1] = movement->QEM_Lanes;
					map_movement_str2from_to_node["EBL"] = { up_node_id , dest_node_id };
					map_movement_str2label["EBL"].push_back(STDStr);
				}
				if (movement_app_turn == DTA_EBT) {
					// read the volumn
					map_movement_str2vec["EBT"][0] = movement->QEM_TurnVolume;
					map_movement_str2vec["EBT"][1] = movement->QEM_Lanes;
					map_movement_str2from_to_node["EBT"] = { up_node_id , dest_node_id };
					map_movement_str2label["EBT"].push_back(STDStr);
				}
				if (movement_app_turn == DTA_EBR) {
					// read the volumn
					map_movement_str2vec["EBR"][0] = movement->QEM_TurnVolume;
					map_movement_str2vec["EBR"][1] = movement->QEM_Lanes;
					map_movement_str2from_to_node["EBR"] = { up_node_id , dest_node_id };
					map_movement_str2label["EBR"].push_back(STDStr);
				}
				
				// west bound 
				if (movement_app_turn == DTA_WBL) {
					// read the volumn
					map_movement_str2vec["WBL"][0] = movement->QEM_TurnVolume;
					map_movement_str2vec["WBL"][1] = movement->QEM_Lanes;
					map_movement_str2from_to_node["WBL"] = { up_node_id , dest_node_id };
					map_movement_str2label["WBL"].push_back(STDStr);
				}
				if (movement_app_turn == DTA_WBT) {
					// read the volumn
					map_movement_str2vec["WBT"][0] = movement->QEM_TurnVolume;
					map_movement_str2vec["WBT"][1] = movement->QEM_Lanes;
					map_movement_str2from_to_node["WBT"] = { up_node_id , dest_node_id };
					map_movement_str2label["WBT"].push_back(STDStr);
				}
				if (movement_app_turn == DTA_WBR) {
					// read the volumn
					map_movement_str2vec["WBR"][0] = movement->QEM_TurnVolume;
					map_movement_str2vec["WBR"][1] = movement->QEM_Lanes;
					map_movement_str2from_to_node["WBR"] = { up_node_id , dest_node_id };
					map_movement_str2label["WBR"].push_back(STDStr);
				}

				// south bound
				if (movement_app_turn == DTA_SBL) {
					// read the volumn
					map_movement_str2vec["SBL"][0] = movement->QEM_TurnVolume;
					map_movement_str2vec["SBL"][1] = movement->QEM_Lanes;
					map_movement_str2from_to_node["SBL"] = { up_node_id , dest_node_id };
					map_movement_str2label["SBL"].push_back(STDStr);
				}
				if (movement_app_turn == DTA_SBT) {
					// read the volumn
					map_movement_str2vec["SBT"][0] = movement->QEM_TurnVolume;
					map_movement_str2vec["SBT"][1] = movement->QEM_Lanes;
					map_movement_str2from_to_node["SBT"] = { up_node_id , dest_node_id };
					map_movement_str2label["SBT"].push_back(STDStr);
				}
				if (movement_app_turn == DTA_SBR) {
					// read the volumn
					map_movement_str2vec["SBR"][0] = movement->QEM_TurnVolume;
					map_movement_str2vec["SBR"][1] = movement->QEM_Lanes;
					map_movement_str2from_to_node["SBR"] = { up_node_id , dest_node_id };
					map_movement_str2label["SBR"].push_back(STDStr);
				}

				// north bound
				if (movement_app_turn == DTA_NBL) {
					// read the volumn
					map_movement_str2vec["NBL"][0] = movement->QEM_TurnVolume;
					map_movement_str2vec["NBL"][1] = movement->QEM_Lanes;
					map_movement_str2from_to_node["NBL"] = { up_node_id , dest_node_id };
					map_movement_str2label["NBL"].push_back(STDStr);
				}
				if (movement_app_turn == DTA_NBT) {
					// read the volumn
					map_movement_str2vec["NBT"][0] = movement->QEM_TurnVolume;
					map_movement_str2vec["NBT"][1] = movement->QEM_Lanes;
					map_movement_str2from_to_node["NBT"] = { up_node_id , dest_node_id };
					map_movement_str2label["NBT"].push_back(STDStr);
				}
				if (movement_app_turn == DTA_NBR) {
					// read the volumn
					map_movement_str2vec["NBR"][0] = movement->QEM_TurnVolume;
					map_movement_str2vec["NBR"][1] = movement->QEM_Lanes;
					map_movement_str2from_to_node["NBR"] = { up_node_id , dest_node_id };
					map_movement_str2label["NBR"].push_back(STDStr);
				}
			}

			//delete &up_node_id;
			//delete &curr_node_id;
			//delete &dest_node_id;
			//delete &timing_plan_name;
			// check the volumn and convert it to the through vehicle equivalent volumes
			// !!!!!column names : volumn(vehicle), lanes, is_protected_left_turn, equivalent_tab, volume(tvu / h), lane group, vol / lan

			// step 1 : develop a phase plan (should the left turn be protected)
			// step 2 : convert volumes to through vehicle equivalents 
			CalThrouthVelEquVol(map_movement_str2vec);

			// step 3 : determine the critical lane volumes
			// 3.1 get the number of protected left turns
			int number_of_protected_left_truns = NumberOfProtectedLeftTurns(map_movement_str2vec);
			// 3.2 assign the phase number of each movement
			AssignPhaseNum(number_of_protected_left_truns, map_movement_str2vec);
			// 3.3 calculate critical lane volume
			vector < double > critical_lane_vol_vec = CriticalLaneVolumes(map_movement_str2vec);
			double critical_lane_vol = critical_lane_vol_vec[critical_lane_vol_vec.size() - 1];

			// step 4 : determine yellow and all red time
			// step 5 : determine the total loss time
			// to simplized the problem, this should be determined by the phase number
			// 4.1 get the total phase number
			int total_phase_num = GetTotalPhaseNumber(map_movement_str2vec);
			// 4.2 the suppose every phase has a loss time of 2.5s
			double total_loss_time = total_phase_num * 2.5;

			// step 6 : determin the desirable cycle length
			// desired cycle length = total_loss_time/ ( 1 - Vc/(1615 * PHF * V/C)) 
			// Vc : the sum of critical lane volume
			// PHF : peak hour factor, here we use 0.9
			// V/C : volume/capacity of critical movements (usually beteen 0.85-0.95), here we use 0.9


			// the maxinum cycle length is 150 seconds
			double desired_cycle_length = min(total_loss_time / max(1 - critical_lane_vol / (1615 * 0.9 * 0.9), 0.01), 150);

			// step 7: allocate effective green time to each phase
			AllocateEffectiveGreen(desired_cycle_length, critical_lane_vol_vec, map_movement_str2vec);

			// step 8: check pedestrian requirement, NOT considered here
			

			for (int i = 1; i <= 8; i++) {
				bool phase_used = false;
				string timing_plan_name = "1";
				string movement_str = "";
				string movement_dir_str = ";";
				string sub_movement_str = "";
				string movement_str_tmp = "";
				phase_id = i; // this is the phase id

				for (auto map_movement_str : map_movement_str2vec) {

					movement_str_tmp = map_movement_str.first;
					int NEMAPhase_no = map_movement_str.second[7];
					
					if (NEMAPhase_no == i && map_movement_str2from_to_node.find(movement_str_tmp) != map_movement_str2from_to_node.end())
					{
						green_duration = max(int(map_movement_str.second[8]), green_duration);
						phase_used = true;

						// !!!! here there could exists multiple movements with the same movement string
						// what if the 'EBT' or 'WBT' or other movement_string has more than one movements
						// let's check if we can shift these movements to other phases
						int up_node_id_first = GetUpAndDesNode(map_movement_str2label[movement_str_tmp][0])[0];
						for (int j = 0; j < map_movement_str2label[movement_str_tmp].size(); j++) {
							int up_node_id = GetUpAndDesNode(map_movement_str2label[movement_str_tmp][j])[0];
							int des_node_id = GetUpAndDesNode(map_movement_str2label[movement_str_tmp][j])[1];
							if (up_node_id == up_node_id_first) { 
								sub_movement_str = to_string(up_node_id) + "_"
									+ to_string(des_node_id) + "_"
									+ movement_str_tmp[2] + ";";
								//fromnode_tonode_turn direction
								movement_str += sub_movement_str;
								movement_dir_str = movement_dir_str + movement_str_tmp + ";";
							}
						}
					}
				}

				if (phase_used) {
					next_phase_id = (phase_id == 8) ? 1 : phase_id + 1;
					SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_GreenDuration, green_duration);
					SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_NextPhaseNo, next_phase_id);
					// SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_MOVEMENT_VECTOR, movement_str);

					SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_MOVEMENT_DIR_VECTOR, movement_dir_str);
					SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_MOVEMENT_VECTOR, movement_str);
				}

				green_duration = 5; // minimum green duration is 5 seconds

				phase_used = false;
			}



			//for (unsigned int m = 0; m < pNode->m_MovementDataMap["0"].m_MovementVector.size(); m++)
			//{
			//	int NEMAPhase_no = 0;
			//	NEMAPhase_no = GetNEMAPhase_from_TurnDirectionString(pNode->m_MovementDataMap["0"].m_MovementVector[m].movement_approach_turn);
			//	if (NEMAPhase_no > 0)
			//	{
			//		if(NEMAPhase_no%2==1) // left turn
			//			green_duration = 10;
			//		else {
			//			 green_duration = 45;

			//			}

			//		phase_id = NEMAPhase_no;

			//		std::string timing_plan_name = "1";
			//		std::string movement_str, movement_dir_str;
			//		movement_dir_str += ";";
			//		SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_GreenDuration, green_duration);
			//		SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_NextPhaseNo, next_phase_id);
			//		SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_MOVEMENT_VECTOR, movement_str);
			//		movement_dir_str += GetTurnDirectionString(pNode->m_MovementDataMap["0"].m_MovementVector[m].movement_approach_turn);

			//		SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_MOVEMENT_DIR_VECTOR, movement_dir_str);
			//		CString sub_movement_str;
			//		sub_movement_str.Format(";%d_%d_%s", m_NodeNoMap[pNode->m_MovementDataMap["0"].m_MovementVector[m].in_link_from_node_id]->m_NodeNumber,
			//			m_NodeNoMap[pNode->m_MovementDataMap["0"].m_MovementVector[m].out_link_to_node_id]->m_NodeNumber,
			//			GetTurnShortString(pNode->m_MovementDataMap["0"].m_MovementVector[m].movement_turn));
			//		//select
			//			movement_str += sub_movement_str;

			//		SetupPhaseData(node_id, timing_plan_name, phase_id, PHASE_MOVEMENT_VECTOR, movement_str);
			//	}
			//}

		}
	}
	return true;
}

bool CTLiteDoc::ReadVehicleBinFile(LPCTSTR lpszFileName, int version_number = 2)
{
	//   cout << "Read vehicle file... "  << endl;
	// vehicle_id,  origin_zone_id, destination_zone_id, departure_time,
	//	arrival_time, complete_flag, trip_time, demand_type, occupancy, information_type,
	//	value_of_time, path_min_cost,distance_in_mile, number_of_nodes,
	//	node id, node arrival time

	bool bCreateMovementMap = true;
	float LengthinMB;
	FILE* pFile;
	fopen_s(&pFile,lpszFileName,"rb");
	
	int Length = 0;
	if(pFile!=NULL)
	{
		fseek(pFile, 0, SEEK_END );
		Length = ftell(pFile);
		fclose(pFile);
		LengthinMB= Length*1.0/1024/1024;
		if(LengthinMB>50)
		{
			CString msg;
			msg.Format("The %s file is %5.1f MB in size.\nIt could take quite a while to load this file.\nWould you like to load the file?", lpszFileName, LengthinMB);
			if(AfxMessageBox(msg,MB_YESNO|MB_ICONINFORMATION)==IDNO)
			{
				fclose(pFile);
				return true;
			}
		}

		//if(LengthinMB>20)  // if the file size is greater then 20 MB, ask the question
		//{
		//	CString msg;

		//	msg.Format("Do you want to create the movement mapping table (necessary for Synchro exporting function)?",LengthinMB);
		//	if(AfxMessageBox(msg,MB_YESNO|MB_ICONINFORMATION)==IDNO)
		//		bCreateMovementMap = false;
		//}
		fclose(pFile);
	}

	CString SettingsFile;
	SettingsFile.Format ("%sDTASettings.txt",m_ProjectDirectory);


	typedef struct  
	{
		int vehicle_id;
		int from_zone_id;
		int to_zone_id;
		float departure_time;
		float arrival_time;
		int complete_flag;
		float trip_time;
		int demand_type;
		int pricing_type;
		int vehicle_type;
		int information_type;
		float value_of_time;
		float toll_cost_in_dollar;
		float PM;
		float distance_in_mile;
		int number_of_nodes;
		float Energy;
		float CO2;
		float NOX;
		float CO;
		float HC;


	} struct_Vehicle_Header_version_1;

	typedef struct  
	{
		int age;
		int version_no;

		int day_no;	
		float PM2_5;
		int number_of_VMS_response_links;


	} struct_Vehicle_Header_extension_version_2;

	typedef  struct  
	{
		int NodeName;
		float AbsArrivalTimeOnDSN;
	} struct_Vehicle_Node;


	bool bLoadedMovement = false;
	if (m_LinkSet.size() <= _MAX_LINK_FOR_LOAD_MOVEMENT_DATA && m_bLoadMovementData == true)
		bLoadedMovement = true;


	FILE* st = NULL;
	fopen_s(&st,lpszFileName,"rb");
	if(st!=NULL)
	{
		m_VehicleSet.clear();

		if (bLoadedMovement)
		{
			m_Movement3NodeMap.clear();
		}

		struct_Vehicle_Header_version_1 header;

		struct_Vehicle_Header_extension_version_2 header_extension;

		CString str_loading;
		int count =0;


		while(!feof(st))
		{

			if (count % 10000 == 0)
			{
				str_loading.Format("loading %dk agents", count/1000);
				SetStatusText(str_loading);
			}

			size_t result;
			size_t result_extension;

			result = fread(&header,sizeof(struct_Vehicle_Header_version_1),1,st);
			if(version_number == 2)
				result_extension = fread(&header_extension,sizeof(struct_Vehicle_Header_extension_version_2),1,st);

			if( header.vehicle_id < 0)
				break;

			if(result!=1)  // read end of file
				break;

			DTAVehicle* pVehicle = 0;

			pVehicle = new DTAVehicle;

			header_extension.day_no =0;

			pVehicle->m_DayNo  = header_extension.day_no;

			g_SimulatedDayDataMap[ header_extension.day_no] = true;
			g_SimulatedLastDayNo= max(g_SimulatedLastDayNo, pVehicle->m_DayNo);
			g_SimulationDayNo = g_SimulatedLastDayNo;

			
			pVehicle->m_VehicleID		= header.vehicle_id;

			// TRACE("Vehicle ID = %d\n", header.vehicle_id);

			pVehicle->m_OriginZoneID	= header.from_zone_id;
			pVehicle->m_DestinationZoneID= header.to_zone_id;
			pVehicle->m_DepartureTime	=  header.departure_time;
			pVehicle->m_ArrivalTime =  header.arrival_time;

			if(g_Simulation_Time_Horizon < pVehicle->m_ArrivalTime)
				g_Simulation_Time_Horizon = pVehicle->m_ArrivalTime;

			if(header.complete_flag==0) 
				pVehicle->m_bComplete = false;
			else
				pVehicle->m_bComplete = true;

			pVehicle->m_TripTime  = header.trip_time;

			pVehicle->m_DemandType = header.demand_type;
			pVehicle->m_PricingType = header.pricing_type;
			
			pVehicle->m_VehicleType = header.vehicle_type;
			pVehicle->m_InformationClass = header.information_type;
			pVehicle->m_VOT = header.value_of_time;
			pVehicle->m_TollDollarCost = header.toll_cost_in_dollar;
			pVehicle->m_PM = header.PM;
			pVehicle->m_PM2_5 = header_extension.PM2_5;
			pVehicle->m_Distance = header.distance_in_mile;
			pVehicle->m_NodeSize	= header.number_of_nodes;

			pVehicle->m_number_of_VMS_response_links = header_extension.number_of_VMS_response_links;

			if(pVehicle->m_bComplete )
			{
				m_ZoneMap[pVehicle->m_OriginZoneID].m_OriginTotalNumberOfVehicles += 1;
				m_ZoneMap[pVehicle->m_OriginZoneID].m_OriginTotalTravelDistance  += pVehicle->m_Distance ;
				m_ZoneMap[pVehicle->m_OriginZoneID].m_OriginTotalTravelTime  += pVehicle->m_TripTime ;

				m_ZoneMap[pVehicle->m_DestinationZoneID].m_DestinationTotalNumberOfVehicles += 1;
				m_ZoneMap[pVehicle->m_DestinationZoneID].m_DestinationTotalTravelDistance  += pVehicle->m_Distance ;
				m_ZoneMap[pVehicle->m_DestinationZoneID].m_DestinationTotalTravelTime  += pVehicle->m_TripTime ;
			}


			pVehicle->m_EmissionData .Energy = header.Energy;

			if(header.Energy >1)
				m_bEmissionDataAvailable =true;

			pVehicle->m_EmissionData .CO2 = header.CO2;
			pVehicle->m_EmissionData .NOX = header.NOX;
			pVehicle->m_EmissionData .CO = header.CO;
			pVehicle->m_EmissionData .HC = header.HC;
			pVehicle->m_EmissionData.PM = header.PM;
			pVehicle->m_EmissionData.PM2_5 = header_extension.PM2_5;
			if (pVehicle->m_NodeSize >= MAX_NODE_SIZE_IN_A_PATH)
			{
				AfxMessageBox("Too many nodes in a path.");
				return true;
			
			}
			if(version_number == 2)
			{
				pVehicle->m_Age = header_extension.age;
			}

			if(pVehicle->m_NodeSize>=1)  // in case reading error
			{
				pVehicle->m_NodeAry = new SVehicleLink[pVehicle->m_NodeSize];

//				TRACE("Node size = %d", pVehicle->m_NodeSize);

				pVehicle->m_NodeNumberSum = 0;
				for(int i=0; i< pVehicle->m_NodeSize; i++)
				{


					struct_Vehicle_Node node_element;
					fread(&node_element,sizeof(node_element),1,st);

					m_PathNodeVectorSP[i] = node_element.NodeName;
					pVehicle->m_NodeNumberSum += m_PathNodeVectorSP[i];
					if (i >= 1)
					{
						DTALink* pLink = FastFindLinkWithNodeNumbers(m_PathNodeVectorSP[i - 1], m_PathNodeVectorSP[i]);
						if (pLink != NULL)
						{
							//	AfxMessageBox("Error in reading file agent.bin");




							pVehicle->m_NodeAry[i].LinkNo = pLink->m_LinkNo;

							// random error beyond 6 seconds for better ainimation

							float random_value = g_RNNOF()*0.01; // 0.1 min = 6 seconds

							pVehicle->m_NodeAry[i].ArrivalTimeOnDSN = node_element.AbsArrivalTimeOnDSN;

							pLink->m_total_assigned_link_volume += 1;

							if (pVehicle->m_NodeAry[i].ArrivalTimeOnDSN < 10000) //feasible arrival time
							{


								float travel_time = 0;

								if (i >= 2)
									travel_time = pVehicle->m_NodeAry[i].ArrivalTimeOnDSN - pVehicle->m_NodeAry[i - 1].ArrivalTimeOnDSN;
								else // first link
									travel_time = pVehicle->m_NodeAry[i].ArrivalTimeOnDSN - pVehicle->m_DepartureTime;


								if (travel_time - pLink->m_FreeFlowTravelTime > 100)
									TRACE("");


								if (travel_time >= 100)
								{
									TRACE("");

								}
								pLink->m_TotalTravelTime += travel_time;

								float delay = travel_time - pLink->m_FreeFlowTravelTime;
								pLink->m_total_delay += delay;
								pLink->AddNodeDelay(pVehicle->m_NodeAry[i].ArrivalTimeOnDSN, delay);
							}
							else
							{
								//TRACE("");

								//TRACE("infeasible link; %f, %d\n", pLink->m_total_link_volume, pVehicle->m_VehicleID);

							}


							pLink->m_total_link_volume_of_incomplete_trips = pLink->m_total_link_volume - pLink->m_total_assigned_link_volume;

						}
						else
						{  // missing link
							
							fclose(st);
							return false;
						}
					}


				}

				// movement count


				m_VehicleSet.push_back (pVehicle);
				m_VehicleIDMap[pVehicle->m_VehicleID]  = pVehicle;


				count++;
			} 
		}

		if (bLoadedMovement)
		{
			UpdateMovementDataFromVehicleTrajector();
		}
		fclose(st);
		m_SimulationVehicleDataLoadingStatus.Format ("%d vehicles are loaded from file %s.",count,lpszFileName);
		return true;

	}
	return false;
}

bool CTLiteDoc::ReadAgentCSVFile(LPCTSTR lpszFileName, int version_number = 2)
{
	CCSVParser parser_agent;

	float total_number_of_vehicles_to_be_generated = 0;

	if (parser_agent.OpenCSVFile(lpszFileName))
	{
		int count = 0;

		int line_no = 1;

		int i = 0;
		while (parser_agent.ReadRecord())
		{

			string agent_id = "0";

			parser_agent.GetValueByFieldName("agent_id", agent_id);
			DTAVehicle* pVehicle = 0;


			pVehicle = new DTAVehicle;

			//pVehicle->m_VehicleID = agent_id;
			pVehicle->m_RandomSeed = pVehicle->m_VehicleID;

			parser_agent.GetValueByFieldName("from_zone_id", pVehicle->m_OriginZoneID);
			parser_agent.GetValueByFieldName("to_zone_id", pVehicle->m_DestinationZoneID);

			int origin_node_id = -1;
			int origin_node_number = -1;


			std::vector<CCoordinate> CoordinateVector;
			string geo_string;

			//if (parser_agent.GetValueByFieldName("geometry", geo_string) == false)
			//{
			//	// overwrite when the field "geometry" exists
			//	CGeometry geometry(geo_string);
			//	CoordinateVector = geometry.GetCoordinateList();
			//	for (int si = 0; si < CoordinateVector.size(); si++)
			//	{
			//		GDPoint	pt;
			//		pt.x = CoordinateVector[si].X;
			//		pt.y = CoordinateVector[si].Y;
			//		pVehicle->m_ShapePoints.push_back(pt);

			//	}

			//}


			parser_agent.GetValueByFieldName("origin_node_id", origin_node_number);

			if (m_NodeNoMap.find(origin_node_number) != m_NodeNoMap.end())  // convert node number to internal node id
			{
				origin_node_id = m_NodeNoMap[origin_node_number]->m_NodeNo;
			}

			int destination_node_id = -1;
			int destination_node_number = -1;
			parser_agent.GetValueByFieldName("destination_node_id", destination_node_number);

			if (m_NodeNoMap.find(destination_node_number) != m_NodeNoMap.end()) // convert node number to internal node id
			{
				destination_node_id = m_NodeNoMap[destination_node_number]->m_NodeNo;
			}

			pVehicle->m_OriginNodeID = origin_node_id;
			pVehicle->m_DestinationNodeID = destination_node_id;


			//if (origin_node_id == destination_node_id)
			//{  // do not simulate intra zone traffic
			//	continue;
			//}

			if (m_ZoneMap.find(pVehicle->m_OriginZoneID) != m_ZoneMap.end())
			{
				m_ZoneMap[pVehicle->m_OriginZoneID].m_Demand += 1;
				m_ZoneMap[pVehicle->m_OriginZoneID].m_OriginVehicleSize += 1;

			}

			float departure_time = 0;
			parser_agent.GetValueByFieldName("departure_time", departure_time);

			pVehicle->m_DepartureTime = departure_time;
			int beginning_departure_time = departure_time;


			parser_agent.GetValueByFieldName("demand_type", pVehicle->m_DemandType);
			parser_agent.GetValueByFieldName("vehicle_type", pVehicle->m_VehicleType);

			parser_agent.GetValueByFieldName("information_type", pVehicle->m_InformationClass);
			parser_agent.GetValueByFieldName("value_of_time", pVehicle->m_VOT);
			parser_agent.GetValueByFieldName("vehicle_age", pVehicle->m_Age);
			int day_no;
			parser_agent.GetValueByFieldName("day_no", day_no);
			std::string agent_type;
			parser_agent.GetValueByFieldName("agent_type", agent_type);

			if (agent_type == "p")
				m_PassengerIDStringVector.push_back(agent_id);
			else
				m_VehicleIDStringVector.push_back(agent_id);



			int number_of_nodes = 0;
			//	parser_agent.GetValueByFieldName("number_of_nodes", number_of_nodes);

			std::vector<int> path_node_sequence;

			string path_node_sequence_str;
			if (parser_agent.GetValueByFieldName("path_node_sequence", path_node_sequence_str) == true)
			{

				path_node_sequence = ParseLineToIntegers(path_node_sequence_str);
				number_of_nodes = path_node_sequence.size();


			}

			std::vector<float> path_time_sequence;

			string path_time_sequence_str;
			if (parser_agent.GetValueByFieldName("path_time_sequence", path_time_sequence_str) == true)
			{

				path_time_sequence = ParseLineToFloat(path_time_sequence_str);
				int number_of_time_nodes = path_time_sequence_str.size();

			}

			pVehicle->m_Distance = 0;

			for (int i = 0; i < path_node_sequence.size(); i++)
			{
				int node_number = path_node_sequence[i];
				if (m_NodeNumbertoNodeNoMap.find(node_number) != m_NodeNumbertoNodeNoMap.end())  // convert node number to internal node id
				{
					int NodeNo = m_NodeNumbertoNodeNoMap[node_number];

					VehicleLocationRecord element;
					element.agent_no = count;
						element.agent_id = agent_id;
						element.agent_type = agent_type;

						element.x = m_NodeNoMap[NodeNo]->pt.x;
						element.y = m_NodeNoMap[NodeNo]->pt.y;

						if (i < path_time_sequence.size())  // the same elements in node and time sequences
						{
							element.time_stamp_in_second = path_time_sequence[i] * 60;  //min to seconds
						}
						else //default
						{
							element.time_stamp_in_second = 0;
						}

						element.day_no = day_no;

						if (path_node_sequence.size()>=2 && i < path_node_sequence.size() - 2)
						{
							DTALink* pLink = FindLinkWithNodeNumbers(path_node_sequence[i], path_node_sequence[i + 1]);

							if (pLink != NULL)
								element.link_no = pLink->m_LinkNo;

						}
						AddLocationRecord(element);
				}  //node fount
			}
			count++;

		}
		m_SimulationVehicleDataLoadingStatus.Format("%d agent data are loaded from file %s.", count, lpszFileName);
		return true;
	}
}

bool CTLiteDoc::ReadTransitTripCSVFile(LPCTSTR lpszFileName, int version_number = 2)
{
	CCSVParser parser_agent;

	float total_number_of_vehicles_to_be_generated = 0;

	if (parser_agent.OpenCSVFile(lpszFileName))
	{
		int count = 0;

		int line_no = 1;

		int i = 0;
		while (parser_agent.ReadRecord())
		{

			string vehicle_id, trip_type;

			parser_agent.GetValueByFieldName("vehicle_id", vehicle_id);
			parser_agent.GetValueByFieldName("trip_type", trip_type);

			int origin_node_id = -1;
			int origin_node_number = -1;


			std::vector<CCoordinate> CoordinateVector;
			string geo_string;

			//if (parser_agent.GetValueByFieldName("geometry", geo_string) == false)
			//{
			//	// overwrite when the field "geometry" exists
			//	CGeometry geometry(geo_string);
			//	CoordinateVector = geometry.GetCoordinateList();
			//	for (int si = 0; si < CoordinateVector.size(); si++)
			//	{
			//		GDPoint	pt;
			//		pt.x = CoordinateVector[si].X;
			//		pt.y = CoordinateVector[si].Y;
			//		pVehicle->m_ShapePoints.push_back(pt);

			//	}

			//}


			parser_agent.GetValueByFieldName("origin_node_id", origin_node_number);

			if (m_NodeNoMap.find(origin_node_number) != m_NodeNoMap.end())  // convert node number to internal node id
			{
				origin_node_id = m_NodeNoMap[origin_node_number]->m_NodeNo;
			}

			int destination_node_id = -1;
			int destination_node_number = -1;
			parser_agent.GetValueByFieldName("destination_node_id", destination_node_number);

			if (m_NodeNoMap.find(destination_node_number) != m_NodeNoMap.end()) // convert node number to internal node id
			{
				destination_node_id = m_NodeNoMap[destination_node_number]->m_NodeNo;
			}


			//if (origin_node_id == destination_node_id)
			//{  // do not simulate intra zone traffic
			//	continue;
			//}

			float schedule_departure_time= 0;
			parser_agent.GetValueByFieldName("schedule_departure_time", schedule_departure_time);

			int day_no;
			parser_agent.GetValueByFieldName("day_no", day_no);

			int number_of_nodes = 0;

			std::vector<int> path_node_sequence;

			string path_node_sequence_str;
			if (parser_agent.GetValueByFieldName("path_node_sequence", path_node_sequence_str) == true)
			{

				path_node_sequence = ParseLineToIntegers(path_node_sequence_str);
				number_of_nodes = path_node_sequence.size();


			}

			float speed_limit_factor = 1;

			parser_agent.GetValueByFieldName("speed_limit_factor", speed_limit_factor);

			if (speed_limit_factor >= 1)
				speed_limit_factor = 1;

			if (speed_limit_factor <= 0.01)
				speed_limit_factor = 0.01;

			std::vector<float> path_time_sequence;

			string path_time_sequence_str;
			int number_of_time_nodes = 0;
			if (parser_agent.GetValueByFieldName("path_time_sequence", path_time_sequence_str) == true)
			{

				path_time_sequence = ParseLineToFloat(path_time_sequence_str);
				number_of_time_nodes = path_time_sequence.size();

			}
			//re-generate time schedule 
			if (number_of_time_nodes == 0)
			{
				path_time_sequence.clear();

				path_time_sequence.push_back(schedule_departure_time);

				float current_time = schedule_departure_time;
				if (path_node_sequence.size() >= 2)
				{
					for (int i = 0; i < path_node_sequence.size() - 1; i++)  // for each pass link
					{
						float transit_travel_time = 1;
						DTALink* pLink = FindLinkWithNodeNumbers(path_node_sequence[i], path_node_sequence[i + 1]);


						if (pLink != NULL)
						{
							transit_travel_time = pLink->m_FreeFlowTravelTime / speed_limit_factor;

						}

						current_time += transit_travel_time;
						path_time_sequence.push_back(current_time);

					}


				}
			}

			// add location data
				for (int i = 0; i < path_node_sequence.size(); i++)
				{
					int node_number = path_node_sequence[i];


					if (m_NodeNumbertoNodeNoMap.find(node_number) != m_NodeNumbertoNodeNoMap.end())  // convert node number to internal node id
					{
						int NodeNo = m_NodeNumbertoNodeNoMap[node_number];

						VehicleLocationRecord element;
						element.agent_no = count;
						//			element.agent_id = agent_id;
						//			element.agent_type = agent_type;

						element.x = m_NodeNoMap[NodeNo]->pt.x;
						element.y = m_NodeNoMap[NodeNo]->pt.y;

						if (2 * i < path_time_sequence.size())  // the same elements in node and time sequences
						{
							element.time_stamp_in_second = path_time_sequence[2 * i] * 60;  //min to seconds
						}
						else //default
						{
							element.time_stamp_in_second = 0;
						}

						element.day_no = day_no;

						if (i< path_node_sequence.size()-1)
						{
							DTALink* pLink = FindLinkWithNodeNumbers(path_node_sequence[i], 
								path_node_sequence[i+1]);

							if (pLink != NULL)
								element.link_no = pLink->m_LinkNo;

						}
						AddLocationRecord(element);

					}  //node fount
				}
			count++;

		}
		m_SimulationVehicleDataLoadingStatus.Format("%d bus schedule lines are loaded from file %s.", count, lpszFileName);
		return true;
	}
}
void  CTLiteDoc::UpdateMovementDataFromVehicleTrajector()
{
	// movement count
	std::list<DTANode*>::iterator iNode;

	for(int tp = 0; tp< m_TimingPlanVector.size(); tp++)  // first loop for each timing plan
	{

		std::string timing_plan_name = m_TimingPlanVector[tp].timing_plan_name;  // fetch timing_plan (unique) name

		for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{
			(*iNode)->ResetMovementMOE (timing_plan_name);		
		}
	}

	std::list<DTAVehicle*>::iterator iVehicle;
	for (iVehicle = m_VehicleSet.begin(); iVehicle != m_VehicleSet.end(); iVehicle++)
	{
		DTAVehicle* pVehicle = (*iVehicle);

		if(pVehicle==NULL)
			break;

		if(pVehicle->m_VehicleID  == 35)
		{
			TRACE("\nveh = %d",pVehicle->m_VehicleID );

		}
		if(pVehicle ->m_bComplete  == false)
			continue;

		for(int i= 2; i<pVehicle->m_NodeSize; i++)
		{

			if(pVehicle->m_NodeAry == NULL)
				return;

			int link = pVehicle->m_NodeAry[i-1].LinkNo;
			if( m_LinkNoMap.find(link)== m_LinkNoMap.end())
				return;  // error 

			DTALink* pLink0 = m_LinkNoMap[pVehicle->m_NodeAry[i-1].LinkNo];
			DTALink* pLink1 = m_LinkNoMap[pVehicle->m_NodeAry[i].LinkNo];

			if(pLink0==NULL || pLink1== NULL)
				break;

			float traveltime = pVehicle->m_NodeAry[i-1].ArrivalTimeOnDSN -  max(pVehicle->m_DepartureTime , pVehicle->m_NodeAry[i-2].ArrivalTimeOnDSN);
			float delay = traveltime - pLink0->m_FreeFlowTravelTime ;

			if(delay <0)
				delay = 0;

			if(traveltime < pLink0->m_FreeFlowTravelTime)
				traveltime = pLink0->m_FreeFlowTravelTime;


			if(pLink0->m_FromNodeNumber == 1 && pLink0->m_ToNodeNumber == 5 && pLink1->m_ToNodeNumber == 3)
			{
				TRACE("delay: %f\n", delay);
			}

			DTANode* pNode = m_NodeNoMap[ pLink0->m_ToNodeID ];

			if(pNode->m_NodeNumber == 36)
			{
				TRACE("");
			}

			if(pLink0 !=NULL && pLink1!=NULL)
			{
				pLink0->m_TotalVolumeForMovementCount++;
				pLink0->m_total_travel_time +=traveltime;



				//
				////TO DO: fetch timing_plan no according to day no and time period
				//DTATimingPlan timing_plan = FindTimingPlanFromStartTime( pVehicle->m_NodeAry[i].ArrivalTimeOnDSN);

				//pNode->AddMovementCountAndDelay(pLink0->m_FromNodeID, pLink1->m_ToNodeID, delay , timing_plan);

				//if(timing_plan.timing_plan_name != "0")
				{
					DTATimingPlan timing_plan_all_day;

					timing_plan_all_day.timing_plan_name = "0";

					timing_plan_all_day.start_time_in_min = m_DemandLoadingStartTimeInMin;
					timing_plan_all_day.end_time_in_min = m_DemandLoadingEndTimeInMin;

					pNode->AddMovementCountAndDelay(pLink0->m_FromNodeID, pLink1->m_ToNodeID, delay , timing_plan_all_day);
				}

			}

		}
	}

	// turning percentage
	for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{
		for(unsigned int m = 0; m< (*iNode)->m_MovementDataMap["0"].m_MovementVector .size(); m++)
		{
			DTANodeMovement* pMovement = &((*iNode)->m_MovementDataMap["0"].m_MovementVector[m]);
			DTALink* pLink0 = m_LinkNoMap[pMovement->IncomingLinkNo  ];

			if(pLink0->m_FromNodeNumber == 1)
			{
				TRACE("");
			}

			int total_link_count = 0;
			for(unsigned int j = 0; j< (*iNode)->m_MovementDataMap["0"].m_MovementVector .size(); j++)
			{

				if((*iNode)->m_MovementDataMap["0"].m_MovementVector[j].IncomingLinkNo == pMovement->IncomingLinkNo )
				{
					total_link_count+= (*iNode)->m_MovementDataMap["0"].m_MovementVector[j].sim_turn_count ;
				}

			}

			if (pMovement->sim_turn_count >=1)
			{
				pMovement->turning_percentage = 
					pMovement->sim_turn_count * 100.0f / max(1,total_link_count);

				pMovement->sim_turn_delay  = 
					pMovement->sim_turn_delay/ max(1,pMovement->sim_turn_count);

			}else
			{
				pMovement->sim_turn_delay  = 0 ;
			}
		}

	}


	std::list<DTALink*>::iterator iLink;

	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->m_avg_simulated_speed =  (*iLink)->m_Length / ((*iLink)->m_total_travel_time/max(1,(*iLink)->m_total_link_volume))*60; 

	}

}
int CTLiteDoc::GetVehilePosition(DTAVehicle* pVehicle, double CurrentTime, float& ratio)
{
	GDPoint pt;
	pt.x = 0;
	pt.y = 0;

	int search_counter = 0;
	int beg, end, mid;
	beg = 1;
	end = pVehicle->m_NodeSize-1;
	mid = (beg+end)/2;
	int i = mid;
	ratio = 0;
	float link_travel_time;
	float traveling_time;

	while(beg<=end)
	{
		if(CurrentTime >= pVehicle->m_NodeAry [i-1].ArrivalTimeOnDSN &&
			CurrentTime <= pVehicle->m_NodeAry [i].ArrivalTimeOnDSN )	// find the link between the time interval
		{

			link_travel_time = pVehicle->m_NodeAry [i].ArrivalTimeOnDSN - pVehicle->m_NodeAry [i-1].ArrivalTimeOnDSN;

			traveling_time = CurrentTime - pVehicle->m_NodeAry [i-1].ArrivalTimeOnDSN;

			ratio = traveling_time/link_travel_time;

			if(ratio <0)
				ratio = 0;

			if(ratio >1)
				ratio = 1;

			if(pVehicle->m_NodeAry [i].LinkNo  == 457)
				TRACE("");

			return pVehicle->m_NodeAry [i].LinkNo ;

			//			pt.x =  ratio*pLink->m_FromPoint .x  +  (1-ratio)*ratio*pLink->m_ToPoint .x;
			//			pt.y =  ratio*pLink->m_FromPoint .y  +  (1-ratio)*ratio*pLink->m_ToPoint .y;

		}else  // not found
		{
			if( CurrentTime>= pVehicle->m_NodeAry [i].ArrivalTimeOnDSN)  // time stamp is after i
			{
				// shift to the right
				beg=mid+1;
				mid=(beg+end)/2;
				i = mid;
			}else //CurrentTime < pVehicle->m_NodeAry[i].ArrivalTimeOnDSN// time stamp is before i
			{   //shift to the left
				end=mid-1;
				mid=(beg+end)/2;
				i = mid;
			}

			search_counter++;

			if(search_counter > pVehicle->m_NodeSize)  // exception handling
			{
				//				ASSERT(false);
				return 0;
			}

		}
	}

	return 0;
}

bool CTLiteDoc::GetAgentPosition(string agent_id, double CurrentTime_in_min, GDPoint& pt)
{
	float ratio = 0;
	double CurrentTime = CurrentTime_in_min * 60;  // in seconds
	pt.x = 0;
	pt.y = 0;

	int search_counter = 0;
	int beg, end, mid;
	beg = 1;
	end = m_VehicleWithLocationVectorMap[agent_id].VehicleLocationRecordVector.size() - 1;
	mid = (beg + end) / 2;
	int i = mid;
	ratio = 0;
	float link_travel_time;
	float traveling_time;

	while (beg <= end)
	{
		if (CurrentTime >= m_VehicleWithLocationVectorMap[agent_id].VehicleLocationRecordVector[i - 1].time_stamp_in_second &&
			CurrentTime <= m_VehicleWithLocationVectorMap[agent_id].VehicleLocationRecordVector[i].time_stamp_in_second)	// find the link between the time interval
		{

			link_travel_time = m_VehicleWithLocationVectorMap[agent_id].VehicleLocationRecordVector[i].time_stamp_in_second 
				- m_VehicleWithLocationVectorMap[agent_id].VehicleLocationRecordVector[i].time_stamp_in_second;

			traveling_time = CurrentTime - m_VehicleWithLocationVectorMap[agent_id].VehicleLocationRecordVector[i - 1].time_stamp_in_second;

			ratio = traveling_time / link_travel_time;

			if (ratio <0)
				ratio = 0;

			if (ratio >1)
				ratio = 1;


			int LinkNo = m_VehicleWithLocationVectorMap[agent_id].VehicleLocationRecordVector[i].link_no;

			if (LinkNo >= 0)
			{
				DTALink* pLink = m_LinkNoMap[LinkNo];
				pt = pLink->GetRelativePosition(ratio);
				return true;
			}
			else
			{
				
				pt.x = m_VehicleWithLocationVectorMap[agent_id].VehicleLocationRecordVector[i - 1].x +

					m_VehicleWithLocationVectorMap[agent_id].VehicleLocationRecordVector[i].x;
				return true;

			}
		}
		else  // not found
		{
			if (CurrentTime >= m_VehicleWithLocationVectorMap[agent_id].VehicleLocationRecordVector[i].time_stamp_in_second)  // time stamp is after i
			{
				// shift to the right
				beg = mid + 1;
				mid = (beg + end) / 2;
				i = mid;
			}
			else //CurrentTime < pVehicle->m_NodeAry[i].ArrivalTimeOnDSN// time stamp is before i
			{   //shift to the left
				end = mid - 1;
				mid = (beg + end) / 2;
				i = mid;
			}

			search_counter++;

			if (search_counter > m_VehicleWithLocationVectorMap[agent_id].VehicleLocationRecordVector.size())  // exception handling
			{
				//				ASSERT(false);
				return false;
			}

		}
	}

	return false;
}

bool CTLiteDoc::GetGPSVehilePosition(DTAVehicle* pVehicle, double CurrentTime, GDPoint& pt)
{

	pt.x = pVehicle->m_LocationRecordAry[0].x;
	pt.y = pVehicle->m_LocationRecordAry[0].y;

	int search_counter = 0;
	int beg, end, mid;
	beg = 1;
	end = pVehicle->m_NodeSize-1;
	mid = (beg+end)/2;
	int i = mid;
	double ratio = 0;
	float link_travel_time;
	float remaining_time;

	while(beg<=end)
	{
		if(CurrentTime >= pVehicle->m_LocationRecordAry [i-1].time_stamp_in_min &&
			CurrentTime <= pVehicle->m_LocationRecordAry [i].time_stamp_in_min )	// find the link between the time interval
		{

			link_travel_time = pVehicle->m_LocationRecordAry [i].time_stamp_in_min - pVehicle->m_LocationRecordAry [i-1].time_stamp_in_min;

			remaining_time = CurrentTime - pVehicle->m_LocationRecordAry [i-1].time_stamp_in_min;

			ratio = 1-remaining_time/link_travel_time;

			if(ratio <0)
				ratio = 0;

			if(ratio >1)
				ratio = 1;

			pt.x = ratio * pVehicle->m_LocationRecordAry [i-1].x + (1- ratio)* pVehicle->m_LocationRecordAry [i].x ;
			pt.y = ratio * pVehicle->m_LocationRecordAry [i-1].y + (1- ratio)* pVehicle->m_LocationRecordAry [i].y ;

			return true ;

		}else  // not found
		{
			if( CurrentTime>= pVehicle->m_LocationRecordAry [i].time_stamp_in_min)  // time stamp is after i
			{
				// shift to the right
				beg=mid+1;
				mid=(beg+end)/2;
				i = mid;
			}else //CurrentTime < pVehicle->m_NodeAry[i].ArrivalTimeOnDSN// time stamp is before i
			{   //shift to the left
				end=mid-1;
				mid=(beg+end)/2;
				i = mid;
			}

			search_counter++;

			if(search_counter > pVehicle->m_NodeSize)  // exception handling
			{
				//				ASSERT(false);
				return false;
			}

		}
	}

	return false;
}

void CTLiteDoc::OnImageImportbackgroundimage()
{

	if(!m_BackgroundBitmapLoaded)
		ReadBackgroundImageFile(m_ProjectFile);

	m_ImageX2  = m_ImageX1+ m_ImageWidth * m_ImageXResolution;
	m_ImageY2  = m_ImageY1+ m_ImageHeight * m_ImageYResolution;

	CDlg_ImageSettings dlg_image;

	dlg_image.m_pDoc = this;

	dlg_image.m_X1 = m_ImageX1;
	dlg_image.m_Y1 = m_ImageY1;

	dlg_image.m_X2 = m_ImageX2;
	dlg_image.m_Y2 = m_ImageY2;

	if(m_BackgroundBitmapLoaded)
	{
		dlg_image.m_Image_File_Message  = "Background bitmap file has been loaded.";

	}
	else 
	{
		dlg_image.m_Image_File_Message  = "Please prepare background bitmap file as\nbackground_image.bmp.";
	}

	if(dlg_image.DoModal ()==IDOK)
	{
		m_ImageX1 = dlg_image.m_X1;
		m_ImageY1 = dlg_image.m_Y1;

		m_ImageX2 = dlg_image.m_X2;
		m_ImageY2 = dlg_image.m_Y2;

		OnFileSaveimagelocation();

	}

	m_bFitNetworkInitialized = false;
	UpdateAllViews(0);
}


void CTLiteDoc::OnFileDataloadingstatus()
{
	CDlgFileLoading dlg;
	dlg.m_pDoc = this;
	dlg.DoModal ();
}

void CTLiteDoc::OnMoeVolume()
{
	m_LinkMOEMode = MOE_volume;
	m_LinkBandWidthMode = LBW_link_volume;
	ShowLegend(true);

	GenerateOffsetLinkBand();
	UpdateAllViews(0);}

void CTLiteDoc::OnMoeSpeed()
{
	m_LinkMOEMode = MOE_speed;
	m_LinkBandWidthMode = LBW_link_volume;

	ShowLegend(true);
	GenerateOffsetLinkBand();
	UpdateAllViews(0);
}

void CTLiteDoc::OnMoeDensity()
{
	m_LinkMOEMode = MOE_density;
	m_LinkBandWidthMode = LBW_link_volume;
	ShowLegend(true);

	GenerateOffsetLinkBand();
	UpdateAllViews(0);
}

void CTLiteDoc::OnMoeQueuelength()
{
	m_LinkMOEMode = MOE_queuelength;
	m_LinkBandWidthMode = LBW_number_of_lanes;


	GenerateOffsetLinkBand();
	UpdateAllViews(0);}

void CTLiteDoc::OnMoeFuelconsumption()
{
	m_LinkMOEMode = MOE_fuel;
	m_LinkBandWidthMode = LBW_link_volume;

	UpdateAllViews(0);}

void CTLiteDoc::OnMoeEmissions()
{
	m_LinkMOEMode = MOE_emissions;
	m_LinkBandWidthMode = LBW_link_volume;
	ShowLegend(true);
	UpdateAllViews(0);}

void CTLiteDoc::OnUpdateMoeVolume(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_volume);
}

void CTLiteDoc::OnUpdateMoeSpeed(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_speed);
}

void CTLiteDoc::OnUpdateMoeDensity(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_density);
}

void CTLiteDoc::OnUpdateMoeQueuelength(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_queuelength);
}

void CTLiteDoc::OnUpdateMoeFuelconsumption(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_fuel);
}

void CTLiteDoc::OnUpdateMoeEmissions(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_emissions);

}

void CTLiteDoc::OnMoeNone()
{
	m_LinkMOEMode = MOE_none;

	ShowLegend(false);

	// visualization configuration
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateMoeNone(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_none);
}

float CTLiteDoc::GetLinkMOE(DTALink* pLink, Link_MOE LinkMOEMode,int CurrentTime, int AggregationIntervalInMin, float &value)
{

	float power = 0.0f;
	float max_link_volume = 8000.0f;
	float max_speed_ratio = 2.0f; 
	float max_density = 45.0f;

	switch (LinkMOEMode)
	{
		//case MOE_volume:  power = pLink->m_total_link_volume/max_link_volume; 
		//	value = pLink->m_total_link_volume;
		//	break;
	case MOE_speed:   power = pLink->m_avg_simulated_speed/max(1, pLink->m_SpeedLimit) * 100;
		value = pLink->m_avg_simulated_speed;
		break;

	case MOE_volume:   
		value = pLink->m_total_link_volume;
		break;
		

	case MOE_reliability:   power = pLink->m_volume_over_capacity_ratio;
		value = pLink->m_volume_over_capacity_ratio;
		break;

	case MOE_density: power = 0;
		value = 0;
		break;
	case MOE_traveltime:  
		value = pLink->m_StaticTravelTime;
		break;

	case MOE_capacity:  
		value = pLink->m_LaneCapacity * pLink->m_NumberOfLanes ;
		break;

	case MOE_speedlimit:  
		value = pLink->m_SpeedLimit  ;
		break;

	case MOE_fftt: 
		value = pLink->m_FreeFlowTravelTime ;
		break;

	case MOE_length: 
		value = pLink->m_Length  ;
		break;

	case MOE_queue_length:
		value =  0;
		break;

	case MOE_safety:  power = pLink->m_number_of_all_crashes/ max(0.0001,pLink->m_Length ); 
		value = pLink->m_number_of_all_crashes/ max(0.0001,pLink->m_Length );
		break;


	default: power = 0.0;

	}

	if(m_TrafficFlowModelFlag > 0)
	{
		int StartTime = CurrentTime;
		int EndTime  = CurrentTime + AggregationIntervalInMin;

		float max_power = 0; 
		float min_power = 1;

		if(LinkMOEMode == MOE_reliability)
		{
			CurrentTime = int(CurrentTime/60)*60;  // hour by hour
			AggregationIntervalInMin = 60;
			EndTime  = CurrentTime + AggregationIntervalInMin;

		}
		float total_value = 0;
		int total_measurement_count = 0;

		//		value = pLink->GetWithinDayLinkMOE(LinkMOEMode, m_PrimaryDataSource, CurrentTime);

		for(CurrentTime  = StartTime; CurrentTime < EndTime; CurrentTime ++)
		{


			if(pLink->IsSimulatedDataAvailable (CurrentTime) && pLink->GetSimulatedLinkVolume (CurrentTime)>=1 ||
				pLink->GetSimulatedDensity(CurrentTime) >= 0.1 || m_PrimaryDataSource == eSensorData || m_PrimaryDataSource == eFloatingCarData)
			{

				total_measurement_count++;

				switch (LinkMOEMode)
				{
				case MOE_volume:  

					total_value+= pLink->GetDynamicLinkVolume (CurrentTime,m_PrimaryDataSource);

					break;
				case MOE_speed: 
					total_value+= pLink->GetDynamicSpeed(CurrentTime,m_PrimaryDataSource);
					break;
				case MOE_traveltime:
					total_value+= pLink->GetDynamicTravelTime (CurrentTime);
					break;
				case MOE_density:
					total_value+= pLink->GetSimulatedDensity(CurrentTime);
					break;
				case MOE_queue_length: 
					total_value+= pLink->GetQueueLengthPercentage(CurrentTime);
					break;

				case MOE_emissions: 
					total_value+= pLink->GetEmissions(CurrentTime, this->m_EmissionType );
					break;

				}

			}
		}  // end of for loop

		//update value when there is at one valid data point
		if(total_measurement_count>=1)
		{
			value = total_value/max(1,total_measurement_count);
		}


		switch (LinkMOEMode)
		{
		case MOE_volume:  
			power = value/max_link_volume;
			break;
		case MOE_speed: 
			power =  min(100,value / pLink->m_SpeedLimit*100);
			break;
		case MOE_reliability: 
			power =  value / pLink->m_SpeedLimit;
			break;
		case MOE_traveltime:
			if(value <=0.1)  // no data
				power = 0;
			else 
				power = pLink->m_SpeedLimit / max(1,value)*100; 

			break;
		case MOE_density:
			power = value; 
			break;
		case MOE_queue_length: 
			power = value  ; 
			break;
		case MOE_safety:  power = pLink->m_number_of_all_crashes/ max(0.0001,pLink->m_Length ); 
			value = pLink->m_number_of_all_crashes/ max(0.0001,pLink->m_Length );
			break;

		case MOE_emissions:
			power = value;
			break;


		}

		if(LinkMOEMode == MOE_speed && CurrentTime >= 1020&& pLink->m_FromNodeNumber == 1354 &&  pLink->m_ToNodeNumber == 756)
		{
			int LOS = GetLOSCode(power);
			TRACE("");
		}

	}

	return power;
}

void CTLiteDoc::OnToolsCarfollowingsimulation()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}


}

DWORD CTLiteDoc::ProcessWait(DWORD PID) 
{
	DWORD dwRetVal = DWORD(-1);
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, PID);
	if(hProcess) {
		do 
		{
			::Sleep(500);
			::GetExitCodeProcess(hProcess, &dwRetVal);
		} while(dwRetVal == STILL_ACTIVE);
		CloseHandle(hProcess);
	}
	return dwRetVal;
}

DWORD CTLiteDoc::ProcessExecute(CString & strCmd, CString & strArgs,  CString & strDir, BOOL bWait)
{

	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	StartupInfo.cb = sizeof(STARTUPINFO);

	PROCESS_INFORMATION ProcessInfo;
	DWORD dwRetVal = DWORD(-1);

	/*
	TCHAR szExe[1024];
	if( int(FindExecutable(
	strPath,									// pointer to filename
	strDir.IsEmpty() ? 0 : LPCTSTR(strDir),					// pointer to default directory
	szExe										// result-buffer
	)) <= 32 ) {
	OnExecError(GetLastError(), strCmd);
	return DWORD(-1);
	}
	*/
	BOOL bStat =	CreateProcess(
		strCmd,						// pointer to name of executable module
		0,			// pointer to command line string
		0,						// pointer to process security attributes
		0,						// pointer to thread security attributes
		TRUE,						// handle inheritance flag
		0,						// creation flags
		0,						// pointer to new environment block
		strDir.IsEmpty() ? 0 : LPCTSTR(strDir),
		&StartupInfo,				// pointer to STARTUPINFO
		&ProcessInfo				// pointer to PROCESS_INFORMATION
		);
	if( bStat ) 
	{
		if( bWait ) 
		{
			::WaitForInputIdle(ProcessInfo.hProcess, INFINITE);
			dwRetVal = ProcessWait(ProcessInfo.dwProcessId);
		} else {
			// before we return to the caller, we wait for the currently
			// started application until it is ready to work.
			::WaitForInputIdle(ProcessInfo.hProcess, INFINITE);
			dwRetVal = ProcessInfo.dwProcessId;
		}

		::CloseHandle(ProcessInfo.hThread);
	}

	return dwRetVal;
}


void CTLiteDoc::OnToolsPerformtrafficassignment()
{
	STARTUPINFO si = { 0 };  
	PROCESS_INFORMATION pi = { 0 };  

	CopyDefaultFiles();

	si.cb = sizeof(si); 

	CWaitCursor curs;
	if(!SetCurrentDirectory(m_ProjectDirectory))
	{
		if(m_ProjectDirectory.GetLength()==0)
		{
			AfxMessageBox("The project directory has not been specified.");
			return;
		}
		CString msg;
		msg.Format ("The specified project directory %s does not exist.", m_ProjectDirectory);
		AfxMessageBox(msg);
		return;
	}

	if(IsModified())
		SaveProject(m_ProjectFile);


	CString sCommand;
	CString strParam;
	CTime ExeStartTime = CTime::GetCurrentTime();

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();


#ifndef _WIN64
		sCommand.Format("%s\\%s", pMainFrame->m_CurrentDirectory,theApp.m_SimulatorString_32);
#else
		sCommand.Format("%s\\%s", pMainFrame->m_CurrentDirectory,theApp.m_SimulatorString_64);
#endif

	ProcessExecute(sCommand, strParam, m_ProjectDirectory, true);


	FILE* pFile;

	int OutputAgentFileSize = 0;
	fopen_s(&pFile,m_ProjectDirectory + "\\output_LinkMOE.csv","rb");
	if(pFile!=NULL)
	{
		fseek(pFile, 0, SEEK_END );
		OutputAgentFileSize = ftell(pFile);
		fclose(pFile);
	}

	CTime ExeEndTime = CTime::GetCurrentTime();

	CTimeSpan ts = ExeEndTime  - ExeStartTime;
	CString str_running_time;

	FILE* st = NULL;

	CString directory = m_ProjectDirectory;

	if(OutputAgentFileSize >=1 && ts.GetTotalSeconds() >=1)
	{

		if(m_bDYNASMARTDataSet)
		{
			str_running_time.Format ("Simulation program execution has completed.\nProgram execution time: %d hour(s) %d min(s) %d sec(s).\nPlease check time-dependent link MOEs during period %s->%s.\nDo you want to load the simulation results now?",

				ts.GetHours(), ts.GetMinutes(), ts.GetSeconds(),
				GetTimeStampString24HourFormat(m_DemandLoadingStartTimeInMin),
				GetTimeStampString24HourFormat(m_DemandLoadingEndTimeInMin));

			if( AfxMessageBox(str_running_time, MB_YESNO| MB_ICONINFORMATION)==IDYES)
			{
				ReadDYNASMARTSimulationResults();
			}
		}else
		{
			str_running_time.Format ("Simulation program execution has completed.\nProgram execution time: %d hour(s) %d min(s) %d sec(s).\nPlease check time-dependent link MOEs during period %s->%s.\nDo you want to view the output summary file now?",

				ts.GetHours(), ts.GetMinutes(), ts.GetSeconds(),
				GetTimeStampString24HourFormat(m_DemandLoadingStartTimeInMin),
				GetTimeStampString24HourFormat(m_DemandLoadingEndTimeInMin));

			if( AfxMessageBox(str_running_time, MB_YESNO| MB_ICONINFORMATION)==IDYES)
			{
				OnToolsViewassignmentsummarylog();
			}
			LoadSimulationOutput();
		}
		UpdateAllViews(0);
	}else
	{
		str_running_time.Format ("Program %s cannot be found or the simulation does not complete.\n", sCommand);
		AfxMessageBox(str_running_time,  MB_ICONINFORMATION);

	}



}

void CTLiteDoc::RunNEXTA_32()
{
  

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	CString sCommand, strParam, strWorkingFolder;
	sCommand.Format("%s\\NEXTA_32.exe", pMainFrame->m_CurrentDirectory);
	strWorkingFolder.Format("%s", pMainFrame->m_CurrentDirectory);
	ProcessExecute(sCommand, strParam, strWorkingFolder, true);



}

void CTLiteDoc::LoadSimulationOutput()
{

	ReadMetaDemandCSVFile(m_ProjectDirectory + "input_demand_file_list.csv");
	ReadScenarioSettingCSVFile(m_ProjectDirectory + "input_scenario_settings.csv");

	CString DTASettingsPath = m_ProjectDirectory + "DTASettings.txt";
	g_Simulation_Time_Horizon = 1440;
	SetStatusText("Loading output link time-dependent data");

	ReadSensorLocationData(m_ProjectDirectory + "input_sensor_location.csv");

	ReadSensorCountData(m_ProjectDirectory + "sensor_count.csv");
	ReadSensorSpeedData(m_ProjectDirectory + "sensor_speed.csv");




	CCSVParser parser;

	//step 1: static link MOE csv
	ReadSimulationLinkOvarvallMOEData(m_ProjectDirectory + "output_linkMOE.csv");

	// step 2: TDMOE bin
	ReadSimulationLinkMOEData_Bin(m_ProjectDirectory + "output_LinkTDMOE.bin");

	// step 3: 
		if (ReadVehicleBinFile(m_ProjectDirectory + "agent_scenario.bin", 2) == false)
	{
		if (ReadVehicleBinFile(m_ProjectDirectory + "agent.bin", 2) == false)
		{
			ReadVehicleCSVFile_Parser(m_ProjectDirectory+ "output_agent.csv");
			RecalculateLinkMOEFromVehicleTrajectoryFile(); 
//			ReadSensorTrajectoryData(m_ProjectDirectory + "output_trajectory.csv");

		}// try version 2 format first
	}

	ReadTransitTripCSVFile(m_ProjectDirectory + "input_transit_trip.csv", 2);


	//ReadModelLinkMOEData_Parser(m_ProjectDirectory+"model_linkMOE.csv");


	ReadInputPathCSVFile(m_ProjectDirectory+ "optional_path.csv");

	int speed_data_aggregation_interval = 15;

	ReadSensorSpeedData(m_ProjectDirectory+ "sensor_speed_matrix.csv", speed_data_aggregation_interval);

	ReadTransitFiles(m_ProjectDirectory+"transit_data\\");  // read transit data

	//char microsimulation_file_name[_MAX_STRING_SIZE];
	//g_GetProfileString("microsimulation_data","file_name","",microsimulation_file_name,sizeof(microsimulation_file_name),m_ProjectFile);

	//CString microsimulation_file_str;

	//microsimulation_file_str.Format("%s",microsimulation_file_name);

	//if(microsimulation_file_str.Find("csv") >=0)
	//{
	//	ReadAimCSVFiles(m_ProjectDirectory+microsimulation_file_name,0);
	//}


	//LoadGPSData();
	
	
	
	SetStatusText("Generating OD statistics...");

	ResetODMOEMatrix();
	//	ReadAMSPathCSVFile(m_ProjectDirectory+"AMS_path_flow.csv");
}


void CTLiteDoc::LoadGPSData()
{
	ReadGPSBinFile(m_ProjectDirectory+"input_GPS.bin",0,0);
}
void CTLiteDoc::OnMoeVcRatio()
{
	m_LinkMOEMode = MOE_density;
	ShowLegend(true);
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateMoeVcRatio(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_density);
}

void CTLiteDoc::OnMoeTraveltime()
{
	m_LinkMOEMode = MOE_traveltime;

	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateMoeTraveltime(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_traveltime);
}

void CTLiteDoc::OnMoeCapacity()
{
	m_LinkMOEMode = MOE_capacity;
	ShowTextLabel();
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateMoeCapacity(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_capacity);
}

void CTLiteDoc::OnMoeSpeedlimit()
{
	m_LinkMOEMode = MOE_speedlimit;
	ShowTextLabel();
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateMoeSpeedlimit(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_speedlimit);
}

void CTLiteDoc::OnMoeFreeflowtravletime()
{
	m_LinkMOEMode = MOE_fftt;
	ShowTextLabel();
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateMoeFreeflowtravletime(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_fftt);
}


CString CTLiteDoc::GetLocalFileName(CString strFullPath)
{
	int len = strFullPath.GetLength();
	/////////////////////////////////

	CString	strPathTitleName;

	strPathTitleName.Empty ();

	for(int k=len-1;k>=0;k--)
	{
		if(strFullPath[k]=='\\')
			break;

			strPathTitleName+=strFullPath[k];

	}

	strPathTitleName.MakeReverse();

	return strPathTitleName;

}

CString CTLiteDoc::GetWorkspaceTitleName(CString strFullPath)
{
	int len = strFullPath.GetLength();
	/////////////////////////////////

	CString	strPathTitleName;

	strPathTitleName.Empty ();
	bool StartFlag = false;

	for(int k=len-1;k>=0;k--)
	{
		if(strFullPath[k]=='\\')
			break;

		if(strFullPath[k]=='.' && StartFlag == false)
		{
			StartFlag = true;
			continue;
		}

		if(StartFlag == true)
			strPathTitleName+=strFullPath[k];

	}

	strPathTitleName.MakeReverse();

	return strPathTitleName;

}
void CTLiteDoc::OnEditDeleteselectedlink()
{

	// TODO: Add your command handler code here
}


void CTLiteDoc::OnMoeLength()
{
	m_LinkMOEMode = MOE_length;
	ShowTextLabel();
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateMoeLength(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_length);
}

void CTLiteDoc::OnEditSetdefaultlinkpropertiesfornewlinks()
{

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	CString DefaultDataFolder;

	DefaultDataFolder.Format ("%s\\default_data_folder\\",pMainFrame->m_CurrentDirectory);

	if(m_LinkSet.size()==0)
		ReadLinkTypeCSVFile(DefaultDataFolder+"input_link_type.csv");

	CDlgDefaultLinkProperties dlg;
	dlg.m_pDoc  = this;

	dlg.SpeedLimit = m_DefaultSpeedLimit ;
	dlg.LaneCapacity = m_DefaultCapacity ;
	dlg.NumLanes = m_DefaultNumLanes;
	dlg.LinkType = m_DefaultLinkType;

	dlg.m_StartNodeNumberForNewNodes = m_StartNodeNumberForNewNodes;
	if(dlg.DoModal() == IDOK)
	{
		m_DefaultSpeedLimit = dlg.SpeedLimit;
		m_DefaultCapacity = dlg.LaneCapacity;
		m_DefaultNumLanes = dlg.NumLanes;
		m_DefaultLinkType = dlg.LinkType;
		m_LongLatFlag = dlg.m_bLongLatSystem;
		if(m_LongLatFlag)
		{
			m_UnitMile = 0.01453788427;
			m_UnitFeet = m_UnitMile/5280.0;			
		}else
		{
			m_UnitMile = 1;
			m_UnitFeet = m_UnitMile/5280.0;			

		}
		m_StartNodeNumberForNewNodes = 	dlg.m_StartNodeNumberForNewNodes ;

	}
}

void CTLiteDoc::OnUpdateEditSetdefaultlinkpropertiesfornewlinks(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CTLiteDoc::OnToolsProjectfolder()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project directory has not been specified.");
		return;
	}

	ShellExecute( NULL,  "explore", m_ProjectDirectory, NULL,  NULL, SW_SHOWNORMAL );
}

void CTLiteDoc::OnToolsOpennextaprogramfolder()
{
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	ShellExecute( NULL,  "explore", pMainFrame->m_CurrentDirectory, NULL,  NULL, SW_SHOWNORMAL );
}

void CTLiteDoc::OnMoeNoodmoe()
{
	m_ODMOEMode = odnone;
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateMoeNoodmoe(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ODMOEMode == MOE_none);
}


void CTLiteDoc::OnOdtableImportOdTripFile()
{
	CString strFilter = "OD Trip File|*.txt";

	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter);
	//	dlg.m_ofn.nFilterIndex = m_nFilterLoad;
	HRESULT hResult = (int)dlg.DoModal();
	if (FAILED(hResult)) {
		return;
	}
	ReadTripTxtFile(dlg.GetPathName());
	OnToolsEditoddemandtable();
	m_bFitNetworkInitialized = false;
	UpdateAllViews(0);
}

void CTLiteDoc::OnToolsEditassignmentsettings()
{
	EditTrafficAssignmentOptions();
}

void CTLiteDoc::OnToolsEditoddemandtable()
{
	CDlgODDemandGridCtrl dlg;
	dlg.m_pDoc = this;
	dlg.DoModal();
}

void CTLiteDoc::OnSearchLinklist()
{

	g_bShowLinkList = !g_bShowLinkList;

	if(g_bShowLinkList)
	{
		if(g_pLinkListDlg==NULL)
		{
			g_pLinkListDlg = new CDlgLinkList();
			g_pLinkListDlg->Create(IDD_DIALOG_LINK_LIST);
		}

		// update using pointer to the active document; 

		if(g_pLinkListDlg->GetSafeHwnd())
		{
			// we udpate the pointer list for document every time we open this link list window
			std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
			while (iDoc != g_DocumentList.end())
			{
				if ((*iDoc)->m_NodeSet.size()>0)
				{

					if(g_pLinkListDlg->m_pDoc ==NULL)
						g_pLinkListDlg->m_pDoc = (*iDoc);
					else if((*iDoc)!= g_pLinkListDlg->m_pDoc)
					{
						g_pLinkListDlg->m_pDoc2 = (*iDoc);
						g_pLinkListDlg->m_bDoc2Ready = true;
					}

				}
				iDoc++;

			}

			g_pLinkListDlg->ReloadData ();
			g_pLinkListDlg->ShowWindow(SW_HIDE);
			g_pLinkListDlg->ShowWindow(SW_SHOW);
		}
	}else
	{
		if(g_pLinkListDlg!=NULL && g_pLinkListDlg->GetSafeHwnd())
		{
			g_pLinkListDlg->ShowWindow(SW_HIDE);
		}
	}

}

void CTLiteDoc::OnMoeVehicle()
{
	m_LinkMOEMode = MOE_vehicle;
	GenerateOffsetLinkBand();
	ShowLegend(false);

	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateMoeVehicle(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_vehicle);
}
LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

	if (retval == ERROR_SUCCESS) {
		long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];
		RegQueryValue(hkey, NULL, data, &datasize);
		lstrcpy(retdata,data);
		RegCloseKey(hkey);
	}

	return retval;
}
HINSTANCE g_OpenDocument(LPCTSTR url, int showcmd)
{
	TCHAR key[MAX_PATH + MAX_PATH];

	// First try ShellExecute()
	HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

	// If it failed, get the .htm regkey and lookup the program
	if ((UINT)result <= HINSTANCE_ERROR) {

		if (GetRegKey(HKEY_CLASSES_ROOT, _T(".csv"), key) == ERROR_SUCCESS) {
			lstrcat(key, _T("\\shell\\open\\command"));

			if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) {
				TCHAR *pos;
				pos = _tcsstr(key, _T("\"%1\""));
				if (pos == NULL) {			   // No quotes found
					pos = strstr(key, _T("%1"));	   // Check for %1, without quotes
					if (pos == NULL)			   // No parameter at all...
						pos = key+lstrlen(key)-1;
					else
						*pos = '\0';			 // Remove the parameter
				}
				else
					*pos = '\0';			  // Remove the parameter

				lstrcat(pos, _T(" "));
				lstrcat(pos, url);
				result = (HINSTANCE) WinExec(key,showcmd);
			}
		}
	}

	return result;
}

HINSTANCE g_GotoURL(LPCTSTR url, int showcmd)
{
	TCHAR key[MAX_PATH + MAX_PATH];

	// First try ShellExecute()
	HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, showcmd);

	// If it failed, get the .htm regkey and lookup the program
	if ((UINT)result <= HINSTANCE_ERROR) {

		if (GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) {
			lstrcat(key, _T("\\shell\\open\\command"));

			if (GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) {
				TCHAR *pos;
				pos = _tcsstr(key, _T("\"%1\""));
				if (pos == NULL) {			   // No quotes found
					pos = strstr(key, _T("%1"));	   // Check for %1, without quotes
					if (pos == NULL)			   // No parameter at all...
						pos = key+lstrlen(key)-1;
					else
						*pos = '\0';			 // Remove the parameter
				}
				else
					*pos = '\0';			  // Remove the parameter

				lstrcat(pos, _T(" "));
				lstrcat(pos, url);
				result = (HINSTANCE) WinExec(key,showcmd);
			}
		}
	}

	return result;
}

void CTLiteDoc::OnToolsViewsimulationsummary()
{
	CDlg_VehicleClassification* m_pDlg = new CDlg_VehicleClassification; 

	g_SummaryDialogVector.push_back (m_pDlg);  // collect memory block
	m_pDlg->m_pDoc = this;
	m_pDlg->SetModelessFlag(true); // voila! this is all it takes to make your dlg modeless!
	m_pDlg->Create(IDD_DIALOG_Summary); 
	m_pDlg->ShowWindow(SW_SHOW); 

	m_bSummaryDialog = true;
}


void CTLiteDoc::OnToolsViewassignmentsummarylog()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_summary.csv");
}

void CTLiteDoc::OnHelpVisitdevelopmentwebsite()
{
	g_OpenDocument("https://github.com/xzhou99/dtalite_software_release", SW_SHOW);
}

bool CTLiteDoc::CheckControlData()
{
	return true;
	std::vector <int> NodeVector;
	std::list<DTANode*>::iterator iNode;
	for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{

		if((*iNode)->m_ControlType == m_ControlType_PretimedSignal || (*iNode)->m_ControlType == m_ControlType_ActuatedSignal)
		{

			if((*iNode)->m_CycleLengthInSecond ==0)
			{
				NodeVector.push_back ((*iNode)->m_NodeNumber);
			}

		}
	}

	if(NodeVector.size()>0 && m_signal_reresentation_model >=1)
	{

		CString message;
		message.Format ("There are %d signalized nodes with zero cycle length (e.g. %d).\n Do you want to continue?\nDTALite will use a continuous flow model with link capacity constraints for those nodes.", NodeVector.size(),NodeVector[0]);

		if(AfxMessageBox(message,MB_YESNO|MB_ICONINFORMATION)==IDYES)
		{

			return true;
		}


		return false;
	}

	return true;

}
bool CTLiteDoc::EditTrafficAssignmentOptions()
{

	CDlgAssignmentSettings dlg;
	dlg.m_pDoc = this;

	ReadDemandTypeCSVFile(m_ProjectDirectory+"input_demand_type.csv");
	if(ReadMetaDemandCSVFile(m_ProjectDirectory+"input_demand_file_list.csv") == false)
		return false;

	ReadScenarioSettingCSVFile(m_ProjectDirectory+"input_scenario_settings.csv");

	if(dlg.DoModal() ==IDOK)
	{
		if(CheckControlData()==false)
		{

			return false;
		}

		return true;
	}else
	{ 
		return false;
	}
}

void CTLiteDoc::OnToolsRuntrafficassignment()
{

	if(EditTrafficAssignmentOptions() == true)
		OnToolsPerformtrafficassignment();

}

void CTLiteDoc::OnImportodtripfile3columnformat()
{
	CDlg_ImportODDemand dlg;
	if(dlg.DoModal ()==IDOK)
	{
		CString strFilter = "OD 3-column Trip File|*.txt";
		CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter);
		//	dlg.m_ofn.nFilterIndex = m_nFilterLoad;
		HRESULT hResult = (int)dlg.DoModal();
		if (FAILED(hResult)) {
			return;
		}
		ReadTripTxtFile(dlg.GetPathName());
		OnToolsEditoddemandtable();
		m_bFitNetworkInitialized = false;
		UpdateAllViews(0);	

	}

}

void CTLiteDoc::OnToolsPerformscheduling()
{
	STARTUPINFO si = { 0 };  
	PROCESS_INFORMATION pi = { 0 };  

	si.cb = sizeof(si); 

	CWaitCursor curs;
	if(!SetCurrentDirectory(m_ProjectDirectory))
	{
		if(m_ProjectDirectory.GetLength()==0)
		{
			AfxMessageBox("The project directory has not been specified.");
			return;
		}
		CString msg;
		msg.Format ("The specified project directory %s does not exist.", m_ProjectDirectory);
		AfxMessageBox(msg);
		return;
	}

	CString sCommand;
	CString strParam;
	CTime ExeStartTime = CTime::GetCurrentTime();

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

	sCommand.Format("%s\\FastTrain.exe", pMainFrame->m_CurrentDirectory);

	ProcessExecute(sCommand, strParam, m_ProjectDirectory, true);

	CTime ExeEndTime = CTime::GetCurrentTime();

	CTimeSpan ts = ExeEndTime  - ExeStartTime;
	CString str_running_time;

	FILE* st = NULL;

	CString directory = m_ProjectDirectory;
	char simulation_short_summary1[200];
	char simulation_short_summary2[200];
	char simulation_short_summary3[200];

	fopen_s(&st,directory+"short_summary.csv","r");
	if(st!=NULL)
	{  
		fgets (simulation_short_summary1, 200 , st);
		fgets (simulation_short_summary2 , 200 , st);
		fgets (simulation_short_summary3, 200 , st);
		fclose(st);
	}

	str_running_time.Format ("Program execution has completed.\nSimulation Statistics: %s\n%s\n%sProgram execution time: %d hour(s) %d min(s) %d sec(s) \nDo you want to load the output now?",
		simulation_short_summary1,simulation_short_summary2, simulation_short_summary3,ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());

	if( AfxMessageBox(str_running_time, MB_YESNO| MB_ICONINFORMATION)==IDYES)
	{
		LoadSimulationOutput();
		UpdateAllViews(0);
	}
}


void CTLiteDoc::ResetBackgroundImageCoordinate()
{
	m_LongLatFlag = true;

	m_UnitMile = 0.01453788427;
	m_UnitFeet = m_UnitMile/5280.0;			

	m_UnitFeet = m_UnitMile/5280.0;

	float m_XScale = 1;
	float m_YScale = 1;

	if(fabs(m_PointB_x- m_PointA_x)>0.000001)
	{
		m_XScale = (m_PointB_long - m_PointA_long)/(m_PointB_x- m_PointA_x);
	}else
	{

		AfxMessageBox("Horizontal distance between two points is too short. Please redefine two points.");
		return;
	}

	if(fabs(m_PointB_y- m_PointA_y)>0.000001)
	{
		m_YScale = (m_PointB_lat - m_PointA_lat)/(m_PointB_y- m_PointA_y);
	}else
	{

		AfxMessageBox("Vertical distance between two points is too short. Please redefine two points.");
		return;
	}

	float m_XOrigin = m_PointA_x - m_PointA_long /m_XScale;

	float m_YOrigin = m_PointA_y- m_PointA_lat /m_YScale;


	/////////////////////////////////adjustment


	// adjust node coordinates
	std::list<DTANode*>::iterator iNode;
	for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{

		(*iNode)->pt .x  = ((*iNode)->pt .x - m_XOrigin)*m_XScale;
		(*iNode)->pt .y  = ((*iNode)->pt .y - m_YOrigin)*m_YScale;
	}
	//adjust link cooridnates

	std::list<DTALink*>::iterator iLink;

	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->m_FromPoint.x = ((*iLink)->m_FromPoint.x -m_XOrigin)*m_XScale;
		(*iLink)->m_FromPoint.y = ((*iLink)->m_FromPoint.y -m_YOrigin)*m_YScale;

		(*iLink)->m_ToPoint.x = ((*iLink)->m_ToPoint.x -m_XOrigin)*m_XScale;
		(*iLink)->m_ToPoint.y = ((*iLink)->m_ToPoint.y -m_YOrigin)*m_YScale;


		for(unsigned int si = 0; si< (*iLink)->m_Original_ShapePoints.size(); si++)
		{

			(*iLink)->m_Original_ShapePoints[si].x = ((*iLink)->m_Original_ShapePoints[si].x - m_XOrigin)*m_XScale;
			(*iLink)->m_Original_ShapePoints[si].y = ((*iLink)->m_Original_ShapePoints[si].y - m_YOrigin)*m_YScale;

		}

		for(unsigned int si = 0; si< (*iLink)->m_ShapePoints.size(); si++)
		{

			(*iLink)->m_ShapePoints[si].x = ((*iLink)->m_ShapePoints[si].x - m_XOrigin)*m_XScale;
			(*iLink)->m_ShapePoints[si].y = ((*iLink)->m_ShapePoints[si].y - m_YOrigin)*m_YScale;

		}

	}

	// zone layer
	std::map<int, DTAZone>	:: iterator itr;

	for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); ++itr)
	{
		for(unsigned int si = 0; si< itr->second.m_ShapePoints.size(); si++)
		{
			itr->second.m_ShapePoints[si].x = (itr->second.m_ShapePoints[si].x - m_XOrigin)*m_XScale;
			itr->second.m_ShapePoints[si].y = (itr->second.m_ShapePoints[si].y - m_YOrigin)*m_YScale;

		}

	}

	// image layer

	m_ImageX1 = (m_ImageX1- m_XOrigin)*m_XScale;
	m_ImageY1 = (m_ImageY1- m_YOrigin)*m_YScale;
	m_ImageX2 = (m_ImageX2- m_XOrigin)*m_XScale;
	m_ImageY2 =  (m_ImageY2- m_YOrigin)*m_YScale;

	m_ImageWidth = fabs(m_ImageX2 - m_ImageX1);
	m_ImageHeight = fabs(m_ImageY2 - m_ImageY1);

	GDPoint pt1, pt2;
	pt1.x = m_ImageX1;
	pt1.y = m_ImageY1;

	pt2.x = m_ImageX2;
	pt2.y = m_ImageY2;

	// update image width in miles
	m_ImageWidthInMile = max(m_ImageWidthInMile,g_CalculateP2PDistanceInMileFromLatitudeLongitude(pt1,pt2));

	m_ImageMoveSize = m_ImageWidth/2000.0f;

	CString str_result;
	str_result.Format ("The coordinates has been adjusted to long/lat format.\nNEXTA will reload this project to reflect changed coordinates.");
	if(AfxMessageBox(str_result, MB_ICONINFORMATION)==IDOK)
	{
		SaveProject(m_ProjectFile);  // save time-dependent MOE to input_link MOE file

		OnOpenDocument(m_ProjectFile);
	}

	m_bFitNetworkInitialized = false;
	CalculateDrawingRectangle(false);
	UpdateAllViews(0);
}

void CTLiteDoc::OnFileChangecoordinatestolong()
{
	m_LongLatFlag = true;
	CDlgNetworkAlignment  dlg;
	if(dlg.DoModal() ==IDOK)
	{
		if(dlg.m_Node1!=dlg.m_Node2 && m_NodeNumbertoNodeNoMap.find(dlg.m_Node1)!= m_NodeNumbertoNodeNoMap.end() && 
			m_NodeNumbertoNodeNoMap.find(dlg.m_Node2)!= m_NodeNumbertoNodeNoMap.end())
		{

			GDPoint m_Node1OrgPt, m_Node2OrgPt;
			m_Node1OrgPt = m_NodeNoMap[m_NodeNumbertoNodeNoMap[dlg.m_Node1]]->pt;
			m_Node2OrgPt = m_NodeNoMap[m_NodeNumbertoNodeNoMap[dlg.m_Node2]]->pt;

			float m_XScale = 1;
			float m_YScale = 1;

			if(m_Node1OrgPt.x - m_Node2OrgPt.x)
			{
				m_XScale = (dlg.m_NodeX1 - dlg.m_NodeX2)/(m_Node1OrgPt.x - m_Node2OrgPt.x);
			}

			if(m_Node1OrgPt.y - m_Node2OrgPt.y)
			{
				m_YScale = (dlg.m_NodeY1 - dlg.m_NodeY2)/(m_Node1OrgPt.y - m_Node2OrgPt.y);
			}

			float m_XOrigin = m_Node1OrgPt.x - dlg.m_NodeX1 /m_XScale;

			float m_YOrigin = m_Node1OrgPt.y - dlg.m_NodeY1 /m_YScale;

			// adjust node coordinates
			std::list<DTANode*>::iterator iNode;
			for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
			{

				(*iNode)->pt .x  = ((*iNode)->pt .x - m_XOrigin)*m_XScale;
				(*iNode)->pt .y  = ((*iNode)->pt .y - m_YOrigin)*m_YScale;
			}
			//adjust link cooridnates

			std::list<DTALink*>::iterator iLink;

			for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			{
				(*iLink)->m_FromPoint.x = ((*iLink)->m_FromPoint.x -m_XOrigin)*m_XScale;
				(*iLink)->m_FromPoint.y = ((*iLink)->m_FromPoint.y -m_YOrigin)*m_YScale;

				(*iLink)->m_ToPoint.x = ((*iLink)->m_ToPoint.x -m_XOrigin)*m_XScale;
				(*iLink)->m_ToPoint.y = ((*iLink)->m_ToPoint.y -m_YOrigin)*m_YScale;


				for(unsigned int si = 0; si< (*iLink)->m_Original_ShapePoints.size(); si++)
				{

					(*iLink)->m_Original_ShapePoints[si].x = ((*iLink)->m_Original_ShapePoints[si].x - m_XOrigin)*m_XScale;
					(*iLink)->m_Original_ShapePoints[si].y = ((*iLink)->m_Original_ShapePoints[si].y - m_YOrigin)*m_YScale;

				}

				for(unsigned int si = 0; si< (*iLink)->m_ShapePoints.size(); si++)
				{

					(*iLink)->m_ShapePoints[si].x = ((*iLink)->m_ShapePoints[si].x - m_XOrigin)*m_XScale;
					(*iLink)->m_ShapePoints[si].y = ((*iLink)->m_ShapePoints[si].y - m_YOrigin)*m_YScale;

				}

			}

			// zone layer
			std::map<int, DTAZone>	:: iterator itr;

			for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); ++itr)
			{
				for(unsigned int si = 0; si< itr->second.m_ShapePoints.size(); si++)
				{
					itr->second.m_ShapePoints[si].x = (itr->second.m_ShapePoints[si].x - m_XOrigin)*m_XScale;
					itr->second.m_ShapePoints[si].y = (itr->second.m_ShapePoints[si].y - m_YOrigin)*m_YScale;

				}

			}

			// image layer

			m_ImageX1 = (m_ImageX1- m_XOrigin)*m_XScale;
			m_ImageY1 = (m_ImageY1- m_YOrigin)*m_YScale;
			m_ImageX2 = (m_ImageX2- m_XOrigin)*m_XScale;
			m_ImageY2 =  (m_ImageY2- m_YOrigin)*m_YScale;

			m_ImageWidth = fabs(m_ImageX2 - m_ImageX1);
			m_ImageHeight = fabs(m_ImageY2 - m_ImageY1);

			m_ImageMoveSize = m_ImageWidth/2000.0f;

			CString str_result;
			str_result.Format ("The coordinates of %d nodes, %d links and %d zones have been adjusted to long/lat format.\nPleaes save the network to confirm the change.\nYou can use NEXTA_32.exe ->menu->Tools->GIS tools->Export GIS shape files to check the changed network on Google Maps",m_NodeSet.size(),m_LinkSet.size(),m_ZoneMap.size());
			AfxMessageBox(str_result, MB_ICONINFORMATION);
		}


		UpdateAllViews(0);
	}
}

void CTLiteDoc::OpenWarningLogFile(CString directory)
{
	m_NEXTALOGFile.open (directory+"NeXTA.log", ios::out);
	if (m_NEXTALOGFile.is_open())
	{
		m_NEXTALOGFile.width(12);
		m_NEXTALOGFile.precision(3) ;
		m_NEXTALOGFile.setf(ios::fixed);
	}else
	{
		AfxMessageBox("File NeXTA.log cannot be opened, and it might be locked by another program!");
	}
}
void CTLiteDoc::OnToolsExportopmodedistribution()
{

}

void CTLiteDoc::OnToolsEnumeratepath()
{

	CWaitCursor cws;

	int OD_index;

	int O_array[10]={53661,101846,165091,226988,209476,41017,54466,94991,78110,16589};

	int D_array[10]={144304,33737,70979,72725,79930,101989,89676,21233,84885,156041};

	float TravelTime_array[10]={132.3,112.87,65,92.12,124.03,147.83,152.58,82.9,54.63,65.49};

	for(OD_index = 0; OD_index<10; OD_index++)
	{
		m_OriginNodeID = m_NodeNumbertoNodeNoMap[O_array[OD_index]];
		m_DestinationNodeID = m_NodeNumbertoNodeNoMap[D_array[OD_index]];
		float TravelTimeBound  = TravelTime_array[OD_index];
		if(m_OriginNodeID>0 && m_DestinationNodeID>0)
		{
			if(m_pNetwork !=NULL)
			{
				delete m_pNetwork;
				m_pNetwork = NULL;
			}

			m_pNetwork = new DTANetworkForSP(m_NodeSet.size(), m_LinkSet.size(), 1, 1, m_AdjLinkSize);  //  network instance for single processor in multi-thread environment

			m_pNetwork->BuildPhysicalNetwork(&m_NodeSet, &m_LinkSet, m_RandomRoutingCoefficient, false);

			m_pNetwork->GenerateSearchTree (m_OriginNodeID,m_DestinationNodeID,m_NodeSet.size()*5,TravelTimeBound);

			FILE* st = NULL;

			//		CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			//			_T("Path file (*.csv)|*.csv|"));
			//		if(dlg.DoModal() == IDOK)
			//		{
			CString str;
			str.Format ("C:\\path_set_%d.csv",OD_index);
			fopen_s(&st,str,"w");

			int NodeList[1000];

			int PathNo = 0;

			int i;
			for(i = 0; i < m_pNetwork->m_TreeListTail; i++)
			{
				if(m_pNetwork->m_SearchTreeList[i].CurrentNode == m_DestinationNodeID)
				{
					int nodeindex = 0;
					NodeList[nodeindex++] = m_pNetwork->m_SearchTreeList[i].CurrentNode;
					int Pred = m_pNetwork->m_SearchTreeList[i].PredecessorNode ;

					while(Pred!=0)
					{
						NodeList[nodeindex++] = m_pNetwork->m_SearchTreeList[Pred].CurrentNode;

						Pred = m_pNetwork->m_SearchTreeList[Pred].PredecessorNode ;
					}
					NodeList[nodeindex++] = m_pNetwork->m_SearchTreeList[Pred].CurrentNode;

					fprintf(st,"%d,%d,", PathNo,nodeindex);

					for(int n = nodeindex-1; n>=0; n--)
					{
						fprintf(st,"%d,", m_NodeNotoNumberMap[NodeList[n]]);
					}

					fprintf(st, "\n");
					PathNo++;
				}

			}
			fclose(st);
		}
	}

}

void CTLiteDoc::OnResearchtoolsExporttodtalitesensordataformat()
{
	CWaitCursor wc;
	int max_day = 23;
	FILE* st = NULL;

	for(int day = 0; day < max_day; day++)
	{
		CString str;
		str.Format( "SensorDataDay%03d.csv",day+1);
		fopen_s(&st,str,"w");
		if(st!=NULL)
		{
			std::list<DTALink*>::iterator iLink;

			fprintf(st,"Unix Timestamp (local time),StationID,Total Flow_per_obs_interval,Avg Density,Avg Speed\n");
			for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			{
				if((*iLink)->m_bSensorData )
				{

					for(int t= day*1440; t<(day+1)*1440; t+=5)
					{
						//
						int hour = (t-day*1440)/60;
						int min =  (t-day*1440-hour*60);
						fprintf(st,"07/%02d/2010 %02d:%02d, %d %4.1f, 0, %4.1f\n", day, hour, min, (*iLink)->m_LinkNo+1,
							(*iLink)->m_LinkMOEAry[ t].LinkFlow/12, (*iLink)->m_LinkMOEAry[t].Speed);
					}


				}
			}

			fclose(st);

		}

	}


}
void CTLiteDoc::OnScenarioConfiguration()
{
	// TODO: Add your command handler code here

	if (m_ProjectDirectory.GetLength() == 0 ) 
	{
		MessageBox(NULL,"No open project!",NULL,MB_ICONWARNING);
		return;
	}

	CDlgScenario dlg;
	dlg.m_pDoc = this;
	dlg.DoModal();
}

void CTLiteDoc::OnMoeViewmoes()
{
	// TODO: Add your command handler code here

	CDlgMOETabView dlg;
	dlg.m_pDoc = this;
	dlg.DoModal();
}

void CTLiteDoc::OnImportdataImportExcelFile()
{

	CDlg_ImportNetwork dlg;

	dlg.m_pDoc = this;
	if(dlg.DoModal() == IDOK)
	{
		OffsetLink();
		GenerateOffsetLinkBand();
		CalculateDrawingRectangle();
		m_bFitNetworkInitialized  = false;
		UpdateAllViews(0);
	
		m_bSaveProjectFromImporting = true;
	}

}

void CTLiteDoc::OnImportdataImport()
{

	CDlg_Information dlg_info;
	dlg_info.m_StringInfo = "This function imports node/link/zone GIS shape files through a CSV configuration file.\r\nThis function requires 32-bit NEXTA.";
	dlg_info.m_SampleFileDirectory = "importing_sample_data_sets\\GIS_files";
	dlg_info.m_OnLineDocumentLink = "https://docs.google.com/document/d/1Ud2FN1utnVrIs4je9CHveAykH8h09x0Ln6_qdXE8F6Y/edit#heading=h.24j41bpvha3d";

	if(dlg_info.DoModal() == IDOK)
	{

	CDlg_ImportNetwork dlg;
		dlg.m_pDoc = this;
		if(dlg.DoModal() == IDOK)
		{
			OffsetLink();
			GenerateOffsetLinkBand();
			CalculateDrawingRectangle();
			m_bFitNetworkInitialized  = false;
			UpdateAllViews(0);
			m_bSaveProjectFromImporting = true;

			if(m_ProjectDirectory.GetLength()==0 && m_NodeSet.size() > 0)  //with newly imported nodes
			{
				if(AfxMessageBox("Do you want to save the imported project now?",MB_YESNO|MB_ICONINFORMATION)==IDYES)
				{
					OnFileSaveProjectAs();
				}
				return;
			}

		}
	}
}

float CTLiteDoc::FillODMatrixFromCSVFile(LPCTSTR lpszFileName)
{

	float total_demand = 0;

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString (lpszFileName);

	// construct a std::string using the LPCSTR input
	std::string strStd (pszConvertedAnsiString);


	CCSVParser parser_ODMatrix;

	if (parser_ODMatrix.OpenCSVFile(strStd))
	{
		while(parser_ODMatrix.ReadRecord())
		{

			int origin_zone_id, destination_zone_id;
			float number_of_vehicles;
			float starting_time_in_min = 0;
			float ending_time_in_min = 1440;

			if(parser_ODMatrix.GetValueByFieldName("from_zone_id",origin_zone_id) == false)
			{
				AfxMessageBox("Field from_zone_id cannot be found in the demand csv file.");
				return false;
			}

			if(parser_ODMatrix.GetValueByFieldName("to_zone_id",destination_zone_id) == false)
			{
				AfxMessageBox("Field to_zone_id cannot be found in the demand csv file.");
				return false;
			}

			DTADemand element;
			element.from_zone_id = origin_zone_id;
			element.to_zone_id = destination_zone_id;
			element.starting_time_in_min = 0;
			element.ending_time_in_min = 60;
			for(unsigned int type = 0; type < m_DemandTypeVector.size(); type++)
			{

				std::ostringstream  demand_string;
				demand_string << "number_of_trips_demand_type" << type+1;

				if(parser_ODMatrix.GetValueByFieldName(demand_string.str(),number_of_vehicles) == false)
				{
					CString str_number_of_vehicles_warning; 
					str_number_of_vehicles_warning.Format("Field demand_value_type%d cannot be found in the demand csv file.", type);
					AfxMessageBox(str_number_of_vehicles_warning);
					return false;
				}
				m_ZoneMap[origin_zone_id].m_ODDemandMatrix [destination_zone_id].SetValue (type, number_of_vehicles);
				element.number_of_vehicles_per_demand_type .push_back (number_of_vehicles);
				total_demand+= number_of_vehicles;

			}


		}
	}
	return total_demand;
}


bool CTLiteDoc::ImportSensorData()
{

	return true;

}
void CTLiteDoc::AdjustCoordinateUnitToMile()
{

	m_AdjLinkSize +=2;  // add two more elements to be safe

	std::list<DTANode*>::iterator iNode;
	//adjust XY coordinates if the corrdinate system is not consistenty
	if(fabs(m_UnitMile-1.00)>0.10)  // ask users if we need to adjust the XY coordinates
	{
		if(AfxMessageBox("The link length information in link.csv is not consistent with the X/Y coordinates in node.csv.\n Do you want to adjust the the X/Y coordinate unit to mile in node.csv?", MB_YESNO) == IDYES)

			for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
			{
				(*iNode)->pt.x = ((*iNode)->pt.x - min(m_NetworkRect.left,m_NetworkRect.right))*m_UnitMile;
				(*iNode)->pt.y = ((*iNode)->pt.y - min(m_NetworkRect.top, m_NetworkRect.bottom))*m_UnitMile;
			}

			std::list<DTALink*>::iterator iLink;
			for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			{
				(*iLink)->m_FromPoint = m_NodeNoMap[(*iLink)->m_FromNodeID]->pt;
				(*iLink)->m_ToPoint = m_NodeNoMap[(*iLink)->m_ToNodeID]->pt;
			}

			m_UnitMile  = 1.0;
			m_UnitFeet = 1/5280.0;

			if(m_LongLatCoordinateFlag)
				m_UnitFeet = m_UnitMile/62/5280.0f;  // 62 is 1 long = 62 miles

			CalculateDrawingRectangle();
			UpdateAllViews(0);

	}
}



bool CTLiteDoc::ReadZoneShapeCSVFile(LPCTSTR lpszFileName)
{
	CString information_msg;
	FILE* st = NULL;
	fopen_s(&st,lpszFileName,"r");

	int zone_shape_point_count = 0;
	int id;
	if(st!=NULL)
	{
		while(!feof(st))
		{
			id			= g_read_integer(st); 
			if(id == -1)  // reach end of file
				break;

			int part_no			= g_read_integer(st);
			int point_id		= g_read_integer(st);

			DTAZone zone = m_ZoneMap[id];

			float x	= g_read_float(st);
			float y	= g_read_float(st);

			GDPoint	pt;
			pt.x = x;
			pt.y = y;

			zone.m_ShapePoints .push_back (pt);
			zone_shape_point_count ++;
			/*
			}else
			{
			CString msg;
			msg.Format ("zone %d in the zone shape csv file has not defined in the input node table.",id);
			AfxMessageBox(msg);
			fclose(st);
			return false;

			}
			*/
		}
		fclose(st);

		return true;
	}

	information_msg.Format ("%d nodes are loaded from the node csv file.",zone_shape_point_count,lpszFileName);

	return true;

}

bool CTLiteDoc::ReadNodeGeoFile(LPCTSTR lpszFileName)
{
	return ReadZoneShapeCSVFile(lpszFileName);

}


bool CTLiteDoc::ReadLinkGeoFile(LPCTSTR lpszFileName)
{
	CString information_msg;
	FILE* st = NULL;
	fopen_s(&st,lpszFileName,"r");

	int link_shape_point_count = 0;

	int id = 0;
	if(st!=NULL)
	{
		while(!feof(st))
		{
			id			= g_read_integer(st);
			if(id == -1)  // reach end of file
				break;

			int number_of_feature_points			= g_read_integer(st);

			DTALink* pLink = FindLinkWithLinkNo (id);
			if(pLink!=NULL)
			{
				for(int i=0; i< number_of_feature_points; i++)
				{
					float x	= g_read_float(st);
					float y	= g_read_float(st);
					GDPoint	pt;
					pt.x = x;
					pt.y = y;

					pLink->m_ShapePoints .push_back (pt);
					link_shape_point_count ++;
				}

			}else
			{
				CString msg;
				msg.Format  ("link %d in the link shape csv file has not defined in the input shape table.",id);
				AfxMessageBox(msg,MB_ICONINFORMATION);
				fclose(st);
				return false;

			}
		}
		fclose(st);

	}

	information_msg.Format ("%d shape points are loaded from the link shape csv file.",link_shape_point_count,lpszFileName);

	std::list<DTALink*>::iterator iLink;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		if( (*iLink)->m_ShapePoints .size() ==0)  // no shape points from external sources, use positoins from upstream and downstream nodes
		{
			(*iLink)->m_ShapePoints.push_back((*iLink)->m_FromPoint);
			(*iLink)->m_ShapePoints.push_back((*iLink)->m_ToPoint);

		}
	}


	return true;
}

void CTLiteDoc::OnMoeVehiclepathanalaysis()
{
	if(g_bValidDocumentChanged && g_pVehiclePathDlg!=NULL)  // either a new document is added or an old document is delete. 
	{
		//	delete g_pVehiclePathDlg; potential memory leak, but operator delete is problematic for gridlist
		g_pVehiclePathDlg= NULL;
		g_bValidDocumentChanged = false;
	}

	g_bShowVehiclePathDialog = !g_bShowVehiclePathDialog;

	if(g_bShowVehiclePathDialog)
	{
		if(g_pVehiclePathDlg==NULL)
		{
			g_pVehiclePathDlg = new CDlg_VehPathAnalysis();
			g_pVehiclePathDlg->m_pDoc = this;
			g_pVehiclePathDlg->Create(IDD_DIALOG_VEHICLE_PATH);
		}

		// update using pointer to the active document; 

		if(g_pVehiclePathDlg->GetSafeHwnd())
		{
			// we udpate the pointer list for document every time we open this link list window
			g_pVehiclePathDlg->m_pDoc = this;
			g_pVehiclePathDlg->ShowWindow(SW_HIDE);
			g_pVehiclePathDlg->ShowWindow(SW_SHOW);
			g_pVehiclePathDlg->FilterOriginDestinationPairs();
		}
	}else
	{
		if(g_pVehiclePathDlg!=NULL && g_pVehiclePathDlg->GetSafeHwnd())
		{
			g_pVehiclePathDlg->ShowWindow(SW_HIDE);
		}
	}

}

void CTLiteDoc::HighlightPath(	std::vector<int>	m_LinkVector, int DisplayID = 1)
{
	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->m_DisplayLinkID = -1;
	}

	for (unsigned int l = 0; l < m_LinkVector.size(); l++)
	{
		DTALink* pLink = FindLinkWithLinkNo (m_LinkVector[l]);
		if(pLink!=NULL)
		{
			pLink->m_DisplayLinkID = DisplayID;
		}

	}
	UpdateAllViews(0);
}



void CTLiteDoc::HighlightSelectedVehicles(bool bSelectionFlag)
{
	CWaitCursor wc;

	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->m_DisplayLinkID = -1;
		(*iLink)->m_NumberOfMarkedVehicles++;
	}

	if(bSelectionFlag) // selection
	{
		std::list<DTAVehicle*>::iterator iVehicle;

		bool bTraceFlag = true;

		for (iVehicle = m_VehicleSet.begin(); iVehicle != m_VehicleSet.end(); iVehicle++)
		{
			DTAVehicle* pVehicle = (*iVehicle);
			if(pVehicle->m_bMarked)
			{
				for(int link= 1; link<pVehicle->m_NodeSize; link++)
				{
					if(m_LinkNoMap.find(pVehicle->m_NodeAry[link].LinkNo) != m_LinkNoMap.end())
					{
						m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo ]->m_DisplayLinkID = 1;
						m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo ]->m_NumberOfMarkedVehicles++;
					}
				}
			}		
		}
	} // de-selection: do nothing
	UpdateAllViews(0);

}

bool CTLiteDoc::ReadInputEmissionRateFile(LPCTSTR lpszFileName)
{
	CCSVParser parser_emission;
	if (parser_emission.OpenCSVFile(lpszFileName))
	{

		int line_no = 1;
		while(parser_emission.ReadRecord())
		{
			int vehicle_type;
			int opModeID;

			if(parser_emission.GetValueByFieldName("vehicle_type",vehicle_type) == false)
				break;
			if(parser_emission.GetValueByFieldName("opModeID",opModeID) == false)
				break;

			CEmissionRate element;
			if(parser_emission.GetValueByFieldName("meanBaseRate_TotalEnergy_(J/hr)",element.meanBaseRate_TotalEnergy) == false)
				break;
			if(parser_emission.GetValueByFieldName("meanBaseRate_CO2_(g/hr)",element.meanBaseRate_CO2) == false)
				break;
			if(parser_emission.GetValueByFieldName("meanBaseRate_NOX_(g/hr)",element.meanBaseRate_NOX) == false)
				break;
			if(parser_emission.GetValueByFieldName("meanBaseRate_CO_(g/hr)",element.meanBaseRate_CO) == false)
				break;
			if(parser_emission.GetValueByFieldName("meanBaseRate_HC_(g/hr)",element.meanBaseRate_HC) == false)
				break;

			ASSERT(vehicle_type < MAX_VEHICLE_TYPE_SIZE);
			ASSERT(opModeID < _MAXIMUM_OPERATING_MODE_SIZE);
			EmissionRateData[vehicle_type][opModeID] = element;
			line_no++;
		}
		m_AMSLogFile << "Read " << line_no << " emission rate elements from file "  << lpszFileName << endl; 

		CString msg;
		msg.Format("Imported %d emission rate elements from file %s",line_no,lpszFileName);
		m_MessageStringVector.push_back (msg);

		if(line_no>=1)
			return true;
		else
			return false;
	}
	CString msg;
	msg.Format("Imported 0 emission rate element from file %s",lpszFileName);
	m_MessageStringVector.push_back (msg);

	return false;

}




void CTLiteDoc::OnFileImportDemandFromCsv()
{
	static char BASED_CODE szFilter[] = "CSV (*.csv)|*.csv||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal() == IDOK)
	{
		FillODMatrixFromCSVFile(dlg.GetPathName ());
	}

	CalculateDrawingRectangle();
	m_bFitNetworkInitialized  = false;
	UpdateAllViews(0);
}


void CTLiteDoc::OnImportSensorData()
{
}

void CTLiteDoc::OnImportLinkmoe()
{
	static char BASED_CODE szFilter[] = "Simulated Link MOE CSV (*.csv)|*.csv||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal() == IDOK)
	{
		ReadSimulationLinkMOEData(dlg.GetPathName ());
	}

	CalculateDrawingRectangle();
	m_bFitNetworkInitialized  = false;
	UpdateAllViews(0);
}

void CTLiteDoc::OnImportVehiclefile()
{
	static char BASED_CODE szFilter[] = "Simulated Vehicle Trajectory CSV (*.csv)|*.csv||";
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter);
	if(dlg.DoModal() == IDOK)
	{
		ReadVehicleCSVFile_Parser(dlg.GetPathName ());
	}

	CalculateDrawingRectangle();
	m_bFitNetworkInitialized  = false;
	UpdateAllViews(0);
}

void CTLiteDoc::OnLinkmoeEmissions()
{

	m_LinkMOEMode = MOE_emissions;
	GenerateOffsetLinkBand();
	ShowLegend(true);

	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateLinkmoeEmissions(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_emissions);

}

void CTLiteDoc::OnLinkmoeReliability()
{
	// TODO: Add your command handler code here
}

void CTLiteDoc::OnUpdateLinkmoeReliability(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CTLiteDoc::OnLinkUserDefinedMOE()
{

	CDlg_TDMOE_Configuration* m_pDlg = new CDlg_TDMOE_Configuration;
	m_pDlg->m_pDoc = this;

	m_pDlg->SetModelessFlag(true); // voila! this is all it takes to make your dlg modeless!
	m_pDlg->Create(IDD_DIALOG_CONFIG_TDMOE); 
	m_pDlg->ShowWindow(SW_SHOW); 

	m_LinkBandWidthMode = LBW_number_of_lanes;
	m_LinkMOEMode = MOE_user_defined;

	//	ShowLegend(true);
}

void CTLiteDoc::OnUpdateUserDefinedMOE(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_user_defined);
}

void CTLiteDoc::OnImportAgentFile()
{
	// TODO: Add your command handler code here
}

void CTLiteDoc::OnImportNgsimFile()
{
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	pMainFrame->OnShowTimetable();

}

void CTLiteDoc::OpenCSVFileInExcel(CString filename)
{
	/*
	*/

	//	CString on_line_address;
	//	on_line_address.Format ("http://www.google.com/fusiontables/DataSource?dsrcid=%s",m_LinkTableID);

	if(filename.Find ("csv") >=0)
	{
		HINSTANCE result = ShellExecute(NULL, _T("open"), filename, NULL,NULL, SW_SHOW);
	}else
	{
		CXLEzAutomation XL;
		//Close Excel if failed to open file 
		if(!XL.OpenExcelFile(filename,1))
		{
			XL.ReleaseExcel();

			return;
		}

	}



}

bool CTLiteDoc::WriteSubareaFiles()
{
	// update m_ProjectDirectory
	FILE* st = NULL;
	//	cout << "Reading file node.csv..."<< endl;

	CString directory;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

	fopen_s(&st,directory+"optional_subarea_node.csv","w");
	if(st!=NULL)
	{

		fprintf(st, "name,node_id,control_type,geometry\n");
		for (unsigned i = 0; i < m_SubareaNodeSet.size(); i++)
		{
			DTANode* pNode = m_SubareaNodeSet[i];
			fprintf(st, "%s,%d,%d,\"<Point><coordinates>%f,%f</coordinates></Point>\"\n", pNode->m_Name.c_str (), pNode->m_NodeNumber , pNode->m_ControlType, pNode->pt .x, pNode->pt .y );
		}

		fclose(st);
	}else
	{
		AfxMessageBox("Error: File optional_subarea_node.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}

	fopen_s(&st,directory+"optional_subarea_link.csv","w");
	if(st!=NULL)
	{
		std::list<DTALink*>::iterator iLink;
		for (iLink = m_SubareaLinkSet.begin(); iLink != m_SubareaLinkSet.end(); iLink++)
		{
			if((*iLink)->m_AVISensorFlag == false)
			{

				(*iLink)->m_ReliabilityIndex  = (*iLink)->m_Length * (*iLink)->m_NumberOfLanes;
				(*iLink)->m_SafetyIndex   = (*iLink)->m_link_type * (*iLink)->m_NumberOfLanes;
				(*iLink)->m_MobilityIndex = (*iLink)->m_SpeedLimit+10*g_GetRandomRatio();
				(*iLink)->m_EmissionsIndex = (*iLink)->ObtainHistCO2Emissions(0);
				float predicted_speed  = (*iLink)->m_SpeedLimit * g_GetRandomRatio();
			}

		}

		fprintf(st,"congestion_index,link_id,from_node_id,to_node_id,direction,length_in_mile,number_of_lanes,speed_limit,lane_cap,link_type,jam_density,wave_speed,mode_code,grade,prohibited_u-turn,geometry\n");
		for (iLink = m_SubareaLinkSet.begin(); iLink != m_SubareaLinkSet.end(); iLink++)
		{
			if((*iLink)->m_AVISensorFlag == false)
			{
				fprintf(st,"%d,%d,%d,%d,%d,%f,%d,%f,%f,%d,%f,%f,%s,%f,%d",
					(*iLink)->m_MobilityIndex,
					(*iLink)->m_LinkID, 
					(*iLink)->m_FromNodeNumber, 
					(*iLink)->m_ToNodeNumber , (*iLink)->m_Direction,(*iLink)->m_Length ,(*iLink)->m_NumberOfLanes ,(*iLink)->m_SpeedLimit,(*iLink)->m_LaneCapacity ,(*iLink)->m_link_type,(*iLink)->m_Kjam, (*iLink)->m_Wave_speed_in_mph,(*iLink)->m_Mode_code.c_str (), 
					(*iLink)->m_Grade,(*iLink)->m_prohibited_u_turn );
				fprintf(st,"\"<LineString><coordinates>");

				for(unsigned int si = 0; si< (*iLink)->m_ShapePoints.size(); si++)
				{
					fprintf(st,"%f,%f,0.0",(*iLink)->m_ShapePoints[si].x, (*iLink)->m_ShapePoints[si].y);

					if(si!=(*iLink)->m_ShapePoints.size()-1)
						fprintf(st," ");
				}


				fprintf(st,"</coordinates></LineString>\"\n");
			}
		}

		fclose(st);
	}else
	{
		AfxMessageBox("Error: File optional_subarea_link.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}

	return true;
}

void CTLiteDoc::OnImportAvi()
{
	// TODO: Add your command handler code here
}

void CTLiteDoc::OnImportGps33185()
{
	// TODO: Add your command handler code here
}

void CTLiteDoc::OnImportVii()
{
	// TODO: Add your command handler code here
}

void CTLiteDoc::OnImportWeather33188()
{
	// TODO: Add your command handler code here
}

void CTLiteDoc::OnImportGps()
{
	// TODO: Add your command handler code here
}

void CTLiteDoc::OnImportWorkzone()
{
	OnScenarioConfiguration();
}

void CTLiteDoc::OnImportIncident()
{
	OnScenarioConfiguration();
}

void CTLiteDoc::OnImportWeather()
{
	OnScenarioConfiguration();
}

void CTLiteDoc::OnImportPricing()
{
	OnScenarioConfiguration();
}

void CTLiteDoc::OnImportAtis()
{
	OnScenarioConfiguration();
}

void CTLiteDoc::OnImportBus()
{
	OnScenarioConfiguration();
}

void CTLiteDoc::OnLinkAddIncident()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink= m_LinkNoMap [m_SelectedLinkNo];
	if(pLink!=NULL)
	{

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->CapacityReductionVector .clear();

		}
		ReadIncidentScenarioData();
		ReadWorkZoneScenarioData();
		ReadTrafficControlScenarioData();

		CapacityReduction cs;
		cs.bWorkzone  = false; 

		cs.ScenarioNo =  0; 
		cs.StartDayNo  = 20;
		cs.EndDayNo	   = 20;
		cs.StartTime = 600;
		cs.EndTime = 640;
		cs.LaneClosurePercentage= 60;
		cs.SpeedLimit = 30;

		pLink->CapacityReductionVector.push_back(cs);


		WriteIncidentScenarioData();

		CDlgScenario dlg(_INCIDENT);
		dlg.m_pDoc = this;

		dlg.DoModal();

		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnLinkAddWorkzone()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink= m_LinkNoMap [m_SelectedLinkNo];
	if(pLink!=NULL)
	{

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->CapacityReductionVector .clear();

		}

		ReadWorkZoneScenarioData();
		CapacityReduction cs;
		cs.bWorkzone  = true; 

		cs.ScenarioNo =  0; 
		cs.StartDayNo  = 0;
		cs.EndDayNo	   = 100;
		cs.StartTime = 0;
		cs.EndTime = 1440;
		cs.LaneClosurePercentage= 50;
		cs.SpeedLimit = 50;

		pLink->CapacityReductionVector.push_back(cs);


		WriteWorkZoneScenarioData();

		CDlgScenario dlg(_WORKZONE);
		dlg.m_pDoc = this;

		dlg.DoModal();

		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnLinkAddvms()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink= m_LinkNoMap [m_SelectedLinkNo];
	if(pLink!=NULL)
	{

		ReadVMSScenarioData();

		// add VMS
		MessageSign ms;

		ms.ScenarioNo = 0;
		ms.StartDayNo = 0;
		ms.EndDayNo  = 100;
		ms.StartTime = 0;
		ms.EndTime = 1440;
		ms.ResponsePercentage= 20;
		pLink->MessageSignVector.push_back(ms);

		WriteVMSScenarioData();

		CDlgScenario dlg(_DYNMSGSIGN);
		dlg.m_pDoc = this;

		dlg.DoModal();

		UpdateAllViews(0);
	}

}

int CTLiteDoc::ReadLink_basedTollScenarioData()
{
	FILE* st = NULL;
	int i =0;
	// toll
	CString toll_file = m_ProjectDirectory+"Scenario_Link_Based_Toll.csv";
	fopen_s(&st,toll_file,"r");
	if(st!=NULL)
	{
		char  str_line[2000]; // input string
		int str_line_size;
		g_read_a_line(st, str_line, str_line_size); //  skip the first line  // with HOV 2 and 3

		// reset
		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->TollVector .clear ();
		}

		while(true)
		{
			int usn  = g_read_integer(st,false);
			if(usn <= 0)
				break;

			int dsn =  g_read_integer(st,false);
			if (dsn <= 0)
				break;

			DTALink* plink = FindLinkWithNodeNumbers(usn,dsn,toll_file );

			if(plink!=NULL)
			{
				DTAToll tl;
				tl.ScenarioNo  = g_read_integer(st);
				if (tl.ScenarioNo < -0.1)
					break;

				tl.StartDayNo   = g_read_integer(st);
				tl.EndDayNo    = g_read_integer(st);

				tl.StartTime = g_read_integer(st);
				tl.EndTime = g_read_integer(st);

				for (int p = 1; p <= m_DemandTypeVector.size(); p++)
				{

					float value = g_read_float(st);

					if (value < -0.1)
						break;

					tl.TollRate[p] = value;
				}
				TRACE("\nadd link toll: %d,%d", usn, dsn);
				plink->TollVector.push_back(tl);
				i++;
			}
		}

		fclose(st);
	}
	return i;
}

bool CTLiteDoc::WriteLink_basedTollScenarioData()
{

	FILE* st = NULL;
	int i =0;
	//  Dynamic Message Sign
	fopen_s(&st,m_ProjectDirectory+"Scenario_Link_Based_Toll.csv","w");
	if(st!=NULL)
	{
		// reset

		fprintf(st, "Link,Scenario No,Start Day,End Day,Start Time in Min,End Time in min,");

		for (int dt = 1; dt <= m_DemandTypeVector.size(); dt++)
		{
			fprintf(st, "Toll for Demand Type %d,", dt);

		}
		fprintf(st,"\n");
		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{

			for(unsigned int i = 0; i < (*iLink)->TollVector  .size(); i++)
			{
				fprintf(st, "\"[%d,%d]\",%d,%d,%d,%3.0f,%3.0f,", (*iLink)->m_FromNodeNumber, (*iLink)->m_ToNodeNumber,
					(*iLink)->TollVector[i].ScenarioNo,
					(*iLink)->TollVector[i].StartDayNo,
					(*iLink)->TollVector[i].EndDayNo, (*iLink)->TollVector[i].StartTime, (*iLink)->TollVector[i].EndTime);

				for (int p = 1; p <= m_DemandTypeVector.size(); p++)
				{
					fprintf(st, "%3.2f,", (*iLink)->TollVector[i].TollRate [p]);
				}

				fprintf(st, "\n");
			}
		}

		fclose(st);
	}

	return true;
}

bool CTLiteDoc::WriteIncidentScenarioData()
{

	FILE* st = NULL;
	int i =0;
	//  incident scenario
	fopen_s(&st,m_ProjectDirectory+"Scenario_Incident.csv","w");
	if(st!=NULL)
	{
		// reset

		fprintf(st, "Link,Scenario No, Day No,Start Time in Min,End Time in min,Capacity Reduction Percentage (%%),Speed Limit\n");

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{

			for(unsigned int i = 0; i < (*iLink)->CapacityReductionVector  .size(); i++)
			{

				CapacityReduction element  = (*iLink)->CapacityReductionVector[i];

				if(element.bWorkzone == false)
				{
					fprintf(st,"\"[%d,%d]\",%d,%d,%3.0f,%3.0f,%3.1f,%3.1f\n", (*iLink)->m_FromNodeNumber , (*iLink)->m_ToNodeNumber ,
						element.ScenarioNo ,element.StartDayNo , element.StartTime , element.EndTime ,element.LaneClosurePercentage, element.SpeedLimit );
				}
			}
		}

		fclose(st);
	}

	return true;
}

bool CTLiteDoc:: WriteCapacityReductionScenarioDataFromSubareaLinks(CString Scenario_File_Name)
{
	FILE* st = NULL;
	int i =0;
	//  incident scenario
	fopen_s(&st,m_ProjectDirectory+Scenario_File_Name,"w");
	if(st!=NULL)
	{
		// reset

		fprintf(st, "Link,Scenario No,Start Day,End Day,Start Time in Min,End Time in min,Capacity Reduction Percentage (%%),Speed Limit\n");

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{

			if( (*iLink)->m_bIncludedinSubarea)
			{

				CapacityReduction element;
				fprintf(st,"\"[%d,%d]\",0,1,1,0,1440,2,%3.1f\n", (*iLink)->m_FromNodeNumber , (*iLink)->m_ToNodeNumber , (*iLink)->m_SpeedLimit  );
			}
		}

		fclose(st);
	}

	return true;
}

bool CTLiteDoc::WriteWorkZoneScenarioData()
{
	FILE* st = NULL;
	int i =0;
	//  incident scenario
	fopen_s(&st,m_ProjectDirectory+"Scenario_Work_Zone.csv","w");
	if(st!=NULL)
	{
		// reset

		fprintf(st, "Link,Scenario No,Start Day,End Day,Start Time in Min,End Time in min,Capacity Reduction Percentage (%%),Speed Limit\n");

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{

			for(unsigned int i = 0; i < (*iLink)->CapacityReductionVector  .size(); i++)
			{

				CapacityReduction element  = (*iLink)->CapacityReductionVector[i];

				if(element.bWorkzone == true)
				{
					fprintf(st,"\"[%d,%d]\",%d,%d,%d,%3.1f,%3.1f,%3.1f,%3.1f\n", (*iLink)->m_FromNodeNumber , (*iLink)->m_ToNodeNumber ,
						element.ScenarioNo,element.StartDayNo  , element.EndDayNo , element.StartTime , element.EndTime ,element.LaneClosurePercentage, element.SpeedLimit );
				}
			}
		}

		fclose(st);
	}

	return true;
}

bool CTLiteDoc::WriteTrafficControlScenarioData()
{
	FILE* st = NULL;
	int i =0;
	//  incident scenario
	fopen_s(&st,m_ProjectDirectory+"Scenario_Generic_Traffic_Control.csv","w");
	if(st!=NULL)
	{
		// reset

		fprintf(st, "Link,Scenario No,Start Day,End Day,Start Time in HH:MM:SS,End Time in HH:MM:SS,Lane Capacity (vph),Speed Limit\n");

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{

			for(unsigned int i = 0; i < (*iLink)->CapacityReductionVector  .size(); i++)
			{

				CapacityReduction element  = (*iLink)->CapacityReductionVector[i];

				if(element.bGenericTrafficControl == true)
				{ 
					int hour1 = element.StartTime / 60;
					int min1 = element.StartTime - hour1 *60;
					int sec1 = (element.StartTime - hour1 * 60) * 60;

					int hour2 = element.EndTime / 60;
					int min2 = element.EndTime - hour2 * 60;
					int sec2 = (element.EndTime - hour2 * 60) * 60;

					fprintf(st,"\"[%d,%d]\",%d,%d,%d,%d:%d:%d,%d:%d:%d,%3.0f,%3.1f,%3.1f\n", (*iLink)->m_FromNodeNumber , (*iLink)->m_ToNodeNumber ,
						element.ScenarioNo,element.StartDayNo  , element.EndDayNo , hour1, min1, sec1, hour2,min2, sec2, element.ServiceFlowRate, element.SpeedLimit );
				}
			}
		}

		fclose(st);
	}

	return true;
}

int CTLiteDoc::ReadWorkZoneScenarioData(int RemoveLinkFromNodeNumber, int RemoveLinkToNodeNumber)
{

	int i = 0;

	CString workzone_file = m_ProjectDirectory+"Scenario_Work_Zone.csv";
	int error_count  = 0; 
	FILE* st =NULL;
	fopen_s(&st,workzone_file,"r");
	if(st!=NULL)
	{
		while(true)
		{
			int usn  = g_read_integer(st);
			if(usn == -1)
				break;

			int dsn =  g_read_integer(st);

			DTALink* plink = FindLinkWithNodeNumbers(usn,dsn,workzone_file );

			if(plink!=NULL)
			{
				CapacityReduction cs;
				cs.bWorkzone  = true; 

				cs.ScenarioNo =  g_read_integer(st); 
				cs.StartDayNo  = g_read_integer(st);
				cs.EndDayNo	   = g_read_integer(st);
				cs.StartTime = g_read_float(st);
				cs.EndTime = g_read_float(st);
				cs.LaneClosurePercentage= g_read_float(st);
				cs.SpeedLimit = g_read_float(st);

				if(RemoveLinkFromNodeNumber==usn && RemoveLinkToNodeNumber== dsn)
				{
					//skip
				}else
				{
					plink->CapacityReductionVector.push_back(cs);
					i++;
				}
			}else
			{
				error_count ++;
				if(error_count >= 3) break; // no more than 3 warnings

			}
		}
		fclose(st);
	}

	return i;
}

int CTLiteDoc::ReadTrafficControlScenarioData(int RemoveLinkFromNodeNumber, int RemoveLinkToNodeNumber)
{

	int i = 0;

	CString workzone_file = m_ProjectDirectory+"Scenario_Generic_Traffic_Control.csv";
	int error_count  = 0; 
	FILE* st =NULL;
	fopen_s(&st,workzone_file,"r");
	if(st!=NULL)
	{
		while(true)
		{
			int usn  = g_read_integer(st);
			if(usn == -1)
				break;

			int dsn =  g_read_integer(st);

			DTALink* plink = FindLinkWithNodeNumbers(usn,dsn,workzone_file );

			if(plink!=NULL)
			{
				CapacityReduction cs;
				cs.bGenericTrafficControl  = true; 

				cs.ScenarioNo =  g_read_integer(st); 
				cs.StartDayNo  = g_read_integer(st);
				cs.EndDayNo	   = g_read_integer(st);
				

				int hour, min, second;

				hour = g_read_integer(st);
				min = g_read_integer(st);
				second=g_read_integer(st);

				cs.StartTime = hour * 60 + min + second / 60.0;

				hour = g_read_integer(st);
				min = g_read_integer(st);
				second = g_read_integer(st);

				cs.EndTime = hour * 60 + min + second / 60.0;
				
				cs.ServiceFlowRate= g_read_float(st);
				cs.SpeedLimit = g_read_float(st);

				if(RemoveLinkFromNodeNumber==usn && RemoveLinkToNodeNumber== dsn)
				{
					//skip
				}else
				{
					plink->CapacityReductionVector.push_back(cs);
					i++;
				}
			}else
			{
				error_count ++;
				if(error_count >= 3) break; // no more than 3 warnings

			}
		}
		fclose(st);
	}

	return i;
}

int CTLiteDoc::ReadIncidentScenarioData(int RemoveLinkFromNodeNumber, int RemoveLinkToNodeNumber)
{


	CString incident_file = m_ProjectDirectory+"Scenario_Incident.csv";
	FILE* st;
	fopen_s(&st,incident_file,"r");
	int error_count  = 0;

	int i = 0;
	if(st!=NULL)
	{
		while(true)
		{
			int usn  = g_read_integer(st);
			if(usn == -1)
				break;

			int dsn =  g_read_integer(st);

			DTALink* plink = FindLinkWithNodeNumbers(usn,dsn,incident_file );

			if(plink!=NULL )
			{
				CapacityReduction cs;
				cs.bIncident = true; 

				cs.ScenarioNo = g_read_integer(st);
				cs.StartDayNo = g_read_integer(st);
				cs.EndDayNo = cs.StartDayNo;
				cs.StartTime = g_read_integer(st);
				cs.EndTime = g_read_integer(st);
				cs.LaneClosurePercentage= g_read_float(st);
				cs.SpeedLimit = g_read_float(st);

				if(RemoveLinkFromNodeNumber==usn && RemoveLinkToNodeNumber== dsn)
				{
					//skip
				}else
				{
					plink->CapacityReductionVector.push_back(cs);
					i++;

				}
			}else
			{
				error_count ++;
				if(error_count >= 3) break; // no more than 3 warnings
			}

		}
		fclose(st);
	}

	return i;
}

int CTLiteDoc::ReadVMSScenarioData(int RemoveLinkFromNodeNumber, int RemoveLinkToNodeNumber )
{
	// clean up all scenario data first
	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->MessageSignVector.clear ();

	}

	FILE* st = NULL;
	int i =0;
	//  Dynamic Message Sign
	CString VMS_file = m_ProjectDirectory+"Scenario_Dynamic_Message_Sign.csv";
	fopen_s(&st,VMS_file,"r");
	if(st!=NULL)
	{
		// reset
		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->MessageSignVector.clear ();
		}

		while(true)
		{
			int usn  = g_read_integer(st);
			if(usn == -1)
				break;

			int dsn =  g_read_integer(st);

			DTALink* plink = FindLinkWithNodeNumbers(usn,dsn,VMS_file );

			if(plink!=NULL)
			{
				MessageSign ms;
				ms.ScenarioNo = g_read_integer(st);
				ms.StartDayNo  = g_read_integer(st);
				ms.EndDayNo  = g_read_integer(st);
				ms.StartTime = g_read_integer(st);
				ms.EndTime = g_read_integer(st);
				ms.ResponsePercentage= g_read_float(st);

				if(RemoveLinkFromNodeNumber==usn && RemoveLinkToNodeNumber== dsn)
				{
					//skip
				}else
				{
					plink->MessageSignVector.push_back(ms);
					i++;
				}
			}
		}

		fclose(st);
	}

	return i;
}
bool CTLiteDoc::WriteVMSScenarioData()
{
	FILE* st = NULL;
	int i =0;
	//  Dynamic Message Sign
	fopen_s(&st,m_ProjectDirectory+"Scenario_Dynamic_Message_Sign.csv","w");
	if(st!=NULL)
	{
		// reset
		fprintf(st, "Link,Scenario No,Start Day No,End Day No,Start Time in Min,End Time in min,Number of Detour Routes,Detour Route 1,Detour Route 2,Detour Route 3\n");

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{

			for(unsigned int i = 0; i < (*iLink)->MessageSignVector .size(); i++)
			{
				fprintf(st,"\"[%d,%d]\",%d,%d,%d,%3.0f,%3.0f,%3.1f\n", (*iLink)->m_FromNodeNumber , (*iLink)->m_ToNodeNumber ,
					(*iLink)->MessageSignVector[i].ScenarioNo ,(*iLink)->MessageSignVector[i].StartDayNo , (*iLink)->MessageSignVector[i].EndDayNo , 
					(*iLink)->MessageSignVector[i].StartTime , (*iLink)->MessageSignVector[i].EndTime ,(*iLink)->MessageSignVector[i].ResponsePercentage);
			}
		}

		fclose(st);
	}

	return true;
}

void CTLiteDoc::OnImportLinklayerinkml()
{
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Extenral Link Layer Data in KML/CSV Format (*.csv)|*.csv|"));
	if(dlg.DoModal() == IDOK)
	{
		ReadLinkCSVFile(dlg.GetPathName(),true, false);

	}
	OffsetLink();
	CalculateDrawingRectangle();
	m_bFitNetworkInitialized  = false;
}

void CTLiteDoc::OnEditOffsetlinks()
{
	m_bLinkToBeShifted = true;
	OffsetLink();
	UpdateAllViews(0);
}


void CTLiteDoc::OnImportSubarealayerformapmatching()
{
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Traffic network project for reference network(*.tnp)|*.tnp|"));
	if(dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;
		CString ProjectFile = dlg.GetPathName();
		CString Second_Project_directory = ProjectFile.Left(ProjectFile.ReverseFind('\\') + 1);

		if(strcmp(Second_Project_directory, m_ProjectDirectory)==0)
		{
			AfxMessageBox("Please ensure the reference network data set is loaded from a folder different from the current project folder.");

			return ;
		}

		ReadLinkCSVFile(Second_Project_directory+"input_link.csv",false, 1);  // 1 for subarea layer
		OffsetLink();
		CalculateDrawingRectangle();
		m_bFitNetworkInitialized  = false;
		UpdateAllViews(0);
	}

}

void CTLiteDoc::ChangeNetworkCoordinates(int LayerNo, float XScale, float YScale, float delta_x, float delta_y)
{
	GDRect NetworkRect;

	bool bRectInitialized = false;


		for (std::list<DTANode*>::iterator iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{
			if ((*iNode)->m_Connections > 0)   // for selected layer only
			{
				if (!bRectInitialized)
				{
					NetworkRect.left = (*iNode)->pt.x;
					NetworkRect.right = (*iNode)->pt.x;
					NetworkRect.top = (*iNode)->pt.y;
					NetworkRect.bottom = (*iNode)->pt.y;
					bRectInitialized = true;
				}

				NetworkRect.Expand((*iNode)->pt);
			}

		}

		float m_XOrigin = NetworkRect.Center().x;

		float m_YOrigin = NetworkRect.Center().y;

		// adjust node coordinates
		std::list<DTANode*>::iterator iNode;
		for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{

			if ((*iNode)->m_LayerNo == LayerNo)  // for selected layer only
			{
				(*iNode)->pt.x = ((*iNode)->pt.x - m_XOrigin)*XScale + m_XOrigin + delta_x;
				(*iNode)->pt.y = ((*iNode)->pt.y - m_YOrigin)*YScale + m_YOrigin + delta_y;
			}
		}

		//adjust link cooridnates

		std::list<DTALink*>::iterator iLink;

		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			if ((*iLink)->m_LayerNo == LayerNo)   // for selected layer only
			{

				(*iLink)->m_FromPoint.x = ((*iLink)->m_FromPoint.x - m_XOrigin)*XScale + m_XOrigin + delta_x;
				(*iLink)->m_FromPoint.y = ((*iLink)->m_FromPoint.y - m_YOrigin)*YScale + m_YOrigin + delta_y;

				(*iLink)->m_ToPoint.x = ((*iLink)->m_ToPoint.x - m_XOrigin)*XScale + m_XOrigin + delta_x;
				(*iLink)->m_ToPoint.y = ((*iLink)->m_ToPoint.y - m_YOrigin)*YScale + m_YOrigin + delta_y;

				for (unsigned int si = 0; si < (*iLink)->m_ShapePoints.size(); si++)
				{

					(*iLink)->m_ShapePoints[si].x = ((*iLink)->m_ShapePoints[si].x - m_XOrigin)*XScale + m_XOrigin + delta_x;
					(*iLink)->m_ShapePoints[si].y = ((*iLink)->m_ShapePoints[si].y - m_YOrigin)*YScale + m_YOrigin + delta_y;

				}

				for (unsigned int si = 0; si < (*iLink)->m_BandLeftShapePoints.size(); si++)
				{

					(*iLink)->m_BandLeftShapePoints[si].x = ((*iLink)->m_BandLeftShapePoints[si].x - m_XOrigin)*XScale + m_XOrigin + delta_x;
					(*iLink)->m_BandLeftShapePoints[si].y = ((*iLink)->m_BandLeftShapePoints[si].y - m_YOrigin)*YScale + m_YOrigin + delta_y;

				}

				for (unsigned int si = 0; si < (*iLink)->m_BandRightShapePoints.size(); si++)
				{

					(*iLink)->m_BandRightShapePoints[si].x = ((*iLink)->m_BandRightShapePoints[si].x - m_XOrigin)*XScale + m_XOrigin + delta_x;
					(*iLink)->m_BandRightShapePoints[si].y = ((*iLink)->m_BandRightShapePoints[si].y - m_YOrigin)*YScale + m_YOrigin + delta_y;

				}


			}


		}
	if (LayerNo == 1) //reference layer
	{
		std::list<DTALine*>::iterator iLine;
		for (iLine = m_DTALineSet.begin(); iLine != m_DTALineSet.end(); iLine++)
		{

			for (unsigned int si = 0; si < (*iLine)->m_ShapePoints.size(); si++)
			{

				(*iLine)->m_ShapePoints[si].x = ((*iLine)->m_ShapePoints[si].x - m_XOrigin)*XScale + m_XOrigin + delta_x;
				(*iLine)->m_ShapePoints[si].y = ((*iLine)->m_ShapePoints[si].y - m_YOrigin)*YScale + m_YOrigin + delta_y;

			}


		}

	}

		
}
void CTLiteDoc::OnFileOpenNetworkOnly()
{
	static char BASED_CODE szFilter[] = "NEXTA Data Files (*.dws;*.tnp)|*.dws; *.tnp|DYNASMART Workspace Files (*.dws)|*.dws|Transportation Network Projects (*.tnp)|*.tnp|All Files (*.*)|*.*||";

	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	if(dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;

		CString ProjectFileName = dlg.GetPathName ();

		if(ProjectFileName.Find("tnp")>=0)  //Transportation network project format
		{
			OnOpenTrafficNetworkDocument(ProjectFileName,true);
		}else if(ProjectFileName.Find("dws")>=0)  //DYNASMART-P format
		{
			OnOpenDYNASMARTProject(ProjectFileName,true);
		}
		CDlgFileLoading dlg;
		dlg.m_pDoc = this;
		dlg.DoModal ();

		UpdateAllViews(0);

	}

}

void CTLiteDoc::OnLinkAddlink()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink= m_LinkNoMap [m_SelectedLinkNo];
	if(pLink!=NULL)
	{

		ReadLink_basedTollScenarioData();

		// add toll
		DTAToll toll;

		toll.StartTime = 0;
		toll.EndTime = 1440;
		toll.TollRate[1] = 0.5;
		toll.TollRate[2] = 0.5;
		toll.TollRate[3] = 0.5;
		toll.TollRate[4] = 0;

		for (int p = 5; p < MAX_DEMAND_TYPE_SIZE; p++)
			toll.TollRate[p] = 999;


		pLink->TollVector.push_back(toll);

		WriteLink_basedTollScenarioData();

		CDlgScenario dlg;
		dlg.m_SelectTab = 2;
		dlg.m_pDoc = this;

		dlg.DoModal();

		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnLinkAddhovtoll()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink= m_LinkNoMap [m_SelectedLinkNo];
	if(pLink!=NULL)
	{

		ReadLink_basedTollScenarioData();

		// add toll
		DTAToll toll;

		toll.StartTime = 0;
		toll.EndTime = 1440;
		toll.TollRate[1] = 999;
		toll.TollRate[2] = 0;
		toll.TollRate[3] = 999;
		toll.TollRate[4] = 999;

		for (int p = 5; p < MAX_DEMAND_TYPE_SIZE; p++)
			toll.TollRate[p] = 999;

		pLink->TollVector.push_back(toll);

		WriteLink_basedTollScenarioData();

		CDlgScenario dlg(0);
		dlg.m_pDoc = this;

		dlg.m_SelectTab = 2;
		dlg.DoModal();

		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnLinkAddhottoll()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink= m_LinkNoMap [m_SelectedLinkNo];
	if(pLink!=NULL)
	{

		ReadLink_basedTollScenarioData();

		// add toll
		DTAToll toll;

		toll.StartTime = 0;
		toll.EndTime = 1440;
		toll.TollRate[1] = 0.5;
		toll.TollRate[2] = 0;
		toll.TollRate[3] = 0.5;
		toll.TollRate[4] = 0.5;


		for (int p = 5; p < MAX_DEMAND_TYPE_SIZE; p++)
			toll.TollRate[p] = 999;

		pLink->TollVector.push_back(toll);

		WriteLink_basedTollScenarioData();

		CDlgScenario dlg(0);
		dlg.m_pDoc = this;

		dlg.m_SelectTab = 2;
		dlg.DoModal();

		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnLinkAddtolledexpresslane()
{
	// TODO: Add your command handler code here
}

void CTLiteDoc::OnLinkConvertgeneralpurposelanetotolledlane()
{
	// TODO: Add your command handler code here
}

CString CTLiteDoc::GetTimeStampStrFromIntervalNo(int time_interval, bool with_single_quote)
{
	CString str;
	int hour = time_interval/4;
	int min = (time_interval - hour*4)*15;

	if(with_single_quote)
	{
		if(hour<10)
			str.Format ("'0%d:%02d",hour,min);
		else
			str.Format ("'%2d:%02d",hour,min);
	}else  //without typewriter single quotes
	{
		if(hour<10)
			str.Format ("0%d:%02d",hour,min);
		else
			str.Format ("%2d:%02d",hour,min);
	}


	return str;

}


CString CTLiteDoc::GetTimeStampFloatingPointStrFromIntervalNo(int time_interval)
{
	CString str;
	int hour = time_interval/4;
	int min = (time_interval - hour*4)*15;

	str.Format ("%dh%02d",hour,min);

	return str;

}

CString CTLiteDoc::GetTimeStampString(int time_stamp_in_min)
{
	CString str;
	int hour = time_stamp_in_min/60;
	int min = time_stamp_in_min - hour*60;

	if(hour<12)
		str.Format ("%02d:%02d AM",hour,min);
	else if(hour==12)
		str.Format ("%02d:%02d PM",12,min);
	else
		str.Format ("%02d:%02d PM",hour-12,min);

	return str;

}

CString CTLiteDoc::GetTimeStampString24HourFormat(int time_stamp_in_min)
{
	CString str;
	int hour = time_stamp_in_min/60;
	int min = time_stamp_in_min - hour*60;

	str.Format ("%2d:%02d",hour,min);

	return str;

}
void CTLiteDoc::OnProjectEdittime()
{
}

int CTLiteDoc::FindClassificationNo(DTAVehicle* pVehicle, VEHICLE_X_CLASSIFICATION x_classfication)
{

	int index = -1;  // no classification

	switch(x_classfication)
	{
	case CLS_all_vehicles: index =0 ; break;
	case CLS_pricing_type: index = pVehicle->m_PricingType ; break;
	case CLS_VOT_10: index = pVehicle->m_VOT /10 ; break;
	case CLS_VOT_15: index = pVehicle->m_VOT /15 ; break;
	case CLS_VOT_10_SOV: 
		if(pVehicle->m_PricingType != 1)
			index =  -1;  // no considered.
		else
			index = pVehicle->m_VOT /10 ; 
		break;
	case CLS_VOT_10_HOV: 
		if(pVehicle->m_PricingType != 2)
			index =  -1;  // no considered.
		else
			index = pVehicle->m_VOT /10 ; 
		break;
	case CLS_VOT_10_truck: 
		if(pVehicle->m_PricingType != 3)
			index =  -1;  // no considered.
		else
			index = pVehicle->m_VOT /10 ; 
		break;
	case CLS_time_interval_5_min: index = pVehicle->m_DepartureTime / 5;

		if (m_VehicleSelectionMode == CLS_path_partial_trip)
			index = pVehicle->m_path_start_node_departure_time / 5;

		break;
	case CLS_time_interval_15_min: index = pVehicle->m_DepartureTime /15; 
		
		if (m_VehicleSelectionMode == CLS_path_partial_trip)
			index = pVehicle->m_path_start_node_departure_time / 15;

		break;
	case CLS_time_interval_30_min: index = pVehicle->m_DepartureTime /30; 
		if (m_VehicleSelectionMode == CLS_path_partial_trip)
			index = pVehicle->m_path_start_node_departure_time / 30;
		break;
	case CLS_time_interval_60_min: index = pVehicle->m_DepartureTime /60; 
		if (m_VehicleSelectionMode == CLS_path_partial_trip)
			index = pVehicle->m_path_start_node_departure_time / 60;
		break;
	case CLS_time_interval_2_hour: index = pVehicle->m_DepartureTime /120;
		if (m_VehicleSelectionMode == CLS_path_partial_trip)
			index = pVehicle->m_path_start_node_departure_time / 120;
		break;
	case CLS_time_interval_4_hour: index = pVehicle->m_DepartureTime /240; 
		if (m_VehicleSelectionMode == CLS_path_partial_trip)
			index = pVehicle->m_path_start_node_departure_time / 240;
		break;

	case CLS_distance_bin_0_2: index = pVehicle->m_Distance /0.2; break;
	case CLS_distance_bin_1: index = pVehicle->m_Distance /1; break;

	case CLS_distance_bin_2: index = pVehicle->m_Distance /2; break;
	case CLS_distance_bin_5: index = pVehicle->m_Distance /5; break;
	case CLS_distance_bin_10: index = pVehicle->m_Distance /10; break;
	case CLS_travel_time_bin_2: index = pVehicle->m_TripTime /2; break;
	case CLS_travel_time_bin_5: index = pVehicle->m_TripTime /5; break;
	case CLS_travel_time_bin_10: index = pVehicle->m_TripTime /10; break;
	case CLS_travel_time_bin_30: index = pVehicle->m_TripTime /30; break;

	case CLS_information_class: index = pVehicle->m_InformationClass ; break;
	case CLS_vehicle_type: index = pVehicle->m_VehicleType  ; break;
	default: 
		TRACE("ERROR. No classification available!");
	};

	return index;
}


CString CTLiteDoc::FindClassificationLabel(VEHICLE_X_CLASSIFICATION x_classfication, int index)
{

	CString label;

	switch(x_classfication)
	{
	case CLS_pricing_type: 
		label = GetPricingTypeStr(index);
		break;

	case CLS_VOT_10: 
	case CLS_VOT_10_SOV:
	case CLS_VOT_10_HOV:
	case CLS_VOT_10_truck:
		label.Format ("$%d-$%d",index*10,(index+1)*10);
		break;

	case CLS_VOT_15:
		label.Format ("$%d-$%d",index*15,(index+1)*15);
		break;

	case CLS_time_interval_5_min:
		label.Format("%s-%s", GetTimeStampStrFromIntervalNo(index, false), GetTimeStampStrFromIntervalNo((index + 1), false));
		break;
	case CLS_time_interval_15_min: 
		label.Format ("%s-%s",GetTimeStampStrFromIntervalNo(index,false),GetTimeStampStrFromIntervalNo((index+1),false));
		break;

	case CLS_time_interval_30_min: 
		label.Format ("%s-%s",GetTimeStampStrFromIntervalNo(index*2,false),GetTimeStampStrFromIntervalNo((index+1)*2,false));
		break;

	case CLS_time_interval_60_min: 
		label.Format ("%s-%s",GetTimeStampStrFromIntervalNo(index*4,false),GetTimeStampStrFromIntervalNo((index+1)*4,false));
		break;

	case CLS_time_interval_2_hour: 
		label.Format ("%s-%s",GetTimeStampStrFromIntervalNo(index*8,false),GetTimeStampStrFromIntervalNo((index+1)*8,false));
		break;

	case CLS_time_interval_4_hour: 
		label.Format ("%s-%s",GetTimeStampStrFromIntervalNo(index*16,false),GetTimeStampStrFromIntervalNo((index+1)*16,false));
		break;

	case CLS_distance_bin_0_2: 
		label.Format ("%.1f-%.1f",index*0.2,(index+1)*0.2);
		break;
	case CLS_distance_bin_1: 
		label.Format ("%d-%d",index*1,(index+1)*1);
		break;
	case CLS_distance_bin_2: 
		label.Format ("%d-%d",index*2,(index+1)*2);
		break;

	case CLS_distance_bin_5: 
		label.Format ("%d-%d",index*5,(index+1)*5);
		break;
	case CLS_distance_bin_10: 
		label.Format ("%d-%d",index*10,(index+1)*10);
		break;
	case CLS_travel_time_bin_2: 
		label.Format ("%d-%d",index*2,(index+1)*2);
		break;
	case CLS_travel_time_bin_5: 
		label.Format ("%d-%d",index*5,(index+1)*5);
		break;
	case CLS_travel_time_bin_10: 
		label.Format ("%d-%d",index*10,(index+1)*10);
		break;
	case CLS_travel_time_bin_30: 
		label.Format ("%d-%d",index*30,(index+1)*30);
		break;
	case CLS_information_class: 
		label = "UE";
		if(index == 1) label = "Historical info";
		if(index == 2) label = "Pretrip info";
		if(index == 3) label = "En-route Info";
		if (index == 4) label = "Personalized Info";
		if (index == 5) label = "Eco-SO Info";
		break;

	case CLS_vehicle_type: 
		label = m_VehicleTypeVector[index-1].vehicle_type_name;
		break;
	default: 
		TRACE("ERROR. No classification available!");
	};

	return label;
}

bool CTLiteDoc::SelectVehicleForAnalysis(DTAVehicle* pVehicle, VEHICLE_CLASSIFICATION_SELECTION vehicle_selection)
{
	if(!pVehicle->m_bComplete)
		return false;

	if(vehicle_selection == CLS_network)
		return  true;  // all the vehicles

	if(vehicle_selection == CLS_OD)
		return  pVehicle->m_bODMarked;  // marked by vehicle path dialog


	if(vehicle_selection == CLS_link_set)
	{
		for(int link= 1; link<pVehicle->m_NodeSize; link++)
		{

			if(m_LinkNoMap.find(pVehicle->m_NodeAry[link].LinkNo) != m_LinkNoMap.end() && m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo]->m_DisplayLinkID>=0)  // in one of selected links
			{
				return true;		
			}
		}
		return false;
	}

	if(vehicle_selection == CLS_path_trip)
	{
		if(m_PathDisplayList.size() == 0)
			return false;

		if(m_SelectPathNo >= m_PathDisplayList.size())
			return false;

		int count_of_links_in_selected_path = 0;

		for(int link= 1; link<pVehicle->m_NodeSize; link++)
		{
			if(m_LinkNoMap.find(pVehicle->m_NodeAry[link].LinkNo) != m_LinkNoMap.end() &&  m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo]->m_bIncludedBySelectedPath )
			{
				count_of_links_in_selected_path++;	
			}
		}

		if(count_of_links_in_selected_path == m_PathDisplayList[m_SelectPathNo].m_LinkVector.size() && m_PathDisplayList[m_SelectPathNo].m_LinkVector.size() >0)
			return true;
		else
			return false;
	}

	if(vehicle_selection == CLS_path_partial_trip)
	{
		if(m_PathDisplayList.size() == 0)
			return false;

		if(m_SelectPathNo >= m_PathDisplayList.size())
			return false;

		int count_of_links_in_selected_path = 0;

		for(int link= 1; link<pVehicle->m_NodeSize; link++)
		{
			if(m_LinkNoMap.find(pVehicle->m_NodeAry[link].LinkNo) != m_LinkNoMap.end() &&  m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo]->m_bIncludedBySelectedPath )
			{
				count_of_links_in_selected_path++;	
			}
		}

		if(count_of_links_in_selected_path == m_PathDisplayList[m_SelectPathNo].m_LinkVector.size() && m_PathDisplayList[m_SelectPathNo].m_LinkVector.size() >0)
		{
			//update subtrip travel time

			float subtrip_distance = 0;
			float subtrip_free_flow_travel_time = 0;
			bool b_distance_counting_flag = false;
			for(int link= 1; link<pVehicle->m_NodeSize; link++)
			{

				if(m_LinkNoMap.find(pVehicle->m_NodeAry[link].LinkNo) != m_LinkNoMap.end() &&  m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo]->m_bFirstPathLink  )
				{
					pVehicle->m_path_start_node_departure_time = pVehicle->m_NodeAry[link-1].ArrivalTimeOnDSN;
					b_distance_counting_flag = true;
				}

				if(b_distance_counting_flag)  // count from the first link to the last link
				{
					subtrip_distance += m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo] ->m_Length ;
					subtrip_free_flow_travel_time += m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo] ->m_FreeFlowTravelTime  ;
				}


				if(m_LinkNoMap.find(pVehicle->m_NodeAry[link].LinkNo) != m_LinkNoMap.end() &&  m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo]->m_bLastPathLink  )
				{
					pVehicle->m_path_end_node_arrival_time  = pVehicle->m_NodeAry[link].ArrivalTimeOnDSN;

					pVehicle->m_path_travel_time = pVehicle->m_path_end_node_arrival_time - pVehicle->m_path_start_node_departure_time;

					b_distance_counting_flag = false;

					break;

				}


			}

			pVehicle->m_path_distance  = subtrip_distance;
			pVehicle->m_path_free_flow_travel_time = subtrip_free_flow_travel_time;

			return true;
		}
		else
			return false;
	}

	if(vehicle_selection == CLS_subarea_generated)
	{
		// if the first link of a vehicle is marked, then return true

		if(pVehicle->m_NodeSize>2)
		{

			for(int link= 1; link< pVehicle->m_NodeSize; link++)
			{

				if(m_LinkNoMap.find(pVehicle->m_NodeAry[link].LinkNo) != m_LinkNoMap.end())  // first  valide link
				{
					if(m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo]->m_bIncludedinSubarea == true)
						return true;
					else
						return false;
				}

			}

		}
		return false;
	}

	if(vehicle_selection == CLS_subarea_traversing_through)
	{

		if(pVehicle->m_NodeSize<3)
			return false;
		// now pVehicle->m_NodeSize>=3;

		// condition 1: if the first link of a vehicle is marked (in the subarea), then return false
		if(m_LinkNoMap.find(pVehicle->m_NodeAry[1].LinkNo) != m_LinkNoMap.end() && m_LinkNoMap.find(pVehicle->m_NodeAry[1].LinkNo) != m_LinkNoMap.end())
		{
			if( m_LinkNoMap[pVehicle->m_NodeAry[1].LinkNo]->m_bIncludedinSubarea == true)
				return false;
		}

		// now the first link is not in subarea
		// condition 2: a link is in subarea
		bool bPassingSubareaFlag = false;
		for(int link= 2; link<pVehicle->m_NodeSize-1; link++)
		{

			if( m_LinkNoMap.find(pVehicle->m_NodeAry[link].LinkNo) != m_LinkNoMap.end() && m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo]->m_bIncludedinSubarea )
			{
				bPassingSubareaFlag = true;	
				break;
			}
		}

		if(bPassingSubareaFlag == false)
			return false;
		// now there is a link is in subarea
		// condition 3: test if the last link is out of subarea

		int last_link  = pVehicle->m_NodeSize-1;
		if(m_LinkNoMap.find(pVehicle->m_NodeAry[last_link].LinkNo) != m_LinkNoMap.end() &&  m_LinkNoMap[pVehicle->m_NodeAry[last_link].LinkNo]->m_bIncludedinSubarea == false)
			return true;  // last link is outside of the subarea area, yes!
		else
			return false;

	}

	if(vehicle_selection == CLS_subarea_internal_to_external)
	{
		if(pVehicle->m_NodeSize<2)
			return false;
		// now pVehicle->m_NodeSize>=2;

		// condition 1: if the first link of a vehicle is not marked (not in the subarea), then return false
		if( m_LinkNoMap.find(pVehicle->m_NodeAry[1].LinkNo) != m_LinkNoMap.end() && m_LinkNoMap[pVehicle->m_NodeAry[1].LinkNo]->m_bIncludedinSubarea == false)
			return false;

		// now the first link is in subarea
		// condition 2: test if the last link is out of subarea

		int last_link  = pVehicle->m_NodeSize-1;
		if(m_LinkNoMap.find(pVehicle->m_NodeAry[last_link].LinkNo) != m_LinkNoMap.end() && m_LinkNoMap[pVehicle->m_NodeAry[last_link].LinkNo]->m_bIncludedinSubarea  == false)
			return true;  // outside, yes,
		else
			return false;
	}

	if(vehicle_selection == CLS_subarea_external_to_internal)
	{

		if(pVehicle->m_NodeSize<2)
			return false;
		// now pVehicle->m_NodeSize>=2;

		// condition 1: if the first link of a vehicle is marked (in the subarea), then return false
		if(m_LinkNoMap.find(pVehicle->m_NodeAry[1].LinkNo) != m_LinkNoMap.end() && m_LinkNoMap[pVehicle->m_NodeAry[1].LinkNo]->m_bIncludedinSubarea == true)
			return false;

		// now the first link is not in the subarea
		// condition 2: test if the last link is in subarea

		int last_link  = pVehicle->m_NodeSize-1;
		if( m_LinkNoMap.find(pVehicle->m_NodeAry[last_link].LinkNo) != m_LinkNoMap.end() && m_LinkNoMap[pVehicle->m_NodeAry[last_link].LinkNo]->m_bIncludedinSubarea  == true)
			return true;  // inside, yes!
		else
			return false;
	}
	if(vehicle_selection == CLS_subarea_internal_to_internal_trip)
	{

		if(pVehicle->m_NodeSize<2)  
			return false;
		// now pVehicle->m_NodeSize>=2;

		for(int link= 1; link<pVehicle->m_NodeSize; link++)
		{
			if( m_LinkNoMap.find(pVehicle->m_NodeAry[link].LinkNo) != m_LinkNoMap.end() && m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo]->m_bIncludedinSubarea == false )
				return false; // this is a link outside of the subarea
		}

		// pass all the tests
		return true;
	}


	if(vehicle_selection == CLS_subarea_internal_to_internal_subtrip)
	{

		if(pVehicle->m_NodeSize<2)  
			return false;
		// now pVehicle->m_NodeSize>=2;

		float subtrip_distance = 0;
		float subtrip_free_flow_travel_time = 0;
		int distance_counting_flag = 0;
		for(int link= 1; link<pVehicle->m_NodeSize; link++)
		{
			if(m_LinkNoMap.find(pVehicle->m_NodeAry[link].LinkNo) != m_LinkNoMap.end())
			{
				DTALink *pLink = m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo];
				if(pLink->m_bIncludedinSubarea == true )
				{
					if(distance_counting_flag==0)
					{
						distance_counting_flag = 1;
						pVehicle->m_subarea_start_node_departure_time = pVehicle->m_NodeAry [link-1].ArrivalTimeOnDSN ;

					}

					if(distance_counting_flag==1)
					{
						subtrip_distance+= m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo]->m_Length ;
						subtrip_free_flow_travel_time += m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo]->m_FreeFlowTravelTime ;


						// keep the last record
						pVehicle->m_subarea_distance = subtrip_distance; // for inside path
						pVehicle->m_subarea_free_flow_travel_time = subtrip_free_flow_travel_time;
						pVehicle->m_subarea_end_node_arrival_time  = pVehicle->m_NodeAry [link].ArrivalTimeOnDSN ;
						pVehicle->m_subarea_travel_time = pVehicle->m_subarea_end_node_arrival_time  - pVehicle->m_subarea_start_node_departure_time;

					}


				}else if(distance_counting_flag == 1) 
				{  //leaving subarea

					distance_counting_flag = 2;

					break;
				}

			}
		}

		// pass all the tests
		if(distance_counting_flag>=1)
			return true;
	}
	if(vehicle_selection == CLS_subarea_boundary_to_bounary_subtrip)
	{

		if(pVehicle->m_NodeSize<2)  
			return false;
		// now pVehicle->m_NodeSize>=2;

		float subtrip_distance = 0;
		float subtrip_free_flow_travel_time = 0;
		int distance_counting_flag = 0;
		for(int link= 1; link<pVehicle->m_NodeSize; link++)
		{
			if( m_LinkNoMap.find(pVehicle->m_NodeAry[link].LinkNo) != m_LinkNoMap.end())
			{
				DTALink* pLink = m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo];
				if(pLink->m_bIncludedinSubarea == true )
				{
					if(distance_counting_flag==0)
					{
						distance_counting_flag = 1;
						//record the boundary arrival time
						pVehicle->m_subarea_start_node_departure_time = pVehicle->m_NodeAry [link-1].ArrivalTimeOnDSN ;

					}

					if(distance_counting_flag==1) // when distance_counting_flag ==0, we will go to this condition right away
					{
						//start counting we  have boundary start time
						subtrip_distance+= m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo]->m_Length ;
						subtrip_free_flow_travel_time += m_LinkNoMap[pVehicle->m_NodeAry[link].LinkNo]->m_FreeFlowTravelTime ;

						// keep the last record
						pVehicle->m_subarea_distance = subtrip_distance; // for inside path
						pVehicle->m_subarea_free_flow_travel_time = subtrip_free_flow_travel_time;
						pVehicle->m_subarea_end_node_arrival_time  = pVehicle->m_NodeAry [link].ArrivalTimeOnDSN ;
						pVehicle->m_subarea_travel_time = pVehicle->m_subarea_end_node_arrival_time  - pVehicle->m_subarea_start_node_departure_time;

						ASSERT(pVehicle->m_subarea_travel_time<900);

					}

				}else if(distance_counting_flag == 1) 
				{  //leaving subarea

					distance_counting_flag = 2;
					break;
				}else
				{

					TRACE("\nvehicle %d: Link: %d->%d ", pVehicle->m_VehicleID , pLink->m_FromNodeNumber , pLink->m_ToNodeNumber );

				}

			}

		}

		// pass all the tests
		if(distance_counting_flag==2)
			return true;
	}
	return false;
}
void CTLiteDoc::MarkLinksInSubarea()
{
	std::list<DTALink*>::iterator iLink;

	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->m_bIncludedinSubarea = false;  // reset all the links are not selected by the path
	}

	for (iLink = m_SubareaLinkSet.begin(); iLink != m_SubareaLinkSet.end(); iLink++)
	{
		(*iLink)->m_bIncludedinSubarea = true;
	}
}

void CTLiteDoc::GenerateVehicleClassificationData(VEHICLE_CLASSIFICATION_SELECTION vehicle_selection, VEHICLE_X_CLASSIFICATION x_classfication)
{
	m_ClassificationTable.clear ();  //reset

	std::list<DTAVehicle*>::iterator iVehicle;

	//calculate free-flow travel time
	for (iVehicle = m_VehicleSet.begin(); iVehicle != m_VehicleSet.end(); iVehicle++)
	{
		DTAVehicle* pVehicle = (*iVehicle);

		if(pVehicle->m_FreeflowTripTime <0.00001f)  // freeflow travel time has not been defined yet
		{

			DTAVehicle* pVehicle = (*iVehicle);

			for(int i=1; i< pVehicle->m_NodeSize ; i++)
			{
				if(m_LinkNoMap.find(pVehicle->m_NodeAry[i].LinkNo)!= m_LinkNoMap.end()) 
				{
					DTALink* pLinkCurrent = m_LinkNoMap[ pVehicle->m_NodeAry[i].LinkNo];
					pVehicle->m_FreeflowTripTime+= pLinkCurrent->m_FreeFlowTravelTime ;
				}
			}

		}
	}

	bool bTraceFlag = true;

	for (iVehicle = m_VehicleSet.begin(); iVehicle != m_VehicleSet.end(); iVehicle++)
	{
		DTAVehicle* pVehicle = (*iVehicle);

		pVehicle->m_bMarked = false;
		if(SelectVehicleForAnalysis(pVehicle, m_VehicleSelectionMode) == true)  // belong this classification 
		{
			pVehicle->m_bMarked = true;
			if(bTraceFlag)  // trace single vehicle id
			{
				TRACE("vehicle name: %d\n",pVehicle->m_VehicleID+1  );
				bTraceFlag = false;
			}
			int index = FindClassificationNo(pVehicle,x_classfication);

			if(index>=0)  // -1 will not be consideded
			{
				m_ClassificationTable[index].TotalVehicleSize+=1;


				if(vehicle_selection== CLS_path_partial_trip)  // subpath
				{
					m_ClassificationTable[index].TotalTravelTime  +=  pVehicle->m_path_travel_time;
					m_ClassificationTable[index].TotalFreeflowTravelTime  +=  pVehicle->m_path_free_flow_travel_time;

					m_ClassificationTable[index].TotalDistance   += pVehicle->m_path_distance;				
					m_ClassificationTable[index].TotalTravelTimePerMile += ( pVehicle->m_path_travel_time /max(0.01,pVehicle->m_path_distance));

					m_ClassificationTable[index].m_data_vector_travel_time .AddData (pVehicle->m_path_travel_time);
					m_ClassificationTable[index].m_data_vector_travel_time_per_mile .AddData ( pVehicle->m_path_travel_time /max(0.01,pVehicle->m_path_distance));


				}else if (vehicle_selection == CLS_subarea_internal_to_internal_subtrip || vehicle_selection == CLS_subarea_boundary_to_bounary_subtrip)
				{
					m_ClassificationTable[index].TotalTravelTime  += pVehicle->m_subarea_travel_time ;
					m_ClassificationTable[index].TotalFreeflowTravelTime  += pVehicle->m_subarea_free_flow_travel_time ;
					m_ClassificationTable[index].TotalDistance   += pVehicle->m_subarea_distance;				
					m_ClassificationTable[index].TotalTravelTimePerMile += (pVehicle->m_subarea_travel_time)/max(0.01,pVehicle->m_subarea_distance);

					m_ClassificationTable[index].m_data_vector_travel_time .AddData (pVehicle->m_subarea_travel_time);
					m_ClassificationTable[index].m_data_vector_travel_time_per_mile .AddData (( pVehicle->m_subarea_travel_time) /max(0.01,pVehicle->m_subarea_distance));
				}
				else  //
				{
					m_ClassificationTable[index].TotalTravelTime  += (pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime);
					m_ClassificationTable[index].TotalFreeflowTravelTime  += (pVehicle->m_FreeflowTripTime);

					m_ClassificationTable[index].TotalDistance   += pVehicle->m_Distance;				
					m_ClassificationTable[index].TotalTravelTimePerMile += ( (pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime)/pVehicle->m_Distance);

					m_ClassificationTable[index].m_data_vector_travel_time .AddData (pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime);
					m_ClassificationTable[index].m_data_vector_travel_time_per_mile .AddData (
						( pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime) /max(0.01,pVehicle->m_Distance));
				}

				m_ClassificationTable[index].TotalCost   += pVehicle->m_TollDollarCost;
				m_ClassificationTable[index].TotalGeneralizedCost += ( pVehicle->m_TollDollarCost + pVehicle->m_TripTime /60.0f* pVehicle->m_VOT );
				m_ClassificationTable[index].TotalGeneralizedTime += pVehicle->m_TollDollarCost/max(1,pVehicle->m_VOT)*60.0f + pVehicle->m_TripTime ;
				m_ClassificationTable[index].TotalPM  += pVehicle->m_PM;

				m_ClassificationTable[index].emissiondata.Energy += pVehicle->m_EmissionData .Energy;
				m_ClassificationTable[index].emissiondata.CO2 += pVehicle->m_EmissionData .CO2;
				m_ClassificationTable[index].emissiondata.NOX += pVehicle->m_EmissionData .NOX;
				m_ClassificationTable[index].emissiondata.CO += pVehicle->m_EmissionData .CO;
				m_ClassificationTable[index].emissiondata.HC += pVehicle->m_EmissionData .HC;
				m_ClassificationTable[index].emissiondata.PM += pVehicle->m_EmissionData.PM;
				m_ClassificationTable[index].emissiondata.PM2_5 += pVehicle->m_EmissionData.PM2_5;


			}

		}

	}

	//variability
	for (iVehicle = m_VehicleSet.begin(); iVehicle != m_VehicleSet.end(); iVehicle++)
	{
		DTAVehicle* pVehicle = (*iVehicle);


		if(SelectVehicleForAnalysis(pVehicle, m_VehicleSelectionMode) == true) 
		{
			int index = FindClassificationNo(pVehicle,x_classfication);

			if(index>=0)  // -1 will not be consideded
			{
				float deviation = 0;
				float travel_time_per_mile_deviation = 0;


				if(vehicle_selection== CLS_path_partial_trip)  //  subpath
				{
					deviation = (pVehicle->m_path_travel_time)- 
						(m_ClassificationTable[index].TotalTravelTime/max(1,m_ClassificationTable[index].TotalVehicleSize));


					travel_time_per_mile_deviation = 
						(pVehicle->m_path_travel_time )/max(0.001,pVehicle->m_path_distance  ) -  
						(m_ClassificationTable[index].TotalTravelTimePerMile/max(1,m_ClassificationTable[index].TotalVehicleSize));
				}else if(vehicle_selection== CLS_subarea_internal_to_internal_subtrip || vehicle_selection == CLS_subarea_boundary_to_bounary_subtrip)  // subarea partial path
				{
					deviation = (pVehicle->m_subarea_travel_time )- 
						(m_ClassificationTable[index].TotalTravelTime/max(1,m_ClassificationTable[index].TotalVehicleSize));


					travel_time_per_mile_deviation = 
						(pVehicle->m_subarea_travel_time )/max(0.001,pVehicle->m_subarea_distance  ) -  
						(m_ClassificationTable[index].TotalTravelTimePerMile/max(1,m_ClassificationTable[index].TotalVehicleSize));

				}else
				{
					deviation = (pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime)- 
						(m_ClassificationTable[index].TotalTravelTime/max(1,m_ClassificationTable[index].TotalVehicleSize));

					travel_time_per_mile_deviation = 
						(pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime)/max(0.001,pVehicle->m_Distance ) -  
						(m_ClassificationTable[index].TotalTravelTimePerMile/max(1,m_ClassificationTable[index].TotalVehicleSize));
				}

				m_ClassificationTable[index].TotalTravelTimeVariance   +=  deviation*deviation;
				m_ClassificationTable[index].TotalTravelTimePerMileVariance   +=  travel_time_per_mile_deviation*travel_time_per_mile_deviation;

			}

		}

	}
}


void CTLiteDoc::GenerateClassificationForDisplay(VEHICLE_X_CLASSIFICATION x_classfication, VEHICLE_Y_CLASSIFICATION y_classfication)
{
	// input: 	m_ClassificationTable
	// output: label and display value

	std::map< int, VehicleStatistics >::iterator iter;

	for ( iter = m_ClassificationTable.begin(); iter != m_ClassificationTable.end(); iter++ )
	{
		int index = iter->first;
		float value= 0;

		m_ClassificationTable[index].Label = FindClassificationLabel(x_classfication,index);

		// enum VEHICLE_Y_CLASSIFICATION {CLS_vehicle_count,CLS_total_travel_time,CLS_avg_travel_time,CLS_total_toll_cost,CLS_avg_toll_cost,CLS_total_generalized_cost,CLS_avg_generalized_cost,CLS_total_travel_distance, CLS_avg_travel_distance,CLS_total_CO2,CLS_avg_CO2};


		switch(y_classfication)
		{
		case CLS_vehicle_count: 
			value = m_ClassificationTable[index].TotalVehicleSize;
			break;
		case CLS_cumulative_vehicle_count: 

			value  = 0;
			for(int ii = 0; ii <= index; ii++)
				if(m_ClassificationTable.find(ii) != m_ClassificationTable.end())
				{
					value += m_ClassificationTable[ii].TotalVehicleSize;
				}
				break;

		case CLS_total_travel_time: 
			value = m_ClassificationTable[index].TotalTravelTime ;
			break;
		case CLS_avg_travel_time: 
			value = m_ClassificationTable[index].TotalTravelTime/max(1,m_ClassificationTable[index].TotalVehicleSize);
			break;

		case CLS_travel_time_95_percentile: 
			value = m_ClassificationTable[index].m_data_vector_travel_time .GetDataVectorPercentage (95);
			break;

		case CLS_travel_time_90_percentile: 
			value = m_ClassificationTable[index].m_data_vector_travel_time .GetDataVectorPercentage (90);
			break;

		case CLS_travel_time_80_percentile: 
			value = m_ClassificationTable[index].m_data_vector_travel_time .GetDataVectorPercentage (80);
			break;

		case CLS_travel_time_per_mile_95_percentile: 
			value = m_ClassificationTable[index].m_data_vector_travel_time_per_mile .GetDataVectorPercentage (95);
			break;

		case CLS_travel_time_per_mile_90_percentile: 
			value = m_ClassificationTable[index].m_data_vector_travel_time_per_mile .GetDataVectorPercentage (90);
			break;

		case CLS_travel_time_per_mile_80_percentile: 
			value = m_ClassificationTable[index].m_data_vector_travel_time_per_mile .GetDataVectorPercentage (80);
			break;
			//Buffer Index = (95th percentile travel time ?mean travel time) / mean travel time
		case CLS_travel_time_Buffer_Index: 
			value = (m_ClassificationTable[index].m_data_vector_travel_time .GetDataVectorPercentage (95)- m_ClassificationTable[index].m_data_vector_travel_time.GetDataVectorMean ())/max(0.01,m_ClassificationTable[index].m_data_vector_travel_time.GetDataVectorMean ());
			break;
			//Skew Index = (90th percentile travel time ?median travel time) / (median travel time ?10th percentile travel time)
		case CLS_travel_time_Skew_Index: 
			value = (m_ClassificationTable[index].m_data_vector_travel_time .GetDataVectorPercentage (90)- m_ClassificationTable[index].m_data_vector_travel_time.GetDataVectorPercentage(50))/
				max(0.01,m_ClassificationTable[index].m_data_vector_travel_time.GetDataVectorPercentage (50) -m_ClassificationTable[index].m_data_vector_travel_time.GetDataVectorPercentage (10) );
			break;
		case CLS_travel_time_Travel_Time_Index:  // mean of travel time distribution/free flow travel time for a given path
			value = m_ClassificationTable[index].TotalTravelTime/max(0.01,m_ClassificationTable[index].TotalFreeflowTravelTime);
			break;
		case CLS_travel_time_Planning_Time_Index: //95th percentile of travel time distribution/free flow travel time for a given path
			value = m_ClassificationTable[index].m_data_vector_travel_time .GetDataVectorPercentage (95)/max(0.01,m_ClassificationTable[index].TotalFreeflowTravelTime/m_ClassificationTable[index].TotalVehicleSize);
			break;
		case CLS_total_toll_cost: 
			value = m_ClassificationTable[index].TotalCost ;
			break;
		case CLS_avg_toll_cost: 
			value = m_ClassificationTable[index].TotalCost /max(1,m_ClassificationTable[index].TotalVehicleSize); // *100, dollar
			break;

		case CLS_total_generalized_cost: 
			value = m_ClassificationTable[index].TotalGeneralizedCost  ;
			break;
		case CLS_total_generalized_travel_time: 
			value = m_ClassificationTable[index].TotalGeneralizedTime  ;
			break;
		case CLS_avg_generalized_cost: 
			value = m_ClassificationTable[index].TotalGeneralizedCost /max(1,m_ClassificationTable[index].TotalVehicleSize);
			break;
		case CLS_avg_generalized_travel_time:
			value = m_ClassificationTable[index].TotalGeneralizedTime /max(1,m_ClassificationTable[index].TotalVehicleSize);
			break;

		case CLS_total_travel_distance: 
			value = m_ClassificationTable[index].TotalDistance   ;
			break;
		case CLS_avg_travel_distance: 
			value = m_ClassificationTable[index].TotalDistance /max(1,m_ClassificationTable[index].TotalVehicleSize);
			break;
		case CLS_avg_speed: 
			value = m_ClassificationTable[index].TotalDistance /max(0.1,m_ClassificationTable[index].TotalTravelTime/60.0f);
			break;
		case CLS_travel_time_STD: 
			value = sqrt(m_ClassificationTable[index].TotalTravelTimeVariance /max(1,m_ClassificationTable[index].TotalVehicleSize));
			break;
		case CLS_travel_time_per_mile_STD: 
			value = sqrt(m_ClassificationTable[index].TotalTravelTimePerMileVariance /max(1,m_ClassificationTable[index].TotalVehicleSize));
			break;
		case CLS_avg_travel_time_per_mile: 
			value = m_ClassificationTable[index].TotalTravelTimePerMile /max(1,m_ClassificationTable[index].TotalVehicleSize);
			break;
		case CLS_total_Energy: 
			value = m_ClassificationTable[index].emissiondata .Energy     ;
			break;
		case CLS_avg_Energy: 
			value = m_ClassificationTable[index].emissiondata .Energy   /max(1,m_ClassificationTable[index].TotalVehicleSize);
			break;
		case CLS_avg_Energy_per_mile: 
			value = m_ClassificationTable[index].emissiondata .Energy   /max(1,m_ClassificationTable[index].TotalDistance);
			break;

		case CLS_total_CO2: 
			value = m_ClassificationTable[index].emissiondata .CO2    ;
			break;
		case CLS_avg_CO2: 
			value = m_ClassificationTable[index].emissiondata .CO2  /max(1,m_ClassificationTable[index].TotalVehicleSize);
			break;
		case CLS_avg_CO2_per_mile: 
			value = m_ClassificationTable[index].emissiondata .CO2  /max(1,m_ClassificationTable[index].TotalDistance);
			break;
		case CLS_total_NOx: 
			value = m_ClassificationTable[index].emissiondata .NOX     ;
			break;
		case CLS_avg_Nox: 
			value = m_ClassificationTable[index].emissiondata .NOX  /max(1,m_ClassificationTable[index].TotalVehicleSize);
			break;
		case CLS_avg_Nox_per_mile: 
			value = m_ClassificationTable[index].emissiondata .NOX  /max(1,m_ClassificationTable[index].TotalDistance);
			break;
		case CLS_total_CO: 
			value = m_ClassificationTable[index].emissiondata .CO     ;
			break;
		case CLS_avg_CO: 
			value = m_ClassificationTable[index].emissiondata .CO  /max(1,m_ClassificationTable[index].TotalVehicleSize);
			break;
		case CLS_avg_CO_per_mile: 
			value = m_ClassificationTable[index].emissiondata .CO 	/max(1,m_ClassificationTable[index].TotalDistance);
			break;
		case CLS_total_HC: 
			value = m_ClassificationTable[index].emissiondata .HC     ;
			break;

		case CLS_total_PM:
			value = m_ClassificationTable[index].emissiondata.PM;
			break;

		case CLS_total_PM2_5:
			value = m_ClassificationTable[index].emissiondata.PM2_5;
			break;

		case CLS_avg_HC: 
			value = m_ClassificationTable[index].emissiondata .HC  /max(1,m_ClassificationTable[index].TotalVehicleSize);
			break;
		case CLS_avg_HC_per_mile: 
			value = m_ClassificationTable[index].emissiondata .HC  /max(1,m_ClassificationTable[index].TotalDistance);
			break;


		case CLS_avg_PM:
			value = m_ClassificationTable[index].emissiondata.PM / max(1, m_ClassificationTable[index].TotalVehicleSize);
			break;
		case CLS_avg_PM_per_mile:
			value = m_ClassificationTable[index].emissiondata.PM / max(1, m_ClassificationTable[index].TotalDistance);
			break;

		case CLS_avg_PM2_5:
			value = m_ClassificationTable[index].emissiondata.PM2_5 / max(1, m_ClassificationTable[index].TotalVehicleSize);
			break;
		case CLS_avg_PM2_5_per_mile:
			value = m_ClassificationTable[index].emissiondata.PM2_5 / max(1, m_ClassificationTable[index].TotalDistance);
			break;

		case CLS_total_gallon: 
			value = m_ClassificationTable[index].emissiondata .Energy/1000/(121.7)     ;
			break;
		case CLS_avg_mile_per_gallon: 
			value = m_ClassificationTable[index].TotalDistance  /max(1,m_ClassificationTable[index].emissiondata .Energy/1000/(121.7));
			break;


		default: 
			value = 0;
			TRACE("ERROR. No classification available!");
		};

		m_ClassificationTable[index].DisplayValue = value;

	}
}


void CTLiteDoc::OnLinkVehiclestatisticsanalaysis()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	m_VehicleSelectionMode = CLS_link_set;  // select link analysis

	CDlg_VehicleClassification dlg;

	dlg.m_VehicleSelectionNo = CLS_link_set;
	dlg.m_pDoc = this;
	dlg.DoModal ();

}



void CTLiteDoc::OnSubareaDeletesubarea()
{
	m_SubareaShapePoints.clear ();
	UpdateAllViews(0);
}

void CTLiteDoc::OnSubareaViewvehiclestatisticsassociatedwithsubarea()
{
	// TODO: Add your command handler code here
}

void CTLiteDoc::OnToolsTraveltimereliabilityanalysis()
{

	//std::vector<float> LinkCapacity;
	//std::vector<float> LinkTravelTime;

	//LinkCapacity.push_back(1800.0f);
	//LinkTravelTime.push_back(20.0f);

	//CDlg_TravelTimeReliability dlg;
	//dlg.m_pDoc= this;
	//dlg.LinkCapacity = LinkCapacity;
	//dlg.LinkTravelTime = LinkTravelTime;

	//dlg.m_BottleneckIdx = 0;
	//dlg.m_PathFreeFlowTravelTime = 20.0f;  // for a predefined corridor
	//dlg.DoModal ();

	m_LinkMOEMode = MOE_reliability;
	m_LinkBandWidthMode = LBW_link_volume;
	ShowLegend(true);
	GenerateOffsetLinkBand();
	UpdateAllViews(0);

}


void CTLiteDoc::OnLinkLinkbar()
{

	CDlg_LinkVisualizationConfig dlg;
	dlg.m_pDoc= this;
	dlg.m_link_band_width_mode = m_LinkBandWidthMode;
	if(dlg.DoModal() == IDOK)
	{
		m_LinkBandWidthMode = dlg.m_link_band_width_mode;
		GenerateOffsetLinkBand();
		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnImportArcgisshapefile()
{
#ifndef _WIN64

#else 
	AfxMessageBox("NEXTA 64-bit version does not support shape file importing function. Please use NEXTA_32.exe.");
	RunNEXTA_32();
	return;
#endif

	CDlg_GISDataExport dlg;
	dlg.m_pDoc = this;
	dlg.DoModal();
	//

	//m_UnitFeet = 1;  // default value
	//m_NodeDisplaySize = 50;
	CalculateDrawingRectangle();
	m_bFitNetworkInitialized  = false;
	UpdateAllViews(0);


	/*	CString str;
	CFileDialog dlg (FALSE, "*.shp", "*.shp",OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_LONGNAMES,
	"Shape File (*.shp)|*.shp||", NULL);
	if(dlg.DoModal() == IDOK)
	{

	CWaitCursor wait;
	ImportOGRShapeFile(dlg.GetPathName());
	}
	*/
}



void CTLiteDoc::OnLinkIncreaseoffsetfortwo()
{
	std::list<DTALink*>::iterator iLink;

	double minimum_link_length = 999999;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		if((*iLink) -> m_bToBeShifted)
		{
			if((*iLink)->m_Length <  minimum_link_length)
				minimum_link_length = (*iLink)->m_Length ;
		}
	}

	double min_offset_value = max(minimum_link_length*0.01*5280, 2);
	m_OffsetInFeet += min_offset_value;
	m_bLinkToBeShifted  = true;
	OffsetLink();  // offset shape points
	GenerateOffsetLinkBand();  // from shape points to reference points
	UpdateAllViews(0);
}

void CTLiteDoc::OnLinkDecreaseoffsetfortwo()
{
	std::list<DTALink*>::iterator iLink;

	double minimum_link_length = 999999;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		if((*iLink) -> m_bToBeShifted)
		{
			if((*iLink)->m_Length <  minimum_link_length)
				minimum_link_length = (*iLink)->m_Length ;
		}
	}

	double min_offset_value = max(minimum_link_length*0.01*5280, 2);
	m_OffsetInFeet -= min_offset_value;

	//if(m_OffsetInFeet<0)
	//	m_OffsetInFeet = 0;


	//	m_OffsetInFeet = max (5,m_OffsetInFeet);  // minimum 5 feet
	m_bLinkToBeShifted  = true;
	OffsetLink();
	GenerateOffsetLinkBand();
	UpdateAllViews(0);
}

void CTLiteDoc::OnLinkNooffsetandnobandwidth()
{
	std::list<DTALink*>::iterator iLink;

	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		(*iLink)->m_ShapePoints.clear();

		(*iLink)->m_ShapePoints .push_back ((*iLink)->m_FromPoint);
		(*iLink)->m_ShapePoints .push_back ((*iLink)->m_ToPoint);
	}

	m_OffsetInFeet=10;
	m_bLinkToBeShifted  = true;
	m_LaneWidthInFeet = 2;
	OffsetLink();
	GenerateOffsetLinkBand();
	UpdateAllViews(0);
}

void CTLiteDoc::OnViewShowhideLegend()
{
	m_bShowLegend = !m_bShowLegend;

	ShowLegend(m_bShowLegend);
}

void CTLiteDoc::ShowLegend(bool ShowLegendStatus)
{
	m_bShowLegend = ShowLegendStatus;
	if(m_bShowLegend)
	{
		if(g_pLegendDlg==NULL)
		{
			g_pLegendDlg = new CDlg_Legend();
			g_pLegendDlg->m_pDoc = this;
			g_pLegendDlg->Create(IDD_DIALOG_Legend);
		}

		// update using pointer to the active document; 

		if(g_pLegendDlg->GetSafeHwnd())
		{
			g_pLegendDlg->m_pDoc = this;
			g_pLegendDlg->ShowWindow(SW_HIDE);
			g_pLegendDlg->ShowWindow(SW_SHOW);
		}
	}else
	{
		if(g_pLegendDlg!=NULL && g_pLegendDlg->GetSafeHwnd())
		{
			g_pLegendDlg->ShowWindow(SW_HIDE);
		}
	}
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	pMainFrame->UpdateLegendView();

}
void CTLiteDoc::OnMoeViewlinkmoesummaryfile()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_LinkMOE.csv");
}

void CTLiteDoc::OnViewCalibrationview()
{
	m_bShowCalibrationResults = !m_bShowCalibrationResults;

	if(m_bShowCalibrationResults == true)
	{
		m_LinkMOEMode = MOE_volume;
		OnMoeViewoddemandestimationsummaryplot();
		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnUpdateViewCalibrationview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowCalibrationResults);
}

void CTLiteDoc::OnMoeViewtrafficassignmentsummaryplot()
{
	NetworkLoadingOutput element;
	CCSVParser parser;

	CString str = m_ProjectDirectory +"output_assignment_log.csv";
	CT2CA pszConvertedAnsiString (str);

	// construct a std::string using the LPCSTR input
	std::string  strStd (pszConvertedAnsiString);

	if (parser.OpenCSVFile(strStd))
	{
		COLORREF crColor1 = RGB (255, 0, 0);
		COLORREF crColor2 = RGB (0, 0, 255);
		COLORREF crColor3 = RGB (0, 128, 0);

		CLinePlotData element_avg_travel_time_in_min;
		element_avg_travel_time_in_min.crPlot = crColor1;
		element_avg_travel_time_in_min.szName  = "Avg Travel Time (min)";

		CLinePlotData element_avg_travel_distance_in_mile;
		element_avg_travel_distance_in_mile.crPlot = crColor2;
		element_avg_travel_distance_in_mile.szName  = "Avg Distance ";

		CLinePlotData element_avg_travel_time_gap_per_vehicle_in_min;
		element_avg_travel_distance_in_mile.crPlot = crColor3;
		element_avg_travel_time_gap_per_vehicle_in_min.szName  = "Avg TT Gap (min)";

		int iteration =0;
		while(parser.ReadRecord())
		{
			FLOATPOINT data;
			data.x = iteration;

			if(parser.GetValueByFieldName("avg_travel_time_in_min",data.y) == false)
				break;
			element_avg_travel_time_in_min.vecData.push_back(data);

			if(parser.GetValueByFieldName("avg_travel_distance_in_mile",data.y) == false)
				break;
			element_avg_travel_distance_in_mile.vecData.push_back(data);

			if(iteration!=0)
			{
				if(parser.GetValueByFieldName("avg_travel_time_gap_per_vehicle_in_min",data.y) == false)
					break;
				element_avg_travel_time_gap_per_vehicle_in_min.vecData.push_back(data);
			}

			iteration++;

		}

		if(iteration > 1)
		{

			CLinePlotTestDlg dlg;
			dlg.m_pDoc  = this;

			dlg.m_XCaption = "Iteration";
			dlg.m_YCaption = "Traffic Assignment MOE";
			dlg.m_PlotDataVector.push_back(element_avg_travel_time_in_min);
			dlg.m_PlotDataVector.push_back(element_avg_travel_distance_in_mile);
			dlg.m_PlotDataVector.push_back(element_avg_travel_time_gap_per_vehicle_in_min);

			dlg.DoModal();
		}else
		{
			AfxMessageBox("File output_assignment_log.csv does not have assignment results. Please first run traffic assignment.");

		}
	}else
	{
		AfxMessageBox("File output_assignment_log.csv does not exist. Please first run traffic assignment.");
	}
}

void CTLiteDoc::OnMoeViewoddemandestimationsummaryplot()
{

	CLinePlotTestDlg dlg;
	dlg.m_pDoc  = this;

		COLORREF crColor1 = RGB (100, 149, 237);
		CLinePlotData element_link_volume;
		element_link_volume.crPlot = crColor1;

			element_link_volume.szName  = "Link volume";
			dlg.m_XCaption = "Observed Houly Link Volume";
			dlg.m_YCaption = "Simulated Houly Link Volume";

			element_link_volume.lineType = enum_LpScatter;

		int count =0;
		for (std::list<DTALink*>::iterator iLink  = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			DTALink* pLink = (*iLink);

			if(pLink->m_SensorDataVector.size()>0 )
			{
				for(unsigned i = 0; i< pLink->m_SensorDataVector.size(); i++)
				{
					DTASensorData element = pLink->m_SensorDataVector[i];

					float SensorCount = element.count*60 / (max(1,element.end_time_in_min - element.start_time_in_min));
					if( SensorCount>1)
					{
						float SimulatedCount = pLink->GetAvgLinkHourlyVolume (
							element.start_time_in_min,element.end_time_in_min);
						FLOATPOINT data;

						if (SimulatedCount < -1)
						{
							CString str;
							str.Format("Error: negative SimulatedCount %f for link %d ->%d, %d(min)->%d(min). Please check file sensor_count.csv.",
								SimulatedCount, pLink->m_FromNodeNumber, pLink->m_ToNodeNumber, element.start_time_in_min, element.end_time_in_min);
								AfxMessageBox(str);

							return;
						
						}
						data.x = SensorCount;
						data.y = SimulatedCount;
						data.Hour = int(element.start_time_in_min / 60);

						data.LinkNo = pLink->m_LinkNo ;
						if( m_LinkTypeMap[pLink->m_link_type].IsFreeway ())
						{   
							data.LinkType = _lp_freeway;
							data.crColor  = theApp.m_FreewayColor;

						}

				if( m_LinkTypeMap[pLink->m_link_type].IsArterial  ())
				{
					data.LinkType = _lp_arterial;
					data.crColor = theApp.m_ArterialColor;
				}

				if( m_LinkTypeMap[pLink->m_link_type].IsHighway   ())
				{
					data.LinkType = _lp_ramp;
					data.crColor = theApp.m_ArterialColor;
				}


				if (element.start_time_in_min >= m_DemandLoadingStartTimeInMin && element.end_time_in_min <= m_DemandLoadingEndTimeInMin)
				{
					element_link_volume.vecData.push_back(data);
				}
				count++;

				}
		}
			}
		}

		if(count >=1)
		{

			dlg.m_PlotDataVector.push_back(element_link_volume);
			dlg.DoModal();

		}else
		{
			AfxMessageBox("No matching sensor data are available from file sensor_count.csv. Please prepare file sensor_count.csv.");
		}
}

void CTLiteDoc::OnMoeViewoddemandestimationsummaryplotLanedensity()
{
	CLinePlotTestDlg dlg;
	dlg.m_pDoc  = this;

		COLORREF crColor1 = RGB (100, 149, 237);
		CLinePlotData element_lane_density;
		element_lane_density.crPlot = crColor1;

			element_lane_density.szName  = "Lane Density";
			dlg.m_XCaption = "Observed Lane Density";
			dlg.m_YCaption = "Simulated Lane Density";

			element_lane_density.lineType = enum_LpScatter;

		int count =0;
		for (std::list<DTALink*>::iterator iLink  = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			DTALink* pLink = (*iLink);

			if(pLink->m_SensorDataVector.size()>0 )
			{
				for(unsigned i = 0; i< pLink->m_SensorDataVector.size(); i++)
				{
					DTASensorData element = pLink->m_SensorDataVector[i];

					float SensorDensity  = element.density ;
					if( SensorDensity>1)
					{
						float max_density = 300;
						float avg_density = pLink->GetSimulatedDensityMOE(element.start_time_in_min,element.end_time_in_min,max_density);
						FLOATPOINT data;

						data.x = SensorDensity;
						data.y = avg_density;

						data.LinkNo = pLink->m_LinkNo ;
						if( m_LinkTypeMap[pLink->m_link_type].IsFreeway ())
						{   
							data.LinkType = _lp_freeway;
							data.crColor  = theApp.m_FreewayColor;

						}

				if( m_LinkTypeMap[pLink->m_link_type].IsArterial  ())
				{
					data.LinkType = _lp_arterial;
					data.crColor = theApp.m_ArterialColor;
				}

				if( m_LinkTypeMap[pLink->m_link_type].IsHighway   ())
				{
					data.LinkType = _lp_ramp;
					data.crColor = theApp.m_ArterialColor;
				}

				if (element.start_time_in_min >= m_DemandLoadingStartTimeInMin && element.end_time_in_min <= m_DemandLoadingEndTimeInMin)
				{
					element_lane_density.vecData.push_back(data);
				}
				count++;

				}
		}
			}
		}

		if(count >=1)
		{

			dlg.m_PlotDataVector.push_back(element_lane_density);
			dlg.DoModal();

		}else
		{
			AfxMessageBox("No matching sensor data are available from file sensor_count.csv. Please prepare lane density data in file sensor_count.csv.");
		}
}

void CTLiteDoc::OnProjectEditpricingscenariodata()
{
	CDlg_PricingConfiguration dlg;
	dlg.m_pDoc = this;
	if(dlg.DoModal() ==IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
}

void CTLiteDoc::OnLinkViewlink()
{
	CDlgScenario dlg(0);
	dlg.m_pDoc = this;

	//dlg.m_SelectTab = 0;
	dlg.DoModal();
}

void CTLiteDoc::OnDeleteSelectedLink()
{
	if(m_SelectedLinkNo == -1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}
	DeleteLink(m_SelectedLinkNo);
	m_SelectedLinkNo = -1;
	UpdateAllViews(0);

}


void CTLiteDoc::OnImportRegionalplanninganddtamodels()
{
	CDlg_ImportPlanningModel dlg;
	dlg.DoModal ();
}

void CTLiteDoc::OnExportGenerateTravelTimeMatrix()
{

	CDlg_UserInput dlg_aggregation_interval;

	g_MOEAggregationIntervalInMin = 60;

	dlg_aggregation_interval.m_StrQuestion  = "Please specify aggregation time interval in min:";
	dlg_aggregation_interval.m_InputValue = "60";

	if(dlg_aggregation_interval.DoModal ()!=IDOK)
		return;

	int aggregation_time_interval = atoi(dlg_aggregation_interval.m_InputValue);

	if(aggregation_time_interval<1)
		aggregation_time_interval = 1;
	CWaitCursor wait;


	std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
	int project_index = 0 ;
	while (iDoc != g_DocumentList.end())
	{
		if((*iDoc)->m_ProjectTitle.GetLength () >0)  
		{

			std::list<DTAVehicle*>::iterator iVehicle;

			for (iVehicle = (*iDoc)->m_VehicleSet.begin(); iVehicle != (*iDoc)->m_VehicleSet.end(); iVehicle++)
			{
				DTAVehicle* pVehicle = (*iVehicle);

				if(pVehicle->m_NodeSize >= 2 && pVehicle->m_bComplete)  // with physical path in the network
				{	

					CString label;
					int departure_time_interval = pVehicle->m_DepartureTime / aggregation_time_interval;
					// reuse label as OD label
					label.Format("%d,%d,%d", departure_time_interval,pVehicle->m_OriginZoneID  ,pVehicle->m_DestinationZoneID);
					m_ODMatrixMap[label].Origin = pVehicle->m_OriginZoneID;
					m_ODMatrixMap[label].Destination  = pVehicle->m_DestinationZoneID;
					m_ODMatrixMap[label].TotalVehicleSize+=1;
					m_ODMatrixMap[label].TotalVehicleSizeVector[project_index]+=1;
					m_ODMatrixMap[label].TotalTravelTimeVector[project_index]  += pVehicle->m_TripTime ;
				}
			}
			project_index++;
		}
		iDoc++;
	}




	CString directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);
	FILE* st;
	fopen_s(&st,directory+"output_travel_time_matrix.csv","w");
	if(st!=NULL)
	{
		fprintf(st,"departure_time_in_min,time_stamp,from_zone_id,to_zone_id,");

		iDoc = g_DocumentList.begin ();
		project_index = 0;
		while (iDoc != g_DocumentList.end())
		{

			if((*iDoc)->m_ProjectTitle.GetLength () >0)
			{
				fprintf(st,"departure_time_in_min,time_stamp,%d,%s,%s,,,,",  project_index+1, (*iDoc)->m_ProjectDirectory, (*iDoc)->m_ProjectTitle);
				project_index++;
			}
			iDoc++;
		}

		fprintf(st,"\n");

		fprintf(st,"departure_time_in_min,time_stamp,from_zone_id,to_zone_id,");

		iDoc = g_DocumentList.begin ();
		project_index = 0;

		while (iDoc != g_DocumentList.end())
		{
			if((*iDoc)->m_ProjectTitle.GetLength () >0)
			{

				if(project_index == 0)
				{
					fprintf(st,"number_of_vehicles,avg_travel_time (min),total_travel_time (min),");
				}else
				{ // 		project_index >=1
					fprintf(st,"number_of_vehicles,avg_travel_time (min),total_travel_time (min),difference_in_number_of_vehicles,difference_of_avg_travel_time,diff_total_travel_time,");		
				}

				project_index ++;
			}

			iDoc++;
		}

		fprintf(st,"\n");


		std::map<int, DTAZone>	:: const_iterator itrFrom;
		std::map<int, DTAZone>	:: const_iterator itrTo;

		for(itrFrom = m_ZoneMap.begin(); itrFrom != m_ZoneMap.end(); itrFrom++)
		{
			for(itrTo = m_ZoneMap.begin(); itrTo != m_ZoneMap.end(); itrTo++)
			{
				int  centroid_pair_size  = 0;
				float total_travel_time  = 0;

				//for(int i = 0; i< itrFrom->second.m_ActivityLocationVector .size(); i++)
				//{
				//	for(int j = 0; j< itrTo->second.m_ActivityLocationVector .size(); j++)
				//	{
				//		int FromNodeNo = m_NodeNumbertoNodeNoMap[itrFrom->second.m_ActivityLocationVector[i].NodeNumber];
				//		int ToNodeNo =  m_NodeNumbertoNodeNoMap[itrTo->second.m_ActivityLocationVector[j].NodeNumber];

				//		float TotalCost = 0;
				//		m_Network.SimplifiedTDLabelCorrecting_DoubleQueue(FromNodeNo, 0, ToNodeNo, 1, 10.0f,PathLinkList,TotalCost, false, false, false,0);   // Pointer to previous node (node)

				//		if(TotalCost<999)  // feasible cost
				//		{
				//		total_travel_time += TotalCost;
				//		centroid_pair_size++;
				//		}

				//	} // per centroid in destination zone
				//} // per centroid in origin zone


				for(int t= this->m_DemandLoadingStartTimeInMin; t<=this->m_DemandLoadingEndTimeInMin; t+= aggregation_time_interval)
				{

					CString label;
					// reuse label as OD label
					int departure_time_interval = t/aggregation_time_interval;
					label.Format("%d,%d,%d", departure_time_interval, itrFrom->first  ,  itrTo->first);

					if(m_ODMatrixMap.find(label) != m_ODMatrixMap.end())
					{

						float avg_travel_time = max(0.5,total_travel_time / max(1,centroid_pair_size));

						fprintf(st, "%d,%s,%d,%d,", t,GetTimeStampString24HourFormat (t), itrFrom->first , itrTo->first);

						project_index = 0;
						iDoc = g_DocumentList.begin ();	

						while (iDoc != g_DocumentList.end())
						{
							if((*iDoc)->m_ProjectTitle.GetLength () >0)  
							{
								fprintf(st, "%d,%5.2f,%5.2f,", 
									m_ODMatrixMap[label].TotalVehicleSizeVector[project_index] , 

									m_ODMatrixMap[label].TotalTravelTimeVector[project_index]/max(1,m_ODMatrixMap[label].TotalVehicleSizeVector[project_index]),

									m_ODMatrixMap[label].TotalTravelTimeVector[project_index]);

								if(project_index >=1)
								{
									fprintf(st, "%d,%5.2f,%5.2f,", 
										m_ODMatrixMap[label].TotalVehicleSizeVector[project_index] - m_ODMatrixMap[label].TotalVehicleSizeVector[0] , 

										m_ODMatrixMap[label].TotalTravelTimeVector[project_index]/max(1,m_ODMatrixMap[label].TotalVehicleSizeVector[project_index]) - m_ODMatrixMap[label].TotalTravelTimeVector[0]/max(1,m_ODMatrixMap[label].TotalVehicleSizeVector[0]),

										m_ODMatrixMap[label].TotalTravelTimeVector[project_index] - m_ODMatrixMap[label].TotalVehicleSizeVector[0] 
									);
								}

								project_index++;
							}
							iDoc++;

							if(project_index  >= _TOTAL_NUMBER_OF_PROJECTS )
								break;
						}

						fprintf(st,"\n");


					}


				} // per time interval
			} //per origin zone

		}//per destination zone

		fclose(st);
	}else
	{
		CString msg;
		msg.Format("File %s cannot be opened.\nIt might be currently used and locked by EXCEL.",directory+"output_travel_time_matrix.csv");
		AfxMessageBox(msg);
		return;
	}

	OpenCSVFileInExcel(directory+"output_travel_time_matrix.csv");
}

void CTLiteDoc::OnExportGenerateshapefiles()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project directory has not been specified. Please save the project to a new folder first.");
		OnFileSaveProjectAs();
		return;
	}

	CWaitCursor wc;

	//DeleteFile(directory+"AMS_node.shp");
	//DeleteFile(directory+"AMS_node.dbf");
	//DeleteFile(directory+"AMS_node.shx");
	//ExportNodeLayerToGISFiles(directory+"AMS_node.shp","ESRI Shapefile");

	//DeleteFile(directory+"AMS_link.shp");
	//DeleteFile(directory+"AMS_link.dbf");
	//DeleteFile(directory+"AMS_link.shx");
	//ExportLinkLayerToGISFiles(directory+"AMS_link.shp","ESRI Shapefile");

	//DeleteFile(directory+"AMS_node.kmz");
	//ExportNodeLayerToGISFiles(directory+"AMS_node.kml","KML");

	m_bExport_Link_MOE_in_input_link_CSF_File = false;
	OnFileSaveProject();  // save time-dependent MOE to input_link MOE file

	CDlg_KML_Configuration dlg;
	dlg.m_pDoc  = this;
	if(dlg.DoModal () == IDOK)
	{

	}



	//	OGDF_WriteGraph(directory+"graph.gml");



	//ExportLink3DLayerToKMLFiles_ColorCode(directory+"AMS_link_green_2D.kml","LIBKML",0,false,1);
	//ExportLink3DLayerToKMLFiles_ColorCode(directory+"AMS_link_green_3D.kml","LIBKML",0,false,-1);
	//ExportLink3DLayerToKMLFiles_ColorCode(directory+"AMS_link_red_3D.kml","LIBKML",1,false,-1);
	//ExportLink3DLayerToKMLFiles_ColorCode(directory+"AMS_link_blue_3D.kml","LIBKML",2,false,-1);
	//ExportLink3DLayerToKMLFiles_ColorCode(directory+"AMS_link_yellow_3D.kml","LIBKML",3,false,-1);

	//ExportLink3DLayerToKMLFiles_ColorCode(directory+"AMS_link_green_3D_SL.kml","LIBKML",0,true,-1);
	//ExportLink3DLayerToKMLFiles_ColorCode(directory+"AMS_link_red_3D_SL.kml","LIBKML",1,true,-1);
	//ExportLink3DLayerToKMLFiles_ColorCode(directory+"AMS_link_blue_3D_SL.kml","LIBKML",2,true,-1);
	//ExportLink3DLayerToKMLFiles_ColorCode(directory+"AMS_link_yellow_3D_SL.kml","LIBKML",3,true,-1);

	//DeleteFile(directory+"AMS_agent.kmz");
	//ExportAgentLayerToKMLFiles(directory+"AMS_agent.kml","KML");

	//	OnToolsProjectfolder();

}
void CTLiteDoc::OnExportGenerateshapefilesPathData()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project directory has not been specified. Please save the project to a new folder first.");
		OnFileSaveProjectAs();
		return;
	}

	CWaitCursor wc;

	CString directory;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);


	m_bExport_Link_MOE_in_input_link_CSF_File = false;
	OnFileSaveProject();  // save time-dependent MOE to input_link MOE file

	CString file = directory+"Corridor_link_3D.kml";
	DeleteFile(file);
	ExportPathLink3DLayerToKMLFiles(file,"LIBKML");
	HINSTANCE result = ShellExecute(NULL, _T("open"), file, NULL,NULL, SW_SHOW);

	//DeleteFile(directory+"AMS_agent.kmz");
	//ExportAgentLayerToKMLFiles(directory+"AMS_agent.kml","KML");

	OnToolsProjectfolder();

}

void CTLiteDoc::OnLinkmoedisplayQueuelength()
{
	m_LinkMOEMode = MOE_queue_length;
	m_LinkBandWidthMode = LBW_number_of_lanes;

	ShowLegend(false);
	GenerateOffsetLinkBand();
	UpdateAllViews(0);

}

void CTLiteDoc::OnUpdateLinkmoedisplayQueuelength(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_queue_length);
}

void CTLiteDoc::OnUpdateLinkmoeTraveltimereliability(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_reliability);
}



void CTLiteDoc::ZoomToSelectedNode(int SelectedNodeNumber)
{
	if(m_NodeNumbertoNodeNoMap.find(SelectedNodeNumber) != m_NodeNumbertoNodeNoMap.end())
	{
		DTANode* pNode= m_NodeNoMap[m_NodeNumbertoNodeNoMap[SelectedNodeNumber]];
		m_Origin = pNode->pt;
		m_SelectedLinkNo = -1;
		m_SelectedNodeID = pNode->m_NodeNo ;

		CTLiteView* pView = 0;
		POSITION pos = GetFirstViewPosition();
		if(pos != NULL)
		{
			pView = (CTLiteView*) GetNextView(pos);
			if(pView!=NULL)
			{
				pView->m_Origin = m_Origin;
				pView->Invalidate ();
			}
		}

	}
}


void CTLiteDoc::ZoomToSelectedSensor(std::string sensor_id)
{
	if (m_SensorMap.find(sensor_id) != m_SensorMap.end())
	{


			GDPoint pt;
			
			m_Origin = m_SensorMap[sensor_id].pt;
			CTLiteView* pView = 0;
			POSITION pos = GetFirstViewPosition();
			if (pos != NULL)
			{
				pView = (CTLiteView*)GetNextView(pos);
				if (pView != NULL)
				{
					pView->m_Origin = m_Origin;
					pView->Invalidate();
				}
			}


	}

}
	

void CTLiteDoc::ZoomToSelectedLink(int SelectedLinkNo)
{
	if(m_LinkNoMap.find(SelectedLinkNo) != m_LinkNoMap.end())
	{
		DTALink* pLink= m_LinkNoMap [SelectedLinkNo];
		GDPoint pt;
		pt.x = (pLink->m_FromPoint.x + pLink->m_ToPoint.x)/2;
		pt.y = (pLink->m_FromPoint.y + pLink->m_ToPoint.y)/2;
		m_Origin = pt;

		CTLiteView* pView = 0;
		POSITION pos = GetFirstViewPosition();
		if(pos != NULL)
		{
			pView = (CTLiteView*) GetNextView(pos);
			if(pView!=NULL)
			{
				pView->m_Origin = m_Origin;
				pView->Invalidate ();
			}
		}

	}
}

void CTLiteDoc::ShowPathListDlg(bool bShowFlag)
{
	if(bShowFlag)
	{
		if(g_pPathListDlg==NULL)
		{
			g_pPathListDlg = new CDlgPathList();
			g_pPathListDlg->m_pDoc = this;
			g_pPathListDlg->Create(IDD_DIALOG_PATH_LIST);
		}

		// update using pointer to the active document; 

		if(g_pPathListDlg->GetSafeHwnd())
		{
			g_pPathListDlg->m_pDoc = this;
			g_pPathListDlg->ReloadData ();
			g_pPathListDlg->ShowWindow(SW_HIDE);
			g_pPathListDlg->ShowWindow(SW_SHOW);
		}
	}else
	{
		if(g_pPathListDlg!=NULL && g_pPathListDlg->GetSafeHwnd())
		{
			g_pPathListDlg->ShowWindow(SW_HIDE);
		}
	}
}

void CTLiteDoc::OnMoePathlist()
{
	//	m_bShowPathList = !m_bShowPathList;
	ShowPathListDlg(m_bShowPathList);
}


bool g_TestValidDocument(CTLiteDoc* pDoc)
{
	std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
	while (iDoc != g_DocumentList.end())
	{
		if(pDoc == (*iDoc))
			return true;

		iDoc++;
	}

	return false;
}

void CTLiteDoc::OnViewShowmoe()
{
	g_LinkMOEDlgShowFlag = !g_LinkMOEDlgShowFlag;
	if(g_LinkMOEDlgShowFlag)
	{
		if(g_LinkMOEDlg==NULL)
		{
			g_LinkMOEDlg = new CDlgMOE();

			g_LinkMOEDlg->m_TimeLeft = 0 ;
			g_LinkMOEDlg->m_TimeRight = g_Simulation_Time_Horizon ;
			g_LinkMOEDlg->m_pDoc = this;
			g_LinkMOEDlg->SetModelessFlag(TRUE);
			g_LinkMOEDlg->Create(IDD_DIALOG_MOE);

			if(m_SensorSpeedDataLoadingStatus.GetLength () >0)  // load sensor speed data 
			{
				g_LinkMOEDlg->Cur_MOE_type1 = speed_mph;
			}
		}

		if(g_LinkMOEDlg->GetSafeHwnd())
		{
			// assignemnt document pointers
			std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
			while (iDoc != g_DocumentList.end())
			{
				if ((*iDoc)->m_NodeSet.size()>0)
				{
					if(g_LinkMOEDlg->m_pDoc ==NULL)
						g_LinkMOEDlg->m_pDoc = (*iDoc);
					else if((*iDoc)!= g_LinkMOEDlg->m_pDoc)
					{
						g_LinkMOEDlg->m_pDoc2 = (*iDoc);
						g_LinkMOEDlg->m_bDoc2Ready = true;
					}
				}
				iDoc++;
			}
			g_LinkMOEDlg->ShowWindow(SW_HIDE);
			g_LinkMOEDlg->ShowWindow(SW_SHOW);
		}
	}else
	{
		if(g_LinkMOEDlg!=NULL && g_LinkMOEDlg->GetSafeHwnd())
		{
			g_LinkMOEDlg->ShowWindow(SW_HIDE);
		}
	}

}

void CTLiteDoc::OnUpdateViewShowmoe(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(g_LinkMOEDlgShowFlag);
}

void CTLiteDoc::OnFileUploadlinkdatatogooglefusiontable()
{
	AfxMessageBox("Please first save the data set to a folder, and they you can upload input_link.csv to Goolge Fusion Tables.");
}

void CTLiteDoc::On3Viewdatainexcel()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_ODMOE.csv");
}

void CTLiteDoc::On5Viewdatainexcel()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_LinkTDMOE.csv");
}

void CTLiteDoc::OnMoeViewnetworktimedependentmoe()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_NetworkTDMOE.csv");
}

void CTLiteDoc::On2Viewdatainexcel33398()
{
	// TODO: Add your command handler code here
}

void CTLiteDoc::On2Viewnetworkdata()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_NetworkTDMOE.csv");
}

void CTLiteDoc::On3Viewoddatainexcel()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_ODMOE.csv");
}

void CTLiteDoc::OnMoeOpenallmoetables()
{


	OpenCSVFileInExcel(m_ProjectDirectory+"output_summary.csv");
	OpenCSVFileInExcel(m_ProjectDirectory+"output_NetworkTDMOE.csv");
	OpenCSVFileInExcel(m_ProjectDirectory+"output_ODMOE.csv");
	OpenCSVFileInExcel(m_ProjectDirectory+"output_LinkMOE.csv");



	//	OpenCSVFileInExcel(m_ProjectDirectory+"output_NetworkTDMOE.csv");
	//	OpenCSVFileInExcel(m_ProjectDirectory+"output_LinkTDMOE.csv");
}



void CTLiteDoc::OnBnClickedButtonDatabase()
{

}

void CTLiteDoc::OnToolsUnittesting()
{

	if(g_pUnitTestingListDlg==NULL)
	{
		g_pUnitTestingListDlg = new CDlg_UnitTestingList();
		g_pUnitTestingListDlg->m_pDoc = this;
		g_pUnitTestingListDlg->Create(IDD_DIALOG_UNIT_TEST_LIST);
	}

	// update using pointer to the active document; 

	if(g_pUnitTestingListDlg->GetSafeHwnd())
	{
		g_pUnitTestingListDlg->m_pDoc = this;
		g_pUnitTestingListDlg->ShowWindow(SW_HIDE);
		g_pUnitTestingListDlg->ShowWindow(SW_SHOW);
	}
}


void CTLiteDoc::OnViewTraininfo()
{
	// potential memory leak
	CDlgTrainInfo* m_pDlg = NULL; 

	m_pDlg = new CDlgTrainInfo; 

	m_pDlg->m_pDoc = this;
	m_pDlg->SetModelessFlag(true); // voila! this is all it takes to make your dlg modeless!
	m_pDlg->Create(IDD_DIALOG_Train); 
	m_pDlg->ShowWindow(SW_SHOW); 

}

void CTLiteDoc::OnImportAmsdataset()
{

#ifndef _WIN64

	CDlg_Information dlg_info;
	dlg_info.m_StringInfo = "This function imports node/link/zone/centroid/connector GIS files\r\nIt supports importing files from TransCAD, CUBE, VISUM and AIMSUN.\r\nThis function requires 32-bit NEXTA.\r\nThis function requires fours files:\r\n(1) input_node_control_type.csv,\r\n(2) input_link_type.csv,\r\n(3) input_demand_file_list.csv (and related demand files),\r\n(4) import_GIS_settings.csv.";
	dlg_info.m_SampleFileDirectory = "importing_sample_data_sets\\GIS_files\\West_Jordan_from_CUBE";
	dlg_info.m_OnLineDocumentLink = "https://docs.google.com/file/d/0Bw8gtHCvOm7WSDBBamdPTDAwYmc"; 



	if(dlg_info.DoModal() == IDOK)
	{

		CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			_T("Importing Configuration (import_GIS_settings.csv)|import_GIS_settings.csv|"));

		if(dlg_info.m_bLoadGISSampleFile)
		{
			dlg.m_ofn.lpstrInitialDir = dlg_info.m_SampleFileDirectory; 
		}

		if(dlg.DoModal() == IDOK)
		{

			CString directory;
			CString ImportProjectFile = dlg.GetPathName ();
			directory = ImportProjectFile.Left(ImportProjectFile.ReverseFind('\\') + 1);

			m_ProjectDirectory = directory;

			CGIS_Import_DataDlg dlg_import;
			dlg_import.m_pDoc = this;
			if(dlg_import.DoModal () == IDOK)
			{
				ImportingTransportationPlanningDataSet(dlg.GetPathName (),true,true);
				CDlgFileLoading dlg_loading;
				dlg_loading.m_pDoc = this;
				dlg_loading.DoModal ();

				UpdateAllViews(0);

			}
		}

	}
#else
	AfxMessageBox("NEXTA 64-bit version does not support shape file exporting function. Please use NEXTA_32.exe.");
	RunNEXTA_32();
#endif
}

void CTLiteDoc::OnDemandfileOddemandmatrix()
{
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Demand matrix csv (*.csv)|*.csv|"));
	if(dlg.DoModal() == IDOK)
	{
		ReadDemandMatrixFile(dlg.GetPathName(),1);
	}
}

void CTLiteDoc::OnDemandfileHovoddemandmatrix()
{
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Demand matrix csv (*.csv)|*.csv|"));
	if(dlg.DoModal() == IDOK)
	{
		ReadDemandMatrixFile(dlg.GetPathName(),2);
	}
}

void CTLiteDoc::OnDemandfileTruckoddemandmatrix()
{
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Demand matrix csv (*.csv)|*.csv|"));
	if(dlg.DoModal() == IDOK)
	{
		ReadDemandMatrixFile(dlg.GetPathName(),3);
	}
}

void CTLiteDoc::OnDemandfileIntermodaloddemandmatrix()
{
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Demand matrix csv (*.csv)|*.csv|"));
	if(dlg.DoModal() == IDOK)
	{
		ReadDemandMatrixFile(dlg.GetPathName(),4);
	}
}


void CTLiteDoc::OnToolsGeneratephysicalzonecentroidsonroadnetwork()
{
	CWaitCursor cursor;
	// step 1: mark old centroids, remove old activity locations 

	int activity_location_count = 0;
	std::map<int, DTAZone>	:: iterator itr;
	for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
	{
		for(int i = 0; i< itr->second.m_ActivityLocationVector .size(); i++)
		{
			DTAActivityLocation element = itr->second.m_ActivityLocationVector[i];

			activity_location_count++;

			m_NodeNoMap [m_NodeNumbertoNodeNoMap[element.NodeNumber ] ] -> m_CentroidUpdateFlag = 1; // old centroid
		}


	}

	if (activity_location_count == 0)
	{
		AfxMessageBox("No valid activity location has been defined in input_activity_location.csv."); 
	}
	// step 2: create new centriods: find a pair of incoming and outgoing links to centroid, if exist

	std::list<DTALink*>::iterator iLink;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		DTALink * pLink = (*iLink);

		if(pLink==NULL)
			break;
		DTALink * pRevLink = NULL; //reversed link
		unsigned long ReversedLinkKey = GetLinkKey(pLink->m_ToNodeID, pLink->m_FromNodeID);

		int reversed_link_id = 0;
		if ( m_NodeNotoLinkMap.find ( ReversedLinkKey) != m_NodeNotoLinkMap.end())
		{
			pRevLink = m_NodeNotoLinkMap[ReversedLinkKey];
		}

		DTANode* pFromNode = m_NodeNoMap[pLink->m_FromNodeID ];
		DTANode* pToNode = m_NodeNoMap[pLink->m_ToNodeID ];

		if(pFromNode->m_NodeNumber == 220 &&  pToNode->m_NodeNumber == 57185)
		{
			TRACE("%d ->%d\n ",  pFromNode->m_NodeNumber ,  pToNode->m_NodeNumber);
		}

		if(pFromNode->m_CentroidUpdateFlag >=1 && pRevLink != NULL && m_LinkTypeMap[pLink->m_link_type ].IsConnector()) // old centroid and no-external origin node // if it is an external origin node, then we do not need to add nodes
		{
			int zone_number = pFromNode->m_ZoneID ;
			int new_node_number;
			// construct new node number based on from and to node ids
			if(pFromNode->m_NodeNo < pToNode->m_NodeNo)
				new_node_number = pFromNode->m_NodeNo * 10000 + pToNode->m_NodeNo ;  // fromID*10000+ToID
			else
				new_node_number = pToNode->m_NodeNo * 10000 +  pFromNode->m_NodeNo ;  // ToID*10000+fromID

			// add new node and update from_node
			if(m_NodeNumbertoNodeNoMap.find(new_node_number) == m_NodeNumbertoNodeNoMap.end() )
			{
				GDPoint	pt;
				pt.x = (pFromNode->pt.x + pToNode->pt.x) /2;
				pt.y = (pFromNode->pt.y + pToNode->pt.y) /2;

				AddNewNode(pt, new_node_number,false);

				// mark it as activity location.
				DTAActivityLocation element;
				element.ZoneID  = zone_number;
				element.NodeNumber = new_node_number;
				element.External_OD_flag = 0;

				int new_node_id = m_NodeNumbertoNodeNoMap[new_node_number];

				DTANode* pNewNode = m_NodeNoMap [new_node_id ] ;
				m_NodeNotoZoneNameMap[new_node_id] = zone_number;

				pNewNode -> m_ZoneID = zone_number;
				pNewNode ->m_Name == "New_Node";
				pNewNode ->m_External_OD_flag = element.External_OD_flag;

				m_ZoneMap [zone_number].m_ActivityLocationVector .push_back (element);

				//update from node of this link
				pLink->m_FromNodeNumber = new_node_number;
				pLink->m_FromNodeID  = new_node_id;
				pLink->m_FromPoint = pt;
				pLink->m_CentroidUpdateFlag = 1;

				// do not look at the downstream node as centroid again!
				pToNode->m_CentroidUpdateFlag  = 0;

				if( pToNode->m_NodeNumber == 54317)
					TRACE("");

				pLink->m_ShapePoints .clear();
				pLink->m_ShapePointRatios  .clear();
				pLink->m_ShapePoints.push_back (pLink->m_FromPoint);
				pLink->m_ShapePoints.push_back (pLink->m_ToPoint);

				//update downstream node of the reversed link
				pRevLink->m_CentroidUpdateFlag = 1;
				pRevLink->m_ToNodeNumber = new_node_number;
				pRevLink->m_ToNodeID  = new_node_id;
				pRevLink->m_ToPoint = pt;

				pRevLink->m_ShapePoints .clear();
				pRevLink->m_ShapePointRatios  .clear();
				pRevLink->m_ShapePoints.push_back (pLink->m_ToPoint);
				pRevLink->m_ShapePoints.push_back (pLink->m_FromPoint);

			}  //two-way link  from node

			if( pFromNode->m_NodeNumber == 54317)
				TRACE("");
			pFromNode->m_CentroidUpdateFlag+=1; // +1 as to be removed, we "touch" this node

		}  // centriod 

	}  // for each link


	// step 2.1: do not delete adjacent nodes of physical links


	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		DTALink * pLink = (*iLink);
		if(m_LinkTypeMap[pLink->m_link_type ].IsConnector()== false)  // not a connector, so the ajacent node number will still appear in input_link.csv file after generating physical links 
		{

			m_NodeNoMap [pLink->m_FromNodeID ] -> m_CentroidUpdateFlag = 0; // no change
			m_NodeNoMap [pLink->m_ToNodeID ] -> m_CentroidUpdateFlag = 0; // no change


		}

	}
	// step 3: remove old centroids
	std::list<DTANode*>::iterator iNode;
	iNode = m_NodeSet.begin();

	while(iNode != m_NodeSet.end())
	{
		if((*iNode)->m_CentroidUpdateFlag >= 2)
		{
			int ZoneID = (*iNode)->m_ZoneID;

			m_ZoneMap[ZoneID].RemoveNodeActivityMode ((*iNode)->m_NodeNumber);

			m_NodeNoMap[(*iNode)->m_NodeNo ] = NULL;
			m_NodeNumbertoNodeNoMap[(*iNode)->m_NodeNumber  ] = -1;
			iNode = m_NodeSet.erase  (iNode); //  correctly update the iterator to point to the location after the iterator we removed.


		}else
		{
			++iNode;
		}
	}

	// update m_path map 
	m_Movement3NodeMap.clear();

	std::map<CString, PathStatistics> ::iterator itr_path;
	for (itr_path = m_PathMap.begin(); itr_path != m_PathMap.end(); itr_path++)
	{

		// update node numbers
		(*itr_path).second.m_NodeVector.clear();

		int i;
		for(i = 0; i < (*itr_path).second.m_LinkPointerVector.size(); i++)
		{

			DTALink* pLink = (*itr_path).second.m_LinkPointerVector[i];

			if(pLink!=NULL)
			{
				int from_node_number = pLink->m_FromNodeNumber   ;
				int to_node_number = pLink->m_ToNodeNumber ;

				if(i==0) 
				{
					(*itr_path).second.m_NodeVector.push_back(from_node_number);
				}

				(*itr_path).second.m_NodeVector.push_back(to_node_number);
			}


		}


		// update movement counts
		for(i = 0; i < (*itr_path).second.m_NodeVector.size(); i++)
		{
			if(i>=2)
			{
				CString movement_label;
				movement_label.Format ("%d;%d;%d",(*itr_path).second.m_NodeVector[i-2],(*itr_path).second.m_NodeVector[i-1],(*itr_path).second.m_NodeVector[i]);

				m_Movement3NodeMap[movement_label].TotalVehicleSize ++;


			}

		}


		// one vehicle type for now

	}

	GenerateOffsetLinkBand();
	UpdateAllViews(0);
}



void CTLiteDoc::OnNodeIncreasenodetextsize()
{

	m_NodeTextDisplayRatio *=1.1;

	UpdateAllViews(0);

}

void CTLiteDoc::OnNodeDecreasenodetextsize()
{
	m_NodeTextDisplayRatio /=1.1;

	if(m_NodeTextDisplayRatio <2)  
		m_NodeTextDisplayRatio = 2;

	UpdateAllViews(0);
}



void CTLiteDoc::OnImportSynchroutdfcsvfiles()
{
	ReadSynchroUniversalDataFiles();
	bSynchroImportReadyToSaveFlag = true;

	AfxMessageBox("Please first save and reload the data set to check movement attributes", MB_ICONINFORMATION);

}


// added by jun 12/01/18
void CTLiteDoc::OnOutputMovementcsvfiles() {
	//// jun: 12/02/2018
	// save the movement str file, if there is not any 
	FILE* g_pFileMovement = NULL;
	g_pFileMovement = fopen("output_Movement.csv", "w");
	if (g_pFileMovement == NULL)
	{
		cout << "File output_Movement.csv cannot be opened." << endl;
		g_ProgramStop_agentlite();
	}
	else
	{
		fprintf(g_pFileMovement, "node_id, up_node_id, dest_node_id, name, turn_type, turn_direction, prohibited_flag\n");
		std::list<DTANode*>::iterator  iNode;

		for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{
			if ((*iNode)->m_LayerNo == 0)
			{
				for (int tp = 0; tp< m_TimingPlanVector.size(); tp++)  // first loop for each timing plan
				{
					std::string timing_plan_name = m_TimingPlanVector[tp].timing_plan_name;  // fetch timing_plan (unique) name
					
					for (unsigned int m = 0; m < (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector.size(); m++)
					{
						
						DTANodeMovement movement = (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m];

						int curr_node_id = (*iNode)->m_NodeNumber;
						int up_node_id = m_NodeNoMap[movement.in_link_from_node_id]->m_NodeNumber;
						int dest_node_id = m_NodeNoMap[movement.out_link_to_node_id]->m_NodeNumber;
						CString name = "";
						CString trun_type = GetTurnString(movement.movement_turn);
						CString turn_direction = GetTurnDirectionString(movement.movement_approach_turn);
						int prohibited_flag = movement.turning_prohibition_flag;

						fprintf(g_pFileMovement, "%d,%d,%d,%s,%s,%s,%d\n",
							curr_node_id,
							up_node_id,
							dest_node_id,
							name,
							trun_type,
							turn_direction,
							prohibited_flag);
					}
				}
			}
		}


		fclose(g_pFileMovement);
	}
	AfxMessageBox("Successfully save movement string as csv file!", MB_ICONINFORMATION);
}

// end 


void CTLiteDoc::OnProjectEditmoesettings()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"input_MOE_settings.csv");
}

void CTLiteDoc::OnProjectMultiScenarioResults()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_multi_scenario_results.csv");
}

void CTLiteDoc::OnProject12()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_Path.csv");
}

void CTLiteDoc::OnViewMovementMoe()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_MovementMOE.csv");
}

void CTLiteDoc::OnProjectTimeDependentLinkMoe()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_LinkTDMOE.csv");
}

void CTLiteDoc::OnViewOdmeResult()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_ODME_MOE.csv");
}

void CTLiteDoc::OnProjectViewAgentMoe()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"output_agent.csv");
}

void CTLiteDoc::OnProjectOdmatrixestimationinput()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"ODME_Settings.txt");
}

void CTLiteDoc::OnProjectInputsensordataforodme()
{
	OpenCSVFileInExcel(m_ProjectDirectory+"sensor_count.csv");
}


void CTLiteDoc::OnToolsGenerateodmatrixgravitymodel()
{
	if(RunGravityModel() == true)
	{
		SaveAMS_ODTable();
		OnToolsProjectfolder();
	}
}

void CTLiteDoc::ShowTextLabel()
{
}
void CTLiteDoc::OnLinkattributedisplayLinkname()
{
	m_LinkMOEMode = MOE_none;
	ShowTextLabel();
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateLinkattributedisplayLinkname(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_none);
}

void CTLiteDoc::OnToolsGeneratesignalcontrollocations()
{



	CWaitCursor wait;

	int MinimumSpeedLimit4SignalControl  = 30;
	int MaximumSpeedLimit4SignalControl  = 60;


	CDlg_UserInput dlg;
	dlg.m_StrQuestion  = "Please specify the default cycle length in second.";
	dlg.m_InputValue = "100";

	int cycle_length_in_second = 100;
	if(dlg.DoModal ()==IDOK)
	{
		cycle_length_in_second = atoi(dlg.m_InputValue) ;
	}

	std::list<DTANode*>::iterator iNode;
	for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{
		DTANode* pNode = (*iNode);
		pNode->m_ControlType = 0;  // reset control type
		pNode->m_CycleLengthInSecond =0;

	}

	std::list<DTALink*>::iterator iLink;


	int signal_count  = 0;
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		DTALink* pLink = (*iLink);

		if( m_LinkTypeMap[pLink->m_link_type ].IsArterial () == true &&
			pLink->m_SpeedLimit> MinimumSpeedLimit4SignalControl && pLink->m_SpeedLimit<= MaximumSpeedLimit4SignalControl && 
			m_NodeNoMap[pLink->m_ToNodeID ]->m_ControlType == 0 && 
			m_NodeNoMap[pLink->m_ToNodeID ]->m_IncomingNonConnectors >=3 && 
			m_NodeNoMap[pLink->m_ToNodeID ]->m_IncomingNonConnectors == m_NodeNoMap[pLink->m_ToNodeID ]->m_IncomingLinkVector .size()/*no incoming connectors*/) 
		{ // speed range between 30 and 60, arterial streets, intersection has at least 3 legs
			m_NodeNoMap[pLink->m_ToNodeID ]->m_ControlType = m_ControlType_PretimedSignal;  // signal control
			m_NodeNoMap[pLink->m_ToNodeID ]->m_CycleLengthInSecond  = cycle_length_in_second;  // signal control
			signal_count ++;
		}

		if(pLink->m_SpeedLimit<=30 && m_NodeNoMap[pLink->m_ToNodeID ]->m_ControlType == 0)
		{
			m_NodeNoMap[pLink->m_ToNodeID ]->m_ControlType = m_ControlType_4wayStopSign;  // signal control
		}

	}
	CString msg;
	msg.Format("%d nodes are assigned to signal control type. You need to save the data to confirm the changes.", signal_count);
	AfxMessageBox(msg,MB_ICONINFORMATION);
	Modify(true);
	UpdateAllViews(0);
}

void CTLiteDoc::GenerateMovementCountFromVehicleFile(float PeakHourFactor)
{

	CWaitCursor wait;
	m_PeakHourFactor = PeakHourFactor;


	if(m_Movement3NodeMap.size()>=1)
		return;

	if(m_VehicleSet.size()>=50000)
	{
		AfxMessageBox("Generating movement count from vehicle/agent files might take a while...", MB_ICONINFORMATION);

	}

	std::list<DTAVehicle*>::iterator iVehicle;

	for (iVehicle = m_VehicleSet.begin(); iVehicle != m_VehicleSet.end(); iVehicle++)
	{
		DTAVehicle* pVehicle = (*iVehicle);

		for(int i= 2; i<pVehicle->m_NodeSize; i++)
		{

			if(m_LinkNoMap.find(pVehicle->m_NodeAry[i-1].LinkNo) != m_LinkNoMap.end()
				&&  m_LinkNoMap.find(pVehicle->m_NodeAry[i].LinkNo) != m_LinkNoMap.end())
			{
				DTALink* pLink0 = m_LinkNoMap[pVehicle->m_NodeAry[i-1].LinkNo]; // i=0, LinkNo = -1;
				DTALink* pLink1 = m_LinkNoMap[pVehicle->m_NodeAry[i].LinkNo];

				CString movement_label;
				movement_label.Format ("%d;%d;%d", pLink0->m_FromNodeNumber  ,  pLink0->m_ToNodeNumber , pLink1->m_ToNodeNumber);
				m_Movement3NodeMap[movement_label].TotalVehicleSize ++;

			}
		}
	}

	FILE* st;

	CString directory;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

	fopen_s(&st,directory+"input_movement_3_node_format.csv","w");
	if(st!=NULL)
	{
		fprintf(st,"movement_index,three-node key,count\n");

		int movement_index = 1;
		std::map<CString, Movement3Node> ::const_iterator itr;
		for (itr = m_Movement3NodeMap.begin(); itr != m_Movement3NodeMap.end(); itr++)
		{
			fprintf(st, "%d,%s,%d\n", 
				movement_index++,
				(*itr).first, 
				(*itr).second.TotalVehicleSize );

		}
		fclose(st);
	}else
	{
		AfxMessageBox("File input_movement.csv cannot be opened.");

	}


}

void CTLiteDoc::OnAssignmentSimulatinSettinsClicked()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project has not been loaded.");
		return;
	}

	
	if(ReadScenarioSettingCSVFile(m_ProjectDirectory+"input_scenario_settings.csv") == false)
	{
		AfxMessageBox("The project does not have scenario data. If this is a data set prepared for traffic assignment/simulation, please save the project first to create scenario files.");
		return;
	
	}


	CAssignmentSimulationSettingDlg dlg;
	dlg.m_pDoc = this;
	dlg.DoModal();
}

void CTLiteDoc::OnProjectNetworkData()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project has not been loaded.");
		return;
	}
	CNetworkDataSettingDlg dlg;
	dlg.m_pDoc = this;
	dlg.DoModal();

}

void CTLiteDoc::OnLinkAddsensor()
{

	//if(m_SelectedLinkNo==-1)
	//{
	//	AfxMessageBox("Please select a link first.");
	//	return;
	//}

	//DTALink* pLink= m_LinkNoMap [m_SelectedLinkNo];
	//if(pLink!=NULL)
	//{

	//	CNetworkDataSettingDlg dlg;
	//	dlg.m_pDoc = this;
	//	dlg.m_SelectTab = 4; // _SENSOR_DATA
	//	dlg.m_SelectedFromNodeName = pLink->m_FromNodeNumber;
	//	dlg.m_SelectedToNodeName = pLink->m_ToNodeNumber;

	//	if(dlg.DoModal()==IDOK)
	//	{
	//		ReadSensorCountData();
	//		UpdateAllViews(0);
	//	}

	//}
}



void CTLiteDoc::OnToolsObtainCyclelengthfromNearbySignals()
{


	FILE* st = NULL;

	CString str_log_file;
	str_log_file.Format ("%s\\signal_data_reading_log.csv",m_ProjectDirectory);

	fopen_s(&st,str_log_file,"w");

	int count = 0;
	if( st !=NULL)
	{

		fprintf(st,"baseline_node_id,reference_node_id,base_line_cycle_length_in_second,reference_cycle_length_in_second\n");

		double min_distance =  m_UnitFeet * 1000;  //1000 feet


		// iNode is the node in the current view, pNode is the reference node in the reference document
		for (std::list<DTANode*>::iterator  iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{
			if((*iNode)->m_LayerNo == 0 && (*iNode)->m_ControlType == m_ControlType_PretimedSignal)
			{
				DTANode* pNode = FindSignalNodeWithCoordinate((*iNode)->pt.x , (*iNode)->pt.y, 1, min_distance);

				if(pNode!=NULL)
				{
					fprintf(st, "%d,%d,%d,%d\n", (*iNode)->m_NodeNumber , pNode->m_NodeNumber ,(*iNode)->m_CycleLengthInSecond,pNode->m_CycleLengthInSecond);
					(*iNode)->m_CycleLengthInSecond = pNode->m_CycleLengthInSecond;

					count++;
				}



			}
		}


		fclose(st);
	}


	OpenCSVFileInExcel(str_log_file);

	CString str;
	str.Format("%d nodes have obtained cycle length from the reference network. To accept the changes, please save the network.",count);
	AfxMessageBox(str, MB_ICONINFORMATION);

}


void CTLiteDoc::OnToolsSaveprojectforexternallayer()
{
	CFileDialog fdlg (FALSE, "*.tnp", "*.tnp",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_LONGNAMES,
		"Transportation Network Project (*.tnp)|*.tnp|");

	if(fdlg.DoModal()==IDOK)
	{
		CString path = fdlg.GetFileName ();
		CWaitCursor wait;
		m_ProjectFile = fdlg.GetPathName();
		m_ProjectTitle = GetWorkspaceTitleName(m_ProjectFile);
		int ExternalLayerNo = 1;
		if(SaveProject(fdlg.GetPathName(),ExternalLayerNo))
		{
			CString msg;
			if(m_NodeSet.size()>0)
			{
				msg.Format ("Files input_node.csv, input_link.csv and input_zone.csv have been successfully saved with %d nodes, %d links, %d zones.",m_NodeSet.size(), m_LinkSet.size(), m_ZoneMap.size());
				AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
			}/*else
			 {
			 msg.Format ("Files input_node.csv, input_link.csv and input_zone.csv have been successfully saved.");
			 AfxMessageBox(msg,MB_OK|MB_ICONINFORMATION);
			 }*/

			SetTitle(m_ProjectTitle);


		}
	}
}


void CTLiteDoc::OnMoeTableDialog()
{
	CDlgMOETabView dlg;
	dlg.m_pDoc = this;
	dlg.DoModal();

}

DTALink* CTLiteDoc::FindLinkFromCoordinateLocation(float x1, float y1, float x2, float y2, float min_distance_in_mile)
{
	DTALink* pSelectedLink = NULL;
	float min_distance = 999999;

	int x_key = (x1 - m_GridRect.left) / m_GridXStep;
	int y_key = (y1 - m_GridRect.bottom) / m_GridYStep;

	//feasible region
	x_key = max(0, x_key);
	x_key = min(_MAX_TRANSIT_GRID_SIZE - 1, x_key);

	y_key = max(0, y_key);
	y_key = min(_MAX_TRANSIT_GRID_SIZE - 1, y_key);

	for (int i = 0; i < m_GridMatrix[x_key][y_key][0].m_LinkNoVector.size(); i++)
	{
		int linkno = m_GridMatrix[x_key][y_key][0].m_LinkNoVector[i];
		DTALink* pLink = m_LinkNoMap[linkno];
		float distance = sqrt(pow(x1 - pLink->m_FromPoint.x, 2) + pow(y1 - pLink->m_FromPoint.y, 2) + pow(x2 - pLink->m_ToPoint.x, 2) + pow(y2 - pLink->m_ToPoint.y, 2));
		if (distance < min_distance)
		{
			min_distance = distance;
			pSelectedLink = pLink;

		}

	}

	int from_x_key = x_key;
	int from_y_key = y_key;


	x_key = (x2 - m_GridRect.left) / m_GridXStep;
	y_key = (y2 - m_GridRect.bottom) / m_GridYStep;

	//feasible region
	x_key = max(0, x_key);
	x_key = min(_MAX_TRANSIT_GRID_SIZE - 1, x_key);

	y_key = max(0, y_key);
	y_key = min(_MAX_TRANSIT_GRID_SIZE - 1, y_key);

	if (from_x_key != x_key || from_y_key != y_key)
	{
		for (int i = 0; i < m_GridMatrix[x_key][y_key][0].m_LinkNoVector.size(); i++)
		{
			int linkno = m_GridMatrix[x_key][y_key][0].m_LinkNoVector[i];
			DTALink* pLink = m_LinkNoMap[linkno];
			float distance = sqrt(pow(x1 - pLink->m_FromPoint.x, 2) + pow(y1 - pLink->m_FromPoint.y, 2) + pow(x2 - pLink->m_ToPoint.x, 2) + pow(y2 - pLink->m_ToPoint.y, 2));
			if (distance < min_distance)
			{
				min_distance = distance;
				pSelectedLink = pLink;

			}

		}
	}


	float distance_in_mile = min_distance / max(0.0000001, m_UnitMile);
	if (distance_in_mile < min_distance_in_mile)
		return pSelectedLink;
	else
		return NULL;
}
DTALink* CTLiteDoc::FindLinkFromCoordinateLocationV2(float x1, float y1, float x2, float y2)
{
	DTALink* pSelectedLink = NULL;
	float min_distance = 999999;

	for (int i = 0; i < m_LinkNoMap.size(); i++)
	{
		DTALink* pLink = m_LinkNoMap[i];
		float distance = sqrt(pow(x1 - pLink->m_FromPoint.x, 2) + pow(y1 - pLink->m_FromPoint.y, 2) + pow(x2 - pLink->m_ToPoint.x, 2) + pow(y2 - pLink->m_ToPoint.y, 2));
		if (distance < min_distance)
		{
			min_distance = distance;
			pSelectedLink = pLink;

		}

	}

	if (min_distance < 9999)
		return pSelectedLink;
	else
		return NULL;
}

bool CTLiteDoc::ReadGPSBinFile(LPCTSTR lpszFileName, int date_id,int max_GPS_data_count)
{
	//   cout << "Read vehicle file... "  << endl;
	// vehicle_id,  origin_zone_id, destination_zone_id, departure_time,
	//	arrival_time, complete_flag, trip_time, demand_type, occupancy, information_type,
	//	value_of_time, path_min_cost,distance_in_mile, number_of_nodes,
	//	node id, node arrival time


	FILE* st_output = NULL;
	fopen_s(&st_output,this->m_ProjectDirectory  + "output_trace.csv","w");
	if(st_output!=NULL)
	{
		fprintf(st_output,"trip_id,trace_id,from_zone_id,to_zone_id,departure_time_in_min,arrival_time_in_min,trip_time_in_min,distance,number_of_nodes,path_node_sequence,path_time_sequence,link_sequence_linkid_travel_time,GPS_geometry,geometry,demand_type,pricing_type,vehicle_type,information_type,value_of_time\n");

	}

	std::list<DTALink*>::iterator iLink;

	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  //m_LinkSet ?????????
	{
		(*iLink)->m_OriginalLength = (*iLink)->m_Length ;  // keep the original link length
	}

	typedef struct  
	{
		int original_vehicle_id;
		int date_id;
		int vehicle_id;
		int from_zone_id;
		int to_zone_id;
		float departure_time;
		float arrival_time;
		float trip_time;
		float total_distance_in_km;
		int number_of_nodes;
		float reserved_field1;
		float reserved_field2;
		float reserved_field3;

	} struct_Vehicle_Header;


	FILE* st = NULL;
	fopen_s(&st,lpszFileName,"rb");
	if(st!=NULL)
	{
		bool bUpdateTravelTime = true;

		CDlg_UserInput dlg;

		dlg.m_StrQuestion  = "Please specify the number of traces to be loaded:";
		dlg.m_InputValue = "100000";

		max_GPS_data_count = 100000;
		if(dlg.DoModal ()==IDOK)
		{
			max_GPS_data_count = atoi(dlg.m_InputValue) ;
		}else
		{
			return false;
		}


		//if(AfxMessageBox("Do you want to update GPS probes' travel time and distance based on map-matching results?", MB_YESNO) == IDYES)
		//{
		//	bUpdateTravelTime = true;
		//}



		date_id = 0;

		//	CDlg_UserInput dlg_date;
		//dlg_date.m_StrQuestion  = "Please specify the day of month to be loaded (0 means all days):";
		//	dlg_date.m_InputValue = "0";

		//	if(dlg_date.DoModal ()==IDOK)
		//	{
		//		date_id = atoi(dlg_date.m_InputValue) ;
		//	}

		CWaitCursor wait;

		m_VehicleSet.clear();

		struct_Vehicle_Header header;

		int count =0;
		int count_processed = 0;
		while(!feof(st))
		{

			size_t result = fread(&header,sizeof(struct_Vehicle_Header),1,st);

			count_processed++;

			if(count_processed>max_GPS_data_count)
				break;

			if( header.vehicle_id < 0)
				break;

			if(result!=1)  // read end of file
				break;

			std::vector<int> PathLinkVector;
			std::vector<int> PathNodeVector;
			std::vector<float>  PathNodeArrivalTimeVector;
			int NetworkNodeNo = -1;
			int NetworkLinkID = -1;

			DTAVehicle* pVehicle = 0;
			pVehicle = new DTAVehicle;
			pVehicle->m_bGPSVehicle = true;
			pVehicle->m_DayNo 		= header.date_id;

			pVehicle->m_VehicleID		= m_VehicleSet.size();

			pVehicle->m_OriginZoneID	= -1;
			pVehicle->m_DestinationZoneID= -1;

			pVehicle->m_Distance = header.total_distance_in_km * 0.621371; // km to miles
			pVehicle->m_DepartureTime	=  header.departure_time;
			pVehicle->m_ArrivalTime =  header.arrival_time;
			pVehicle->m_TripTime  = header.trip_time;

			if(g_Simulation_Time_Horizon < pVehicle->m_ArrivalTime)
				g_Simulation_Time_Horizon = pVehicle->m_ArrivalTime;

			pVehicle->m_bComplete = true;
			pVehicle->m_DemandType = 1;

			pVehicle->m_PricingType = 1;

			//pVehicle->m_VehicleType = (unsigned char)g_read_integer(pFile);

			pVehicle->m_VOT = 10;
			pVehicle->m_TollDollarCost = 0;
			pVehicle->m_PM = 0;


			pVehicle->m_VehicleLocationSize	= header.number_of_nodes;

			if(pVehicle->m_VehicleLocationSize==0 )
			{
				CString error_msg;
				error_msg.Format("reading error in file %s: vehicle id =%d",lpszFileName, header.vehicle_id);

			}

			if(pVehicle->m_VehicleLocationSize>=1)  // in case reading error
			{

				/************************************/
				//step 1: initialize all node-to-gps distance

				std::list<DTANode*>::iterator iNode;
				for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
				{

					DTANode* pNode = (*iNode);

					pNode->m_min_distance_from_GPS_point = 10; // 10 miles;
					pNode->m_GPS_arrival_time = 99999;
				}

				/************************************/


				float total_distance = 0;
				pVehicle->m_LocationRecordAry = new VehicleLocationRecord[pVehicle->m_VehicleLocationSize];

				pVehicle->m_NodeNumberSum = 0;
				for(int i=0; i< pVehicle->m_VehicleLocationSize; i++)
				{
					VehicleLocationRecord node_element;
					fread(&pVehicle->m_LocationRecordAry[i],sizeof(VehicleLocationRecord),1,st);

					double min_distance = 1*m_UnitMile; // in mile

					// update origin and destination zone ids

					if(i==0 )
					{
						int ZoneNumber  = FindClosestZone( pVehicle->m_LocationRecordAry[i].x, pVehicle->m_LocationRecordAry[i].y, min_distance	);

						if(ZoneNumber!=-1)
							pVehicle->m_OriginZoneID = ZoneNumber;

					}

					if(i== pVehicle->m_VehicleLocationSize -1)
					{
						int ZoneNumber  = FindClosestZone( pVehicle->m_LocationRecordAry[i].x, pVehicle->m_LocationRecordAry[i].y, min_distance	);
						if(ZoneNumber!=-1)
							pVehicle->m_DestinationZoneID = ZoneNumber;

					}

					int NodeID = FindClosestNode( pVehicle->m_LocationRecordAry[i].x, pVehicle->m_LocationRecordAry[i].y, min_distance, 1,pVehicle->m_LocationRecordAry[i].time_stamp_in_min	);

					if(NodeID>=0)
					{
						if(PathNodeVector.size() ==0 )
						{
							PathNodeVector.push_back (NodeID);

						}else
							if( PathNodeVector[PathNodeVector.size()-1]!=NodeID)
							{
								PathNodeVector.push_back (NodeID);

							}
					}


					/*if( PathNodeVector.size() >0 &&  PathNodeVector[PathNodeVector.size()-1]!=NodeID && NodeID>=0)
					{
					PathNodeVector.push_back (NodeID);

					DTALink* pLink = FindLinkWithNodeIDs(PathNodeVector[PathNodeVector.size()-2], PathNodeVector[PathNodeVector.size()-1]);

					if(pLink!= NULL)
					{
					PathLinkVector.push_back(pLink->m_LinkNo );
					PathNodeArrivalTimeVector.push_back (pVehicle->m_LocationRecordAry[i].time_stamp_in_min);
					}


					}*/


				}

				if(PathNodeVector.size()>=1)
				{
					for(int i = 0; i< PathNodeVector.size(); i++)
					{

						DTANode* pNode = m_NodeNoMap[ PathNodeVector[i]];

						TRACE("Node Id, %d, %d", i,pNode->m_NodeNumber );

					}

				}


				// map matching step no.2;

				for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  //m_LinkSet 
				{

					// new link length is the average node-to-gps distances
					(*iLink)->m_Length  = (m_NodeNoMap[(*iLink)->m_FromNodeID ]->m_min_distance_from_GPS_point + m_NodeNoMap[(*iLink)->m_ToNodeID ]->m_min_distance_from_GPS_point)/2.0;

				}


				if(m_pNetwork ==NULL)  // we only build the network once
				{
					m_pNetwork = new DTANetworkForSP(m_NodeSet.size(), m_LinkSet.size(), 1, 1, m_AdjLinkSize);  //  network instance for single processor in multi-thread environment
				}


				// map matching step no.3:
				// rebuild the link cost as the modified link length

				if(PathNodeVector.size()<=2)
					continue;

				bool bMapMatchingFlag = false;

				if(bMapMatchingFlag)
				{
				m_pNetwork->BuildPhysicalNetwork(&m_NodeSet, &m_LinkSet, 0, false);

				int PathLinkList[MAX_NODE_SIZE_IN_A_PATH];
				float TotalCost = 0;

				int NodeSize= m_pNetwork->SimplifiedTDLabelCorrecting_DoubleQueue(PathNodeVector[0], 0, PathNodeVector[PathNodeVector.size()-1], 1, 10.0f,PathLinkList,TotalCost, false, false, false,0);   // Pointer to previous node (node)


				DTALink* pLink =NULL;
				DTANode* pNode = NULL;


				//post processing  to remove non-marked node with arrival time of 99999

				// 
				int i;
				for (i=0 ; i < NodeSize-1; i++)
				{

					PathLinkVector.push_back(PathLinkList[i]) ;
				}

				float arrival_time_at_downstream_node;
				// go backward
				for (i=NodeSize-2 ; i >=0 ; i--)
				{
					pLink = m_LinkNoMap[PathLinkList[i]];
					pNode = m_NodeNoMap[pLink->m_FromNodeID ];

					if(i == NodeSize-2)
						arrival_time_at_downstream_node = min(1440,pNode->m_GPS_arrival_time);

					if(pNode->m_GPS_arrival_time > 90000)  // node has not been touched, reset the travel time: this is a crude approximatio
					{
						pNode->m_GPS_arrival_time = arrival_time_at_downstream_node;
					}
				}


				for (i=0 ; i < NodeSize-1; i++)
				{

					pLink = m_LinkNoMap[PathLinkList[i]];

					pNode = m_NodeNoMap[pLink->m_FromNodeID ];

					PathNodeArrivalTimeVector.push_back (pNode->m_GPS_arrival_time);

					// last node
					if(i== NodeSize-2)
					{
						pNode = m_NodeNoMap[pLink->m_ToNodeID ];

						PathNodeArrivalTimeVector.push_back (pNode->m_GPS_arrival_time);

					}
				}



				/********************************************************************/


				if(PathLinkVector.size()>=1 &&(date_id==0 || date_id == pVehicle->m_DayNo ))
				{

					pVehicle->m_NodeSize = PathLinkVector.size()+1;
					pVehicle->m_NodeAry = new SVehicleLink[PathLinkVector.size()+1];

					pVehicle->m_NodeAry[0].ArrivalTimeOnDSN = PathNodeArrivalTimeVector[0];
					double total_distance = 0;
					for(int i = 0; i< PathLinkVector.size(); i++)
					{
						pVehicle->m_NodeAry[i+1].LinkNo  = PathLinkVector[i] ;


						DTALink* pLink = FindLinkWithLinkNo(PathLinkVector[i]);
						total_distance+= pLink->m_OriginalLength;

						TRACE("\n%d->%d", pLink->m_FromNodeNumber , pLink->m_ToNodeNumber );


						pVehicle->m_FreeflowTripTime += pLink->m_FreeFlowTravelTime ;

						float travel_time  = PathNodeArrivalTimeVector[i+1] - PathNodeArrivalTimeVector[i];
						float delay = travel_time- pLink->m_FreeFlowTravelTime;
						pLink->m_total_delay += delay;
						pLink->AddNodeDelay(pVehicle->m_NodeAry[i].ArrivalTimeOnDSN ,delay);

						pVehicle->m_NodeAry[i+1].ArrivalTimeOnDSN = PathNodeArrivalTimeVector[i+1];
						pVehicle->m_NodeNumberSum += pLink->m_FromNodeNumber ;

						//	//overwrite zone id for nodes
						//	if(i==1 && pVehicle->m_OriginZoneID !=0 )
						//	{

						//DTAActivityLocation element;
						//element.ZoneID  = pVehcle->m_OriginZoneID;
						//element.NodeNumber = pLink->m_FromNodeNumber;
						//element.External_OD_flag = 0;

						//m_NodeNoMap[pLink->m_FromNodeID ] ->m_ZoneID = pVehicle->m_OriginZoneID;
						//m_ZoneMap [element.ZoneID ].m_ActivityLocationVector .push_back (element);
						//	
						//	}

					}

					if(bUpdateTravelTime)
					{
						pVehicle->m_Distance = total_distance;
						pVehicle->m_DepartureTime	=  PathNodeArrivalTimeVector[0];
						pVehicle->m_ArrivalTime =  PathNodeArrivalTimeVector[PathNodeArrivalTimeVector.size()-1];
						pVehicle->m_TripTime  = max(0.1,pVehicle->m_ArrivalTime  - pVehicle->m_DepartureTime);


					}

				}

			if(pVehicle->m_OriginZoneID ==0 || pVehicle->m_DestinationZoneID ==0)
			{
				TRACE("\nvehicle id: %d, O: %d, D: %d", header.vehicle_id, header.from_zone_id,header.to_zone_id );
			}


			if(count%10 == 0 && count >=1)
			{
				CString str;
				str.Format ("Loading %d out of %d probe records...", count, count_processed);
				SetStatusText(str);
			}
			// movement count

			if(pVehicle->m_OriginZoneID >=0 && pVehicle->m_DestinationZoneID >=0 && PathLinkVector.size()>=1)
			{
				m_VehicleSet.push_back (pVehicle);
				m_VehicleIDMap[pVehicle->m_VehicleID]  = pVehicle;

				if(st_output!=NULL)
				{
					//	fprintf("agent_id,trip_id,from_zone_id,to_zone_id,departure_time_in_min,arrival_time_in_min,trip_time_in_min,distance,geometry,GPS_geometry,node_sequence,link_sequence,demand_type,pricing_type,vehicle_type,information_type,value_of_time");
					fprintf(st_output,"%d,%d,%d,%d,", pVehicle->m_VehicleID ,1,pVehicle->m_OriginZoneID ,pVehicle->m_DestinationZoneID);
					fprintf(st_output,"%5.2f,%5.2f,%5.2f,%5.2f,", pVehicle->m_DepartureTime, pVehicle->m_ArrivalTime,pVehicle->m_TripTime,pVehicle->m_Distance );

					fprintf(st_output,"%d,", pVehicle->m_NodeSize );

					for(int ni = 1; ni < pVehicle->m_NodeSize ; ni++)
					{

						DTALink* pLink = FindLinkWithLinkNo(pVehicle->m_NodeAry [ni].LinkNo);

						if(pLink!=NULL)
						{
							fprintf(st_output,"%d;",pLink->m_FromNodeNumber );

							// last node
							if(ni== pVehicle->m_NodeSize-1)
							{
								DTANode* pNode = m_NodeNoMap[pLink->m_ToNodeID ];

								fprintf(st_output,"%d;",pLink->m_ToNodeNumber);

							}
						}
					}
					fprintf(st_output,",");
					for(int ni = 1; ni < pVehicle->m_NodeSize ; ni++)
					{

						DTALink* pLink = FindLinkWithLinkNo(pVehicle->m_NodeAry [ni].LinkNo);

						if(pLink!=NULL)
						{
							fprintf(st_output,"%5.2f;",pVehicle->m_NodeAry [ni-1].ArrivalTimeOnDSN );

							// last node
							if(ni== pVehicle->m_NodeSize-1)
							{
								DTANode* pNode = m_NodeNoMap[pLink->m_ToNodeID ];

								fprintf(st_output,"%5.2f;",pVehicle->m_NodeAry [ni].ArrivalTimeOnDSN );

							}
						}
					}
					fprintf(st_output,",");

					fprintf(st_output,"<");

					for(int ni = 1; ni < pVehicle->m_NodeSize ; ni++)
					{

						DTALink* pLink = FindLinkWithLinkNo(pVehicle->m_NodeAry [ni].LinkNo);

						if(pLink!=NULL)
						{
							float prev_time =  pVehicle->m_NodeAry [ni-1].ArrivalTimeOnDSN;

							float travel_time = pVehicle->m_NodeAry [ni].ArrivalTimeOnDSN - prev_time;

							//		float speed =  pLink->m_Length /  max(0.000001,travel_time / 60);
							fprintf(st_output,"%d->%d:%5.3f:%5.2f:%5.2f; ",pLink->m_FromNodeNumber , pLink->m_ToNodeNumber, pLink->m_Length , pVehicle->m_NodeAry [ni].ArrivalTimeOnDSN,travel_time);

						}
					}

					fprintf(st_output,">,");

					// geometry
					fprintf(st_output,"\"<LineString><coordinates>");

					for(int mi=0; mi< pVehicle->m_VehicleLocationSize; mi++)
					{
						fprintf(st_output,"%f,%f,0.0",pVehicle->m_LocationRecordAry[mi].x , pVehicle->m_LocationRecordAry[mi].y );

						if(mi!=pVehicle->m_VehicleLocationSize-1)
							fprintf(st_output," ");
					}

					fprintf(st_output,"</coordinates></LineString>\",");

					fprintf(st_output,"\"<LineString><coordinates>");

					for(int ni = 1; ni < pVehicle->m_NodeSize ; ni++)
					{

						DTALink* pLink = FindLinkWithLinkNo(pVehicle->m_NodeAry [ni].LinkNo);

						if(pLink!=NULL)
						{ 

							for(int si = 0; si <pLink-> m_Original_ShapePoints.size(); si++)
							{
								fprintf(st_output,"%f,%f,0.0 ",pLink->m_Original_ShapePoints[si].x, pLink->m_Original_ShapePoints[si].y);

							}
						}


					}
					fprintf(st_output,"</coordinates></LineString>\",");
					fprintf(st_output,"\n");

				}
				count++;
			} 

			}
		// map matching here  // retore the distance of links 

		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  //m_LinkSet 
		{
			(*iLink)->m_Length  = (*iLink)->m_OriginalLength ;
		}

		}

		}

		if(st_output!=NULL)
			fclose(st_output);

		fclose(st);

		if(count >=1 ) //with GPS data
		{
			CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
			pMainFrame->m_bShowLayerMap[layer_vehicle_position]  = true;
			//			m_bGPSDataSet  = true;

		}


		m_SimulationVehicleDataLoadingStatus.Format ("%d valid %d records are loaded  files %s...", m_VehicleSet.size(),count_processed,lpszFileName);
		return true;

	}
	
	return false;
}

bool CTLiteDoc::ReadSensorTrajectoryData(LPCTSTR lpszFileName)
{
	CCSVParser parser;
	int i= 0;

	bool bRectInitialized = false;

	if (parser.OpenCSVFile(CString2StdString(lpszFileName)))
	{

		while(parser.ReadRecord())
		{

			VehicleLocationRecord element;
			
			if(parser.GetValueByFieldName("time_stamp_in_second",element.time_stamp_in_second) == false)
				break;

			if(parser.GetValueByFieldName("trace_id",element.agent_id) == false)
				break;


			if(parser.GetValueByFieldName("x",element.x) == false)
				break;

			if(parser.GetValueByFieldName("y",element.y) == false)
				break;


			AddLocationRecord(element);
			
			if(!bRectInitialized)
			{
				m_NetworkRect.left = element.x ;
				m_NetworkRect.right = element.x;
				m_NetworkRect.top = element.y;
				m_NetworkRect.bottom = element.y;
				bRectInitialized = true;
			}

			GDPoint point;
			point.x = element.x;
			point.y = element.y;

			m_NetworkRect.Expand(point);
		
			
			i ++;

		}
	
		m_AgentLocationLoadingStatus.Format("%d trace location records are loaded from file %s.",i,lpszFileName);	
	}

	// int date_id,int max_GPS_data_count;

	////   cout << "Read vehicle file... "  << endl;
	//// vehicle_id,  origin_zone_id, destination_zone_id, departure_time,
	////	arrival_time, complete_flag, trip_time, demand_type, occupancy, information_type,
	////	value_of_time, path_min_cost,distance_in_mile, number_of_nodes,
	////	node id, node arrival time

	//FILE* st_output = NULL;
	//fopen_s(&st_output,this->m_ProjectDirectory  + "output_trace.csv","w");
	//if(st_output!=NULL)
	//{
	//	fprintf(st_output,"trace_id,trip_id,from_zone_id,to_zone_id,departure_time_in_min,arrival_time_in_min,trip_time_in_min,distance,number_of_nodes,node_sequence_node:timestamp,link_sequence_linkid_travel_time,GPS_geometry,geometry,demand_type,pricing_type,vehicle_type,information_type,value_of_time\n");

	//}

	//std::list<DTALink*>::iterator iLink;

	//for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  //m_LinkSet ?????????
	//{
	//	(*iLink)->m_OriginalLength = (*iLink)->m_Length ;  // keep the original link length
	//}


	//	date_id = 0;

	//	CWaitCursor wait;

	//	m_VehicleSet.clear();

	//	struct_Vehicle_Header header;

	//	int count =0;
	//	int count_processed = 0;
	//	while(!feof(st))
	//	{

	//		std::vector<int> PathLinkVector;
	//		std::vector<int> PathNodeVector;
	//		std::vector<float>  PathNodeArrivalTimeVector;
	//		int NetworkNodeNo = -1;
	//		int NetworkLinkID = -1;

	//		DTAVehicle* pVehicle = 0;
	//		pVehicle = new DTAVehicle;
	//		pVehicle->m_bGPSVehicle = true;
	//		pVehicle->m_DayNo 		= header.date_id;

	//		pVehicle->m_VehicleID		= m_VehicleSet.size();

	//		pVehicle->m_OriginZoneID	= -1;
	//		pVehicle->m_DestinationZoneID= -1;

	//		pVehicle->m_Distance = header.total_distance_in_km * 0.621371; // km to miles
	//		pVehicle->m_DepartureTime	=  header.departure_time;
	//		pVehicle->m_ArrivalTime =  header.arrival_time;
	//		pVehicle->m_TripTime  = header.trip_time;

	//		if(g_Simulation_Time_Horizon < pVehicle->m_ArrivalTime)
	//			g_Simulation_Time_Horizon = pVehicle->m_ArrivalTime;

	//		pVehicle->m_bComplete = true;
	//		pVehicle->m_DemandType = 1;

	//		pVehicle->m_PricingType = 1;

	//		//pVehicle->m_VehicleType = (unsigned char)g_read_integer(pFile);

	//		pVehicle->m_VOT = 10;
	//		pVehicle->m_TollDollarCost = 0;
	//		pVehicle->m_PM = 0;


	//		pVehicle->m_VehicleLocationSize	= header.number_of_nodes;

	//		if(pVehicle->m_VehicleLocationSize==0 )
	//		{
	//			CString error_msg;
	//			error_msg.Format("reading error in file %s: vehicle id =%d",lpszFileName, header.vehicle_id);

	//		}

	//		if(pVehicle->m_VehicleLocationSize>=1)  // in case reading error
	//		{

	//			/************************************/
	//			//step 1: initialize all node-to-gps distance

	//			std::list<DTANode*>::iterator iNode;
	//			for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	//			{

	//				DTANode* pNode = (*iNode);

	//				pNode->m_min_distance_from_GPS_point = 10; // 10 miles;
	//				pNode->m_GPS_arrival_time = 99999;
	//			}

	//			/************************************/


	//			pVehicle->m_NodeNumberSum = 0;
	//			for(int i=0; i< pVehicle->m_VehicleLocationSize; i++)
	//			{
	//				VehicleLocationRecord node_element;
	//				fread(&pVehicle->m_LocationRecordAry[i],sizeof(VehicleLocationRecord),1,st);

	//				double min_distance = 1*m_UnitMile; // in mile

	//				// update origin and destination zone ids

	//				if(i==0 )
	//				{
	//					int ZoneNumber  = FindClosestZone( pVehicle->m_LocationRecordAry[i].x, pVehicle->m_LocationRecordAry[i].y, min_distance	);

	//					if(ZoneNumber!=-1)
	//						pVehicle->m_OriginZoneID = ZoneNumber;

	//				}

	//				if(i== pVehicle->m_VehicleLocationSize -1)
	//				{
	//					int ZoneNumber  = FindClosestZone( pVehicle->m_LocationRecordAry[i].x, pVehicle->m_LocationRecordAry[i].y, min_distance	);
	//					if(ZoneNumber!=-1)
	//						pVehicle->m_DestinationZoneID = ZoneNumber;

	//				}

	//				int NodeID = FindClosestNode( pVehicle->m_LocationRecordAry[i].x, pVehicle->m_LocationRecordAry[i].y, min_distance, 1,pVehicle->m_LocationRecordAry[i].time_stamp_in_min	);

	//				if(NodeID>=0)
	//				{
	//					if(PathNodeVector.size() ==0 )
	//					{
	//						PathNodeVector.push_back (NodeID);

	//					}else
	//						if( PathNodeVector[PathNodeVector.size()-1]!=NodeID)
	//						{
	//							PathNodeVector.push_back (NodeID);

	//						}
	//				}


	//			}

	//			if(PathNodeVector.size()>=1)
	//			{
	//				for(int i = 0; i< PathNodeVector.size(); i++)
	//				{

	//					DTANode* pNode = m_NodeNoMap[ PathNodeVector[i]];

	//					TRACE("Node Id, %d, %d", i,pNode->m_NodeNumber );

	//				}

	//			}


	//			// map matching step no.2;

	//			for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  //m_LinkSet 
	//			{

	//				// new link length is the average node-to-gps distances
	//				(*iLink)->m_Length  = (m_NodeNoMap[(*iLink)->m_FromNodeID ]->m_min_distance_from_GPS_point + m_NodeNoMap[(*iLink)->m_ToNodeID ]->m_min_distance_from_GPS_point)/2.0;

	//			}


	//			if(m_pNetwork ==NULL)  // we only build the network once
	//			{
	//				m_pNetwork = new DTANetworkForSP(m_NodeSet.size(), m_LinkSet.size(), 1, 1, m_AdjLinkSize);  //  network instance for single processor in multi-thread environment
	//			}


	//			// map matching step no.3:
	//			// rebuild the link cost as the modified link length

	//			if(PathNodeVector.size()<=2)
	//				continue;


	//			m_pNetwork->BuildPhysicalNetwork(&m_NodeSet, &m_LinkSet, 0, false);

	//			int PathLinkList[MAX_NODE_SIZE_IN_A_PATH];
	//			float TotalCost = 0;

	//			int NodeSize= m_pNetwork->SimplifiedTDLabelCorrecting_DoubleQueue(PathNodeVector[0], 0, PathNodeVector[PathNodeVector.size()-1], 1, 10.0f,PathLinkList,TotalCost, false, false, false,0);   // Pointer to previous node (node)


	//			DTALink* pLink =NULL;
	//			DTANode* pNode = NULL;


	//			//post processing  to remove non-marked node with arrival time of 99999

	//			// 
	//			int i;
	//			for (i=0 ; i < NodeSize-1; i++)
	//			{

	//				PathLinkVector.push_back(PathLinkList[i]) ;
	//			}

	//			float arrival_time_at_downstream_node;
	//			// go backward
	//			for (i=NodeSize-2 ; i >=0 ; i--)
	//			{
	//				pLink = m_LinkNoMap[PathLinkList[i]];
	//				pNode = m_NodeNoMap[pLink->m_FromNodeID ];

	//				if(i == NodeSize-2)
	//					arrival_time_at_downstream_node = min(1440,pNode->m_GPS_arrival_time);

	//				if(pNode->m_GPS_arrival_time > 90000)  // node has not been touched, reset the travel time: this is a crude approximatio
	//				{
	//					pNode->m_GPS_arrival_time = arrival_time_at_downstream_node;
	//				}
	//			}


	//			for (i=0 ; i < NodeSize-1; i++)
	//			{

	//				pLink = m_LinkNoMap[PathLinkList[i]];

	//				pNode = m_NodeNoMap[pLink->m_FromNodeID ];

	//				PathNodeArrivalTimeVector.push_back (pNode->m_GPS_arrival_time);

	//				// last node
	//				if(i== NodeSize-2)
	//				{
	//					pNode = m_NodeNoMap[pLink->m_ToNodeID ];

	//					PathNodeArrivalTimeVector.push_back (pNode->m_GPS_arrival_time);

	//				}
	//			}



	//			/********************************************************************/


	//			if(PathLinkVector.size()>=1 &&(date_id==0 || date_id == pVehicle->m_DayNo ))
	//			{

	//				pVehicle->m_NodeSize = PathLinkVector.size()+1;
	//				pVehicle->m_NodeAry = new SVehicleLink[PathLinkVector.size()+1];

	//				pVehicle->m_NodeAry[0].ArrivalTimeOnDSN = PathNodeArrivalTimeVector[0];
	//				double total_distance = 0;
	//				for(int i = 0; i< PathLinkVector.size(); i++)
	//				{
	//					pVehicle->m_NodeAry[i+1].LinkNo  = PathLinkVector[i] ;


	//					DTALink* pLink = FindLinkWithLinkNo(PathLinkVector[i]);
	//					total_distance+= pLink->m_OriginalLength;

	//					TRACE("\n%d->%d", pLink->m_FromNodeNumber , pLink->m_ToNodeNumber );


	//					pVehicle->m_FreeflowTripTime += pLink->m_FreeFlowTravelTime ;

	//					float travel_time  = PathNodeArrivalTimeVector[i+1] - PathNodeArrivalTimeVector[i];
	//					float delay = travel_time- pLink->m_FreeFlowTravelTime;
	//					pLink->m_total_delay += delay;
	//					pLink->AddNodeDelay(pVehicle->m_NodeAry[i].ArrivalTimeOnDSN ,delay);

	//					pVehicle->m_NodeAry[i+1].ArrivalTimeOnDSN = PathNodeArrivalTimeVector[i+1];
	//					pVehicle->m_NodeNumberSum += pLink->m_FromNodeNumber ;


	//				}

	//				if(bUpdateTravelTime)
	//				{
	//					pVehicle->m_Distance = total_distance;
	//					pVehicle->m_DepartureTime	=  PathNodeArrivalTimeVector[0];
	//					pVehicle->m_ArrivalTime =  PathNodeArrivalTimeVector[PathNodeArrivalTimeVector.size()-1];
	//					pVehicle->m_TripTime  = max(0.1,pVehicle->m_ArrivalTime  - pVehicle->m_DepartureTime);


	//				}
	//			}

	//		}

	//		if(pVehicle->m_OriginZoneID ==0 || pVehicle->m_DestinationZoneID ==0)
	//		{
	//			TRACE("\nvehicle id: %d, O: %d, D: %d", header.vehicle_id, header.from_zone_id,header.to_zone_id );
	//		}


	//		if(count%10 == 0 && count >=1)
	//		{
	//			CString str;
	//			str.Format ("Loading %d out of %d probe records...", count, count_processed);
	//			SetStatusText(str);
	//		}
	//		// movement count

	//		if(pVehicle->m_OriginZoneID >=0 && pVehicle->m_DestinationZoneID >=0 && PathLinkVector.size()>=1)
	//		{
	//			m_VehicleSet.push_back (pVehicle);
	//			m_VehicleIDMap[pVehicle->m_VehicleID]  = pVehicle;

	//			if(st_output!=NULL)
	//			{
	//				//	fprintf("agent_id,trip_id,from_zone_id,to_zone_id,departure_time_in_min,arrival_time_in_min,trip_time_in_min,distance,geometry,GPS_geometry,node_sequence,link_sequence,demand_type,pricing_type,vehicle_type,information_type,value_of_time");
	//				fprintf(st_output,"%d,%d,%d,%d,", pVehicle->m_VehicleID ,1,pVehicle->m_OriginZoneID ,pVehicle->m_DestinationZoneID);
	//				fprintf(st_output,"%5.2f,%5.2f,%5.2f,%5.2f,", pVehicle->m_DepartureTime, pVehicle->m_ArrivalTime,pVehicle->m_TripTime,pVehicle->m_Distance );

	//				fprintf(st_output,"%d,", pVehicle->m_NodeSize );

	//				fprintf(st_output,"<");
	//				for(int ni = 1; ni < pVehicle->m_NodeSize ; ni++)
	//				{

	//					DTALink* pLink = FindLinkWithLinkNo(pVehicle->m_NodeAry [ni].LinkNo);

	//					if(pLink!=NULL)
	//					{
	//						fprintf(st_output,"%d:%5.2f;",pLink->m_FromNodeNumber,pVehicle->m_NodeAry [ni-1].ArrivalTimeOnDSN );

	//						// last node
	//						if(ni== pVehicle->m_NodeSize-1)
	//						{
	//							DTANode* pNode = m_NodeNoMap[pLink->m_ToNodeID ];

	//							fprintf(st_output,"%d:%5.2f;",pLink->m_ToNodeNumber,pVehicle->m_NodeAry [ni].ArrivalTimeOnDSN );

	//						}
	//					}
	//				}
	//				fprintf(st_output,">,");

	//				fprintf(st_output,"<");

	//				for(int ni = 1; ni < pVehicle->m_NodeSize ; ni++)
	//				{

	//					DTALink* pLink = FindLinkWithLinkNo(pVehicle->m_NodeAry [ni].LinkNo);

	//					if(pLink!=NULL)
	//					{
	//						float prev_time =  pVehicle->m_NodeAry [ni-1].ArrivalTimeOnDSN;

	//						float travel_time = pVehicle->m_NodeAry [ni].ArrivalTimeOnDSN - prev_time;

	//						//		float speed =  pLink->m_Length /  max(0.000001,travel_time / 60);
	//						fprintf(st_output,"%d->%d:%5.3f:%5.2f:%5.2f; ",pLink->m_FromNodeNumber , pLink->m_ToNodeNumber, pLink->m_Length , pVehicle->m_NodeAry [ni].ArrivalTimeOnDSN,travel_time);

	//					}
	//				}

	//				fprintf(st_output,">,");

	//				// geometry
	//				fprintf(st_output,"\"<LineString><coordinates>");

	//				for(int mi=0; mi< pVehicle->m_VehicleLocationSize; mi++)
	//				{
	//					fprintf(st_output,"%f,%f,0.0",pVehicle->m_LocationRecordAry[mi].x , pVehicle->m_LocationRecordAry[mi].y );

	//					if(mi!=pVehicle->m_VehicleLocationSize-1)
	//						fprintf(st_output," ");
	//				}

	//				fprintf(st_output,"</coordinates></LineString>\",");

	//				fprintf(st_output,"\"<LineString><coordinates>");

	//				for(int ni = 1; ni < pVehicle->m_NodeSize ; ni++)
	//				{

	//					DTALink* pLink = FindLinkWithLinkNo(pVehicle->m_NodeAry [ni].LinkNo);

	//					if(pLink!=NULL)
	//					{ 

	//						for(int si = 0; si <pLink-> m_Original_ShapePoints.size(); si++)
	//						{
	//							fprintf(st_output,"%f,%f,0.0 ",pLink->m_Original_ShapePoints[si].x, pLink->m_Original_ShapePoints[si].y);

	//						}
	//					}


	//				}
	//				fprintf(st_output,"</coordinates></LineString>\",");
	//				fprintf(st_output,"\n");

	//			}
	//			count++;
	//		} 

	//	}
	//	// map matching here  // retore the distance of links 

	//	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  //m_LinkSet 
	//	{
	//		(*iLink)->m_Length  = (*iLink)->m_OriginalLength ;
	//	}


	//	if(st_output!=NULL)
	//		fclose(st_output);

	//	fclose(st);

	//	if(count >=1 ) //with GPS data
	//	{
	//		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	//		pMainFrame->m_bShowLayerMap[layer_vehicle_position]  = true;
	//		//			m_bGPSDataSet  = true;

	//	}


//		m_SimulationVehicleDataLoadingStatus.Format ("%d valid traces (%d records) are loaded  files %s...", m_VehicleSet.size(),count_processed,lpszFileName);
		return true;
}

void CTLiteDoc::ResetZoneIDVector()
{
	m_ZoneNumberVector.clear();
	m_ZoneIDVector.clear ();

	//update m_ODSize
	std::map<int, DTAZone>	:: const_iterator itr;

	for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
	{
		if( itr->first > m_ODSize)
		{

			m_ODSize = itr->first;
		}
	}

	//push all data to m_ZoneIDVector

	for(int i = 0; i<= m_ODSize; i++)
	{
		m_ZoneIDVector.push_back(-1);
	}

	int count = 0;

	if(m_ZoneMap.size () ==0)  // no data
	{
		for(int i = 1; i<= m_ODSize; i++)
		{
			m_ZoneIDVector[i]=count++;
			m_ZoneNumberVector.push_back (i);
		}

		m_ZoneNoSize  =  m_ODSize;


	}else
	{

		std::map<int, DTAZone>	:: const_iterator itr;

		for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
		{
			m_ZoneNumberVector.push_back ( itr->first);

			if(itr->first < m_ZoneIDVector.size())
			{
				m_ZoneIDVector [itr->first ] = count++;
			}
		}

		m_ZoneNoSize  = m_ZoneNumberVector.size();

	}
}

void CTLiteDoc::ResetODMOEMatrix()
{

	if(m_ODSize== m_PreviousODSize)
		return; 


	ResetZoneIDVector();

	if(m_VehicleSet.size() ==0)  // no vheicle data, no need to process OD matrix
		return;

	if(m_ODMOEMatrix == NULL  )
	{
		m_ODMOEMatrix = Allocate3DDynamicArray<VehicleStatistics>(m_DemandTypeSize,m_ZoneNoSize,m_ZoneNoSize);
		m_PreviousDemandTypeSize = m_DemandTypeSize;
		m_PreviousZoneNoSize = m_ZoneNoSize ;
	}
	else
	{
		Deallocate3DDynamicArray<VehicleStatistics>(m_ODMOEMatrix,m_PreviousDemandTypeSize, m_PreviousZoneNoSize);
		m_ODMOEMatrix = Allocate3DDynamicArray<VehicleStatistics>(m_DemandTypeSize,m_ZoneNoSize,m_ZoneNoSize);
		m_PreviousDemandTypeSize = m_DemandTypeSize;
		m_PreviousZoneNoSize = m_ZoneNoSize ;
	}

	int p, i, j;

	for(p=0;p<m_DemandTypeSize; p++)
	{
		for(i= 0; i < m_ZoneNoSize ; i++)
		{
			for(j= 0; j< m_ZoneNoSize ; j++)
			{
				m_ODMOEMatrix[p][i][j].Reset ();
			}

		}

	}



	int count  = 0;
	std::list<DTAVehicle*>::iterator iVehicle;


	for (iVehicle = m_VehicleSet.begin(); iVehicle != m_VehicleSet.end(); iVehicle++, count++)
	{
		DTAVehicle* pVehicle = (*iVehicle);
		if(/*pVehicle->m_NodeSize >= 2 && */pVehicle->m_bComplete )
		{
			int p = pVehicle->m_DemandType ;
			p = 0;

			if(pVehicle->m_OriginZoneID >m_ZoneIDVector.size())
			{

				CString msg;
				msg.Format("Zone ID %d for vehicle %d has not been defined in the input_zone.csv file. Please check.",pVehicle->m_OriginZoneID , pVehicle->m_VehicleID );
				AfxMessageBox(msg);
				return;
			}


			if(pVehicle->m_OriginZoneID >= m_ZoneIDVector.size())
				continue;

			if(pVehicle->m_DestinationZoneID >= m_ZoneIDVector.size())
				continue;

			int OrgNo = m_ZoneIDVector[pVehicle->m_OriginZoneID];
			int DesNo = m_ZoneIDVector[pVehicle->m_DestinationZoneID];

			if(OrgNo<0 || DesNo< 0)
				continue;
			m_ODMOEMatrix[p][OrgNo][DesNo].TotalVehicleSize+=1;
			m_ODMOEMatrix[p][OrgNo][DesNo].TotalTravelTime += (pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime);
			m_ODMOEMatrix[p][OrgNo][DesNo].TotalDistance += pVehicle->m_Distance;
			m_ODMOEMatrix[p][OrgNo][DesNo].TotalCost += pVehicle->m_TollDollarCost;

			m_ODMOEMatrix[p][OrgNo][DesNo].emissiondata.Energy += pVehicle->m_EmissionData .Energy;
			m_ODMOEMatrix[p][OrgNo][DesNo].emissiondata.CO2 += pVehicle->m_EmissionData .CO2;
			m_ODMOEMatrix[p][OrgNo][DesNo].emissiondata.NOX += pVehicle->m_EmissionData .NOX;
			m_ODMOEMatrix[p][OrgNo][DesNo].emissiondata.CO += pVehicle->m_EmissionData .CO;
			m_ODMOEMatrix[p][OrgNo][DesNo].emissiondata.HC += pVehicle->m_EmissionData .HC;

		}
	}

	m_PreviousODSize = m_ODSize;

}


void CTLiteDoc::OnSensortoolsConverttoHourlyVolume()
{
	SensortoolsConverttoHourlyVolume();
	OpenCSVFileInExcel( m_ProjectDirectory +"export_validation_results.csv");
}
void CTLiteDoc::SensortoolsConverttoHourlyVolume()
{
	int calibration_data_start_time_in_min=0;
	int calibration_data_end_time_in_min= 1440;

	CCSVParser SettingFile;

	CCSVWriter DataFile;

	CString data_str = m_ProjectDirectory +"export_validation_results.csv";

	// Convert a TCHAR string to a LPCSTR
	if (DataFile.Open(CString2StdString(data_str)))
	{

		DataFile.SetFieldName("from_node_id");
		DataFile.SetFieldName("to_node_id");
		DataFile.SetFieldName("name");
		DataFile.SetFieldName("link_key");
		DataFile.SetFieldName("link_type_name");
		DataFile.SetFieldName("observed_link_count");
		DataFile.SetFieldName("simulated_link_count");
		DataFile.SetFieldName("observed_lane_density");
		DataFile.SetFieldName("simulated_lane_density");

		DataFile.SetFieldName("observed_link_speed");
		DataFile.SetFieldName("simulated_link_speed");

		DataFile.SetFieldName("start_time_in_min");
		DataFile.SetFieldName("end_time_in_min");
		DataFile.SetFieldName("sensor_type");
		DataFile.SetFieldName("aggregation_interval");
		DataFile.SetFieldName("start_time_in_hour");
		DataFile.SetFieldName("end_time_in_hour");
		DataFile.SetFieldName("reference_avg_speed");
		DataFile.SetFieldName("reference_from_node_id");
		DataFile.SetFieldName("reference_to_node_id");
		DataFile.SetFieldName("reference_matching_distance");

		DataFile.WriteHeader();

		//find refernce document pointer

		CTLiteDoc* pReferenceDoc = NULL;

		std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin();
		while (iDoc != g_DocumentList.end())
		{

			if ((*iDoc) != this && (*iDoc)->m_NodeSet.size() > 0)
			{
				pReferenceDoc = (*iDoc);
			}

			iDoc++;
		}

		CDlg_UserInput dlg_aggregation_interval;

		g_MOEAggregationIntervalInMin = 60;

		dlg_aggregation_interval.m_StrQuestion = "Please specify aggregation time interval in min:";
		dlg_aggregation_interval.m_InputValue = "60";

		if (dlg_aggregation_interval.DoModal() == IDOK)
		{
			g_MOEAggregationIntervalInMin = atoi(dlg_aggregation_interval.m_InputValue);

			if (g_MOEAggregationIntervalInMin<1)
				g_MOEAggregationIntervalInMin = 5;
		}


		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			DTALink* pLink = (*iLink);

			if (pLink->m_SensorDataVector.size()>0)
			{
				for (unsigned i = 0; i < pLink->m_SensorDataVector.size(); i++)
				{
					DTASensorData element = pLink->m_SensorDataVector[i];

					float SimulatedCount = pLink->GetAvgLinkHourlyVolume(element.start_time_in_min, element.end_time_in_min)*
						((element.end_time_in_min - element.start_time_in_min) / 60.0);

					int AggregationTimeIntervalInMin = element.end_time_in_min - element.start_time_in_min;
					DataFile.SetValueByFieldName("from_node_id", pLink->m_FromNodeNumber);
					DataFile.SetValueByFieldName("to_node_id", pLink->m_ToNodeNumber);
					DataFile.SetValueByFieldName("name", pLink->m_Name);
					DataFile.SetValueByFieldName("link_key", pLink->m_LinkKey);
					DataFile.SetValueByFieldName("link_type_name", m_LinkTypeMap[pLink->m_link_type].link_type_name);
					DataFile.SetValueByFieldName("sensor_type", "point detector");


					DataFile.SetValueByFieldName("start_time_in_min", element.start_time_in_min);

					DataFile.SetValueByFieldName("end_time_in_min", element.end_time_in_min);
					int start_time_in_hour = element.start_time_in_min / 60;
					DataFile.SetValueByFieldName("start_time_in_hour", start_time_in_hour);

					int end_time_in_hour = element.end_time_in_min / 60;
					DataFile.SetValueByFieldName("end_time_in_hour", end_time_in_hour);
					DataFile.SetValueByFieldName("aggregation_interval", AggregationTimeIntervalInMin);

					if (element.count >= 1)
					{
						DataFile.SetValueByFieldName("observed_link_count", element.count);
						DataFile.SetValueByFieldName("simulated_link_count", SimulatedCount);
					}

					if (element.density >= 0.01)
					{
						DataFile.SetValueByFieldName("observed_link_count", element.count);
						DataFile.SetValueByFieldName("simulated_link_count", SimulatedCount);

						DataFile.SetValueByFieldName("observed_lane_density", element.density);

						float max_density = 0;
						float SimulatedDensity = pLink->GetSimulatedDensityMOE(element.start_time_in_min, element.end_time_in_min, max_density);


						DataFile.SetValueByFieldName("simulated_lane_density", SimulatedDensity);
					}

					if (element.speed >= 0.01)
					{
						DataFile.SetValueByFieldName("observed_link_speed", element.speed);

						float simulated_link_speed = pLink->GetSimulatedSpeed(element.start_time_in_min, element.end_time_in_min);
						DataFile.SetValueByFieldName("simulated_link_speed", simulated_link_speed);

					}


					float reference_avg_speed = 0;

					if (pReferenceDoc != NULL)
					{
						float ratio = 0.5;
						GDPoint current_link_point = pLink->GetRelativePosition(ratio);

						double matching_distance = 1000;
						int linkid = pReferenceDoc->SelectLink(current_link_point, matching_distance);
						if (linkid >= 0)
						{
							DTALink* pReferenceLink = NULL;

							if (pReferenceDoc->m_LinkNoMap.find(linkid) != pReferenceDoc->m_LinkNoMap.end())
							{
								pReferenceLink = pReferenceDoc->m_LinkNoMap[linkid];
								reference_avg_speed = pReferenceLink->GetSimulatedSpeed(start_time_in_hour);

								int reference_from_node_id = pReferenceLink->m_FromNodeNumber;
								int reference_to_node_id = pReferenceLink->m_ToNodeNumber;


								double matching_distance_in_feet = matching_distance / m_UnitFeet;

								DataFile.SetValueByFieldName("reference_from_node_id", reference_from_node_id);
								DataFile.SetValueByFieldName("reference_to_node_id", reference_to_node_id);
								DataFile.SetValueByFieldName("reference_matching_distance", matching_distance_in_feet);


							}

						}

					}

					DataFile.SetValueByFieldName("reference_avg_speed", reference_avg_speed);

					DataFile.WriteRecord();


				}

			}

		}

		DataFile.CloseCSVFile();

	}
}


void CTLiteDoc::ReadSensorSpeedData(LPCTSTR lpszFileName, int speed_data_aggregation_interval)
{

	std::vector<string> Missing_TMC_vector;
	CCSVParser parser;
	int i= 0;
	if (parser.OpenCSVFile(lpszFileName))
	{

		g_Simulation_Time_Horizon = 1440;


		while(parser.ReadRecord())
		{

			string speed_sensor_id;
			if(parser.GetValueByFieldName("speed_sensor_id",speed_sensor_id) == false)
				break;

			string s_day_no;
			if(parser.GetValueByFieldName("day_no",s_day_no) == false)
				s_day_no = "1";

			int day_no = atoi(s_day_no.c_str ());

			g_SensorDayDataMap[ day_no] = true;
			g_SensorLastDayNo= max(g_SensorLastDayNo,  day_no);
			g_SensorDayNo = g_SensorLastDayNo;


			DTALink* pLink = NULL;

			if(m_SpeedSensorIDMap.find(speed_sensor_id)!=m_SpeedSensorIDMap.end())
			{

				pLink = m_SpeedSensorIDMap[speed_sensor_id];
				float max_speed = 10;
				float min_speed = 100;


				for (int t = 0; t< 1440; t+= speed_data_aggregation_interval)
				{
					CString timestamp;
					timestamp.Format ("Min_%d",t);
					std::string StdString = CString2StdString(timestamp);
					float speed_in_mph = -1;  // no data
					if(parser.GetValueByFieldName(StdString,speed_in_mph) != false && speed_in_mph >=1)
					{
						pLink->m_bSensorData = true;
						pLink->m_bSpeedSensorData = true;
							
							//TRACE("speed = %f,\n",speed_in_mph);
						if(min_speed > speed_in_mph)
							min_speed = speed_in_mph;

						if(max_speed < speed_in_mph)
							max_speed = speed_in_mph;

						for(int s= 0 ; s<15; s++)
						{

							int time = day_no*1440 + t +s;  // allow shift of start time
							// day specific value	

							if(pLink->m_LinkSensorMOEMap.find(time) == pLink->m_LinkSensorMOEMap.end()) // no traffic count data
								pLink->m_LinkSensorMOEMap[ time].LinkFlow = 1000;


							pLink->m_LinkSensorMOEMap[ time].Speed = speed_in_mph;

						}

					}

				}
				//reset speed limit
				pLink->m_SpeedLimit = max_speed;


				i++;
			}else
			{

				if(speed_sensor_id.size() > 0)
				{
				TRACE("speed_sensor_id not found: %s\n",speed_sensor_id.c_str ());

				Missing_TMC_vector.push_back(speed_sensor_id);
				}

			}


		}



		if(Missing_TMC_vector.size())
		{
			CCSVWriter DataFile;

			CString data_str = m_ProjectDirectory +"missing_speed_sensor_id.log";

			// Convert a TCHAR string to a LPCSTR
			if(DataFile.Open(CString2StdString(data_str)))
			{

				DataFile.SetFieldName ("speed_sensor_id");
				DataFile.WriteHeader ();


				for(unsigned i = 0; i < Missing_TMC_vector.size(); i++)
				{

					DataFile.SetValueByFieldName ("speed_sensor_id",Missing_TMC_vector[i] );

					DataFile.WriteRecord ();
				}


			}
			//		CString str;
			//		str.Format("%d TMC records cannot be found.", Missing_TMC_vector.size(), data_str);

		}
		m_bSimulationDataLoaded = true;

		g_Simulation_Time_Stamp = 0; // reset starting time
		g_SimulationStartTime_in_min = 0;

		m_SimulationStartTime_in_min = 0;
		m_SimulationEndTime_in_min = 1440;

		m_TrafficFlowModelFlag = 3; //enable dynamic moe display
		m_SensorSpeedDataLoadingStatus.Format ("%d speed records are loaded from file %s.\n",i,lpszFileName);


	}
}

int CTLiteDoc::SelectLink(GDPoint point, double& final_matching_distance)
{
	double Min_distance  = 99; // at least interset with a link

	int SelectedLinkNo = -1;

	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink !=m_LinkSet.end(); iLink++)
	{

		for(int si = 0; si < (*iLink) ->m_ShapePoints .size()-1; si++)
		{

			GDPoint p0 = point;
			GDPoint pfrom =  (*iLink)->m_ShapePoints[si];
			GDPoint pto = (*iLink)->m_ShapePoints[si+1];;

			float distance = g_GetPoint2LineDistance(p0, pfrom, pto, m_UnitMile);

			if(distance >0 && distance < Min_distance && (*iLink)->m_DisplayLinkID <0)  // not selected
			{

				m_SelectedLinkNo = (*iLink)->m_LinkNo ;
				Min_distance = distance;
				SelectedLinkNo = (*iLink)->m_LinkNo ;
			}
		}
	}

	final_matching_distance = Min_distance;

	return SelectedLinkNo;
}



void CTLiteDoc::OnTrafficcontroltoolsTransfermovementdatafromreferencenetworktocurrentnetwork()
{

	MapSignalDataAcrossProjects();
}

void CTLiteDoc::OnDemandtoolsGenerateinput()
{

	typedef struct  
	{
		int vehicle_id;
		int from_zone_id;
		int to_zone_id;
		float departure_time;
		float arrival_time;
		int complete_flag;
		float trip_time;
		int demand_type;
		int pricing_type;
		int vehicle_type;
		int information_type;
		float value_of_time;
		float toll_cost_in_dollar;
		float emissions;
		float distance_in_mile;
		int number_of_nodes;
		float Energy;
		float CO2;
		float NOX;
		float CO;
		float HC;

		int age;
		int version_no;

		int reserverd_field1;
		float reserverd_field2;
		int reserverd_field3;

	} struct_VehicleInfo_Header;






	FILE* st_struct = NULL;

	fopen_s(&st_struct,m_ProjectDirectory+"input_agent.bin","wb");


	typedef  struct  
	{
		int NodeName;
		float AbsArrivalTimeOnDSN;
	} struct_Vehicle_Node;

	if(st_struct!=NULL)
	{
		std::list<DTAVehicle*>::iterator iVehicle;

		//   int vehicle_id=0;//index for every vehicle 

		for (iVehicle = m_VehicleSet.begin(); iVehicle != m_VehicleSet.end(); iVehicle++)
		{

			DTAVehicle* pVehicle = (*iVehicle);

			if(pVehicle->m_NodeSize >= 2)  // with physical path in the network
			{
				int UpstreamNodeID = 0;
				int DownstreamNodeID = 0;

				float TripTime = 0;

				if(pVehicle->m_bComplete)
					TripTime = pVehicle->m_ArrivalTime-pVehicle->m_DepartureTime;


				float m_gap = 0;

				struct_VehicleInfo_Header header;
				header.vehicle_id = pVehicle->m_VehicleID-1; //start from 0
				header.from_zone_id = pVehicle->m_OriginZoneID;
				header. to_zone_id = pVehicle->m_DestinationZoneID;
				header. departure_time = pVehicle->m_DepartureTime;
				header. arrival_time = pVehicle->m_ArrivalTime;
				header. complete_flag = pVehicle->m_bComplete;
				header. trip_time = TripTime;
				header. demand_type = pVehicle->m_DemandType;
				header. pricing_type = pVehicle->m_PricingType;
				header. vehicle_type =pVehicle->m_VehicleType;
				header. information_type = pVehicle->m_InformationClass;
				header. value_of_time =pVehicle->m_VOT;
				header. toll_cost_in_dollar = pVehicle->m_TollDollarCost;
				header. distance_in_mile = pVehicle->m_Distance;
				header. number_of_nodes = pVehicle->m_NodeSize;

				struct_VehicleInfo_Header InfoHeaderAsVehicleInput;
				InfoHeaderAsVehicleInput.vehicle_id = pVehicle->m_VehicleID;
				InfoHeaderAsVehicleInput.from_zone_id = pVehicle->m_OriginZoneID;
				InfoHeaderAsVehicleInput. to_zone_id = pVehicle->m_DestinationZoneID;
				InfoHeaderAsVehicleInput. departure_time = pVehicle->m_DepartureTime;
				InfoHeaderAsVehicleInput. demand_type = pVehicle->m_DemandType;
				InfoHeaderAsVehicleInput. pricing_type = pVehicle->m_PricingType;
				InfoHeaderAsVehicleInput. vehicle_type =pVehicle->m_VehicleType;
				InfoHeaderAsVehicleInput. information_type = pVehicle->m_InformationClass;
				InfoHeaderAsVehicleInput. value_of_time =pVehicle->m_VOT;

				fwrite(&header, sizeof(struct_VehicleInfo_Header), 1, st_struct);


				int j = 0;


				struct_Vehicle_Node node_element;

				float LinkWaitingTime = 0;
				for(j = 1; j< pVehicle->m_NodeSize; j++)  // for all nodes
				{
					int LinkID = pVehicle->m_NodeAry [j].LinkNo;

					node_element. NodeName = m_LinkNoMap[LinkID]->m_FromNodeNumber ;
					node_element. AbsArrivalTimeOnDSN = 0;
					fwrite(&node_element, sizeof(node_element), 1, st_struct);

					if(j== pVehicle->m_NodeSize-1)
					{
						node_element. NodeName = m_LinkNoMap[LinkID]->m_ToNodeNumber ;
						node_element. AbsArrivalTimeOnDSN = 0;
						fwrite(&node_element, sizeof(node_element), 1, st_struct);
					}

				} //for all nodes in path

			}
		} // for all paths

		// not loaded in simulation


		fclose(st_struct);
	}

}

bool CTLiteDoc::ReadModelAgentTrajectory(LPCTSTR lpszFileName)
{
	CCSVParser parser;
	int i= 0;

	bool bRectInitialized = false;

	if (parser.OpenCSVFile(CString2StdString(lpszFileName)))
	{

			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			pMainFrame->m_bShowLayerMap[layer_vehicle_position] = true;
			//			m_bGPSDataSet  = true;
		while(parser.ReadRecord())
		{

			VehicleLocationRecord element;
			
			if(parser.GetValueByFieldName("time_stamp_in_second",element.time_stamp_in_second) == false)
				continue;

			if(parser.GetValueByFieldName("agent_id",element.agent_id) == false)
				continue;


			if(parser.GetValueByFieldName("x",element.x, false) == false)
				continue;

			if(parser.GetValueByFieldName("y",element.y, false) == false)
				continue;



			if(parser.GetValueByFieldName("to_x",element.to_x) == true)
			{
				if(parser.GetValueByFieldName("to_y",element.to_y) == true)
					element.b_to_data_flag  = true;
			}
				
			AddLocationRecord(element);
			
			if(!bRectInitialized)
			{
				m_NetworkRect.left = element.x ;
				m_NetworkRect.right = element.x;
				m_NetworkRect.top = element.y;
				m_NetworkRect.bottom = element.y;
				bRectInitialized = true;
			}

			GDPoint point;
			point.x = element.x;
			point.y = element.y;

			m_NetworkRect.Expand(point);
			
			
			i ++;

		}


///

			m_ZoneMap[0].m_ZoneID = 0;

				std::map<string,VehicleLocationTimeIndexedMap>::iterator itr2;

				for(itr2 = m_VehicleWithLocationVectorMap.begin();
					itr2 != m_VehicleWithLocationVectorMap .end(); itr2++)
				{		//scan all vehicle records at this timestamp


			DTAVehicle* pVehicle = 0;
			pVehicle = new DTAVehicle;
			pVehicle->m_bGPSVehicle = true;
			pVehicle->m_DayNo 		= 1;

			pVehicle->m_VehicleID		= m_VehicleSet.size();

			pVehicle->m_OriginZoneID	= 0;
			pVehicle->m_DestinationZoneID= 0;

			pVehicle->m_Distance = 10; // km to miles
			pVehicle->m_DepartureTime	=  itr2->second .VehicleLocationRecordVector[0].time_stamp_in_second/10;
			pVehicle->m_ArrivalTime =  itr2->second .VehicleLocationRecordVector[itr2->second .VehicleLocationRecordVector.size()-1].time_stamp_in_second;


			pVehicle->m_TripTime  = pVehicle->m_ArrivalTime - pVehicle->m_DepartureTime;

			if(g_Simulation_Time_Horizon < pVehicle->m_ArrivalTime)
				g_Simulation_Time_Horizon = pVehicle->m_ArrivalTime;

			pVehicle->m_bComplete = true;
			pVehicle->m_DemandType = 1;

			pVehicle->m_PricingType = 1;

			//pVehicle->m_VehicleType = (unsigned char)g_read_integer(pFile);

			pVehicle->m_VOT = 10;
			pVehicle->m_TollDollarCost = 0;
			pVehicle->m_PM = 0;

				m_VehicleSet.push_back (pVehicle);
				m_VehicleIDMap[pVehicle->m_VehicleID]  = pVehicle;


				}
	
	//	m_AgentLocationLoadingStatus.Format("%d agent location records for %d agents are loaded from file %s.",i,m_VehicleWithLocationVectorMap.size(),lpszFileName);	
	}
return true;
}



bool CTLiteDoc::ReadGPSTrajectory(LPCTSTR lpszFileName)
{
	CCSVParser parser;
	int i = 0;

	bool bRectInitialized = false;

	if (parser.OpenCSVFile(CString2StdString(lpszFileName)))
	{

		while (parser.ReadRecord())
		{

			VehicleLocationRecord element;

	

			if (parser.GetValueByFieldName("car_id", element.agent_id) == false)
				continue;


			string geo_string;
			if (parser.GetValueByFieldName("location", geo_string) == false)
				continue;

			std::vector<float> vect;

			std::stringstream ss(geo_string);
			float value;

			while (ss >> value)
			{
				vect.push_back(value);

				if (ss.peek() == ',')
					ss.ignore();
			}


			if (vect.size() == 2)
			{
				element.y = vect[0];
				element.x = vect[1];
			}


			string date_string;
			if (parser.GetValueByFieldName("date", date_string) == false)
				continue;

			std::vector<int> date_vect;

			std::stringstream ss_date(date_string);
			int date_value;

			while (ss_date >> date_value)
			{
				date_vect.push_back(date_value);

				if (ss_date.peek() == ',' || ss_date.peek() == ' ' || ss_date.peek() == '-' || ss_date.peek() == ':')
					ss_date.ignore();
			}

			if (date_vect.size() == 6)
			{
				int sec = date_vect[5] - date_vect[5] % 20;
				element.time_stamp_in_second = date_vect[3] * 3600 + date_vect[4] * 60 + sec;  // hour, min, second
			
				element.day_no = date_vect[2];
				if (element.time_stamp_in_second == 30780 && element.agent_no == 3)
				{ 
					TRACE("line no. %d", i);
				}
			}



			AddLocationRecord(element);

			if (!bRectInitialized)
			{
				m_NetworkRect.left = element.x;
				m_NetworkRect.right = element.x;
				m_NetworkRect.top = element.y;
				m_NetworkRect.bottom = element.y;
				bRectInitialized = true;
			}

			GDPoint point;
			point.x = element.x;
			point.y = element.y;

			m_NetworkRect.Expand(point);


			i++;

		}


		///

		m_ZoneMap[0].m_ZoneID = 0;

		std::map<string, VehicleLocationTimeIndexedMap>::iterator itr2;

		for (itr2 = m_VehicleWithLocationVectorMap.begin();
			itr2 != m_VehicleWithLocationVectorMap.end(); itr2++)
		{		//scan all vehicle records at this timestamp


			DTAVehicle* pVehicle = 0;
			pVehicle = new DTAVehicle;
			pVehicle->m_bGPSVehicle = true;
			pVehicle->m_DayNo = 1;

			pVehicle->m_VehicleID = m_VehicleSet.size();

			pVehicle->m_OriginZoneID = 0;
			pVehicle->m_DestinationZoneID = 0;

			pVehicle->m_Distance = 10; // km to miles
			pVehicle->m_DepartureTime = itr2->second.VehicleLocationRecordVector[0].time_stamp_in_second / 10;
			pVehicle->m_ArrivalTime = itr2->second.VehicleLocationRecordVector[itr2->second.VehicleLocationRecordVector.size() - 1].time_stamp_in_second;


			pVehicle->m_TripTime = pVehicle->m_ArrivalTime - pVehicle->m_DepartureTime;

			if (g_Simulation_Time_Horizon < pVehicle->m_ArrivalTime)
				g_Simulation_Time_Horizon = pVehicle->m_ArrivalTime;

			pVehicle->m_bComplete = true;
			pVehicle->m_DemandType = 1;

			pVehicle->m_PricingType = 1;

			//pVehicle->m_VehicleType = (unsigned char)g_read_integer(pFile);

			pVehicle->m_VOT = 10;
			pVehicle->m_TollDollarCost = 0;
			pVehicle->m_PM = 0;

			m_VehicleSet.push_back(pVehicle);
			m_VehicleIDMap[pVehicle->m_VehicleID] = pVehicle;


		}

	///	m_AgentLocationLoadingStatus.Format("%d agent location records for %d agents are loaded from file %s.", i, m_VehicleWithLocationVectorMap.size(), lpszFileName);
	}
	return true;
}


bool CTLiteDoc::ReadAimCSVFiles(LPCTSTR lpszFileName, int date_id)
{


	//   cout << "Read vehicle file... "  << endl;
	// vehicle_id,  origin_zone_id, destination_zone_id, departure_time,
	//	arrival_time, complete_flag, trip_time, demand_type, occupancy, information_type,
	//	value_of_time, path_min_cost,distance_in_mile, number_of_nodes,
	//	node id, node arrival time

	// step 1: read zone mapping files

	CString error_message_list;

	if(AfxMessageBox("Do you want to load Aimsun trajector and trip files?", MB_YESNO) == IDNO)
		return false;

	// coordinate conversion

	double m_XScale = 1;
	double m_YScale = 1;

	double Original_Point1_x=990312.078783;
	double Original_Point1_y=215179.584433;
	double Original_Point2_x=997335.415388;
	double Original_Point2_y=219143.052253;
	double Changed_Point1_x=-73.978119;
	double Changed_Point1_y=40.757293;
	double Changed_Point2_x=-73.95276;
	double Changed_Point2_y=40.768164;




	m_XScale = (Changed_Point2_x - Changed_Point1_x)/(Original_Point2_x - Original_Point1_x);
	m_YScale = (Changed_Point2_y - Changed_Point1_y)/(Original_Point2_y - Original_Point1_y);

	double m_XOrigin = Original_Point1_x - Changed_Point1_x /m_XScale;

	double m_YOrigin =  Original_Point1_y - Changed_Point1_y /m_YScale;



	std::map<int,int> AimsunZone2TAZMapping;

	CString taz_mapping_file = m_ProjectDirectory + "input_microsimulation_zone_to_TAZ_mapping.csv";
	CCSVParser parser;
	int i= 0;
	if (parser.OpenCSVFile(CString2StdString(taz_mapping_file)))
	{

		while(parser.ReadRecord())
		{

			int AimsunZone;
			if(parser.GetValueByFieldName("AimsunZone",AimsunZone) == false)
				break;
			int TAZ ;
			if(parser.GetValueByFieldName("TAZ",TAZ) == false)
				break;

			AimsunZone2TAZMapping[AimsunZone]= TAZ;

		}
	}else
	{
		CString msg;
		msg.Format ("Please check if file %s exists.", taz_mapping_file);  // +2 for the first field name line
		AfxMessageBox(msg);
		return false;

	}

	// step 2: read trip file

	CString trip_file = m_ProjectDirectory + "input_microsimulation_trip_data.csv";

	class SimpleVehicleData
	{
	public:
		long oid;
		int TAZ_origin;
		int TAZ_destination;
		float departure_time_in_min;
		float travel_time_in_min;
		float arrival_time_in_min;
	};

	std::map<long, SimpleVehicleData> SimpleVehicleDataMap;
	FILE* st = NULL;
	fopen_s(&st,trip_file,"r");

	long lineno = 1;
	int error_log_count = 0;
	std::map<int, int > missing_vehicle_id_map;
	if(st!=NULL)
	{
		char  str_line[2000]; // input string
		int str_line_size;
		g_read_a_line(st,str_line, str_line_size); //  skip the first line


		while(!feof(st))
		{

			int oid = -1;
			int origin= -1;
			int destination = -1;
			float entranceTime = 0;
			float exitTime = 0;
			float travelTime = 0;
			float delayTime = 0;

			fscanf(st,"%d,%d,%d,%f,%f,%f,%f\n", &oid, &origin, &destination, &entranceTime, &exitTime, &travelTime, &delayTime);

			if(oid<=0)  // invalid record;
				break;

			SimpleVehicleData element;
			element.oid = oid; 
			element.departure_time_in_min = entranceTime/60;
			element.travel_time_in_min = travelTime/60;
			element.arrival_time_in_min = exitTime/60;

			if(AimsunZone2TAZMapping.find(origin)!= AimsunZone2TAZMapping.end())
			{
				element.TAZ_origin = AimsunZone2TAZMapping[origin];
			}else
			{

				CString error_message;
				error_message.Format("Error: origin = %d does not have a TAZ mapping record. Line %d in file %s.",origin, lineno,  trip_file);

				AfxMessageBox(error_message);

				fclose(st);
				return false;
			}
			if(AimsunZone2TAZMapping.find(destination)!= AimsunZone2TAZMapping.end())
			{
				element.TAZ_destination  = AimsunZone2TAZMapping[destination];
			}else
			{

				CString error_message;
				error_message.Format("Error: destination = %d does not have a TAZ mapping record. Line %d in file %s.",origin, lineno,  trip_file);

				AfxMessageBox(error_message);

				fclose(st);
				return false;
			}

			SimpleVehicleDataMap[oid] = element;


			if(lineno%100 == 0 && lineno >=1)
			{
				CString str;
				str.Format ("Loading %d Aimsum trip records...", lineno);
				SetStatusText(str);
			}
			lineno++;
		}

	}else
	{
		CString msg;
		msg.Format ("Please check if file %s exists.", taz_mapping_file);  // +2 for the first field name line
		AfxMessageBox(msg);
		return false;

	}

	std::string trajectory_file = m_ProjectDirectory + "input_microsimulation_trajectory.csv";

	typedef struct 
	{
		int oid;
		int sectionId;
		float xCoord;
		float yCoord;
		float timeSta;
	} section_struc;

	std::vector<section_struc> section_vector; 
	std::vector<section_struc> valid_section_vector; 
	std::vector<section_struc> location_vector; 

	CString LogFile_str = m_ProjectDirectory+"microsimulation_data_error_log.csv";
	FILE * st_log;
	st_log = fopen(LogFile_str,"w");

	CString LogFile_str2 = m_ProjectDirectory+"microsimulation_data_log.csv";
	FILE * st_log2;
	st_log2 = fopen(LogFile_str2,"w");

	if(st_log2!=NULL)
	{
		fprintf(st_log2, "vehicle id, origin TAZ, destination TAZ, departure time in min, hour, section size, valid section size, diff,last section id\n");			
	}

	fopen_s(&st,lpszFileName,"r");

	lineno = 1;
	int valid_vehicle_count = 0;
	if(st!=NULL)
	{
		char  str_line[2000]; // input string
		int str_line_size;
		g_read_a_line(st,str_line, str_line_size); //  skip the first line

		int last_valid_oid= 0;
		int last_valid_sectionId= 0;


		while(!feof(st))
		{

			long oid= 0;
			long sectionId = 0;
			float xCoord = 0;
			float yCoord = 0;
			float timeSta = 0;

			section_struc element;

			//oid = g_read_integer(st);
			//if(oid<=0)
			//	break;

			//sectionId= g_read_integer(st);
			//if(sectionId<=0)
			//	break;
			//xCoord = g_read_float(st);
			//yCoord = g_read_float(st);

			//timeSta = g_read_float(st);

			fscanf(st,"%d,%d,%f,%f,%f\n", &oid, &sectionId, &xCoord, &yCoord, &timeSta);

			if(oid == 30026)
			{
				TRACE("section id: %d\n", sectionId);
			}


			element.oid = oid;
			element.sectionId = sectionId;
			element.xCoord = xCoord;
			element.yCoord = yCoord;
			element.timeSta = timeSta/60;


			if(last_valid_oid==0)  //initial condition 
			{
				last_valid_oid = oid;

			}

			element.oid = oid;
			element.sectionId = sectionId;
			element.xCoord = xCoord;
			element.yCoord = yCoord;

			if(oid!=last_valid_oid)  //finish all records
			{
				//add section id sequence 

				if( SimpleVehicleDataMap.find(last_valid_oid)  != SimpleVehicleDataMap.end())
				{	
					if(section_vector.size()>=2)  // find vehicle id
					{

						valid_section_vector.clear();

						for(unsigned k = 0; k< section_vector.size(); k++)
						{
							DTALink* pLink = FindLinkWithLinkID(section_vector[k].sectionId);
							if(pLink!=NULL)
							{
								valid_section_vector.push_back(section_vector[k]);
							}

						}


						if(valid_section_vector.size() >=2)  // at least two valid sections
						{

							if(last_valid_oid==2)
								TRACE("oid = 2!");

							DTAVehicle* pVehicle = new DTAVehicle;
							//			pVehicle->m_bGPSVehicle = true;
							pVehicle->m_DayNo 		= 0;

							SimpleVehicleData se =  SimpleVehicleDataMap[last_valid_oid];

							pVehicle->m_VehicleID	= se.oid;
							pVehicle->m_OriginZoneID	= se.TAZ_origin;
							pVehicle->m_DestinationZoneID= se.TAZ_destination;

							pVehicle->m_DepartureTime	=  se.departure_time_in_min;
							pVehicle->m_ArrivalTime =  se.arrival_time_in_min;
							pVehicle->m_TripTime  =  se.travel_time_in_min;

							if(g_Simulation_Time_Horizon < pVehicle->m_ArrivalTime)
								g_Simulation_Time_Horizon = pVehicle->m_ArrivalTime;

							pVehicle->m_bComplete = true;
							pVehicle->m_DemandType = 1;
							pVehicle->m_PricingType = 1;
							pVehicle->m_VOT = 10;
							pVehicle->m_TollDollarCost = 0;
							pVehicle->m_PM = 0;

							pVehicle->m_VehicleLocationSize	= 0;

							pVehicle->m_NodeSize = valid_section_vector.size()+1;
							pVehicle->m_NodeAry = new SVehicleLink[pVehicle->m_NodeSize];
							pVehicle->m_NodeNumberSum = 0;


							pVehicle->m_NodeAry[0].ArrivalTimeOnDSN = valid_section_vector[0].timeSta;
							for(int i = 1; i< valid_section_vector.size(); i++)
							{
								DTALink* pLink = FindLinkWithLinkID(valid_section_vector[i-1].sectionId);
								if(pLink!=NULL)
								{
									pVehicle->m_NodeAry[i-1].LinkNo  = pLink->m_LinkNo ;
									pVehicle->m_FreeflowTripTime += pLink->m_FreeFlowTravelTime ;
									pVehicle->m_NodeAry[i].ArrivalTimeOnDSN = valid_section_vector[i].timeSta;
									pVehicle->m_NodeNumberSum += pLink->m_FromNodeNumber ;

									pVehicle->m_Distance += pLink->m_Length ;

								}
							}

							DTALink* pLastLink = FindLinkWithLinkID(valid_section_vector[valid_section_vector.size()-1].sectionId);
							if(pLastLink!=NULL)
							{
								pVehicle->m_NodeAry[valid_section_vector.size()].LinkNo  = pLastLink->m_LinkNo ;
								pVehicle->m_NodeNumberSum += pLastLink->m_FromNodeNumber ;
								pVehicle->m_Distance += pLastLink->m_Length ;

							}

							pVehicle->m_VehicleLocationSize = 0;

							bool load_location_data = false;

							if(load_location_data)
							{

								pVehicle->m_VehicleLocationSize = location_vector.size();
								pVehicle->m_LocationRecordAry = new VehicleLocationRecord[location_vector.size()];


								for(int i=0; i< pVehicle->m_VehicleLocationSize; i++)
								{
									VehicleLocationRecord location_element;

									location_element.x = (location_vector[i].xCoord - m_XOrigin)*m_XScale;
									location_element.y = (location_vector[i].yCoord - m_YOrigin)*m_YScale;
									location_element.time_stamp_in_min = location_vector[i].timeSta;
									location_element.distance_in_km = 0.0001;
									location_element.link_no = location_vector[i].sectionId;

									pVehicle->m_LocationRecordAry[i] = location_element;
								}


							}
							m_VehicleSet.push_back (pVehicle);
							m_VehicleIDMap[pVehicle->m_VehicleID]  = pVehicle;
							valid_vehicle_count++;

							if(st_log2!=NULL)
							{

								fprintf(st_log2, "%d,%d,%d,%.2f,%.2f,%d,%d,%d,%d\n", pVehicle->m_VehicleID , pVehicle->m_OriginZoneID , pVehicle->m_DestinationZoneID , pVehicle->m_DepartureTime,pVehicle->m_DepartureTime/60,section_vector.size(), valid_section_vector.size(),section_vector.size()- valid_section_vector.size(),last_valid_sectionId );
							}

						}else
						{  // output vehicle data with invalid section ids
							if(st_log!=NULL)
							{
								fprintf(st_log, "veh %d,# of valid sections <=1 \n",last_valid_oid);
							}
						}

						last_valid_oid = oid;
						section_vector.clear();
						section_vector.push_back (element);  // first section of the next vehicle
						location_vector.clear();
						location_vector.push_back (element);  // first section of the next vehicle

					}else
					{
						if(st_log!=NULL && error_log_count<1000)
						{
							fprintf(st_log, "veh %d,# of valid sections <=1 \n",last_valid_oid);
						}			
					}
				}else
				{
					if(st_log!=NULL && error_log_count<1000)
					{  

						if(missing_vehicle_id_map.find(last_valid_oid)==missing_vehicle_id_map.end())
						{
							fprintf(st_log, "missing vehicle id %d, which has not been defined in trip file\n", last_valid_oid );
							missing_vehicle_id_map[last_valid_oid] =1;
							error_log_count++;
						}

						last_valid_oid = 0; // reset vehicle id to 0, so the remaining vehicles can be read
					}
				}

			}else if(last_valid_sectionId!= sectionId) 
			{
				last_valid_sectionId = sectionId;
				section_vector.push_back (element);
			}

			location_vector.push_back(element);

			if(valid_vehicle_count%1000 == 0 && valid_vehicle_count >=1)
			{
				CString str;
				str.Format ("Loading %d K trip trajectories...", valid_vehicle_count);
				SetStatusText(str);
			}

			lineno++;
		}

		m_SimulationVehicleDataLoadingStatus.Format ("%d trips are loaded from trajector files...", valid_vehicle_count);

		AfxMessageBox(m_SimulationVehicleDataLoadingStatus, MB_ICONINFORMATION );

		//if(error_message_list.GetLength ()>=1)
		//	AfxMessageBox(error_message_list);

		std::list<DTAVehicle*>::iterator iVehicle;

		for (iVehicle = m_VehicleSet.begin(); iVehicle != m_VehicleSet.end(); iVehicle++)
		{
			DTAVehicle* pVehicle = (*iVehicle);

			if(pVehicle->m_bComplete == false && pVehicle->m_NodeSize ==0)
			{
				if(st_log!=NULL)
				{
					fprintf(st_log, "%d,%.2f,%.2f do not have valid section records\n", pVehicle->m_VehicleID ,  pVehicle->m_DepartureTime,pVehicle->m_DepartureTime/60);
				}			

			}
		}

	}
	else
	{
		CString msg;
		msg.Format("File %s cannot be opened.", trajectory_file.c_str());
		AfxMessageBox(msg);
		return false;
	}

	if(st_log!=NULL)
		fclose(st_log);

	if(st_log2!=NULL)
		fclose(st_log2);


	if(m_VehicleSet.size() >=1 ) //with GPS data
	{
		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
		pMainFrame->m_bShowLayerMap[layer_vehicle_position]  = true;
		//			m_bGPSDataSet  = true;
	}

	return true;


}


void CTLiteDoc::OnDemandReconstructlinkmoeth()
{
	if(AfxMessageBox("Do you want to use vehicle trajectory data to reconstruct link MOEs(inflow, outflow counts, density)?", MB_YESNO) == IDYES)
	{
		CWaitCursor wait;
		RecalculateLinkMOEFromVehicleTrajectoryFile();
	}

}

void CTLiteDoc::OnNetworktoolsResetlinklength()
{
	if(AfxMessageBox("Do you want to use long/lat coordinate data in the link shape file to recalculate the link length?", MB_YESNO) == IDYES)
	{
		CWaitCursor wait;

		double total_length = 0;

		double default_coordinate_distance_sum = 0;

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->m_Length  = 0;


			for(unsigned int si = 0; si < (*iLink) ->m_ShapePoints .size()-1; si++)
			{
				double distance  =  g_CalculateP2PDistanceInMileFromLatitudeLongitude((*iLink)->m_ShapePoints[si] , (*iLink)->m_ShapePoints[si+1]);
				(*iLink)->m_Length += distance;
			}

			total_length+= (*iLink)->m_Length;

			default_coordinate_distance_sum+= (*iLink)->DefaultDistance();


		}

		if(total_length>0.000001f)
		{
			m_UnitMile= default_coordinate_distance_sum / total_length ;
			m_UnitFeet = m_UnitMile/5280.0f;  

			m_bUnitMileInitialized = true;  // the unit mile has been reset according to the actual distance 
		}


		CString str;
		str.Format("The updated average link length per link is now %.3f.\nPlease save the project to confirm the change.", total_length/max(1,m_LinkSet.size() ));
		AfxMessageBox(str,MB_ICONINFORMATION);

	}
}

void CTLiteDoc::UpdateUnitMile()
{
	CWaitCursor wait;

	double total_length_in_mile = 0;

	double default_coordinate_distance_sum = 0;

	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		total_length_in_mile+= (*iLink)->m_Length;

		default_coordinate_distance_sum+= (*iLink)->DefaultDistance();


	}

	if(total_length_in_mile>0.000001f)
	{
		m_UnitMile= default_coordinate_distance_sum / total_length_in_mile ;
		m_UnitFeet = m_UnitMile/5280.0f;  

		m_bUnitMileInitialized = true;  // the unit mile has been reset according to the actual distance 
	}


}

void CTLiteDoc::OnSubareaCreatezonefromsubarea()
{
	int zone_number = -1;  // starting default number 


//	PushBackNetworkState();


	Modify ();
	if(m_SubareaNodeSet.size()>0 )
	{
		m_PreviousODSize  = -1; // require to regenerate od zone vector

		DTANode* pNode = m_SubareaNodeSet[0];



		if(m_ZoneMap.find(pNode->m_NodeNumber)==m_ZoneMap.end())
		{
			zone_number = pNode->m_NodeNumber ;

			m_ZoneMap [zone_number].m_ZoneID = zone_number;
			m_ZoneMap [zone_number].SetNodeActivityMode(pNode->m_NodeNumber,0);

		}else
		{  // Zone has been defined. we can just update zone boundary

			// this node has a zone Id already, we just update their zone boundary 
			if(m_ZoneMap.find(pNode->m_ZoneID)!=m_ZoneMap.end())
			{

				zone_number = pNode->m_ZoneID;
				m_ZoneMap [zone_number].m_ShapePoints.clear ();

				for (unsigned int sub_i= 0; sub_i < m_SubareaShapePoints.size(); sub_i++)
				{
					m_ZoneMap [zone_number].m_ShapePoints .push_back (m_SubareaShapePoints[sub_i]);

				}



			}


			return;
		}

	}
	if(zone_number==-1)
	{ // find largest zone number for the new zone


		std::map<int, DTAZone>	:: const_iterator itr;

		for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
		{
			if( zone_number <= itr->first)
				zone_number = itr->first +1;

		}

		if(zone_number==-1)  // no zone has been defined yet
			zone_number = 1; 

		m_ZoneMap [zone_number].m_ZoneID = max(1,zone_number);

	}


	for (unsigned int sub_i= 0; sub_i < m_SubareaShapePoints.size(); sub_i++)
	{
		m_ZoneMap [zone_number].m_ShapePoints .push_back (m_SubareaShapePoints[sub_i]);

	}


	// add activity locations if there is no activity location being assigned yet
	RegenerateactivitylocationsForEmptyZone(zone_number);

	m_SubareaShapePoints.clear();
	UpdateAllViews(0);
}


bool CTLiteDoc::FindObject(eSEARCHMODE SearchMode, int value1, int value2)
{

	if(SearchMode == efind_link)
	{

		DTALink* pLink = FindLinkWithNodeNumbers(value1 ,value2 );

		if(pLink !=NULL)
		{
			m_SelectedLinkNo = pLink->m_LinkNo ;
			m_SelectedNodeID = -1;

			ZoomToSelectedLink(pLink->m_LinkNo);


		}
	}

	if(SearchMode == efind_node)
	{
		DTANode* pNode = FindNodeWithNodeNumber (value1);
		if(pNode !=NULL)
		{
			m_SelectedLinkNo = -1;
			m_SelectedNodeID = pNode->m_NodeNo ;

			ZoomToSelectedNode(value1);

		} return false;
	}

	if(SearchMode == efind_path)
	{

		DTANode* pFromNode = FindNodeWithNodeNumber (value1);
		if(pFromNode ==NULL)
		{
			CString str;
			str.Format ("From Node %d cannot be found.",value1);
			AfxMessageBox(str);
			return false; 
		}else
		{
			m_OriginNodeID = pFromNode->m_NodeNo;

		}
		DTANode* pToNode = FindNodeWithNodeNumber (value2);
		if(pToNode ==NULL)
		{
			CString str;
			str.Format ("To Node %d cannot be found.",value2);
			AfxMessageBox(str);
			return false;
		}else
		{
			m_DestinationNodeID = pToNode->m_NodeNo;
		}

		Routing(false);

	}

	if (SearchMode == efind_sensor)
	{

			m_SelectedLinkNo = -1;
			m_SelectedNodeID = -1;

			CString value_str;
			value_str.Format("%d", value1);

			ZoomToSelectedSensor(CString2StdString(value_str));

			return false;
	}

	if(SearchMode == efind_vehicle)
	{
		int SelectedVehicleID = value1; // internal vehicle index starts from zero
		std::vector<int> LinkVector;

		if(m_VehicleIDMap.find(SelectedVehicleID) == m_VehicleIDMap.end())
		{
			if(SelectedVehicleID>=0)
			{
				CString str_message;
				str_message.Format ("Vehicle Id %d cannot be found.", SelectedVehicleID+1);
				AfxMessageBox(str_message);
			}

			m_SelectedVehicleID = -1;
			HighlightPath(LinkVector,1);

			return true;
		}

		m_SelectedVehicleID = SelectedVehicleID;

		DTAVehicle* pVehicle = m_VehicleIDMap[SelectedVehicleID];

		CPlayerSeekBar m_wndPlayerSeekBar;


		// set departure time to the current time of display
		g_Simulation_Time_Stamp = pVehicle->m_DepartureTime +1;

		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();

		pMainFrame->m_wndPlayerSeekBar.SetPos(g_Simulation_Time_Stamp);

		m_LinkMOEMode = MOE_vehicle;

		for(int link= 1; link<pVehicle->m_NodeSize; link++)
		{
			LinkVector.push_back (pVehicle->m_NodeAry[link].LinkNo);
		}

		HighlightPath(LinkVector,1);

		m_HighlightGDPointVector.clear();

		m_HighlightGDPointVector = pVehicle->m_GPSLocationVector ; // assign the GPS points to be highlighted

	}
	UpdateAllViews(0);

	return true;
}

void CTLiteDoc::RegenerateactivitylocationsForEmptyZone(int zoneid)
{
	if(m_ZoneMap[zoneid].m_ActivityLocationVector.size()<0)
		return;

	// scan all nodes 
	std::list<DTANode*>::iterator iNode;
	for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{

		DTANode* pNode = (*iNode);
		int nodeid = pNode->m_NodeNo ;

		// three conditions: not pretimed signal, not actuated signal, not connected to freeway and ramp
		if(pNode->m_ControlType != m_ControlType_PretimedSignal && pNode->m_ControlType != m_ControlType_ActuatedSignal && pNode->m_bConnectedToFreewayORRamp == false)
		{

			if(m_ZoneMap[zoneid].IsInside (pNode->pt))
			{

				DTAActivityLocation element;
				element.ZoneID  = zoneid;
				element.NodeNumber = pNode->m_NodeNumber ;
				element.pt = pNode ->pt;

				int External_OD_Flag  = 0;

				m_NodeNotoZoneNameMap[nodeid] = zoneid;
				m_NodeNoMap [nodeid ] -> m_ZoneID = zoneid;
				m_NodeNoMap [nodeid] ->m_External_OD_flag = 0;

				m_ZoneMap[zoneid].m_ActivityLocationVector.push_back (element);

			}


		} 


	} // for each node
}
void CTLiteDoc::OnDemandRegenerateactivitylocations()
{
	CWaitCursor wait;

	if(m_ZoneMap.size()>=100)
	{
		if(AfxMessageBox("There are more than 100 zones, so reidentifing all activity locations might take a while.\nDo you wan to continue?", MB_YESNO|MB_ICONINFORMATION)==IDNO)
			return;

	}

	int activity_node_count = 0; 
	CString message_vector;

	std::vector<int> zones_without_activity_nodes;

	//reset activity flag
	std::list<DTANode*>::iterator iNode;
	for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{
		DTANode* pNode = (*iNode);
		pNode->m_bZoneActivityLocationFlag  = false;

		pNode->m_ZoneID = 0;
	}
	std::map<int, DTAZone>	:: iterator itr;

	for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++)
	{
		(*itr).second .m_ActivityLocationVector .clear();

		int zoneid = (*itr).first;

		RegenerateactivitylocationsForEmptyZone(zoneid);

		activity_node_count+=(*itr).second.m_ActivityLocationVector.size();

		if((*itr).second.m_ActivityLocationVector.size() ==0)
		{
			zones_without_activity_nodes.push_back (zoneid);


		}

	}  // for each zone


	if(zones_without_activity_nodes.size()>0)
	{
		message_vector = "The following zones do not have any activity location/node being assigned.\n";

		for(unsigned int i= 0; i < zones_without_activity_nodes.size(); i++)
		{
			CString str;
			str.Format("%d,",zones_without_activity_nodes[i]);
			message_vector+= str;

		}

		AfxMessageBox(message_vector);

	}else
	{

		CString str;
		str.Format("%d nodes have been assigned as activcity locations.",activity_node_count);

		AfxMessageBox(str,MB_ICONINFORMATION);

	}

}


void CTLiteDoc::OnTrafficcontroltoolsTransfersignaldatafromreferencenetworktocurrentnetwork()
{
	// step 0: make sure two networks have been opened

	CTLiteDoc* pReferenceDoc = NULL;
	std::list<CTLiteDoc*>::iterator iDoc = g_DocumentList.begin ();
	int project_index = 0 ;
	while (iDoc != g_DocumentList.end())
	{
		if((*iDoc)->m_NodeSet.size() >0 && (*iDoc)!=this)  
		{

			pReferenceDoc = (*iDoc);

			project_index++;
		}
		iDoc++;
	}

	if(pReferenceDoc == NULL)
	{

		AfxMessageBox("Please open the reference network data set in anoter window.");
		return;
	}

	int count_node = 0 ;
	int count_link = 0 ;
	std::list<DTANode*>::iterator  iNode;


	FILE* st = NULL;

	CString str_log_file;
	str_log_file.Format ("%s\\signal_data_reading_log.csv",m_ProjectDirectory);

	fopen_s(&st,str_log_file,"w");


	if( st !=NULL)
	{


		//	fprintf(st,"intersection_name1,intersection_name2, baseline_node_id,reference_node_id,base_line_cycle_length_in_second,reference_cycle_length_in_second\n");


		fprintf(st,"------\n");
		fprintf(st,"Step 2: Read Cycle length and offset,\n");

		for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{

			int baseline_node_id = (*iNode)->m_NodeNumber  ;
			if((*iNode)->m_ControlType == m_ControlType_PretimedSignal || (*iNode)->m_ControlType == m_ControlType_ActuatedSignal)
			{

				DTANode* pRefNode = NULL;
				int reference_node_number =  (*iNode)->m_NodeNumber;


				if( pReferenceDoc->m_NodeNumbertoNodeNoMap.find(reference_node_number) !=  pReferenceDoc->m_NodeNumbertoNodeNoMap.end())						
				{
					pRefNode = pReferenceDoc->m_NodeNumberMap[reference_node_number];

					if(pRefNode->m_ControlType == pReferenceDoc->m_ControlType_PretimedSignal || pRefNode->m_ControlType == pReferenceDoc->m_ControlType_ActuatedSignal)
					{

						fprintf(st,"Node,%d,obtains cycle length =,%d,updated cycle length,%d,\n", 
							reference_node_number,
							(*iNode)->m_CycleLengthInSecond,
							pRefNode ->m_CycleLengthInSecond );

						(*iNode)->m_CycleLengthInSecond = pRefNode ->m_CycleLengthInSecond ;
						(*iNode)->m_SignalOffsetInSecond = pRefNode ->m_SignalOffsetInSecond  ;
						count_node++;

					}
				}

			}

		}

		// step 2: fetch data for all links
		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			DTALink* pThisLink = (*iLink);
			DTALink* pReferenceLink =  pReferenceDoc->FindLinkWithNodeNumbers (pThisLink->m_FromNodeNumber ,pThisLink->m_ToNodeNumber );

			DTANode* pDownstreamNode = m_NodeNumberMap[pThisLink->m_ToNodeNumber ];

			if(pReferenceLink!=NULL && 
				(pDownstreamNode->m_ControlType == m_ControlType_PretimedSignal || pDownstreamNode->m_ControlType == m_ControlType_ActuatedSignal))
			{
				DTANode* pRefNode = pReferenceDoc->m_NodeNumberMap[pThisLink->m_ToNodeNumber ];

				if(pRefNode->m_ControlType == pReferenceDoc->m_ControlType_PretimedSignal || pRefNode->m_ControlType == pReferenceDoc->m_ControlType_ActuatedSignal)
				{

					fprintf(st,"Link,%d->%d,  Old # of left turn lanes,%d, Updated of left turn lanes,%d,Old # of right turn lanes,%d, Updated of right turn lanes,%d\n", 
						pThisLink->m_FromNodeNumber,pThisLink->m_ToNodeNumber,
						pThisLink->m_NumberOfLeftTurnLanes,
						pReferenceLink->m_NumberOfLeftTurnLanes,
						pThisLink->m_NumberOfRightTurnLanes ,
						pReferenceLink->m_NumberOfRightTurnLanes
						);

					pThisLink->m_NumberOfLeftTurnLanes   = pReferenceLink->m_NumberOfLeftTurnLanes;
					pThisLink->m_NumberOfRightTurnLanes    = pReferenceLink->m_NumberOfRightTurnLanes;


					count_link++;
				}
			}
		}


		fclose(st);
	}else
	{
		CString str;
		str.Format("File %s cannot be opened.", str_log_file);
		AfxMessageBox(str);
	}


	OpenCSVFileInExcel(str_log_file);

	CString str;
	str.Format("%d nodes and %d links have obtained cycle length and signal timing from the reference network. To accept the changes, please save the network.",count_node, count_link);
	AfxMessageBox(str, MB_ICONINFORMATION);

}

void CTLiteDoc::OnImportBackgroundimage()
{

	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_LONGNAMES|OFN_ENABLESIZING,
		_T("background image file(*.bmp)|*.bmp|"),NULL,0);
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CString bitmap_string  = "//importing_sample_data_sets";
	CString NetworkFile = pMainFrame->m_CurrentDirectory + bitmap_string;
	dlg.m_ofn.lpstrInitialDir = NetworkFile ;


	if(dlg.DoModal() == IDOK)
	{

		if(m_ProjectFile.GetLength ()==0)
		{

			if(AfxMessageBox("Please specify a new transportation network project folder and project file (*.tnp).\nTo avoid data inconsistency, please do not overwrite an existing project/data folder with simulation results already.",MB_ICONINFORMATION)==IDOK)
			{

				OnFileSaveProjectAs();
				CopyFile(dlg.GetPathName (),m_ProjectDirectory+"background_image.bmp",false);
				if(ReadBackgroundImageFile(m_ProjectFile)==true)
				{
					CDlg_RealWorldWidth dlg_width;
					if(dlg_width.DoModal ()==IDOK)
					{
						m_ImageX1 = 0;
						m_ImageY1 = 0;
						m_ImageX2 = dlg_width.m_RealWorldWidth;
						m_ImageWidth = dlg_width.m_RealWorldWidth;
						double height = m_BackgroundBitmap.GetHeight();
						double width  = m_BackgroundBitmap.GetWidth ();
						m_ImageY2 = height/width * dlg_width.m_RealWorldWidth;
						m_ImageWidth = fabs(m_ImageX2 - m_ImageX1);
						m_ImageHeight = fabs(m_ImageY2 - m_ImageY1);

						CalculateDrawingRectangle();


					}

				}
			}

		}else
		{
			CopyFile(dlg.GetPathName (),m_ProjectDirectory+"background_image.bmp",false);
			m_ImageX1  = m_NetworkRect.left;
			m_ImageY1  = m_NetworkRect.bottom;
			m_ImageX2 = m_NetworkRect.right;
			m_ImageY2 = m_NetworkRect.top;
			m_ImageWidth = fabs(m_ImageX2 - m_ImageX1);
			m_ImageHeight = fabs(m_ImageY2 - m_ImageY1);

		}

	}
	m_bFitNetworkInitialized  = false;
	CalculateDrawingRectangle(false);
	UpdateAllViews(0);
}

void CTLiteDoc::OnZoneDeletezone()
{
	Modify ();

	m_ZoneMap.erase (this->m_SelectedZoneID );
	this->m_SelectedZoneID = -1;
	UpdateAllViews(0);


}





void CTLiteDoc::OnZoneViewzonedata()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project has not been loaded.");
		return;
	}
	CNetworkDataSettingDlg dlg;
	dlg.m_SelectTab = _ZONE_DATA; // _ZONE_DATA
	dlg.m_pDoc = this;

	//find zone sequence no from zone id

	int zi = 0;
	std::map<int, DTAZone>	:: const_iterator itr;
	for(itr = m_ZoneMap.begin(); itr != m_ZoneMap.end(); itr++,zi++)
	{
		if((*itr).first  == m_SelectedZoneID)
		{
			dlg.m_SelectRowByUser = zi;

		}

	}

	dlg.DoModal();
}

void CTLiteDoc::OnNodeViewnodedata()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project has not been loaded.");
		return;
	}
	CNetworkDataSettingDlg dlg;
	dlg.m_SelectTab = _NODE_DATA; 
	dlg.m_pDoc = this;

	if(m_SelectedNodeID>= 0 )
	{
		dlg.m_SelectRowByUser  = m_SelectedNodeID;

	}
	dlg.DoModal();
}

void CTLiteDoc::OnLinkViewlinkdata()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project has not been loaded.");
		return;
	}
	CNetworkDataSettingDlg dlg;
	dlg.m_SelectTab = _LINK_DATA; 
	dlg.m_pDoc = this;

	dlg.m_SelectRowByUser =  m_SelectedLinkNo;
	dlg.DoModal();


}

void CTLiteDoc::OnMovementViewmovementdatatable()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project has not been loaded.");
		return;
	}
	CNetworkDataSettingDlg dlg;
	dlg.m_SelectTab = _MOVEMENT_DATA; 
	dlg.m_pDoc = this;
	dlg.DoModal();
}

void CTLiteDoc::OnOdmatrixOddemandmatrix()
{
	// TODO: Add your command handler code here
}

void CTLiteDoc::OnWorkzoneViewworkzonedata()
{
	CDlgScenario dlg;
	dlg.m_pDoc = this;
	dlg.m_SelectTab = _WORKZONE;
	dlg.DoModal();
}

void CTLiteDoc::OnVmsViewvmsdatatable()
{
	CDlgScenario dlg;
	dlg.m_pDoc = this;
	dlg.m_SelectTab = _DYNMSGSIGN;
	dlg.DoModal();
}

void CTLiteDoc::OnTollViewtolldatatable()
{
	CDlgScenario dlg;
	dlg.m_pDoc = this;
	dlg.m_SelectTab = _LINKBASEDTOLL;
	dlg.DoModal();
}

void CTLiteDoc::OnDetectorViewsensordatatable()
{
	CSensorNetworkDataSettingDlg dlg;
	dlg.m_SelectTab = _SENSOR_LINK_DATA; 
	dlg.m_pDoc = this;
	dlg.DoModal();
}

void CTLiteDoc::OnConnectorViewactivitylocationdatatable()
{
	CNetworkDataSettingDlg dlg;
	dlg.m_SelectTab = _ACTIVITY_LOCATION_DATA; 
	dlg.m_pDoc = this;
	dlg.DoModal();
}

void CTLiteDoc::OnDetectorViewcalibration()
{
	CNetworkDataSettingDlg dlg;
	dlg.m_SelectTab = _CALIBRATION_RESULT_DATA; 
	dlg.m_pDoc = this;
	dlg.DoModal();
}

void CTLiteDoc::OnLinkmoeExportlinkmoedatatoshapefile()
{
	ExportToGISFile(m_ProjectDirectory + "output_LinkMOE.csv", m_ProjectDirectory + "output_LinkMOE.shp", GIS_Line_Type);

}

void CTLiteDoc::ExportToGISFile(LPCTSTR lpszCSVFileName,LPCTSTR lpszShapeFileName, _GIS_DATA_TYPE GIS_data_type)
{

#ifndef _WIN64

	CWaitCursor wait;
	CString GISTypeString = "ESRI Shapefile";
	CString ShapeFileName;

	ShapeFileName.Format("%s",lpszShapeFileName);

	CString ShapeFileName_WithoutExtension = ShapeFileName.Left(ShapeFileName.ReverseFind('.') + 1);

	DeleteFile(ShapeFileName_WithoutExtension+"shp");  // delete shape file first
	DeleteFile(ShapeFileName_WithoutExtension+"dbf");  // delete shape file first
	DeleteFile(ShapeFileName_WithoutExtension+"shx");  // delete shape file first

	CCSVParser parser;
	int i= 0;

	CString message_str;

	// open csv file
	if (parser.OpenCSVFile(lpszCSVFileName))
	{

		CString message_str;

		OGRSFDriver *poDriver;

		OGRRegisterAll();

		poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(GISTypeString );
		if( poDriver == NULL )
		{
			message_str.Format ( "%s driver not available.", GISTypeString );
			AfxMessageBox(message_str);
			return;
		}

		OGRDataSource *poDS;

		poDS = poDriver->CreateDataSource(lpszShapeFileName, NULL );
		if( poDS == NULL )
		{
			message_str.Format ( "Creation of GIS output file %s failed.\nPlease do not overwrite the exiting file and please select a new file name.", 
				lpszShapeFileName );
			AfxMessageBox(message_str);
			return;
		}

		///// export to link layer

		// link layer 

		OGRLayer *poLayer;
		poLayer = poDS->CreateLayer( "link", NULL, wkbLineString, NULL );
		if( poLayer == NULL )
		{
			AfxMessageBox ("link Layer creation failed");
			return;
		}



		vector<string> HeaderVector = parser.GetHeaderVector();

		std::vector <CString> LongFieldVector;
		for(unsigned int i = 0; i < HeaderVector.size(); i++)
		{
			if(HeaderVector[i].find ("geometry") !=  string::npos||  HeaderVector[i].find ("name") !=  string::npos || HeaderVector[i].find ("code") !=  string::npos)
			{
				OGRFieldDefn oField (HeaderVector[i].c_str (), OFTString);

				CString str;  
				if( poLayer->CreateField( &oField ) != OGRERR_NONE ) 
				{ 
					str.Format("Creating field %s failed", oField.GetNameRef()); 

					AfxMessageBox(str);
					return; 

				}
			}else
			{
				CString field_string  = HeaderVector[i].c_str ();

				OGRFieldDefn oField (field_string, OFTReal);

				CString str;  
				if( poLayer->CreateField( &oField ) != OGRERR_NONE ) 
				{ 
					str.Format("Creating field %s failed", oField.GetNameRef()); 

					AfxMessageBox (str);
					return; 
				}

			}

			if(HeaderVector[i].size()>=11)
			{
				LongFieldVector.push_back (HeaderVector[i].c_str ());
			}

		}


		//if(LongFieldVector.size() >=1)
		//{
		//	message_str.Format("Warning: Arc GIS file only supports field names with not more than 10 characters.\nThe following fields have long field names. "); 
		//	m_MessageList.AddString (message_str);
		//	for(unsigned l = 0; l< LongFieldVector.size(); l++)
		//	{
		//		message_str.Format ("%s",LongFieldVector[l]);
		//		m_MessageList.AddString (message_str);


		//	}
		//}

		int count = 0 ;
		while(parser.ReadRecord())
		{
			//create feature
			OGRFeature *poFeature;
			poFeature = OGRFeature::CreateFeature( poLayer->GetLayerDefn() );

			//step 1: write all fields except geometry
			for(unsigned int i = 0; i < HeaderVector.size(); i++)
			{
				if(HeaderVector[i]!="geometry")
				{
					if(HeaderVector[i].find ("name") !=  string::npos || HeaderVector[i].find ("code") !=  string::npos)
					{

						std::string str_value;

						parser.GetValueByFieldName(HeaderVector[i],str_value);

						//							TRACE("field: %s, value = %s\n",HeaderVector[i].c_str (),str_value.c_str ());
						poFeature->SetField(i,str_value.c_str ());
					}else
					{
						double value = 0;

						parser.GetValueByFieldName(HeaderVector[i],value);

						//							TRACE("field: %s, value = %f\n",HeaderVector[i].c_str (),value);

						CString field_name = HeaderVector[i].c_str ();
						poFeature->SetField(i,value);



					}

				}
			}

			string geo_string;
			std::vector<CCoordinate> CoordinateVector;
			if(parser.GetValueByFieldName("geometry",geo_string))
			{
				// overwrite when the field "geometry" exists
				CGeometry geometry(geo_string);
				CoordinateVector = geometry.GetCoordinateList();

				if( GIS_data_type == GIS_Point_Type && CoordinateVector.size ()==1)
				{
					OGRPoint pt;
					pt.setX( CoordinateVector[0].X );
					pt.setY( CoordinateVector[0].Y);
					poFeature->SetGeometry( &pt ); 

				}



				if( GIS_data_type == GIS_Line_Type)
				{


					OGRLineString line;
					for(unsigned int si = 0; si< CoordinateVector.size(); si++)
					{
						line.addPoint (CoordinateVector[si].X , CoordinateVector[si].Y);
					}

					poFeature->SetGeometry( &line ); 
				}


				if( GIS_data_type == GIS_Polygon_Type)
				{

					OGRPolygon polygon;
					OGRLinearRing  ring;

					for(unsigned int si = 0; si<  CoordinateVector.size(); si++)
					{
						ring.addPoint (CoordinateVector[si].X , CoordinateVector[si].Y,1);
					}

					polygon.addRing(&ring);

					poFeature->SetGeometry( &polygon ); 

				}


			} else
			{ // no geometry field


				/// create geometry data from GIS_data_type == GIS_Point_Type

				if( GIS_data_type == GIS_Point_Type )
				{

					double x, y;
					if(parser.GetValueByFieldName("x",x) && parser.GetValueByFieldName("y",y) )
					{
						OGRPoint pt;
						pt.setX( x );
						pt.setY( y);
						poFeature->SetGeometry( &pt ); 

					}else
					{
						AfxMessageBox("Pleaes prepare fields x and y in the csv file in order to create a node GIS layer.", MB_ICONINFORMATION);
						return;

					}

				}

				///create geometry

				if( GIS_data_type == GIS_Line_Type)
				{

					int number_of_shape_points = 0;
					if(parser.GetValueByFieldName("number_of_shape_points", number_of_shape_points))
					{

						if(number_of_shape_points>=2)
						{

							OGRLineString line;

							for(int s= 1; s<= number_of_shape_points; s++)
							{
								CString str_x, str_y;
								str_x.Format ("x%d",s);
								str_y.Format ("y%d",s);
								double x = 0;
								double y = 0;

								string string_x, string_y;
								string_x  = CString2StdString (str_x);
								string_y  = CString2StdString (str_y);

								if(parser.GetValueByFieldName(string_x, x) && parser.GetValueByFieldName(string_y, y))
								{
									line.addPoint(x,y);
								}else
								{
									AfxMessageBox("Pleaes prepare fields x1,y1,x2,y2,...,xn,yn in the csv file in order to create a link GIS layer.", MB_ICONINFORMATION);

									return; 
								}


							}
							poFeature->SetGeometry( &line ); 

						}

					}else
					{ 
						AfxMessageBox("Pleaes prepare fields number_of_shape_points, x1,y1,x2,y2,...,xn,yn in the csv file in order to create a link GIS layer.", MB_ICONINFORMATION);
						return;
					}


				}

				// 


				if( GIS_data_type == GIS_Polygon_Type)
				{

					OGRPolygon polygon;
					OGRLinearRing  ring;

					int number_of_shape_points = 0;
					if(parser.GetValueByFieldName("number_of_shape_points", number_of_shape_points))
					{

						if(number_of_shape_points>=2)
						{

							OGRLineString line;

							for(int s= 0; s< number_of_shape_points; s++)
							{
								CString str_x, str_y;
								str_x.Format ("x%d",str_x);
								str_y.Format ("y%d",str_y);
								double x = 0;
								double y = 0;

								string string_x, string_y;
								string_x  = CString2StdString (str_x);
								string_y  = CString2StdString (str_y);

								if(parser.GetValueByFieldName(string_x, x) && parser.GetValueByFieldName(string_y, y))
								{
									ring.addPoint (x,y,1);
								}else
								{
									AfxMessageBox("Pleaes prepare fields x1,y1,x2,y2,...,xn,yn in the csv file in order to create a zone GIS layer.", MB_ICONINFORMATION);

									return; 
								}

							}
							polygon.addRing(&ring);

							poFeature->SetGeometry( &polygon ); 

						}


					}


				}

			}


			if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
			{
				AfxMessageBox("Failed to create feature in shapefile.\n");
				return;
			}  

			OGRFeature::DestroyFeature( poFeature );

			count++;
		}

		//message_str.Format ("%d records have been created.",count);
		//m_MessageList.AddString (message_str);


		OGRDataSource::DestroyDataSource( poDS );

		CString ShapeFile = lpszShapeFileName;
		CString ShapeFileFolder = ShapeFile.Left(ShapeFile.ReverseFind('\\') + 1);


		CString OutputFile;
		OutputFile.Format("Shape file %s has been generated with %d records.",lpszShapeFileName,count);
		if(AfxMessageBox(OutputFile,MB_ICONINFORMATION)==IDOK)
		{
			ShellExecute( NULL,  "explore", ShapeFileFolder, NULL,  NULL, SW_SHOWNORMAL );

		}
		CalculateDrawingRectangle();
		m_bFitNetworkInitialized  = false;
		UpdateAllViews(0);

	}
#else
	AfxMessageBox("NEXTA 64-bit version does not support shape file exporting function. Please use NEXTA_32.exe.");
	RunNEXTA_32();
#endif

}


void CTLiteDoc::OnNodeExportnodelayertogisshapefile()
{
	ExportToGISFile(m_ProjectDirectory + "input_node.csv", m_ProjectDirectory + "input_node.shp", GIS_Point_Type);

	//m_UnitFeet = 1;  // default value
	//m_NodeDisplaySize = 50;

}

void CTLiteDoc::OnZoneExportzonelayertogisshapefile()
{
	ExportToGISFile(m_ProjectDirectory + "input_zone.csv", m_ProjectDirectory + "input_zone.shp", GIS_Polygon_Type);
}

void CTLiteDoc::OnGridUsemileasunitoflength()
{
	m_bUseMileVsKMFlag = true;
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateGridUsemileasunitoflength(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bUseMileVsKMFlag == true);
}

void CTLiteDoc::OnGridUsekmasunitoflength()
{
	m_bUseMileVsKMFlag = false;
	UpdateAllViews(0);

}

void CTLiteDoc::OnUpdateGridUsekmasunitoflength(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bUseMileVsKMFlag == false);
}

void CTLiteDoc::OnGridUselong()
{
	m_LongLatFlag = !m_LongLatFlag;
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateGridUselong(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LongLatFlag);
}



void CTLiteDoc::OnCrashViewincidentdatatable()
{
	CDlgScenario dlg;
	dlg.m_pDoc = this;
	dlg.m_SelectTab = _INCIDENT;
	dlg.DoModal();
}

void CTLiteDoc::OnZoneRemoveactivitylocationsofselectedzone()
{
	if(m_SelectedZoneID<0)
		AfxMessageBox("Please select a zone first.",MB_ICONINFORMATION);


	//remove node's zone id
	for(unsigned int i = 0; i< m_ZoneMap[m_SelectedZoneID]. m_ActivityLocationVector.size(); i++)
	{
		int node_number  = m_ZoneMap[m_SelectedZoneID]. m_ActivityLocationVector[i].NodeNumber;
		if(m_NodeNumberMap.find(node_number)!= m_NodeNumberMap.end())
			m_NodeNumberMap[node_number]->m_ZoneID = -1;
	}

	m_ZoneMap[m_SelectedZoneID].m_ActivityLocationVector .clear();

	UpdateAllViews(0);
}

void CTLiteDoc::OnZoneRegenerateactivitylocationsforselectedzone()
{
	if(m_SelectedZoneID<0)
		AfxMessageBox("Please select a zone first.",MB_ICONINFORMATION);

	RegenerateactivitylocationsForEmptyZone(m_SelectedZoneID);
	UpdateAllViews(0);

}

void CTLiteDoc::ReadInputPath()
{

	//for(unsigned int p = 0; p < m_PathDisplayList.size(); p++) // for each path
	//{

	//	m_PathDisplayList[p].m_LinkVector.clear();
	//}
	//

	//	CFileDialog path_file_dlg (TRUE, "optional_path.csv", "optional_path.csv",OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_LONGNAMES,
	//	"Path Link Data File (*.csv)|*.csv||", NULL);
	//if(path_file_dlg.DoModal() == IDOK)
	//{

	//	m_PathDisplayList.clear();
	//	m_SelectPathNo=0;


	//	char lpszFileName[_MAX_PATH];
	//	
	//	sprintf(lpszFileName,"%s",path_file_dlg.GetPathName ());

	//	int prev_path_id = -1;
	//CCSVParser parser;
	//if (parser.OpenCSVFile(lpszFileName))
	//{
	//	bool bNodeNonExistError = false;

	//	m_OriginNodeID = -1;
	//	m_DestinationNodeID = -1;

	//	int count = 0;
	//	DTALink* pLink =NULL;
	//	while(parser.ReadRecord())
	//	{
	//		int link_id = 0;
	//		int from_node_id;
	//		int to_node_id;

	//		int path_id = 0;
	//		if(!parser.GetValueByFieldName("path_id",path_id))
	//		{
	//			AfxMessageBox("Field path_id has not been defined in file optional_path.csv. Please check.");
	//			break;
	//		}

	//		string path_name;
	//		parser.GetValueByFieldName("path_name",path_name);
	//		//find reference travel time
	//		int link_sequence_no;
	//		parser.GetValueByFieldName("link_sequence_no",link_sequence_no);


	//		std::string TMC;
	//		parser.GetValueByFieldName("TMC",TMC);

	//		if(TMC.size () >=1)
	//		{

	//			if( m_SpeedSensorIDMap.find (TMC) != m_SpeedSensorIDMap.end())
	//			{
	//			pLink = m_SpeedSensorIDMap[TMC];
	//			}else
	//			{
	//			CString str;
	//			str.Format ("TMC %s cannot be found in the current data set.",TMC.c_str () );
	//			AfxMessageBox(str);
	//			return;
	//			}

	//		}
	//		else
	//		{

	//		if(!parser.GetValueByFieldName("from_node_id",from_node_id)) 
	//		{
	//			AfxMessageBox("Field from_node_id has not been defined in file optional_path.csv. Please check.");
	//			break;
	//		}
	//		if(!parser.GetValueByFieldName("to_node_id",to_node_id))
	//		{
	//			AfxMessageBox("Field to_node_id has not been defined in file optional_path.csv. Please check.");
	//			break;
	//		}

	//		pLink = FindLinkWithNodeNumbers(from_node_id,to_node_id,lpszFileName);
	//		}

	//		if(pLink==NULL)
	//		{
	//			CString ErrorMessage;
	//			ErrorMessage.Format("Path ID: %d, Link no.%d, %d->%d does not exist.\n", path_id,link_sequence_no,from_node_id, to_node_id);

	//			ErrorMessageVector+= ErrorMessage;
	//			continue;
	//		}

	//		if(count==0)
	//		{
	//		m_OriginNodeID  = pLink->m_FromNodeID ;
	//		}


	//		if(prev_path_id!= path_id)  //find new route
	//		{
	//			DTAPath path_element;
	//			m_PathDisplayList.push_back(path_element);
	//			prev_path_id = path_id;
	//		}
	//		int route_no = m_PathDisplayList.size()-1;
	//		m_PathDisplayList[route_no].m_LinkVector.push_back (pLink->m_LinkNo );


	//		if(link_sequence_no == 1)
	//		{
	//			for(int t = 0 ; t< 1440; t+= 15)  // for each starting time
	//			{
	//				CString str;
	//				str.Format("min_%d",t);
	//				std::string str_time = CString2StdString (str);
	//	
	//				float travel_time = 0;
	//				parser.GetValueByFieldName(str_time,travel_time);
	//			

	//				if(travel_time >0.1f)
	//				{
	//					m_PathDisplayList[route_no].m_bWithSensorTravelTime = true;
	//					for(int s = 0; s<15; s++)
	//					{
	//						m_PathDisplayList[route_no].m_SensorTimeDependentTravelTime[t+s] = travel_time;
	//					}
	//				}
	//			
	//			}

	//		
	//		}


	//		CString c_path_name;
	//		c_path_name.Format("%d",route_no+1);


	//		m_PathDisplayList[route_no].m_PathLabelVector.push_back (c_path_name);

	//		if(m_PathDisplayList[route_no].m_path_name.size() ==0)  // no value yet
	//		{
	//			if(path_name.size()>=1)
	//			{
	//				m_PathDisplayList[route_no].m_path_name = path_name;
	//			}else
	//			{
	//				m_PathDisplayList[route_no].m_path_name = CString2StdString (c_path_name);
	//			}
	//		}

	//		count++;
	//	}

	//	if(pLink!=NULL)  // last link
	//	{
	//		m_DestinationNodeID  = pLink->m_ToNodeID ;	
	//	}

	//}else
	//{
	//AfxMessageBox("File cannot be found.");
	//}
	//	ReloadData();

	//	if(	m_PathList.GetCount() >=1)
	//	{
	//		m_PathList.SetCurSel(0);
	//		Invalidate();

	//	}


	//	if(ErrorMessageVector.GetLength ()>0)
	//	{
	//		WriteStringToLogFile("error_log_path_list.csv", ErrorMessageVector);

	//		ErrorMessageVector+= "Please check file error_log_path_list.csv in Excel.";
	//		AfxMessageBox(ErrorMessageVector);

	//	}
}
void CTLiteDoc::OnShowMoePathlist()
{
	m_bShowPathList = true;
	ShowPathListDlg(m_bShowPathList);}

void CTLiteDoc::OnExportExportaggregatedlinkmoefile()
{
	if(m_ProjectDirectory.GetLength () >0)
	{
		SaveLinkData(m_ProjectDirectory+"AMS_link.csv",true,0);
		OpenCSVFileInExcel(m_ProjectDirectory + "AMS_link.csv");

	}

}

void CTLiteDoc::OnHelpReportbug()
{
	g_OpenDocument("http://code.google.com/p/nexta/issues/list", SW_SHOW);
}

void CTLiteDoc::OnFreewaytoolsView()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project has not been loaded.");
		return;
	}
	CCorridorDataDlg dlg;
	dlg.m_pDoc = this;
	dlg.DoModal();
}


void CTLiteDoc::PerformPathTravelTimeReliabilityAnalysis()
{

	bool b_Impacted = false;
	float OriginalCapacity = 0.0f;
	float ImpactDuration = 0.0f;
	float LaneClosurePercentage = 0.0f;

	float CurrentTime = g_Simulation_Time_Stamp;

	std::vector<float> LinkCapacity;
	std::vector<float> LinkTravelTime;

	float max_density = 0.0f;

	int BottleneckIdx = 0;
	int ImpactedLinkIdx = -1;

	float free_flow_travel_time = 0.0f;


	if(m_PathDisplayList.size()>0)
	{

		DTAPath* pPath = &(m_PathDisplayList[0]);  // 0 is the current selected path
		for (int i=0;i<pPath->m_LinkVector.size();i++)  // for each pass link
		{
			DTALink* pLink = m_LinkNoMap[pPath->m_LinkVector[i]];

			float linkcapacity = pLink->m_LaneCapacity;
			float linktraveltime = pLink->GetDynamicTravelTime (CurrentTime,m_PrimaryDataSource);
			float density = pLink->GetSimulatedDensity(CurrentTime);

			if (density > max_density) BottleneckIdx = i;

			LinkCapacity.push_back(linkcapacity);
			LinkTravelTime.push_back(linktraveltime);
			free_flow_travel_time += linktraveltime;

			// for the first link, i==0, use your current code to generate delay, 
			//additional for user-specified incidents along the routes, add additional delay based on input

			if (!b_Impacted)
			{
				LaneClosurePercentage = pLink->GetImpactedFlag(CurrentTime); // check capacity reduction event

				if(LaneClosurePercentage > 0.01) // This link is 
				{  
					// use the incident duration data in CapacityReductionVector[] to calculate the additional delay...
					//
					// CurrentTime +=additional delay...

					if (pLink->CapacityReductionVector.size() != 0)
					{
						ImpactDuration = pLink->CapacityReductionVector[0].EndTime - pLink->CapacityReductionVector[0].StartTime;
					}

					ImpactedLinkIdx = i;

					b_Impacted = true;

				}
			}

			CurrentTime += pLink->GetDynamicTravelTime (CurrentTime,m_PrimaryDataSource);
		}
	}

	CDlg_TravelTimeReliability dlg;
	dlg.m_pDoc= this;
	dlg.LinkCapacity = LinkCapacity;
	dlg.LinkTravelTime = LinkTravelTime;

	dlg.m_BottleneckIdx = BottleneckIdx;

	if (b_Impacted)
	{
		dlg.m_bImpacted = b_Impacted;
		dlg.m_ImpactDuration = ImpactDuration;
		dlg.m_LaneClosurePercentage = LaneClosurePercentage/100.0f;
		dlg.m_ImpactedLinkIdx = ImpactedLinkIdx;
	}

	dlg.m_PathFreeFlowTravelTime = free_flow_travel_time;
	dlg.DoModal ();
}



void CTLiteDoc::OnDeleteWorkzone()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink= m_LinkNoMap [m_SelectedLinkNo];
	if(pLink!=NULL)
	{

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->CapacityReductionVector .clear();

		}
		ReadWorkZoneScenarioData(pLink->m_FromNodeNumber , pLink->m_ToNodeNumber );
		WriteWorkZoneScenarioData();


		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnCrashDeleteincidentonselectedlink()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink= m_LinkNoMap [m_SelectedLinkNo];
	if(pLink!=NULL)
	{

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->CapacityReductionVector .clear();

		}



		UpdateAllViews(0);
	}
}


void CTLiteDoc::OnSubareaGenerateevacuationzonescenariofile()
{
	WriteCapacityReductionScenarioDataFromSubareaLinks("Scenario_Evacuation_Zone.csv");
	OpenCSVFileInExcel(m_ProjectDirectory + "Scenario_Evacuation_Zone.csv");

}

void CTLiteDoc::OnSubareaGenerateweatherscenariofile()
{
	WriteCapacityReductionScenarioDataFromSubareaLinks("Scenario_Weather.csv");
	OpenCSVFileInExcel(m_ProjectDirectory + "Scenario_Weather.csv");
}

void CTLiteDoc::OnSubareaGenerateworkzonescenariofilefromlinksinsidesubarea()
{
	WriteCapacityReductionScenarioDataFromSubareaLinks("Scenario_Work_Zone.csv");
	OpenCSVFileInExcel(m_ProjectDirectory + "Scenario_Work_Zone.csv");
}

void CTLiteDoc::SaveMovementData(CString MovementFileName, int NodeNumber = -1, bool bObservationFlag = true)
{

	CWaitCursor wait;
	CCSVWriter MovementFile;

	CString movement_str = MovementFileName;

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString (movement_str);

	// construct a std::string using the LPCSTR input
	std::string strStd (pszConvertedAnsiString);

	//if(NodeNumber<0)
	//{
	//GenerateMovementShapePoints();
	//}

	if(MovementFile.Open(strStd))
	{


//		MovementFile.SetFieldName ("timing_plan_name");
		MovementFile.SetFieldName ("node_id");
		MovementFile.SetFieldName ("up_node_id");
		MovementFile.SetFieldName ("dest_node_id");
		MovementFile.SetFieldName ("name");
		MovementFile.SetFieldName ("turn_type");
		MovementFile.SetFieldName ("turn_direction");

		MovementFile.SetFieldName ("prohibited_flag");
		//MovementFile.SetFieldName ("protected_flag");
		//MovementFile.SetFieldName ("permitted_flag");

		//MovementFile.SetFieldName ("sim_turn_volume");
		//MovementFile.SetFieldName ("sim_turn_hourly_volume");
		//MovementFile.SetFieldName ("sim_turn_percentage");
		//MovementFile.SetFieldName ("sim_turn_delay_in_second");

		//MovementFile.SetFieldName ("obs_turn_volume");
		//MovementFile.SetFieldName ("obs_turn_hourly_volume");
		//MovementFile.SetFieldName ("obs_turn_percentage");
		//MovementFile.SetFieldName ("obs_turn_delay_in_second");


		//MovementFile.SetFieldName ("QEM_TurnVolume");
		//MovementFile.SetFieldName ("LinkVolume");
		//MovementFile.SetFieldName ("Lanes");
		//MovementFile.SetFieldName ("Shared");
		//MovementFile.SetFieldName ("Width");
		//MovementFile.SetFieldName ("Storage");
		//MovementFile.SetFieldName ("StLanes");
		//MovementFile.SetFieldName ("Grade");
		//MovementFile.SetFieldName ("Speed");
		//MovementFile.SetFieldName ("IdealFlow");
		//MovementFile.SetFieldName ("LostTime");
		//MovementFile.SetFieldName ("Phase1");
		//MovementFile.SetFieldName ("PermPhase1");
		//MovementFile.SetFieldName ("DetectPhase1");

		//MovementFile.SetFieldName ("EffectiveGreen");
		//MovementFile.SetFieldName ("Capacity");
		//MovementFile.SetFieldName ("VOC");
		//MovementFile.SetFieldName ("Delay");
		//MovementFile.SetFieldName ("LOS");
		//MovementFile.SetFieldName ("SatFlowRatePerLaneGroup");

		//MovementFile.SetFieldName ("GreenStartTime");
		//MovementFile.SetFieldName ("GreenEndTime");

		//MovementFile.SetFieldName ("geometry");


		MovementFile.WriteHeader();

		std::list<DTANode*>::iterator iNode;
		for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{

			if(NodeNumber>=0)  // with valid node number
			{

				if((*iNode)->m_NodeNumber != NodeNumber)
					continue; // skip not selected node number
			}


			for(int tp = 0; tp< m_TimingPlanVector.size(); tp++)  // first loop for each timing plan
			{

				std::string timing_plan_name = m_TimingPlanVector[tp].timing_plan_name;  // fetch timing_plan (unique) name


				if((*iNode)->m_MovementDataMap.find(timing_plan_name) != (*iNode)->m_MovementDataMap.end())
				{
					for(unsigned int m = 0; m< (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector .size(); m++)
					{
						if( (*iNode)->IsEmptyPhaseNumber(timing_plan_name) == true)
							(*iNode)->ResetToDefaultPhaseNumbers(timing_plan_name); 

						float number_of_hours = max(0.01, (m_DemandLoadingEndTimeInMin - m_DemandLoadingEndTimeInMin)/60.0);
						float sim_turn_hourly_count = (*iNode)->m_MovementDataMap[ m_TimingPlanVector[tp].timing_plan_name].m_MovementVector[m].sim_turn_count/ number_of_hours;


						if(bObservationFlag)
							(*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m].QEM_TurnVolume  =  (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m].obs_turn_hourly_count ;
						else
							(*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m].QEM_TurnVolume  =  sim_turn_hourly_count ;

						DTANodeMovement movement = (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m];

						if( ((*iNode)->m_ControlType == m_ControlType_PretimedSignal ||  (*iNode)->m_ControlType == m_ControlType_ActuatedSignal) && movement.turning_prohibition_flag==0)
						{  // overwite movement effective time for signalized intersections
							
							
							if(movement.QEM_EffectiveGreen < 6)
							{
								if(movement.QEM_Lanes >=1 )
								{
									DTALink* pIncomingLink = m_LinkNoMap[movement.IncomingLinkNo ];

									if(pIncomingLink!=NULL)
									{
										(*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m].QEM_EffectiveGreen =  max(12,(int)((*iNode)->m_CycleLengthInSecond* pIncomingLink->m_LaneCapacity / pIncomingLink->m_Saturation_flow_rate_in_vhc_per_hour_per_lane));

									}						
								}else
								{
									(*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m].QEM_EffectiveGreen =  6;  // minimum effective green

								}

							}

						}

						//re-fetch data
						movement = (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m];



						MovementFile.SetValueByFieldName  ("timing_plan_name",timing_plan_name);
						MovementFile.SetValueByFieldName  ("node_id",(*iNode)->m_NodeNumber);

						DTA_Phasing_Data_Matrix element = GetPhaseData((*iNode)->m_NodeNumber ,  timing_plan_name);

	
						MovementFile.SetValueByFieldName  ("name",(*iNode)->m_Name);

						MovementFile.SetValueByFieldName ("turn_type",GetTurnString(movement.movement_turn));

						MovementFile.SetValueByFieldName ("turn_direction",GetTurnDirectionString(movement.movement_approach_turn));


						//if nodes have been cut outside the network, so we do not need save them
						if(m_NodeNoMap.find (movement.in_link_from_node_id) == m_NodeNoMap.end())
							continue;

						if(m_NodeNoMap.find (movement.out_link_to_node_id) == m_NodeNoMap.end())
							continue;

						int up_node_id = m_NodeNoMap[movement.in_link_from_node_id]->m_NodeNumber  ;


						MovementFile.SetValueByFieldName ("up_node_id",up_node_id);

						if (m_NodeNoMap.find(movement.out_link_to_node_id) == m_NodeNoMap.end())
							return; 
						int dest_node_id = m_NodeNoMap[movement.out_link_to_node_id ]->m_NodeNumber ;
						MovementFile.SetValueByFieldName ("dest_node_id",dest_node_id);

						if((*iNode)->m_NodeNumber == 91 && up_node_id == 46 && dest_node_id == 37)
						{
							TRACE("");
						}

						MovementFile.SetValueByFieldName ("prohibited_flag",movement.turning_prohibition_flag);
						MovementFile.SetValueByFieldName ("protected_flag",movement.turning_prohibition_flag);
						MovementFile.SetValueByFieldName ("permitted_flag",movement.turning_prohibition_flag);

						MovementFile.SetValueByFieldName ("sim_turn_volume",movement.sim_turn_count );

						MovementFile.SetValueByFieldName ("sim_turn_hourly_volume",sim_turn_hourly_count );
						MovementFile.SetValueByFieldName ("sim_turn_percentage",movement.turning_percentage);
						MovementFile.SetValueByFieldName ("sim_turn_delay_in_second",movement.sim_turn_delay );

						MovementFile.SetValueByFieldName ("obs_turn_volume",movement.obs_turn_count  );
						MovementFile.SetValueByFieldName ("obs_turn_hourly_volume",movement.obs_turn_hourly_count) ;
						MovementFile.SetValueByFieldName ("obs_turn_percentage",movement.obs_turn_percentage  );
						MovementFile.SetValueByFieldName ("obs_turn_delay_in_second",movement.obs_turn_delay );

						MovementFile.SetValueByFieldName ("QEM_TurnVolume",movement.QEM_TurnVolume );


						MovementFile.SetValueByFieldName ("LinkVolume", movement.QEM_LinkVolume);

						MovementFile.SetValueByFieldName ("Lanes",movement.QEM_Lanes );
						MovementFile.SetValueByFieldName ("Shared",movement.QEM_Shared );
						MovementFile.SetValueByFieldName ("Width",movement.QEM_Width );
						MovementFile.SetValueByFieldName ("Storage",movement.QEM_Storage );
						MovementFile.SetValueByFieldName ("StLanes",movement.QEM_StLanes );
						MovementFile.SetValueByFieldName ("Grade",movement.QEM_Grade );
						MovementFile.SetValueByFieldName ("Speed",movement.QEM_Speed );
						MovementFile.SetValueByFieldName ("IdealFlow",movement.QEM_IdealFlow );
						MovementFile.SetValueByFieldName ("LostTime",movement.QEM_LostTime );
						MovementFile.SetValueByFieldName ("Phase1",movement.QEM_Phase1 );
						MovementFile.SetValueByFieldName ("PermPhase1",movement.QEM_PermPhase1 );
						MovementFile.SetValueByFieldName ("DetectPhase1",movement.QEM_DetectPhase1 );

						MovementFile.SetValueByFieldName ("EffectiveGreen",movement.QEM_EffectiveGreen );

						MovementFile.SetValueByFieldName ("MovementStartTime",movement.QEM_StartTime );
						MovementFile.SetValueByFieldName ("MovementEndTime",movement.QEM_EndTime );

						MovementFile.SetValueByFieldName ("Capacity",movement.QEM_Capacity );
						MovementFile.SetValueByFieldName ("VOC",movement.QEM_VOC );
						MovementFile.SetValueByFieldName ("Delay",movement.QEM_Delay );

						CT2CA pszConvertedAnsiString (movement.QEM_LOS);
						// construct a std::string using the LPCSTR input
						std::string strStd (pszConvertedAnsiString);

						MovementFile.SetValueByFieldName ("LOS", strStd);

						MovementFile.SetValueByFieldName ("SatFlowRatePerLaneGroup",movement.QEM_SatFlow );
						MovementFile.SetValueByFieldName ("GreenStartTime", movement.QEM_StartTime );
						MovementFile.SetValueByFieldName ("GreenEndTime", movement.QEM_EndTime );


						std::string str_geometry;

						// geometry
						str_geometry += "\"<LineString><coordinates>";

						for(unsigned int si = 0; si< movement.m_ShapePoints.size(); si++)
						{
							CString str;
							str.Format ("%f,%f,0.0",movement.m_ShapePoints[si].x, movement.m_ShapePoints[si].y);

							str_geometry += CString2StdString(str);

							if(si!=movement.m_ShapePoints.size()-1)
								str_geometry +=" ";
						}

						str_geometry +="</coordinates></LineString>\",";

						MovementFile.SetValueByFieldName ("geometry", str_geometry );
						MovementFile.WriteRecord ();

					}
				}

			}
		}

	}
}

void CTLiteDoc::OnImportShapefile()
{
	CDlg_Information dlg_info;
	dlg_info.m_StringInfo = "This function imports a signle shape file or Openstreetmap OSM file route point shape file.\r\nThis function requires 32-bit NEXTA.";
	dlg_info.m_SampleFileDirectory = "importing_sample_data_sets\\OpenStreetMap_files";
	dlg_info.m_OnLineDocumentLink = "https://docs.google.com/document/d/1Ud2FN1utnVrIs4je9CHveAykH8h09x0Ln6_qdXE8F6Y/edit#heading=h.24j41bpvha3d";

	if(dlg_info.DoModal() == IDOK)
	{

#ifndef _WIN64

		CDlg_GISDataExchange dlg;
		dlg.m_pDoc = this;

		if(dlg.DoModal() == IDOK)
		{
			CalculateDrawingRectangle();
			m_bFitNetworkInitialized  = false;
			UpdateAllViews(0);

		}
#endif
	}


	//m_UnitFeet = 1;  // default value
	//m_NodeDisplaySize = 50;

}

void CTLiteDoc::OnFileOpentestsets()
{

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CString folder = "\\test_data_sets\\";
	CString NetworkFile = pMainFrame->m_CurrentDirectory + folder;

	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_LONGNAMES|OFN_ENABLESIZING,
		_T("Transportation Network Project (*.tnp)|*.tnp|"),NULL,0,true);
	dlg.m_ofn.lpstrInitialDir = NetworkFile;

	if(dlg.DoModal() == IDOK)
	{
		OnOpenDocument(dlg.GetPathName());
	}

}

void CTLiteDoc::OnFileOpensampledatasetfolder()
{
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CString folder = "\\sample_data_sets\\";
	CString NetworkFile = pMainFrame->m_CurrentDirectory + folder;

	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_LONGNAMES|OFN_ENABLESIZING,
		_T("Transportation Network Project (*.tnp)|*.tnp|"),NULL,0,true);

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
		OnOpenDocument(dlg.GetPathName());
	}
}

void CTLiteDoc::OnLinkAddRadioMessage()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink= m_LinkNoMap [m_SelectedLinkNo];
	if(pLink!=NULL)
	{

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->RadioMessageVector.clear();

		}
		ReadRadioMessageScenarioData();

		RadioMessage ms;

		pLink->RadioMessageVector.push_back(ms);


		WriteRadioMessageScenarioData();


	}
}

int CTLiteDoc::ReadRadioMessageScenarioData(int RemoveLinkFromNodeNumber, int RemoveLinkToNodeNumber )
{
	// clean up all scenario data first
	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->RadioMessageVector.clear ();

	}

	FILE* st = NULL;
	int i =0;
	//  Dynamic Message Sign
	CString VMS_file = m_ProjectDirectory+"Scenario_Radio_Message.csv";
	fopen_s(&st,VMS_file,"r");
	if(st!=NULL)
	{
		// reset
		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->RadioMessageVector.clear ();
		}

		while(true)
		{
			int usn  = g_read_integer(st);
			if(usn == -1)
				break;

			int dsn =  g_read_integer(st);

			DTALink* plink = FindLinkWithNodeNumbers(usn,dsn,VMS_file );

			if(plink!=NULL)
			{
				RadioMessage ms;
				ms.ScenarioNo = g_read_integer(st);
				ms.StartDayNo  = g_read_integer(st);
				ms.EndDayNo  = g_read_integer(st);
				ms.StartTime = g_read_integer(st);
				ms.EndTime = g_read_integer(st);
				ms.ResponsePercentage= g_read_float(st);
				ms.DelayPenalty_InMin= g_read_float(st);

				if(RemoveLinkFromNodeNumber==usn && RemoveLinkToNodeNumber== dsn)
				{
					//skip
				}else
				{
					plink->RadioMessageVector.push_back(ms);
					i++;
				}
			}
		}

		fclose(st);
	}

	return i;
}

int CTLiteDoc:: FindNonCentroidNodeNumberWithCoordinate(double x, double y, int this_node_name)
{

	DTANode* pNode= NULL;

	double min_distance = 999999;
	int NodeID = -1;
	for (std::list<DTANode*>::iterator  iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{

		if((*iNode)->m_NodeNumber  != this_node_name && (*iNode)->m_ZoneID <=0)  // non centroid node
		{
			double distance = sqrt( ((*iNode)->pt.x - x)*((*iNode)->pt.x - x) + ((*iNode)->pt.y - y)*((*iNode)->pt.y - y));
			if( distance <  min_distance)
			{
				min_distance= distance;
				pNode = (*iNode);
			}
		}
	}
	if(pNode != NULL)
		return pNode->m_NodeNumber;
	else
		return NULL;
}
bool CTLiteDoc::WriteRadioMessageScenarioData()
{
	FILE* st = NULL;
	int i =0;
	//  Dynamic Message Sign
	fopen_s(&st,m_ProjectDirectory+"Scenario_Radio_Message.csv","w");
	if(st!=NULL)
	{
		// reset
		fprintf(st, "Link,Scenario No,Start Day No,End Day No,Start Time in Min,End Time in min,Responce Percentage (%%),Delay Penalty In Min\n");

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{

			for(unsigned int i = 0; i < (*iLink)->RadioMessageVector .size(); i++)
			{
				fprintf(st,"\"[%d,%d]\",%d,%d,%d,%3.0f,%3.0f,%3.1f,%4.1f\n", (*iLink)->m_FromNodeNumber , (*iLink)->m_ToNodeNumber ,
					(*iLink)->RadioMessageVector[i].ScenarioNo ,(*iLink)->RadioMessageVector[i].StartDayNo , (*iLink)->RadioMessageVector[i].EndDayNo , 
					(*iLink)->RadioMessageVector[i].StartTime , (*iLink)->RadioMessageVector[i].EndTime ,(*iLink)->RadioMessageVector[i].ResponsePercentage,
					(*iLink)->RadioMessageVector[i].DelayPenalty_InMin);
			}
		}

		fclose(st);
	}

	return true;
}

void CTLiteDoc::OnSensortoolsSensordata()
{
	// TODO: Add your command handler code here
}


void CTLiteDoc::OnExportExportzone3dkmlfile()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project directory has not been specified. Please save the project to a new folder first.");
		OnFileSaveProjectAs();
		return;
	}

	CWaitCursor wc;

	OnFileSaveProject();  // save time-dependent MOE to input_link MOE file

	CDlg_KML_Configuration_Zone dlg;
	dlg.m_pDoc  = this;
	if(dlg.DoModal () == IDOK)
	{

	}

}

void CTLiteDoc::OnLinkApplydefaultlinkattributestoalllinks()
{
	CWaitCursor wait;

	std::list<DTALink*>::iterator iLink;
	Modify();

	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		DTALink* pLink = (*iLink);

		pLink->m_SpeedLimit = m_LinkTypeMap[pLink->m_link_type ].default_speed ;

	}

}

void CTLiteDoc::OnLinkApplydefaultlanecapacitytoalllinks()
{
	CWaitCursor wait;

	std::list<DTALink*>::iterator iLink;

	Modify();
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		DTALink* pLink = (*iLink);

		pLink->m_LaneCapacity = m_LinkTypeMap[pLink->m_link_type ].default_lane_capacity ;

	}
}

void CTLiteDoc::OnLinkApplydefaultnumberoflanestoalllinks()
{
	CWaitCursor wait;

	std::list<DTALink*>::iterator iLink;

	Modify();
	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{

		DTALink* pLink = (*iLink);

		pLink->m_NumberOfLanes  = m_LinkTypeMap[pLink->m_link_type ].default_number_of_lanes ;

	}
}

void CTLiteDoc::OnExportExportlink2dkmlfile()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project directory has not been specified. Please save the project to a new folder first.");
		OnFileSaveProjectAs();
		return;
	}

	CWaitCursor wc;

	CString directory;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

	//DeleteFile(directory+"AMS_node.kml");
	//ExportNodeLayerToGISFiles(directory+"AMS_node.kml","KML");

	CString Link_2D_File = directory+"AMS_link_2D.kml";

	DeleteFile(Link_2D_File);
	ExportLinkLayerToGISFiles(Link_2D_File,"KML");
	HINSTANCE result = ShellExecute(NULL, _T("open"), Link_2D_File, NULL,NULL, SW_SHOW);
}

void CTLiteDoc::OnExportExportsignalnodekmlfile()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project directory has not been specified. Please save the project to a new folder first.");
		OnFileSaveProjectAs();
		return;
	}

	CWaitCursor wc;

	CString directory;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

	CString Signal_File = directory+"AMS_signal.kml";

	DeleteFile(Signal_File);
	ExportNodeLayerToGISFiles(Signal_File,"KML", true);


	HINSTANCE result = ShellExecute(NULL, _T("open"), Signal_File, NULL,NULL, SW_SHOW);
}

void CTLiteDoc::OnExportExportnonsignalnodekmlfile()
{
	if(m_ProjectDirectory.GetLength()==0)
	{
		AfxMessageBox("The project directory has not been specified. Please save the project to a new folder first.");
		OnFileSaveProjectAs();
		return;
	}

	CWaitCursor wc;

	CString directory;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

	CString Signal_File = directory+"AMS_node.kml";

	DeleteFile(Signal_File);
	ExportNodeLayerToGISFiles(Signal_File,"KML", false);


	HINSTANCE result = ShellExecute(NULL, _T("open"), Signal_File, NULL,NULL, SW_SHOW);
}

void CTLiteDoc::CalculateZoneCapacity()
{ 
	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		int zone_id = m_NodeNoMap[(*iLink)->m_FromNodeID]->m_ZoneID;

		if(m_NodeNoMap[(*iLink)->m_FromNodeID]->m_ZoneID > 0)
			m_ZoneMap [zone_id].m_ProductionCapacity += (*iLink)->m_LaneCapacity * (*iLink)->m_NumberOfLanes ;

		zone_id = m_NodeNoMap[(*iLink)->m_ToNodeID]->m_ZoneID;
		if(m_NodeNoMap[(*iLink)->m_ToNodeID]->m_ZoneID > 0)
			m_ZoneMap [zone_id].m_AttractionCapacity += (*iLink)->m_LaneCapacity * (*iLink)->m_NumberOfLanes ;
	}

}
void CTLiteDoc::OnSensortoolsImportsensordatafromimport()
{
	// step 1: build a map based on link_key

	std::map<CString, DTALink*> LinkKeyMap;


	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		LinkKeyMap[(*iLink)->m_LinkKey ] = (*iLink);
	}


	CString SensorFileName;
	SensorFileName.Format("%s//import_sensor.csv", m_ProjectDirectory);

	CCSVParser parser;
	int error_count = 0;

	int data_count = 0;
	if (parser.OpenCSVFile(CString2StdString(SensorFileName)))
	{

		FILE* st;
		fopen_s(&st,m_ProjectDirectory+"sensor_count.csv","w");
		if(st!=NULL)
		{
		}else
		{
			AfxMessageBox("File sensor_count.csv cannot be opened to write.");
			return;
		}

		fprintf(st, "sensor_id,x_coord,y_coord,from_node_id,to_node_id,day_no,unix_timestamp,start_time_in_min,end_time_in_min,sensor_type,direction,count,occupancy,travel_time,avg_speed\n");



		CString error_message;

		CString prev_error_message;
		while(parser.ReadRecord())
		{
			DTA_sensor sensor;

			parser.GetValueByFieldName("x_coord",sensor.pt.x );
			parser.GetValueByFieldName("y_coord",sensor.pt.y );


			DTALink* pLink = NULL;
			string link_key;
			parser.GetValueByFieldName("link_key",link_key );
			if(link_key.size () >0)
				pLink = LinkKeyMap[link_key.c_str ()];

			if(pLink!=NULL)
			{
				sensor.LinkID = pLink->m_LinkNo ;

				pLink->m_bSensorData  = true;

				int start_time_in_min =0;
				int end_time_in_min = 0;

				parser.GetValueByFieldName("start_time_in_min",start_time_in_min );
				parser.GetValueByFieldName("end_time_in_min",end_time_in_min );


				float volume_count= 0;

				parser.GetValueByFieldName ("link_count",volume_count);

				data_count++;

				if(start_time_in_min <0 && error_message.GetLength () < 1000)
				{
					CString msg;
					msg.Format ("Sensor %s has an error of start_time_in_min <0.\n",link_key.c_str ());

					AfxMessageBox(msg);
					return;


				}

				if(end_time_in_min < start_time_in_min+1 && error_message.GetLength () < 1000)
				{
					CString msg;
					msg.Format ("Sensor %s has an error of end_time_in_min <= start_time_in_min: %d < %d.\n",link_key.c_str (),
						end_time_in_min, start_time_in_min);

					AfxMessageBox(msg);
					return;

				}

				fprintf(st, "%s,,,%d,%d,0,,%d,%d,link_count,,%.1f,0,0,0\n", 
					link_key.c_str (), 
					pLink->m_FromNodeNumber , 
					pLink->m_ToNodeNumber ,start_time_in_min,end_time_in_min, volume_count);
				data_count++;

			}



		}

		fclose(st);
	}

	CString Message;
	Message.Format("%d sensor records have been processed into file sensor_count.csv.",data_count);
	AfxMessageBox(Message,MB_ICONINFORMATION);
}
void CTLiteDoc::OnTrafficcapacitySetdefault()
{
	float  jam_density = 200;

	float total_jam_density = 0;
	int count = 0;


	for (std::list<DTALink*>::iterator  iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		if((*iLink)->m_LayerNo == 0 && m_LinkTypeMap[(*iLink)->m_link_type  ].IsArterial  () )
		{
			total_jam_density += (*iLink)->m_Kjam;
			count++;
		}
	}

	jam_density= total_jam_density / max(1,count);

	CString j_str; 
	j_str.Format ("%.0f",jam_density);
	CDlg_UserInput dlg_jam_density;

	count = 0;
	dlg_jam_density.m_StrQuestion  = "Please specify jam density (vehicles/mile/lane for all arterial links:";
	dlg_jam_density.m_InputValue = j_str;

	if(dlg_jam_density.DoModal ()==IDOK)
	{
		jam_density = atoi(dlg_jam_density.m_InputValue) ;

		for (std::list<DTALink*>::iterator  iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			if((*iLink)->m_LayerNo == 0 && m_LinkTypeMap[(*iLink)->m_link_type  ].IsArterial  () )
			{
				(*iLink)->m_Kjam = jam_density;
				count++;
			}
		}

		CString str;
		str.Format("%d arterial links have an updated jam density. To accept the changes, please save the network.",count);
		AfxMessageBox(str, MB_ICONINFORMATION);

	}

}

void CTLiteDoc::OnFileRemovenonessentialfilestoreducefoldersize()
{
	DeleteFile(m_ProjectDirectory+"output_LinkTDMOE.csv");
	DeleteFile(m_ProjectDirectory+"output_agent.csv");
	DeleteFile(m_ProjectDirectory+"AMS_od_pairs.csv");

	AfxMessageBox("Files such as output_LinkTDMOE.csv and output_LinkTDMOE.csv are removed to save spaec.", MB_ICONINFORMATION);


}

void CTLiteDoc::OnMoeViewoddemandestimationsummaryplotHourly()
{
	m_sensor_data_aggregation_type = 1;
	OnMoeViewoddemandestimationsummaryplot();
	m_sensor_data_aggregation_type = 0; // restore default
}


void CTLiteDoc::OnMoeViewoddemandestimationsummaryplotLaneHourly()
{
	m_sensor_data_aggregation_type = 2;

	OnMoeViewoddemandestimationsummaryplot();
	m_sensor_data_aggregation_type = 1;
}



void CTLiteDoc::OnTransitOutputtransitaccesssibilityfromallactivitylocations()
{
	GDPoint pt;
	PrintOutAccessibleMap(pt, true); 
}

void CTLiteDoc::OnMovementHidenon()
{
	m_hide_non_specified_movement_on_freeway_and_ramp = !m_hide_non_specified_movement_on_freeway_and_ramp;
}

void CTLiteDoc::OnUpdateMovementHidenon(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_hide_non_specified_movement_on_freeway_and_ramp);
}

void CTLiteDoc::OnMovementSetpeakhourfactor()
{

	CDlg_UserInput dlg_PeakHourFactor;

	dlg_PeakHourFactor.m_StrQuestion  = "Please specify peak hour factor:";
	dlg_PeakHourFactor.m_InputValue.Format ("%.2f",m_PeakHourFactor);

	if(dlg_PeakHourFactor.DoModal ()==IDOK)
	{
		m_PeakHourFactor = atof(dlg_PeakHourFactor.m_InputValue) ;

	}
}

void CTLiteDoc::OnZoneChangezonenumber()
{
	Modify ();

	DTAZone element = m_ZoneMap[this->m_SelectedZoneID];

	CDlg_UserInput dlg;

	dlg.m_StrQuestion  = "New zone number:";
	dlg.m_InputValue = "0";

	if(dlg.DoModal ()==IDOK && dlg.m_InputValue!="0")
	{
		int NewZoneID =  atoi( dlg.m_InputValue);
		CString msg;

		if(m_ZoneMap.find(NewZoneID)!= m_ZoneMap.end ())
		{
			msg.Format("The zone ID %d has been defined. Please select a new zone ID.",NewZoneID );
			AfxMessageBox("");
			return;

		}
		m_ZoneMap.erase (this->m_SelectedZoneID  );

		element.m_ZoneID = NewZoneID;

		m_ZoneMap[element.m_ZoneID] = element;

		msg.Format("The zone ID %d has been changed to %d. Please ensure the demand table has a consistent zone ID system.",m_SelectedZoneID, NewZoneID );

		this->m_SelectedZoneID = NewZoneID;
		AfxMessageBox(msg, MB_ICONINFORMATION);

	}

	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateZoneDeletezone(CCmdUI *pCmdUI)
{
	pCmdUI->Enable (m_SelectedZoneID>=1);
}

void CTLiteDoc::OnUpdateZoneChangezonenumber(CCmdUI *pCmdUI)
{
	pCmdUI->Enable (m_SelectedZoneID>=1);

}

void CTLiteDoc::OnChangelinktypecolorFreeway()
{

	CColorDialog dlg(theApp.m_FreewayColor, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		theApp.m_FreewayColor= dlg.GetColor();
		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnChangelinktypecolorRamp()
{
	CColorDialog dlg(theApp.m_RampColor, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		theApp.m_RampColor= dlg.GetColor();
		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnChangelinktypecolorArterial()
{
	CColorDialog dlg(theApp.m_ArterialColor, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		theApp.m_ArterialColor= dlg.GetColor();
		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnChangelinktypecolorConnector()
{
	CColorDialog dlg(theApp.m_ConnectorColor, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		theApp.m_ConnectorColor= dlg.GetColor();
		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnChangelinktypecolorTransit()
{
	CColorDialog dlg(theApp.m_TransitColor, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		theApp.m_TransitColor= dlg.GetColor();
		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnChangelinktypecolorWalkingmode()
{
	CColorDialog dlg(theApp.m_WalkingColor, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		theApp.m_WalkingColor= dlg.GetColor();
		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnChangelinktypecolorResettodefaultcolorschema()
{

	theApp.m_FreewayColor = RGB(030,144,255);
	theApp.m_RampColor = RGB(160,032,240); 
	theApp.m_ArterialColor = RGB(034,139,034);
	theApp.m_ConnectorColor = RGB(255,165,000);
	theApp.m_TransitColor = RGB(255,0,255);
	theApp.m_WalkingColor = RGB(127,255,0);
	theApp.m_ParkingandRideColor = RGB(0, 255, 0);
}

void CTLiteDoc::OnNodeChangenodecolor()
{
	CColorDialog dlg(theApp.m_NodeColor , CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		theApp.m_NodeColor= dlg.GetColor();
		UpdateAllViews(0);
	}

}

void CTLiteDoc::OnNodeChangenodebackgroundcolor()
{
	CColorDialog dlg(theApp.m_NodeBrushColor , CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		theApp.m_NodeBrushColor= dlg.GetColor();
		UpdateAllViews(0);
	}
}

void CTLiteDoc::OnZoneChangezonecolor()
{
	CColorDialog dlg(theApp.m_ZoneColor , CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		theApp.m_ZoneColor= dlg.GetColor();
		UpdateAllViews(0);
	}

}

void CTLiteDoc::OnEditUndo33707()
{
	Undo();
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateEditUndo33707(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_NetworkState.size()>0);
}

void CTLiteDoc::OnEditRedo33709()
{
	Redo();
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateEditRedo33709(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_RedoNetworkState.size()>0);
}
void CTLiteDoc::PushBackNetworkState()
{
	CWaitCursor wait;

	m_RedoNetworkState.clear();

	NetworkState element;

	element.l_NodeNoMap = m_NodeNoMap;
	element.l_NodeNumberMap = m_NodeNumberMap;
	element.l_LinkNoMap = m_LinkNoMap;
	element.l_NodeNotoLinkMap = m_NodeNotoLinkMap;
	element.l_NodeNumbertoLinkMap = m_NodeNumbertoLinkMap;
	element.l_LinkNotoLinkMap = m_LinkNotoLinkMap;
	element.l_LinkIDtoLinkMap = m_LinkIDtoLinkMap;


	element.l_NodeSet = m_NodeSet;
	element.l_LinkSet = m_LinkSet;

	element.l_SubareaNodeSet = m_SubareaNodeSet;
	element.l_SubareaLinkSet = m_SubareaLinkSet;
	element.l_ZoneMap =  m_ZoneMap;

	element.l_NodeNumbertoNodeNoMap = m_NodeNumbertoNodeNoMap;
	element.l_NodeNotoZoneNameMap = m_NodeNotoZoneNameMap ;
	element.l_SubareaShapePoints = m_SubareaShapePoints;


	m_NetworkState.push_back(element);

	if(m_NetworkState.size()>=20)  // no more than 20 steps in undo
	{
		m_NetworkState.erase (m_NetworkState.begin ());

	}



}
void CTLiteDoc::Undo()
{

	NetworkState current_element;

	current_element.l_NodeSet.clear ();

	current_element.l_NodeNoMap = m_NodeNoMap;
	current_element.l_NodeNumberMap = m_NodeNumberMap;
	current_element.l_LinkNoMap = m_LinkNoMap;
	current_element.l_NodeNotoLinkMap = m_NodeNotoLinkMap;
	current_element.l_NodeNumbertoLinkMap = m_NodeNumbertoLinkMap;
	current_element.l_LinkNotoLinkMap = m_LinkNotoLinkMap;
	current_element.l_LinkIDtoLinkMap = m_LinkIDtoLinkMap;


	current_element.l_NodeSet = m_NodeSet;
	current_element.l_LinkSet = m_LinkSet;

	current_element.l_SubareaNodeSet = m_SubareaNodeSet;
	current_element.l_SubareaLinkSet = m_SubareaLinkSet;
	current_element.l_ZoneMap =  m_ZoneMap;

	current_element.l_NodeNumbertoNodeNoMap = m_NodeNumbertoNodeNoMap;
	current_element.l_NodeNotoZoneNameMap = m_NodeNotoZoneNameMap ;
	current_element.l_SubareaShapePoints = m_SubareaShapePoints;
	TRACE("\n%d links", current_element.l_LinkSet.size());

	m_RedoNetworkState.push_back(current_element);		

	if(m_NetworkState.size() >0)
	{
		CWaitCursor wait;
		NetworkState element =  m_NetworkState[m_NetworkState.size ()-1];


		m_NodeSet = element.l_NodeSet;
		m_LinkSet = element.l_LinkSet;

		m_SubareaNodeSet = element.l_SubareaNodeSet;
		m_SubareaLinkSet = element.l_SubareaLinkSet;

		m_NodeNotoLinkMap = element.l_NodeNotoLinkMap;

		m_ZoneMap = element.l_ZoneMap ;

		m_NodeNumbertoNodeNoMap = element.l_NodeNumbertoNodeNoMap;
		m_NodeNotoZoneNameMap = element.l_NodeNotoZoneNameMap;

		m_NodeNoMap = element.l_NodeNoMap;
		m_NodeNumberMap = element.l_NodeNumberMap;
		m_LinkNoMap = element.l_LinkNoMap;
		m_NodeNumbertoLinkMap = element.l_NodeNumbertoLinkMap;
		m_LinkNotoLinkMap = element.l_LinkNotoLinkMap;
		m_LinkIDtoLinkMap = element.l_LinkIDtoLinkMap;
		m_SubareaShapePoints = element.l_SubareaShapePoints;

		m_NetworkState.pop_back ();

		TRACE("\nRedo: %d links", element.l_LinkSet.size());

		//	m_RedoNetworkState.push_back (element);




		UpdateAllViews(0);
	}


}

void CTLiteDoc::Redo()
{	

	if(m_RedoNetworkState.size() >0)
	{
		CWaitCursor wait;

		NetworkState current_element;

		current_element.l_NodeSet.clear ();

		current_element.l_NodeNoMap = m_NodeNoMap;
		current_element.l_NodeNumberMap = m_NodeNumberMap;
		current_element.l_LinkNoMap = m_LinkNoMap;
		current_element.l_NodeNotoLinkMap = m_NodeNotoLinkMap;
		current_element.l_NodeNumbertoLinkMap = m_NodeNumbertoLinkMap;
		current_element.l_LinkNotoLinkMap = m_LinkNotoLinkMap;
		current_element.l_LinkIDtoLinkMap = m_LinkIDtoLinkMap;


		current_element.l_NodeSet = m_NodeSet;
		current_element.l_LinkSet = m_LinkSet;

		current_element.l_SubareaNodeSet = m_SubareaNodeSet;
		current_element.l_SubareaLinkSet = m_SubareaLinkSet;
		current_element.l_ZoneMap =  m_ZoneMap;

		current_element.l_NodeNumbertoNodeNoMap = m_NodeNumbertoNodeNoMap;
		current_element.l_NodeNotoZoneNameMap = m_NodeNotoZoneNameMap ;
		current_element.l_SubareaShapePoints = m_SubareaShapePoints;

		m_NetworkState.push_back (current_element);

		NetworkState element =  m_RedoNetworkState[m_RedoNetworkState.size()-1];

		TRACE("\nRedo to %d links", element.l_LinkSet.size());

		m_NodeSet = element.l_NodeSet;
		m_LinkSet = element.l_LinkSet;

		m_SubareaNodeSet = element.l_SubareaNodeSet;
		m_SubareaLinkSet = element.l_SubareaLinkSet;

		m_NodeNotoLinkMap = element.l_NodeNotoLinkMap;

		m_ZoneMap = element.l_ZoneMap ;

		m_NodeNumbertoNodeNoMap = element.l_NodeNumbertoNodeNoMap;
		m_NodeNotoZoneNameMap = element.l_NodeNotoZoneNameMap;

		m_NodeNoMap = element.l_NodeNoMap;
		m_NodeNumberMap = element.l_NodeNumberMap;
		m_LinkNoMap = element.l_LinkNoMap;
		m_NodeNumbertoLinkMap = element.l_NodeNumbertoLinkMap;
		m_LinkNotoLinkMap = element.l_LinkNotoLinkMap;
		m_LinkIDtoLinkMap = element.l_LinkIDtoLinkMap;
		m_SubareaShapePoints = element.l_SubareaShapePoints;



		m_RedoNetworkState.pop_back ();

		TRACE("\n%d links", element.l_LinkSet.size());


		UpdateAllViews(0);
	}


}


void CTLiteDoc::GenerateMovementShapePoints()
{

	std::list<DTANode*>::iterator iNode;
	for (iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
	{
		DTANode* pNode = (*iNode);

		double size_ratio= m_MovementTextBoxSizeInFeet/250;

		double node_set_back = size_ratio*150 * m_UnitFeet;

		double link_length = size_ratio*250 * m_UnitFeet;
		double movement_length = size_ratio*100 * m_UnitFeet;
		double lane_width = size_ratio*10 * m_UnitFeet;


		double text_length =  m_MovementTextBoxSizeInFeet * 0.15 *  m_UnitFeet ;

		std::map<CString, double> Turn_Degree_map;

		for (unsigned int i=0;i< pNode->m_MovementDataMap["0"].m_MovementVector .size();i++)
		{
			DTANodeMovement movement = pNode->m_MovementDataMap["0"].m_MovementVector[i];

			pNode->m_MovementDataMap["0"].m_MovementVector[i].m_ShapePoints .clear ();


			if(pNode->m_ControlType != m_ControlType_PretimedSignal && 
				pNode->m_ControlType != m_ControlType_ActuatedSignal)
				continue;


			if(m_LinkNoMap.find(movement.IncomingLinkNo) ==  m_LinkNoMap.end())
				return;

			if(m_LinkNoMap.find(movement.OutgoingLinkNo) ==  m_LinkNoMap.end())
				return;

			DTALink* pInLink  = m_LinkNoMap [movement.IncomingLinkNo];
			DTALink* pOutLink  = m_LinkNoMap [movement.OutgoingLinkNo ];

			int destination_node  = pOutLink->m_ToNodeNumber; 


			GDPoint p1, p2, p3, p_text;
			// 1: fetch all data
			p1  = m_NodeNoMap[movement.in_link_from_node_id ]->pt;
			p2  = m_NodeNoMap[movement.in_link_to_node_id ]->pt;
			p3  = m_NodeNoMap[movement.out_link_to_node_id]->pt;

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
			double lane_width =  size_ratio*3*m_LaneWidthInFeet * m_UnitFeet;
			double movement_offset = 0;
			double text_offset = 0.7*lane_width;
			int mid_lane_number = 4+ pInLink->m_NumberOfLanes / 2;
			float control_point_ratio = 0;

			float MovementTextFontSize = 8;

			CString movement_approach_turnection_label;
			movement_approach_turnection_label.Format ("%d,%s", movement.in_link_from_node_id , GetTurnString(movement.movement_turn));

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

			// 		GDPoint p1_new, p2_new, p3_new;
			pNode->m_MovementDataMap["0"].m_MovementVector[i].m_ShapePoints.push_back(p1_new);
			pNode->m_MovementDataMap["0"].m_MovementVector[i].m_ShapePoints.push_back(p2_new);
			pNode->m_MovementDataMap["0"].m_MovementVector[i].m_ShapePoints.push_back(p3_new);

		}  // per movement
	} // per node

}
void CTLiteDoc::OnMovementSetupnumberofleftturnlanesforsignalizednodes()
{

	CDlg_LeftTurnLaneSetup dlg;


	if(dlg.DoModal() == IDOK)
	{

		CWaitCursor wait;

		int number_of_links_changed = 0;


		for (std::list<DTANode*>::iterator  iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{
			if(((*iNode)->m_ControlType == m_ControlType_PretimedSignal || (*iNode)->m_ControlType == m_ControlType_ActuatedSignal))  // volume data exists
			{

				float max_left_turn_hourly_volume = 0;

				for(int tp = 0; tp< m_TimingPlanVector.size(); tp++)  //  loop for all timing plans
				{

					std::string timing_plan_name = m_TimingPlanVector[tp].timing_plan_name;  // fetch timing_plan (unique) name

					if((*iNode)->m_MovementDataMap.find (timing_plan_name)!=(*iNode)->m_MovementDataMap.end())
					{
						for(unsigned int m = 0; m< (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector .size(); m++)
						{
							DTANodeMovement* pMovement = &((*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m]);

							float number_of_hours = max(0.01, (m_DemandLoadingEndTimeInMin - m_DemandLoadingStartTimeInMin) / 60.0);
							float sim_turn_hourly_count = (*iNode)->m_MovementDataMap[m_TimingPlanVector[tp].timing_plan_name].m_MovementVector[m].sim_turn_count / number_of_hours;

							if((pMovement->movement_turn == DTA_LeftTurn || pMovement->movement_turn == DTA_LeftTurn2) && sim_turn_hourly_count > max_left_turn_hourly_volume)
								max_left_turn_hourly_volume = sim_turn_hourly_count;

						}
					}

				}


				if(max_left_turn_hourly_volume >1)
				{
					int number_left_turn_lanes = 0;

					if(max_left_turn_hourly_volume < dlg.m_LeftturnVolume1)
						number_left_turn_lanes = 0;
					else if (max_left_turn_hourly_volume < dlg.m_LeftturnVolume2)
						number_left_turn_lanes = 1;
					else if (max_left_turn_hourly_volume < dlg.m_LeftturnVolume3)
						number_left_turn_lanes = 2;
					else 
						number_left_turn_lanes = 3;


					if(number_left_turn_lanes>=1)
						number_of_links_changed++;

					// set # of left turn lanes
					for(int tp = 0; tp< m_TimingPlanVector.size(); tp++)  //  loop for all timing plans
					{

						std::string timing_plan_name = m_TimingPlanVector[tp].timing_plan_name;  // fetch timing_plan (unique) name

						if((*iNode)->m_MovementDataMap.find (timing_plan_name)!=(*iNode)->m_MovementDataMap.end())
						{
							for(unsigned int m = 0; m< (*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector .size(); m++)
							{
								DTANodeMovement* pMovement = &((*iNode)->m_MovementDataMap[timing_plan_name].m_MovementVector[m]);

								if((pMovement->movement_turn == DTA_LeftTurn || pMovement->movement_turn == DTA_LeftTurn2))
								{
									pMovement->QEM_Lanes = max(pMovement->QEM_Lanes , number_left_turn_lanes);
									DTALink* pLink0 = m_LinkNoMap[pMovement->IncomingLinkNo  ];
									pLink0->m_NumberOfLeftTurnLanes = pMovement->QEM_Lanes;

								}

							}

						}


					}
				}
			}

		}



		Modify();


		CString str;
		str.Format("The numbers of left turn lanes have been modified for %d links. Please save the project to confirm the changes.", number_of_links_changed);
		AfxMessageBox(str, MB_ICONINFORMATION);
	}
}

void CTLiteDoc::OnMovementOptimizephasingandtimingdataforalltrafficsignalsthroughqem()
{
	CDlg_SignalDataExchange dlg;
	dlg.m_pDoc = this;
	dlg.DoModal();
}

void CTLiteDoc::OnLinkmoedisplayImpact()
{
	m_LinkMOEMode = MOE_impact;
	m_LinkBandWidthMode = LBW_congestion_duration;

	ShowLegend(true);
	GenerateOffsetLinkBand();
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateLinkmoedisplayImpact(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_impact);
}

void CTLiteDoc::OnLinkmoedisplayBottleneck()
{
	m_LinkMOEMode = MOE_bottleneck;
	m_LinkBandWidthMode = LBW_number_of_lanes;

	ShowLegend(true);
	GenerateOffsetLinkBand();
	UpdateAllViews(0);
}

void CTLiteDoc::OnUpdateLinkmoedisplayBottleneck(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_LinkMOEMode == MOE_bottleneck);
}
float CTLiteDoc::GetNodeTotalDelay(int ToNodeNumber, int time, int& LOS)
{
	float total_delay = 0;
	DTANode* pToNode = m_NodeNumberMap[ToNodeNumber];

	int RelativeStartTime = 9999;
	for(unsigned int i = 0; i< pToNode->m_IncomingLinkVector.size(); i++)
	{

		DTALink* pLink = m_LinkNoMap[pToNode->m_IncomingLinkVector[i]];
		total_delay+= pLink->GetDynamicNodeDelay(time, this->m_PrimaryDataSource );

		RelativeStartTime = min(RelativeStartTime, pLink->GetImpactRelativeStartTime(g_Simulation_Time_Stamp,this->m_PrimaryDataSource ));

	}
	float power = RelativeStartTime*1.0/g_ImpactStudyPeriodInMin*100;
	if(power>100)
		power = 100;

	LOS = GetLOSCode(power);

	return total_delay/max(1,pToNode->m_IncomingLinkVector.size());

}



void CTLiteDoc::OnGisplanningdatasetConfigureimportingsettingfile()
{

	//CDlg_DataImportWizard dlgWizard;
	//dlgWizard.DoModal();
#ifndef _WIN64
	CDlg_GIS_Setting_Config dlg;

	dlg.m_pDoc = this;
	dlg.DoModal();
#else 
	AfxMessageBox("NEXTA 64-bit version does not support shape file importing function. Please use NEXTA_32.exe. ");
	RunNEXTA_32();
	return;
#endif


	// TODO: Add your command handler code here
}


void CTLiteDoc::OnSubareaExporttotalnumberofvehiclesinsubarea()
{

	CString file_name  = m_ProjectDirectory+"Export_total_number_of_vehicles_in_subarea.csv";
	FILE* pFile;
	fopen_s(&pFile,file_name,"w");
	if(pFile!=NULL)
	{
		fprintf(pFile, "time_in_min,time_clock,total_number_of_vehicles_in_subarea,avg_speed_in_mile,avg_speed_in_km\n");
		for(int t = 0; t< 1440; t++)
		{

			float total_number_of_vehicles = 0;

			float total_speed_value = 0;

				for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			{
				if((*iLink)->m_bIncludedinSubarea && m_SubareaShapePoints.size()>=3)
				{

					float number_of_vehicles = (*iLink)->GetSimulatedDensity (t)*(*iLink)->m_Length * (*iLink)->m_NumberOfLanes;
				
					total_number_of_vehicles += number_of_vehicles;
					total_speed_value+= (*iLink)->GetSimulatedSpeed (t) * number_of_vehicles;
				
				}


			}

				float avg_speed = total_speed_value/max(1,total_number_of_vehicles);
				float avg_speed_in_km = avg_speed*1.609344;


				fprintf(pFile,"%d,%s,%.0f,%.2f,%.2f\n",t,GetTimeStampString24HourFormat (t),total_number_of_vehicles,avg_speed,avg_speed_in_km);

		}

		fclose(pFile);

	this->OpenCSVFileInExcel (file_name);

	}

}

void CTLiteDoc::OnLinkAddRampmeter()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink= m_LinkNoMap [m_SelectedLinkNo];
	if(pLink!=NULL)
	{

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->CapacityReductionVector .clear();

		}
		ReadIncidentScenarioData();
		ReadWorkZoneScenarioData();
		ReadTrafficControlScenarioData();

		CapacityReduction cs;
		cs.bGenericTrafficControl  = true; 

		cs.ScenarioNo =  0; 
		cs.StartDayNo  = 0;
		cs.EndDayNo	   = 100;
		cs.StartTime = 0;
		cs.EndTime = 1440;
		cs.ServiceFlowRate = 1400;
		cs.SpeedLimit = 50;

		pLink->CapacityReductionVector.push_back(cs);


		WriteTrafficControlScenarioData();

	
	}
}


void CTLiteDoc::OnRampRampdata()
{
	CDlgScenario dlg;
	dlg.m_pDoc = this;
	dlg.m_SelectTab = _GENERICTRAFFICCONTROL;
	dlg.DoModal();
}



void CTLiteDoc::OnDeleteRampmeter()
{
	if(m_SelectedLinkNo==-1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink= m_LinkNoMap [m_SelectedLinkNo];
	if(pLink!=NULL)
	{

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			(*iLink)->CapacityReductionVector .clear();

		}
		ReadIncidentScenarioData();
		ReadWorkZoneScenarioData();
		ReadTrafficControlScenarioData(pLink->m_FromNodeNumber , pLink->m_ToNodeNumber );


		WriteTrafficControlScenarioData();


		UpdateAllViews(0);
	}

}


void CTLiteDoc::OnToolsConfiguration()
{
	CDlg_NEXTA_Configuration dlg;

	if (m_bRightHandTrafficFlag==1)
		dlg.m_bRightHandTrafficFlag = 1;
	else
		dlg.m_bRightHandTrafficFlag = 0;

	int old_right_hand_traffic_value = dlg.m_bRightHandTrafficFlag;

	if (dlg.DoModal() == IDOK)
	{
		if (old_right_hand_traffic_value != dlg.m_bRightHandTrafficFlag)
		{
	
		if (dlg.m_bRightHandTrafficFlag == 1)
			m_bRightHandTrafficFlag = 1;
		else
			m_bRightHandTrafficFlag = -1;

		GenerateOffsetLinkBand();
		OffsetLink();
		g_WritePrivateProfileInt("coordinate_info", "right_hand_traffic_flag", m_bRightHandTrafficFlag, m_ProjectFile);
		UpdateAllViews(0);

		AfxMessageBox("To show link direction arrows, please uncheck node layer display, \ncheck menu->View ->Link->Show/Hide Link Direction Arrow,\nand increase link offset (through tool bar button) to obtain right offsets.", MB_ICONINFORMATION);
		}
	}

}


void CTLiteDoc::OnDetectorOverwritesensorlocationdata()
{
	CWaitCursor wait;


	FILE* st;
	fopen_s(&st, m_ProjectDirectory + "input_sensor_location.csv", "w");
	if (st != NULL)
	{
		std::list<DTALink*>::iterator iLink;
		fprintf(st, "sensor_id,name,x,y,direction,type_code,loop_code,orientation_code,orientation2_code,matched_from_node_id,matched_to_node_id\n");

		std::map<string, DTA_sensor>::iterator iSensor;
		for (iSensor = m_SensorMap.begin(); iSensor != m_SensorMap.end(); iSensor++)
		{

			(*iSensor).second.pt.x = m_SensorMapX + (*iSensor).second.pt.x;
			(*iSensor).second.pt.y = m_SensorMapY + (*iSensor).second.pt.y;

			std::replace((*iSensor).second.description.begin(), (*iSensor).second.description.end(), ',', '_');
			std::replace((*iSensor).second.loop_code.begin(), (*iSensor).second.loop_code.end(), ',', '_');
			
			fprintf(st, "%s,%s,%f,%f,%s,%s,%s,%s,%s,%d,%d,\n", 
				(*iSensor).second.SensorID.c_str(), 
				(*iSensor).second.description.c_str(),
				(*iSensor).second.pt.x, (*iSensor).second.pt.y,
				(*iSensor).second.direction.c_str(),
				(*iSensor).second.type_code.c_str(),
				(*iSensor).second.loop_code.c_str(),
				(*iSensor).second.orientation_code.c_str(),
				(*iSensor).second.orientation2_code.c_str(),
				(*iSensor).second.matched_from_node_id,
				(*iSensor).second.matched_to_node_id);


		}
	

		m_SensorMapX = 0;
		m_SensorMapY = 0;

		fclose(st);

	/*	CString message;
		message.Format("Sensor location data have been saved in File %s.", m_ProjectDirectory + "input_sensor_location.csv");
		AfxMessageBox(message, MB_ICONINFORMATION);*/

	}
	else
	{
		CString message;
		message.Format("Error: File %s cannot be opened.\nIt might be currently used and locked by EXCEL.", m_ProjectDirectory + "input_sensor_location.csv");
		AfxMessageBox(message);
		return;
	}
}


void CTLiteDoc::OnSensortoolsMapsensorlocationtomodellinks()
{

	double User_input_min_distance = 0.01;
	CDlg_UserInput dlg;
	dlg.m_StrQuestion = "Please specify the matching distance value (in mile):";
	dlg.m_InputValue = "0.01";
	if (dlg.DoModal() == IDOK)
	{

		User_input_min_distance = atof(dlg.m_InputValue);
		

	}


	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->m_CountSensorID = "";

	}
	std::map<string, DTA_sensor>::iterator iSensor;
	int maptch_count = 0;
	for (iSensor = m_SensorMap.begin(); iSensor != m_SensorMap.end(); iSensor++)
	{

		(*iSensor).second.pt.x = m_SensorMapX + (*iSensor).second.pt.x;
		(*iSensor).second.pt.y = m_SensorMapY + (*iSensor).second.pt.y;


		double Min_distance = User_input_min_distance;

		DTALink* pMatchedLink = NULL;

		// first step, find links with type code restriction 

		for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			if ((*iLink)->m_ShapePoints.size() <2)
				continue;

			// if this sensor has direction code being specified but the link has a different direction, skip

			if ((*iSensor).second.loop_code.size() > 0)  // with loop code
			{
				if((*iLink)->m_loop_code != (*iSensor).second.loop_code)  // if the loop code of links is different, skip
					continue;
			}
			else  // without loop code 
			{

				if ((*iLink)->m_loop_code.size() > 0)  // a link has a special loop code, skip
					continue;


				if ((*iSensor).second.orientation_code.size() > 0 && (*iLink)->m_orientation_code != (*iSensor).second.orientation_code)
					continue;
			
			}


			if ((*iLink)->m_FromNodeNumber == 153 && (*iLink)->m_ToNodeNumber == 836)
			{
				TRACE("");
			}


			// the same type code or freeway

			if (((*iSensor).second.type_code.size() > 0 && m_LinkTypeMap[(*iLink)->m_link_type].type_code == (*iSensor).second.type_code)
				|| ((*iSensor).second.type_code.size() == 0 && m_LinkTypeMap[(*iLink)->m_link_type].IsFreeway ()))
			{
				for (int si = 0; si < (*iLink)->m_ShapePoints.size() - 1; si++)
				{
					GDPoint p0, pfrom, pto;

					p0 = (*iSensor).second.pt;
					pfrom = (*iLink)->m_ShapePoints[si];
					pto = (*iLink)->m_ShapePoints[si +1];


					float distance = g_GetPoint2LineDistance(p0, pfrom, pto, m_UnitMile, false);

					if (distance > 0 && distance < Min_distance)
					{
						pMatchedLink = (*iLink);

						Min_distance = distance;

						(*iSensor).second.matched_from_node_id = pMatchedLink->m_FromNodeNumber;
						(*iSensor).second.matched_to_node_id = pMatchedLink->m_ToNodeNumber;

					}
				}
			}
		
		
		}

		if (pMatchedLink != NULL)
		{
			if ((*iSensor).second.direction.size() >0)
				pMatchedLink->m_CountSensorID = (*iSensor).second.SensorID + "_" + (*iSensor).second.direction ;
			else
				pMatchedLink->m_CountSensorID = (*iSensor).second.SensorID ;



			maptch_count++;

		}
		else  // we do not find the matched links with type code restriction in the first step, then we relax the type code restriction in the link scanning process but find the link with the highest speed
		{

			double max_speed = 0;

			

			for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
			{
				if ((*iLink)->m_ShapePoints.size() <2)
					continue;

				// if this sensor has direction code being specified but the link has a different direction, skip

				if ((*iLink)->m_loop_code.size() > 0)  // a link has a special loop code, skip
						continue;


				if ((*iSensor).second.orientation_code.size() > 0 && (*iLink)->m_orientation_code != (*iSensor).second.orientation_code)
						continue;


				// the ariterial link

				if ( m_LinkTypeMap[(*iLink)->m_link_type].IsArterial ())
				{
					for (int si = 0; si < (*iLink)->m_ShapePoints.size() - 1; si++)
					{
						GDPoint p0, pfrom, pto;

						p0 = (*iSensor).second.pt;
						pfrom = (*iLink)->m_ShapePoints[si];
						pto = (*iLink)->m_ShapePoints[si + 1];


						float distance = g_GetPoint2LineDistance(p0, pfrom, pto, m_UnitMile, false);

						if (distance > 0 && distance < User_input_min_distance && (*iLink)->m_SpeedLimit >= max_speed)
						{
							pMatchedLink = (*iLink);

							max_speed = (*iLink)->m_SpeedLimit;

							(*iSensor).second.matched_from_node_id = pMatchedLink->m_FromNodeNumber;
							(*iSensor).second.matched_to_node_id = pMatchedLink->m_ToNodeNumber;

						}
					}
				}


			}

			if (pMatchedLink != NULL)
			{
				if ((*iSensor).second.direction.size() > 0)
					pMatchedLink->m_CountSensorID = (*iSensor).second.SensorID + "_" + (*iSensor).second.direction;
				else
					pMatchedLink->m_CountSensorID = (*iSensor).second.SensorID;



				maptch_count++;

			}
		}



	}

	CString message;
	message.Format("%d sensor locations out of %d sensors have been matched to link layer. Please save the data set to write sensor id into file input_link.csv.", maptch_count, m_SensorMap.size());
	AfxMessageBox(message, MB_ICONINFORMATION);

	m_SensorMapX = 0;
	m_SensorMapY = 0;
}


void CTLiteDoc::OnNetworktoolsGenerateloopcodeanddirectioncode()
{
	int count = 0;

	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		if ((*iLink)->m_ShapePoints.size() <2)
			continue;

		// the same type code or freeway
		GDPoint pfrom, pto;

		pfrom = (*iLink)->m_ShapePoints[0];
		pto = (*iLink)->m_ShapePoints[(*iLink)->m_ShapePoints.size()-1];
		

		if ((*iLink)->m_FromNodeNumber == 2269 && (*iLink)->m_FromNodeNumber == 2251)
			TRACE("");


		int angle = Find_P2P_Angle(pfrom, pto);
		
		if (angle < 45 || angle >= 225)
		{
			(*iLink)->m_orientation_code = "down";
		}
		else
		{
			(*iLink)->m_orientation_code = "up";
		}
		count++;

	}

	CString message;
	message.Format("%d links have been assigned with orientation codes. Please save the data set to write the information into file input_link.csv.", count);
	AfxMessageBox(message, MB_ICONINFORMATION);


}


void CTLiteDoc::OnSensortoolsCleansensordatawithreasonablerange()
{ 
	for (int i = 0; i < m_SensorCountVector.size(); i++)
	{
		DTASensorData element = m_SensorCountVector[i];

		DTALink* pLink = FindLinkWithNodeNumbers(element.link_from_node_id, element.link_to_node_id);

		if (pLink->m_FromNodeNumber == 721 && pLink->m_ToNodeNumber == 685)
		{
			TRACE("");
		}

		if (pLink != NULL )
		{
			if ((m_LinkTypeMap[pLink->m_link_type].IsFreeway() && pLink->m_sensor_hourly_lane_volume_max <= 1200) || pLink->m_sensor_hourly_lane_volume_max <= 500 || pLink->m_sensor_hourly_lane_volume_min >= 2000)
			{
				m_SensorCountVector[i].b_valid_data = false;
				pLink->m_b_invalid_sensor = true;
			}
		}
	}

	UpdateAllViews(0);

	int total_count = 0;
	int invalid_count = 0;

	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		if ((*iLink)->m_bCountSensorData == true)
		{
			total_count++;


			if ((*iLink)->m_b_invalid_sensor == true)
			{
				invalid_count++;
			}
		}
	}
		
	

	CString msg;
	msg.Format("There are %d invalid sensors being identified out of %d sensors. Please save the network to make the change.", invalid_count, total_count);
	AfxMessageBox(msg);
}


void CTLiteDoc::OnReferenceCreatespeedsensormappingforbaselinenetwork()
{
	float min_distance_in_mile = 0.1;
	int matched_count = 0;
	int total_count = 0;

	std::list<DTALine*>::iterator iLine;
	for (std::list<DTALine*>::iterator iLine = m_DTALineSet.begin(); iLine != m_DTALineSet.end(); iLine++)
	{
			total_count++;

			DTALink* pLink =
				FindLinkFromCoordinateLocationV2(
				(*iLine)->m_ShapePoints[0].x,
				(*iLine)->m_ShapePoints[0].y,
				(*iLine)->m_ShapePoints[(*iLine)->m_ShapePoints.size() - 1].x,
				(*iLine)->m_ShapePoints[(*iLine)->m_ShapePoints.size() - 1].y);
		
			if (pLink != NULL)
			{
				pLink->m_SpeedSensorID = (*iLine)->m_LineID;  // from the reference network's link id to the base line network
				matched_count ++;
			}


	}

	CString msg;
	msg.Format("There are %d reference links being identified out of %d links in the reference network. Please save the network to make the change.", matched_count, total_count);
	AfxMessageBox(msg);
	
}


void CTLiteDoc::OnDetectorOverwritesensorlocationandsensorcountdata()
{
	OnDetectorOverwritesensorlocationdata();
	SaveSensorCountData();
}


void CTLiteDoc::OnSensortoolsViewvalidationplotforlinkspeed()
{
	CLinePlotTestDlg dlg;
	dlg.m_pDoc = this;

	COLORREF crColor1 = RGB(100, 149, 237);
	CLinePlotData element_link_speed;
	element_link_speed.crPlot = crColor1;

	element_link_speed.szName = "Link Speed";
	dlg.m_XCaption = "Observed Link Speed";
	dlg.m_YCaption = "Simulated Link Speed";

	element_link_speed.lineType = enum_LpScatter;

	int count = 0;
	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		DTALink* pLink = (*iLink);

		if (pLink->m_SensorDataVector.size()>0)
		{
			for (unsigned i = 0; i< pLink->m_SensorDataVector.size(); i++)
			{
				DTASensorData element = pLink->m_SensorDataVector[i];

				float SensorSpeed = element.speed;
				if (SensorSpeed>1)
				{
					float SimulatedSpeed = pLink->GetSimulatedSpeed(
						element.start_time_in_min, element.end_time_in_min);
					FLOATPOINT data;

					data.x = SensorSpeed;
					data.y = SimulatedSpeed;
					data.Hour = int(element.start_time_in_min / 60);

					data.LinkNo = pLink->m_LinkNo;
					if (m_LinkTypeMap[pLink->m_link_type].IsFreeway())
					{
						data.LinkType = _lp_freeway;
						data.crColor = theApp.m_FreewayColor;

					}

					if (m_LinkTypeMap[pLink->m_link_type].IsArterial())
					{
						data.LinkType = _lp_arterial;
						data.crColor = theApp.m_ArterialColor;
					}

					if (m_LinkTypeMap[pLink->m_link_type].IsHighway())
					{
						data.LinkType = _lp_ramp;
						data.crColor = theApp.m_ArterialColor;
					}


					if (element.start_time_in_min >= m_DemandLoadingStartTimeInMin && element.end_time_in_min <= m_DemandLoadingEndTimeInMin)
					{
						element_link_speed.vecData.push_back(data);
					}
					count++;

				}
			}
		}
	}

	if (count >= 1)
	{

		dlg.m_PlotDataVector.push_back(element_link_speed);
		dlg.DoModal();

	}
	else
	{
		AfxMessageBox("No matching sensor data are available from file sensor_speed.csv. Please prepare file sensor_speed.csv.");
	}
}


void CTLiteDoc::OnTollAddbustoll()
{
	if (m_SelectedLinkNo == -1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink = m_LinkNoMap[m_SelectedLinkNo];
	if (pLink != NULL)
	{

		ReadLink_basedTollScenarioData();

		// add toll
		DTAToll toll;

		toll.StartTime = 0;
		toll.EndTime = 1440;

		for (int p = 0; p < MAX_DEMAND_TYPE_SIZE; p++)
			toll.TollRate[p] = 999;

		toll.TollRate[6] = 0;

		pLink->TollVector.push_back(toll);

		WriteLink_basedTollScenarioData();

		CDlgScenario dlg(0);
		dlg.m_pDoc = this;

		dlg.m_SelectTab = 3;
		dlg.DoModal();

		UpdateAllViews(0);
	}
}


void CTLiteDoc::OnTollAddbrttoll()
{
	if (m_SelectedLinkNo == -1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink = m_LinkNoMap[m_SelectedLinkNo];
	if (pLink != NULL)
	{

		ReadLink_basedTollScenarioData();

		// add toll
		DTAToll toll;

		toll.StartTime = 0;
		toll.EndTime = 1440;

		for (int p = 0; p < MAX_DEMAND_TYPE_SIZE; p++)
			toll.TollRate[p] = 999;

		toll.TollRate[7] = 0;

		pLink->TollVector.push_back(toll);

		WriteLink_basedTollScenarioData();

		CDlgScenario dlg(0);
		dlg.m_pDoc = this;

		dlg.m_SelectTab = 3;
		dlg.DoModal();

		UpdateAllViews(0);
	}
}


void CTLiteDoc::OnTollAddmetrotoll()
{
	if (m_SelectedLinkNo == -1)
	{
		AfxMessageBox("Please select a link first.");
		return;
	}

	DTALink* pLink = m_LinkNoMap[m_SelectedLinkNo];
	if (pLink != NULL)
	{

		ReadLink_basedTollScenarioData();

		// add toll
		DTAToll toll;

		toll.StartTime = 0;
		toll.EndTime = 1440;

		for (int p = 0; p < MAX_DEMAND_TYPE_SIZE; p++)
			toll.TollRate[p] = 999;

		toll.TollRate[9] = 0;

		pLink->TollVector.push_back(toll);

		WriteLink_basedTollScenarioData();

		CDlgScenario dlg(0);
		dlg.m_pDoc = this;

		dlg.m_SelectTab = 3;
		dlg.DoModal();

		UpdateAllViews(0);
	}
}




void CTLiteDoc::OnPathClearallpathdisplay()
{
	for (unsigned int p = 0; p < m_PathDisplayList.size(); p++) // for each path
	{
		m_PathDisplayList[p].m_LinkVector.clear();
	}
	m_PathDisplayList.clear();

	m_OriginNodeID = -1;
	m_DestinationNodeID = -1;

	UpdateAllViews(0);
}


BOOL CTLiteDoc::SaveDefaultInputSignalData(LPCTSTR lpszPathName)
{
	CWaitCursor wait;


	FILE* st;
	fopen_s(&st, lpszPathName, "w");
	if (st != NULL)
	{
		std::list<DTALink*>::iterator iLink;
		fprintf(st, "from_node_id,to_node_id,plan_no,plan_startime_in_sec,plan_endtime_in_sec,to_node_cycle_in_sec,to_node_offset_in_sec,through_saturation_flow_rate_per_hour_per_lane,through_green_start_time_in_sec,through_green_end_time_in_sec,left_saturation_flow_rate_per_hour_per_lane,left_green_start_time_in_sec,left_green_end_time_in_sec,\n");

		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
			int ToNodeID = (*iLink)->m_ToNodeID;
			DTANode* pNode = m_NodeNoMap[ToNodeID];
			//set default green time 
			if (pNode->m_ControlType == m_ControlType_PretimedSignal || pNode->m_ControlType == m_ControlType_ActuatedSignal)
			{
				int left_turn_green_time = 0;
				int through_turn_green_time = max(100, (*iLink)->m_LaneCapacity / 1800 * 100);

				if ((*iLink)->m_NumberOfLeftTurnLanes > 0)
				{
					left_turn_green_time = max(100, (*iLink)->m_LaneCapacity / 1800 * 100);
				}

				fprintf(st, "%d,%d,1,0,86400,100,0,1800,0,%d,1800,0,%d\n",

					(*iLink)->m_FromNodeNumber,
					(*iLink)->m_ToNodeNumber,
					through_turn_green_time,
					left_turn_green_time);

			}
		}

		fclose(st);
	}
	else
	{
		CString message;
		message.Format("Error: File %s cannot be opened.\nIt might be currently used and locked by EXCEL.", lpszPathName);
		AfxMessageBox(message);
		return false;
	}
}


void CTLiteDoc::OnToolsGeneratedefaultinputsignalfile()
{
	CString directory;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);
	SaveDefaultInputSignalData(directory + "input_signal_default.csv");

	CString msg;
	msg.Format("The file %s is now available.", directory + "input_signal_default.csv");  // +2 for the first field name line
	AfxMessageBox(msg);

}




void CTLiteDoc::SaveTimingData()
{
	CString directory;
	directory = m_ProjectFile.Left(m_ProjectFile.ReverseFind('\\') + 1);

	CString AMS_File = directory + "input_timing.csv";
	FILE* st = NULL;

	fopen_s(&st, AMS_File, "w");
	if (st != NULL)
	{
		fprintf(st, "int_id,timing_plan_no,phase_id,next_phase_id,green_duration,movement_str,movement_dir_str,movement_sim_hourly_volume_delay_in_sec,\n");

		// print out matrix 
		for (std::list<DTANode*>::iterator iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++)
		{
			if (((*iNode)->m_ControlType == m_ControlType_PretimedSignal || (*iNode)->m_ControlType == m_ControlType_ActuatedSignal))
			{


				CString timing_plan_name;

				int p;
				for (int timeplan = 1; timeplan < 20; timeplan++)
				{

					timing_plan_name.Format("%d", timeplan);

					for (p = 1; p < 20; p++)
					{

						//refetch data 
						DTA_Phasing_Data_Matrix element = GetPhaseData((*iNode)->m_NodeNumber, timing_plan_name.GetString());

						if (element.GetString((DTA_SIG_PHASE)(DTA_SIG_PHASE_VALUE + p), PHASE_GreenDuration).GetLength() > 0 ||
							element.GetString((DTA_SIG_PHASE)(DTA_SIG_PHASE_VALUE + p), PHASE_MOVEMENT_VECTOR).GetLength() > 0
							)
						{
							fprintf(st, "%d,%d,%d,%s,%s,%s,%s,",
								(*iNode)->m_NodeNumber,
								timeplan, p,
								element.GetString((DTA_SIG_PHASE)(DTA_SIG_PHASE_VALUE + p), PHASE_NextPhaseNo).GetString(),
								element.GetString((DTA_SIG_PHASE)(DTA_SIG_PHASE_VALUE + p), PHASE_GreenDuration).GetString(),
								element.GetString((DTA_SIG_PHASE)(DTA_SIG_PHASE_VALUE + p), PHASE_MOVEMENT_VECTOR).GetString(),
								element.GetString((DTA_SIG_PHASE)(DTA_SIG_PHASE_VALUE + p), PHASE_MOVEMENT_DIR_VECTOR).GetString());

							string movement_str = element.GetString((DTA_SIG_PHASE)(DTA_SIG_PHASE_VALUE + p), PHASE_MOVEMENT_VECTOR).GetString();

							CString movement_MOE_str;
							if (movement_str.size() > 0)
							{
								for (unsigned int m = 0; m < (*iNode)->m_MovementDataMap["0"].m_MovementVector.size(); m++)
								{
									bool bMovementIncluded = IfMovementDirIncludedInPhase((*iNode)->m_NodeNumber,
										timing_plan_name.GetString(), p, m);

									if (bMovementIncluded)
									{

										CString sub_movement_str;

										float number_of_hours = max(0.01, (m_DemandLoadingEndTimeInMin - m_DemandLoadingStartTimeInMin) / 60.0);

										float sim_turn_hourly_count = (*iNode)->m_MovementDataMap["0"].m_MovementVector[m].sim_turn_count / number_of_hours;

										if(sim_turn_hourly_count>1)
										{ 
										sub_movement_str.Format(";%d_%d_%s:%.1f:%.1f", m_NodeNoMap[(*iNode)->m_MovementDataMap["0"].m_MovementVector[m].in_link_from_node_id]->m_NodeNumber,
											m_NodeNoMap[(*iNode)->m_MovementDataMap["0"].m_MovementVector[m].out_link_to_node_id]->m_NodeNumber,
											GetTurnShortString((*iNode)->m_MovementDataMap["0"].m_MovementVector[m].movement_turn),
											sim_turn_hourly_count,
											(*iNode)->m_MovementDataMap["0"].m_MovementVector[m].sim_turn_delay*60);

										movement_MOE_str += sub_movement_str;
										}


									}

								}


								//
							}

							fprintf(st, "%s,", movement_MOE_str);
							fprintf(st, "\n");
						}


					} //p
				}

			}
		}

			fclose(st);
		}
	else
	{
		AfxMessageBox("File input_timing_backup.csv cannot be opened.");
	}

	}



void CTLiteDoc::OnToolsImportsynchroncombinedfile()
{
	// TODO: Add your command handler code here
}


void CTLiteDoc::OnOdmatrixRapid()
{
	CWaitCursor ws;
	m_TrafficFlowModelFlag = 0;
	m_DemandAlpha += 0.1;
	g_AgentLiteProjectDirectory = m_ProjectDirectory;
	AgentLite_main(m_DemandAlpha);

	int link_index = 0;
	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->m_total_link_volume = g_link_volume_vector[link_index];
		(*iLink)->m_avg_simulated_speed = g_link_speed_vector[link_index];
		link_index++;
				
	}

	m_LinkBandWidthMode = LBW_link_volume;
	GenerateOffsetLinkBand();
	UpdateAllViews(0);
}


void CTLiteDoc::OnOdmatrixRapidDecrease()
{

	CWaitCursor ws;
	m_TrafficFlowModelFlag = 0;
	m_DemandAlpha -= 0.1;
	g_AgentLiteProjectDirectory = m_ProjectDirectory;
	AgentLite_main(m_DemandAlpha);
	int link_index = 0;
	for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		(*iLink)->m_total_link_volume = g_link_volume_vector[link_index];
		(*iLink)->m_avg_simulated_speed = g_link_speed_vector[link_index];
		link_index++;
	}
	m_LinkBandWidthMode = LBW_link_volume;
	GenerateOffsetLinkBand();

	UpdateAllViews(0);
}


void CTLiteDoc::OnButtonAbm()
{

	CWaitCursor curs;
	if (!SetCurrentDirectory(m_ProjectDirectory))
	{
		if (m_ProjectDirectory.GetLength() == 0)
		{
			AfxMessageBox("The project directory has not been specified.");
			return;
		}
		CString msg;
		msg.Format("The specified project directory %s does not exist.", m_ProjectDirectory);
		AfxMessageBox(msg);
		return;
	}
	CString sCommand;
	CString strParam;
	CTime ExeStartTime = CTime::GetCurrentTime();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	sCommand.Format("%s\\%s", pMainFrame->m_CurrentDirectory, "AgBM.exe");

	ProcessExecute(sCommand, strParam, m_ProjectDirectory, true);

}
