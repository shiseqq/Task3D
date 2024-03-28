#pragma once
#include "TruncIcosInPyr.h"
#include "Octahedron.h"
#include "utilities.h"
#include <vector>

class CStarOctahedron :virtual public COctahedron
{
public:
	CStarOctahedron();
	virtual ~CStarOctahedron() = default;

	Acad::ErrorStatus AddEntityToBTR(AcDbBlockTableRecord* pBlockTableRecord);

private:
	AcDbUniquePtr<COctahedron> m_pOctahedron;
	//AcDbUniquePtr <COctahedron> m_pOctahedron; 
	std::vector<AcDbUniquePtr<ÑTruncIcosInPyr>> m_aStars;

	const int m_cnNumberOfStars = 8;

	void FillStars();
};
