

class CrashPredictionModel
{
public:

	CrashPredictionModel()
	{
		urban_flag = 1;
	
		directional_flag = 1;
	}
	
	int model_id;

	// the reason we use string as the following types is to ensure sufficent flexbility in safety planing model coding
	std::string facility_type;
	std::string severity_level;
	std::string urban_flag;

	std::string group_1_code; 
	std::string group_2_code; 
	std::string group_3_code; 

	float coefficient_a;
	float coefficient_b;
	float coefficient_c;

	int directional_flag;


};
class DTASafetyPredictionModel
{
public:


	std::vector<CrashPredictionModel>  CrashModelVector;

	double EstimateCrashRatePerYear(std::string facility_type, std::string severity_level,std::string urban_flag, 
		std::string group_1_code,
		std::string group_2_code,
		std::string group_3_code,
		float AADT, float link_length, float &coefficient_a, float &coefficient_b, float &coefficient_c)
	{

		double crash_rate  = 0;

		for(unsigned int i = 0; i< CrashModelVector.size(); i++)
		{
		
			CrashPredictionModel model = CrashModelVector[i];

			if(model.facility_type == facility_type &&
			   model.severity_level == severity_level && 
			   model.urban_flag == urban_flag &&
			   model.group_1_code  == group_1_code && 
			   model.group_2_code  == group_2_code && 
			   model.group_3_code  == group_3_code )

			{
				// find the model

				if(model.directional_flag  == 1)
				{
					crash_rate   =  exp(model.coefficient_a ) * pow(AADT,model.coefficient_b ) * pow(link_length, model.coefficient_c );
					
				}else  // you can use 0 or 2 to represent bi-directional flow
				{
					crash_rate   =  0.5 * exp(model.coefficient_a ) * pow(2*AADT,model.coefficient_b ) * pow(link_length, model.coefficient_c );
		
				}

				coefficient_a  = model.coefficient_a ;
				coefficient_b  = model.coefficient_b ;
				coefficient_c  = model.coefficient_c ;

				return crash_rate;
			
			}
		


		
		}
		TRACE("warning: crash prediction model is not found for %s,%s,%s,%s,%s\n", 
			facility_type.c_str (),severity_level.c_str (),urban_flag.c_str (),
			group_1_code.c_str (), group_2_code.c_str (), group_3_code.c_str ());
	
			// no model has been found
		return 0;
	}


	int safety_crash_model_id;

	double input_length_in_miles;
	double input_upstream_ADT;
	double input_on_ramp_ADT;
	double input_off_ramp_ADT;
	double input_spacing_in_feet;
	double input_aux_lane;
	double input_num_lanes;


	double coeff_length_in_miles;
	double coeff_upstream_ADT;
	double coeff_on_ramp_ADT;
	double coeff_off_ramp_ADT;
	double coeff_freeway_constant;
	double coeff_inverse_spacing_in_feet;
	double coeff_aux_lane;
	double coeff_num_lanes;

	double num_fi_crashes_per_year;
	double avg_additional_delay_per_vehicle_per_crash_in_min;
	double avg_additional_delay_per_peak_period;
	double peak_hour_time_of_day_ratio;


	double num_pdo_crashes_per_year;
	double add_delay_per_period;
	double AADT;
	double minor_leg_AADT;
	double two_way_AADT;
	double on_ramp_AADT;
	double off_ramp_AADT;
	double upstream_AADT;
	double num_driveway;
	double intersection_3sg;
	double intersection_4sg;
	double intersection_3st;
	double intersection_4st;

	DTASafetyPredictionModel()
	{
		peak_hour_time_of_day_ratio = 0.15;
		num_fi_crashes_per_year = 0;
		input_length_in_miles = 0.7f;
		input_num_lanes = 3;
		input_upstream_ADT = 80000;
		input_on_ramp_ADT = 2500;
		input_off_ramp_ADT = 3500;
		input_spacing_in_feet = 2000;
		input_aux_lane = 1;

		//  	1/3 * [length_in_miles^1.0*upstream_ADT^0.9212*on_ramp_ADT^0.1209*off_ramp_ADT^0.0445*exp(-8.492 + (513.59/spacing_in_feet) – 300.89*(aux_lane/spacing_in_feet) + 0.1638*input_aux_lane)] 

		coeff_length_in_miles = 1.0f;
		coeff_upstream_ADT = 0.9212f;
		coeff_on_ramp_ADT = 0.1209f;
		coeff_off_ramp_ADT = 0.0445f;
		coeff_freeway_constant = -8.492f;
		coeff_inverse_spacing_in_feet = 513.59f;
		coeff_aux_lane = -300.89f;
		coeff_num_lanes = 0.1638f;
		avg_additional_delay_per_vehicle_per_crash_in_min = 10.15;



	}

	void EstimateDelayPerPeakPeriod()
	{
		avg_additional_delay_per_peak_period  = num_fi_crashes_per_year/260 * peak_hour_time_of_day_ratio  * avg_additional_delay_per_vehicle_per_crash_in_min;		 
		TRACE("additional_delay: %f\b",avg_additional_delay_per_peak_period);
	}


	double EstimateFreewayCrashRatePerYear(double input_upstream_ADT, double input_length_in_miles)
	{
		//  	1/3 * [length_in_miles^1.0*upstream_ADT^0.9212*on_ramp_ADT^0.1209*off_ramp_ADT^0.0445*exp(-8.492 + (513.59/spacing_in_feet) – 300.89*(aux_lane/spacing_in_feet) + 0.1638*input_aux_lane)] 
		double total_crash_per_year  = 1.0/3.0*(pow(input_length_in_miles,coeff_length_in_miles)*
			pow(input_upstream_ADT,coeff_upstream_ADT)*
			pow(input_on_ramp_ADT,coeff_on_ramp_ADT)*
			pow(input_off_ramp_ADT,coeff_off_ramp_ADT)  * 
			exp( coeff_freeway_constant + coeff_inverse_spacing_in_feet/max(0.000001,input_spacing_in_feet) +
			coeff_aux_lane * input_aux_lane / max(0.000001,input_spacing_in_feet) + 
			coeff_num_lanes* input_num_lanes));

		//		TRACE("crash %f\b",num_fi_crashes_per_year);

		return total_crash_per_year;

	}

	// data for aterial crashes
	double Nmvnd_total, Nmvnd_fatal_injury,Nmvnd_PDO;
	double Nsv_total, Nsv_fatal_injury,Nsv_PDO;
	double Nmvd_total, Nmvd_fatal_injury,Nmvd_PDO;
	double Nmv_total_3SG,Nmv_fatal_injury_3SG,Nmv_PDO_3SG;
	double Nmv_total_3ST,Nmv_fatal_injury_3ST,Nmv_PDO_3ST;
	double Nmv_total_4SG, Nmv_fatal_injury_4SG,Nmv_PDO_4SG;
	double Nmv_total_4ST, Nmv_fatal_injury_4ST,Nmv_PDO_4ST;
	double Nsv_total_3SG, Nsv_fatal_injury_3SG,Nsv_PDO_3SG;
	double Nsv_total_3ST, Nsv_fatal_injury_3ST, NSv_PDO_3ST;
	double Nsv_total_4SG, Nsv_fatal_injury_4SG,Nsv_PDO_4SG;
	double Nsv_total_4ST,Nsv_fatal_injury_4ST,NSv_PDO_4ST;


	double EstimateArterialCrashRatePerYear(double crash_ratio_on_this_link, 
		double &total_fatal_injury_per_year, 
		double &total_PDO_per_year,
		double &total_intersection_crash_per_year, 
		double &total_intersection_fatal_injury_per_year, 
		double &total_intersection_PDO_per_year,

		double AADT = 26000,
		double Length  = 0.89, 
		double Num_Driveways_Per_Mile = 20,
		double volume_proportion_on_minor_leg = 0.1,
		double Num_3SG_Intersections = 1,
		double Num_3ST_Intersections = 1,
		double Num_4SG_Intersections = 3,
		double Num_4ST_Intersections = 0)
	{
		Nmvnd_total = exp(-11.63 + 1.33*log(AADT) + log(Length));
		Nmvnd_fatal_injury = Nmvnd_total*(exp(-12.08 + 1.25*log(AADT) 
			+ log(Length))/max(0.00000001,(exp(-12.08 + 1.25*log(AADT) + log(Length)) + exp(-12.53 + 1.38*log(AADT) + log(Length)))));
		Nmvnd_PDO = Nmvnd_total - Nmvnd_fatal_injury;

		Nsv_total = exp(-7.99 + 0.81*log(AADT) + log(Length));
		Nsv_fatal_injury = Nsv_total*(exp(-7.37 + 0.61*log(AADT) + log(Length))/max(0.0000001,(exp(-7.37 + 0.61*log(AADT) + log(Length)) + exp(-8.5 + 0.84*log(AADT) + log(Length)))));
		Nsv_PDO = Nsv_total - Nsv_fatal_injury;

		Nmvd_total = pow(0.083*Num_Driveways_Per_Mile*Length*(AADT/15000),1);
		Nmvd_fatal_injury = Nmvd_total*0.323;
		Nmvd_PDO = Nmvd_total*0.677;

		Nmv_total_3SG = Num_3SG_Intersections*exp(-12.13 + 1.11*log(AADT) + 0.26*log(volume_proportion_on_minor_leg*AADT));
		Nmv_fatal_injury_3SG = Nmv_total_3SG*
			exp(-11.58 + 1.02*log(AADT) + 0.17*log(volume_proportion_on_minor_leg*AADT))/
			max(0.00000001,( exp(-11.58 + 1.02*log(AADT) + 0.17*log(volume_proportion_on_minor_leg*AADT))+ 
			exp(-13.24 + 1.14*log(AADT) + 0.3*log(volume_proportion_on_minor_leg*AADT))));
		Nmv_PDO_3SG = Nmv_total_3SG - Nmv_fatal_injury_3SG;


		Nmv_total_3ST = Num_3ST_Intersections*exp(-13.36 + 1.11*log(AADT) + 0.41*log(volume_proportion_on_minor_leg*AADT));
		Nmv_fatal_injury_3ST = Nmv_total_3ST* exp(-14.01 + 1.16*log(AADT) + 0.3*log(volume_proportion_on_minor_leg*AADT))
			/max(0.00000001,( exp(-14.01 + 1.16*log(AADT) + 0.3*log(volume_proportion_on_minor_leg*AADT))+ 
			exp(-15.38 + 1.2*log(AADT) + 0.51*log(volume_proportion_on_minor_leg*AADT)) ));
		Nmv_PDO_3ST = Nmv_total_3ST - Nmv_fatal_injury_3ST ;


		Nmv_total_4SG = Num_4SG_Intersections*exp(-10.99 + 1.07*log(AADT) + 0.23*log(volume_proportion_on_minor_leg*AADT));
		Nmv_fatal_injury_4SG = Nmv_total_4SG
			*(exp(-13.14 + 1.18*log(AADT) + 0.22*log(volume_proportion_on_minor_leg*AADT))
			/max(0.00000001,(exp(-13.14 + 1.18*log(AADT) + 0.22*log(volume_proportion_on_minor_leg*AADT))
			+exp(-11.02 + 1.02*log(AADT) + 0.24*log(volume_proportion_on_minor_leg*AADT)))));
		Nmv_PDO_4SG = Nmv_total_4SG - Nmv_fatal_injury_4SG;

		Nmv_total_4ST = Num_4ST_Intersections*exp(-8.9 + 0.82*log(AADT) + 0.25*log(volume_proportion_on_minor_leg*AADT));
		Nmv_fatal_injury_4ST = Nmv_total_4ST
			*(exp(-11.13 + 0.93*log(AADT) + 0.28*log(volume_proportion_on_minor_leg*AADT))
			/max(0.00000001,(exp(-11.13 + 0.93*log(AADT) + 0.28*log(volume_proportion_on_minor_leg*AADT))
			+exp(-8.74 + 0.77*log(AADT) + 0.23*log(volume_proportion_on_minor_leg*AADT)))));
		Nmv_PDO_4ST = Nmv_total_4ST - Nmv_fatal_injury_4ST;

		Nsv_total_3SG = Num_3SG_Intersections*exp(-9.02 + 0.42*log(AADT) + 0.4*log(volume_proportion_on_minor_leg*AADT));
		Nsv_fatal_injury_3SG =Nsv_total_3SG
			*(exp(-9.75 + 0.27*log(AADT) + 0.51*log(volume_proportion_on_minor_leg*AADT))
			/max(0.000001,( exp(-9.75 + 0.27*log(AADT) + 0.51*log(volume_proportion_on_minor_leg*AADT))
			+exp(-9.08 + 0.45*log(AADT) + 0.33*log(volume_proportion_on_minor_leg*AADT)))));
		Nsv_PDO_3SG = Nsv_total_3SG - Nsv_fatal_injury_3SG;

		Nsv_total_3ST = Num_3ST_Intersections*exp(-6.81 + 0.16*log(AADT) + 0.51*log(volume_proportion_on_minor_leg*AADT));
		Nsv_fatal_injury_3ST = Nsv_total_3ST
			*(0.31*exp(-6.81 + 0.16*log(AADT) + 0.51*log(volume_proportion_on_minor_leg*AADT)))
			/max(0.0000001,((0.31*exp(-6.81 + 0.16*log(AADT) + 0.51*log(volume_proportion_on_minor_leg*AADT)))
			+ exp(-8.36 + 0.25*log(AADT) + 0.55*log(volume_proportion_on_minor_leg*AADT) ) ));
		NSv_PDO_3ST = Nsv_total_3ST - Nsv_fatal_injury_3ST;

		Nsv_total_4SG = Num_4SG_Intersections*exp(-10.21 + 0.68*log(AADT) + 0.27*log(volume_proportion_on_minor_leg*AADT));
		Nsv_fatal_injury_4SG = Nsv_total_4SG*
			exp(-9.25 + 0.43*log(AADT) + 0.29*log(volume_proportion_on_minor_leg*AADT))
			/ max(0.000001,(exp(-9.25 + 0.43*log(AADT) + 0.29*log(volume_proportion_on_minor_leg*AADT))
			+ exp(-11.34 + 0.78*log(AADT) + 0.25*log(volume_proportion_on_minor_leg*AADT))));
		Nsv_PDO_4SG = Nsv_total_4SG - Nsv_fatal_injury_4SG;

		Nsv_total_4ST = Num_4ST_Intersections*exp(-5.33 + 0.33*log(AADT) + 0.12*log(volume_proportion_on_minor_leg*AADT));
		Nsv_fatal_injury_4ST = Nsv_total_4ST*
			(0.28*exp(-5.33 + 0.33*log(AADT) + 0.12*log(volume_proportion_on_minor_leg*AADT))
			/max(0.000001,(0.28*exp(-5.33 + 0.33*log(AADT) + 0.12*log(volume_proportion_on_minor_leg*AADT))
			+exp(-7.04 + 0.36*log(AADT) + 0.25*log(volume_proportion_on_minor_leg*AADT)))));
		NSv_PDO_4ST = Nsv_total_4ST - Nsv_fatal_injury_4ST ;

		TRACE("%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n",
			Nmvnd_total, Nmvnd_fatal_injury,Nmvnd_PDO,
			Nsv_total, Nsv_fatal_injury,Nsv_PDO,
			Nmvd_total, Nmvd_fatal_injury,Nmvd_PDO,
			Nmv_total_3SG,Nmv_fatal_injury_3SG,Nmv_PDO_3SG,
			Nmv_total_3ST,Nmv_fatal_injury_3ST,Nmv_PDO_3ST,
			Nmv_total_4SG, Nmv_fatal_injury_4SG,Nmv_PDO_4SG,
			Nmv_total_4ST, Nmv_fatal_injury_4ST,Nmv_PDO_4ST,
			Nsv_total_3SG, Nsv_fatal_injury_3SG,Nsv_PDO_3SG,
			Nsv_total_3ST, Nsv_fatal_injury_3ST, NSv_PDO_3ST,
			Nsv_total_4SG, Nsv_fatal_injury_4SG,Nsv_PDO_4SG,
			Nsv_total_4ST,Nsv_fatal_injury_4ST,NSv_PDO_4ST);


		Nmvnd_total*= crash_ratio_on_this_link;

		Nmvnd_fatal_injury*=crash_ratio_on_this_link;
		Nmvnd_PDO*=crash_ratio_on_this_link;
		Nsv_total*=crash_ratio_on_this_link; 
		Nsv_fatal_injury*=crash_ratio_on_this_link;
		Nsv_PDO*=crash_ratio_on_this_link;
		Nmvd_total*=crash_ratio_on_this_link; 
		Nmvd_fatal_injury*=crash_ratio_on_this_link;
		Nmvd_PDO*=crash_ratio_on_this_link;
		Nmv_total_3SG*=crash_ratio_on_this_link;
		Nmv_fatal_injury_3SG*=crash_ratio_on_this_link;
		Nmv_PDO_3SG*=crash_ratio_on_this_link;
		Nmv_total_3ST*=crash_ratio_on_this_link;
		Nmv_fatal_injury_3ST*=crash_ratio_on_this_link;
		Nmv_PDO_3ST*=crash_ratio_on_this_link;
		Nmv_total_4SG*=crash_ratio_on_this_link; 
		Nmv_fatal_injury_4SG*=crash_ratio_on_this_link;
		Nmv_PDO_4SG*=crash_ratio_on_this_link;
		Nmv_total_4ST*=crash_ratio_on_this_link; 
		Nmv_fatal_injury_4ST*=crash_ratio_on_this_link;
		Nmv_PDO_4ST*=crash_ratio_on_this_link;
		Nsv_total_3SG*=crash_ratio_on_this_link; 
		Nsv_fatal_injury_3SG*=crash_ratio_on_this_link;
		Nsv_PDO_3SG*=crash_ratio_on_this_link;
		Nsv_total_3ST*=crash_ratio_on_this_link; 
		Nsv_fatal_injury_3ST*=crash_ratio_on_this_link; 
		NSv_PDO_3ST*=crash_ratio_on_this_link;
		Nsv_total_4SG*=crash_ratio_on_this_link; 
		Nsv_fatal_injury_4SG*=crash_ratio_on_this_link;
		Nsv_PDO_4SG*=crash_ratio_on_this_link;
		Nsv_total_4ST*=crash_ratio_on_this_link;
		Nsv_fatal_injury_4ST*=crash_ratio_on_this_link;
		NSv_PDO_4ST*=crash_ratio_on_this_link;

		double total_crash_per_year = (
			Nmvnd_total +
			Nsv_total +
			Nmvd_total + 
			Nmv_total_3SG +
			Nmv_total_3ST +
			Nmv_total_4SG +
			Nmv_total_4ST +
			Nsv_total_3SG +
			Nsv_total_3ST + 
			Nsv_total_4SG +
			Nsv_total_4ST);

		total_fatal_injury_per_year =  (
			Nmvnd_fatal_injury +
			Nsv_fatal_injury +
			Nmvd_fatal_injury + 
			Nmv_fatal_injury_3SG +
			Nmv_fatal_injury_3ST +
			Nmv_fatal_injury_4SG +
			Nmv_fatal_injury_4ST +
			Nsv_fatal_injury_3SG +
			Nsv_fatal_injury_3ST + 
			Nsv_fatal_injury_4SG +
			Nsv_fatal_injury_4ST);

		total_PDO_per_year =  (
			Nmvnd_PDO +
			Nsv_PDO +
			Nmvd_PDO + 
			Nmv_PDO_3SG +
			Nmv_PDO_3ST +
			Nmv_PDO_4SG +
			Nmv_PDO_4ST +
			Nsv_PDO_3SG +
			NSv_PDO_3ST + 
			Nsv_PDO_4SG +
			NSv_PDO_4ST );

		total_intersection_crash_per_year = (
			Nmv_total_3SG +
			Nmv_total_3ST +
			Nmv_total_4SG +
			Nmv_total_4ST +
			Nsv_total_3SG +
			Nsv_total_3ST + 
			Nsv_total_4SG +
			Nsv_total_4ST);

		total_intersection_fatal_injury_per_year =  (
			Nmv_fatal_injury_3SG +
			Nmv_fatal_injury_3ST +
			Nmv_fatal_injury_4SG +
			Nmv_fatal_injury_4ST +
			Nsv_fatal_injury_3SG +
			Nsv_fatal_injury_3ST + 
			Nsv_fatal_injury_4SG +
			Nsv_fatal_injury_4ST);

		total_intersection_PDO_per_year = crash_ratio_on_this_link * (
			Nmv_PDO_3SG +
			Nmv_PDO_3ST +
			Nmv_PDO_4SG +
			Nmv_PDO_4ST +
			Nsv_PDO_3SG +
			NSv_PDO_3ST + 
			Nsv_PDO_4SG +
			NSv_PDO_4ST );
		return total_crash_per_year;
	}




};