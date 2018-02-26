#include "cmainlogic.h"
#include "cscenetype.h"
#include "CSocketModule.h"
#include "cdata.h"
#include "cloginmodule.h"

//����ʼ
int CMainLogic::MyMain()
{
	//ע��ģ��
	RegistModule();
	//ģ���ʼ��
	InitModule();

	//��ʼ������
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

//�������
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