#pragma once
//此自定义实体继承于AcDbEntity，这是最常用的自定义实体父类
class __declspec(dllexport) SampleCustEnt : public AcDbEntity {
public:
	AcGePoint3d m_center = AcGePoint3d::kOrigin;
	double m_radius = 0;
	AcDbObjectId m_srcId;
	ACRX_DECLARE_MEMBERS(SampleCustEnt);//定义了一些运行时类型识别和转换函数，如cast, desc, isA等
protected:
	static Adesk::UInt32 kCurrentVersionNumber;//记录自定义实体的版本，方便做版本兼容
public:
	SampleCustEnt();
	SampleCustEnt(const AcDbObjectId&);
	SampleCustEnt(AcGePoint3d m_center, double m_radius);
	virtual ~SampleCustEnt();
	void setCenter(AcGePoint3d center);
	void setRadius(double radius);
protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);//视口无关的显示
	virtual void subViewportDraw(AcGiViewportDraw* mode);//视口相关的显示
	virtual Adesk::UInt32 subSetAttributes(AcGiDrawableTraits *traits);
	Acad::ErrorStatus subTransformBy(const AcGeMatrix3d & xform);
	Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray & gripPoints, AcDbIntArray & osnapModes, AcDbIntArray & geomIds) const;
	Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray & indices, const AcGeVector3d & offset);
	Acad::ErrorStatus subGetOsnapPoints(AcDb::OsnapMode osnapMode, Adesk::GsMarker gsSelectionMark, const AcGePoint3d & pickPoint, const AcGePoint3d & lastPoint, const AcGeMatrix3d & viewXform, AcGePoint3dArray & snapPoints, AcDbIntArray & geomIds) const;
	Acad::ErrorStatus dwgOutFields(AcDbDwgFiler * pFiler) const;
	Acad::ErrorStatus dxfOutFields(AcDbDxfFiler * pFiler) const;
	Acad::ErrorStatus dxfInFields(AcDbDxfFiler * pFiler);
	Acad::ErrorStatus subDeepClone(AcDbObject * pOwner, AcDbObject *& pClonedObject, AcDbIdMapping & idMap, Adesk::Boolean isPrimary) const;
	Acad::ErrorStatus subWblockClone(AcRxObject * pOwner, AcDbObject *& pClonedObject, AcDbIdMapping & idMap, Adesk::Boolean isPrimary) const;
	Acad::ErrorStatus dwgInFields(AcDbDwgFiler * pFiler);
	//设置显示相关的属性
};

#ifdef ARXPROJECT1_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(SampleCustEnt)
#endif