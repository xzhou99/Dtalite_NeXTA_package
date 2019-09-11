//TLiteDoc.cpp : implementation of the CTLiteDoc class
//Portions Copyright 2012 Xuesong Zhou (xzhou99@gmail.com), Jeff Taylor (jeffrey.taylor.d@gmail.com)

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
#include "..//TLiteView.h"
#include "..//TLiteView.h"
#include "SafetyPlanning.h"

void CTLiteDoc::IdentifyLinkGroupCode()
{

	std::list<DTALink*>::iterator iLink;


	// set default values for all freeway and ramp links

	for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
	{
		
		DTALink* pLink = (*iLink);

		//freeway link
		if(m_LinkTypeMap[pLink->m_link_type].IsFreeway ())
		{

		//default;
		if(pLink->group_1_code.size() ==0)
		{
		pLink->group_1_code = "non_interchange";
		}

		CString number_of_lanes_str;

		int number_of_lanes = min(4,max(2,pLink->m_NumberOfLanes));  // the lookup table has values available between 2 and 4 lanes
		
		number_of_lanes_str.Format ("%d", number_of_lanes);

		pLink->group_2_code = CString2StdString(number_of_lanes_str);
		}

		// ramp link
		if(m_LinkTypeMap[pLink->m_link_type].IsRamp ())
		{
		//default;
			if(pLink->group_1_code.size() ==0)
			{
				pLink->group_1_code = "ramp";
			}

			if(pLink->group_2_code.size() ==0)
			{
			pLink->group_2_code = "diamond";
			}
		}


		
	} 

			// identify the group code 
		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{
		
		DTALink* pLink = (*iLink);


		if(m_LinkTypeMap[pLink->m_link_type].IsFreeway ())  // scan all freeway link
		{


			if(pLink->m_FromNodeNumber == 10307 && pLink->m_ToNodeNumber == 10306)
			{
			
			TRACE("");
			}
		for(int incoming_link = 0; incoming_link <  m_NodeNoMap[pLink->m_FromNodeID ]->m_IncomingLinkVector.size(); incoming_link++) // one outgoing link without considering u-turn
		{
			int incoming_link_id = m_NodeNoMap[pLink->m_FromNodeID ]->m_IncomingLinkVector[incoming_link];
				
			
			//incoming ramp exits
			if (m_LinkTypeMap[m_LinkNotoLinkMap[incoming_link_id]->m_link_type].IsRamp ())
				{
				
//					if(m_LinkNotoLinkMap[incoming_link_id]->group_1_code.size() == 0) // only overwrite fields if they are empty
//					{
					m_LinkNotoLinkMap[incoming_link_id]->group_1_code = "onramp";
//					}

		 // overwrite this code when the existing field is empty, so the user can specify other ramp types in input_link.csv
					if( m_LinkNotoLinkMap[incoming_link_id]->group_2_code.size()==0) 
					{
					m_LinkNotoLinkMap[incoming_link_id]->group_2_code = "diamond";
					}
					
					// so this link is identified as an interchange
					pLink->group_1_code = "interchange";

					break;
				
				}


		
		}

		for(int outgoing_link = 0; outgoing_link <  m_NodeNoMap[pLink->m_ToNodeID ]->m_OutgoingLinkVector.size(); outgoing_link++) // one outgoing link without considering u-turn
		{
			int outgoing_link_id = m_NodeNoMap[pLink->m_ToNodeID ]->m_OutgoingLinkVector[outgoing_link];
				if (m_LinkNotoLinkMap.find(outgoing_link_id)!= m_LinkNotoLinkMap.end() && m_LinkTypeMap[m_LinkNotoLinkMap[outgoing_link_id]->m_link_type].IsRamp ())
				{
				
					// outgoing ramp from a freeway link
//					if(m_LinkNotoLinkMap[outgoing_link_id]->group_1_code.size() == 0) // only overwrite fields if they are empty
//					{
					m_LinkNotoLinkMap[outgoing_link_id]->group_1_code = "offramp";
//					}

			 // overwrite this code when the existing field is empty, so the user can specify other ramp types in input_link.csv
					if(m_LinkNotoLinkMap[outgoing_link_id]->group_2_code.size() == 0)
					{
					m_LinkNotoLinkMap[outgoing_link_id]->group_2_code = "diamond";
					}

					// so this link is identified as an interchange
					pLink->group_1_code = "interchange";

					break;
				
				}
		
		}

		} // for all freeway links

		}

}


void CTLiteDoc::OnSafetyplanningtoolsRun()
{
	CWaitCursor wait;
	IdentifyLinkGroupCode();

	// step 0: copy input_crash_prediction_model.csv from default data folder if the data does not exist
	CopyFileFromDefaultDataFolder("input_crash_prediction_model.csv");
	DTASafetyPredictionModel SafePredictionModel;


	// step 1: read input_crash_prediction_model.csv
	CString file_input_str = m_ProjectDirectory +"input_crash_prediction_model.csv";

	CCSVParser parser;
	if (parser.OpenCSVFile(CString2StdString(file_input_str)))
	{
		int i=0;
		while(parser.ReadRecord())
		{

			CrashPredictionModel model;
			if(parser.GetValueByFieldName("model_id",model.model_id) == false)
				break;

			if(parser.GetValueByFieldName("facility_type",model.facility_type) == false)
				break;
			
			parser.GetValueByFieldName("severity_level",model.severity_level );
			parser.GetValueByFieldName("urban_flag",model.urban_flag);
			parser.GetValueByFieldName("group_1_code",model.group_1_code);


			TRACE("\n id: %d,%s,%s," , model.model_id, model.facility_type.c_str (), model.group_1_code.c_str ());
			parser.GetValueByFieldName("group_2_code",model.group_2_code);
			parser.GetValueByFieldName("group_3_code",model.group_3_code);
			parser.GetValueByFieldName("coefficient_a",model.coefficient_a);
			parser.GetValueByFieldName("coefficient_b",model.coefficient_b);
			parser.GetValueByFieldName("coefficient_c",model.coefficient_c);
			parser.GetValueByFieldName("directional_flag",model.directional_flag);

			// add model into the crash model vector

			SafePredictionModel.CrashModelVector.push_back (model);

		}
	}

	//



	m_bRunCrashPredictionModel = true;

	CCSVWriter SafetyPredictionFile;

	CString file_str = m_ProjectDirectory +"output_crash_prediction.csv";

	 // Convert a TCHAR string to a LPCSTR
	  CT2CA pszConvertedAnsiString (file_str);

	  // construct a std::string using the LPCSTR input
	  std::string strStd (pszConvertedAnsiString);

	  double total_volume  = 0;
	  double max_number_of_crashes_per_mile = 0;
	 if(SafetyPredictionFile.Open(strStd))
	 {


	SafetyPredictionFile.SetFieldName ("node_id");

	SafetyPredictionFile.SetFieldName("link_id");
	SafetyPredictionFile.SetFieldName("from_node");
	SafetyPredictionFile.SetFieldName("to_node");
	SafetyPredictionFile.SetFieldName("length_mi");
	SafetyPredictionFile.SetFieldName("sim_volume");
	SafetyPredictionFile.SetFieldName("aadt");

	SafetyPredictionFile.SetFieldName("link_type_name");
	SafetyPredictionFile.SetFieldName("group_1_code");
	SafetyPredictionFile.SetFieldName("group_2_code");
	SafetyPredictionFile.SetFieldName("group_3_code");

	SafetyPredictionFile.SetFieldName("all_crash");
	SafetyPredictionFile.SetFieldName("fi_crash");
	SafetyPredictionFile.SetFieldName("pdo_crash");

	SafetyPredictionFile.SetFieldName("all_crash_pm");
	SafetyPredictionFile.SetFieldName("pdo_crash_pm");

	SafetyPredictionFile.SetFieldName("coefficient_a");
	SafetyPredictionFile.SetFieldName("coefficient_b");
	SafetyPredictionFile.SetFieldName("coefficient_c");


	SafetyPredictionFile.SetFieldName("seg_crash");
	SafetyPredictionFile.SetFieldName("int_crash");
	//SafetyPredictionFile.SetFieldName("fi_crash");
	//SafetyPredictionFile.SetFieldName("pdo_crash");
	SafetyPredictionFile.SetFieldName("seg_fi");
	SafetyPredictionFile.SetFieldName("seg_pdo");
	SafetyPredictionFile.SetFieldName("seg_sv_fi");
	SafetyPredictionFile.SetFieldName("seg_sv_pdo");
	SafetyPredictionFile.SetFieldName("seg_mv_fi");
	SafetyPredictionFile.SetFieldName("seg_mv_pdo");
	SafetyPredictionFile.SetFieldName("seg_dvw_fi");
	SafetyPredictionFile.SetFieldName("seg_dvw_pd");
	SafetyPredictionFile.SetFieldName("int_fi");
	SafetyPredictionFile.SetFieldName("int_pdo");
	SafetyPredictionFile.SetFieldName("int_3st");
	SafetyPredictionFile.SetFieldName("int_3sg");
	SafetyPredictionFile.SetFieldName("int_4st");
	SafetyPredictionFile.SetFieldName("int_4sg");
	SafetyPredictionFile.SetFieldName("int_3st_pd");
	SafetyPredictionFile.SetFieldName("int_3sg_pd");
	SafetyPredictionFile.SetFieldName("int_4st_pd");
	SafetyPredictionFile.SetFieldName("int_4sg_pd");
	SafetyPredictionFile.SetFieldName("int_3st_fi");
	SafetyPredictionFile.SetFieldName("int_3sg_fi");
	SafetyPredictionFile.SetFieldName("int_4st_fi");
	SafetyPredictionFile.SetFieldName("int_4sg_fi");

	SafetyPredictionFile.SetFieldName("seg_crash_pm");
	SafetyPredictionFile.SetFieldName("int_crash_pm");
	SafetyPredictionFile.SetFieldName("fi_crash_pm");
	SafetyPredictionFile.SetFieldName("seg_fi_pm");
	SafetyPredictionFile.SetFieldName("seg_pdo_pm");
	SafetyPredictionFile.SetFieldName("seg_sv_fi_pm");
	SafetyPredictionFile.SetFieldName("seg_sv_pdo_pm");
	SafetyPredictionFile.SetFieldName("seg_mv_fi_pm");
	SafetyPredictionFile.SetFieldName("seg_mv_pdo_pm");
	SafetyPredictionFile.SetFieldName("seg_dvw_fi_pm");
	SafetyPredictionFile.SetFieldName("seg_dvw_pd_pm");
	SafetyPredictionFile.SetFieldName("int_fi_pm");
	SafetyPredictionFile.SetFieldName("int_pdo_pm");
	SafetyPredictionFile.SetFieldName("int_3st_pm");
	SafetyPredictionFile.SetFieldName("int_3sg_pm");
	SafetyPredictionFile.SetFieldName("int_4st_pm");
	SafetyPredictionFile.SetFieldName("int_4sg_pm");
	SafetyPredictionFile.SetFieldName("int_3st_pd_pm");
	SafetyPredictionFile.SetFieldName("int_3sg_pd_pm");
	SafetyPredictionFile.SetFieldName("int_4st_pd_pm");
	SafetyPredictionFile.SetFieldName("int_4sg_pd_pm");
	SafetyPredictionFile.SetFieldName("int_3st_fi_pm");
	SafetyPredictionFile.SetFieldName("int_3sg_fi_pm");
	SafetyPredictionFile.SetFieldName("int_4st_fi_pm");
	SafetyPredictionFile.SetFieldName("int_4sg_fi_pm");
	SafetyPredictionFile.SetFieldName("geometry");

	SafetyPredictionFile.SetFieldName("all_crash_pv");

	 SafetyPredictionFile.WriteHeader();


			std::list<DTALink*>::iterator iLink;

		for (iLink = m_LinkSet.begin(); iLink != m_LinkSet.end(); iLink++)
		{

		float coefficient_a=0; 
		float coefficient_b=0; 
		float coefficient_c=0;
		
		DTALink* pLink = (*iLink);

		pLink->m_AADT = pLink->m_total_link_volume/max(pLink->m_AADT_conversion_factor,0.0001) ;
		total_volume+= pLink->m_AADT;

		double CrashRate= 0;
		//if( m_LinkTypeMap[pLink->m_link_type].IsFreeway ())  // freeway
		//{
	

				if (pLink->m_FromNodeNumber == 12352 && pLink->m_ToNodeNumber == 14966)
				{
					TRACE("");
				}

				std::string facility_type, severity_level, urban_flag,  group_1_code, group_2_code, group_3_code;

				if(m_LinkTypeMap[pLink->m_link_type].IsFreeway ())
				{
					facility_type = "freeway";
				}

				if(m_LinkTypeMap[pLink->m_link_type].IsRamp  ())
				{
					facility_type = "ramp";
				}
				if(m_LinkTypeMap[pLink->m_link_type].IsArterial ()  ||  m_LinkTypeMap[pLink->m_link_type].IsConnector () || m_LinkTypeMap[pLink->m_link_type].IsHighway ())
				{
					facility_type = "arterial";
				}

				urban_flag = "urban";




		if( m_LinkTypeMap[pLink->m_link_type].IsArterial () || m_LinkTypeMap[pLink->m_link_type].IsHighway ())   //arterial (highway is also arterial)
		{

		// gather two-way data
		// sum of two-way AADT
		// make prediction
		// split crash statistics to two different directions, according to AADT
			float two_way_AADT = pLink->m_AADT;
			float crash_ratio_on_this_link = 1.0f;  // default value

			DTALink* pReversedLink = NULL;

			unsigned long ReversedLinkKey = GetLinkKey(pLink->m_ToNodeID, pLink->m_FromNodeID);

			int reversed_link_id = 0;
			if ( m_NodeNotoLinkMap.find ( ReversedLinkKey) != m_NodeNotoLinkMap.end())
			{
				  pReversedLink = m_NodeNotoLinkMap[ReversedLinkKey];
			}

	
			if(pReversedLink != NULL)
			{ 
				two_way_AADT += pReversedLink->m_AADT;
				crash_ratio_on_this_link =  pLink->m_AADT/max(1,two_way_AADT);
			}


			pLink->m_number_of_all_crashes  = 
				 SafePredictionModel.EstimateArterialCrashRatePerYear(crash_ratio_on_this_link, 
				 pLink->m_num_of_fatal_and_injury_crashes_per_year, 
				 pLink->m_num_of_PDO_crashes_per_year,
				 pLink->m_Intersection_NumberOfCrashes, 
				 pLink->m_Intersection_NumberOfFatalAndInjuryCrashes, 
				 pLink->m_Intersection_NumberOfPDOCrashes,
				two_way_AADT, 
				pLink->m_Length,
				pLink->m_Num_Driveways_Per_Mile,
				pLink->m_volume_proportion_on_minor_leg,
				pLink->m_Num_3SG_Intersections,
				pLink->m_Num_3ST_Intersections,
				pLink->m_Num_4SG_Intersections,
				pLink->m_Num_4ST_Intersections);

		}else
		{

				if (pLink->m_FromNodeNumber == 1 && pLink->m_ToNodeNumber == 2873)
				{
					TRACE("");
				}

			std::string severity_level_FI = "FI";
			pLink->m_num_of_fatal_and_injury_crashes_per_year  = SafePredictionModel.EstimateCrashRatePerYear(facility_type,severity_level_FI,urban_flag,
				pLink->group_1_code , pLink->group_2_code ,pLink->group_3_code ,			
				pLink->m_AADT , pLink->m_Length, coefficient_a, coefficient_b,coefficient_c);

			std::string severity_level_TOT = "TOT";
			pLink->m_number_of_all_crashes  = SafePredictionModel.EstimateCrashRatePerYear(facility_type,severity_level_TOT,urban_flag,
				pLink->group_1_code , pLink->group_2_code ,pLink->group_3_code ,			
				pLink->m_AADT , pLink->m_Length ,coefficient_a, coefficient_b,coefficient_c);





			pLink->m_num_of_PDO_crashes_per_year = pLink->m_number_of_all_crashes - pLink->m_num_of_fatal_and_injury_crashes_per_year;

		//}	
		
		}
		

	SafetyPredictionFile.SetValueByFieldName("link_id",pLink->m_LinkID );

	SafetyPredictionFile.SetValueByFieldName("from_node",pLink->m_FromNodeNumber );
	SafetyPredictionFile.SetValueByFieldName("to_node",pLink->m_ToNodeNumber );
	SafetyPredictionFile.SetValueByFieldName("length_mi",pLink->m_Length );
	SafetyPredictionFile.SetValueByFieldName("sim_volume",pLink->m_total_link_volume);
	SafetyPredictionFile.SetValueByFieldName("aadt",pLink->m_AADT );


	CString link_type_name = " ";

	if(m_LinkTypeMap.find(pLink->m_link_type) != m_LinkTypeMap.end())
	{
	link_type_name = m_LinkTypeMap[pLink->m_link_type].link_type_name.c_str ();
	}

	SafetyPredictionFile.SetValueByFieldName("link_type_name",link_type_name );
	SafetyPredictionFile.SetValueByFieldName("group_1_code",pLink->group_1_code );
	SafetyPredictionFile.SetValueByFieldName("group_2_code",pLink->group_2_code );
	SafetyPredictionFile.SetValueByFieldName("group_3_code",pLink->group_3_code );

	SafetyPredictionFile.SetValueByFieldName("all_crash",pLink->m_number_of_all_crashes);
	SafetyPredictionFile.SetValueByFieldName("fi_crash", pLink->m_num_of_fatal_and_injury_crashes_per_year);
	SafetyPredictionFile.SetValueByFieldName("pdo_crash",pLink->m_num_of_PDO_crashes_per_year);

	SafetyPredictionFile.SetValueByFieldName("coefficient_a",coefficient_a);
	SafetyPredictionFile.SetValueByFieldName("coefficient_b",coefficient_b);
	SafetyPredictionFile.SetValueByFieldName("coefficient_c",coefficient_c);


	double all_crash_pm = pLink->m_number_of_all_crashes/ max(0.0001,pLink->m_Length );
	SafetyPredictionFile.SetValueByFieldName("all_crash_pm",all_crash_pm);

	if (max_number_of_crashes_per_mile < all_crash_pm)
	{
	max_number_of_crashes_per_mile = all_crash_pm;
	}

	double ft_crash_pm = pLink->m_num_of_fatal_and_injury_crashes_per_year/ max(0.0001,pLink->m_Length );
	double pdo_crash_pm = pLink->m_num_of_PDO_crashes_per_year/ max(0.0001,pLink->m_Length );
	
	SafetyPredictionFile.SetValueByFieldName("pdo_crash_pm",pdo_crash_pm);
	SafetyPredictionFile.SetValueByFieldName("fi_crash_pm",ft_crash_pm);

	double seg_crash = (SafePredictionModel.Nsv_total + SafePredictionModel.Nmvnd_total + SafePredictionModel.Nmvd_total);
	SafetyPredictionFile.SetValueByFieldName("seg_crash", seg_crash );
	SafetyPredictionFile.SetValueByFieldName("int_crash", pLink->m_Intersection_NumberOfCrashes);


	double seg_fi = (SafePredictionModel.Nsv_fatal_injury + SafePredictionModel.Nmvnd_fatal_injury + SafePredictionModel.Nmvd_fatal_injury);
	SafetyPredictionFile.SetValueByFieldName("seg_fi",seg_fi);

	double seg_pdo = (SafePredictionModel.Nsv_PDO + SafePredictionModel.Nmvnd_PDO + SafePredictionModel.Nmvd_PDO);
	SafetyPredictionFile.SetValueByFieldName("seg_pdo",seg_pdo);

	SafetyPredictionFile.SetValueByFieldName("seg_sv_fi",SafePredictionModel.Nsv_fatal_injury);
	SafetyPredictionFile.SetValueByFieldName("seg_sv_pdo",SafePredictionModel.Nsv_PDO);
	SafetyPredictionFile.SetValueByFieldName("seg_mv_fi",SafePredictionModel.Nmvd_fatal_injury);
	SafetyPredictionFile.SetValueByFieldName("seg_mv_pdo",SafePredictionModel.Nmvd_PDO);
	SafetyPredictionFile.SetValueByFieldName("seg_dvw_fi",SafePredictionModel.Nmvnd_fatal_injury);
	SafetyPredictionFile.SetValueByFieldName("seg_dvw_pd",SafePredictionModel.Nmvnd_PDO);

	double int_3st = SafePredictionModel.Nmv_total_3ST + SafePredictionModel.Nsv_total_3ST;
	SafetyPredictionFile.SetValueByFieldName("int_3st",int_3st);
	double int_3sg = SafePredictionModel.Nmv_total_3SG + SafePredictionModel.Nsv_total_3SG;
	SafetyPredictionFile.SetValueByFieldName("int_3sg",int_3sg);
	double int_4st = SafePredictionModel.Nmv_total_4ST + SafePredictionModel.Nsv_total_4ST;
	SafetyPredictionFile.SetValueByFieldName("int_4st",int_4st);
	double int_4sg = SafePredictionModel.Nmv_total_4SG + SafePredictionModel.Nsv_total_4SG;
	SafetyPredictionFile.SetValueByFieldName("int_4sg",int_4sg);

	double int_3st_pd = SafePredictionModel.Nmv_PDO_3ST + SafePredictionModel.NSv_PDO_3ST;
	SafetyPredictionFile.SetValueByFieldName("int_3st_pd",int_3st_pd);
	double int_3sg_pd = SafePredictionModel.Nmv_PDO_3SG + SafePredictionModel.Nsv_PDO_3SG;
	SafetyPredictionFile.SetValueByFieldName("int_3sg_pd",int_3sg_pd);
	double int_4st_pd = SafePredictionModel.Nmv_PDO_4ST + SafePredictionModel.NSv_PDO_4ST;
	SafetyPredictionFile.SetValueByFieldName("int_4st_pd",int_4st_pd);
	double int_4sg_pd = SafePredictionModel.Nmv_PDO_4SG + SafePredictionModel.Nsv_PDO_4SG;
	SafetyPredictionFile.SetValueByFieldName("int_4sg_pd",int_4sg_pd);

	double int_pdo = int_3st_pd + int_3sg_pd + int_4st_pd + int_4sg_pd;
	SafetyPredictionFile.SetValueByFieldName("int_pdo",int_pdo);

	double int_3st_fi = SafePredictionModel.Nmv_fatal_injury_3ST + SafePredictionModel.Nsv_fatal_injury_3ST;
	SafetyPredictionFile.SetValueByFieldName("int_3st_fi",int_3st_fi);
	double int_3sg_fi = SafePredictionModel.Nmv_fatal_injury_3SG + SafePredictionModel.Nsv_fatal_injury_3SG;
	SafetyPredictionFile.SetValueByFieldName("int_3sg_fi",int_3sg_fi);
	double int_4st_fi = SafePredictionModel.Nmv_fatal_injury_4ST + SafePredictionModel.Nsv_fatal_injury_4ST;
	SafetyPredictionFile.SetValueByFieldName("int_4st_fi",int_4st_fi);
	double int_4sg_fi = SafePredictionModel.Nmv_fatal_injury_4SG + SafePredictionModel.Nsv_fatal_injury_4SG;
	SafetyPredictionFile.SetValueByFieldName("int_4sg_fi",int_4sg_fi);

	double int_fi = int_3st_fi + int_3sg_fi + int_4st_fi + int_4sg_fi;
	SafetyPredictionFile.SetValueByFieldName("int_fi",int_fi);



	double all_crash_pv =  pLink->m_number_of_all_crashes/ max(0.0001,pLink->m_Length )/ max(1,pLink->m_AADT);
	SafetyPredictionFile.SetValueByFieldName("all_crash_pv",all_crash_pv);


	//SafetyPredictionFile.SetValueByFieldName("seg_crash_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_crash_pm");
	
	//SafetyPredictionFile.SetValueByFieldName("seg_fi_pm");
	//SafetyPredictionFile.SetValueByFieldName("seg_pdo_pm");
	//SafetyPredictionFile.SetValueByFieldName("seg_sv_fi_pm");
	//SafetyPredictionFile.SetValueByFieldName("seg_sv_pdo_pm");
	//SafetyPredictionFile.SetValueByFieldName("seg_mv_fi_pm");
	//SafetyPredictionFile.SetValueByFieldName("seg_mv_pdo_pm");
	//SafetyPredictionFile.SetValueByFieldName("seg_dvw_fi_pm");
	//SafetyPredictionFile.SetValueByFieldName("seg_dvw_pd_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_fi_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_pdo_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_3st_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_3sg_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_4st_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_4sg_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_3st_pd_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_3sg_pd_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_4st_pd_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_4sg_pd_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_3st_fi_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_3sg_fi_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_4st_fi_pm");
	//SafetyPredictionFile.SetValueByFieldName("int_4sg_fi_pm");

	std::string  geo_string = "\"" + pLink->m_geo_string + "\"";

	SafetyPredictionFile.SetValueByFieldName("geometry",geo_string);

	SafetyPredictionFile.WriteRecord ();

		}  // for each link
		}
		

		if(total_volume<0.1)
		{
		
		AfxMessageBox("Simulated link volume data are not available.\nPlease run simulation to generate volume results.");
		return;
		}


	m_LOSBound[MOE_safety][1] = max_number_of_crashes_per_mile*1/7.0;
	m_LOSBound[MOE_safety][2] =  max_number_of_crashes_per_mile*2/7.0;
	m_LOSBound[MOE_safety][3] = max_number_of_crashes_per_mile*3/7.0;;
	m_LOSBound[MOE_safety][4] = max_number_of_crashes_per_mile*4/7.0;;
	m_LOSBound[MOE_safety][5] = max_number_of_crashes_per_mile*5/7.0;;
	m_LOSBound[MOE_safety][6] = max_number_of_crashes_per_mile*6/7.0;;
	m_LOSBound[MOE_safety][7] =  max_number_of_crashes_per_mile*7/7.0;



		CString message;
		message.Format("Crash prediction model completes for %d links using %d crash prediction models.",m_LinkSet.size(), SafePredictionModel.CrashModelVector .size());

		AfxMessageBox(message, MB_ICONINFORMATION);
}


void CTLiteDoc::OnSafetyplanningtoolsGeneratenode()
{
	CString csv_file_str = m_ProjectDirectory +"output_crash_prediction.csv";
	CString shape_file_str = m_ProjectDirectory +"output_crash_prediction.shp";

	DeleteFile(shape_file_str);
	ConvertLinkCSV2ShapeFiles(csv_file_str,shape_file_str, "ESRI Shapefile",GIS_Line_Type);

	AfxMessageBox("output_crash_prediction.shp is generated.",MB_ICONINFORMATION);

}