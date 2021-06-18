
#include "pch.h"
#include "SampleReactor.h"

ACRX_CONS_DEFINE_MEMBERS(SampleReactor, AcDbObject, 1)

void SampleReactor::modified(const AcDbObject* pObj) {
	AcDbLine *pLine = AcDbLine::cast(pObj);
	if (!pLine) {
		return;//反应器很容易附着到错误的实体上，要注意检查
	}
	AcDbLine *pLine2 = nullptr;
	if (acdbOpenObject((AcDbObject*&)pLine2, mId, AcDb::kForWrite) == Acad::eOk) {
		AcGePoint3d p = pLine->startPoint();
		AcGePoint3d q = pLine->endPoint();
		AcGeVector3d v = q - p;
		double len = v.length();
		p = pLine2->startPoint();
		q = pLine2->endPoint();
		v = q - p;
		v = len * mFactor * v.normal();
		pLine2->setEndPoint(p + v);
		pLine2->close();
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
