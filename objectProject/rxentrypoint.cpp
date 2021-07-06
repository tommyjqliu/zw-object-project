#include "pch.h"

#include "adsmigr.h"
#include "adsdef.h"
#include "adscodes.h"
#include "acestext.h"
#include "acedads.h"

#include "SampleCustEnt.h"
#include "SampleReactor.h"
#include "SampleDrawOverrule.h"
//#include "AsdkEntCenter.h"

SampleDrawOverrule* g_pSampleOverrule;
//AsdkDefaultCenter* pDefaultCen;
//AsdkCircleCenter* pCircleCen;
//AsdkLinesCenter* pLinesCen;
void initapp()
{
	g_pSampleOverrule = new SampleDrawOverrule();
	AcRxOverrule::addOverrule(AcDbLine::desc(), g_pSampleOverrule);

	SampleCustEnt::rxInit();//注册自定义实体
	SampleReactor::rxInit();
	/*AsdkEntCenter::rxInit();*/
	
	acrxBuildClassHierarchy();//重生成运行时类树

	/*pDefaultCen = new AsdkDefaultCenter();
	pCircleCen = new AsdkCircleCenter();
	pLinesCen = new AsdkLinesCenter();
	AcDbEntity::desc()->addX(AsdkEntCenter::desc(), pDefaultCen);
	AcDbCircle::desc()->addX(AsdkEntCenter::desc(), pCircleCen);
	AcDbPolyline::desc()->addX(AsdkEntCenter::desc(), pLinesCen);*/
}

void unloadapp()
{
	if (g_pSampleOverrule)
	{
		AcRxOverrule::removeOverrule(AcDbLine::desc(), g_pSampleOverrule);
		delete g_pSampleOverrule;
		g_pSampleOverrule = nullptr;
	}
	//AcDbEntity::desc()->delX(AsdkEntCenter::desc());
	//delete pDefaultCen;
	//AcDbCircle::desc()->delX(AsdkEntCenter::desc());
	//delete pCircleCen;
	//AcDbPolyline::desc()->delX(AsdkEntCenter::desc());
	//delete pLinesCen;

	deleteAcRxClass(SampleCustEnt::desc());//注销自定义实体
	deleteAcRxClass(SampleReactor::desc());//注销自定义实体
	//deleteAcRxClass(AsdkEntCenter::desc());


}


extern "C" AcRx::AppRetCode zcrxEntryPoint(AcRx::AppMsgCode msg, void* appId)
{
	switch (msg)
	{
		case AcRx::kInitAppMsg:
		{
			acrxDynamicLinker->unlockApplication(appId);
			acrxDynamicLinker->registerAppMDIAware(appId);
			initapp();
			
		}
		break;
		case AcRx::kUnloadAppMsg:
		{
			unloadapp();
		}
		break;
		
		case AcRx::kLoadDwgMsg:
		{	
		}
		break;
		case AcRx::kUnloadDwgMsg:
		{
			// Add your code here.
		}
		break;
		case AcRx::kInvkSubrMsg:
		{
		}
		break;
		
		default:
			break;
	}
	return AcRx::kRetOK;
}



#ifdef _WIN64
#pragma comment(linker, "/export:zcrxEntryPoint,PRIVATE")
#pragma comment(linker, "/export:zcrxGetApiVersion,PRIVATE")
#else // WIN32
#pragma comment(linker, "/export:_zcrxEntryPoint,PRIVATE")
#pragma comment(linker, "/export:_zcrxGetApiVersion,PRIVATE")
#endif

