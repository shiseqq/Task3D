#pragma once
#include "StdAfx.h"
#include "Pyramid.h"
#include "TruncatedIcosahedron.h"

class ÑTruncIcosInPyr : virtual public CPyramid
{
public:
	ÑTruncIcosInPyr();
	virtual ~ÑTruncIcosInPyr() = default;

	CPyramid* GetPyramid() { return m_pPyramid.get(); }

	void Rotate(double fAngle, 
				const AcGeVector3d& cAxis, 
				const AcGePoint3d& cCenter = AcGePoint3d::kOrigin);
	void MoveXY(const double cfX, const double cfY);

	Acad::ErrorStatus AddEntityToBTR(AcDbBlockTableRecord* pBlockTableRecord);

private:
	AcDbUniquePtr <CPyramid> m_pPyramid;
	AcDbUniquePtr <CTruncatedIcosahedron> m_pTruncIcos;
};