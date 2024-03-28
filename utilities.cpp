#include "StdAfx.h"
#include "utilities.h"

std::vector<AcGePoint3d> GetPolylineVertices(AcDb3dPolyline* pPolyline) {
    std::vector<AcGePoint3d> aVertices;
    AcDbObjectIterator* pVertIter = pPolyline->vertexIterator();
    AcDb3dPolylineVertex* pVertex;
    AcGePoint3d location;
    AcDbObjectId vertexObjId;

    for (pVertIter->start(); !pVertIter->done(); pVertIter->step())
    {
        vertexObjId = pVertIter->objectId();
        pVertex = (AcDb3dPolylineVertex*)pVertIter->entity();

        location = pVertex->position();
        aVertices.emplace_back(location);

        pVertex->close();
    }

    delete pVertIter;
    return aVertices;
}

AcDbUniquePtr<AcDb3dPolyline> MovePolyline(AcDb3dPolyline* pPolyline, const double cfX, const double cfY)
{
    AcDbUniquePtr<AcDb3dPolyline> newBase = AcDbMakeUnique<AcDb3dPolyline>(AcDb::Poly3dType::k3dSimplePoly,
                                                                               AcGePoint3dArray(),
                                                                               Adesk::kTrue);
    std::vector <AcGePoint3d> aBaseVertices = GetPolylineVertices(pPolyline);

    for (auto& i : aBaseVertices)
    {
        newBase->appendVertex(new AcDb3dPolylineVertex(AcGePoint3d(i[0] + cfX, 
                                                                   i[1] + cfY, 
                                                                   i[2])));
    }

    return newBase;
}

const double deg2rad(const double deg)
{
    return deg * acos(-1.0f) / 180.0;
}