#include "StdAfx.h"
#include "Polygon.h"

CPolygon::CPolygon(int nNumerOfVertices, double fCenterX, double fCenterY, double fCenterZ) :
    m_nNumerOfVertices(nNumerOfVertices),
    m_fCenterX(fCenterX),
    m_fCenterY(fCenterY),
    m_fCenterZ(fCenterZ)
{
    m_pPolyline = AcDbMakeUnique<AcDb3dPolyline>(AcDb::Poly3dType::k3dSimplePoly,
                                                   AcGePoint3dArray(),
                                                   Adesk::kTrue);

    CPolygon::FillVertices();
}

void CPolygon::FillVertices()
{
    std::complex<double>  fFirstVertex(0.0f, 1.0f);
    std::complex<double>  fVertexRadius(fFirstVertex.real() - m_fCenterX, 
                                        fFirstVertex.imag() - m_fCenterY);

    m_pPolyline->appendVertex(new AcDb3dPolylineVertex(AcGePoint3d(fFirstVertex.real(), 
                                                                   fFirstVertex.imag(),
                                                                   m_fCenterZ)));

    for (int i = 0; i < m_nNumerOfVertices - 1; ++i)
    {
        fVertexRadius *= std::polar(1.0f, 2*acos(-1.0f) / m_nNumerOfVertices);
        std::complex<double> fNextVertex = std::complex<double>(m_fCenterX, m_fCenterY) 
                                         + fVertexRadius;
        AcDb3dPolylineVertex* pVertex = new AcDb3dPolylineVertex(AcGePoint3d(fNextVertex.real(),
                                                                             fNextVertex.imag(),
                                                                             m_fCenterZ));

        m_pPolyline->appendVertex(pVertex);
    }
}

void CPolygon::MoveXY(const double cfX, const double cfY) 
{
    AcDbUniquePtr <AcDb3dPolyline> pNewShape = AcDbMakeUnique <AcDb3dPolyline>(AcDb::Poly3dType::k3dSimplePoly,
                                                                                   AcGePoint3dArray(),
                                                                                   Adesk::kTrue);
    std::vector <AcGePoint3d> aShapeVertices = GetVertices();

    for (auto& i : aShapeVertices) 
    {
        pNewShape->appendVertex(new AcDb3dPolylineVertex(AcGePoint3d(i[0] + cfX,
                                                                     i[1] + cfY, 
                                                                     i[2])));
    }

    m_pPolyline = std::move(pNewShape);
}

Acad::ErrorStatus CPolygon::AddEntityToBTR(AcDbBlockTableRecord* pBlockTableRecord)
{
    return pBlockTableRecord->appendAcDbEntity(m_pPolyline.get());
}