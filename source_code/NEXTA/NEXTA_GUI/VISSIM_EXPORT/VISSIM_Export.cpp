//  Portions Copyright 2010 Peng @ pdu@bjtu.edu.cn; xzhou99@gmail.com
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

#include "stdafx.h"
#include "..//TLite.h"
#include "..//Network.h"
#include "..//CSVParser.h"
#include "..//Geometry.h"
#include "..//TLiteDoc.h"

#include "comutil.h" 
#include <clocale>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include <io.h>
#include <tchar.h>

// added by Xuesong
#include <iostream>
#include <locale>
//#include <msxml2.h>   
//#import "msxml4.dll" 


#include "VISSIM_Export.h"

// This function is the interface with the main program
void CTLiteDoc::ConstructandexportVISSIMdata(bool bUseSequentialNodeNumber)
{
	CWaitCursor cursor;
	MicroSimulatorInterface ms(this);
	CString strFolder  = m_ProjectDirectory;
	ms.m_strFolder = strFolder;
	ms.m_bUseSequentialNodeNumber = bUseSequentialNodeNumber;
	CString strNodeCSV = strFolder + _T("input_node.csv");;
	CString strLinkCSV = strFolder + _T("input_link.csv");
	CString strLinkTypeCSV = strFolder + _T("input_link_type.csv");
	CString strANMFile = strFolder + _T("simulation.anm");
	CString strANMRoutesFile = strFolder + _T("simulation.anmRoutes");
	CString strLogFile = strFolder + _T("msLog.log");
	CString strZoneCSV = strFolder + _T("input_activity_location.csv");
	CString strODCSV = strFolder + _T("AMS_od_flow.csv");
	CString strPathCSV = strFolder + _T("AMS_path_flow.csv");

	std::string strNodeFileName,strLinkFileName,strLinkTypeFileName,strANMFileName,strODFileName,strPathFileName,strANMRoutesFileName,strLogFileName,strZoneFileName;
	USES_CONVERSION;
	strNodeFileName = T2A(strNodeCSV.GetBuffer());
	strLinkFileName = T2A(strLinkCSV.GetBuffer());
	strLinkTypeFileName = T2A(strLinkTypeCSV.GetBuffer());
	strANMFileName  = T2A(strANMFile.GetBuffer());
	strANMRoutesFileName = T2A(strANMRoutesFile.GetBuffer());
	strLogFileName  = T2A(strLogFile.GetBuffer());
	strZoneFileName = T2A(strZoneCSV.GetBuffer());
	strODFileName	= T2A(strODCSV.GetBuffer());
	strPathFileName = T2A(strPathCSV.GetBuffer());

	ms.OpenLogFile(strLogFileName);
	ms.ReadInputNodeCSV(strNodeFileName);
	CWaitCursor wait;
	ms.ReadInputLinkCSV(strLinkFileName);
	ms.ReadInputLinkTypeCSV(strLinkTypeFileName);
	ms.	ReadInputActivityLocationCSV(strZoneFileName);

	ms.ReadInputSignalCSV("ms_signal.csv");

	bool bReturn = ms.ClassifyNodes();
	ms.DumpNodeLink2Log();
	bReturn = ms.CreateDefaultLanes();
	bReturn = ms.ProcessLanes();
	ms.CreateDefaultData();
	ms.CreateANMFile2(strANMFileName);

	ms.ReadOutputODFlowCSV(strODFileName);
	ms.ReadOutputPathFlowCSV(strPathFileName);
	ms.CreateANMRoutesFile(strANMRoutesFileName);
	ms.CloseLogFile();

	AfxMessageBox("Files simulation.anm and simulation.anmRoutes have been generated.\n", MB_ICONINFORMATION);
	OnToolsProjectfolder();	
}
////////////////////////////////////////////////////////
MSigG::MSigG()
{
}
MSigG::~MSigG()
{
}
MSigC::MSigC()
{
}
MSigC::~MSigC()
{
}PhaseRecord::PhaseRecord()
{
}
PhaseRecord::~PhaseRecord()
{
}
PhaseRecord::PhaseRecord(int PhaseNo, CString Name,int PhaseType, int Angle, int GroupNo, int SequenceType, int StartTime, int GreenLength, int YellowLength/*=0*/, int RedLength/*=0*/, int CycleLength/*=120*/, int Offset /*=0*/)
{
	nPhaseNo	= PhaseNo;
	nPhaseType	= PhaseType;
	nAngle		= Angle;
	nSequenceType = SequenceType;
	nGroupNo	  = GroupNo;
	nStartTime	  = StartTime;
	nGreenLength  = GreenLength;
	nYellowLength = YellowLength;
	nRedLength	  = RedLength;
	strName		  = Name;
	nCycleLength  = CycleLength;
	nOffset		  = Offset;
}
////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
//		realization of MicroSimulatorInterface
///////////////////////////////////////////////////////////////////////////////////////
MicroSimulatorInterface::MicroSimulatorInterface()
{
}
MicroSimulatorInterface::MicroSimulatorInterface(CTLiteDoc* pDoc)
{
	m_pDoc = pDoc;
	m_bUseSequentialNodeNumber = false;

}
MicroSimulatorInterface::~MicroSimulatorInterface()
{
	std::list<MNode*>::iterator iNode;
	for (iNode = m_NodeList.begin(); iNode != m_NodeList.end(); iNode++)
	{
		std::vector<MLaneTurn*>::iterator iTurn;
		for(iTurn=(*iNode)->LaneTurns.begin();iTurn!=(*iNode)->LaneTurns.end();iTurn++)
			delete (*iTurn);
		(*iNode)->LaneTurns.clear();
		delete *iNode;
	}
	m_NodeList.clear();

	std::list<MLink*>::iterator iLink;
	for(iLink = m_LinkList.begin();iLink!=m_LinkList.end();iLink++)
	{
		std::vector<MLane*>::iterator iLane;
		for(iLane=(*iLink)->inLanes.begin();iLane!=(*iLink)->inLanes.end();iLane++)
			delete (*iLane);
		(*iLink)->inLanes.clear();
		for(iLane=(*iLink)->outLanes.begin();iLane!=(*iLink)->outLanes.end();iLane++)
			delete (*iLane);
		(*iLink)->outLanes.clear();
		delete (*iLink);
	}
	m_LinkList.clear();

	std::vector<MDemand*>::iterator iod;
	for(iod = m_odDemand.begin();iod!=m_odDemand.end();iod++)
	{
		delete (*iod);
	}
	m_odDemand.clear();

	std::vector<MPath*>::iterator ipath;
	for(ipath = m_pathDemand.begin();ipath!=m_pathDemand.end();ipath++)
	{
		delete (*ipath);
	}
	m_pathDemand.clear();
}
MNode::MNode()
{
}
MNode::~MNode()
{
}
MLink::MLink()
{
}
MLink::~MLink()
{
}
MLane::MLane()
{
}
MLane::~MLane()
{
}
MMovement::MMovement()
{
}
MMovement::~MMovement()
{
}
MLaneTurn::MLaneTurn()
{
}
MZone::MZone()
{
}
MZone::~MZone()
{
}
CMVehClass::CMVehClass()
{
}

CMVehClass::~CMVehClass()
{
}
CMSignalControl::CMSignalControl()
{
}
CMSignalControl::~CMSignalControl()
{
}
CMSignalGroup::CMSignalGroup()
{
}
CMSignalGroup::~CMSignalGroup()
{
}
MPath::MPath()
{
}
MPath::~MPath()
{
}
CString MicroSimulatorInterface::Minutes2PTString(int nMin)
{
	CString	szRt = _T("");
	int mm = nMin / 60;
	int ss = nMin - mm*60;
	if ( 0 == mm)
		szRt.Format(TEXT("PT%dS"),ss);
	else if ( 0 == ss)
		szRt.Format(TEXT("PT%dM"),mm);
	else
		szRt.Format(TEXT("PT%dM%dS"),mm,ss);

	return szRt;
}
MLaneTurn::~MLaneTurn()
{
}
MDemand::MDemand(){}
MDemand::~MDemand(){}
float MicroSimulatorInterface::L2X(float longitude,float latitude)
{
	float x = 0.0;
	x = (longitude - refLongi) * refScale * cos(latitude / 180);
	return x;
}
float MicroSimulatorInterface::L2Y(float latitude)
{
	float y = 0.0;
	y = (latitude - refLati) * refScale;
	return y;
}
bool MicroSimulatorInterface::CloseLogFile()
{
	if (m_logFile.is_open())
		m_logFile.close();
	return true;

}
bool MicroSimulatorInterface::OpenLogFile(std::string strLogFileName)
{
	m_logFile.open (strLogFileName.c_str(), ios::out);
	if (m_logFile.is_open())
	{
		m_logFile.width(12);
		m_logFile.precision(3) ;
		m_logFile.setf(ios::fixed);
		return true;
	}
	else
	{
		AfxMessageBox("File NeXTA.log cannot be opened, and it might be locked by another program!");
		return false;
	}

}
bool MicroSimulatorInterface::ReadInputNodeCSV(std::string strFileName)
{
	m_logFile<< "ReadInputNodeCSV function called!"<<endl;
	CCSVParser parser;
	if (parser.OpenCSVFile(strFileName))
	{
		int i=0;
		while(parser.ReadRecord())
		{
			int node_id;
			string name;
			MNode* pNode = 0;

			int control_type;
			double X;
			double Y;
			if(parser.GetValueByFieldName("node_id",node_id) == false)
				break;


			m_OriginalNodeNumbertoNodeNumberMap[node_id] = i+1;

			if(m_bUseSequentialNodeNumber)  // replace the original node number by the sequential node no. 
			{
						node_id = i+1;
			}
			if(!parser.GetValueByFieldName("name",name))
				name = "";

			if(!parser.GetValueByFieldName("control_type",control_type))
				control_type = 0;

			// use the X and Y as default values first
			bool bFieldX_Exist = parser.GetValueByFieldName("x",X);
			parser.GetValueByFieldName("y",Y);

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
					//X /= 1000000;
					//Y /= 1000000;
				}

			}

			pNode = new MNode;
			pNode->m_Name = name;
			pNode->nControlType = control_type;
			if ( 0 == i)
			{
				refLongi = X;
				refLati  = Y;
				refScale = 1.0 / (m_pDoc->m_UnitFeet)*0.3048; // from feet to meter
			}

			pNode->ptLL.x = X;
			pNode->ptLL.y = Y;

			pNode->pt.x = L2X(X,Y);
			pNode->pt.y = L2Y(Y);

			pNode->m_NodeNumber = node_id;
			pNode->m_NodeNo = i;
			pNode->m_ZoneID = 0;
			m_NodeList.push_back(pNode);
			m_NodeNoMap[i] = pNode;
			m_NodeNotoNumberMap[i] = node_id;
			m_NodeNumbertoNodeNoMap[node_id] = i;
			i++;

			//			cout << "node = " << node << ", X= " << X << ", Y = " << Y << endl;

		}
		//CString strMsg;
		//strMsg.Format("%d nodes are loaded from file %s.",m_NodeList.size(),strFileName.c_str());
		//m_logFile<<strMsg<<endl;
		//AfxMessageBox(strMsg, MB_ICONINFORMATION);
		return true;
	}
	else
	{
		AfxMessageBox("Error: File input_node.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}
}
bool MicroSimulatorInterface::ReadInputLinkCSV(std::string strFileName)
{
	CCSVParser parser;
	CString error_message;
	int i=0;

	if (parser.OpenCSVFile(strFileName))
	{
		while(parser.ReadRecord())
		{
			std::string name;
			int link_id = 0;
			int from_node_id;
			int to_node_id;
			int number_of_lanes= 1;
			int speed_limit_in_mph;
			int type = 1;
			std::string from_approach;
			std::string to_approach;
			int reverse_link_id = 0;
			int nFrom = 0;

			if(!parser.GetValueByFieldName("name",name))				name = "";

			link_id = m_LinkList.size()+1;

			//if(!parser.GetValueByFieldName("link_id",link_id))
			//{
			//	AfxMessageBox("Field link_id has not been defined in file input_link.csv. Please check.");
			//	break;
			//}
			// 检查link_id的唯一性
			//if ( !CheckDuplicateLink(link_id)) continue;
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


			if(m_bUseSequentialNodeNumber  )  // replace the original node number by the sequential node no. 
			{
				if (m_OriginalNodeNumbertoNodeNumberMap.find(from_node_id)==m_OriginalNodeNumbertoNodeNumberMap.end() ||
					m_OriginalNodeNumbertoNodeNumberMap.find(to_node_id)	==m_OriginalNodeNumbertoNodeNumberMap.end() )
				{
					AfxMessageBox("Either from_node or to_node_id is not found in node list. Please check.");
					break;
				}


				from_node_id = m_OriginalNodeNumbertoNodeNumberMap[from_node_id];
				to_node_id = m_OriginalNodeNumbertoNodeNumberMap[to_node_id];
			
			}

			if (m_NodeNumbertoNodeNoMap.find(from_node_id)==m_NodeNumbertoNodeNoMap.end() ||
				m_NodeNumbertoNodeNoMap.find(to_node_id)	==m_NodeNumbertoNodeNoMap.end() )
			{
				AfxMessageBox("Either from_node or to_node_id is not found in node list. Please check.");
				break;
			}

			if(!parser.GetValueByFieldName("number_of_lanes",number_of_lanes))		 number_of_lanes = 1; // default
			if(!parser.GetValueByFieldName("speed_limit_in_mph",speed_limit_in_mph)) speed_limit_in_mph = 20;
			if(!parser.GetValueByFieldName("link_type",type))						 type= 100;
			if(!parser.GetValueByFieldName("from_approach",from_approach))			 from_approach = "0";
			if(!parser.GetValueByFieldName("to_approach",to_approach))				 to_approach   = "0";
			if(!parser.GetValueByFieldName("reversed_link_id",reverse_link_id))		 reverse_link_id = 0;

			string geo_string;
			std::vector<CCoordinate> CoordinateVector;
			if(parser.GetValueByFieldName("geometry",geo_string))
			{
				// overwrite when the field "geometry" exists
				CGeometry geometry(geo_string);
				CoordinateVector = geometry.GetCoordinateList();
			}
			else
			{
				// no geometry information
				CCoordinate cc_from, cc_to; 
				cc_from.X = m_NodeNoMap[m_NodeNumbertoNodeNoMap[from_node_id]]->ptLL.x;
				cc_from.Y = m_NodeNoMap[m_NodeNumbertoNodeNoMap[from_node_id]]->ptLL.y;

				cc_to.X = m_NodeNoMap[m_NodeNumbertoNodeNoMap[to_node_id]]->ptLL.x;
				cc_to.Y = m_NodeNoMap[m_NodeNumbertoNodeNoMap[to_node_id]]->ptLL.y;

				CoordinateVector.push_back(cc_from);
				CoordinateVector.push_back(cc_to);
			}

			if (0 == from_node_id || 0 == to_node_id)
			{
				m_logFile<<"Warning: Link ID ["<<link_id<<"] has no approach! Discarded."<<endl;
				continue;
			}

			MLink*	pLink = new MLink();
			m_LinkList.push_back(pLink);

			pLink->strANMID =_T("NULL");
			pLink->strANMRID=_T("NULL");

			pLink->m_LinkNo = i;
			pLink->m_Name  = name;
			pLink->m_LinkID = link_id;
			pLink->m_FromNodeNumber = from_node_id;
			pLink->m_ToNodeNumber = to_node_id;
			pLink->m_FromNodeID = m_NodeNumbertoNodeNoMap[from_node_id];
			pLink->m_ToNodeID= m_NodeNumbertoNodeNoMap[to_node_id];

			for(int si = 0; si < CoordinateVector.size(); si++)
			{

				// Xuesong: 09/06/2012: avoid reading and writing too many shape points for curve fitting

				if(si==0 || si == CoordinateVector.size()-1)  // first point and the last point
				{
				GDPoint	pt;
				pt.x = L2X(CoordinateVector[si].X,CoordinateVector[si].Y);
				pt.y = L2Y(CoordinateVector[si].Y);

				pLink->m_ShapePoints .push_back (pt);
				}
			}

			pLink->m_NumberOfLanes= max(1,number_of_lanes);
			pLink->m_SpeedLimit = max(20, speed_limit_in_mph*1.60934);  // minimum speed limit is 20 km per hour, 1.60934 from mile to meter	
			pLink->m_LinkType= type; //type, default = 100;

			GetMLinkApproach(from_approach,&nFrom);
			pLink->m_FromNodeApproach	= nFrom;
			GetMLinkApproach(to_approach,&nFrom);
			pLink->m_ToNodeApproach		= nFrom;
			pLink->m_ReverseLinkID		= reverse_link_id;

			//calculate angles
			pLink->nInAngle = m_pDoc->Find_P2P_Angle( m_NodeNoMap[pLink->m_FromNodeID]->pt,m_NodeNoMap[pLink->m_ToNodeID]->pt);
			pLink->nOutAngle = m_pDoc->Find_P2P_Angle( m_NodeNoMap[pLink->m_ToNodeID]->pt,m_NodeNoMap[pLink->m_FromNodeID]->pt);

			m_NodeNoMap[pLink->m_FromNodeID]->AddOutLink(pLink);
			m_NodeNoMap[pLink->m_ToNodeID]->AddInLink(pLink);
			//m_NodeNoMap[pLink->m_FromNodeID]->outLinks.push_back(pLink);
			//m_NodeNoMap[pLink->m_ToNodeID]->inLinks.push_back(pLink);
			//m_NodeNoMap[pLink->m_FromNodeID]->outLinkMap[pLink->m_FromNodeApproach] = pLink;
			//m_NodeNoMap[pLink->m_ToNodeID]->inLinkMap[pLink->m_ToNodeApproach] = pLink;

			//m_LinkNotoLinkMap[i] = pLink;
			i++;
		}
		m_logFile<<"Sumary: All together "<<i<<" links have been read!"<<endl;
		return true;
	}
	else
	{
		AfxMessageBox("Error: File input_link.csv cannot be opened.\n It might be currently used and locked by EXCEL.");
		return false;
	}
}
bool MicroSimulatorInterface::CheckDuplicateLink(int link_id)
{
	std::list<MLink*>::iterator iLink;
	for(iLink = m_LinkList.begin();iLink != m_LinkList.end();iLink++)
	{
		if ( link_id == (*iLink)->m_LinkID)
			return false;
	}
	return true;
}
void MicroSimulatorInterface::DumpNodeLink2Log()
{
	std::list<MNode*>::iterator iNode;
	for(iNode=m_NodeList.begin();iNode!=m_NodeList.end();iNode++)
	{
		MNode *pNode = (*iNode);
		m_logFile<<"NodeID:"<<pNode->m_NodeNo<<" NodeNumber:"<<pNode->m_NodeNumber<<" ProcsType: "<<pNode->m_nProcessType<<" inLink "<<pNode->inLinks.size()<<" outLink "<<pNode->outLinks.size()<<endl;
		std::vector<MLink*>::iterator iLink;
		for(iLink=pNode->inLinks.begin();iLink!=pNode->inLinks.end();iLink++)
			m_logFile<<"inLinkID: "<<(*iLink)->m_LinkID<<" toAppr: "<<(*iLink)->m_ToNodeApproach<<endl;
		for(iLink=pNode->outLinks.begin();iLink!=pNode->outLinks.end();iLink++)
			m_logFile<<"outLinkID:"<<(*iLink)->m_LinkID<<" fmAppr: "<<(*iLink)->m_FromNodeApproach<<endl;
	}
}

int MLink::GetLaneCount(int nInOut, int RTL /* = 0*/)
{
	int nCount = 0;
	std::vector<MLane*>::iterator iLane;

	if ( 1 == nInOut )
	{
		if ( 0 == RTL)
		{
			nCount = inLanes.size();
		}
		else if ( 1 == RTL)
		{
			for(iLane=inLanes.begin();iLane!=inLanes.end();iLane++)
			{
				if ((*iLane)->rightTurn)
				{
					nCount ++;
				}
			}
		}
		else if ( 2 == RTL )
		{
			for(iLane=inLanes.begin();iLane!=inLanes.end();iLane++)
			{
				if ((*iLane)->through)
				{
					nCount ++;
				}
			}
		}
		else if ( 3 == RTL )
		{
			for(iLane=inLanes.begin();iLane!=inLanes.end();iLane++)
			{
				if ((*iLane)->leftTurn)
				{
					nCount ++;
				}
			}
		}
		else
		{
		}
	}
	else // 0 == nInOut
	{
		if ( 0 == RTL)
		{
			nCount = outLanes.size();
		}
		else if ( 1 == RTL)
		{
			for(iLane=outLanes.begin();iLane!=outLanes.end();iLane++)
			{
				if ((*iLane)->rightTurn)
				{
					nCount ++;
				}
			}
		}
		else if ( 2 == RTL )
		{
			for(iLane=outLanes.begin();iLane!=outLanes.end();iLane++)
			{
				if ((*iLane)->through)
				{
					nCount ++;
				}
			}
		}
		else if ( 3 == RTL )
		{
			for(iLane=outLanes.begin();iLane!=outLanes.end();iLane++)
			{
				if ((*iLane)->leftTurn)
				{
					nCount ++;
				}
			}
		}
		else
		{
		}
	}

	return nCount;
}
int MicroSimulatorInterface::GetMLinkApproach(std::string dir,int* nAppr)
{
	std::string nn("N");
	std::string ee("E");
	std::string ss("S");
	std::string ww("W");

	if ( dir == nn)
		*nAppr = 1;
	else if ( dir == ee) 
		*nAppr = 2;
	else if ( dir == ss) 
		*nAppr = 3;
	else if ( dir == ww) 
		*nAppr = 4;
	else
		*nAppr = 0;

	return *nAppr;
}
int MicroSimulatorInterface::GetNeighborApproach(int la, int RTL,int *appr)
{
	int neighbor=0;
	switch (RTL)
	{
	case 1://R
		if		(1	== la) neighbor = 4;
		else if (2	== la) neighbor = 1;
		else if (3	== la) neighbor = 2;
		else			   neighbor = 3;
		break;
	case 2://T
		if		(1	== la) neighbor = 3;
		else if (2	== la) neighbor = 4;
		else if (3	== la) neighbor = 1;
		else			   neighbor = 2;
		break;
	case 3://L
		if		(1	== la) neighbor = 2;
		else if (2	== la) neighbor = 3;
		else if (3	== la) neighbor = 4;
		else			   neighbor = 1;
		break;
	}
	if ( neighbor == 0)
		AfxMessageBox("Neighbor direction is wrong!");
	else
		*appr = neighbor;

	return neighbor;
}

bool MicroSimulatorInterface::ReadInputLaneCSV(std::string strFileName)
{
	return true;
}
bool MicroSimulatorInterface::ReadInputLinkTypeCSV(std::string strFileName)
{
	m_logFile<< "ReadInputLinkTypeCSV function called!"<<endl;
	m_szLinkTypes = _T("");     // This string is reserved for attributes of LINKTYPES. The following 3 are in groups

	CCSVParser parser;
	if (parser.OpenCSVFile(strFileName))
	{
		int i=0;
		while(parser.ReadRecord())
		{
			int link_type;
			std::string link_type_name;
			int freeway_flag;
			int ramp_flag;
			int arterial_flag;
			CString tt;

			if(parser.GetValueByFieldName("link_type",link_type) == false)			 continue;
			if(parser.GetValueByFieldName("link_type_name",link_type_name) == false) continue;

			// remove space
			link_type_name.erase(link_type_name.find_last_not_of(" \n\r\t")+1);


			if(parser.GetValueByFieldName("freeway_flag",freeway_flag) == false)	 freeway_flag=0;
			if(parser.GetValueByFieldName("ramp_flag",ramp_flag) == false)			 ramp_flag=0;
			if(parser.GetValueByFieldName("arterial_flag",arterial_flag) == false)	 arterial_flag=0;

			tt.Format("%d",link_type);
			m_szLinkTypeNo.Add(tt);
			m_szLinkTypeName.Add(link_type_name.c_str());
			m_szDrivingBehavior.Add(_T("Urban"));
		}
	}
	return true;
}
bool MicroSimulatorInterface::ReadInputDemandCSV(std::string strFileName)
{
	m_logFile<< "ReadInputDemandCSV function called!"<<endl;
	CCSVParser parser;
	if (parser.OpenCSVFile(strFileName))
	{
		int i=0;
		while(parser.ReadRecord())
		{
			int from_zone_id;
			int to_zone_id;
			float number_of_vehicle_trips_type1;
			float number_of_vehicle_trips_type2;
			float number_of_vehicle_trips_type3;
			float number_of_vehicle_trips_type4;
			int starting_time_in_min;
			int ending_time_in_min;

			if(parser.GetValueByFieldName("from_zone_id",from_zone_id) == false)
				break;

			if(parser.GetValueByFieldName("to_zone_id",to_zone_id) == false)
				break;

			if(parser.GetValueByFieldName("number_of_vehicle_trips_type1",number_of_vehicle_trips_type1) == false)
				break;
			if(parser.GetValueByFieldName("number_of_vehicle_trips_type2",number_of_vehicle_trips_type2) == false)
				break;
			if(parser.GetValueByFieldName("number_of_vehicle_trips_type3",number_of_vehicle_trips_type3) == false)
				break;
			if(parser.GetValueByFieldName("number_of_vehicle_trips_type4",number_of_vehicle_trips_type4) == false)
				break;

			if(parser.GetValueByFieldName("starting_time_in_min",starting_time_in_min) == false)
				break;

			if(parser.GetValueByFieldName("ending_time_in_min",ending_time_in_min) == false)
				break;

			MDemand* pDemand = new MDemand();
			pDemand->m_nFromZone = from_zone_id;
			pDemand->m_nToZone = to_zone_id;
			pDemand->m_fDemand[0] = number_of_vehicle_trips_type1;
			pDemand->m_fDemand[1] = number_of_vehicle_trips_type2;
			pDemand->m_fDemand[2] = number_of_vehicle_trips_type3;
			pDemand->m_fDemand[3] = number_of_vehicle_trips_type4;
			//pDemand->m_nStartMin  = starting_time_in_min;
			//pDemand->m_nEndMin	  = ending_time_in_min;

			m_DemandVector.push_back(pDemand);
			i++;
		}
		std::vector<MDemand*>::iterator iDemand;
		for(iDemand=m_DemandVector.begin();iDemand!=m_DemandVector.end();iDemand++)
			m_logFile<<"Demand:"<<(*iDemand)->m_nFromZone<<","<<(*iDemand)->m_nToZone<<"["<<(*iDemand)->m_fDemand[0]<<"]"<<endl;
		return true;
	}
	else
	{
		AfxMessageBox("Error: File input_demand.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}
}
void MicroSimulatorInterface::ReadInputZoneCentroid(std::string strFileName)
{
	m_logFile<< "ReadInputZoneCentroid function called!"<<endl;
	CCSVParser parser;
	if (parser.OpenCSVFile(strFileName))
	{
		while(parser.ReadRecord())
		{
			int zone_id;
			if(parser.GetValueByFieldName("zone_id",zone_id) == false)
				break;

			string geo_string;
			std::vector<CCoordinate> CoordinateVector;
			if(parser.GetValueByFieldName("geometry",geo_string))
			{
				CGeometry geometry(geo_string);
				CoordinateVector = geometry.GetCoordinateList();
				float x=0,y=0;
				for(int kk = 0; kk<CoordinateVector.size();kk++)
				{
					x += CoordinateVector[kk].X;
					y += CoordinateVector[kk].Y;
				}
				x = x / CoordinateVector.size();
				y = y / CoordinateVector.size();
				GDPoint pt;
				pt.x = L2X(x,y);
				pt.y = L2Y(y);

				MZone* pRefZone = new MZone();
				pRefZone->m_nID = zone_id;
				pRefZone->pt    = pt;
				m_RefZone.push_back(pRefZone);				
			}
		}
	}
	m_logFile<< "ReadInputZoneCentroid function ended!"<<endl;
	return ;
}
GDPoint MicroSimulatorInterface::GetZoneCentroid(int nZoneID)
{
	GDPoint pt;
	pt.x=0;pt.y=0;

	for(int i=0;i<m_RefZone.size();i++)
	{
		if ( nZoneID == m_RefZone[i]->m_nID )
		{
			pt = m_RefZone[i]->pt;
			break;
		}
	}
	return pt;
}
bool MicroSimulatorInterface::ReadInputZoneCSV2(std::string strFileName)
{
	// 读入zone后，判断zone中节点的类型，剔除！
	// 一种是纯虚拟点，其所衔接的link均应为(connector)，该node应从物理node中剔除，同时所有衔接的物理node均为该zone包含的点
	// 还需要修改od path，把相关的首/尾点去掉！
	// 另一种为具有虚拟属性的物理点，其所衔接的link可能为物理link，可能为(connector)
	m_logFile<< "ReadInputZoneCSV function called!"<<endl;
	CCSVParser parser;
	if (parser.OpenCSVFile(strFileName))
	{
		int i=0;
		MZone* pZone;
		int old_zone_id = -1;

		while(parser.ReadRecord())
		{
			int node_id;
			int zone_id;

			if(parser.GetValueByFieldName("node_id",node_id) == false)
				break;

	
			if(parser.GetValueByFieldName("zone_id",zone_id) == false)
				break;

			if ( 0 == i || old_zone_id != zone_id )
			{
				pZone = new MZone();
				pZone->m_nID = zone_id;
				old_zone_id = zone_id;
				m_ZoneVector.push_back(pZone);
				i++;
				MNode* pNode = m_NodeNoMap[m_NodeNumbertoNodeNoMap[node_id]];
				if (!pNode)
				{
					CString strError;
					strError.Format("In zone_id = %d, node_id = %d does not exist!",zone_id,node_id);
					m_logFile<<strError<<endl;
					AfxMessageBox(strError);
				}
				pZone->m_nodes.push_back(pNode);
				GDPoint pt;// = GetZoneCentroid(pZone->m_nID);
				//if ( 0 == pt.x && 0 == pt.y)
				//{
				pt.x = pNode->pt.x * 1.01;
				pt.y = pNode->pt.y * 1.01;
				//}
				pZone->pt = pt;
			}
			else
			{
				MNode* pNode = m_NodeNoMap[m_NodeNumbertoNodeNoMap[node_id]];
				if (!pNode)
				{
					CString strError;
					strError.Format("In zone_id = %d, node_id = %d does not exist!",zone_id,node_id);
					m_logFile<<strError<<endl;
					AfxMessageBox(strError);
				}
				pZone->m_nodes.push_back(pNode);
			}
		}
		std::vector<MZone*>::iterator iZone;
		for(iZone=m_ZoneVector.begin();iZone!=m_ZoneVector.end();iZone++)
		{
			m_logFile<<" zone_id: "<<(*iZone)->m_nID<<" node number:["<<(*iZone)->m_nodes.size()<<"]"<<endl;
			std::vector<MNode*>::iterator iNode;
			int i=0;
			for(iNode=(*iZone)->m_nodes.begin();iNode!=(*iZone)->m_nodes.end();iNode++)
				m_logFile<<"["<<++i<<"] node_id: "<<(*iNode)->m_NodeNumber<<endl;
		}
		return true;
	}
	else
	{
		AfxMessageBox("Error: File input_zone.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}
}

bool MicroSimulatorInterface::ReadInputActivityLocationCSV(std::string strFileName)
{
	m_logFile<< "ReadInputZoneCSV function called!"<<endl;
	CCSVParser parser;
	if (parser.OpenCSVFile(strFileName))
	{
		int i=0;
		MZone* pZone;
		int old_zone_id = -1;

		while(parser.ReadRecord())
		{
			int node_id;
			int zone_id;

			if(parser.GetValueByFieldName("node_id",node_id) == false)
				break;
			
			
			if(m_bUseSequentialNodeNumber  )  // replace the original node number by the sequential node no. 
			{
				if (m_OriginalNodeNumbertoNodeNumberMap.find(node_id)==m_OriginalNodeNumbertoNodeNumberMap.end())
				{
					break;
				}


				node_id = m_OriginalNodeNumbertoNodeNumberMap[node_id];
			}
			

			if(parser.GetValueByFieldName("zone_id",zone_id) == false)
				break;

			if ( 0 == i || old_zone_id != zone_id )
			{
				pZone = new MZone();
				pZone->m_nID = zone_id;
				old_zone_id = zone_id;
				m_ZoneVector.push_back(pZone);
				i++;
				MNode* pNode = m_NodeNoMap[m_NodeNumbertoNodeNoMap[node_id]];
				if (!pNode)
				{
					CString strError;
					strError.Format("In zone_id = %d, node_id = %d does not exist!",zone_id,node_id);
					m_logFile<<strError<<endl;
					AfxMessageBox(strError);
				}
				pZone->m_nodes.push_back(pNode);
				GDPoint pt;// = GetZoneCentroid(pZone->m_nID);
				//if ( 0 == pt.x && 0 == pt.y)
				//{
				pt.x = pNode->pt.x * 1.01;
				pt.y = pNode->pt.y * 1.01;
				//}
				pZone->pt = pt;
			}
			else
			{
				MNode* pNode = m_NodeNoMap[m_NodeNumbertoNodeNoMap[node_id]];
				if (!pNode)
				{
					CString strError;
					strError.Format("In zone_id = %d, node_id = %d does not exist!",zone_id,node_id);
					m_logFile<<strError<<endl;
					AfxMessageBox(strError);
				}
				pZone->m_nodes.push_back(pNode);
			}
		}
		std::vector<MZone*>::iterator iZone;
		for(iZone=m_ZoneVector.begin();iZone!=m_ZoneVector.end();iZone++)
		{
			m_logFile<<" zone_id: "<<(*iZone)->m_nID<<" node number:["<<(*iZone)->m_nodes.size()<<"]"<<endl;
			std::vector<MNode*>::iterator iNode;
			int i=0;
			for(iNode=(*iZone)->m_nodes.begin();iNode!=(*iZone)->m_nodes.end();iNode++)
				m_logFile<<"["<<++i<<"] node_id: "<<(*iNode)->m_NodeNumber<<endl;
		}
		return true;
	}
	else
	{
		AfxMessageBox("Error: File input_zone.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}
}
bool MicroSimulatorInterface::ReadInputSignalCSV(std::string strFileName)
{
	m_logFile<< "ReadInputSignalCSV function called!"<<endl;
	CCSVParser parser;
	int c=0,g=0;

	if (parser.OpenCSVFile(strFileName))
	{
		int oldsc = -1;
		MSigC* pSC;
		while(parser.ReadRecord())
		{
			int sc;
			std::string name;
			int cycle_length;
			int offset;
			int sg;
			int gt_start;
			int gt_end;
			int min_gt;
			int at;
			int sc_type;

			parser.GetValueByFieldName("sc",sc);
			parser.GetValueByFieldName("name",name);
			parser.GetValueByFieldName("cycle_length",cycle_length);
			parser.GetValueByFieldName("offset",offset);
			parser.GetValueByFieldName("sg",sg);
			parser.GetValueByFieldName("gt_start",gt_start);
			parser.GetValueByFieldName("gt_end",gt_end);
			parser.GetValueByFieldName("min_gt",min_gt);
			parser.GetValueByFieldName("at",at);
			parser.GetValueByFieldName("sc_type",sc_type);

			if ( sc != oldsc || !pSC )
			{
				pSC = new MSigC();
				pSC->nID = sc;
				pSC->strName = name;
				pSC->nCycleLength = cycle_length;
				pSC->nOffset = offset;
				pSC->nType   = sc_type;
				m_SCs.push_back(pSC);
				oldsc = sc;
				c++;
			}
			MSigG* pSG = new MSigG();
			pSG->nSGNO = sg;
			pSG->nGTStart = gt_start;
			pSG->nGTEnd   = gt_end;
			pSG->nMinGT   = min_gt;
			pSG->nAT      = at;
			pSC->SGs.push_back(pSG);
			g++;
		}
	}
	CString strLine;
	strLine.Format("ReadInputSignalCSV function ended with %d SCs and %d SGs!\n",c,g);
	m_logFile<<strLine;
	return true;
}
int  MicroSimulatorInterface::GetSCNO(int nNodeNumber)
{
	int n = 9133;
	return n;
}
int  MicroSimulatorInterface::GetSGNO(int nNodeNumber, int appr, int RTL)
{
	//int		SG_NORTH_RIGHT	=1;
	//int		SG_NORTH_THROUGH=1;
	//int		SG_SOUTH_RIGHT	=1;
	//int		SG_SOUTH_THROUGH=1;
	//int		SG_EAST_RIGHT	=2;
	//int		SG_EAST_THROUGH	=2;
	//int		SG_WEST_RIGHT	=2;
	//int		SG_WEST_THROUGH	=2;
	//int		SG_EAST_LEFT	=3;
	//int		SG_WEST_LEFT	=3;
	//int		SG_NORTH_LEFT	=4;
	//int		SG_SOUTH_LEFT	=4;

	int SigGroupNo = 1;
	if ( 1 == appr || 3 == appr )
	{
		if ( 1 == RTL || 2 == RTL)
			SigGroupNo = 1;
		else // ( 3 == RTL)
			SigGroupNo = 4;
	}
	else // ( 4 == appr || 2 == appr)
	{
		if ( 1 == RTL || 2 == RTL)
			SigGroupNo = 2;
		else // ( 3 == RTL)
			SigGroupNo = 3;
	}
	return SigGroupNo;
}
bool MicroSimulatorInterface::ClassifyNodes(void)
{
	m_logFile<< "ClassifyNodes function called!"<<endl;
	// classify node types
	std::list<MNode*>::iterator iNode;
	MNode* pNode;
	std::vector<MLink*>::iterator iLink;
	int i=0;

	// classify nodes
	for (iNode = m_NodeList.begin(); iNode != m_NodeList.end(); iNode++,i++)
	{
		pNode = (*iNode);
		int nInDegree = pNode->inLinks.size();
		int nOutDegree= pNode->outLinks.size();

		if ( nInDegree ==0 && nOutDegree == 0)
		{
			pNode->m_nProcessType = -1; // lonely node, deserted
			continue;
		}

		if ( nInDegree ==0 || nOutDegree == 0)
		{
			pNode->m_nProcessType = 0;
			pNode->nControlType   = 0;
		}
		else if (nInDegree == 1 && nOutDegree == 1)
		{
			iLink=pNode->inLinks.begin();
			int nReverseID = (*iLink)->m_ReverseLinkID;
			iLink=pNode->outLinks.begin();
			int nOutLinkID = (*iLink)->m_LinkID;
			if (nReverseID == nOutLinkID && nReverseID != 0)
				pNode->m_nProcessType = 1;
			else
				pNode->m_nProcessType = 2;
			pNode->nControlType = 0;
		}
		else if (nInDegree == 2 && nOutDegree == 2)
		{
			pNode->m_nProcessType =2;
			pNode->nControlType   =0;
		}
		else if (nInDegree ==3 && nOutDegree == 3)
		{
			// in和out缺少的方向相同；没有重复方向
			int nMiss = pNode->CheckMissingApproach();

			if (nMiss >= 1 && nMiss <=4)
			{
				pNode->m_nProcessType = 3;
				pNode->nControlType   = 1;
				// 赋方向映射
				for(iLink=pNode->inLinks.begin();iLink!=pNode->inLinks.end();iLink++)
					pNode->inLinkMap[(*iLink)->m_ToNodeApproach] = (*iLink);
				for(iLink=pNode->outLinks.begin();iLink!=pNode->outLinks.end();iLink++)
					pNode->outLinkMap[(*iLink)->m_FromNodeApproach] = (*iLink);
			}
			else
			{
				pNode->m_nProcessType = 5;
				pNode->nControlType   = 0;
			}
		}
		else if (nInDegree == 4 && nOutDegree == 4)
		{
			int nIns=0,nOuts=0;
			for(iLink=pNode->inLinks.begin();iLink!=pNode->inLinks.end();iLink++)
				nIns += (*iLink)->m_ToNodeApproach;
			for(iLink=pNode->outLinks.begin();iLink!=pNode->outLinks.end();iLink++)
				nOuts += (*iLink)->m_FromNodeApproach;

			if (nIns == nOuts && nIns == 10)
			{
				pNode->m_nProcessType = 4;
				pNode->nControlType   = 1;
				// 赋方向映射
				for(iLink=pNode->inLinks.begin();iLink!=pNode->inLinks.end();iLink++)
					pNode->inLinkMap[(*iLink)->m_ToNodeApproach] = (*iLink);
				for(iLink=pNode->outLinks.begin();iLink!=pNode->outLinks.end();iLink++)
					pNode->outLinkMap[(*iLink)->m_FromNodeApproach] = (*iLink);
			}
			else
			{
				pNode->m_nProcessType = 5;
				pNode->nControlType   = 0;
			}
		}
		else
		{
			pNode->m_nProcessType = 5;
			pNode->nControlType   = 0;
		}
	}
	CString strMsg;
	strMsg.Format("ClassifyNodes function ended with %d nodes!",i);
	m_logFile<< strMsg<<endl;
	return true;
}
bool MicroSimulatorInterface::CreateMovement(void)
{
	std::list<MNode*>::iterator iNode;
	MNode* pNode;
	int num=0;

	m_logFile<<"CreateMovement function called!"<<endl;
	for (iNode = m_NodeList.begin(); iNode != m_NodeList.end(); iNode++,num++)
	{
		pNode = (*iNode);
		int nSeq = 0;
		for(int i=0;i<pNode->inLinks.size();i++)
		{
			MLink* pInLink = pNode->inLinks[i];
			for(int j=0;j<pNode->outLinks.size();j++)
			{
				MLink* pOutLink = pNode->outLinks[j];
				if ( pInLink->m_ReverseLinkID == pOutLink->m_LinkID ) continue; // no u-turn
				MMovement * pm = new MMovement();
				nSeq ++;
				pm->nSeqNo = nSeq;
				pm->bForbid = false;
				pm->nFromLinkId = pInLink->m_LinkID;
				pm->nToLinkId   = pOutLink->m_LinkID;
				pm->nSCNO = 0;
				pm->nSGNO = 0;
				GDPoint p1,p2,p3;
				p1 = GetMNodebyID(pInLink->m_FromNodeID)->pt;
				p2 = pNode->pt;
				p3 = GetMNodebyID(pOutLink->m_ToNodeID)->pt;
				DTA_Turn turn = m_pDoc->Find_PPP_to_Turn(p1,p2,p3);
				if ( turn == DTA_RightTurn )
					pm->nRTL = 1;
				else if ( turn == DTA_Through )
					pm->nRTL = 2;
				else if ( turn == DTA_LeftTurn )
					pm->nRTL = 3;
				else
					pm->nRTL = 0;
				pNode->Movements.push_back(pm);
			}
		}
	}
	CString strMsg;
	strMsg.Format("CreateMovement function ended with %d nodes!",num);
	m_logFile<< strMsg<<endl;
	return true;
}
void MicroSimulatorInterface::CreateSignal(void)
{
	std::list<MNode*>::iterator iNode;
	MNode* pNode;

	int nCrossing=9133,nT=3333;

	for(int i=0;i<m_SCs.size();i++)
	{
		if ( 4 == m_SCs[i]->nType )
		{
			nCrossing = m_SCs[i]->nID;
			break;
		}
	}
	for(int i=0;i<m_SCs.size();i++)
	{
		if ( 3 == m_SCs[i]->nType )
		{
			nT = m_SCs[i]->nID;
			break;
		}
	}


	for (iNode = m_NodeList.begin(); iNode != m_NodeList.end(); iNode++)
	{
		pNode = (*iNode);
		if ( 1 != pNode->nControlType ) continue;
		if ( 4 == pNode->m_nProcessType ) //crossing
		{
			pNode->nSCNO = nCrossing;
			// 把4个inLinks排序，然后组合！。。。。
			int nPhaseNo[8] = {1,6,7,4,5,2,3,8};
			int nGroupNo[8] = {3,1,4,2,3,1,4,2};
			int nMovementNum = pNode->Movements.size();
			for(int i=0;i<nMovementNum;i++)
			{
				for(int j=0;j<pNode->inLinks.size();j++)
				{
					if ( pNode->Movements[i]->nFromLinkId == pNode->inLinks[j]->m_LinkID )
					{
						if ( pNode->Movements[i]->nRTL == 1 || pNode->Movements[i]->nRTL == 2 ) //right and through
						{
							pNode->Movements[i]->nPhaseNO = nPhaseNo[j*2];
							pNode->Movements[i]->nSGNO    = nGroupNo[j*2];
						}
						else
						{
							pNode->Movements[i]->nPhaseNO = nPhaseNo[j*2+1];
							pNode->Movements[i]->nSGNO    = nGroupNo[j*2+1];
						}
						pNode->Movements[i]->nSCNO	  = pNode->nSCNO;
					}
				}
			}				
		}
		if ( 3 == pNode->m_nProcessType )// brunch,判断brunch在左还是在右
		{
			pNode->nSCNO = nT;
			//定义标准的丁字型，确定三个link再写SG
			MLink* pPre = pNode->GetBrunchLink(7);
			MLink* pBrn = pNode->GetBrunchLink(8);
			MLink* pPst = pNode->GetBrunchLink(9);

			for(int i=0;i<pNode->Movements.size();i++)
			{
				MMovement * pm= pNode->Movements[i];
				pm->nSCNO = pNode->nSCNO;

				if ( pm->nFromLinkId == pPre->m_LinkID )
				{
					pm->nSGNO = 1;
				}
				else if ( pm->nFromLinkId == pBrn->m_LinkID )
				{
					pm->nSGNO = 2;
				}
				else if ( pm->nFromLinkId == pPst->m_LinkID )
				{
					if ( 2 == pm->nRTL )
						pm->nSGNO = 1;
					else
						pm->nSGNO = 3;
				}
			}
		}
	}
}
bool MicroSimulatorInterface::CreateDefaultLanes(void)
{
	// create by default leftturn pocket for standard crossing and T junction
	std::list<MNode*>::iterator iNode;
	MNode* pNode;
	std::vector<MLink*>::iterator iLink;
	MLink* pLink;
	MLane* pLane;
	int i=0;
	// create lanes
	// 根据inlink和outlink中laneNumber生成lane：默认给标准十字路口和T路口生成leftTurn Pocket，边界点、不规则点和其他点只生成lane，无额外的pocket
	// 标准十字和T字，lane均给出方向；其他node的lane，只有直行属性
	for (iNode = m_NodeList.begin(); iNode != m_NodeList.end(); iNode++)
	{
		pNode = (*iNode);
		//if (pNode->m_NodeNo == 27 || pNode->m_NodeNo == 38)
		//{
		//	int abc;
		//	abc = 1 * 200;
		//}
		if ( 4 == pNode->m_nProcessType) // ltp at all inLinks
		{
			for(iLink=pNode->inLinks.begin();iLink!=pNode->inLinks.end();iLink++)
			{
				pLink = (*iLink);				 
				for(i=1;i<=pLink->m_NumberOfLanes;i++)
				{
					pLane = new MLane();
					pLink->inLanes.push_back(pLane);
					pLane->m_Index = i;
					pLane->m_LinkID = pLink->m_LinkID;
					pLane->m_NodeNo = pNode->m_NodeNo;
					pLane->through = 1;
					pLane->m_PocketLength = 0.0;
					pLane->leftTurn = 0;
					pLane->rightTurn = (1==i)?1:0; // 第一条lane准右转
					m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] in, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
						<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;
				}
				pLane = new MLane(); // extra leftturn pocket
				pLink->inLanes.push_back(pLane);
				pLane->m_Index = i;
				pLane->m_LinkID = pLink->m_LinkID;
				pLane->m_NodeNo = pNode->m_NodeNo;
				pLane->through = 0;
				pLane->m_PocketLength = 15.0;
				pLane->leftTurn = 1;
				pLane->rightTurn = 0; 
				m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] in, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
					<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;
			}
			for( iLink = pNode->outLinks.begin(); iLink != pNode->outLinks.end(); iLink++)// out lane
			{
				pLink = (*iLink);				 
				for(int i=1;i<=pLink->m_NumberOfLanes;i++)
				{
					pLane = new MLane();
					pLink->outLanes.push_back(pLane);
					pLane->m_Index = i;
					pLane->m_LinkID = pLink->m_LinkID;
					pLane->m_NodeNo = pNode->m_NodeNo;
					pLane->through = 1;         
					pLane->m_PocketLength = 0.0;
					pLane->leftTurn = 1;
					pLane->rightTurn = 1; 
					m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] out, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
						<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;
				}
			}
		}
		else if ( 3 == pNode->m_nProcessType) //ltp at two inLinks
		{
			int nIns=0,missingAppr=0;
			for(iLink=pNode->inLinks.begin();iLink!=pNode->inLinks.end();iLink++)
				nIns += (*iLink)->m_ToNodeApproach;
			missingAppr = 10 - nIns ; 
			// 找出6根link
			int branchAppr,leftAppr,rightAppr;
			GetNeighborApproach(missingAppr,2,&branchAppr);
			GetNeighborApproach(branchAppr,3,&leftAppr);   //RTL
			GetNeighborApproach(branchAppr,1,&rightAppr);

			MLink* piBLink = pNode->inLinkMap[branchAppr];
			MLink* poBLink = pNode->outLinkMap[branchAppr];

			MLink* piLLink = pNode->inLinkMap[leftAppr];
			MLink* poLLink = pNode->outLinkMap[leftAppr];

			MLink* piRLink = pNode->inLinkMap[rightAppr];
			MLink* poRLink = pNode->outLinkMap[rightAppr];

			// 产生所有的车道
			for(i=1;i<=piBLink->m_NumberOfLanes;i++) // branch in, all lanes right turn, extra left turn pocket
			{
				pLane = new MLane();
				piBLink->inLanes.push_back(pLane);
				pLane->m_Index = i;
				pLane->m_LinkID = piBLink->m_LinkID;
				pLane->m_NodeNo = pNode->m_NodeNo;
				pLane->through = 0;
				pLane->m_PocketLength = 0.0;
				pLane->leftTurn = 0;
				pLane->rightTurn =1; // 第一条lane准右转
				m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] in, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
					<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;
			}
			pLane = new MLane(); // extra leftturn pocket
			piBLink->inLanes.push_back(pLane);
			pLane->m_Index = i;
			pLane->m_LinkID = piBLink->m_LinkID;
			pLane->m_NodeNo = pNode->m_NodeNo;
			pLane->through = 0;
			pLane->m_PocketLength = 15.0;
			pLane->leftTurn = 1;
			pLane->rightTurn = 0; 
			m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] in, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
				<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;
			for(i=1;i<=poBLink->m_NumberOfLanes;i++)// branch out
			{
				pLane = new MLane();
				poBLink->outLanes.push_back(pLane);
				pLane->m_Index = i;
				pLane->m_LinkID = poBLink->m_LinkID;
				pLane->m_NodeNo = pNode->m_NodeNo;
				pLane->through = 1;         
				pLane->m_PocketLength = 0.0;
				pLane->leftTurn = 1;
				pLane->rightTurn = 1; 
				m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] out, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
					<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;
			}

			for(i=1;i<=piRLink->m_NumberOfLanes;i++) // right in, all lanes through, extra left turn pocket
			{
				pLane = new MLane();
				piRLink->inLanes.push_back(pLane);
				pLane->m_Index = i;
				pLane->m_LinkID = piRLink->m_LinkID;
				pLane->m_NodeNo = pNode->m_NodeNo;
				pLane->through = 1;
				pLane->m_PocketLength = 0.0;
				pLane->leftTurn = 0;
				pLane->rightTurn =0; 
				m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] in, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
					<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;
			}
			pLane = new MLane(); // extra leftturn pocket
			piRLink->inLanes.push_back(pLane);
			pLane->m_Index = i;
			pLane->m_LinkID = piRLink->m_LinkID;
			pLane->m_NodeNo = pNode->m_NodeNo;
			pLane->through = 0;
			pLane->m_PocketLength = 15.0;
			pLane->leftTurn = 1;
			pLane->rightTurn = 0; 
			m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] in, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
				<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;

			for(i=1;i<=poRLink->m_NumberOfLanes;i++)// right out
			{
				pLane = new MLane();
				poRLink->outLanes.push_back(pLane);
				pLane->m_Index = i;
				pLane->m_LinkID = poRLink->m_LinkID;
				pLane->m_NodeNo = pNode->m_NodeNo;
				pLane->through = 1;         
				pLane->m_PocketLength = 0.0;
				pLane->leftTurn = 1;
				pLane->rightTurn = 1; 
				m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] out, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
					<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;
			}	

			for(i=1;i<=piLLink->m_NumberOfLanes;i++) // left in, all lanes through, first lane share right turn
			{
				pLane = new MLane();
				piLLink->inLanes.push_back(pLane);
				pLane->m_Index = i;
				pLane->m_LinkID = piLLink->m_LinkID;
				pLane->m_NodeNo = pNode->m_NodeNo;
				pLane->through = 1;
				pLane->m_PocketLength = 0.0;
				pLane->leftTurn = 0;
				pLane->rightTurn =(1==i)?1:0; 
				m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] in, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
					<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;
			}
			for(i=1;i<=poLLink->m_NumberOfLanes;i++)// right out
			{
				pLane = new MLane();
				poLLink->outLanes.push_back(pLane);
				pLane->m_Index = i;
				pLane->m_LinkID = poLLink->m_LinkID;
				pLane->m_NodeNo = pNode->m_NodeNo;
				pLane->through = 1;         
				pLane->m_PocketLength = 0.0;
				pLane->leftTurn = 1;
				pLane->rightTurn = 1; 
				m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] out, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
					<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;
			}	
		}
		else
		{
			for(iLink=pNode->inLinks.begin();iLink!=pNode->inLinks.end();iLink++)
			{
				pLink = (*iLink);
				for(i=1;i<=pLink->m_NumberOfLanes;i++)
				{
					pLane = new MLane();
					pLink->inLanes.push_back(pLane);
					pLane->m_Index = i;
					pLane->m_LinkID = pLink->m_LinkID;
					pLane->m_NodeNo = pNode->m_NodeNo;
					pLane->through = 1;
					pLane->m_PocketLength = 0.0;
					pLane->leftTurn = (i==pLink->m_NumberOfLanes)?1:0; //最后一条准左转
					pLane->rightTurn = (1==i)?1:0; // 第一条lane准右转
					m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] out, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
						<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;
				}
			}
			for(iLink=pNode->outLinks.begin();iLink!=pNode->outLinks.end();iLink++)
			{
				pLink = (*iLink);
				for(i=1;i<=pLink->m_NumberOfLanes;i++)
				{
					pLane = new MLane();
					pLink->outLanes.push_back(pLane);
					pLane->m_Index = i;
					pLane->m_LinkID = pLink->m_LinkID;
					pLane->m_NodeNo = pNode->m_NodeNo;
					pLane->through = 0;
					pLane->m_PocketLength = 0.0;
					pLane->leftTurn = 0; 
					pLane->rightTurn = 0; 
					m_logFile<<"Lane created: NodeNumber["<<m_NodeNotoNumberMap[pLane->m_NodeNo]<<"] out, linkID: ["<<pLane->m_LinkID<<"] index: "<<pLane->m_Index
						<<" RTL ["<<pLane->rightTurn<<","<<pLane->through<<","<<pLane->leftTurn<<"] pocket: "<<pLane->m_PocketLength<<endl;
				}
			}
		}
	}

	return true;
}
bool MicroSimulatorInterface::ProcessLanes(void)
{
	// 生成所有laneTurn，标准十字路口和T路口同时生成信号
	// inLinks的循环；outLinks的循环；根据方向判断RTL、如为u turn则continue; new一个turn；同时判断phase，简易和NEMA
	// 联接node和边界node的判断和处理：边界点至多只有一in一out、且互为reverse，不生成turn；联接node两in两out、或一in一out但不为reverse，生成turn但不生成信号。此二种情况均不考虑方向
	std::list<MNode*>::iterator iNode;
	MNode* pNode;
	std::vector<MLink*>::iterator iLink;
	MLink* pLink;
	int i;
	int nInCount,nOutCount,nTurnCount,nLeftTurnInIndex,nOffsetIndexIn,nOffsetIndexOut;
	int branchAppr=0;
	int leftAppr=0;
	int rightAppr=0;


	for (iNode = m_NodeList.begin(); iNode != m_NodeList.end(); iNode++)
	{
		pNode = (*iNode);
		if (pNode->m_nProcessType == 4)
		{

			//根据车道的转向属性和方位，产生laneturn，并给出默认的信号控制
			MLink *piLink;
			MLink *poRLink,*poTLink,*poLLink;
			int i;

			for(int appr=1;appr <=4;appr++)
			{
				piLink = pNode->inLinkMap[appr];
				int nRt=0;
				GetNeighborApproach(appr,1,&nRt);
				poRLink = pNode->outLinkMap[nRt];
				GetNeighborApproach(appr,2,&nRt);
				poTLink = pNode->outLinkMap[nRt];
				GetNeighborApproach(appr,3,&nRt);
				poLLink = pNode->outLinkMap[nRt];
				// 右转，与西出link的车道同号联接，直至不能配对（下同！）；
				int nInCount = piLink->GetLaneCount(1,1);
				int nOutCount= poRLink->GetLaneCount(0);
				int nTurnCount = min(nInCount,nOutCount);
				for(i=1;i<=nTurnCount;i++)
				{
					MLaneTurn *pLaneTurn = new MLaneTurn();
					pNode->LaneTurns.push_back(pLaneTurn);

					pLaneTurn->nFromLinkId = piLink->m_LinkID;
					pLaneTurn->nFromIndex  = i;
					pLaneTurn->nToLinkId   = poRLink->m_LinkID;
					pLaneTurn->nToIndex	   = i;
					pLaneTurn->nRTL		   = 1;
					pLaneTurn->nSCNO       = GetSCNO(pNode->m_NodeNumber);
					pLaneTurn->nSignalGroupNo = GetSGNO(pNode->m_NodeNumber,appr,1);
				}
				// 直行，与南出link的车道同号或错号联接
				// 入直行车道大于出车道，则从入的2号开始联接，即把入1作为Right turnonly；
				//           等于      ，同号联接
				//           小于      ，则从出的2好开始联接，即把出1作为right turnonly。
				nInCount = piLink->GetLaneCount(1,2);
				nOutCount= poTLink->GetLaneCount(0);
				int nOffsetIndexIn,nOffsetIndexOut;
				if ( nInCount > nOutCount)
				{
					nTurnCount = nOutCount;
					nOffsetIndexIn = 1;
					nOffsetIndexOut= 0;
				}
				else if ( nInCount == nOutCount)
				{
					nTurnCount = nOutCount;
					nOffsetIndexIn = 0;
					nOffsetIndexOut= 0;
				}
				else // nInCount < nOutCount
				{
					nTurnCount = nInCount;
					nOffsetIndexIn = 0;
					nOffsetIndexOut= 1;
				}
				for(i=1;i<=nTurnCount;i++)
				{
					MLaneTurn *pLaneTurn = new MLaneTurn();
					pNode->LaneTurns.push_back(pLaneTurn);

					pLaneTurn->nFromLinkId = piLink->m_LinkID;
					pLaneTurn->nFromIndex  = i + nOffsetIndexIn;
					pLaneTurn->nToLinkId   = poTLink->m_LinkID;
					pLaneTurn->nToIndex	   = i + nOffsetIndexOut;
					pLaneTurn->nRTL        = 2;
					pLaneTurn->nSCNO       = GetSCNO(pNode->m_NodeNumber);
					pLaneTurn->nSignalGroupNo = GetSGNO(pNode->m_NodeNumber,appr,2);
				}
				// 左转，与东出link的车道从大号开始对应联接
				nInCount = piLink->GetLaneCount(1,3);
				nOutCount= poLLink->GetLaneCount(0);
				nTurnCount = min(nInCount,nOutCount);
				int nLeftTurnInIndex = piLink->GetLaneCount(1,0);
				for(i=0;i<nTurnCount;i++)
				{
					MLaneTurn *pLaneTurn = new MLaneTurn();
					pNode->LaneTurns.push_back(pLaneTurn);

					pLaneTurn->nFromLinkId = piLink->m_LinkID;
					pLaneTurn->nFromIndex  = nLeftTurnInIndex - i;
					pLaneTurn->nToLinkId   = poLLink->m_LinkID;
					pLaneTurn->nToIndex	   = nOutCount - i;
					pLaneTurn->nRTL		   = 3;
					pLaneTurn->nSCNO       = GetSCNO(pNode->m_NodeNumber);
					pLaneTurn->nSignalGroupNo = GetSGNO(pNode->m_NodeNumber,appr,3);
				}
			}

		}
		else if (pNode->m_nProcessType == 3)
		{
			// T type 关键是确定出主干道和唯一的一个branch。缺少哪个方向，对应的方向就是branch
			// 3 个方向加起来的和，应相等，同时也知道缺少哪个方向了

			int  nTotal=0,missingAppr;
			for(iLink = pNode->inLinks.begin();iLink!=pNode->inLinks.end();iLink++)
				nTotal += (*iLink)->m_ToNodeApproach;
			missingAppr = 10 - nTotal;

			// 找出6根link
			GetNeighborApproach(missingAppr,2,&branchAppr);
			GetNeighborApproach(branchAppr,3,&leftAppr);   //RTL
			GetNeighborApproach(branchAppr,1,&rightAppr);

			MLink* piBLink = pNode->inLinkMap[branchAppr];
			MLink* poBLink = pNode->outLinkMap[branchAppr];

			MLink* piLLink = pNode->inLinkMap[leftAppr];
			MLink* poLLink = pNode->outLinkMap[leftAppr];

			MLink* piRLink = pNode->inLinkMap[rightAppr];
			MLink* poRLink = pNode->outLinkMap[rightAppr];	

			// 产生laneTurns
			// ① branch in, to right out and to left out
			nInCount = piBLink->GetLaneCount(1,1);
			nOutCount= poRLink->GetLaneCount(0);
			nTurnCount = min(nInCount,nOutCount);
			int appr = piBLink->m_ToNodeApproach;
			for(i=1;i<=nTurnCount;i++)
			{
				MLaneTurn *pLaneTurn = new MLaneTurn();
				pNode->LaneTurns.push_back(pLaneTurn);

				pLaneTurn->nFromLinkId = piBLink->m_LinkID;
				pLaneTurn->nFromIndex  = i;
				pLaneTurn->nToLinkId   = poRLink->m_LinkID;
				pLaneTurn->nToIndex	   = i;
				pLaneTurn->nRTL		   = 1; // right turn
				if ( 1 == pNode->nControlType)
				{
					pLaneTurn->nSCNO       = GetSCNO(pNode->m_NodeNumber);
					pLaneTurn->nSignalGroupNo = GetSGNO(pNode->m_NodeNumber,appr,1);
				}
			}
			nInCount = piBLink->GetLaneCount(1,3);
			nOutCount= poLLink->GetLaneCount(0);
			nTurnCount = min(nInCount,nOutCount);
			nLeftTurnInIndex = piBLink->GetLaneCount(1,0);
			for(i=0;i<nTurnCount;i++)
			{
				MLaneTurn *pLaneTurn = new MLaneTurn();
				pNode->LaneTurns.push_back(pLaneTurn);

				pLaneTurn->nFromLinkId = piBLink->m_LinkID;
				pLaneTurn->nFromIndex  = nLeftTurnInIndex - i;
				pLaneTurn->nToLinkId   = poLLink->m_LinkID;
				pLaneTurn->nToIndex	   = nOutCount - i;
				pLaneTurn->nRTL		   = 3; //left turn
				if ( 1 == pNode->nControlType)
				{
					pLaneTurn->nSCNO       = GetSCNO(pNode->m_NodeNumber);
					pLaneTurn->nSignalGroupNo = GetSGNO(pNode->m_NodeNumber,appr,3);
				}
			}
			// ② right in, to branch(left turn) and left(through) out
			nInCount = piRLink->GetLaneCount(1,3);
			nOutCount= poBLink->GetLaneCount(0);
			nTurnCount = min(nInCount,nOutCount);
			nLeftTurnInIndex = piRLink->GetLaneCount(1,0);
			appr = piRLink->m_ToNodeApproach;
			for(i=0;i<nTurnCount;i++)
			{
				MLaneTurn *pLaneTurn = new MLaneTurn();
				pNode->LaneTurns.push_back(pLaneTurn);

				pLaneTurn->nFromLinkId = piRLink->m_LinkID;
				pLaneTurn->nFromIndex  = nLeftTurnInIndex - i;
				pLaneTurn->nToLinkId   = poBLink->m_LinkID;
				pLaneTurn->nToIndex	   = nOutCount - i;
				pLaneTurn->nRTL  = 3;
				if ( 1 == pNode->nControlType)
				{
					pLaneTurn->nSCNO       = GetSCNO(pNode->m_NodeNumber);
					pLaneTurn->nSignalGroupNo = GetSGNO(pNode->m_NodeNumber,appr,3);
				}
			}
			nInCount = piRLink->GetLaneCount(1,2);
			nOutCount= poLLink->GetLaneCount(0);
			nOffsetIndexIn,nOffsetIndexOut;
			if ( nInCount > nOutCount)
			{
				nTurnCount = nOutCount;
				nOffsetIndexIn = 1;
				nOffsetIndexOut= 0;
			}
			else if ( nInCount == nOutCount)
			{
				nTurnCount = nOutCount;
				nOffsetIndexIn = 0;
				nOffsetIndexOut= 0;
			}
			else // nInCount < nOutCount
			{
				nTurnCount = nInCount;
				nOffsetIndexIn = 0;
				nOffsetIndexOut= 1;
			}
			for(i=1;i<=nTurnCount;i++)
			{
				MLaneTurn *pLaneTurn = new MLaneTurn();
				pNode->LaneTurns.push_back(pLaneTurn);

				pLaneTurn->nFromLinkId = piRLink->m_LinkID;
				pLaneTurn->nFromIndex  = i + nOffsetIndexIn;
				pLaneTurn->nToLinkId   = poLLink->m_LinkID;
				pLaneTurn->nToIndex	   = i + nOffsetIndexOut;
				pLaneTurn->nRTL = 2; //through
				if ( 1 == pNode->nControlType)
				{
					pLaneTurn->nSCNO       = GetSCNO(pNode->m_NodeNumber);
					pLaneTurn->nSignalGroupNo = GetSGNO(pNode->m_NodeNumber,appr,2);
				}
			}
			// ③ left in, to branch(right turn) and right(through) out
			nInCount = piLLink->GetLaneCount(1,1);
			nOutCount= poBLink->GetLaneCount(0);
			nTurnCount = min(nInCount,nOutCount);
			appr = piLLink->m_ToNodeApproach;
			for(i=1;i<=nTurnCount;i++)
			{
				MLaneTurn *pLaneTurn = new MLaneTurn();
				pNode->LaneTurns.push_back(pLaneTurn);

				pLaneTurn->nFromLinkId = piLLink->m_LinkID;
				pLaneTurn->nFromIndex  = i;
				pLaneTurn->nToLinkId   = poBLink->m_LinkID;
				pLaneTurn->nToIndex	   = i;
				pLaneTurn->nRTL  = 1;
				if ( 1 == pNode->nControlType)
				{
					pLaneTurn->nSCNO       = GetSCNO(pNode->m_NodeNumber);
					pLaneTurn->nSignalGroupNo = GetSGNO(pNode->m_NodeNumber,appr,1);
				}
			}
			nInCount = piLLink->GetLaneCount(1,2);
			nOutCount= poRLink->GetLaneCount(0);
			nOffsetIndexIn,nOffsetIndexOut;
			if ( nInCount > nOutCount)
			{
				nTurnCount = nOutCount;
				nOffsetIndexIn = 1;
				nOffsetIndexOut= 0;
			}
			else if ( nInCount == nOutCount)
			{
				nTurnCount = nOutCount;
				nOffsetIndexIn = 0;
				nOffsetIndexOut= 0;
			}
			else // nInCount < nOutCount
			{
				nTurnCount = nInCount;
				nOffsetIndexIn = 0;
				nOffsetIndexOut= 1;
			}
			for(i=1;i<=nTurnCount;i++)
			{
				MLaneTurn *pLaneTurn = new MLaneTurn();
				pNode->LaneTurns.push_back(pLaneTurn);

				pLaneTurn->nFromLinkId = piLLink->m_LinkID;
				pLaneTurn->nFromIndex  = i + nOffsetIndexIn;
				pLaneTurn->nToLinkId   = poRLink->m_LinkID;
				pLaneTurn->nToIndex	   = i + nOffsetIndexOut;
				pLaneTurn->nRTL = 2;
				if ( 1 == pNode->nControlType)
				{
					pLaneTurn->nSCNO       = GetSCNO(pNode->m_NodeNumber);
					pLaneTurn->nSignalGroupNo = GetSGNO(pNode->m_NodeNumber,appr,2);
				}
			}

		}
		else if (pNode->m_nProcessType == 2)
		{
			for(iLink=pNode->inLinks.begin();iLink!=pNode->inLinks.end();iLink++)
			{
				pLink = (*iLink);
				std::vector<MLink*>::iterator ioLink;
				for(ioLink=pNode->outLinks.begin();ioLink!=pNode->outLinks.end();ioLink++)
				{
					MLink *poLink = (*ioLink);
					if (pLink->m_ReverseLinkID == poLink->m_LinkID )
						continue;
					int nTurnCount = min(pLink->m_NumberOfLanes,poLink->m_NumberOfLanes);
					for(i=1;i<=nTurnCount;i++)
					{
						MLaneTurn* pLaneTurn = new MLaneTurn();
						pNode->LaneTurns.push_back(pLaneTurn);
						pLaneTurn->nFromLinkId = pLink->m_LinkID;
						pLaneTurn->nFromIndex  = i;
						pLaneTurn->nToLinkId   = poLink->m_LinkID;
						pLaneTurn->nToIndex    = i;
						pLaneTurn->nRTL		   = 2;  //connector is considered as through
						pLaneTurn->nSignalGroupNo = 0;
						pLaneTurn->nSCNO          = 0;
					}
				}
			}
		}
		else if (pNode->m_nProcessType == 1 || pNode->m_nProcessType == 0)
		{
			// do nothing for the border node
		}
		else
		{
			for(iLink=pNode->inLinks.begin();iLink!=pNode->inLinks.end();iLink++)
			{
				pLink = (*iLink);
				std::vector<MLink*>::iterator ioLink;
				for(ioLink=pNode->outLinks.begin();ioLink!=pNode->outLinks.end();ioLink++)
				{
					MLink *poLink = (*ioLink);
					if (pLink->m_ReverseLinkID == poLink->m_LinkID )
						continue;
					int nTurnCount = min(pLink->m_NumberOfLanes,poLink->m_NumberOfLanes);
					for(i=1;i<=nTurnCount;i++)
					{
						MLaneTurn* pLaneTurn = new MLaneTurn();
						pNode->LaneTurns.push_back(pLaneTurn);
						pLaneTurn->nFromLinkId = pLink->m_LinkID;
						pLaneTurn->nFromIndex  = i;
						pLaneTurn->nToLinkId   = poLink->m_LinkID;
						pLaneTurn->nToIndex    = i;
						pLaneTurn->nRTL		   = 0;  // can not judged
						pLaneTurn->nSignalGroupNo = 0;
						pLaneTurn->nSCNO          = 0;
					}
				}
			}
		}
		// dump all the laneturns
		m_logFile<<endl;
		m_logFile<<"NodeNumber: "<<pNode->m_NodeNumber<<" LaneTurns: ["<<pNode->LaneTurns.size()<<"]"<<endl;
		std::vector<MLaneTurn*>::iterator iTurn;
		for(iTurn=pNode->LaneTurns.begin();iTurn!=pNode->LaneTurns.end();iTurn++)
		{
			m_logFile<<"RTL="<<(*iTurn)->nRTL<<" FromLink "<<(*iTurn)->nFromLinkId<<" Lane "<<(*iTurn)->nFromIndex
				<<" ToLink "<<(*iTurn)->nToLinkId<<" Lane "<<(*iTurn)->nToIndex
				<<"SC ["<<(*iTurn)->nSCNO<<"] SG["<<(*iTurn)->nSignalGroupNo<<"]"<<endl;
		}
		m_logFile<<endl;

	} // end of for node list
	return true;
}
bool MicroSimulatorInterface::CreateANMFile2(std::string strFileName)
{
	CString fromTime,toTime;

	fromTime.Format("%s","09:00:00");
	toTime.Format("%s","12:00:00");

	std::vector<std::string> anmNames;

	anmNames.push_back ("VEHTYPES");   //0
	anmNames.push_back ("VEHCLASSES"); //1
	anmNames.push_back ("NODES");	   //2
	anmNames.push_back ("ZONES");	   //3
	anmNames.push_back ("LINKTYPES");  //4
	anmNames.push_back ("LINKS");      //5
	anmNames.push_back ("PTSTOPS");    //6
	anmNames.push_back ("PTLINES");    //7
	anmNames.push_back ("SIGNALCONTROLS");//8

	m_rf.open(strFileName.c_str());
	m_rf<<"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
	m_rf<<"<ABSTRACTNETWORKMODEL VERSNO=\"1.0\" FROMTIME=\""<<fromTime<<"\" TOTIME=\""<<toTime<<"\" NAME=\"simulation.anm\">\n";
	m_rf<<"<NETWORK LEFTHANDTRAFFIC = \"0\">\n";
	for(int i=0;i<anmNames.size();i++)
	{
		m_rf<<"\t<"<<anmNames[i].c_str()<<">\n";
		if ( 0 == i) WriteVehTypes();
		if ( 1 == i) WriteVehClasses();
		if ( 2 == i) WriteNodes();
		if ( 3 == i) WriteZones();
		if ( 4 == i) WriteLinkTypes();
		if ( 5 == i) WriteLinks();
		if ( 6 == i) WritePTStops();
		if ( 7 == i) WritePTLines();
		if ( 8 == i) WriteSignalControls();
		m_rf<<"\t</"<<anmNames[i].c_str()<<">\n";
	}
	m_rf<<"</NETWORK>\n";
	m_rf<<"</ABSTRACTNETWORKMODEL>\n";
	m_rf.close();
	return true;
}
bool MicroSimulatorInterface::WriteVehTypes()
{	
	CString strVTCSV = m_strFolder + _T("ms_vehtypes.csv");
	std::string strFileName;
	strFileName = T2A(strVTCSV.GetBuffer());

	CCSVParser parser;
	if (parser.OpenCSVFile(strFileName))
	{
		int i=0;
		while(parser.ReadRecord())
		{
			int vehtype_no;
			string name;
			string vehcategory;

			if(!parser.GetValueByFieldName("vehtype_no",vehtype_no))
				vehtype_no = 100;

			if(!parser.GetValueByFieldName("name",name))
				name = "AllPurp";

			if(!parser.GetValueByFieldName("vehcategory",vehcategory))
				vehcategory = "Car";

			CString strLine;
			strLine.Format("\t\t<VEHTYPE NO = \"%d\"  NAME = \"%s\" VEHCATEGORY = \"%s\" />\n",vehtype_no,name.c_str(),vehcategory.c_str());
			m_rf<<strLine;
		}
		return true;
	}
	else
	{
		AfxMessageBox("Error: File ms_vehtypes.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}
}
bool MicroSimulatorInterface::WriteVehClasses()
{	
	CString strVCCSV = m_strFolder + _T("ms_vehclasses.csv");
	std::string strFileName;
	strFileName = T2A(strVCCSV.GetBuffer());

	CCSVParser parser;
	if (parser.OpenCSVFile(strFileName))
	{
		int i=0;
		CString strLine;
		string vc_id_old = "null";
		while(parser.ReadRecord())
		{
			string vehclass_id;
			int vehtypeid_no;
			std::vector<int> idvector;


			if(!parser.GetValueByFieldName("vehclass_id",vehclass_id))
				vehclass_id = "PtDefault";

			if(!parser.GetValueByFieldName("vehtypeid_no",vehtypeid_no))
				vehtypeid_no = 1;

			if ( vc_id_old != vehclass_id ) // new class
			{
				vc_id_old = vehclass_id;
				if ( 0 != i )
				{
					strLine.Format("\t\t</VEHCLASS>\n");
					m_rf<<strLine;
				}
				strLine.Format("\t\t<VEHCLASS ID = \"%s\">\n",vehclass_id.c_str());
				m_rf<<strLine;
				i ++;
			}

			strLine.Format("\t\t\t<VEHTYPEID NO = \"%d\"/>\n",vehtypeid_no);
			m_rf<<strLine;
		}
		strLine.Format("\t\t</VEHCLASS>\n");
		m_rf<<strLine;
		return true;
	}
	else
	{
		AfxMessageBox("Error: File ms_vehtypes.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}
}
bool MicroSimulatorInterface::WriteNodes()
{
	std::list<MNode*>::iterator iMNode;
	CString strLine;
	for(iMNode = m_NodeList.begin(); iMNode != m_NodeList.end(); iMNode++)
	{
		MNode *pMNode = (*iMNode);
		CString szTemp;
		switch (pMNode->nControlType)
		{
		case 1:
			szTemp = _T("Signalized");
			break;
		case 2:
			szTemp = _T("TwoWayStop");
			break;
		default:
			szTemp = _T("Unknown");
			break;
		}
		strLine.Format("\t\t\t<NODE NO=\"%d\" NAME=\"%s\" XCOORD=\"%.4f\" YCOORD=\"%.4f\" CONTROLTYPE=\"%s\">\n",
			pMNode->m_NodeNumber,pMNode->m_Name.c_str(),pMNode->pt.x,pMNode->pt.y,szTemp);
		m_rf<<strLine;

		strLine.Format("\t\t\t\t<LANES>\n");
		m_rf<<strLine;

		std::vector<MLink*>::iterator iMLink;
		std::vector<MLane*>::iterator iMLane;
		double default_width = 3.5f; // meters

		// in lanes in in links
		for(iMLink=pMNode->inLinks.begin();iMLink!=pMNode->inLinks.end();iMLink++)
		{
			MLink* pMLink = (*iMLink);
			for(iMLane=pMLink->inLanes.begin();iMLane!=pMLink->inLanes.end();iMLane++)
			{
				MLane* pLane = (*iMLane);
				if ( pLane->m_PocketLength == 0 ) szTemp = _T("false");
				else	szTemp = _T("true");
				strLine.Format("\t\t\t\t\t<LANE LINKID=\"%d\" INDEX=\"%d\" POCKET=\"%s\" POCKETLENGTH=\"%.4f\" WIDTH=\"%.4f\" />\n",
					pLane->m_LinkID,pLane->m_Index,szTemp,pLane->m_PocketLength,default_width);
				m_rf<<strLine;
			}
		}
		// out lanes in out links
		for(iMLink=pMNode->outLinks.begin();iMLink!=pMNode->outLinks.end();iMLink++)
		{
			MLink* pMLink = (*iMLink);
			for(iMLane=pMLink->outLanes.begin();iMLane!=pMLink->outLanes.end();iMLane++)
			{
				MLane* pLane = (*iMLane);
				if ( pLane->m_PocketLength == 0 ) szTemp = _T("false");
				else	szTemp = _T("true");
				strLine.Format("\t\t\t\t\t<LANE LINKID=\"%d\" INDEX=\"%d\" POCKET=\"%s\" POCKETLENGTH=\"%.4f\" WIDTH=\"%.4f\" />\n",
					pLane->m_LinkID,pLane->m_Index,szTemp,pLane->m_PocketLength,default_width);
				m_rf<<strLine;
			}
		}

		strLine.Format("\t\t\t\t</LANES>\n");
		m_rf<<strLine;

		strLine.Format("\t\t\t\t<LANETURNS>\n");
		m_rf<<strLine;

		std::vector<MLaneTurn*>::iterator iTurn;
		for(iTurn = pMNode->LaneTurns.begin();iTurn!=pMNode->LaneTurns.end();iTurn++)
		{
			MLaneTurn* pTurn = (*iTurn);
			if (1 == pMNode->nControlType)
			{
				strLine.Format("\t\t\t\t\t<LANETURN FROMLINKID=\"%d\" FROMLANEINDEX=\"%d\" TOLINKID=\"%d\" TOLANEINDEX=\"%d\" SCNO=\"%d\" SGNO=\"%d\" />\n",
					GetMLinkbyID(pTurn->nFromLinkId)->m_LinkID ,pTurn->nFromIndex,GetMLinkbyID(pTurn->nToLinkId)->m_LinkID ,pTurn->nToIndex,pTurn->nSCNO,pTurn->nSignalGroupNo);
			}
			else
			{
				strLine.Format("\t\t\t\t\t<LANETURN FROMLINKID=\"%d\" FROMLANEINDEX=\"%d\" TOLINKID=\"%d\" TOLANEINDEX=\"%d\" />\n",
					GetMLinkbyID(pTurn->nFromLinkId)->m_LinkID,pTurn->nFromIndex,GetMLinkbyID(pTurn->nToLinkId)->m_LinkID ,pTurn->nToIndex);
			}
			m_rf<<strLine;
		}

		strLine.Format("\t\t\t\t</LANETURNS>\n");
		m_rf<<strLine;

		strLine.Format("\t\t\t\t<CROSSWALKS />\n");
		m_rf<<strLine;

		strLine.Format("\t\t\t</NODE>\n");
		m_rf<<strLine;
	}
	return true;
}
bool MicroSimulatorInterface::WriteZones()
{
	CString strLine;

	for(int i=0;i<m_ZoneVector.size();i++)
	{
		MZone* pZone = m_ZoneVector[i];
		strLine.Format("\t\t\t<ZONE NO=\"%d\" XCOORD=\"%.4f\" YCOORD=\"%.4f\">\n",pZone->m_nID,pZone->pt.x,pZone->pt.y);
		m_rf<<strLine;
		strLine.Format("\t\t\t\t<CONNECTORS>\n");
		m_rf<<strLine;
		for(int j=0;j<pZone->m_nodes.size();j++)
		{
			strLine.Format("\t\t\t\t\t<CONNECTOR NODENO=\"%d\" DIRECTION=\"false\" />\n",pZone->m_nodes[j]->m_NodeNumber);
			m_rf<<strLine;
			strLine.Format("\t\t\t\t\t<CONNECTOR NODENO=\"%d\" DIRECTION=\"true\" />\n",pZone->m_nodes[j]->m_NodeNumber);
			m_rf<<strLine;
		}
		strLine.Format("\t\t\t\t</CONNECTORS>\n");
		m_rf<<strLine;
		strLine.Format("\t\t\t</ZONE>\n");
		m_rf<<strLine;
	}
	return true;
}
bool MicroSimulatorInterface::WriteLinkTypes()
{
	CString strLTCSV = m_strFolder + _T("input_link_type.csv");
	std::string strFileName;
	strFileName = T2A(strLTCSV.GetBuffer());

	CCSVParser parser;
	if (parser.OpenCSVFile(strFileName))
	{
		int i=0;
		while(parser.ReadRecord())
		{
			int linktype_no;
			string name;
			string drivingbehavior;

			if(!parser.GetValueByFieldName("link_type",linktype_no))
				linktype_no = 0;

			//if(!parser.GetValueByFieldName("link_type_name",name))
			//	name = "Unknown Error";

			//std::replace( name.begin(), name.end(), ' ', '_'); 

			CString str_name;
			str_name.Format("linktype%d",  linktype_no);
			
			name = T2A(str_name.GetBuffer()); ;

			drivingbehavior = "Urban";

			CString strLine;
			strLine.Format("\t\t<LINKTYPE NO = \"%d\" NAME = \"%s\" DRIVINGBEHAVIOR = \"%s\" />\n",linktype_no,name.c_str(),drivingbehavior.c_str());
			m_rf<<strLine;
		}
		return true;
	}
	else
	{
		AfxMessageBox("Error: File ms_linktypes.csv cannot be opened.\nIt might be currently used and locked by EXCEL.");
		return false;
	}
}
bool MicroSimulatorInterface::WriteLinks()
{
	std::list<MLink*>::iterator iMLink;
	for(iMLink = m_LinkList.begin();iMLink!=m_LinkList.end();iMLink++)
	{
		MLink* pLink=(*iMLink);
		CString strLine;

	//	if(pLink->m_ReverseLinkID==0 || pLink->m_ReverseLinkID==pLink->m_LinkID) //Xuesong: ReserseLinkID is null
		{
			if ( pLink->m_ShapePoints.size() == 2 )
			{
				strLine.Format("\t\t\t<LINK ID=\"%d\" FROMNODENO=\"%d\" TONODENO=\"%d\" LINKTYPENO=\"%d\" SPEED=\"%.4f\" NUMLANES=\"%d\" />\n",
					pLink->m_LinkID ,pLink->m_FromNodeNumber,pLink->m_ToNodeNumber,pLink->m_LinkType,pLink->m_SpeedLimit,pLink->m_NumberOfLanes);
				m_rf<<strLine;
			}
			else
			{
				strLine.Format("\t\t\t<LINK ID=\"%d\" FROMNODENO=\"%d\" TONODENO=\"%d\" LINKTYPENO=\"%d\" SPEED=\"%.4f\" NUMLANES=\"%d\" >\n",
					pLink->m_LinkID ,pLink->m_FromNodeNumber,pLink->m_ToNodeNumber,pLink->m_LinkType,pLink->m_SpeedLimit,pLink->m_NumberOfLanes);
				m_rf<<strLine;
				strLine.Format("\t\t\t\t<LINKPOLY>\n");
				m_rf<<strLine;

				// Xuesong do write out duplicated points;

				GDPoint GDP_prev;
				GDP_prev.x = 0;
				GDP_prev.y = 0;

				for(int i=1 ; i< pLink->m_ShapePoints.size()-1 ; i++)
				{
					
					GDPoint GDP = pLink->m_ShapePoints[i];

					if(fabs(GDP_prev.x-GDP.x) + fabs(GDP_prev.y-GDP.y)>0.0001 )
					{
					strLine.Format("\t\t\t\t\t<POINT INDEX=\"%d\" XCOORD=\"%.4f\" YCOORD=\"%.4f\" />\n",i,GDP.x,GDP.y);
					}

					GDP_prev.x = GDP.x;
					GDP_prev.y = GDP.y;

					m_rf<<strLine;
				}
				strLine.Format("\t\t\t\t</LINKPOLY>\n");
				m_rf<<strLine;
				strLine.Format("\t\t\t</LINK>\n");
				m_rf<<strLine;
			}
		}
		//else
		//{
		//	if ( pLink->m_ShapePoints.size() == 2 )
		//	{
		//		strLine.Format("\t\t\t<LINK ID=\"%d\" FROMNODENO=\"%d\" TONODENO=\"%d\" LINKTYPENO=\"%d\" SPEED=\"%.4f\" NUMLANES=\"%d\" REVERSELINK=\"%d\" />\n",
		//			pLink->m_LinkID ,pLink->m_FromNodeNumber,pLink->m_ToNodeNumber,pLink->m_LinkType,pLink->m_SpeedLimit,pLink->m_NumberOfLanes,pLink->m_ReverseLinkID );
		//		m_rf<<strLine;
		//	}
		//	else
		//	{
		//		strLine.Format("\t\t\t<LINK ID=\"%d\" FROMNODENO=\"%d\" TONODENO=\"%d\" LINKTYPENO=\"%d\" SPEED=\"%.4f\" NUMLANES=\"%d\" REVERSELINK=\"%d\" >\n",
		//			pLink->m_LinkID ,pLink->m_FromNodeNumber,pLink->m_ToNodeNumber,pLink->m_LinkType,pLink->m_SpeedLimit,pLink->m_NumberOfLanes,pLink->m_ReverseLinkID);
		//		m_rf<<strLine;
		//		strLine.Format("\t\t\t\t<LINKPOLY>\n");
		//		m_rf<<strLine;
		//		for(int i=1 ; i< pLink->m_ShapePoints.size()-1 ; i++)
		//		{
		//			GDPoint GDP = pLink->m_ShapePoints[i];
		//			strLine.Format("\t\t\t\t\t<POINT INDEX=\"%d\" XCOORD=\"%.4f\" YCOORD=\"%.4f\" />\n",i,GDP.x,GDP.y);
		//			m_rf<<strLine;
		//		}
		//		strLine.Format("\t\t\t\t</LINKPOLY>\n");
		//		m_rf<<strLine;
		//		strLine.Format("\t\t\t</LINK>\n");
		//		m_rf<<strLine;
		//	}
		//}
	}
	return true;
}
bool MicroSimulatorInterface::WritePTStops()
{
	return true;
}
bool MicroSimulatorInterface::WritePTLines()
{
	return true;
}
bool MicroSimulatorInterface::WriteSignalControls()
{
	for(int i=0;i<m_SCs.size();i++)
	{
		MSigC* p = m_SCs[i];
		int		nSCNo	= p->nID;
		CString strName = p->strName.c_str();
		int	nCycleLength= p->nCycleLength;
		int nOffset		= p->nOffset;
		CString strCL	= Minutes2PTString(nCycleLength);
		CString strOS	= Minutes2PTString(nOffset);
		CString strSigalizationType = _T("FixedTime");
		CString strCycleTimeFixed = _T("true");
		CString strSigalGroupType = _T("Cycle");

		CString strLine;
		strLine.Format("\t\t<SIGNALCONTROL NO = \"%d\" NAME = \"%s\" CYCLETIME = \"%s\" TIMEOFFSET = \"%s\" SIGNALIZATIONTYPE = \"%s\" CYCLETIMEFIXED = \"%s\" >\n",nSCNo,strName,strCL,strOS,strSigalizationType,strCycleTimeFixed);
		m_rf<<strLine;
		strLine.Format("\t\t\t<SIGNALGROUPS>\n");
		m_rf<<strLine;
		for(int j=0;j<p->SGs.size();j++)
		{
			MSigG* pg = p->SGs[j];
			//write a group
			strLine.Format("\t\t\t\t<SIGNALGROUP NO=\"%d\" NAME=\"%s\" GTSTART=\"%s\" GTEND=\"%s\" MINGTIME=\"%s\" ATIME=\"%s\" SIGNALGROUPTYPE=\"%s\" />\n",
				pg->nSGNO,"",
				Minutes2PTString(pg->nGTStart),
				Minutes2PTString(pg->nGTEnd),
				Minutes2PTString(pg->nMinGT),
				Minutes2PTString(pg->nAT),
				strSigalGroupType);
			m_rf<<strLine;
		}

		strLine.Format("\t\t\t</SIGNALGROUPS>\n");
		m_rf<<strLine;
		strLine.Format("\t\t\t<INTERGREENS />\n");
		m_rf<<strLine;
		strLine.Format("\t\t</SIGNALCONTROL>\n");
		m_rf<<strLine;

	}
	return true;
}
bool MicroSimulatorInterface::WritePhases(std::vector<PhaseRecord*> phases)
{
	PhaseRecord *p = phases[0];
	int		nSCNo	= p->nSCNo;
	CString strName = p->strName;
	int	nCycleLength= p->nCycleLength;
	int nOffset		= p->nOffset;
	CString strCL	= Minutes2PTString(nCycleLength);
	CString strOS	= Minutes2PTString(nOffset);
	CString strSigalizationType = _T("FixedTime");
	CString strCycleTimeFixed = _T("true");
	CString strSigalGroupType = _T("Cycle");

	CString strLine;
	strLine.Format("\t\t<SIGNALCONTROL NO = \"%d\" NAME = \"%s\" CYCLETIME = \"%s\" TIMEOFFSET = \"%s\" SIGNALIZATIONTYPE = \"%s\" CYCLETIMEFIXED = \"%s\"\n",nSCNo,strName,strCL,strOS,strSigalizationType,strCycleTimeFixed);
	m_rf<<strLine;
	strLine.Format("\t\t\t<SIGNALGROUPS>\n");
	m_rf<<strLine;
	int nGroupIndex = 1;
	while (true)
	{
		int i;
		for(i=0;i<phases.size();i++)
		{
			p = phases[i];
			if ( nGroupIndex == p->nGroupNo)
				break;
		}
		if ( i >= phases.size() )
			break;
		//write a group
		strLine.Format("\t\t\t\t<SIGNALGROUP NO=\"%d\" NAME=\"%s\" GTSTART=\"%s\" GTEND=\"%s\" MINGTIME=\"%s\" ATIME=\"%s\" SIGNALGROUPTYPE=\"%s\" />\n",
			p->nGroupNo,p->strName,
			Minutes2PTString(p->nStartTime),
			Minutes2PTString(p->nStartTime+p->nGreenLength),
			Minutes2PTString(0),
			Minutes2PTString(0),
			strSigalGroupType);
		m_rf<<strLine;
		nGroupIndex ++;
	}

	strLine.Format("\t\t\t</SIGNALGROUPS>\n");
	m_rf<<strLine;
	strLine.Format("\t\t\t<INTERGREENS />\n");
	m_rf<<strLine;
	strLine.Format("\t\t</SIGNALCONTROL>\n");
	m_rf<<strLine;

	return true;
}
std::vector<string> MicroSimulatorInterface::GetSignalControls()
{// get those signal files
	_finddata_t file;
	long lf;
	std::vector<string> files;
	CString str;
	str.Format("%s\\sig*.csv",m_strFolder);
	if((lf = _findfirst(str, &file))==-1l)//_findfirst返回的是long型; long __cdecl _findfirst(const char *, struct _finddata_t *)
		AfxMessageBox("文件没有找到!",MB_OK);
	else
	{
		while( _findnext( lf, &file ) == 0 )//int __cdecl _findnext(long, struct _finddata_t *);如果找到下个文件的名字成功的话就返回0,否则返回-1
		{
			cout<<file.name;
			files.push_back(file.name);
		}
	}
	_findclose(lf);
	return files;
}
bool MicroSimulatorInterface::GetPhases(std::string sigFileName,std::vector<PhaseRecord*> phases)
{
	CString strCSV= m_strFolder + sigFileName.c_str();
	std::string strFileName;
	strFileName = T2A(strCSV.GetBuffer());
	bool rt = true;

	CCSVParser parser;
	if (parser.OpenCSVFile(strFileName))
	{
		int i=0;
		while(parser.ReadRecord())
		{
			int phase_no;
			string phase_name;
			int phase_type;
			int sequence_type;
			int group_no;
			int start_time;
			int green_length;
			int yellow_length;
			int red_length;
			int cycle_length;
			int offset;
			int sc_no;
			rt = true;

			if( rt ) rt = parser.GetValueByFieldName("phase_no",phase_no);
			if( rt ) rt = parser.GetValueByFieldName("phase_name",phase_name);
			if( rt ) rt = parser.GetValueByFieldName("phase_type",phase_type);
			if( rt ) rt = parser.GetValueByFieldName("sequence_type",sequence_type);
			if( rt ) rt = parser.GetValueByFieldName("group_no",group_no);
			if( rt ) rt = parser.GetValueByFieldName("start_time",start_time);
			if( rt ) rt = parser.GetValueByFieldName("green_length",green_length);
			if( rt ) rt = parser.GetValueByFieldName("yellow_length",yellow_length);
			if( rt ) rt = parser.GetValueByFieldName("red_length",red_length);
			if( rt ) rt = parser.GetValueByFieldName("cycle_length",cycle_length);
			if( rt ) rt = parser.GetValueByFieldName("offset",offset);
			if( rt ) rt = parser.GetValueByFieldName("sc_no",sc_no);
			if( rt )
			{
				PhaseRecord* pRecord = new PhaseRecord(phase_no,
					phase_name.c_str(),
					phase_type,
					-1,
					group_no,
					sequence_type,
					start_time,
					green_length,
					yellow_length,
					red_length,
					cycle_length,
					offset);
				pRecord->nSCNo = sc_no;
				phases.push_back(pRecord);				
			}
			else
				break;
		}
	}
	else
	{
		CString errorMsg;
		errorMsg.Format("File %s cannot be opened!",strCSV);
		AfxMessageBox(errorMsg);
		return false;
	}

	if ( rt )  return true;
	else
	{
		for(int i=0;i<phases.size();i++)
		{
			PhaseRecord* p = phases[i];
			delete p;
		}
		phases.clear();
		return false;
	}	
}
void MicroSimulatorInterface::CreateDefaultData()
{
	int i;

	m_szANMFileName = _T("testData.anm");
	m_szVersNo = _T("1.0");
	m_szFromTime = _T("00:00:00");
	m_szToTime = _T("10:00:00");
	m_szName = _T("MicroSimulatorInterface.ver");
	m_nLeftHandTraffic=0;

	m_szVehTypes = _T("");      // This string is reserved for attributes of VEHTYPES. The following 3 are in groups
	CString s1,s2,s3;
	s1.Format(TEXT("%d"),1);
	s2.Format(TEXT("%s"),"PtDefault");
	s3.Format(TEXT("%s"),"Bus");
	m_szVehTypeNo.Add(s1);
	m_szVehTypeName.Add(s2);
	m_szVehCategory.Add(s3);

	s1.Format(TEXT("%d"),100);
	s2.Format(TEXT("%s"),"AllPurp");
	s3.Format(TEXT("%s"),"Car");
	m_szVehTypeNo.Add(s1);
	m_szVehTypeName.Add(s2);
	m_szVehCategory.Add(s3);

	s1.Format(TEXT("%d"),200);
	s2.Format(TEXT("%s"),"XX");
	s3.Format(TEXT("%s"),"Car");
	m_szVehTypeNo.Add(s1);
	m_szVehTypeName.Add(s2);
	m_szVehCategory.Add(s3);

	CMVehClass *pVehClass = new CMVehClass();
	pVehClass->m_szVehClassId = _T("PtDefault");
	s1 = _T("1");
	pVehClass->m_szVehTypeIdArray.Add(s1);
	m_VehClassArray.Add(pVehClass);

	pVehClass = new CMVehClass();
	pVehClass->m_szVehClassId = _T("C");
	s1 = _T("100");
	pVehClass->m_szVehTypeIdArray.Add(s1);
	s1 = _T("200");
	pVehClass->m_szVehTypeIdArray.Add(s1);
	m_VehClassArray.Add(pVehClass);

	//m_szLinkTypes = _T("");     // This string is reserved for attributes of LINKTYPES. The following 3 are in groups
	//m_szLinkTypeNo.Add(_T("1"));
	//m_szLinkTypeName.Add(_T("Major State Hwy"));
	//m_szDrivingBehavior.Add(_T("Urban"));
	//m_szLinkTypeNo.Add(_T("3"));
	//m_szLinkTypeName.Add(_T("Minor State Hwy"));
	//m_szDrivingBehavior.Add(_T("Urban"));
	//m_szLinkTypeNo.Add(_T("5"));
	//m_szLinkTypeName.Add(_T("Principal Arterial"));
	//m_szDrivingBehavior.Add(_T("Urban"));
	//m_szLinkTypeNo.Add(_T("7"));
	//m_szLinkTypeName.Add(_T("Minor Arterial"));
	//m_szDrivingBehavior.Add(_T("Urban"));
	//m_szLinkTypeNo.Add(_T("61"));
	//m_szLinkTypeName.Add(_T("Collector Arterial"));
	//m_szDrivingBehavior.Add(_T("Urban"));
	//m_szLinkTypeNo.Add(_T("71"));
	//m_szLinkTypeName.Add(_T("Neighborh. Collector"));
	//m_szDrivingBehavior.Add(_T("Urban"));
	//m_szLinkTypeNo.Add(_T("81"));
	//m_szLinkTypeName.Add(_T("Neighborhood"));
	//m_szDrivingBehavior.Add(_T("Urban"));
	//m_szLinkTypeNo.Add(_T("85"));
	//m_szLinkTypeName.Add(_T("Major Parking Access"));
	//m_szDrivingBehavior.Add(_T("Urban"));
	//m_szLinkTypeNo.Add(_T("91"));
	//m_szLinkTypeName.Add(_T("Artificial Connector"));
	//m_szDrivingBehavior.Add(_T("Urban"));
	//m_szLinkTypeNo.Add(_T("92"));
	//m_szLinkTypeName.Add(_T("Connector"));
	//m_szDrivingBehavior.Add(_T("Urban"));
	//m_szLinkTypeNo.Add(_T("100"));
	//m_szLinkTypeName.Add(_T("Common Connector"));
	//m_szDrivingBehavior.Add(_T("Urban"));

	m_szPTStops = _T("");

	m_szPTLines = _T("");

	CMSignalControl* pSC = new CMSignalControl();
	pSC->szNo = _T("9133");
	pSC->szName = _T("Signal9133");
	pSC->nCycleTime = 120;
	pSC->nTimeOffset = 0;
	pSC->nSignalizationType = 1;
	pSC->bCycleTimeFixed = true;
	m_SignalControlArray.Add(pSC);
	for(i=1;i<=4;i++)
	{
		CMSignalGroup *pSG = new CMSignalGroup();
		pSC->signalgroups.Add(pSG);
		pSG->nNo = i;
		pSG->szName.Format(TEXT("SG%2d"),i); 
		pSG->nGTStart = (i-1)*30;
		pSG->nGTEnd = i*30;
		pSG->nMingTime = 0;
		pSG->nATime = 0;
		pSG->szSignalControlType = _T("Cycle");
	}
}
bool MicroSimulatorInterface::CreateANMRoutesFile(std::string strFileName)
{
	char elem[256];
	std::ofstream rf;
	rf.open(strFileName.c_str());
	rf<<"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
	rf<<"<ABSTRACTNETWORKMODEL VERSNO=\"1.0\" FROMTIME=\"09:00:00\" TOTIME=\"12:00:00\" NAME=\"simulation.anmRoutes\">\n";
	rf<<"\t<MATRICES>\n";
	rf<<"\t\t<MATRIX VEHTYPEID=\"100\" FROMTIME=\"09:00:00\" TOTIME=\"12:00:00\">\n";
	rf<<"\t\t\t<ELEMS>\n";
	for(int i=0;i<m_odDemand.size();i++)
	{
		sprintf_s(elem,"\t\t\t\t<ELEM FROMZONENO=\"%d\" TOZONENO=\"%d\" VOLUME=\"%.3f\"/>\n",
			m_odDemand[i]->m_nFromZone,
			m_odDemand[i]->m_nToZone,
			m_odDemand[i]->m_fDemand[0]);
		rf<<elem;
	}
	rf<<"\t\t\t</ELEMS>\n";
	rf<<"\t\t</MATRIX>\n";
	rf<<"\t\t<MATRIX VEHTYPEID=\"200\" FROMTIME=\"09:00:00\" TOTIME=\"12:00:00\">\n";
	rf<<"\t\t\t<ELEMS>\n";
	rf<<"\t\t\t</ELEMS>\n";
	rf<<"\t\t</MATRIX>\n";
	rf<<"\t</MATRICES>\n";
	rf<<"\t<ROUTING>\n";
	rf<<"\t\t<VEHTYPETIS>\n";
	rf<<"\t\t\t<VEHTYPETI INDEX=\"1\" FROMTIME=\"09:00:00\" TOTIME=\"12:00:00\" VEHTYPEID=\"100\"/>\n";
	rf<<"\t\t\t<VEHTYPETI INDEX=\"2\" FROMTIME=\"09:00:00\" TOTIME=\"12:00:00\" VEHTYPEID=\"200\"/>\n";
	rf<<"\t\t</VEHTYPETIS>\n";
	rf<<"\t\t<ROUTES>\n";
	for(int i=0;i<m_pathDemand.size();i++)
	{
		MPath *pPath = m_pathDemand[i];
		sprintf_s(elem,"\t\t\t\t<ROUTE FROMZONENO=\"%d\" TOZONENO=\"%d\" INDEX=\"%d\">\n",pPath->m_nFromZone,pPath->m_nToZone,pPath->m_nIndex);
		rf<<elem;
		rf<<"\t\t\t\t\t<ITEMS>\n";
		for(int j=0;j<pPath->m_nodes.size();j++)
		{
			sprintf_s(elem,"\t\t\t\t\t\t<ITEM NODE=\"%d\"/>\n",pPath->m_nodes[j]);
			rf<<elem;
		}
		rf<<"\t\t\t\t\t</ITEMS>\n";
		rf<<"\t\t\t\t\t<DEMANDS>\n";
		sprintf_s(elem,"\t\t\t\t\t\t<DEMAND VTI=\"1\" VOLUME=\"%.3f\"/>\n",pPath->m_vti1);
		rf<<elem;
		rf<<"\t\t\t\t\t</DEMANDS>\n";
		rf<<"\t\t\t\t</ROUTE>\n";
	}
	rf<<"\t\t</ROUTES>\n";
	rf<<"\t</ROUTING>\n";
	rf<<"</ABSTRACTNETWORKMODEL>";
	rf.close();
	return true;
}


/*
// Follow is from Xuesong
void CTLiteDoc::ConstructandexportVISSIMdata()
{
CString str;
CFileDialog dlg (FALSE, "*.csv", "*.csv",OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_LONGNAMES,
"VISSIM Data File (*.csv)|*.csv||", NULL);
if(dlg.DoModal() == IDOK)
{

CWaitCursor wait;
char fname[_MAX_PATH];
wsprintf(fname,"%s", dlg.GetPathName());

CString SynchroProjectFile = dlg.GetPathName();
m_Synchro_ProjectDirectory  = SynchroProjectFile.Left(SynchroProjectFile.ReverseFind('\\') + 1);

m_Network.Initialize (m_NodeSet.size(), m_LinkSet.size(), 1, m_AdjLinkSize);
m_Network.BuildPhysicalNetwork(&m_NodeSet, &m_LinkSet, true, false);

}


// generate all movements
int i = 0;
for (std::list<DTANode*>::iterator  iNode = m_NodeSet.begin(); iNode != m_NodeSet.end(); iNode++, i++)
{  // for current node

if ((*iNode)->m_ControlType > 1)  //(m_Network.m_InboundSizeAry[i] >= 3) // add node control types
{


// scan each inbound link and outbound link

for(int inbound_i= 0; inbound_i< m_Network.m_InboundSizeAry[i]; inbound_i++)
{
// for each incoming link
for(int outbound_i= 0; outbound_i< m_Network.m_OutboundSizeAry [i]; outbound_i++)
{
//for each outging link
int LinkID = m_Network.m_InboundLinkAry[i][inbound_i];

if (m_Network.m_FromIDAry[LinkID] != m_Network.m_OutboundNodeAry [i][outbound_i])
{
// do not consider u-turn

DTA_Movement element;

element.CurrentNodeID = i;						

element.InboundLinkID = LinkID;
element.UpNodeID = m_Network.m_FromIDAry[LinkID];
element.DestNodeID = m_Network.m_OutboundNodeAry [i][outbound_i];

GDPoint p1, p2, p3;
p1  = m_NodeNoMap[element.UpNodeID]->pt;
p2  = m_NodeNoMap[element.CurrentNodeID]->pt;
p3  = m_NodeNoMap[element.DestNodeID]->pt;

element.movement_direction = g_Angle_to_Approach_New(Find_P2P_Angle(p1,p2));
element.movement_turn = Find_PPP_to_Turn(p1,p2,p3);

// determine  movement type /direction here
element.dir = DTA_LANES_COLUME_init;
switch (element.movement_direction)
{
case DTA_North:
switch (element.movement_turn)
{
case DTA_LeftTurn: element.dir = DTA_NBL; break;
case DTA_Through: element.dir = DTA_NBT; break;
case DTA_RightTurn: element.dir = DTA_NBR; break;
}
break;
case DTA_East:
switch (element.movement_turn)
{
case DTA_LeftTurn: element.dir = DTA_EBL; break;
case DTA_Through: element.dir = DTA_EBT; break;
case DTA_RightTurn: element.dir = DTA_EBR; break;
}
break;
case DTA_South:
switch (element.movement_turn)
{
case DTA_LeftTurn: element.dir = DTA_SBL; break;
case DTA_Through: element.dir = DTA_SBT; break;
case DTA_RightTurn: element.dir = DTA_SBR; break;
}
break;
case DTA_West:
switch (element.movement_turn)
{
case DTA_LeftTurn: element.dir = DTA_WBL; break;
case DTA_Through: element.dir = DTA_WBT; break;
case DTA_RightTurn: element.dir = DTA_WBR; break;
}
break;
}


}  // for each feasible movement (without U-turn)

} // for each outbound link

} // for each inbound link

} // checking control type
}// for each node
}
*/
MLink* MNode::GetBrunchLink(int nLeg /*= 8*/)
{
	if ( inLinks.size() != 3) return NULL;

	MLink* p0 = inLinks[0];
	MLink* p1 = inLinks[1];
	MLink* p2 = inLinks[2];

	int nResolution = 30;

	if ( p1->nInAngle - p0->nInAngle >= 90 - nResolution && p1->nInAngle - p0->nInAngle <= 90 + nResolution ) // 0 and 1 vertical
	{
		if ( p2->nInAngle - p1->nInAngle >= 90 - nResolution && p2->nInAngle - p1->nInAngle <= 90 + nResolution ) // 1 and 2 vertical
		{
			if ( 7 == nLeg ) return p0;
			if ( 8 == nLeg ) return p1;
			if ( 9 == nLeg ) return p2;
		}
		else // 1 and 2 horizontal
		{
			if ( 7 == nLeg ) return p2;
			if ( 8 == nLeg ) return p0;
			if ( 9 == nLeg ) return p1;
		}
	}
	else // 0 and 1 horizontal, then 1 and 2 must be vertical
	{
		if ( 7 == nLeg ) return p1;
		if ( 8 == nLeg ) return p2;
		if ( 9 == nLeg ) return p0;
	}
	return NULL;
}
int MNode::CheckMissingApproach(void)
{
	if (inLinks.size() != 3 || outLinks.size() != 3) return 0;

	int sumIn=0,mulIn=1;
	std::vector<MLink*>::iterator iLink;
	for(iLink = inLinks.begin(); iLink != inLinks.end(); iLink++)
	{
		sumIn += (*iLink)->m_ToNodeApproach;
		mulIn *= (*iLink)->m_ToNodeApproach;
	}
	if (mulIn == 0) return 0;

	if ( (sumIn == 6 && mulIn == 6) ||
		(sumIn == 9 && mulIn == 24)||
		(sumIn == 8 && mulIn == 12)||
		(sumIn == 7 && mulIn == 8) )
	{
	}
	else
		return 0;

	int sumOut=0,mulOut=1;
	for(iLink = outLinks.begin(); iLink != outLinks.end(); iLink++)
	{
		sumOut += (*iLink)->m_FromNodeApproach;
		mulOut *= (*iLink)->m_FromNodeApproach;
	}
	if ( sumIn != sumOut || mulIn != mulOut) return 0;

	return 10-sumIn;
}
bool MicroSimulatorInterface::ReadOutputODFlowCSV(std::string strFileName)
{
	CCSVParser parser;
	CString error_message;
	int i=0;

	if (parser.OpenCSVFile(strFileName))
	{
		m_logFile<<"AMS_od_flow.csv reading...."<<endl;
		while(parser.ReadRecord())
		{
			int od_index = 0;
			int vehicle_type = 1;
			int from_zone_id;
			int to_zone_id;
			float time_span_volume=0.0,day_volume=0.0;

			if(!parser.GetValueByFieldName("od_index",od_index))				od_index	 = 0;
			if(!parser.GetValueByFieldName("vehicle_type",vehicle_type))		vehicle_type = 1;
			if(!parser.GetValueByFieldName("from_zone_id",from_zone_id) || !parser.GetValueByFieldName("to_zone_id",to_zone_id)) 
			{
				AfxMessageBox("Field from_zone_id or to_zone_id has not been defined in file AMS_od_flow.csv. Please check.");
				break;
			}
			if(!parser.GetValueByFieldName("time_span_volume",time_span_volume)) time_span_volume = 0.0;   
			if(!parser.GetValueByFieldName("day_volume",day_volume))			 day_volume		  = 0.0;   

			// check to ensure both zones are valide!
			// desert those duplicated od
			if ( from_zone_id == to_zone_id ) continue;
			if ( CheckDuplicateODDemand(from_zone_id,to_zone_id,vehicle_type) ) continue;
			MDemand *p = new MDemand();
			p->m_nFromZone	= from_zone_id;
			p->m_nToZone    = to_zone_id;
			p->m_nVehicleType = vehicle_type;
			p->m_fDemand[0] = time_span_volume;
			p->m_fDemand[1] = day_volume;
			m_odDemand.push_back(p);

			i++;
		}
		m_logFile<<"Sumary: All together "<<i<<" od flows have been read!"<<endl;
		return true;
	}
	else
	{
		AfxMessageBox("Error: File AMS_od_flow.csv cannot be opened.\n It might be currently used and locked by EXCEL.");
		return false;
	}
}
bool MicroSimulatorInterface::CheckDuplicateODDemand(int fromZoneID,int toZoneID,int nVehType /*=1*/)
{
	bool bDuplicate = false;

	for(int i=0;i<m_odDemand.size();i++)
	{
		if ( fromZoneID == m_odDemand[i]->m_nFromZone && toZoneID == m_odDemand[i]->m_nToZone && nVehType == m_odDemand[i]->m_nVehicleType )
		{
			bDuplicate = true;
			break;
		}
	}
	return bDuplicate;
}
bool MicroSimulatorInterface::ReadOutputPathFlowCSV(std::string strFileName)
{
	CCSVParser parser;
	CString error_message;
	int i=0;

	if (parser.OpenCSVFile(strFileName))
	{
		m_logFile<<"AMS_path_flow.csv reading...."<<endl;
		while(parser.ReadRecord())
		{
			int route_index = 0;
			int vehicle_type = 1;
			int from_zone_id;
			int from_node_id;
			int to_zone_id;
			int to_node_id;
			float time_span_volume=0.0,day_volume=0.0;
			int node_chain_number_of_nodes;

			if(!parser.GetValueByFieldName("route_index",route_index))			route_index	 = 0;
			if(!parser.GetValueByFieldName("vehicle_type",vehicle_type))		vehicle_type = 1;
			if( !parser.GetValueByFieldName("from_zone_id",from_zone_id) || 
				!parser.GetValueByFieldName("from_node_id",from_node_id) || 
				!parser.GetValueByFieldName("to_zone_id",to_zone_id)	 ||
				!parser.GetValueByFieldName("to_node_id",to_node_id)     )
			{
				AfxMessageBox("Field from_id or to_id has not been defined in file AMS_path_flow.csv. Please check.");
				break;
			}
			if(!parser.GetValueByFieldName("time_span_volume",time_span_volume)) time_span_volume = 0.0;   
			if(!parser.GetValueByFieldName("day_volume",day_volume))			 day_volume		  = 0.0;   
			if(!parser.GetValueByFieldName("node_chain_number_of_nodes",node_chain_number_of_nodes))   
			{
				AfxMessageBox("node_chain_number_of_nodes is zero. Please check.");
				break;
			}

			string node_chain_node_sequence;
			std::vector<int> nodeVector;
			if(parser.GetValueByFieldName("node_chain_node_sequence",node_chain_node_sequence))
			{
				nodeVector = GetNodeVector(node_chain_node_sequence);
			}
			else
			{
				AfxMessageBox("node_chain_node_sequence is empty. Please check.");
				break;
			}
			MPath *pPath = new MPath();
			pPath->m_nFromZone = from_zone_id;
			pPath->m_nToZone   = to_zone_id;
			pPath->m_nIndex    = route_index;
			if ( 1 == vehicle_type )
				pPath->m_vti1 = day_volume;
			else
				pPath->m_vti2 = day_volume;


			if(m_bUseSequentialNodeNumber  )  // replace the original node number by the sequential node no. 
			{
				for(unsigned i = 0; i < nodeVector.size(); i++)
				{
					int node_id = nodeVector[i];
					if (m_OriginalNodeNumbertoNodeNumberMap.find(node_id)==m_OriginalNodeNumbertoNodeNumberMap.end() )
					{
						break;
					}

					nodeVector[i] = m_OriginalNodeNumbertoNodeNumberMap[node_id]; 
				}
			}

			pPath->m_nodes = nodeVector;
			m_pathDemand.push_back(pPath);

			i++;
		}
		m_logFile<<"Sumary: All together "<<i<<" links have been read!"<<endl;
		return true;
	}
	else
	{
		AfxMessageBox("Error: File input_link.csv cannot be opened.\n It might be currently used and locked by EXCEL.");
		return false;
	}
}
std::vector<int> MicroSimulatorInterface::GetNodeVector(std::string s)
{
	std::vector<int> nodes;
	int node_id=0;

	string tmp = s.substr(s.find_first_not_of(' '));
	size_t start_idx = tmp.find_first_of('[');
	size_t end_idx = tmp.find_first_of(']');

	string type_str = tmp.substr(start_idx + 1,end_idx - start_idx -1);

	istringstream ss(type_str);
	string sub_str;

	while(std::getline(ss,sub_str,';'))
	{
		istringstream sub_ss(sub_str);
		sub_ss >> node_id;
		nodes.push_back(node_id);
	}
	return nodes;
}
void MNode::AddInLink(MLink *pLink)
{
	int nNewAngle = pLink->nInAngle;
	bool bInserted = false;
	std::vector<MLink*>::iterator iLink;

	for(iLink = inLinks.begin();iLink!=inLinks.end();iLink++)
	{
		int nAngle = (*iLink)->nInAngle;
		if (nNewAngle < nAngle)
		{
			inLinks.insert(iLink,pLink);
			bInserted = true;
			break;
		}
	}
	if (!bInserted)
	{
		inLinks.push_back(pLink);
	}
}
void MNode::AddOutLink(MLink *pLink)
{
	int nNewAngle = pLink->nOutAngle;
	bool bInserted = false;
	std::vector<MLink*>::iterator iLink;

	for(iLink = outLinks.begin();iLink!=outLinks.end();iLink++)
	{
		int nAngle = (*iLink)->nInAngle;
		if (nNewAngle < nAngle)
		{
			outLinks.insert(iLink,pLink);
			bInserted = true;
			break;
		}
	}
	if (!bInserted)
	{
		outLinks.push_back(pLink);
	}
}
MMovement* MNode::GetMMovement(int nFromLinkID, int nToLinkID)
{
	MMovement* pMove = NULL;
	for(int i=0;i<Movements.size();i++)
	{
		if ( nFromLinkID == Movements[i]->nFromLinkId && nToLinkID == Movements[i]->nToLinkId)
		{
			pMove = Movements[i];
			break;
		}
	}
	return pMove;
}
int  MNode::GetLaneTurnIndex(int nInLink, int nInLane, int nOutLink, int nOutLane)
{
	int nIndex = -1;
	for(int i=0;i<LaneTurns.size();i++)
	{
		MLaneTurn* p = LaneTurns[i];
		if ( nInLink == p->nFromLinkId && nInLane == p->nFromIndex && nOutLink == p->nToLinkId && nOutLane == p->nToIndex )
		{
			nIndex = i;
			break;
		}
	}
	return nIndex;
}
void MNode::OrderLink(MLink *pMLink /* = 0*/)
{
	if ( pMLink )
	{
		int nNewAngle;
		if ( pMLink->m_FromNodeID == this->m_NodeNo ) // out
			nNewAngle = pMLink->nOutAngle;
		else
			nNewAngle = pMLink->nInAngle;

		bool bInserted = false;
		std::vector<MLink*>::iterator iLink;

		for(iLink = orderedLinks.begin();iLink!=orderedLinks.end();iLink++)
		{
			int nAngle = ((*iLink)->m_FromNodeID==this->m_NodeNo) ? (*iLink)->nOutAngle : (*iLink)->nInAngle;
			if (nNewAngle <= nAngle)
			{
				if ( (nNewAngle < nAngle) || (nNewAngle == nAngle && (*iLink)->m_FromNodeID==this->m_NodeNo))
				{ // insert before this pos
					orderedLinks.insert(iLink,pMLink);
				}
				else // nNewAngle == nAngle && 1 == p->nIO
				{// insert after this pose
					orderedLinks.insert(iLink+1,pMLink);
				}
				bInserted = true;
				break;
			}
		}
		if (!bInserted)
		{
			orderedLinks.push_back(pMLink);
		}
	}
	else
	{
		if ( orderedLinks.size() > 0 ) orderedLinks.clear();

		for(int i=0;i<inLinks.size();i++)
		{
			OrderLink(inLinks[i]);
		}
		for(int i=0;i<outLinks.size();i++)
		{
			OrderLink(outLinks[i]);
		}
	}
	return;
}
MNode* MicroSimulatorInterface::GetMNodebyID(int nMNodeID)
{
	MNode* p=NULL;
	std::list<MNode*>::iterator iNode;
	for(iNode=m_NodeList.begin();iNode!=m_NodeList.end();iNode++)
	{
		if ( (*iNode)->m_NodeNo == nMNodeID )
		{
			p = (*iNode);
			break;
		}
	}
	return p;
}
MLink* MicroSimulatorInterface::GetMLinkbyID(int nMLinkID)
{
	MLink* p=NULL;
	std::list<MLink*>::iterator iLink;
	for(iLink=m_LinkList.begin();iLink!=m_LinkList.end();iLink++)
	{
		if ( (*iLink)->m_LinkID == nMLinkID )
		{
			p = (*iLink);
			break;
		}
	}
	return p;
}
void MicroSimulatorInterface::CreateLanes(bool bPocket /* = true*/)
{// create lanes according to movement
	float fPocketLength = 40.0;
	std::list<MNode*>::iterator iNode;
	for(iNode=m_NodeList.begin();iNode!=m_NodeList.end();iNode++)
	{
		MNode* p = (*iNode);
		if ( bPocket && (p->m_nProcessType == 4 || p->m_nProcessType == 3)) //只处理标准的十字和三岔路口
		{
			for(int i=0;i<p->Movements.size();i++)
			{
				MMovement* pm = p->Movements[i];
				if ( pm->nRTL == 3 )
				{
					MLane* pLane = new MLane();
					MLink* pLink = GetMLinkbyID(pm->nFromLinkId);
					pLink->inLanes.push_back(pLane);
					pLane->leftTurn = true;
					pLane->m_ChannelLength = 0;
					pLane->m_LinkID = pLink->m_LinkID;
					pLane->m_NodeNo = p->m_NodeNo;
					pLane->m_PocketLength = fPocketLength;
					pLane->through = false;
					pLane->rightTurn = false;
					pLane->m_Index = pLink->m_NumberOfLanes + 1;
				}
			}
		}
		// 只按照laneNum生成lane
		for(int i=0;i<p->inLinks.size();i++)
		{
			MLink* pLink = p->inLinks[i];
			for(int j=1;j<=pLink->m_NumberOfLanes;j++)
			{
				MLane * pLane = new MLane();
				pLink->inLanes.push_back(pLane);
				pLane->leftTurn = false;
				pLane->m_ChannelLength = 0;
				pLane->m_LinkID = pLink->m_LinkID;
				pLane->m_NodeNo = p->m_NodeNo;
				pLane->m_PocketLength = 0;
				pLane->through = true;
				pLane->rightTurn = j==pLink->m_NumberOfLanes ? true : false;
				pLane->m_Index = j;
			}
		}		
		for(int i=0;i<p->outLinks.size();i++)
		{
			MLink* pLink = p->outLinks[i];
			for(int j=1;j<=pLink->m_NumberOfLanes;j++)
			{
				MLane * pLane = new MLane();
				pLink->outLanes.push_back(pLane);
				pLane->leftTurn = false;
				pLane->m_ChannelLength = 0;
				pLane->m_LinkID = pLink->m_LinkID;
				pLane->m_NodeNo = p->m_NodeNo;
				pLane->m_PocketLength = 0;
				pLane->through = false;
				pLane->rightTurn = false;
				pLane->m_Index = j;
			}
		}		
	}
}
void MicroSimulatorInterface::CreateLaneTurns(void)
{
	m_logFile<<"CreateLaneTurns function called!"<<endl;

	std::list<MNode*>::iterator iNode;
	for(iNode=m_NodeList.begin();iNode!=m_NodeList.end();iNode++)
	{
		MNode* pNode = (*iNode);
		m_logFile<<"node id "<<pNode->m_NodeNo<<endl;
		for(int i=0;i<pNode->Movements.size();i++)
		{
			MMovement* pm = pNode->Movements[i];
			m_logFile<<"Movement:"<<i<<" From "<<pm->nFromLinkId<<" To "<<pm->nToLinkId<<" RTL "<<pm->nRTL<<endl;
			MLink* piLink = GetMLinkbyID(pm->nFromLinkId);
			MLink* poLink = GetMLinkbyID(pm->nToLinkId);
			if ( 1 ==pm->nRTL ) //Right turn
			{
				// 右转，i与olink的车道同号联接，直至不能配对（下同！）；
				int nInCount = piLink->GetLaneCount(1,1);
				int nOutCount= poLink->GetLaneCount(0);
				int nTurnCount = min(nInCount,nOutCount);
				for(int l=1;l<=nTurnCount;l++)
				{
					MLaneTurn *pLaneTurn = new MLaneTurn();
					pNode->LaneTurns.push_back(pLaneTurn);
					pLaneTurn->bForbid     = pm->bForbid;
					pLaneTurn->nFromLinkId = piLink->m_LinkID;
					pLaneTurn->nFromIndex  = l;
					pLaneTurn->nToLinkId   = poLink->m_LinkID;
					pLaneTurn->nToIndex	   = l;
					pLaneTurn->nRTL		   = pm->nRTL;
					pLaneTurn->nSCNO       = pm->nSCNO;
					pLaneTurn->nSignalGroupNo = pm->nSGNO;
				}
			}
			if ( 2 ==pm->nRTL ) //Through
			{
				// 直行，与出link的车道同号或错号联接
				// 入直行车道大于出车道，则从入的2号开始联接，即把入1作为Right turnonly；
				//           等于      ，同号联接
				//           小于      ，则从出的2好开始联接，即把出1作为right turnonly。
				int nInCount = piLink->GetLaneCount(1,2);
				int nOutCount= poLink->GetLaneCount(0);
				int nOffsetIndexIn,nOffsetIndexOut;
				int nTurnCount;
				if ( nInCount > nOutCount)
				{
					nTurnCount = nOutCount;
					nOffsetIndexIn = 1;
					nOffsetIndexOut= 0;
				}
				else if ( nInCount == nOutCount)
				{
					nTurnCount = nOutCount;
					nOffsetIndexIn = 0;
					nOffsetIndexOut= 0;
				}
				else // nInCount < nOutCount
				{
					nTurnCount = nInCount;
					nOffsetIndexIn = 0;
					nOffsetIndexOut= 1;
				}
				for(int l=1;l<=nTurnCount;l++)
				{
					MLaneTurn *pLaneTurn = new MLaneTurn();
					pNode->LaneTurns.push_back(pLaneTurn);
					pLaneTurn->bForbid	   = pm->bForbid;
					pLaneTurn->nFromLinkId = piLink->m_LinkID;
					pLaneTurn->nFromIndex  = l + nOffsetIndexIn;
					pLaneTurn->nToLinkId   = poLink->m_LinkID;
					pLaneTurn->nToIndex	   = l + nOffsetIndexOut;
					pLaneTurn->nRTL		   = pm->nRTL;
					pLaneTurn->nSCNO       = pm->nSCNO;
					pLaneTurn->nSignalGroupNo = pm->nSGNO;
				}
			}
			if ( 3 == pm->nRTL ) // Left turn
			{
				// 左转，与出link的车道从大号开始对应联接
				int nInCount = piLink->GetLaneCount(1,3);
				int nOutCount= poLink->GetLaneCount(0);
				int nTurnCount = min(nInCount,nOutCount);
				int nLeftTurnInIndex = piLink->GetLaneCount(1,0);
				for(int l=0;l<nTurnCount;l++)
				{
					MLaneTurn *pLaneTurn = new MLaneTurn();
					pNode->LaneTurns.push_back(pLaneTurn);
					pLaneTurn->bForbid	   = pm->bForbid;
					pLaneTurn->nFromLinkId = piLink->m_LinkID;
					pLaneTurn->nFromIndex  = nLeftTurnInIndex - l;
					pLaneTurn->nToLinkId   = poLink->m_LinkID;
					pLaneTurn->nToIndex	   = nOutCount - l;
					pLaneTurn->nRTL		   = pm->nRTL;
					pLaneTurn->nSCNO       = pm->nSCNO;
					pLaneTurn->nSignalGroupNo = pm->nSGNO;
				}
			}
		}
	}
	m_logFile<<"CreateLaneTurns function ended!"<<endl;
}
void MicroSimulatorInterface::LoadData4Editing()
{//在当前目录下读三个ms文件创建路口信息
	CWaitCursor cursor;
	if ( ! m_pDoc ) return;

	CString strFolder  = m_pDoc->m_ProjectDirectory;
	m_strFolder = strFolder;
	CString strNodeCSV = strFolder + _T("input_node.csv");
	CString strLinkCSV = strFolder + _T("input_link.csv");
	CString strLinkTypeCSV = strFolder + _T("input_link_type.csv");
	CString strZoneCSV = strFolder + _T("input_activity_location.csv");

	CString strMSSignal= strFolder + _T("ms_signal.csv");
	CString strMSLane  = strFolder + _T("ms_lane.csv");
	CString strMSLaneturn= strFolder + _T("ms_laneturn.csv");

	std::string strNodeFileName,strLinkFileName,strLinkTypeFileName,strZoneFileName,strMSSignalFileName,strMSLaneFileName,strMSLaneturnFileName;
	USES_CONVERSION;

	strNodeFileName = T2A(strNodeCSV.GetBuffer());
	strLinkFileName = T2A(strLinkCSV.GetBuffer());
	strLinkTypeFileName = T2A(strLinkTypeCSV.GetBuffer());
	strZoneFileName = T2A(strZoneCSV.GetBuffer());

	strMSSignalFileName = T2A(strMSSignal.GetBuffer());
	strMSLaneFileName = T2A(strMSLane.GetBuffer());
	strMSLaneturnFileName = T2A(strMSLaneturn.GetBuffer());

	ReadInputNodeCSV(strNodeFileName);
	ReadInputLinkCSV(strLinkFileName);
	ReadInputLinkTypeCSV(strLinkTypeFileName);
	ReadInputActivityLocationCSV(strZoneFileName);
	FillReverseMLinkID();
	ReadInputSignalCSV(strMSSignalFileName);
	ClassifyNodes();
	ReadMSLane(strMSLaneFileName);
	ReadMSLaneturn(strMSLaneturnFileName);
	CreateSignal();

	//WriteMSSignal(strMSSignalFileName);
}
void MicroSimulatorInterface::Create2Files()
{// this function use memory content to create files, in the future should be replaced by dlls
	CWaitCursor cursor;
	if ( ! m_pDoc ) return;

	CString strFolder  = m_pDoc->m_ProjectDirectory;
	m_strFolder = strFolder;
	CString strANMFile = strFolder + _T("simulation.anm");
	CString strANMRoutesFile = strFolder + _T("simulation.anmRoutes");

	std::string strANMFileName,strANMRoutesFileName;
	USES_CONVERSION;

	strANMFileName  = T2A(strANMFile.GetBuffer());
	strANMRoutesFileName = T2A(strANMRoutesFile.GetBuffer());

	//WriteMSSignal(strMSSignalFileName);
	//WriteMSLane(strMSLaneFileName);
	//WriteMSLaneturn(strMSLaneturnFileName);

	CreateANMFile2(strANMFileName);
	CreateANMRoutesFile(strANMRoutesFileName);
}
void MicroSimulatorInterface::SaveJunctions()
{//create 3 ms_*.csv files
	CWaitCursor cursor;
	if ( ! m_pDoc ) return;

	CString strFolder  = m_pDoc->m_ProjectDirectory;
	m_strFolder = strFolder;
	CString strMSSignal= strFolder + _T("ms_signal.csv");
	CString strMSLane  = strFolder + _T("ms_lane.csv");
	CString strMSLaneturn= strFolder + _T("ms_laneturn.csv");

	std::string strMSSignalFileName,strMSLaneFileName,strMSLaneturnFileName;
	USES_CONVERSION;

	strMSSignalFileName = T2A(strMSSignal.GetBuffer());
	strMSLaneFileName = T2A(strMSLane.GetBuffer());
	strMSLaneturnFileName = T2A(strMSLaneturn.GetBuffer());

	WriteMSSignal(strMSSignalFileName);
	WriteMSLane(strMSLaneFileName);
	WriteMSLaneturn(strMSLaneturnFileName);
}
void MicroSimulatorInterface::PrepareData4Editing()
{
	CWaitCursor cursor;
	if ( ! m_pDoc ) return;

	CString strFolder  = m_pDoc->m_ProjectDirectory;
	m_strFolder = strFolder;
	CString strNodeCSV = strFolder + _T("input_node.csv");
	CString strLinkCSV = strFolder + _T("input_link.csv");
	CString strLinkTypeCSV = strFolder + _T("input_link_type.csv");
	CString strANMFile = strFolder + _T("simulation.anm");
	CString strANMRoutesFile = strFolder + _T("simulation.anmRoutes");
	CString strLogFile = strFolder + _T("msLog.log");
	CString strZoneCentroid = strFolder + _T("input_zone.csv");
	CString strZoneCSV = strFolder + _T("input_activity_location.csv");
	CString strODCSV = strFolder + _T("AMS_od_flow.csv");
	CString strPathCSV = strFolder + _T("AMS_path_flow.csv");
	CString strMSSignal= strFolder + _T("ms_signal.csv");
	CString strMSLane  = strFolder + _T("ms_lane.csv");
	CString strMSLaneturn= strFolder + _T("ms_laneturn.csv");

	std::string strNodeFileName,strLinkFileName,strLinkTypeFileName,strANMFileName,strODFileName,strPathFileName,strANMRoutesFileName,strLogFileName,strZoneFileName,strZoneCentroidFileName,strMSSignalFileName,strMSLaneFileName,strMSLaneturnFileName;
	USES_CONVERSION;
	strNodeFileName = T2A(strNodeCSV.GetBuffer());
	strLinkFileName = T2A(strLinkCSV.GetBuffer());
	strLinkTypeFileName = T2A(strLinkTypeCSV.GetBuffer());
	strANMFileName  = T2A(strANMFile.GetBuffer());
	strANMRoutesFileName = T2A(strANMRoutesFile.GetBuffer());
	strLogFileName  = T2A(strLogFile.GetBuffer());
	strZoneCentroidFileName = T2A(strZoneCentroid.GetBuffer());
	strZoneFileName = T2A(strZoneCSV.GetBuffer());
	strODFileName	= T2A(strODCSV.GetBuffer());
	strPathFileName = T2A(strPathCSV.GetBuffer());
	strMSSignalFileName = T2A(strMSSignal.GetBuffer());
	strMSLaneFileName = T2A(strMSLane.GetBuffer());
	strMSLaneturnFileName = T2A(strMSLaneturn.GetBuffer());

	OpenLogFile(strLogFileName);
	ReadInputNodeCSV(strNodeFileName);
	ReadInputLinkCSV(strLinkFileName);
	ReadInputLinkTypeCSV(strLinkTypeFileName);
	//ReadInputZoneCentroid(strZoneCentroidFileName);
	ReadInputActivityLocationCSV(strZoneFileName);
	FillReverseMLinkID();
	ReadInputSignalCSV(strMSSignalFileName);
	bool bReturn = ClassifyNodes();
	//DumpNodeLink2Log();
	if (bReturn) bReturn = CreateMovement();
	else 
		AfxMessageBox("Movement cannot be created!",MB_OK);

	CreateSignal();
	CreateLanes();
	CreateLaneTurns();

	//WriteMSSignal(strMSSignalFileName);
	//WriteMSLane(strMSLaneFileName);
	//WriteMSLaneturn(strMSLaneturnFileName);

	//CreateANMFile2(strANMFileName); //debug only
	ReadOutputODFlowCSV(strODFileName);
	ReadOutputPathFlowCSV(strPathFileName);
	//CreateANMRoutesFile(strANMRoutesFileName);//debug only
	CloseLogFile();
}
void MicroSimulatorInterface::FillReverseMLinkID()
{
	// 同时赋anmID的值
	//	m_logFile<< "FillReverseMLinkID function called!"<<endl;
	std::list<MLink*>::iterator iLink;
	int nANMID = 1;
	for (iLink = m_LinkList.begin(); iLink != m_LinkList.end(); iLink++)
	{
		int nReverseLinkID = GetReverseMLinkID( (*iLink)->m_LinkID );
		(*iLink)->m_ReverseLinkID = nReverseLinkID;
		if ( (*iLink)->strANMID == _T("NULL") )
		{
			(*iLink)->strANMID.Format("%dA",nANMID);
			(*iLink)->strANMRID.Format("%dB",nANMID);
			MLink* pReverse = GetMLinkbyID(nReverseLinkID);
			pReverse->strANMID.Format("%dB",nANMID);
			pReverse->strANMRID.Format("%dA",nANMID);
			nANMID ++;
		}
	}
	//	m_logFile<< "FillReverseMLinkID function Ended!"<<endl;
}
int  MicroSimulatorInterface::GetReverseMLinkID(int nMLinkID)
{
	// classify node types
	std::list<MLink*>::iterator iLink;
	int nReverseMLinkID = 0;
	MLink* pLink = GetMLinkbyID(nMLinkID);
	if (!pLink) return 0;

	// classify nodes
	for (iLink = m_LinkList.begin(); iLink != m_LinkList.end(); iLink++)
	{
		if ( (*iLink)->m_FromNodeID == pLink->m_ToNodeID && (*iLink)->m_ToNodeID == pLink->m_FromNodeID )
		{
			nReverseMLinkID = (*iLink)->m_LinkID;
			break;
		}
	}
	return nReverseMLinkID;
}
void MicroSimulatorInterface::WriteMSLane(std::string strFileName)
{
	FILE* f = NULL;
	fopen_s(&f,m_strFolder+"ms_lane.csv","w");
	if(f!=NULL)
	{
		fprintf(f, "node_id,link_id,in_n_out,in_angle,out_angle,lane_no,p_length,turn,dirty\n");
		std::list<MNode*>::iterator iNode;
		for(iNode=m_NodeList.begin();iNode!=m_NodeList.end();iNode++)
		{
			MNode* pNode = (*iNode);
			for(int j=0;j<pNode->inLinks.size();j++)
			{
				MLink* pLink = pNode->inLinks[j];
				for(int k=0;k<pLink->inLanes.size();k++)
				{
					MLane* pLane = pLink->inLanes[k];
					int nTurn =0;
					if (pLane->leftTurn ) nTurn+=1;
					if (pLane->through )  nTurn+=2;
					if (pLane->rightTurn) nTurn+=4;
					fprintf(f,"%d,%d,%d,%d,%d,%d,%.2f,%d,%d\n",pNode->m_NodeNo,pLink->m_LinkID,1,pLink->nInAngle,pLink->nOutAngle,pLane->m_Index,
						pLane->m_PocketLength+pLane->m_ChannelLength,nTurn,0);
				}
				for(int k=0;k<pLink->outLanes.size();k++)
				{
					int nTurn=0;
					MLane* pLane = pLink->outLanes[k];
					fprintf(f,"%d,%d,%d,%d,%d,%d,%.2f,%d,%d\n",pNode->m_NodeNo,pLink->m_LinkID,0,pLink->nInAngle,pLink->nOutAngle,pLane->m_Index,
						0.00,nTurn,0);
				}
			}
		}
		fclose(f);
	}
	else
	{
		AfxMessageBox("Error: cannot create ms_lane.csv file!");
	}
}
void MicroSimulatorInterface::WriteMSLaneturn(std::string strFileName)
{
	FILE* f = NULL;
	fopen_s(&f,m_strFolder+"ms_laneturn.csv","w");
	if(f!=NULL)
	{
		fprintf(f,"node_id,from_link,to_link,m_type,forbid,from_lane,to_lane,sc_no,sg_no,dirty\n");
		std::list<MNode*>::iterator iNode;
		for(iNode=m_NodeList.begin();iNode!=m_NodeList.end();iNode++)
		{
			MNode* pNode = (*iNode);
			for(int j=0;j<pNode->LaneTurns.size();j++)
			{
				MLaneTurn* pTurn = pNode->LaneTurns[j];
				MLink* pFromLink = GetMLinkbyID(pTurn->nFromLinkId);
				MLink* pToLink   = GetMLinkbyID(pTurn->nToLinkId);
				fprintf(f,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",pNode->m_NodeNo,pFromLink->m_LinkID,pToLink->m_LinkID,pTurn->nRTL,
					pTurn->bForbid ? 1 : 0,pTurn->nFromIndex,pTurn->nToIndex,pTurn->nSCNO,pTurn->nSignalGroupNo,0);
			}
		}
		fclose(f);
	}
	else
	{
		AfxMessageBox("Error: cannot create ms_laneturn.csv file!");
	}
}
void MicroSimulatorInterface::WriteMSSignal(std::string strFileName)
{
	FILE* f = NULL;
	fopen_s(&f,m_strFolder+"ms_signal.csv","w");
	if(f!=NULL)
	{
		fprintf(f, "sc,name,cycle_length,offset,sg,gt_start,gt_end,min_gt,at,sc_type\n");
		for(int i=0;i<m_SCs.size();i++)
		{
			MSigC* pc = m_SCs[i];
			for(int j=0;j<pc->SGs.size();j++)
			{
				MSigG* pg = pc->SGs[j];
				CString str;
				str.Format("%d,%s,%d,%d,%d,%d,%d,%d,%d,%d\n",pc->nID,pc->strName.c_str(),pc->nCycleLength,pc->nOffset,pg->nSGNO,pg->nGTStart,pg->nGTEnd,pg->nMinGT,pg->nAT,pc->nType);
				fprintf(f, "%d,%s,%d,%d,%d,%d,%d,%d,%d,%d\n",pc->nID,pc->strName.c_str(),pc->nCycleLength,pc->nOffset,pg->nSGNO,pg->nGTStart,pg->nGTEnd,pg->nMinGT,pg->nAT,pc->nType);
			}
		}
		fclose(f);
	}
	else
	{
		AfxMessageBox("Error: cannot create ms_signal.csv file!");
	}
}
void MicroSimulatorInterface::ReadMSLane(std::string strFileName)
{
	// lane是记录在link上的，所以之前link已经有值
	if ( m_LinkList.size() == 0 || m_NodeList.size() == 0 ) return;

	CCSVParser parser;

	if (parser.OpenCSVFile(strFileName))
	{
		MLink* pLink;
		int oldlinkid = -1;

		while(parser.ReadRecord())
		{
			int			node_id;
			int			link_id;
			int			in_n_out;
			int			in_angle;
			int			out_angle;
			int			lane_no;
			float		p_length;
			int			turn;

			parser.GetValueByFieldName("node_id",	node_id);
			parser.GetValueByFieldName("link_id",	link_id);
			parser.GetValueByFieldName("in_n_out",	in_n_out);
			parser.GetValueByFieldName("in_angle",	in_angle);
			parser.GetValueByFieldName("out_angle",	out_angle);
			parser.GetValueByFieldName("lane_no",	lane_no);
			parser.GetValueByFieldName("p_length",	p_length);
			parser.GetValueByFieldName("turn",		turn);

			if ( link_id != oldlinkid )
			{
				pLink = GetMLinkbyID(link_id);
				if (!pLink) return;
				oldlinkid = link_id;
			}
			MLane* pLane = new MLane();
			pLane->m_Index	= lane_no;
			pLane->m_LinkID = link_id;
			pLane->m_NodeNo = node_id;
			if ( turn >= 4 )
			{
				pLane->rightTurn = 1;
				turn -= 4;
			}
			else
			{
				pLane->rightTurn = 0;
			}
			if ( turn >= 2 )
			{
				pLane->through = 1;
				turn -= 2;
			}
			else
			{
				pLane->through = 0;
			}
			if ( turn >= 1 )
				pLane->leftTurn = 1;
			else
				pLane->leftTurn = 0;

			if ( p_length > 0 )
			{
				if ( 1 == pLane->leftTurn ) pLane->m_PocketLength = p_length;
				if ( 1 == pLane->rightTurn) pLane->m_ChannelLength= p_length;
			}
			else
			{
				pLane->m_PocketLength = p_length;
				pLane->m_ChannelLength= p_length;
			}

			if ( 1 == in_n_out )
			{
				pLink->inLanes.push_back(pLane);
			}
			else
			{
				pLink->outLanes.push_back(pLane);
			}
		}
	}
	return;
}
void MicroSimulatorInterface::ReadMSLaneturn(std::string strFileName)
{
	// lane是记录在link上的，所以之前link已经有值
	if ( m_LinkList.size() == 0 || m_NodeList.size() == 0 ) return;

	CCSVParser parser;

	if (parser.OpenCSVFile(strFileName))
	{
		MNode* pNode;
		int oldnodeid = -1;
		int oldfromlink = -1;
		int oldtolink = -1;

		while(parser.ReadRecord())
		{
			int			node_id;
			int			from_link;  
			int			to_link;
			int			m_type;
			int			forbid;
			int			from_lane;
			float		to_lane;
			int			sc_no;
			int			sg_no;

			parser.GetValueByFieldName("node_id",	node_id);
			parser.GetValueByFieldName("from_link",	from_link);
			parser.GetValueByFieldName("to_link",	to_link);
			parser.GetValueByFieldName("m_type",	m_type);
			parser.GetValueByFieldName("forbid",	forbid);
			parser.GetValueByFieldName("from_lane",	from_lane);
			parser.GetValueByFieldName("to_lane",	to_lane);
			parser.GetValueByFieldName("sc_no",		sc_no);
			parser.GetValueByFieldName("sg_no",		sg_no);

			if ( node_id != oldnodeid )
			{
				pNode = GetMNodebyID(node_id);
				if (!pNode) return;
				oldnodeid = node_id;
			}
			if ( oldfromlink != from_link || oldtolink != to_link )
			{
				MMovement* pMove = new MMovement();
				pNode->Movements.push_back(pMove);
				pMove->nFromLinkId = from_link;
				pMove->nToLinkId   = to_link;
				pMove->nSCNO	   = sc_no;
				pMove->nSGNO	   = sg_no;
				pMove->nRTL        = m_type;
				pMove->bForbid     = (1==forbid)?true:false;
				pMove->nSeqNo      = 0;
				if ( oldfromlink != from_link ) oldfromlink = from_link;
				if ( oldtolink   != to_link   ) oldtolink	= to_link;
			}
			MLaneTurn* pTurn = new MLaneTurn();
			pNode->LaneTurns.push_back(pTurn);
			pTurn->nFromLinkId	= from_link;
			pTurn->nToLinkId	= to_link;
			pTurn->nRTL			= m_type;
			pTurn->bForbid		= (1==forbid)?true:false;
			pTurn->nFromIndex	= from_lane;
			pTurn->nToIndex		= to_lane;
			pTurn->nSCNO		= sc_no;
			pTurn->nSignalGroupNo=sg_no;
		}
	}
	return;
}