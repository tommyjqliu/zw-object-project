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



SampleCustEnt::SampleCustEnt() {

}
SampleCustEnt::SampleCustEnt(AcGePoint3d center, double radius) {
	m_center = center;
	m_radius = radius;
	AcGePoint3d left(m_center.x - m_radius, m_center.y, 0);
	m_left = left;
	AcGePoint3d right(m_center.x + m_radius, m_center.y, 0);
	m_right = right;
	AcGePoint3d up(m_center.x, m_center.y + m_radius, 0);
	m_up = up;
	AcGePoint3d down(m_center.x, m_center.y - m_radius, 0);
	m_down = down;
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
	return 0;
}



Acad::ErrorStatus SampleCustEnt::subGetGripPoints(AcGePoint3dArray& gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds) const {
	assertReadEnabled();
	gripPoints.append(m_center);
	gripPoints.append(m_left);
	gripPoints.append(m_right);
	gripPoints.append(m_up);
	gripPoints.append(m_down);
	return Acad::eOk;
}
Acad::ErrorStatus SampleCustEnt::subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d& offset) {
	assertWriteEnabled();
	AcGeVector3d original(0, 0, 0);
	switch (indices[0]) {
	case(0):
		m_center += offset;
		m_left += offset;
		m_right += offset;
		m_up += offset;
		m_down += offset;
		break;
	case(1):
		original = m_left - m_center;
		original += offset;
		m_radius = original.length();
		m_left = AcGePoint3d(m_center.x - m_radius, m_center.y, 0);
		m_right = AcGePoint3d(m_center.x + m_radius, m_center.y, 0);
		m_up = AcGePoint3d(m_center.x, m_center.y + m_radius, 0);
		m_down = AcGePoint3d(m_center.x, m_center.y - m_radius, 0);
		break;
	case(2):
		original = (m_right - m_center);
		original += offset;
		m_radius = original.length();
		m_left = AcGePoint3d(m_center.x - m_radius, m_center.y, 0);
		m_right = AcGePoint3d(m_center.x + m_radius, m_center.y, 0);
		m_up = AcGePoint3d(m_center.x, m_center.y + m_radius, 0);
		m_down = AcGePoint3d(m_center.x, m_center.y - m_radius, 0);
		break;

	case(3):
		original = (m_up - m_center);
		original += offset;
		m_radius = original.length();
		m_left = AcGePoint3d(m_center.x - m_radius, m_center.y, 0);
		m_right = AcGePoint3d(m_center.x + m_radius, m_center.y, 0);
		m_up = AcGePoint3d(m_center.x, m_center.y + m_radius, 0);
		m_down = AcGePoint3d(m_center.x, m_center.y - m_radius, 0);
		break;

	case(4):
		original = (m_down - m_center);
		original += offset;
		m_radius = original.length();
		m_left = AcGePoint3d(m_center.x - m_radius, m_center.y, 0);
		m_right = AcGePoint3d(m_center.x + m_radius, m_center.y, 0);
		m_up = AcGePoint3d(m_center.x, m_center.y + m_radius, 0);
		m_down = AcGePoint3d(m_center.x, m_center.y - m_radius, 0);
		break;
	}
	return Acad::eOk;
}

Acad::ErrorStatus SampleCustEnt::subGetOsnapPoints(AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const AcGePoint3d & pickPoint, const AcGePoint3d & lastPoint, const AcGeMatrix3d & viewXform, AcGePoint3dArray & snapPoints, AcDbIntArray & geomIds) const
{
	return Acad::ErrorStatus();
}


Acad::ErrorStatus SampleCustEnt::dwgInFields(AcDbDwgFiler * pFiler)
{

	pFiler->readPoint3d(&m_center);
	pFiler->readPoint3d(&m_left);
	pFiler->readPoint3d(&m_right);
	pFiler->readPoint3d(&m_up);
	pFiler->readPoint3d(&m_down);

	return Acad::ErrorStatus();
}



Acad::ErrorStatus SampleCustEnt::dwgOutFields(AcDbDwgFiler * pFiler) const
{
	pFiler->writePoint3d(m_center);
	pFiler->writePoint3d(m_left);
	pFiler->writePoint3d(m_right);
	pFiler->writePoint3d(m_up);
	pFiler->writePoint3d(m_down);


	return Acad::ErrorStatus();
}
