#include "StdAfx.h"
#include "StarOctahedron.h"

CStarOctahedron::CStarOctahedron()
{
	m_pOctahedron = AcDbMakeUnique<COctahedron>();

	FillStars();
}

void CStarOctahedron::FillStars() 
{
	std::vector<std::pair<double, double>> CoordinateSigns =
	{ {1.0f, 1.0f},
	  {1.0f, -1.0f},
	  {-1.0f, -1.0f},
	  {-1.0f, 1.0f} };

	for (int i = 0; i < m_cnNumberOfStars; ++i)
	{
		AcDbUniquePtr<ÑTruncIcosInPyr> pIcosInPyr = AcDbMakeUnique<ÑTruncIcosInPyr>();

		double fRotationAngle = 15.0f + (double)i * 30.0f;
		pIcosInPyr->Rotate(fRotationAngle, AcGeVector3d(0, 0, 1));

		std::vector<AcGePoint3d> BasePyrVertices = GetPolylineVertices(pIcosInPyr->GetPyramid()->GetBase());
		AcGePoint3d firstPyrPoint = BasePyrVertices[(0 + (i * 2)) % 3];
		AcGePoint3d secondPyrPoint = BasePyrVertices[(2 + (i * 2)) % 3];

		std::vector<AcGePoint3d> BaseOctVertices = GetPolylineVertices(m_pOctahedron->GetBase());
		AcGePoint3d firstOctPoint = BaseOctVertices[(0 + (i * 3)) % 4];
		AcGePoint3d secondOctPoint = BaseOctVertices[(3 + (i * 3)) % 4];

		double fX = (abs(firstOctPoint[0] - firstPyrPoint[0]) + abs(secondOctPoint[0] - secondPyrPoint[0])) / 2;
		double fY = (abs(firstOctPoint[1] - firstPyrPoint[1]) + abs(secondOctPoint[1] - secondPyrPoint[1])) / 2;
		fX *= CoordinateSigns[i % 4].first;;
		fY *= CoordinateSigns[i % 4].second;
		pIcosInPyr->MoveXY(fX, fY);

		std::vector<AcGePoint3d> BaseVertices = GetPolylineVertices(pIcosInPyr->GetPyramid()->GetBase());
		AcGePoint3d firstPoint = BaseVertices[(0 + (i * 2)) % 3];
		AcGePoint3d secondPoint = BaseVertices[(2 + (i * 2)) % 3];

		if (i >= m_cnNumberOfStars / 2)
		{
			pIcosInPyr->Rotate(234.7f, firstPoint - secondPoint, secondPoint);
		}
		else
		{
			pIcosInPyr->Rotate(54.7f, firstPoint - secondPoint, secondPoint);
		}
		m_aStars.emplace_back(std::move(pIcosInPyr));
	}
}

Acad::ErrorStatus CStarOctahedron::AddEntityToBTR(AcDbBlockTableRecord* pBlockTableRecord)
{
	Acad::ErrorStatus es;

	if ((es = m_pOctahedron->AddEntityToBTR(pBlockTableRecord)) != Acad::eOk) {
		return es;
	}

	for (auto& i : m_aStars) 
	{
		if ((es = i->AddEntityToBTR(pBlockTableRecord)) != Acad::eOk) {
			return es;
		}
	}

	return Acad::eOk;
}