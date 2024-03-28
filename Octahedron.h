#pragma once
#include "StdAfx.h"
#include "Pyramid.h"

class COctahedron : virtual public CPyramid
{
public:
	COctahedron();
	virtual ~COctahedron() = default;

	AcDb3dPolyline* GetBase() { return m_pTopPyramid->GetBase();  }

	Acad::ErrorStatus AddEntityToBTR(AcDbBlockTableRecord* pBlockTableRecord);

private:
	AcDbUniquePtr<CPyramid> m_pTopPyramid;
	AcDbUniquePtr<CPyramid>  m_pBottomPyramid;
};