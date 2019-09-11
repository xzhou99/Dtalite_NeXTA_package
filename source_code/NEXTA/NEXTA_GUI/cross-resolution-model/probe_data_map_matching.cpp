//  Portions Copyright 2012 Hongkai Yu (hongkaiyu2012@gmail.com), Xuesong Zhou (xzhou99@gmail.com)
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
#include "..//Geometry.h"
#include "..//CSVParser.h"
#include "..//TLite.h"
#include "..//Network.h"
#include "..//TLiteDoc.h"
//#include "..//Data-Interface//include//ogrsf_frmts.h"
#include "..//MainFrm.h"


struct DSP_GPS_Sample{
   int  ID;
   int  DeviceID;

   int LocalHour;
   int LocalMin;
   int LocalSec;
   int InternalClockTime;
   double x;
   double y;

   double dLong;
   double dLat;

   double Speed;
   double Bearing;

   int MatchedLinkID;
   int Flag;
   double InstantSpeed;
   double InstantBearing;
   int SatNum;
   double PDOP;
   double HDOP ;
   double VDOP ;

   double MoveTime;
   double MoveDistance;

   double MatchedSpeed;
   double MatchedBearing;
   double MatchedY;
   double MatchedX;
   double MatchedLinkRatio;
   double MatchedLong;
   double MatchedLat;


   bool bMatchedFlag;
   double LinkLength;
   char   nt_st_name[20];

};

/*bool CTLiteDoc::ReadGPSDataFile(LPCTSTR lpszFileName)
{

	FILE* pFile = NULL;

	fopen_s(&pFile,m_ProjectDirectory+lpszFileName,"r");

	if(pFile!=NULL)
	{
	int good_vehicle_id = 1;
	while(!feof(pFile) )
	{
		int m_VehicleID= g_read_integer(pFile);
		if(m_VehicleID == -1)
			break;

		TRACE("ID = %d\n",m_VehicleID);
		if(m_VehicleID!= m_ProbeSet.size())
		{
		TRACE("Error!");
		}
		DTAVehicle* pVehicle = 0;
		pVehicle = new DTAVehicle;
		pVehicle->m_VehicleID		= m_VehicleID;
         pVehicle-> m_VehicleKey = m_VehicleID;
        
		 int origin_zone_id = g_read_integer(pFile);

		int  dest_zone_id = g_read_integer(pFile);
		float trajecory_start_time = g_read_float(pFile);
		float trajecory_travel_time = g_read_float(pFile);

		pVehicle->m_TripTime = trajecory_travel_time/60; // second to min

		int number_of_segments  = g_read_integer(pFile);

		for(int i = 0; i< number_of_segments; i++)
		{

		 GDPoint pt;

		 pt.x =  g_read_float(pFile);

		 pt.y =  g_read_float(pFile);
       
		if(i==0)  // origin
        pVehicle->m_GPSLocationVector.push_back(pt); 

		 

		 pt.x =  g_read_float(pFile);
		 pt.y =  g_read_float(pFile);
    
		float segement_start_time = g_read_float(pFile);
		float segement_travel_time = g_read_float(pFile);

		if(i== number_of_segments-1) // destination
         pVehicle->m_GPSLocationVector.push_back(pt); 

        }

		if(pVehicle->m_GPSLocationVector.size() >=2)
		{
		 pVehicle->m_NodeSize = 2;
		pVehicle->m_bComplete = true;

		pVehicle->m_OriginZoneID = GetZoneIDFromShapePoints( pVehicle->m_GPSLocationVector[0]);
		pVehicle->m_DestinationZoneID  = GetZoneIDFromShapePoints( pVehicle->m_GPSLocationVector[pVehicle->m_GPSLocationVector.size()-1]);
		}

        m_ProbeSet.push_back (pVehicle);

    }

	fclose(pFile);
	return false;

	return true;

	}else
	{
		CString msg;
		msg.Format("File %s cannot be openned.", lpszFileName);

		AfxMessageBox(msg);
		return false;
	}


}
*/
bool CTLiteDoc::ReadGPSDataFile(LPCTSTR lpszFileName)
{

	FILE* pFile = NULL;

	fopen_s(&pFile,m_ProjectDirectory+lpszFileName,"r");

	if(pFile!=NULL)
	{
	int good_vehicle_id = 1;
	while(!feof(pFile) )
	{
		int m_VehicleID= g_read_integer(pFile);
		if(m_VehicleID == -1)
			break;

		TRACE("ID = %d\n",m_VehicleID);
		if(m_VehicleID!= m_ProbeSet.size())
		{
		TRACE("Error!");
		}
		DTAVehicle* pVehicle = 0;

		 
		int origin_zone_id = g_read_integer(pFile);

		int  dest_zone_id = g_read_integer(pFile);
		float trajecory_start_time = g_read_float(pFile);
		float trajecory_travel_time = g_read_float(pFile);

		int number_of_segments  = g_read_integer(pFile);

		 GDPoint pt;
		 GDPoint pt_origin, pt_destination;
		 for(int i = 0; i< number_of_segments; i++)
		{


		 pt.x =  g_read_float(pFile);

		 pt.y =  g_read_float(pFile);
       
		if(i==0)  // origin
        pt_origin= pt; 

		 

		 pt.x =  g_read_float(pFile);
		 pt.y =  g_read_float(pFile);
    
		float segement_start_time = g_read_float(pFile);
		float segement_travel_time = g_read_float(pFile);

		if(i== number_of_segments-1) // destination
        pt_destination= pt; 

        }

		if(number_of_segments >=2)
		{

		int m_OriginZoneID = GetZoneIDFromShapePoints(pt_origin);
		int m_DestinationZoneID  = GetZoneIDFromShapePoints( pt_destination);

		}

  
    }

	fclose(pFile);
	
	return true;

	}else
	{
		CString msg;
		msg.Format("File %s cannot be openned.", lpszFileName);

		AfxMessageBox(msg);
		return false;
	}


}
bool CTLiteDoc::ReadGPSCSVFile(LPCTSTR lpszFileName)
{
    // save the original code
    //CCSVParser parser;

    //if (parser.OpenCSVFile(lpszFileName))
    //{
    //    m_VehicleSet.clear();

    //    string trajecory_id = "-1";
    //    string prev_trajecory_id = "0";

    //    float x1, y1, x2, y2;
    //    float SegmentTravelTime;

    //    DTAVehicle* pVehicle = 0;


    //    while(parser.ReadRecord())
    //    {
    //        if(parser.GetValueByFieldName("trajecory_id",trajecory_id) == false)
    //        {
    //            break;
    //        }

    //        GDPoint pt1;
    //        GDPoint pt2;

    //        parser.GetValueByFieldName("x1",pt1.x);
    //        parser.GetValueByFieldName("y1",pt1.y);
    //        parser.GetValueByFieldName("x2",pt2.x);
    //        parser.GetValueByFieldName("y2",pt2.y);

    //        if(prev_trajecory_id.compare(trajecory_id)!=0)
    //        {
    //        // we have a new vehicle
    //        pVehicle = new DTAVehicle;
    //        pVehicle->m_VehicleID = m_VehicleSet.size();  // we skip vehicle id
    //        pVehicle-> m_VehicleKey = trajecory_id;
    //    
    //        m_VehicleSet.push_back (pVehicle);
    //        m_VehicleIDMap[pVehicle->m_VehicleID]  = pVehicle;
    //    
    //        }

    //        pVehicle->m_GPSLocationVector.push_back(pt1); 
    //        pVehicle->m_GPSLocationVector.push_back(pt2); 
    //        prev_trajecory_id = trajecory_id;
    //    }
    //}
 //  return true;
// save the original code


    CCSVParser parser;

    if (parser.OpenCSVFile(lpszFileName))
    {    
        m_VehicleSet.clear();
        float TrajecoryTravelTime;
        DTAVehicle* pVehicle = 0;
        std::vector<string> SeperatedStrings;
        while(parser.ReadRecord())
        {
           SeperatedStrings=parser.GetLineRecord();
           TrajecoryTravelTime=::atof(SeperatedStrings[4].c_str());
           pVehicle = new DTAVehicle;
           m_VehicleSet.push_back(pVehicle);
           m_VehicleIDMap[pVehicle->m_VehicleID]  = pVehicle;
           for(int i=6;i<(SeperatedStrings.size()-6);i=i+6)// read the data every 6 numbers
           {
               if((SeperatedStrings[i]!="")&&(SeperatedStrings[i+1]!="")&&(SeperatedStrings[i+2]!="")&&(SeperatedStrings[i+3]!=""))
               {
                   GDPoint pt1;
                   GDPoint pt2;

                   pt1.x=::atof(SeperatedStrings[i].c_str());
                   pt1.y=::atof(SeperatedStrings[i+1].c_str());
                   pt2.x=::atof(SeperatedStrings[i+2].c_str());
                   pt2.y=::atof(SeperatedStrings[i+3].c_str());

                   pVehicle->m_GPSLocationVector.push_back(pt1); 
                   pVehicle->m_GPSLocationVector.push_back(pt2); 
               }
            }
        }

    parser.CloseCSVFile();
    return true;
    }

	return true;
}

void CTLiteDoc::OnToolsGpsmapmatching()
{
 

// create a network
    // for each link, define the distance cost
    // find the shortest path
	// step 1: find out the origin zone and destination zone

	std::list<DTAVehicle*>::iterator iVehicle;


    std::list<DTALink*>::iterator iLink;

    for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  //m_LinkSet ?????????
    {
        (*iLink)->m_OriginalLength = (*iLink)->m_Length ;  // keep the original link length
    }
    
        for (std::list<DTALink*>::iterator iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
    {
        (*iLink)->m_DisplayLinkID = -1;
    }

   


    // for each vehicle

    //each vehicle GPS data are stored in the list called m_VehicleSet

    for (iVehicle = m_VehicleSet.begin(); iVehicle != m_VehicleSet.end(); iVehicle++)
    {
        DTAVehicle* pVehicle = (*iVehicle);
        if(m_pNetwork ==NULL)  // we only build the network once
        {
          m_pNetwork = new DTANetworkForSP(m_NodeSet.size(), m_LinkSet.size(), 1, 1, m_AdjLinkSize);  //  network instance for single processor in multi-thread environment
        }
        if(pVehicle->m_GPSLocationVector.size()>=2)
        {
           //find OD nodes for each vehicle
           m_OriginNodeID = FindClosestNode(pVehicle->m_GPSLocationVector[0]);
           m_DestinationNodeID = FindClosestNode(pVehicle->m_GPSLocationVector[pVehicle->m_GPSLocationVector.size()-1]);
           // set new length for links
           std::list<DTALink*>::iterator iLink;
           int max_link_id = 1;
           for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  // for each link in this network
           {
             double min_distance = 999999;
             GDPoint pfrom = (*iLink)->m_FromPoint;
             GDPoint pto = (*iLink)->m_ToPoint;
             for(int i = 0; i < pVehicle->m_GPSLocationVector.size(); i++ )
             {
               float distance = g_GetPoint2LineDistance(pVehicle->m_GPSLocationVector[i], pfrom, pto);  // go through each GPS location point
               if(distance < min_distance)
               {
                  min_distance = distance;
               }
             }
             // determine the distance to GPS traces as length
             (*iLink)->m_Length = min_distance; // keep the original link length
           }
           m_RandomRoutingCoefficient  = 0;  // no random cost
           //build physical network
           m_pNetwork->BuildPhysicalNetwork(&m_NodeSet, &m_LinkSet, m_RandomRoutingCoefficient, false);
           int NodeNodeSum = 0;
           int PathLinkList[MAX_NODE_SIZE_IN_A_PATH];//save link ids in a path(a path means the trajactory of a vehicle obtained from GPS)
           float TotalCost;
           bool distance_flag = true;
           int  NodeSize;
           //get the total number of the nodes in the network we build,and give the link ids to PathLinkList
           NodeSize= m_pNetwork->SimplifiedTDLabelCorrecting_DoubleQueue(m_OriginNodeID, 0, m_DestinationNodeID, 1, 10.0f,PathLinkList,TotalCost, distance_flag, false, false,m_RandomRoutingCoefficient);   // Pointer to previous node (node)    
           //update m_PathDisplayList
           if(NodeSize <= 1)
           {
              TRACE("error");
           }
            DTAPath path_element;
            path_element.Init (NodeSize-1,g_Simulation_Time_Horizon);

            std::vector<int> link_vector;// a vector to save link ids
            for (int i=0 ; i < NodeSize-1; i++)
            {
                path_element.m_LinkVector [i] = PathLinkList[i] ; //starting from m_NodeSizeSP-2, to 0
                link_vector.push_back (PathLinkList[i]);// save the link ids
                DTALink* pLink = m_LinkNotoLinkMap[PathLinkList[i]];
                if(pLink!=NULL)
                { 
                    path_element.m_Distance += m_LinkNotoLinkMap[PathLinkList[i]]->m_Length ;
                    pLink->m_bIncludedBySelectedPath = true; // mark this link as a link along the selected path
                    if(i==0) // first link
                    {
                        path_element.m_TravelTime = pLink->GetSimulatedTravelTime(g_Simulation_Time_Stamp);
                    }
                    else
                      {
                        path_element.m_TravelTime = path_element.m_TravelTime + pLink->GetSimulatedTravelTime(path_element.m_TravelTime);
                      }
                }
           }
            HighlightPath(link_vector,m_PathDisplayList.size()); //highlight the path for each vehicle in different color.
            m_PathDisplayList.push_back(path_element);
 
            // do not know how to use link_id to find from and to nodes of link 
            
            for(int i=0;i<link_vector.size();i++) 
            {
                 int index=0;
                 int link_id=link_vector[i]; //link id
                 for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)  // for each link in this network
                 {
                        if(index==link_id)// find from and to nodes for the link
                        {
                              GDPoint LinkFromNode = (*iLink)->m_FromPoint;
                              GDPoint LinkToNode = (*iLink)->m_ToPoint;
//                              fprintf(st_meta_data, "%d,%d,%f,%f,%f,%f\n", vehicle_id,link_id,LinkFromNode.x,LinkFromNode.y,LinkToNode.x,LinkToNode.y);
                        }
                        index +=1;
                 }
            }
//            vehicle_id +=1;// update vehicle_id
        }
     }
     //fclose(st_meta_data);
}