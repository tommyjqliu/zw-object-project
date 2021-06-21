#include "pch.h"

#include "tchar.h"
#include <aced.h>
#include <rxregsvc.h>
#include <dbproxy.h>
#include "SampleCustEnt.h"

//运行时类型识别函数的实现
ACRX_DXF_DEFINE_MEMBERS(
	SampleCustEnt, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, SAMPLECUSTENT,
	ARXPROJECT1APP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

Adesk::UInt32 SampleCustEnt::kCurrentVersionNumber = 1;

SampleCustEnt::SampleCustEnt() {

}
SampleCustEnt::SampleCustEnt(AcGePoint3d center, double radius) {
	m_center = center;
	m_radius = radius;
}
SampleCustEnt::~SampleCustEnt() {

}

void SampleCustEnt::setCenter(AcGePoint3d center)
{
	assertWriteEnabled();
	m_center = center;
}

void SampleCustEnt::setRadius(double radius)
{
	assertWriteEnabled();
	m_radius = radius;
}


Adesk::Boolean SampleCustEnt::subWorldDraw(AcGiWorldDraw *mode) {
	assertReadEnabled();
	mode->geometry().circle(m_center, m_radius, AcGeVector3d::kZAxis);
	return (AcDbEntity::subWorldDraw(mode));
}

//视口相关的显示
void SampleCustEnt::subViewportDraw(AcGiViewportDraw* mode) {

}

//设置显示相关的属性
Adesk::UInt32 SampleCustEnt::subSetAttributes(AcGiDrawableTraits *traits) {
	assertReadEnabled();
	traits->setColor(0);
	return 0;
}

Acad::ErrorStatus SampleCustEnt::subTransformBy(const AcGeMatrix3d& xform) {
	assertWriteEnabled();
	m_center = m_center.transformBy(xform);
	return Acad::eOk;
}

Acad::ErrorStatus SampleCustEnt::subGetGripPoints(AcGePoint3dArray& gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds) const {
	assertReadEnabled();
	AcGePoint3d left(m_center.x - m_radius, m_center.y, 0);
	AcGePoint3d right(m_center.x + m_radius, m_center.y, 0);
	AcGePoint3d up(m_center.x, m_center.y + m_radius, 0);
	AcGePoint3d down(m_center.x, m_center.y - m_radius, 0);

	gripPoints.append(m_center);
	gripPoints.append(left);
	gripPoints.append(right);
	gripPoints.append(up);
	gripPoints.append(down);
	return Acad::eOk;
}
Acad::ErrorStatus SampleCustEnt::subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d& offset) {
	assertWriteEnabled();
	AcGeVector3d original;
	AcGeVector3d selectVector;
	double diff = 0;
	switch (indices[0]) {
	case 0:
		m_center += offset;
		break;
	case 1:
		selectVector = AcGeVector3d(-m_radius, 0, 0);
		m_radius = (selectVector + offset).length();
		break;
	case 2:
		selectVector = AcGeVector3d(m_radius, 0, 0);
		m_radius = (selectVector + offset).length();
		break;
	case 3:
		selectVector = AcGeVector3d(0, m_radius, 0);
		m_radius = (selectVector + offset).length();
		break;
	case 4:
		selectVector = AcGeVector3d(0, -m_radius, 0);
		m_radius = (selectVector + offset).length();
		break;
	}
	
	return Acad::eOk;
}

Acad::ErrorStatus SampleCustEnt::subGetOsnapPoints(AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const AcGePoint3d & pickPoint, const AcGePoint3d & lastPoint, const AcGeMatrix3d & viewXform, AcGePoint3dArray & snapPoints, AcDbIntArray & geomIds) const
{
	assertReadEnabled();
	AcDbCircle circle;
	circle.setCenter(m_center);
	circle.setRadius(m_radius);
	return circle.getOsnapPoints(osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds);
}


Acad::ErrorStatus SampleCustEnt::dwgInFields(AcDbDwgFiler * pFiler)
{
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dwgInFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
		return (es);
	if (version > SampleCustEnt::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	if (version < SampleCustEnt::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//----- Read params
	pFiler->readPoint3d(&m_center);
	pFiler->readDouble(&m_radius);
	return (pFiler->filerStatus());

}



Acad::ErrorStatus SampleCustEnt::dwgOutFields(AcDbDwgFiler * pFiler) const
{
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dwgOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(SampleCustEnt::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	pFiler->writePoint3d(m_center);
	pFiler->writeDouble(m_radius);
	return (pFiler->filerStatus());

}
