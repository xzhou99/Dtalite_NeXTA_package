
// Becasese the functions below might relate to file interfaces with other proprietary software packages, no copyright or GPL statement is made here.
// Utility.cpp : Utility functions used for reading and outputing

#include "stdafx.h"
#include "math.h"
#include "Network.h"
#include "Utility.h"
#include <vector>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



using namespace std;


int g_ParserIntSequence(std::string str, std::vector<int> &vect) 
{

std::stringstream ss(str);

int i;

	while (ss >> i)
	{
		vect.push_back(i);

		if (ss.peek() == ';')
			ss.ignore();
	}

	return vect.size();
}


int g_ParserFloatSequence(std::string str, std::vector<float> &vect) 
{

std::replace( str.begin(), str.end(), '<', ' '); 
std::replace( str.begin(), str.end(), '>', ' '); 
std::stringstream ss(str);

float i;

	while (ss >> i)
	{
		vect.push_back(i);

		if (ss.peek() == ';' || ss.peek() == ':')
			ss.ignore();
	}

	return vect.size();
}

int g_read_number_of_numerical_values(char* line_string, int length, std::vector<float> & values)
//read a floating point number from the current pointer of the file,
//skip all spaces

{
	char ch, buf[ 32 ];

	int number_count= 0;
	int string_index = 0;

	/* returns -1 if end of file is reached */
	while(string_index<length)
	{

		int i = 0;
		int flag = 1;

		while(true)
		{
			if(string_index==length)
			{
				break;
			}


			ch = line_string[string_index++];
			if( ch == EOF ) return number_count;
			if (isdigit(ch))
				break;

			if (ch == '-')
				flag = -1;
			else
				flag = 1;

		};
		if( ch == EOF ) return number_count;
		while( isdigit( ch ) || ch == '.' ) {
			buf[ i++ ] = ch;
			ch = line_string[string_index++];

		}
		buf[ i ] = 0;

		double value = atof( buf );

		if(value>-0.0000001)  // positive values
		{
		values.push_back (value);

			number_count++;
		}
	}

	/* atof function converts a character string (char *) into a doubleing
	pointer equivalent, and if the string is not a floting point number,
	a zero will be return.
	*/

	return number_count;

}

float g_read_float_from_a_line(FILE *f, int &end_of_line)
//read a floating point number from the current pointer of the file,
//skip all spaces

{
	char ch, buf[ 32 ];
	int i = 0;
	int flag = 1;
	end_of_line = 0;

	/* returns -100 if end of line is reached */

	while(true)
	{
		ch = getc( f );
		if( ch == EOF || ch == '*' || ch == '$' ) 
			return -1;

		if( ch == '\n' ) 
			return -100;

		if (isdigit(ch))
			break;

		if (ch == '-')
			flag = -1;
		else
			flag = 1;

	};
	if( ch == EOF ) return -1;
	while( isdigit( ch ) || ch == '.' ) {
		buf[ i++ ] = ch;
		ch = fgetc( f );
		if (ch == '\n')
		{
		
		end_of_line=1;
		break;
		}
		

	}
	buf[ i ] = 0;

	/* atof function converts a character string (char *) into a doubleing
	pointer equivalent, and if the string is not a floting point number,
	a zero will be return.
	*/

	return (float)(atof( buf ) * flag);

}


std::string string_format(const std::string fmt, ...)
{
    int size = 100;
    std::string str;
    va_list ap;
    while (1) {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {
            str.resize(n);
            return str;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    }
    return str;
}


CString g_GetExcelColumnFromNumber(int column)
        {
            CString columnString = "";
            int columnNumber = column;
            while (columnNumber > 0)
            {
                int currentLetterNumber = (columnNumber - 1) % 26;
                char currentLetter = (char)(currentLetterNumber + 65);
                columnString = currentLetter + columnString;
                columnNumber = (columnNumber - (currentLetterNumber + 1)) / 26;
            }
            return columnString;
        }


bool g_read_a_line(FILE* f, char* aline, int & size)
   /* read a line from the current line from the file */
{

   char ch;
   size = 0;

   while( 1 ) {
      ch = getc( f );
      if( ch != 13 && ch != 10 && ch != EOF )
	 aline[ size++ ] = ch;
      else { /* terminate if it's end of line or end of file */
	 aline[ size ] = 0;
	 if( ch == EOF )
	    return false;

	 return true;
      }
   }
}
void g_ProgramStop()
{
	getchar();
	exit(0);
};


double g_GetPoint2Point_Distance(GDPoint p1, GDPoint p2)
{
return pow(((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)),0.5);
}

double g_CalculateP2PDistanceInMileFromLatitudeLongitude(GDPoint p1, GDPoint p2)
{
	       double Equatorial_Radius = 3963.19059; // unit: mile
	       double toradians=3.1415926/180.0;
	       double todeg = 180.0/PI;

	       double p2lat= p2.x*toradians;
	       double p2lng= p2.y*toradians;

	       double p1lat= p1.x*toradians;
	       double p1lng= p1.y*toradians;

	       double distance=acos(sin(p1lat)*sin(p2lat)+cos(p1lat)*cos(p2lat)*cos(p2lng-p1lng))*Equatorial_Radius;  // unit: mile
		   return distance;
}


//  public domain function by Darel Rex Finley, 2006
//  Determines the intersection point of the line defined by points A and B with the
//  line defined by points C and D.
//
//  Returns YES if the intersection point was found, and stores that point in X,Y.
//  Returns NO if there is no determinable intersection point, in which case X,Y will
//  be unmodified.

bool g_get_line_intersection(
float Ax, float Ay,
float Bx, float By,
float Cx, float Cy,
float Dx, float Dy,
float *X, float *Y) 
{

 double  distAB, theCos, theSin, newX, ABpos ;

  //  Fail if either line segment is zero-length.
//  if (Ax==Bx && Ay==By || Cx==Dx && Cy==Dy) return false;
  if (Ax==Bx && Ay==By ) return false;  // comment: C and D can be the same point from a vehile with the same timestamp

  //  Fail if the segments share an end-point.
  if (Ax==Cx && Ay==Cy || Bx==Cx && By==Cy
  ||  Ax==Dx && Ay==Dy || Bx==Dx && By==Dy) {
    return false; }

  //  (1) Translate the system so that point A is on the origin.
  Bx-=Ax; By-=Ay;
  Cx-=Ax; Cy-=Ay;
  Dx-=Ax; Dy-=Ay;

  //  Discover the length of segment A-B.
  distAB=sqrt(Bx*Bx+By*By);

  //  (2) Rotate the system so that point B is on the positive X axis.
  theCos=Bx/distAB;
  theSin=By/distAB;
  newX=Cx*theCos+Cy*theSin;
  Cy  =Cy*theCos-Cx*theSin; Cx=newX;
  newX=Dx*theCos+Dy*theSin;
  Dy  =Dy*theCos-Dx*theSin; Dx=newX;

  //  Fail if segment C-D doesn't cross line A-B.
  if (Cy<0. && Dy<0. || Cy>=0. && Dy>=0.) return false;

  //  (3) Discover the position of the intersection point along line A-B.
  ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

  //  Fail if segment C-D crosses line A-B outside of segment A-B.
  if (ABpos<0. || ABpos>distAB) return false;

  //  (4) Apply the discovered position to line A-B in the original coordinate system.
  *X=Ax+ABpos*theCos;
  *Y=Ay+ABpos*theSin;

  //  Success.
  return true; 
}

double g_GetPoint2LineDistance(GDPoint pt, GDPoint FromPt, GDPoint ToPt, double UnitMile, bool no_intersection_requirement)
{
    float U;
    GDPoint Intersection;

    float  LineLength = g_GetPoint2Point_Distance( FromPt, ToPt );
 
    U = (  (pt.x - ToPt.x) * (FromPt.x - ToPt.x ) + ( pt.y - ToPt.y ) * ( FromPt.y - ToPt.y ) ) /(LineLength * LineLength );
 
	if (no_intersection_requirement == false)
	{

		if (U < 0.0f || U > 1.0f)
			return max(UnitMile * 100, 999999);   // intersection does not fall within the segment
	}
    Intersection.x = ToPt.x + U * ( FromPt.x - ToPt.x );
    Intersection.y = ToPt.y + U * ( FromPt.y - ToPt.y );
    
	float distance_1 = g_GetPoint2Point_Distance( pt, Intersection );
	float distance_0 = g_GetPoint2Point_Distance( pt, FromPt );
	float distance_2 = g_GetPoint2Point_Distance( pt, ToPt );

	if (no_intersection_requirement)
	{
		return min(min(distance_1, distance_0), distance_2) / max(0.000001,UnitMile);
	}
	else
		return distance_1 / max(0.000001, UnitMile);
}
float g_GetRandomRatio()
{
	/*		g_RandomSeed = (g_LCG_a * g_RandomSeed + g_LCG_c) % g_LCG_M;  //m_RandomSeed is automatically updated.

			return float(g_RandomSeed)/g_LCG_M; */

	return float(rand())/RAND_MAX;
}


// polar form of the Box-Muller transformation to get two random numbers that follow a standard normal distribution 
float g_RNNOF()
{
    float x1, x2, w, y1, y2;
 
         do {
                 x1 = 2.0f * g_GetRandomRatio() - 1.0f;
                 x2 = 2.0f * g_GetRandomRatio()- 1.0f;
                 w = x1 * x1 + x2 * x2;
         } while ( w >= 1.0f );

         w = sqrt( (-2.0f * log( w ) ) / w );
         y1 = x1 * w;
         y2 = x2 * w;

	 return y1;  // we only use one random number
}


int g_read_integer_with_char_O(FILE* f)
   // read an integer from the current pointer of the file, skip all spaces, if read "O", return 0;
{
   char ch, buf[ 32 ];
   int i = 0;
   int flag = 1;
   /* returns -1 if end of file is reached */

   while(true)
      {
      ch = getc( f );
      if( ch == EOF ) return -1;
      if( ch == 'O' ) return 0;  // special handling

      if (isdigit(ch))
         break;
      if (ch == '-')
         flag = -1;
      else
         flag = 1;
      };
   if( ch == EOF ) return -1;
   while( isdigit( ch )) {
      buf[ i++ ] = ch;
      ch = fgetc( f );
      }
   buf[ i ] = 0;


   return atoi( buf ) * flag;

}


int read_2_float_points_from_a_string(CString str, double &value1, double &value2)
// read an integer from the current pointer of the file, skip all spaces
{
	value1 = 0;
	value2 = 0;
	char string_line[100];

	int string_lenghth  = str.GetLength();
	ASSERT(str.GetLength() < 100);

	sprintf(string_line,"%s\n",str);

	char ch, buf[ 32 ];
	int i = 0;
	int buffer_i = 0;
	int flag = 1;
	/* returns -1 if end of file is reached */

	for(int i_try  =0 ; i_try < 2; i_try++)
	{
		buffer_i = 0;
	while(true)
	{
		ch = string_line[i++];
		if( ch=='\n' || i > string_lenghth)
		{
			return -1; // * and $ are special characters for comments
		}
		if (isdigit(ch))
			break;
		if (ch == '-')
			flag = -1;
		else
			flag = 1;
	};
	if( ch == '\n' )
	{
		return -1;
	}
	
	while( isdigit( ch ) || ch == '.')
	{
		buf[ buffer_i++ ] = ch;
		ch =  string_line[i++];
	}
	buf[ buffer_i ] = 0;

	if(i_try==0)
	value1 = atof( buf ) * flag;

	if(i_try==1)
	value2 = atof( buf ) * flag;
	}

	return 0;
}


int read_2_integers_from_a_string(CString str, int &value1, int &value2)
// read an integer from the current pointer of the file, skip all spaces
{
	value1 = 0;
	value2 = 0;
	char string_line[100];

	int string_lenghth  = str.GetLength();
	ASSERT(str.GetLength() < 10000);

	sprintf(string_line,"%s\n",str);

	char ch, buf[ 32 ];
	int i = 0;
	int buffer_i = 0;
	int flag = 1;
	/* returns -1 if end of file is reached */

	for(int i_try  =0 ; i_try < 2; i_try++)
	{
		buffer_i = 0;
	while(true)
	{
		ch = string_line[i++];
		if( ch=='\n' || i > string_lenghth)
		{
			return -1; // * and $ are special characters for comments
		}
		if (isdigit(ch))
			break;
		if (ch == '-')
			flag = -1;
		else
			flag = 1;
	};
	if( ch == '\n' )
	{
		return -1;
	}
	
	while( isdigit( ch ))
	{
		buf[ buffer_i++ ] = ch;
		ch =  string_line[i++];
	}
	buf[ buffer_i ] = 0;

	if(i_try==0)
	value1 = atoi( buf ) * flag;

	if(i_try==1)
	value2 = atoi( buf ) * flag;
	}

	return 0;
}

bool g_detect_if_a_file_is_column_format(LPCTSTR lpszFileName)
{
	FILE* st;
	fopen_s(&st, lpszFileName, "r");
	if (st != NULL)
	{
		char  str_line[2000]; // input string
		int str_line_size = 1000;
		g_read_a_line(st, str_line, str_line_size); 

		fclose(st);

		if (strstr(str_line, "number_of_trips_demand_type1") != NULL)
			return true;
		else
			return false;

	}
	return false;
}
int read_multiple_integers_from_a_string(CString str, std::vector<int> &vector)
// read an integer from the current pointer of the file, skip all spaces
{

	if(str.GetLength () ==0 )
		return 0;

	char string_line[1000];

	int string_lenghth  = str.GetLength();
	ASSERT(str.GetLength() < 100);

	sprintf(string_line,"%s\n",str);

	char ch, buf[ 32 ];
	int i = 0;
	int buffer_i = 0;
	int flag = 1;
	/* returns -1 if end of file is reached */

	for(int i_try  =0 ; i_try < 200; i_try++)  // maximal 200 numbers
	{
		buffer_i = 0;
	while(true)
	{
		ch = string_line[i++];
		if( ch=='\n' || i > string_lenghth)
		{
			return -1; // * and $ are special characters for comments
		}
		if (isdigit(ch))
			break;
		if (ch == '-')
			flag = -1;
		else
			flag = 1;
	};
	if( ch == '\n' )
	{
		return -1;
	}
	
	while( isdigit( ch ))
	{
		buf[ buffer_i++ ] = ch;
		ch =  string_line[i++];
	}
	buf[ buffer_i ] = 0;

	int value = atoi( buf ) * flag;

	vector.push_back (value);
	}

	return 0;
}

int g_read_integer_with_special_character(FILE* f, bool speicial_char_handling, char special_ch )
// read an integer from the current pointer of the file, skip all spaces
{
	char ch, buf[ 32 ];
	int i = 0;
	int flag = 1;
	/* returns -1 if end of file is reached */

	while(true)
	{
		ch = getc( f );
		if( ch == EOF || (speicial_char_handling && (ch == '*' || ch == '$' || ch == special_ch)))
			return -1; // * and $ are special characters for comments
		if (isdigit(ch))
			break;
		if (ch == '-')
			flag = -1;
		else
			flag = 1;


	};
	if( ch == EOF ) return -1;

	
	while( isdigit( ch )) {
		buf[ i++ ] = ch;
		ch = fgetc( f );
	}
	buf[ i ] = 0;


	return atoi( buf ) * flag;

}


int g_read_integer(FILE* f, bool speicial_char_handling)
// read an integer from the current pointer of the file, skip all spaces
{
	char ch, buf[ 32 ];
	int i = 0;
	int flag = 1;
	/* returns -1 if end of file is reached */

	while(true)
	{
		ch = getc( f );
		if( ch == EOF || (speicial_char_handling && (ch == '*' || ch == '$' )))
			return -1; // * and $ are special characters for comments
		if (isdigit(ch))
			break;
		if (ch == '-')
			flag = -1;
		else
			flag = 1;
	};
	if( ch == EOF ) return -1;

	
	while( isdigit( ch ) ) {
		buf[ i++ ] = ch;
		ch = fgetc( f );
	}
	buf[ i ] = 0;


	return atoi( buf ) * flag;

}


float g_read_float(FILE *f)
   //read a floating point number from the current pointer of the file,
   //skip all spaces

{
   char ch, buf[ 32 ];
   int i = 0;
   int flag = 1;
   

   /* returns -1 if end of file is reached */

   while(true)
      {
      ch = getc( f );
      if( ch == EOF ) return -1;
      if (isdigit(ch))
         break;

      if (ch == '-')
         flag = -1;
      else
         flag = 1;

	  
      };
   if( ch == EOF ) return -1;
   while( isdigit( ch ) || ch == '.' ) 
   {
      buf[ i++ ] = ch;
      ch = fgetc( f );

      }
   buf[ i ] = 0;

   /* atof function converts a character string (char *) into a doubleing
      pointer equivalent, and if the string is not a floting point number,
      a zero will be return.
      */

   return (float)(atof( buf ) * flag);

}

void ReadDSm_pNetworkData(char fname[_MAX_PATH])
{
/*   FILE* st = NULL;
   fopen_s(&st,fname,"r");
   if(st!=NULL)
      {
      g_ODZoneSize = g_read_integer(st);
      int num_nodes= g_read_integer(st);
      int num_links = g_read_integer(st);
      int KSPNumShortestPaths = g_read_integer(st);
      int UseSuperzonesFlag = g_read_integer(st);


      // Read node block
      int i;
      int id = 0, zoneNum = 0;
      DTANode* pNode = 0;
      for(i = 0; i < num_nodes; i++)
         {

         id			= g_read_integer(st);
         zoneNum	= g_read_integer(st);

         // Create and insert the node
         pNode = new DTANode;
         pNode->m_NodeNo = i;
         pNode->m_ZoneID = 0;
         g_NodeSet.insert(pNode);
         g_NodeMap[id] = pNode;
         g_NodeIDtoNameMap[i] = id;
         }

      // Read link block
      DTALink* pLink = 0;
      for(i = 0; i < num_links; i++)
         {
         pLink = new DTALink(g_SimulationHorizon);
         pLink->m_LinkNo = i;
         pLink->m_FromNodeNumber = g_read_integer(st);
         pLink->m_ToNodeNumber = g_read_integer(st);
         pLink->m_FromNodeID = g_NodeMap[pLink->m_FromNodeNumber ]->m_NodeNo;
         pLink->m_ToNodeID= g_NodeMap[pLink->m_ToNodeNumber]->m_NodeNo;

         int m_LeftBays= g_read_integer(st);
         int m_RightBays= g_read_integer(st);

         pLink->m_Length= g_read_float(st);
         pLink->m_NumberOfLanes= g_read_integer(st);

         int m_FlowModel= g_read_integer(st);
         int m_SpeedAdjustment= g_read_integer(st);
         pLink->m_SpeedLimit= g_read_integer(st);
         pLink->m_MaximumServiceFlowRatePHPL= g_read_float(st);
         int m_SaturationFlowRate= g_read_integer(st);
         pLink->m_link_type= g_read_integer(st);
         int m_grade= g_read_integer(st);

         g_LinkSet.insert(pLink);
         g_LinkMap[i]  = pLink;
         }

      fclose(st);
      }
   g_RTNodePredAry = Allocate3DDynamicArray<int>(g_InfoTypeSize,g_NodeSet.size(),g_NodeSet.size());
   g_RTLabelCostAry = Allocate3DDynamicArray<unsigned char>(g_InfoTypeSize,g_NodeSet.size(),g_NodeSet.size());

   if(g_RTNodePredAry==NULL || g_RTLabelCostAry ==NULL)
      {
      cout << "No sufficient memory..."<< endl;
      exit(1);
      }
   cout << "Number of Zones = "<< g_ODZoneSize  << endl;
   cout << "Number of Nodes = "<< g_NodeSet.size() << endl;
   cout << "Number of Links = "<< g_LinkSet.size() << endl;
*/
}

void ReadDSPDestinationData(char fname[_MAX_PATH])
{
/*   FILE* st = NULL;
   fopen_s(&st,fname,"r");
   if(st!=NULL)
      {
      int i;
      int id = 0, zoneNum = 0;
      DTANode* pNode = 0;
      for(i = 0; i < g_ODZoneSize; i++)
         {

         zoneNum	= g_read_integer(st);
         int num_nodes= g_read_integer(st);

         if(num_nodes > g_AdjLinkSize)
            g_AdjLinkSize = num_nodes + 10;  // increaes buffer size


         for(int n = 0; n< num_nodes; n++)
            {
            int node_number= g_read_integer(st);


            g_NodeMap[node_number]->m_ZoneID = zoneNum;
            }
         }
      fclose(st);
      }
*/
}
int g_GetProfileString( LPCTSTR section, LPCTSTR key, LPCTSTR lpdefault, LPTSTR lpbuffer, DWORD nSize, LPCTSTR filename) 
{
   int char_size  = GetPrivateProfileString(section,key,lpdefault, lpbuffer,nSize,filename);

   if(strcmp(lpbuffer,lpdefault) == 0)  //  the parameter might not exist
   {
   WritePrivateProfileString(section,key,lpdefault,filename);
   }

	return char_size; 
} 


int g_GetPrivateProfileInt( LPCTSTR section, LPCTSTR key, int def_value, LPCTSTR filename) 
{

   char lpbuffer[64];
   int value = def_value;
   if(GetPrivateProfileString(section,key,"",lpbuffer,sizeof(lpbuffer),filename)) 
   {
	   value =  atoi(lpbuffer); 
   }

   if(value == def_value)  //  the parameter might not exist
   {
   sprintf_s(lpbuffer,"%d",def_value);
   WritePrivateProfileString(section,key,lpbuffer,filename);
   }
	   return value; 
}



int g_WritePrivateProfileInt(LPCTSTR section, LPCTSTR key, int value, LPCTSTR filename)
{

	char lpbuffer[64];

	sprintf_s(lpbuffer, "%d", value);
	WritePrivateProfileString(section, key, lpbuffer, filename);

	return value;
}
double g_GetPrivateProfileDouble( LPCTSTR section, LPCTSTR key, double def_value, LPCTSTR filename) 
{
   char lpbuffer[64];
   double value = def_value;
   if(GetPrivateProfileString(section,key,"",lpbuffer,sizeof(lpbuffer),filename)) 
   {
	   value = atof(lpbuffer);
   }

   if(value == def_value)  //  the parameter might not exist
   {
   sprintf_s(lpbuffer,"%5.5f",def_value);
   WritePrivateProfileString(section,key,lpbuffer,filename);
   }

	   return value; 
} 

double  g_FindClosestYResolution(double Value)
{
	 vector<double> ResolutionVector;

		 ResolutionVector.push_back(0.001);
		 ResolutionVector.push_back(0.005);
		 ResolutionVector.push_back(0.01);
		 ResolutionVector.push_back(0.05);
		 ResolutionVector.push_back(0.1);
		 ResolutionVector.push_back(0.2);
		 ResolutionVector.push_back(0.5);
		 ResolutionVector.push_back(1);
		 ResolutionVector.push_back(2);
		 ResolutionVector.push_back(5);
		 ResolutionVector.push_back(10);
		 ResolutionVector.push_back(20);
		 ResolutionVector.push_back(50);
		 ResolutionVector.push_back(100);
		 ResolutionVector.push_back(200);
		 ResolutionVector.push_back(500);
		 ResolutionVector.push_back(1000);
		 ResolutionVector.push_back(2000);
		 ResolutionVector.push_back(5000);
		 ResolutionVector.push_back(10000);
		 ResolutionVector.push_back(20000);
		 ResolutionVector.push_back(50000);
		 ResolutionVector.push_back(100000);
		 ResolutionVector.push_back(200000);
		 ResolutionVector.push_back(500000);
		 ResolutionVector.push_back(1000000);
		 
		double min_distance  = 99999999;

		double ClosestResolution=1;
		for(unsigned int i=0; i<ResolutionVector.size();i++)
		{
			if(	fabs(Value-ResolutionVector[i]) < min_distance)
			{
				min_distance = fabs(Value-ResolutionVector[i]);
				ClosestResolution = ResolutionVector[i];
			}
		}
		return ClosestResolution;
}

double  g_FindClosestMOEYResolution(double Value)
{
	 vector<double> ResolutionVector;

	 int j;



		 for(j = 1; j<10; j++)
		 {
		 ResolutionVector.push_back(0.00001*j);
		 }

		 for(j = 1; j<10; j++)
		 {
		 ResolutionVector.push_back(0.0001*j);
		 }

		 for(j = 1; j<10; j++)
		 {
		 ResolutionVector.push_back(0.001*j);
		 }

		 for(j = 1; j<10; j++)
		 {
		 ResolutionVector.push_back(0.01*j);
		 }
		 for(j = 1; j<10; j++)
		 {
		 ResolutionVector.push_back(0.1*j);
		 }
		 for(j = 1; j<10; j++)
		 {
		 ResolutionVector.push_back(1*j);
		 }

		 for(j = 1; j<10; j++)
		 {
		 ResolutionVector.push_back(10*j);
		 }

		 for(j = 1; j<10; j++)
		 {
		 ResolutionVector.push_back(100*j);
		 }

		 for(j = 1; j<10; j++)
		 {
		 ResolutionVector.push_back(1000*j);
		 }

		 for(j = 1; j<10; j++)
		 {
		 ResolutionVector.push_back(10000*j);
		 }

		 for(j = 1; j<10; j++)
		 {
		 ResolutionVector.push_back(100000*j);
		 }

		 for(j = 1; j<10; j++)
		 {
		 ResolutionVector.push_back(1000000*j);
		 }
		 ResolutionVector.push_back(1000000);
		 

		double ClosestResolution=0.000001;
		for(unsigned int i=0; i<ResolutionVector.size();i++)
		{
			ClosestResolution =  ResolutionVector[i];
			if(	Value < ResolutionVector[i])
			{
			break;
			}

		}
		return ClosestResolution;
}

int  g_FindClosestTimeResolution(double Value)
{

		int ResolutionVector[9] = {1,10,30,60,120,240,480,720,1440};
		double min_distance  = 9999999;

		Value = max(1,Value/6);  //1/6 of time horizion as resolution

		int ClosestResolution=1;
		for(int i=0; i<9;i++)
		{
			if(	fabs(Value-ResolutionVector[i]) < min_distance)
			{
				min_distance = fabs(Value-ResolutionVector[i]);
				ClosestResolution = ResolutionVector[i];
			}
		}
		return ClosestResolution;
}