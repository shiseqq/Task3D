#include "StdAfx.h"
#include "Pyramid.h"

CPyramid::CPyramid(int nNumberOfBaseVertices, double fFirstPeakDistance, double fHeight) :
    m_nNumberOfBaseVertices(nNumberOfBaseVertices),
    m_fFirstPeakDistance(fFirstPeakDistance),
    m_fHeight(fHeight)
{
    m_pPyramidBase = AcDbMakeUnique<AcDb3dPolyline>(AcDb::Poly3dType::k3dSimplePoly,
                                                      AcGePoint3dArray(),
                                                      Adesk::kTrue);

    FillVertices();
}

void CPyramid::FillVertices()
{
    std::complex<double>  fVertexRadius(0, m_fFirstPeakDistance);

    m_pPyramidBase->appendVertex(new AcDb3dPolylineVertex(AcGePoint3d(0.0f, m_fFirstPeakDistance, 0.0f)));

    for (int i = 0; i < m_nNumberOfBaseVertices - 1; ++i)
    {
        fVertexRadius *= std::polar(1.0f, 2*acos(-1.0f) / m_nNumberOfBaseVertices);
        std::complex<double> fNextVertex = std::complex<double>(0.0f, 0.0f) + fVertexRadius;
        AcDb3dPolylineVertex* pVertex = new AcDb3dPolylineVertex(AcGePoint3d(fNextVertex.real(),
                                                                             fNextVertex.imag(),
                                                                             0.0f));
        m_pPyramidBase->appendVertex(pVertex);
    }

    FillEdges();
}

void CPyramid::FillEdges()
{
    std::vector <AcGePoint3d> aBaseVertices = GetPolylineVertices(m_pPyramidBase.get());

    m_aEdges.resize(aBaseVertices.size());

    for (int i = 0; i < aBaseVertices.size(); ++i) 
    {
        m_aEdges[i] = AcDbMakeUnique<AcDb3dPolyline>(AcDb::Poly3dType::k3dSimplePoly, AcGePoint3dArray(), Adesk::kFalse);
        m_aEdges[i]->appendVertex(new AcDb3dPolylineVertex((AcGePoint3d)aBaseVertices[i]));
        m_aEdges[i]->appendVertex(new AcDb3dPolylineVertex(AcGePoint3d(0.0f, 0.0f, m_fHeight)));
    }
}

Acad::ErrorStatus CPyramid::AddEntityToBTR(AcDbBlockTableRecord* pBlockTableRecord)
{
    Acad::ErrorStatus es;

    if ((es = pBlockTableRecord->appendAcDbEntity(m_pPyramidBase.get())) != Acad::eOk) {
        return es;
    }

    for (auto& i : m_aEdges)
    {
        if ((es = pBlockTableRecord->appendAcDbEntity(i.get())) != Acad::eOk) {
            return es;
        }
    }
    return Acad::eOk;
}


void CPyramid::Rotate(double fAngle, const AcGeVector3d& cAxis,const AcGePoint3d& cCenter)
{
    AcGeMatrix3d rotationMatrix;
    rotationMatrix.setToRotation(deg2rad(fAngle), cAxis, cCenter);

    m_pPyramidBase->transformBy(rotationMatrix);

    for (auto& i : m_aEdges) 
    {
        i->transformBy(rotationMatrix);
    }
}


void CPyramid::MoveXY(const double cfX, const double cfY)
{
    m_pPyramidBase = MovePolyline(m_pPyramidBase.get(), cfX, cfY);

    for (auto& i : m_aEdges) 
    {
        i = MovePolyline(i.get(), cfX, cfY);
    }
}