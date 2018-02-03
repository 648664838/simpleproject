#include "scenelogic.h"
#include "cmessage.h"
#include "cplayer.h"
#include <time.h>
#include "cscenetype.h"

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
			{//ÒÑµÇÂ¼
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
			return;
		}

		CMessageLoginSceneResponse tResponse;
		tResponse.mResult = SUCCESS;
		mClientHandle.SendClientMessage(nSocket, &tResponse);

		mConnectSocket.erase(tpInfo->mSocketID);

		

		//TODO£ºÐ£ÑéÕËºÅ

		//bool bQuery = mDataBase.Query("select * from ccc");
		//CQueryResult & rQueryResult = mDataBase.GetQueryResult();
		//while (bQuery && rQueryResult.NextRow())
		//{
		//	CField* pField = rQueryResult.Fetch();
		//	if (pField != NULL)
		//	{
		//		for (int i = 0; i < rQueryResult.GetFieldCount(); ++i)
		//		{
		//			cout << pField[i].GetType() << "\t";
		//			cout << pField[i].GetString() << endl;
		//		}
		//	}
		//}
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
