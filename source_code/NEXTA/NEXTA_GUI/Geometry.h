#pragma once
#include <string>
#include <vector>

using std::string;

struct GDPoint;

typedef struct
{
	double X,Y,Z;
}CCoordinate;

class CGeometry
{
public:
	enum GeometryType {
		POINT, 
		LINE,
		POLYGON,
		UNKNOWN
	};
private:
	GeometryType m_Type;
	int m_NumOfCoordinates;
	std::vector<CCoordinate> v_Coordinates;
	bool ReadPointCoordinate(string s);
	bool ReadLineStringCoordinates(string s);
	bool ReadPolygonCoordinates(string s);

public:
	CGeometry(string s);
	~CGeometry(void);

	GeometryType GetGeometryType(void);
	std::vector<CCoordinate> GetCoordinateList(void);
	int GetNumberOfCoordinates(void);
};




 