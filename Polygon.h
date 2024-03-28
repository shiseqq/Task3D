#pragma once
#include "StdAfx.h"
#include "utilities.h"
#include <vector>
#include <cmath>
#include <complex>


class CPolygon : virtual public AcDb3dPolyline
{
public:
	CPolygon(int nNumerOfVertices = 6,
		double fCenterX = 0.0f,
		double fCenterY = 0.0f,
		double fCenterZ = 0.0f);
	virtual ~CPolygon() = default;

	AcDb3dPolyline* GetPolyline() { return m_pPolyline.get(); };
	std::vector<AcGePoint3d> GetVertices() { return GetPolylineVertices(m_pPolyline.get()); };

	void MoveXY(const double cfX, const double cfY = 0.0f);
	Acad::ErrorStatus AddEntityToBTR(AcDbBlockTableRecord* pBlockTableRecord);

private:
	AcDbUniquePtr<AcDb3dPolyline> m_pPolyline;

	int m_nNumerOfVertices;

	double m_fCenterX;
	double m_fCenterY;
	double m_fCenterZ;

	void FillVertices();
};