#include "StdAfx.h"
#include "TruncIcosInPyr.h"

ÑTruncIcosInPyr::ÑTruncIcosInPyr()
{
	double fFirstPeakDistance = 8.0f;
	double fBaseHeight = fFirstPeakDistance * 3.0f/2;
	double fPyrSide = fBaseHeight * sqrt(4.0f / 3);
	double fPyrHeight = (double)sqrt(2.0f / 3) * fPyrSide;
	int nNumberOfBaseVertices = 3;

	m_pPyramid = AcDbMakeUnique <CPyramid>(nNumberOfBaseVertices, fFirstPeakDistance, fPyrHeight);
	
	m_pTruncIcos = AcDbMakeUnique <CTruncatedIcosahedron>();
}

void ÑTruncIcosInPyr::Rotate(double fAngle, const AcGeVector3d& cAxis, const AcGePoint3d& cCenter)
{
	m_pPyramid->Rotate(fAngle, cAxis, cCenter);
	m_pTruncIcos->Rotate(fAngle, cAxis, cCenter);
}

void ÑTruncIcosInPyr::MoveXY(const double cfX, const double cfY)
{
	m_pPyramid->MoveXY(cfX, cfY);
	m_pTruncIcos->MoveXY(cfX, cfY);
}

Acad::ErrorStatus ÑTruncIcosInPyr::AddEntityToBTR(AcDbBlockTableRecord* pBlockTableRecord)
{
	Acad::ErrorStatus es;

	if ((es = m_pPyramid->AddEntityToBTR(pBlockTableRecord)) != Acad::eOk) 
	{
		return es;
	}

	if ((es = m_pTruncIcos->AddEntityToBTR(pBlockTableRecord)) != Acad::eOk) 
	{
		return es;
	}

	return Acad::eOk;
}