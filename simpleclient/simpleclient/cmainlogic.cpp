#include "cmainlogic.h"
#include "cscenetype.h"
#include "CSocketModule.h"
#include "cdata.h"
#include "cloginmodule.h"

//程序开始
int CMainLogic::MyMain()
{
	//注册模块
	RegistModule();
	//模块初始化
	InitModule();

	//初始化网络
	int tRet = CSocketModule::GetSingletonPtr()->InitClientSocket();
	if (tRet != SUCCESS)
	{
		return tRet;
	}

	LaunchComplete();

	return SUCCESS;
}

void CMainLogic::RegistModule()
{
	mModuleNum = 0;

	//mLogicModule[mModuleNum++] = CLoginServerModule::GetSingletonPtr();

	mLogicModule[mModuleNum++] = CSocketModule::GetSingletonPtr();

	mLogicModule[mModuleNum++] = CLoginModule::GetSingletonPtr();
}
void CMainLogic::InitModule()
{
	for (int i = 0; i < mModuleNum; ++i)
	{
		mLogicModule[i]->OnInit();
	}
}
void CMainLogic::LaunchComplete()
{
	for (int i = 0; i < mModuleNum; ++i)
	{
		mLogicModule[i]->OnComplete();
	}
}

//程序结束
int CMainLogic::MyFinal()
{

	return 0;
}

void CMainLogic::SendTimerSignal()
{
	for (int i = 0; i < mModuleNum; ++i)
	{
		mLogicModule[i]->OnTimer();
	}
}

void CMainLogic::ProcessMessage(CMessage * pMsg)
{
	if (pMsg == NULL)
	{
		return;
	}

	if (pMsg->mID == emMessageID_KickPlayerNotify)
	{
		return;
	}

	for (int i = 0; i < mModuleNum; ++i)
	{
		mLogicModule[i]->OnRecvServerMessage(CSocketModule::GetSingletonPtr()->GetSocketFb(), pMsg);
	}
}

int CMainLogic::OnTimer(unsigned int nIDEvent)
{
	SendTimerSignal();
	CSocketModule::GetSingletonPtr()->HandleMessage();
	CSocketModule::GetSingletonPtr()->DispatchServerMessage();
	return 0;
}