#include "scenelogic.h"
#include "cmessage.h"
#include "cplayer.h"
#include <time.h>

namespace af
{
	void CSceneLogic::Init()
	{
		mClientHandle.Init("127.0.0.1",24100);
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
			//else 
			//{ // 已连接，但长时间不登陆，也踢掉
			//	tpInfo->mLastTime = time(NULL);
			//}
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
			return;
		}

		//TODO：校验账号
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
	CSceneLogic::CSocketInfo * CSceneLogic::GetSceneVerifyData(int nSocket)
	{
		ConnectSocketIter it = mVerifyData.find(nSocket);
		if (it == mVerifyData.end())
		{
			return NULL;
		}

		return &it->second;
	}

}
