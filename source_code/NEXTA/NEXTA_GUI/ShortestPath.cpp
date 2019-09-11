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

//shortest path calculation

// note that the current implementation is only suitable for time-dependent minimum time shortest path on FIFO network, rather than time-dependent minimum cost shortest path
// the key reference (1) Shortest Path Algorithms in Transportation Models http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.51.5192
// (2) most efficient time-dependent minimum cost shortest path algorithm for all departure times
// Time-dependent, shortest-path algorithm for real-time intelligent vehicle highway system applications&quot;, Transportation Research Record 1408 ?Ziliaskopoulos, Mahmassani - 1993

#include "stdafx.h"
#include "Network.h"
extern long g_Simulation_Time_Horizon;


void DTANetworkForSP::BuildPhysicalNetwork(std::list<DTANode*>*	p_NodeSet, std::list<DTALink*>*		p_LinkSet, float RandomCostCoef,bool bOverlappingCost,  int OriginNodeID, int DestinationNodeID)
{

	// build a network from the current zone centroid (1 centroid here) to all the other zones' centroids (all the zones)
	float Perception_error_ratio = 0.7f;

	std::list<DTANode*>::iterator iterNode;
	std::list<DTALink*>::iterator iterLink;

	m_NodeSize = p_NodeSet->size();

	int FromID, ToID;

	int i;

	for(i=0; i< m_NodeSize; i++)
	{
		m_OutboundSizeAry[i] = 0;
		m_InboundSizeAry[i] = 0;

	}

	// add physical links

	for(iterLink = p_LinkSet->begin(); iterLink != p_LinkSet->end(); iterLink++)
	{

		FromID = (*iterLink)->m_FromNodeID;
		ToID   = (*iterLink)->m_ToNodeID;

		if (FromID == 19)
		{
		TRACE("");
		}

		if ((*iterLink)->m_bConnector || (*iterLink)->m_bTransit || (*iterLink)->m_bWalking)  // no connectors: here we might have some problems here, as the users cannot select a zone centroid as origin/destination
		{
			if(FromID!=OriginNodeID && ToID !=DestinationNodeID)  // if not the first link or last link, skip
				continue; 
		}

		if((*iterLink)->m_AdditionalCost >1)  // skip prohibited links (defined by users)
			continue;

		int link_type = (*iterLink)->m_link_type ;


		m_FromIDAry[(*iterLink)->m_LinkNo] = FromID;
		m_ToIDAry[(*iterLink)->m_LinkNo]   = ToID;

		//      TRACE("FromID %d -> ToID %d \n", FromID, ToID);
		m_OutboundNodeAry[FromID][m_OutboundSizeAry[FromID]] = ToID;
		m_OutboundLinkAry[FromID][m_OutboundSizeAry[FromID]] = (*iterLink)->m_LinkNo ;
		m_OutboundSizeAry[FromID] +=1;

		m_InboundLinkAry[ToID][m_InboundSizeAry[ToID]] = (*iterLink)->m_LinkNo  ;
		m_InboundSizeAry[ToID] +=1;

		m_LinkTDTimeAry[(*iterLink)->m_LinkNo][0] = (*iterLink)->m_Length + (*iterLink)->m_AdditionalCost;
		m_LinkTDCostAry[(*iterLink)->m_LinkNo][0]=  (*iterLink)->m_Length ;
			// use travel time now, should use cost later

	}

	m_LinkSize = p_LinkSet->size();
}





int DTANetworkForSP::SimplifiedTDLabelCorrecting_DoubleQueue(int origin, int departure_time, int destination, int pricing_type, float VOT,
															 int PathLinkList[MAX_NODE_SIZE_IN_A_PATH],float &TotalCost, bool distance_flag,bool check_connectivity_flag, bool debug_flag, float RandomCostCoef)   // Pointer to previous node (node)
// time -dependent label correcting algorithm with deque implementation
{

	float CostUpperBound = MAX_SPLABEL;

    int	temp_reversed_PathLinkList[MAX_NODE_SIZE_IN_A_PATH];
	int i;
	debug_flag = 1;

	if(m_OutboundSizeAry[origin]== 0)
		return 0;

	if(origin == destination)
		return 0;

	for(i=0; i <m_NodeSize; i++) // Initialization for all nodes
	{
		NodePredAry[i]  = -1;
		NodeStatusAry[i] = 0;

		LabelTimeAry[i] = MAX_SPLABEL;
		LabelCostAry[i] = MAX_SPLABEL;

	}
 
	// Initialization for origin node
	LabelTimeAry[origin] = float(departure_time);
	LabelCostAry[origin] = 0;

	SEList_clear();
	SEList_push_front(origin);

	int FromID, LinkNo, ToID;


	float NewTime, NewCost;
	while(!SEList_empty())
	{
		FromID  = SEList_front();
		SEList_pop_front();

		if(debug_flag)
			TRACE("\nScan from node %d",FromID);

		NodeStatusAry[FromID] = 2;        //scaned

		for(i=0; i<m_OutboundSizeAry[FromID];  i++)  // for each arc (i,j) belong A(j)
		{
			LinkNo = m_OutboundLinkAry[FromID][i];
			ToID = m_OutboundNodeAry[FromID][i];

			if(ToID == origin)
				continue;


			  TRACE("\n   to node %d",ToID);
			// need to check here to make sure  LabelTimeAry[FromID] is feasible.

			float random_value = g_RNNOF();
			if(random_value >5)
				random_value = 5;

			float random_cost = max(0.1f,m_LinkTDTimeAry[LinkNo][0]*(1+RandomCostCoef*random_value));


			NewTime	 = LabelTimeAry[FromID] + random_cost;  // time-dependent travel times come from simulator
			NewCost    = LabelCostAry[FromID] +random_cost;       // costs come from time-dependent tolls, VMS, information provisions

			if(NewCost < LabelCostAry[ToID] &&  NewCost < CostUpperBound) // be careful here: we only compare cost not time
			{

				TRACE("\n         UPDATE to %f, link travel time %f", NewCost, m_LinkTDCostAry[LinkNo][0]);

				LabelTimeAry[ToID] = NewTime;
				LabelCostAry[ToID] = NewCost;
				NodePredAry[ToID]   = FromID;
				LinkNoAry[ToID] = LinkNo;

				if (ToID == destination) // special feature 7.2: update upper bound cost
				{
					CostUpperBound = LabelCostAry[ToID];
				}

				// Dequeue implementation
				//
				if(NodeStatusAry[ToID]==2) // in the SEList_TD before
				{
					SEList_push_front(ToID);
					NodeStatusAry[ToID] = 1;
				}
				if(NodeStatusAry[ToID]==0)  // not be reached
				{
					SEList_push_back(ToID);
					NodeStatusAry[ToID] = 1;
				}

				//another condition: in the SELite now: there is no need to put this node to the SEList, since it is already there.
			}

		}      // end of for each link

	} // end of while

	if(check_connectivity_flag) 
		return 0;


		int LinkSize = 0;
		int PredNode = NodePredAry[destination];	
		temp_reversed_PathLinkList[LinkSize++] = LinkNoAry[destination];

		while(PredNode != origin && PredNode!=-1 && LinkSize< MAX_NODE_SIZE_IN_A_PATH) // scan backward in the predessor array of the shortest path calculation results
			{
				ASSERT(LinkSize< MAX_NODE_SIZE_IN_A_PATH-1);
				temp_reversed_PathLinkList[LinkSize++] = LinkNoAry[PredNode];

				PredNode = NodePredAry[PredNode];
			}
	
		int j = 0;
		for(i = LinkSize-1; i>=0; i--)
		{
		PathLinkList[j++] = temp_reversed_PathLinkList[i];
		}

		TotalCost = LabelCostAry[destination];

		if(TotalCost > MAX_SPLABEL-10)
		{
			//ASSERT(false);
			return 0;
		}

		return LinkSize+1; // as }


}


void DTANetworkForSP::BuildSpaceTimeNetworkForTimetabling(std::list<DTANode*>* p_NodeSet, std::list<DTALink*>* p_LinkSet, int TrainType)
{
	std::list<DTANode*>::iterator iterNode;
	std::list<DTALink*>::iterator iterLink;

	m_NodeSize = p_NodeSet->size();

	int FromID, ToID;

	int i,t;

	for(i=0; i< m_NodeSize; i++)
	{
		m_OutboundSizeAry[i] = 0;
		m_InboundSizeAry[i] = 0;

	}

	// add physical links

	for(iterLink = p_LinkSet->begin(); iterLink != p_LinkSet->end(); iterLink++)
	{
		FromID = (*iterLink)->m_FromNodeID;
		ToID   = (*iterLink)->m_ToNodeID;

		m_FromIDAry[(*iterLink)->m_LinkNo] = FromID;
		m_ToIDAry[(*iterLink)->m_LinkNo]   = ToID;

		m_OutboundNodeAry[FromID][m_OutboundSizeAry[FromID]] = ToID;
		m_OutboundLinkAry[FromID][m_OutboundSizeAry[FromID]] = (*iterLink)->m_LinkNo ;
		m_OutboundSizeAry[FromID] +=1;

		m_InboundLinkAry[ToID][m_InboundSizeAry[ToID]] = (*iterLink)->m_LinkNo  ;
		m_InboundSizeAry[ToID] +=1;


//		TRACE("------Link %d->%d:\n",FromID,ToID);
		ASSERT(m_AdjLinkSize > m_OutboundSizeAry[FromID]);

		for(t=0; t <m_OptimizationHorizon; t+=m_OptimizationTimeInveral)
		{
			m_LinkTDTimeAry[(*iterLink)->m_LinkNo][t] = (*iterLink)->GetTrainRunningTime(TrainType);  // in the future, we can extend it to time-dependent running time

//			TRACE("Time %d, Travel Time %f, Cost %f\n", t,m_LinkTDTimeAry[(*iterLink)->m_LinkNo][t] ,m_LinkTDCostAry[(*iterLink)->m_LinkNo][t]);

			// use travel time now, should use cost later
		}


	}

	m_LinkSize = p_LinkSet->size();
}


bool DTANetworkForSP::OptimalTDLabelCorrecting_DoubleQueue(int origin, int departure_time)
// time -dependent label correcting algorithm with deque implementation
{

	int i;
	int debug_flag = 0;  // set 1 to debug the detail information
				if(debug_flag)
				TRACE("\nCompute shortest path from %d at time %d",origin, departure_time);


	if(m_OutboundSizeAry[origin]== 0)
		return false;

	for(i=0; i <m_NodeSize; i++) // Initialization for all nodes
	{
		NodeStatusAry[i] = 0;

		for(int t=departure_time; t <m_OptimizationHorizon; t+=m_OptimizationTimeInveral)
		{
			TD_LabelCostAry[i][t] = MAX_SPLABEL;
			TD_NodePredAry[i][t] = -1;  // pointer to previous NODE INDEX from the current label at current node and time
			TD_TimePredAry[i][t] = -1;  // pointer to previous TIME INDEX from the current label at current node and time
		}

	}

	//	TD_LabelCostAry[origin][departure_time] = 0;

	int AllowedDelayTime  = 0;  // external parameter
	// Initialization for origin node at the preferred departure time, at departure time, cost = 0, otherwise, the delay at origin node

	//+1 in "departure_time + 1+ AllowedDelayTime" is to allow feasible value for t = departure time
	for(int t=departure_time; t <departure_time + 1+ AllowedDelayTime; t+=m_OptimizationTimeInveral)
	{
		TD_LabelCostAry[origin][t]= t-departure_time;
	}

	SEList_clear();
	SEList_push_front(origin);



	while(!SEList_empty())
	{
		int FromID  = SEList_front();
		SEList_pop_front();  // remove current node FromID from the SE list


		NodeStatusAry[FromID] = 2;        //scaned

		//scan all outbound nodes of the current node
		for(i=0; i<m_OutboundSizeAry[FromID];  i++)  // for each arc (i,j) belong A(j)
		{
			int LinkNo = m_OutboundLinkAry[FromID][i];
			int ToID = m_OutboundNodeAry[FromID][i];

			if(ToID == origin)  // remove possible loop back to the origin
				continue;

			if(debug_flag)
				TRACE("\nScan from node %d to node %d",FromID,ToID);

			// for each time step, starting from the departure time
			for(int t=departure_time; t <m_OptimizationHorizon; t+=m_OptimizationTimeInveral)
			{
				if(TD_LabelCostAry[FromID][t]<MAX_SPLABEL-1)  // for feasible time-space point only
				{

					for(int time_delay = 0; time_delay <=AllowedDelayTime; time_delay++)
					{
						int NewToNodeArrivalTime	 = (int)(t + m_LinkTDTimeAry[LinkNo][t] + time_delay);  // time-dependent travel times for different train type
						float NewCost  =  TD_LabelCostAry[FromID][t] + m_LinkTDCostAry[LinkNo][t] + m_LinkTDTimeAry[LinkNo][t];
						// costs come from time-dependent resource price or road toll

						if(NewToNodeArrivalTime > (m_OptimizationHorizon -1))  // prevent out of bound error
							NewToNodeArrivalTime = (m_OptimizationHorizon-1);

						if(NewCost < TD_LabelCostAry[ToID][NewToNodeArrivalTime] ) // we only compare cost at the downstream node ToID at the new arrival time t
						{

							if(debug_flag)
								TRACE("\n         UPDATE to %f, link cost %f at time %d", NewCost, m_LinkTDCostAry[LinkNo][t],NewToNodeArrivalTime);

							// update cost label and node/time predecessor

							TD_LabelCostAry[ToID][NewToNodeArrivalTime] = NewCost;
							TD_NodePredAry[ToID][NewToNodeArrivalTime] = FromID;  // pointer to previous NODE INDEX from the current label at current node and time
							TD_TimePredAry[ToID][NewToNodeArrivalTime] = t;  // pointer to previous TIME INDEX from the current label at current node and time

							// Dequeue implementation
							if(NodeStatusAry[ToID]==2) // in the SEList_TD before
							{
								SEList_push_front(ToID);
								NodeStatusAry[ToID] = 1;
							}
							if(NodeStatusAry[ToID]==0)  // not be reached
							{
								SEList_push_back(ToID);
								NodeStatusAry[ToID] = 1;
							}

						}
					}
				}
				//another condition: in the SELite now: there is no need to put this node to the SEList, since it is already there.
			}

		}      // end of for each link

	} // end of while
	return true;
}


int DTANetworkForSP::FindOptimalSolution(int origin, int departure_time, int destination, DTA_Train* pTrain)  // the last pointer is used to get the node array
{

	// step 1: scan all the time label at destination node, consider time cost
	// step 2: backtrace to the origin (based on node and time predecessors)
	// step 3: reverse the backward path
	// return final optimal solution

	// step 1: scan all the time label at destination node, consider time cost
	STrainNode tmp_AryTN[MAX_NODE_SIZE_IN_A_PATH]; //backward temporal solution

	float min_cost = MAX_SPLABEL;
	int min_cost_time_index = -1;

	for(int t=departure_time; t <m_OptimizationHorizon; t+=m_OptimizationTimeInveral)
	{
		if(TD_LabelCostAry[destination][t] < min_cost)
		{
			min_cost = TD_LabelCostAry[destination][t];
			min_cost_time_index = t;
		}

	}

	ASSERT(min_cost_time_index>0); // if min_cost_time_index ==-1, then no feasible path if founded

	// step 2: backtrace to the origin (based on node and time predecessors)

	int	NodeSize = 0;

	//record the first node backward, destination node
	tmp_AryTN[NodeSize].NodeID = destination;
	tmp_AryTN[NodeSize].NodeTimestamp = min_cost_time_index;

	NodeSize++;

	int PredTime = TD_TimePredAry[destination][min_cost_time_index];
	int PredNode = TD_NodePredAry[destination][min_cost_time_index];

	while(PredNode != origin && PredNode!=-1 && NodeSize< MAX_NODE_SIZE_IN_A_PATH) // scan backward in the predessor array of the shortest path calculation results
	{
		ASSERT(NodeSize< MAX_NODE_SIZE_IN_A_PATH-1);

		tmp_AryTN[NodeSize].NodeID = PredNode;
		tmp_AryTN[NodeSize].NodeTimestamp = PredTime;

		NodeSize++;

		//record current values of node and time predecessors, and update PredNode and PredTime
		int PredTime_cur = PredTime;
		int PredNode_cur = PredNode;

		PredNode = TD_NodePredAry[PredNode_cur][PredTime_cur];
		PredTime = TD_TimePredAry[PredNode_cur][PredTime_cur];

	}

	tmp_AryTN[NodeSize].NodeID = origin;
	tmp_AryTN[NodeSize].NodeTimestamp = departure_time;
	NodeSize++;

	// step 3: reverse the backward solution

	if(pTrain->m_aryTN !=NULL)
		delete pTrain->m_aryTN;

	pTrain->m_aryTN = new STrainNode[NodeSize];

	for(int i = 0; i< NodeSize; i++)
	{
		pTrain->m_aryTN[i].NodeID			= tmp_AryTN[NodeSize-1-i].NodeID;
		pTrain->m_aryTN[i].NodeTimestamp	= tmp_AryTN[NodeSize-1-i].NodeTimestamp;
	}

	pTrain->m_ActualTripTime = pTrain->m_aryTN[NodeSize-1].NodeTimestamp - pTrain->m_DepartureTime ;

	return NodeSize;
}



bool DTANetworkForSP::GenerateSearchTree(int origin, int destination, int node_size, float TravelTimeBound)   // Pointer to previous node (node)
// time -dependent label correcting algorithm with deque implementation
{
//  breadth-first search (BFS) is a graph search algorithm that begins at the root node and explores all the neighboring nodes

// Then for each of those nearest nodes, it explores their unexplored neighbor nodes, and so on, until it finds the goal.

// compared to BFS, we continue search even a node has been marked before

// we allow a loop for simplicity

// we can add a label cost constraint

	int i;

	if(m_OutboundSizeAry[origin]== 0)
		return false;

	m_TreeListSize = 5000000;
	m_SearchTreeList = new SearchTreeElement[m_TreeListSize];

	for(i = 0; i < m_TreeListSize; i++)
	{
		m_SearchTreeList[i].CurrentNode = 0;
		m_SearchTreeList[i].SearchLevel = 0;
		m_SearchTreeList[i].TravelTime = 0;
		m_SearchTreeList[i].PredecessorNode = -1;
	}

	m_SearchTreeList[0].CurrentNode  = origin;
	m_SearchTreeList[0].SearchLevel = 0;

	m_TreeListFront = 0;
	m_TreeListTail = 1;

	int FromID, LinkNo, ToID;

	int PathNo = 0;

	while(m_TreeListTail < m_TreeListSize-1 && m_TreeListFront <  m_TreeListTail)
		// not exceed search tree size, or not search to the end of queue/list
	{
		FromID  = m_SearchTreeList[m_TreeListFront].CurrentNode ;


		if(FromID==destination || m_SearchTreeList[m_TreeListFront].SearchLevel  >= node_size || m_SearchTreeList[m_TreeListFront].TravelTime   >= TravelTimeBound)
		{

		m_TreeListFront ++; // move to the next front node for breadth first search
		continue;

		// when we finish all search, we should backtrace from a node at position i == destination)
		}

		for(i=0; i<m_OutboundSizeAry[FromID];  i++)  // for each arc (i,j) belong A(j)
		{
			LinkNo = m_OutboundLinkAry[FromID][i];
			ToID = m_OutboundNodeAry[FromID][i];

			if(ToID == origin)
				continue;

			// search if ToID in the path
			bool bToID_inSubPathFlag = false;
			{
				int Pred = m_SearchTreeList[m_TreeListFront].PredecessorNode ;
				while(Pred>0)
				{
						if(m_SearchTreeList[Pred].CurrentNode == ToID)  // in the subpath
						{
						bToID_inSubPathFlag = true;
						break;
						}

				Pred = m_SearchTreeList[Pred].PredecessorNode ;

				}
			
			}

			if(bToID_inSubPathFlag)
				continue;

			m_SearchTreeList[m_TreeListTail].CurrentNode = ToID;
			m_SearchTreeList[m_TreeListTail].PredecessorNode = m_TreeListFront;
			m_SearchTreeList[m_TreeListTail].SearchLevel = m_SearchTreeList[m_TreeListFront].SearchLevel + 1;

			float FFTT = m_LinkTDTimeAry[LinkNo][0];
			m_SearchTreeList[m_TreeListTail].TravelTime  = m_SearchTreeList[m_TreeListFront].TravelTime + FFTT;

		m_TreeListTail++;

			if(m_TreeListTail >= m_TreeListSize-1)
				break;  // the size of list is exceeded

			

		}// end of for each link

		m_TreeListFront ++; // move to the next front node for breadth first search

	} // end of while

		for(i = 0; i < m_TreeListTail; i++)
	{
		if(m_SearchTreeList[i].CurrentNode == destination)
		{
			int Pred = m_SearchTreeList[i].PredecessorNode ;

			while(Pred!=0)
			{
				Pred = m_SearchTreeList[Pred].PredecessorNode ;
			}
			PathNo++;
		}

	}

	if(m_TreeListFront ==  m_TreeListTail && m_TreeListTail < m_TreeListSize-1)
		return true;
	else 
		return false;  // has not be enumerated. 
}

