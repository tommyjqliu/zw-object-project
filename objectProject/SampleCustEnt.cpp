#include "pch.h"
#include "tchar.h"
#include <aced.h>
#include <rxregsvc.h>
#include <dbproxy.h>
#include "rxboiler.h"
#include "dbapserv.h"
#include "dbidmap.h"
#include "dbcfilrs.h"
#include "acadstrc.h"

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
SampleCustEnt::SampleCustEnt(const AcDbObjectId &id) : AcDbEntity()
{
	m_srcId = id;
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
	AcDbHardPointerId id;
	pFiler->readHardPointerId(&id);
	m_srcId = id;
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
	AcDbHardPointerId id(m_srcId);
	pFiler->writeHardPointerId(id);
	return (pFiler->filerStatus());

}

//- Dxf Filing protocol
Acad::ErrorStatus SampleCustEnt::dxfOutFields(AcDbDxfFiler *pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dxfOutFields(pFiler);
	if (es != Acad::eOk)
		return (es);
	es = pFiler->writeItem(AcDb::kDxfSubclass, _T("SampleCustEnt"));
	if (es != Acad::eOk)
		return (es);
	//----- Object version number needs to be saved first
	if ((es = pFiler->writeUInt32(AcDb::kDxfInt32, SampleCustEnt::kCurrentVersionNumber)) != Acad::eOk)
		return (es);
	//----- Output params
	//.....

	return (pFiler->filerStatus());
}

Acad::ErrorStatus SampleCustEnt::dxfInFields(AcDbDxfFiler *pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	Acad::ErrorStatus es = AcDbEntity::dxfInFields(pFiler);
	if (es != Acad::eOk || !pFiler->atSubclassData(_T("SampleCustEnt")))
		return (pFiler->filerStatus());
	//----- Object version number needs to be read first
	struct resbuf rb;
	pFiler->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt32) {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode, _T("\nError: expected group code %d (version #)"), AcDb::kDxfInt32);
		return (pFiler->filerStatus());
	}
	Adesk::UInt32 version = (Adesk::UInt32)rb.resval.rlong;
	if (version > SampleCustEnt::kCurrentVersionNumber)
		return (Acad::eMakeMeProxy);
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < SampleCustEnt::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	while (es == Acad::eOk && (es = pFiler->readResBuf(&rb)) == Acad::eOk) {
		switch (rb.restype) {
			//----- Read params by looking at their DXF code (example below)
		case AcDb::kDxfXCoord:
			break;

		default:
			//----- An unrecognized group. Push it back so that the subclass can read it again.
			pFiler->pushBackItem();
			es = Acad::eEndOfFile;
			break;
		}
	}
	//----- At this point the es variable must contain eEndOfFile
	//----- - either from readResBuf() or from pushback. If not,
	//----- it indicates that an error happened and we should
	//----- return immediately.
	if (es != Acad::eEndOfFile)
		return (Acad::eInvalidResBuf);

	return (pFiler->filerStatus());
}

Acad::ErrorStatus
SampleCustEnt::subDeepClone(AcDbObject*    pOwner,
	AcDbObject*&   pClonedObject,
	AcDbIdMapping& idMap,
	Adesk::Boolean isPrimary) const
{
	//调用者理应在传进来的时候就设为NULL，但是以防万一，还是要再置空
	//否则万一没复制成功拿出去就是野指针
	pClonedObject = NULL;

	//如果这个实体已经存在于Id Map而且已经被复制过了（即Id Map里已经有与其成对的Id了），
	//就不用再复制了，直接返回
	//思考：什么情况下会发生这种事？
	bool isPrim = false;
	if (isPrimary)//isPrimary表示当前被复制的对象是被主动复制的还是因为所有关系而被复制的
		isPrim = true;
	AcDbIdPair idPair(objectId(), (AcDbObjectId)NULL,
		false, isPrim);
	if (idMap.compute(idPair) && (idPair.value() != NULL))
		return Acad::eOk;

	//创建一个类型相同的对象
	auto *pClone = (SampleCustEnt*)isA()->create();
	if (pClone != NULL)
		pClonedObject = pClone;    //这个就是复制出来的对象，可以先存到返回值里
	else
		return Acad::eOutOfMemory;

	AcDbDeepCloneFiler filer;
	dwgOut(&filer);//通过dwgOut获取本对象的数据
	filer.seek(0L, AcDb::kSeekFromStart);//重置虚拟文件的指针，方便接下来dwgIn
	pClone->dwgIn(&filer);

	//这里要设置owner，分三种情况
	//1. 如果owner是块表记录，那就直接调用appendAcDbEntity加进去
	//2. 如果owner是词典，则使用setAt加到词典里（这个例子没有体现）
	//3. 如果当前对象是因为引用关系而被复制的，则使用addAcDbObject()
	//把它加到owner的数据库里并直接设置它的owner
	bool bOwnerXlated = false;//表示owner的id是否已经转译
	if (isPrimary)
	{
		AcDbBlockTableRecord *pBTR =
			AcDbBlockTableRecord::cast(pOwner);
		if (pBTR != NULL)
		{
			pBTR->appendAcDbEntity(pClone);
			bOwnerXlated = true;
		}
		else
		{//在这个例子里这个分支其实是不可能进来的，这里只是容错
			pOwner->database()->addAcDbObject(pClone);
		}
	}
	else {
		pOwner->database()->addAcDbObject(pClone);
		pClone->setOwnerId(pOwner->objectId());
		bOwnerXlated = true;
	}


	//每一个在deepclone函数里创建的对象都要在加入Id Map前调用setAcDbObjectIdsInFlux()方法
	pClone->setAcDbObjectIdsInFlux();
	pClone->disableUndoRecording(true);//pClone现在已经在数据库里，通过这个方式可以禁用掉它的undo


	//把源对象和复制对象凑成一对，放到Id Map里
	idPair.setValue(pClonedObject->objectId());
	idPair.setIsCloned(Adesk::kTrue);
	idPair.setIsOwnerXlated(bOwnerXlated);
	idMap.assign(idPair);

	//查找源对象的所有关系，调用它们的deepClone方法
	AcDbObjectId id;
	while (filer.getNextOwnedObject(id)) {

		AcDbObject *pSubObject;
		AcDbObject *pClonedSubObject;

		//所有关系的对象id不一定都是有效的，需要做检查
		if (id == NULL)
			continue;

		//注意因为这些对象都是由于所有关系而被复制的，所以deepClone的最后一个参数(isPrimary)需要设为false
		acdbOpenAcDbObject(pSubObject, id, AcDb::kForRead);
		pClonedSubObject = NULL;
		pSubObject->deepClone(pClonedObject,
			pClonedSubObject,
			idMap, Adesk::kFalse);

		//在某特殊情况下(比如某些情况下的预览)，复制实际上只是数据“移动”了
		//pSubObject和pClonedSubObject会指向同一个对象，这个时候就不能close pSubObject了
		if (pSubObject != pClonedSubObject)
			pSubObject->close();

		//最后如果复制失败pClonedSubObject就会是空指针，需要检查
		if (pClonedSubObject != NULL)
			pClonedSubObject->close();
	}

	//注意此时pClonedObject还没有被关闭的，交由调用者来处理
	return Acad::eOk;
}

Acad::ErrorStatus SampleCustEnt::subWblockClone(
		AcRxObject*    pOwner,
		AcDbObject*&   pClonedObject,
		AcDbIdMapping& idMap,
		Adesk::Boolean isPrimary) const
	{
		//记得先把返回值置空
		pClonedObject = NULL;

		//如果源数据库和目标数据库是同一个，可以省略大部分步骤，直接调用基类的wblockClone就可以了
		AcDbDatabase *pDest, *pOrig;
		idMap.destDb(pDest);
		idMap.origDb(pOrig);
		if (pDest == pOrig)
			return AcDbEntity::wblockClone(pOwner, pClonedObject,
				idMap, isPrimary);

		//如果这是在进行xbind操作，由于xbind只处理模型空间的对象，如果源对象在图纸空间，就直接返回
		AcDbObjectId pspace;
		AcDbBlockTable *pTable;
		database()->getSymbolTable(pTable, AcDb::kForRead);
		pTable->getAt(ACDB_PAPER_SPACE, pspace);
		pTable->close();
		if (idMap.deepCloneContext() == AcDb::kDcXrefBind
			&& ownerId() == pspace)
			return Acad::eOk;

		//如果源对象已经被复制过了，直接返回
		bool isPrim = false;
		if (isPrimary)
			isPrim = true;
		AcDbIdPair idPair(objectId(), (AcDbObjectId)NULL,
			false, isPrim);
		if (idMap.compute(idPair) && (idPair.value() != NULL))
			return Acad::eOk;

		//源对象的owner有可能是数据库，也有可能是一个AcDbObject，
		//如果owner是数据库，表示这个对象可能是由于硬指针关系而被复制的，它的真实的owner会在转译阶段被正确设置
		//此时pOwn的值会是NULL，作为一个标志
		AcDbObject   *pOwn = AcDbObject::cast(pOwner);
		AcDbDatabase *pDb = AcDbDatabase::cast(pOwner);
		if (pDb == NULL)
			pDb = pOwn->database();

		//创建一个同类型的对象
		auto *pClone = (SampleCustEnt*)isA()->create();
		if (pClone != NULL)
			pClonedObject = pClone;
		else
			return Acad::eOutOfMemory;

		//如果owner是个块表，或者我们能知道它到底是什么，就可以直接设置复制对象的owner
		//否则就把源对象的owner设置给复制对象，在转译阶段进行转换，这个（源对象与目标对象owner一样）也会作为转译阶段要用到的一个标志
		//要注意这代表源对象的owner也是需要被复制到目标的数据库里的，否则转译阶段会出错
		AcDbBlockTableRecord *pBTR = NULL;
		if (pOwn != NULL)
			pBTR = AcDbBlockTableRecord::cast(pOwn);
		if (pBTR != NULL && isPrimary) {
			pBTR->appendAcDbEntity(pClone);
		}
		else {
			pDb->addAcDbObject(pClonedObject);
		}

		//通过dwgOut和dwgIn复制数据到复制对象里
		AcDbWblockCloneFiler filer;
		dwgOut(&filer);
		filer.seek(0L, AcDb::kSeekFromStart);
		pClone->dwgIn(&filer);

		//添加源对象与复制对象到Id Map，注意AcDbIdPair构造函数的最后一个参数isOwnerXlated代表复制对象的owner是否需要转译
		//在这个例子里是通过上面的pOwn是否为空来判断的
		idMap.assign(AcDbIdPair(objectId(), pClonedObject->objectId(),
			Adesk::kTrue,
			isPrim, (Adesk::Boolean)(pOwn != NULL)));
		pClonedObject->setOwnerId((pOwn != NULL) ?
			pOwn->objectId() : ownerId());

		//与deepclone一样，要对每个复制对象调用setAcDbObjectIdsInFlux()
		pClone->setAcDbObjectIdsInFlux();

		//递归复制所有的硬所有和硬指针
		AcDbObjectId id;
		while (filer.getNextHardObject(id)) {
			AcDbObject *pSubObject;
			AcDbObject *pClonedSubObject;

			//某些情况下会出现引用的对象为空，需要跳过
			if (id == NULL)
				continue;

			//如果引用对象来自于另一个数据库，不需要复制
			acdbOpenAcDbObject(pSubObject, id, AcDb::kForRead);
			if (pSubObject->database() != database()) {
				pSubObject->close();
				continue;
			}

			//如上面所述，如果这个对象不是引用对象的owner，就把数据库作为owner传进去
			//另外跟deepclone一样，这里isPrimary参数要设为false
			pClonedSubObject = NULL;
			if (pSubObject->ownerId() == objectId()) {
				pSubObject->wblockClone(pClone,
					pClonedSubObject,
					idMap, Adesk::kFalse);
			}
			else {
				pSubObject->wblockClone(
					pClone->database(),
					pClonedSubObject,
					idMap, Adesk::kFalse);
			}
			pSubObject->close();

			//如果复制失败，pClonedSubObject会是空指针，需要检查
			if (pClonedSubObject != NULL)
				pClonedSubObject->close();
		}

		//与deepclone一样，复制对象由调用者来close
		return Acad::eOk;
	}
