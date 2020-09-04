extern int no_zones, no_nodes, no_links, FirstThruNode;
extern struct link_record* Link;
/* Gloabal variables */

my_float** ODflow, TotalODflow;

/* Local Declarations */
/* void FW(void); Should there be a function for fw, or should it be included in main? */
static void Init();
static void Close();
static void InitODflow(void);
static void CloseODflow(void);
/* End of local declarations. */

void main(int argc, char** argv)
{
	my_float* MainVolume, * SubVolume, * SDVolume, Lambda;
	int** MinPathPredLink;
	int** MinPathQueueNext;
	double** MinPathCostTo;

	struct fw_status_struct fw_status;

	no_zones = 0;
	no_nodes = 0;
	no_links = 0;
	FirstThruNode = 0;  // this parameter is used to prevent the zone centriods from entering the the scan eligible list 

	CCSVParser parser_node;
	int node_id;
	int zone_id;
	if (parser_node.OpenCSVFile("node.csv", true))
	{
		while (parser_node.ReadRecord())  // if this line contains [] mark, then we will also read field headers.
		{
			zone_id = -1;

			parser_node.GetValueByFieldName("node_id", node_id);
			parser_node.GetValueByFieldName("zone_id", zone_id);
			if (zone_id <= 0 && FirstThruNode == 0)
			{
				FirstThruNode = node_id;
			}

			if (zone_id > no_zones)
			{
				no_zones = zone_id;
			}
			no_nodes++;

		}
		parser_node.CloseCSVFile();
	}

	cout << "read  " << no_nodes << " nodes; " << no_zones << " zones" << endl;
	cout << "FirstThruNode =  " << FirstThruNode << endl;

	CCSVParser parser_link;
	int from_node_id;

	if (parser_link.OpenCSVFile("road_link.csv", true))
	{
		while (parser_link.ReadRecord())  // if this line contains [] mark, then we will also read field headers.
		{
			parser_link.GetValueByFieldName("from_node_id", from_node_id);


			no_links++;
		}
		parser_link.CloseCSVFile();

	}


	InitLinks();
	InitODflow();

	clock_t start_t, end_t, total_t;
	start_t = clock();

	cout << "read  " << no_links << " links" << endl;

	MainVolume = (my_float*)Alloc_1D(no_links, sizeof(my_float));
	SDVolume = (my_float*)Alloc_1D(no_links, sizeof(my_float));
	SubVolume = (my_float*)Alloc_1D(no_links, sizeof(my_float)); /* Compute search direction and sub-volume in the same place. */


	MinPathPredLink = (int**)Alloc_2D(no_zones, no_nodes, sizeof(int));
	MinPathQueueNext = (int**)Alloc_2D(no_zones, no_nodes, sizeof(int));
	MinPathCostTo = (double**)Alloc_2D(no_zones, no_nodes, sizeof(double));


	for (int l = 0; l <= no_links; l++)
	{
		SubVolume[l] = 0;
		SDVolume[l] = 0;
	}
	InitFWstatus(&fw_status);


	FindMinCostRoutes(MinPathPredLink, MinPathQueueNext, MinPathCostTo, NULL);
	Assign(ODflow, MinPathPredLink, MainVolume);

	FirstFWstatus(MainVolume, &fw_status);
	UpdateLinkCost(MainVolume);

	for (fw_status.Iteration = 1; ContinueFW(fw_status); fw_status.Iteration++)
	{
		cout << "iteration: " << fw_status.Iteration << endl;

		FindMinCostRoutes(MinPathPredLink, MinPathQueueNext, MinPathCostTo, NULL);
		Assign(ODflow, MinPathPredLink, SubVolume);


		//for (int k = 1; k <= no_links; k++)
		//{
		//	cout << "assignment :" << Link[k].Tail << "->" << Link[k].Head << ":" << SubVolume[k] << endl;
		//}

		VolumeDifference(SubVolume, MainVolume, SDVolume);	/* Which yields the search direction. */

//		Lambda = LinksSDLineSearch ( MainVolume, SDVolume );
		Lambda = 1.0 / (fw_status.Iteration + 1);
		UpdateFWstatus(MainVolume, SDVolume, &fw_status);
		UpdateVolume(MainVolume, SDVolume, Lambda);
		UpdateLinkCost(MainVolume);

		clock_t iteration_t;
		end_t = clock();
		iteration_t = end_t - start_t;
		cout << "CPU time per iteration: " << iteration_t / 1000.0 / (fw_status.Iteration) << " s" << endl;

	}

	CloseFWstatus(MainVolume);


	free(MainVolume);
	free(SubVolume);
	Free_2D((void**)MinPathPredLink, no_zones, no_nodes);
	Free_2D((void**)MinPathQueueNext, no_zones, no_nodes);
	Free_2D((void**)MinPathCostTo, no_zones, no_nodes);

	clock_t iteration_t;
	end_t = clock();
	iteration_t = end_t - start_t;
	cout << "assignment CPU time " << iteration_t / 1000.0 << " s" << endl;


	cout << "Complete.  " << endl;
	getchar();
	Close();
	StatusMessage("General", "The end");
}



static void Init() {
	InitLinks();
	InitODflow();
	InitLineSearch();
}

static void Close() {
	StatusMessage("General", "Closing all modules");
	CloseLinks();
	CloseODflow();
	CloseLineSearch();
}

static void InitODflow(void) {

	double Factor = 1.0;
	ODflow = Read_ODflow(&TotalODflow, &no_zones);
	cout << "read  " << TotalODflow << " ODflow" << endl;

	Factor = 1.0;
}
