#pragma once
#include "StdAfx.h"
#include <vector>

template<class T>
struct Deleter {
    void operator()(T* p) const {
        p->close();
        delete p;
    }
};

template<class T>
using AcDbUniquePtr = std::unique_ptr<T, Deleter<T>>;

template<class T, class... Args>
AcDbUniquePtr<T> AcDbMakeUnique(Args&&... args) {
    return AcDbUniquePtr<T>(new T(std::forward<Args>(args)...), Deleter<T>());
}

const double deg2rad(const double deg);

std::vector<AcGePoint3d> GetPolylineVertices(AcDb3dPolyline* m_pPline);

AcDbUniquePtr<AcDb3dPolyline> MovePolyline(AcDb3dPolyline* pPolyline, 
											 const double cfX, 
											 const double cfY = 0.0f);
