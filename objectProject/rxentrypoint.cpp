#include "pch.h"

#include "adsmigr.h"
#include "adsdef.h"
#include "adscodes.h"
#include "acestext.h"
#include "acedads.h"
#include "helloworld.h"
#include "SampleCustEnt.h"


void initapp()
{
	// acedRegCmds->addCommand(cmd_group_name, _T("helloworld"), _T("helloworld"), ACRX_CMD_MODAL, helloworld);
	SampleCustEnt::rxInit();//注册自定义实体
	acrxBuildClassHierarchy();//重生成运行时类树
}

void unloadapp()
{
	// acedRegCmds->removeGroup(cmd_group_name);
	deleteAcRxClass(SampleCustEnt::desc());//注销自定义实体
}


#define ELEMENTS(array) (sizeof(array)/sizeof((array)[0]))

struct func_entry { TCHAR *func_name; int(*func) (struct resbuf *); };

struct func_entry func_table[] = {
	{_T("showhello"), showhello},
	{_T("c:showhello"), showhellocmd},
};

int funcload()
{
	for (int i = 0; i < ELEMENTS(func_table); i++)
	{
		if (!acedDefun(func_table[i].func_name, i))
			return RTERROR;
	}

	return RTNORM;
}

int dofunc()
{
	struct resbuf* rb = NULL;
	int val = 0;

	if ((val = acedGetFunCode()) < 0 || val >= ELEMENTS(func_table))
	{
		acdbFail(_T("Received nonexistent function code."));
		return RTERROR;
	}
	rb = acedGetArgs();
	val = (*func_table[val].func)(rb);
	acutRelRb(rb);
	return val;
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
			funcload();
		}
		break;
		case AcRx::kUnloadDwgMsg:
		{
			// Add your code here.
		}
		break;
		case AcRx::kInvkSubrMsg:
		{
			dofunc();
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

