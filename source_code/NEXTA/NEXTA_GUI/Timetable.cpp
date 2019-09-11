// Timetable.cpp : implementation of timetabling algorithm in the CTLiteDoc class
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
#include "TLite.h"
#include "Network.h"
#include "TLiteDoc.h"
#include "TLiteView.h"
#define _MAX_OPTIMIZATION_HORIZON 1440

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
void CTLiteDoc::ReadTrainProfileCSVFile(LPCTSTR lpszFileName)
{
        FILE* st = NULL;
        fopen_s(&st,lpszFileName,"r");

        int i = 0;
        if(st!=NULL)
        {
                DTALink* pLink = 0;

                double default_distance_sum=0;
                double length_sum = 0;
                while(!feof(st))
                {
                        int FromNode =  g_read_integer(st);
                        if(FromNode == -1)  // reach end of file
                                break;
                        int ToNode = g_read_integer(st);

                        int m_SimulationHorizon = 1;

                        DTALink* pLink = FindLinkWithNodeNumbers(FromNode, ToNode);

                        if(pLink!=NULL)
                        {
                        int TrainType = g_read_integer(st);
                        int TrainRunningTime = g_read_integer(st);
						int TrainMaxWaitingTime =  g_read_integer(st);
                        pLink->m_RuningTimeMap[TrainType] = TrainRunningTime;
						i++;
                
                        }else
                        {
                        // to do: output error here
                        
                        }
                }

                fclose(st);
		m_LinkTrainTravelTimeDataLoadingStatus.Format ("%d train-link travel time records are loaded from file %s.",i,lpszFileName);
        }
}

bool CTLiteDoc::ReadTimetableCVSFile(LPCTSTR lpszFileName)
{
	FILE* st = NULL;
	fopen_s(&st,lpszFileName,"r");

	if(st!=NULL)
	{
		int train_no = 1;
		while(!feof(st))
		{

			int train_id =  g_read_integer(st);

			if(train_id == -1)
				break;
			DTA_Train* pTrain = new DTA_Train();
			pTrain->m_TrainID = train_id;

			pTrain->m_TrainType =  g_read_integer(st);
			pTrain->m_OriginNodeNumber =  g_read_integer(st);
			pTrain->m_DestinationNodeNumber =  g_read_integer(st);
			pTrain->m_OriginNodeID =  m_NodeNumbertoNodeNoMap[pTrain->m_OriginNodeNumber];
			pTrain->m_DestinationNodeID =  m_NodeNumbertoNodeNoMap[pTrain->m_DestinationNodeNumber ];

			pTrain->m_DepartureTime=  g_read_integer(st);
			pTrain->m_NodeSize	=  g_read_integer(st);
			pTrain->m_ActualTripTime =  g_read_integer(st);

			if(pTrain->m_NodeSize>0)
			{
				pTrain->m_aryTN = new STrainNode[pTrain->m_NodeSize];

				for(int i =0; i< pTrain->m_NodeSize; i++)
				{
					int NodeNumber = g_read_integer(st);
					pTrain->m_aryTN[i].NodeID = m_NodeNumbertoNodeNoMap[NodeNumber];
					pTrain->m_aryTN[i].NodeTimestamp = g_read_integer(st);
					g_read_float(st);  // read position number

					if(i>=1)
					{
						DTALink* pLink = FindLinkWithNodeIDs(pTrain->m_aryTN[i-1].NodeID , pTrain->m_aryTN[i].NodeID  );

						pTrain->m_aryTN[i].RailLinkID = pLink->m_LinkNo;
						if(pLink==NULL)
						{
							CString msg;
							msg.Format("train %d's link %d -> %d does not exist",pTrain->m_TrainID ,m_NodeNotoNumberMap[pTrain->m_aryTN[i-1].NodeID],m_NodeNotoNumberMap[pTrain->m_aryTN[i].NodeID]);
							AfxMessageBox(msg);
							fclose(st);
							return false;
						}

					}
				}
			}
			m_TrainVector.push_back(pTrain);

		}

		m_TimetableDataLoadingStatus.Format ("%d trains are loaded from file %s.",m_TrainVector.size(),lpszFileName);

		fclose(st);
		return true;
	}

	return false;
}


void CTLiteDoc::OnTimetableImporttimetable()
{
	CString str;
	CFileDialog dlg (FALSE, "*.csv", "*.csv",OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_LONGNAMES,
		"(*.csv)|*.csv||", NULL);
	if(dlg.DoModal() == IDOK)
	{
		char fname[_MAX_PATH];
		wsprintf(fname,"%s", dlg.GetPathName());
		CWaitCursor wait;

		if(!ReadTimetableCVSFile(fname))
		{
			str.Format("The file %s could not be opened.\nPlease check if it is opened by Excel.", fname);
			AfxMessageBox(str);
		}
	}
}



