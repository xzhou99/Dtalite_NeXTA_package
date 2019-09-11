// for train timetabling
// node element of a train path
class STrainNode
{  public:
int  NodeID; 
int  RailLinkID; // starting from the second element, [i-1,i]

int TaskProcessingTime;  // the task is associated with the previous arc, // first node: origin node release time, other nodes: station entry or exit time, 1, 3, 5,: entry station, 2, 4, 6: exit station
int TaskScheduleWaitingTime;  // to be scheduled by the program
int NodeTimestamp;  // to be calculated 

};

class DTA_Train  // for train timetabling
{
public:

	int m_TrainID;  //range: +2,147,483,647
	int m_TrainType; // e.g. 1: high-speed train, 2: medium-speed train
	int m_OriginNodeNumber;  // defined from node.csv
	int m_DestinationNodeNumber; // defined from node.csv
	int m_OriginNodeID;  // internal node id, for shortest path
	int m_DestinationNodeID; // internal node id, for shortest path
	int m_DepartureTime;   // given input
	int m_PreferredArrivalTime;  // determined by the schedule
	int m_ActualTripTime;  // determined by the schedule
	int m_NodeSize;        // initial value could be 0, given from extern input or calculated from shortest path algorithm
	STrainNode *m_aryTN; // node list arrary of a vehicle path


	DTA_Train()
	{
		m_NodeSize = 0;
		m_aryTN = NULL;
	}
	~DTA_Train()
	{
		if(m_aryTN != NULL)
			delete m_aryTN;
	}

};

class SResource
{  
public:
	int UsageCount;
	float Price;
	int LastUseIterationNo;

	SResource()
	{
		UsageCount = 0;
		Price = 0;
		LastUseIterationNo = -100;
	}


};