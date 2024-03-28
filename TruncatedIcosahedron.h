#pragma once
#include "StdAfx.h"
#include "Polygon.h"
#include "utilities.h"
#include <vector>

class CTruncatedIcosahedron : virtual public AcDb3dPolyline
{
public:
	CTruncatedIcosahedron();
	virtual ~CTruncatedIcosahedron();

	double GetSideLength();

	void Rotate(double angle,
				const AcGeVector3d& axis,
				const AcGePoint3d& center = AcGePoint3d::kOrigin);
	void MoveXY(const double cfX, const double cfY);

	Acad::ErrorStatus AddEntityToBTR(AcDbBlockTableRecord* pBlockTableRecord);

private:
	std::vector<AcDbUniquePtr<AcDb3dPolyline>> m_aHexagons;

	CPolygon* m_pBase;
	const int m_cnNumberOfHexagons = 20;

	std::pair<double, double> GetHexCenterBySide(AcGePoint3d firstPoint, 
												 AcGePoint3d secondPoint);
	
	void BuildFirstLayer();
	void BuildSecondLayer();
	void BuildThirdLayer();
	void BuildFourthLayer();
	void AttachHexToSide(AcGePoint3d firstPoint, 
						 AcGePoint3d secondPoint,
						 double angle = 37.377f);

};