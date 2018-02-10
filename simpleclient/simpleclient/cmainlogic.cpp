#include "cmainlogic.h"
#include "cscenetype.h"
#include "CSocketModule.h"


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

void CMainLogic::LoginServer(char * acount, char * password)
{
	if (acount == NULL || password == NULL)
	{
		return;
	}
	if (!CSocketModule::GetSingletonPtr()->IsConnectServer())
	{
		int nRet = CSocketModule::GetSingletonPtr()->ClientConnectServer();
		if (nRet != SUCCESS)
		{
			return;
		}
	}

	CMessageLoginSceneRequest tRequest;
	strncpy(tRequest.mAccount, acount, sizeof(tRequest.mAccount) - 1);
	strncpy(tRequest.mPassWord, password, sizeof(tRequest.mPassWord) - 1);
	CSocketModule::GetSingletonPtr()->SendServerMessage(&tRequest);
}

void CMainLogic::CreateAccount(char * acount, char * password)
{
	if (acount == NULL || password == NULL)
	{
		return;
	}

	CMessageCreateAccountRequest tRequest;
	strncpy(tRequest.mAccount, acount, sizeof(tRequest.mAccount) - 1);
	strncpy(tRequest.mPassWord, password, sizeof(tRequest.mPassWord) - 1);
	CSocketModule::GetSingletonPtr()->SendServerMessage(&tRequest);
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

	if (pMsg->mID == emMessageID_LoginSceneResponse)
	{
		OnLoginServerResponse(pMsg);
		return;
	}

	if (pMsg->mID == emMessageID_CreateAccountResponse)
	{
		OnCreateRoleResponse(pMsg);
		return;
	}

	if (pMsg->mID == emMessageID_S2C_PlayerClientDataNotify)
	{
		OnPlayerClientDataNotify(pMsg);
		return;
	}

	for (int i = 0; i < mModuleNum; ++i)
	{
		mLogicModule[i]->OnRecvServerMessage(CSocketModule::GetSingletonPtr()->GetSocketFb(), pMsg);
	}
}

void CMainLogic::OnPlayerClientDataNotify(CMessage * pMsg)
{
	if (pMsg == NULL)
	{
		return;
	}

	CMessagePlayerClientDataNotify * pNotify = (CMessagePlayerClientDataNotify *)pMsg;

	int mMoney = pNotify->mMoney;
}

void CMainLogic::OnLoginServerResponse(CMessage * pMsg)
{
	if (pMsg == NULL)
	{
		return;
	}

	CMessageLoginSceneResponse * pResponse = (CMessageLoginSceneResponse *)pMsg;
	int nResult = pResponse->mResult;
}

void CMainLogic::OnCreateRoleResponse(CMessage * pMsg)
{
	if (pMsg == NULL)
	{
		return;
	}

	CMessageCreateAccountResponse * pResponse = (CMessageCreateAccountResponse *)pMsg;
	int nResult = pResponse->mResult;
}

int CMainLogic::OnTimer(unsigned int nIDEvent)
{
	SendTimerSignal();
	CSocketModule::GetSingletonPtr()->HandleMessage();
	CSocketModule::GetSingletonPtr()->DispatchServerMessage();
	return 0;
}