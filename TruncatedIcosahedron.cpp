#include "StdAfx.h"
#include "TruncatedIcosahedron.h"

CTruncatedIcosahedron::CTruncatedIcosahedron()
{
    m_pBase = new CPolygon(5);

    BuildFirstLayer();
    BuildSecondLayer();
    BuildThirdLayer();
    BuildFourthLayer();
}

CTruncatedIcosahedron::~CTruncatedIcosahedron()
{
    delete m_pBase;
}

Acad::ErrorStatus CTruncatedIcosahedron::AddEntityToBTR(AcDbBlockTableRecord* pBlockTableRecord)
{
    for (auto& i : m_aHexagons) 
    {
        pBlockTableRecord->appendAcDbEntity(i.get());
    }
    return Acad::eOk;
}

std::pair<double, double> CTruncatedIcosahedron::GetHexCenterBySide(AcGePoint3d firstPoint,
                                                                    AcGePoint3d secondPoint)
{
    double fX1 = firstPoint[0], fY1 = firstPoint[1];
    double fX2 = secondPoint[0], fY2 = secondPoint[1];

    double fX3 = (fX1 + fX2 - (fY1 - fY2) * sqrt(3)) / 2;
    double fY3 = (fY1 + fY2 - (fX2 - fX1) * sqrt(3)) / 2;
    
    return { fX3, fY3 };
}

void CTruncatedIcosahedron::AttachHexToSide(AcGePoint3d firstPoint, AcGePoint3d secondPoint, double angle)
{
    AcDb3dPolyline* pHexagon = new AcDb3dPolyline(AcDb::Poly3dType::k3dSimplePoly,
                                                  AcGePoint3dArray(),
                                                  Adesk::kTrue);
    const int cnNumerHexVertices = 6;

    auto [fCenterX, fCenterY] = GetHexCenterBySide(firstPoint, secondPoint);

    pHexagon->appendVertex(new AcDb3dPolylineVertex(secondPoint));
    pHexagon->appendVertex(new AcDb3dPolylineVertex(firstPoint));

    std::complex<double>  fVertexRadius(firstPoint[0] - fCenterX, firstPoint[1] - fCenterY);
    for (int i = 0; i < cnNumerHexVertices - 2; ++i)
    {
        fVertexRadius *= std::polar(1.0f, 2*acos(-1.0f) / cnNumerHexVertices);
        std::complex<double> fNextVertex = std::complex<double>(fCenterX, fCenterY) + fVertexRadius;
        AcDb3dPolylineVertex* pVertex = new AcDb3dPolylineVertex(AcGePoint3d(fNextVertex.real(),
                                                                             fNextVertex.imag(),
                                                                             firstPoint[2]));
        
        pHexagon->appendVertex(pVertex);
    }

    AcGeMatrix3d rotationMatrix;
    rotationMatrix.setToRotation(deg2rad(angle), firstPoint - secondPoint, secondPoint);
    pHexagon->transformBy(rotationMatrix);
    m_aHexagons.emplace_back(std::move(pHexagon));
}

void CTruncatedIcosahedron::BuildFirstLayer() 
{
    std::vector <AcGePoint3d> aPentVertices = m_pBase->GetVertices();

    AttachHexToSide(aPentVertices[0], aPentVertices[1]);

    for (int i = 1; i < aPentVertices.size() - 1; ++i) 
    {
        AttachHexToSide(aPentVertices[i], aPentVertices[i + 1]);
    }

    AttachHexToSide(aPentVertices[aPentVertices.size() - 1], aPentVertices[0]);
}

void CTruncatedIcosahedron::BuildSecondLayer() 
{
    std::vector<std::vector <AcGePoint3d>> aVertices;

    for (auto& i : m_aHexagons) 
    {
        aVertices.emplace_back(GetPolylineVertices(i.get()));
    }

    for (auto& i : aVertices) 
    {
        AttachHexToSide(i[3], i[4], 79.188f);
    }
}

void CTruncatedIcosahedron::BuildThirdLayer()
{
    std::vector<std::vector <AcGePoint3d>> aVertices;

    for (int i = 5; i < m_aHexagons.size(); ++i) 
    {
        aVertices.push_back(GetPolylineVertices(&(*m_aHexagons[i])));
    }

    for (int i = 0; i < aVertices.size() - 1; ++i) 
    {
        AttachHexToSide(aVertices[i][5], aVertices[i + 1][2], 100.812f);
    }

    AttachHexToSide(aVertices[aVertices.size() - 1][5], aVertices[0][2], 100.812f);
}

void CTruncatedIcosahedron::BuildFourthLayer()
{
    std::vector<std::vector <AcGePoint3d>> aVertices;

    for (int i = 10; i < m_aHexagons.size(); ++i) 
    {
        aVertices.push_back(GetPolylineVertices(&(*m_aHexagons[i])));
    }

    for (int i = 0; i < aVertices.size(); ++i) 
    {
        AttachHexToSide(aVertices[i][3], aVertices[i][4], 142.623f);
    }
}

double CTruncatedIcosahedron::GetSideLength()
{
    std::vector <AcGePoint3d> aVertices = GetPolylineVertices(&(*m_aHexagons[0]));
    AcGePoint3d firstPoint = aVertices[0];
    AcGePoint3d secondPoint = aVertices[1];

    return sqrt((secondPoint[0] - firstPoint[0]) * (secondPoint[0] - firstPoint[0])
              + (secondPoint[1] - firstPoint[1]) * (secondPoint[1] - firstPoint[1])
              + (secondPoint[2] - firstPoint[2]) * (secondPoint[2] - firstPoint[2]));
}

void CTruncatedIcosahedron::Rotate(double angle, const AcGeVector3d& axis, const AcGePoint3d& center)
{
    AcGeMatrix3d rotationMatrix;
    rotationMatrix.setToRotation(deg2rad(angle), axis, center);

    for (auto& i : m_aHexagons) 
    {
        i->transformBy(rotationMatrix);
    }
}

void CTruncatedIcosahedron::MoveXY(const double cfX, const double cfY)
{
    for (auto& i : m_aHexagons) 
    {
        i = MovePolyline(i.get(), cfX, cfY);
    }
}