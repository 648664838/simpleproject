#include "scenelogic.h"
#include "cmessage.h"
#include "cplayer.h"
#include <time.h>
#include "cscenetype.h"
#include "error.h"

namespace af
{
	void CSceneLogic::Init()
	{
		mClientHandle.Init("192.168.10.12",24100);
		mDataBase.Initialize("./config/mysqlinfo.xml");
	}
	void CSceneLogic::Final()
	{

	}

	void CSceneLogic::Run()
	{
		mClientHandle.Recv();
	}

	void CSceneLogic::ProcessMessage(int nSocketID, CMessage * pMessage)
	{
		switch (pMessage->mSrcAddrType)
		{
		case emMessageAddrType_PlayerClient:
		{
			ProcessPlayerClientMessage(nSocketID, pMessage);
			break;
		}
		default:
			break;
		}

	}

	void CSceneLogic::ProcessPlayerClientMessage(int nSocketID, CMessage * pMessage)
	{
		switch (pMessage->mID)
		{
		case emMessageID_Connect:
		{
			CPlayerData * pPlayerData = GetScenePlayerData(nSocketID);
			if (pPlayerData != NULL)
			{//已登录
				return;
			}
			CSocketInfo * tpInfo = GetSceneConnectInfo(nSocketID);
			if (tpInfo == NULL)
			{
				CSocketInfo tInfo;
				tInfo.mSocketID = nSocketID;
				tInfo.mLastTime = time(NULL);
				mConnectSocket.insert(make_pair(nSocketID,tInfo));
			}

			return;
		}
		case emMessageID_LoginSceneRequest:
		{
			OnMessageLoginSceneRequest(nSocketID,pMessage);
			return;
		}
		default:
			break;
		}	
	}

	void CSceneLogic::OnMessageLoginSceneRequest(int nSocket, CMessage * pMessage)
	{
		if (nSocket < 0 || pMessage == NULL)
		{
			return;
		}

		CSocketInfo * tpInfo = GetSceneConnectInfo(nSocket);
		if (tpInfo == NULL)
		{
			SendLoginSceneResponse(nSocket, ERROR_LOGIN_SCENE_NOT_CONNECT);
			return;
		}
		CMessageLoginSceneRequest * pRequest = (CMessageLoginSceneRequest *)pMessage;
		//校验账号
		bool bExecute = mDataBase.ExecuteSql("select role_id from tb_account from where user_name ='%s' and password ='%s'", pRequest->mAccount, pRequest->mPassWord);
		if (!bExecute || mDataBase.GetQueryResult().GetRowCount() <= 0)
		{
			SendLoginSceneResponse(nSocket, ERROR_LOGIN_SCENE_ACOUNT_INVAILD);
			return;
		}

		int nRoleID = 0;
		int nResult = mDataBase.GetQueryResult().NextRow();
		if (nResult)
		{
			CField * pRoleIdField = mDataBase.GetQueryResult().GetCurRowFieldByIndex(0);
			if (pRoleIdField != NULL)
			{
				nRoleID = pRoleIdField->GetInt32();
			}
		}

		if (nRoleID <= 0)
		{
			SendLoginSceneResponse(nSocket, ERROR_LOGIN_SCENE_ROLEID_ZERO);
			return;
		}
		CPlayerData tPlayerData;
		nResult = LoadPlayerData(nSocket, nRoleID, tPlayerData);
		if (nResult != SUCCESS)
		{
			SendLoginSceneResponse(nSocket, nResult);
			return;
		}
	
		mConnectSocket.erase(tpInfo->mSocketID);
		mPlayerData.insert(make_pair(nSocket, tPlayerData));

		SendLoginSceneResponse(nSocket, nResult);
	}

	int CSceneLogic::LoadPlayerData(int nSocket, int nRoleId, CPlayerData & rPlayerData)
	{
		if (nRoleId <= 0)
		{
			return ERROR_LOGIN_SCENE_ROLEID_ZERO;
		}
		//加载相关数据
		bool bExecute = mDataBase.ExecuteSql("select role_money from tb_role from where role_id ='%d'", nRoleId);
		if (!bExecute || mDataBase.GetQueryResult().GetRowCount() <= 0)
		{
			return ERROR_LOGIN_SCENE_ROLEDATA_NULL;
		}

		bool bResult = mDataBase.GetQueryResult().NextRow();
		if (!bResult)
		{
			return ERROR_LOGIN_SCENE_ROLEDATA_RESULT;
		}

		if(mDataBase.GetQueryResult().GetFieldCount() != 1  //检查列数
			||mDataBase.GetQueryResult().GetRowCount() <= 0
		  )
		{
			return ERROR_LOGIN_SCENE_ROLEDATA_INVALLD;
		}

		CField * pRoleIdField = mDataBase.GetQueryResult().Fetch();
		if (pRoleIdField == NULL)
		{
			return ERROR_LOGIN_SCENE_ROLEDATA_FECTH;
		}

		rPlayerData.mSocket = nSocket;
		rPlayerData.mCharID = nRoleId;
		rPlayerData.mMoney = pRoleIdField[0].GetInt32();
		return SUCCESS;
	}

	void CSceneLogic::SendLoginSceneResponse(int nSocket, int nResult)
	{
		CMessageLoginSceneResponse tResponse;
		tResponse.mResult = nResult;
		mClientHandle.SendClientMessage(nSocket, &tResponse);
	}

	CPlayerData * CSceneLogic::GetScenePlayerData(int nSocket)
	{
		PlayerDataIter it = mPlayerData.find(nSocket);
		if (it == mPlayerData.end())
		{
			return NULL;
		}

		return &it->second;
	}

	CSceneLogic::CSocketInfo * CSceneLogic::GetSceneConnectInfo(int nSocket)
	{
		ConnectSocketIter it = mConnectSocket.find(nSocket);
		if (it == mConnectSocket.end())
		{
			return NULL;
		}

		return &it->second;
	}

}
