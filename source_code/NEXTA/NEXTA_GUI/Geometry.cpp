#include "StdAfx.h"
#include "Geometry.h"

#include <sstream>
using std::istringstream;


CGeometry::CGeometry(string s)
{
	m_NumOfCoordinates = 0;

	string tmp = s.substr(s.find_first_not_of(' '));
	size_t start_idx = tmp.find_first_of('<');
	size_t end_idx = tmp.find_first_of('>');

	if(start_idx==std::string::npos || end_idx==std::string::npos )
		return;

	string type_str = tmp.substr(start_idx + 1,end_idx - start_idx -1);
	
	string start_tag = "<coordinates>";
	string end_tag = "</coordinates>";
	
	start_idx = tmp.find(start_tag);
	start_idx += start_tag.length();
	end_idx = tmp.find(end_tag);

	tmp = tmp.substr(start_idx, end_idx - start_idx);

	if (type_str.compare("Point") == 0)
	{
		m_Type = POINT;
	}
	else if (type_str.compare("LineString") == 0)
	{
			m_Type = LINE;
	}else if ( type_str.compare("Polygon") == 0)
	{
		m_Type = POLYGON;
	}else
	{
			m_Type = UNKNOWN;
	}

	switch (m_Type)
	{
		case POINT:
			ReadPointCoordinate(tmp);
			break;
		case LINE:
			ReadLineStringCoordinates(tmp);
			break;
		case POLYGON:
			ReadLineStringCoordinates(tmp);
			break;
		default:
			break;
	}

}

CGeometry::~CGeometry(void)
{
}

CGeometry::GeometryType CGeometry::GetGeometryType(void)
{
	return m_Type;
}

int CGeometry::GetNumberOfCoordinates(void)
{
	return m_NumOfCoordinates;
}

std::vector<CCoordinate> CGeometry::GetCoordinateList(void)
{
	return v_Coordinates;
}

bool CGeometry::ReadLineStringCoordinates(string s)
{
	istringstream ss(s);
	string sub_str;

	if(std::string::npos == s.find_first_of("0123456789"))
	{
			// "digit not found!, empty string//
		return false;
	}

	while(std::getline(ss,sub_str, ' '))
	{


		CCoordinate coordinate;
		istringstream sub_ss(sub_str);
		string tmp;

		std::getline(sub_ss,tmp,',');
		istringstream x_ss(tmp);
		x_ss >> coordinate.X;
		
		std::getline(sub_ss,tmp,',');
		istringstream y_ss(tmp);
		y_ss >> coordinate.Y;

		std::getline(sub_ss,tmp,',');
		istringstream z_ss(tmp);
		z_ss >> coordinate.Z;

		v_Coordinates.push_back(coordinate);
		m_NumOfCoordinates += 1;
	}
	return true;
}

bool CGeometry::ReadPolygonCoordinates(string s)
{
	istringstream ss(s);
	string sub_str;

	while(std::getline(ss,sub_str, ' '))
	{
		CCoordinate coordinate;
		istringstream sub_ss(sub_str);
		string tmp;

		std::getline(sub_ss,tmp,',');
		istringstream x_ss(tmp);
		x_ss >> coordinate.X;
		
		std::getline(sub_ss,tmp,',');
		istringstream y_ss(tmp);
		y_ss >> coordinate.Y;

		std::getline(sub_ss,tmp,',');
		istringstream z_ss(tmp);
		z_ss >> coordinate.Z;

		v_Coordinates.push_back(coordinate);
		m_NumOfCoordinates += 1;
	}
	return true;
}
bool CGeometry::ReadPointCoordinate(string s)
{
	CCoordinate coordinate;
	istringstream ss(s);

	string sub_str;
	std::getline(ss,sub_str,',');
	istringstream x_ss(sub_str);

	std::getline(ss,sub_str,',');
	istringstream y_ss(sub_str);
	x_ss >> coordinate.X;
	y_ss >> coordinate.Y;
	coordinate.Z = 0.0;

	v_Coordinates.push_back(coordinate);
	m_NumOfCoordinates = 1;

	return true;
}


