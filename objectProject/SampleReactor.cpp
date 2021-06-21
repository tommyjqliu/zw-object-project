
#include "pch.h"
#include <dbproxy.h>
#include "SampleReactor.h"
#include "SampleCustEnt.h"
#include <acestext.h>

ACRX_DXF_DEFINE_MEMBERS(
	SampleReactor, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, SAMPLEOBJECT,
	ARXPROJECT1APP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

SampleReactor::SampleReactor()
{
	acutPrintf(_T("\nreactor created"));
}

SampleReactor::~SampleReactor()
{
}

void SampleReactor::eLinkage(AcDbObjectId i, double f)
{	
	mId = i; 
	mFactor = f; 
}

void SampleReactor::modified(const AcDbObject* pObj) {
	SampleCustEnt *pCustObj = SampleCustEnt::cast(pObj);
	if (!pCustObj) {
		return;//反应器很容易附着到错误的实体上，要注意检查
	}
	acutPrintf(_T("\nemit reactor"));
	SampleCustEnt *pCustObj2 = nullptr;
	auto es = acdbOpenObject((AcDbObject*&)pCustObj2, mId, AcDb::kForWrite);
	acutPrintf(_T("\nError Code: %s"), acadErrorStatusText(es));

	if (es == Acad::eOk) {
		pCustObj2->setRadius(pCustObj->m_radius);
		if (pCustObj->isErased() == true) {
			pCustObj2->erase();
		}
		pCustObj2->close();
	}
}

Acad::ErrorStatus SampleReactor::dwgInFields(AcDbDwgFiler* filer) {
	assertWriteEnabled();
	AcDbObject::dwgInFields(filer);
	filer->readItem(&mFactor);
	filer->readItem((AcDbSoftPointerId*)&mId);
	return filer->filerStatus();
}
Acad::ErrorStatus SampleReactor::dwgOutFields(AcDbDwgFiler* filer) const {
	assertReadEnabled();
	AcDbObject::dwgOutFields(filer);
	filer->writeItem(mFactor);
	filer->writeItem((AcDbSoftPointerId&)mId);
	return filer->filerStatus();
}
