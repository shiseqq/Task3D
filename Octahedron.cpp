#include "StdAfx.h"
#include "Octahedron.h"

COctahedron::COctahedron()
{
	m_pTopPyramid = AcDbMakeUnique<CPyramid>(4, 9.79f, 9.79f);

	m_pBottomPyramid = AcDbMakeUnique<CPyramid>(4, 9.79f, -9.79f);
}

Acad::ErrorStatus COctahedron::AddEntityToBTR(AcDbBlockTableRecord* pBlockTableRecord)
{
    Acad::ErrorStatus es;

    if ((es = m_pTopPyramid->CPyramid::AddEntityToBTR(pBlockTableRecord)) != Acad::eOk) {
        return es;
    }

	if ((es = m_pBottomPyramid->CPyramid::AddEntityToBTR(pBlockTableRecord)) != Acad::eOk) {
		return es;
	}

	return Acad::eOk;
}