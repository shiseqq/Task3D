#include "StdAfx.h"
#include "resource.h"

#include "Polygon.h"
#include "Pyramid.h"
#include "Octahedron.h"
#include "TruncatedIcosahedron.h"
#include "TruncIcosInPyr.h"
#include "StarOctahedron.h"

#define szRDS _RXST("ADSK")

class CTask3DApp : public AcRxArxApp {

public:
	CTask3DApp() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void* pkt) {
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);
		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void* pkt) {
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);
		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}

	static void Asdk_CREATE(void) {
		AcDbBlockTable* pBlockTable;
		Acad::ErrorStatus es;
		if ((es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead)) != Acad::eOk) {
			acutPrintf(_T("\nCouldn't open the block table!"));
			return;
		}

		AcDbBlockTableRecord* pBlockTableRecord = new AcDbBlockTableRecord;
		pBlockTableRecord->setName(_T("StarOctahedron"));
		pBlockTableRecord->setOrigin(AcGePoint3d::kOrigin);

		CStarOctahedron* starOct = new CStarOctahedron;
		starOct->AddEntityToBTR(pBlockTableRecord);

		pBlockTable->upgradeOpen();
		pBlockTable->add(pBlockTableRecord);
		pBlockTable->close();

		pBlockTableRecord->close();
		starOct->close();

		return;
	}
};

IMPLEMENT_ARX_ENTRYPOINT(CTask3DApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CTask3DApp, Asdk, _CREATE, CREATE, ACRX_CMD_TRANSPARENT, NULL)