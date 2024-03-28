#pragma once
#include "StdAfx.h"
#include "Polygon.h"
#include "utilities.h"
#include <cmath>
#include <complex>
#include <vector>

class CPyramid : virtual  public AcDb3dPolyline
{
public:
	CPyramid(int nNumberOfBaseVertices = 4, double fFirstPeakDistance = 1.0f, double fHeight = 2.0f);
	virtual ~CPyramid() = default;

	AcDb3dPolyline* GetBase() { return m_pPyramidBase.get(); }

	void Rotate(double fAngle,
		        const AcGeVector3d& cAxis,
		        const AcGePoint3d& cCenter = AcGePoint3d::kOrigin);
	void MoveXY(const double cfX, const double cfY = 0.0f);

	Acad::ErrorStatus AddEntityToBTR(AcDbBlockTableRecord* pBlockTableRecord);

private:
	AcDbUniquePtr<AcDb3dPolyline> m_pPyramidBase;
	std::vector<AcDbUniquePtr<AcDb3dPolyline>> m_aEdges;

	int m_nNumberOfBaseVertices;
	double m_fHeight;
	double m_fFirstPeakDistance;
	
	void FillVertices();
	void FillEdges();
};
