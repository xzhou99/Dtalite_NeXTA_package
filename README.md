# DTALite/NeXTA Software Introduction

####Contact: Dr. Xuesong Zhou at Arizona State University, U.S.
Email: xzhou74@asu.edu,xzhou99@gmail.com

###NeXTA: Network EXplorer for Traffic Analysis

### Latest Software Release 09-15-2016:

#####NOTE: NeXTA is only compatible with a Microsoft Operating System. 

Instruction for the use of NeXTA and DTALite:

Step 1: make sure that you have installed the Microsoft Visual C++ 2015 Redistributable Package (x86) for parallel computing in DTALite (https://www.microsoft.com/en-us/download/details.aspx?id=3387)

Step 2: make sure that you have installed Gnuplot Software for some visualization functions in NeXTA (http://www.gnuplot.info/)

Step 3: Download and unzip the NeXTA/DTALite software package. 

If GIS data importing/exporting are needed for you, please download and unzip the NeXTA GIS software package and obey the related instructions.

#[A: NEXTA-DTALite Software Release]

###[A1:Data structure and workflow of DTALite]()
###[A2:NEXTA user guide]()
###[A3:ABM+DTA Integration user guide ](https://docs.google.com/document/d/1oc1FAapyN0K1v3KSIB04gXrlL1bFTYVH3I6S1URR6oY)
###[A4:OD demand estimation user guide] (https://docs.google.com/document/d/1UqFXVRbf0eOuq-liPXCDF6PVuSNjglWggLPbwT_y3TI)

#[B: NEXTA-for-GIS Software Release]().
###[B1: User guide]()

###DTALite: Light-weight Dynamic Traffic Assignment Engine

White Paper: [DTALite: A queue-based mesoscopic traffic simulator for fast model evaluation and calibration. Xuesong Zhou, Jeffrey Taylor. Cogent Engineering, Vol. 1, Iss. 1, 2014](http://www.tandfonline.com/doi/full/10.1080/23311916.2014.961345)

Network EXplorer for Traffic Analysis (NEXTA) (Version 3) is an open-source GUI that aims to facilitate the preparation, post-processing and analysis of transportation assignment, simulation and scheduling datasets. NeXTA Version 3 uses DTALite, a fast dynamic traffic assignment engine, for transportation network analysis.

1. Create, import, edit, store, export and visualize transportation network data.
![nexta](Images/nexta1.png)
![nexta](Images/nexta2.png)
2. The NEXTA now support importing the following data 

  -	GIS shape files 
  
  - CSV file based network data 
  
  -	TMC-based sensor data (e.g. Inrix or traffic.com) 
  
   NEXTA also can export to the following formatNEXTA
  
  -	Google Earth KML 
  
  -	GIS shape file: node/link/zone, or convert user-defined CSV to shape 
  
  -	Subarea analysis to provide path flow pattern from Vissim simulation.
   
3. NEXTA provides an excellent multi-project management interface with the following features. 

  -  Synchronized display 
  
  -  Click the same location across different networks 
  
  -  Compare link moe across different networks 
  
  -  Find link moe difference between baseline and alternative networks 
  
  -  Vehicle path analysis across different simulation results 
  
  -  Simulation vs. sensor data 
  
  -  Link based comparison 
  
  -  Path travel time comparison 
  
  -  Validation results, diagonal line display to identify outliers
  
4. Import multi-day traffic measurement data and provide multi-criteria path finding results (mobility, reliability and emissions)
![nexta](Images/nexta3.png)

5. Simulation and visuailiing dynamic outputs

![nexta](Images/nexta4.png)
![nexta](Images/output_e1.png)
![nexta](Images/output_e2.png)


DTALite uses a computationally simple but theoretically rigorous traffic queuing model in its lightweight mesoscopic simulation engine. Its built-in parallel computing capability dramatically speeds-up the analysis process by using widely available multi-core CPU hardware. It takes about 1 hour to compute agent-based dynamic traffic equilibrium for a large-scale network with 1 million vehicles for 20 iterations.

The scope includes:

- dynamic traffic assignment of large-scale network

	- typical network: 2000 traffic zones, 200000 links, 2-10 million vehiches

- Network capacity planning
  
	- add/remove link, prepare basic data for optimizing signal timing (Synchro), and Micro simulation (Vissim)

	- export traffic capacity analising package (HCM, travel time reliability) 

- Operation of work zone area, application of tolling road/link: based on dynamic tolling strategy, value of time, Agent model

- Emission analysis, traffic safety analysis


The DTALite package provides the following unique features using the AMS data hub format through NEXTA.

- Unlimited number of link types

- Unlimited number of demand types/demand files, 24 hour loading period: Flexible network conversion and linkage with GIS Shapefile (importing, script for mapping planning data to our data hub): save time, allow flexible number of link types and node types)

- Flexible demand data format: 3-column (o,d, value), multiple columns (o,d, SOV, HOV, Truck, subtotal), matrix, with 15-min departure time interval. Agent file

- Typical vehicle types: mapping from trip types to vehicle types, vehicle emission rates for different vehicle types, different ages
Semi-continuous Value Of Time distribution

- Common types of sensor data, link count, lane count, at user-defined interval, speed data, density data, route travel time data,

- Unlimited number of safety prediction models, based on link volume, length, link type, # of intersections/drive ways per miles
Movement-specific parameters (based on HCM/QEM methodology)

![dta](Images/dta1.png)

DTALite/NEXTA package provides a wide range of data output. 
-  Agent based trajectory 

-  Link-based MOEs, band-width display, user defined offset 

-  Safety and emission statistics based on dynamic assignment results. 

-  Time-dependent path travel times for user-defined path 

-  OD based MOEs 

-  Select link analysis 

-  Select path analysis 

-  Subarea analysis 

-  Summary plot based on a wide range of categories and MOEs


DTALite/NeXTA applications in The United States

![maps](Images/Project_US.png)


