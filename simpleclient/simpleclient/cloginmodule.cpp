#include "cloginmodule.h"
#include "CSocketModule.h"
#include "cdata.h"


void CLoginModule::OnInit()
{

}

void CLoginModule::OnEnd()
{

}

void CLoginModule::OnSendClientMessage(int s, CMessage * pMsg)
{

}
void CLoginModule::OnRecvServerMessage(int s, CMessage * pMsg)
{
	if (pMsg == NULL)
	{
		return;
	}

	switch (pMsg->mID)
	{
	case emMessageID_LoginSceneResponse:
	{
		OnLoginServerResponse(pMsg);
		break;
	}
	case emMessageID_CreateAccountResponse:
	{
		OnCreateRoleResponse(pMsg);
		break;
	}
	case emMessageID_S2C_PlayerClientDataNotify:
	{
		OnPlayerClientDataNotify(pMsg);
		break;
	}
	default:
		break;
	}

}

void CLoginModule::OnPlayerClientDataNotify(CMessage * pMsg)
{
	if (pMsg == NULL)
	{
		return;
	}

	CMessagePlayerClientDataNotify * pNotify = (CMessagePlayerClientDataNotify *)pMsg;

	int mMoney = pNotify->mMoney;
	CStaticData::GetSingletonPtr()->mPlayerData.mMoney = mMoney;
}

void CLoginModule::OnLoginServerResponse(CMessage * pMsg)
{
	if (pMsg == NULL)
	{
		return;
	}

	CMessageLoginSceneResponse * pResponse = (CMessageLoginSceneResponse *)pMsg;
	int nResult = pResponse->mResult;

}

void CLoginModule::OnCreateRoleResponse(CMessage * pMsg)
{
	if (pMsg == NULL)
	{
		return;
	}

	CMessageCreateAccountResponse * pResponse = (CMessageCreateAccountResponse *)pMsg;
	int nResult = pResponse->mResult;
}

void CLoginModule::LoginServer(char * acount, char * password)
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