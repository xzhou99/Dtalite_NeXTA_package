# STALite/DTALite/NeXTA Software Introduction

####Contact: Dr. Xuesong Zhou at Arizona State University, U.S.
Email: xzhou74@asu.edu, google discussion group: https://groups.google.com/g/dtalite-development.

STAlite/DTALite is an open-source AMS library for efficiently macroscopic traffic assignment 
 based on General Modeling Network Specification (GMNS) format
 
   #Goals of STALite/DTALite development
   1. Provide an open-source code base to enable transportation researchers and software developers to expand its range of capabilities to various traffic management application.
   2. Present results to other users by visualizing time-varying traffic flow dynamics and traveler route choice behavior in an integrated environment.
   3. Provide a free, educational tool for students to understand the complex decision-making process in transportation planning and optimization processes. 
  
  #What is GMNS?
  General Travel Network Format Specification is a product of Zephyr Foundation, which aims to advance the field through flexible and efficient support, education, guidance, encouragement, and incubation.
  Further Details in https://zephyrtransport.org/projects/2-network-standard-and-tools/
   
 #Features
 
 1.Bridging the gap from macroscopic static traffic assignment to mesoscopic dynamic traffic assignment
 
 2.Network representation based on GMNS format
 
 3.Integrated graphic user interface and model/simulation package
 
 4.Parallel computing on shared memory multi-core computer, Efficient multi-threading parallel computation and memory management, implemented in C++
 	Utilize up to 40 CPU cores, 200 GB of Memory for networks with more than 500K nodes
 	
5. Integrated signal timing optimization and traffic assignment 

6. Integrated OD demand estimation through path flow estimator.

The latest software release can be downloaded at our Github website. The source code can be downloaded at https://github.com/xzhou99/STALite.
The stable release (09-04-2020, Version GMNS0.9_beta_1) is located at https://github.com/xzhou99/stalite-dtalite_software_release/releases.
The learning documents can be found at https://github.com/xzhou99/stalite-dtalite_software_release/tree/gh-pages/learning_document


###NeXTA: Network EXplorer for Traffic Analysis
https://github.com/xzhou99/NeXTA-GMNS

#####NOTE: NeXTA is only compatible with a Microsoft Operating System. 

Instruction for the use of NeXTA and STALite/DTALite:

#[A: Documentation]

###[A1:Data structure and workflow](https://docs.google.com/document/d/e/2PACX-1vR_k5Hjlc5B4Oxf4Q41RqHM-Q6RApHiIkfmPssSikhEGSA55r3_yPkC1TYtBlormFDQ_wvLO6M8NYt0/pub)
You can add your comments on this on-line document here. https://docs.google.com/document/d/1WTy-6V6EfFtHeZTmBteX_kCFoaehc6LKq5viakw7NgQ/edit?usp=sharing
###[A2:NEXTA user guide] (https://github.com/xzhou99/NeXTA-GMNS/tree/master/docs)
###[A3:OD demand estimation user guide] (https://docs.google.com/document/d/1UqFXVRbf0eOuq-liPXCDF6PVuSNjglWggLPbwT_y3TI)

#[B: NEXTA-for-GIS Software Release]().
###[B1: User guide]()

###DTALite: Light-weight Dynamic Traffic Assignment Engine
White Paper: [DTALite: A queue-based mesoscopic traffic simulator for fast model evaluation and calibration. Xuesong Zhou, Jeffrey Taylor. Cogent Engineering, Vol. 1, Iss. 1, 2014](http://www.tandfonline.com/doi/full/10.1080/23311916.2014.961345)

Network EXplorer for Traffic Analysis (NEXTA) (Version 3) is an open-source GUI that aims to facilitate the preparation, post-processing and analysis of transportation assignment, simulation and scheduling datasets. NeXTA Version 3 uses DTALite, a fast dynamic traffic assignment engine, for transportation network analysis.

1. Create, import, edit, store, export and visualize transportation network and link performance data.
![nexta](Images/nexta1.png)
![nexta](Images/nexta2.png)
2. The NEXTA now support importing the following data format

  - GIS shape files 
  - CSV file based network data 
           
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
  
4. Simulation and visuailiing dynamic outputs
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

- Flexible demand data format: 3-column (o,d, value), Agent file, and routing policy files.

Semi-continuous Value Of Time distribution

- Common types of sensor data, link count, lane count, at user-defined interval, speed data, density data, route travel time data,

![dta](Images/dta1.png)

DTALite/NEXTA package provides a wide range of data output. 
-  Agent based trajectory 

-  Link-based MOEs, band-width display, user defined offset 

-  Time-dependent path travel times for user-defined path 

-  Select link analysis 

-  Select path analysis 

-  Subarea analysis 

-  Summary plot based on a wide range of categories and MOEs


DTALite/NeXTA applications in The United States

![maps](Images/Project_US.png)


