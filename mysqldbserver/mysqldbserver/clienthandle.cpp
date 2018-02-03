#include "clienthandle.h"

#include "scenelogic.h"

namespace af
{
	void CClientHandle::Init()
	{

	}
	void CClientHandle::Init(const char * Ip, int nPort)
	{
		mMyEpoll.InitEpoll(Ip,nPort);
		mMyEpoll.SetMessageManger(this);
	}

	void CClientHandle::Recv()
	{
		mMyEpoll.RunEpoll(0);
	}

	void CClientHandle::SendClientMessage(int nScoket, CMessage * pMsg)
	{
		if (nScoket <= 0 || pMsg == NULL)
		{
			return;
		}
		pMsg->mSrcAddrType = emMessageAddrType_GameServer;

		mMyEpoll.Send(nScoket, (char *)pMsg, pMsg->mSize);
	}

	void CClientHandle::OnRecvMessage(int nSocket, CMessage * pMsg)
	{
		if (nSocket <= 0 || pMsg == NULL )
		{
			return;
		}

		CSceneLogic::GetSingletonPtr()->ProcessMessage(nSocket, pMsg);
	}
	//检测有连接
	void CClientHandle::OnConnectSocket(int nSocket)
	{
		CConnectMessage tMsg;
		tMsg.mSrcAddrType = emMessageAddrType_PlayerClient;
		CSceneLogic::GetSingletonPtr()->ProcessMessage(nSocket, &tMsg);
	}
	//检测断开
	void CClientHandle::OnDisConnectSokcet(int nSocket)
	{
		CDisConnectMessage tMsg;
		tMsg.mSrcAddrType = emMessageAddrType_PlayerClient;
		CSceneLogic::GetSingletonPtr()->ProcessMessage(nSocket, &tMsg);
	}
	//主动断开
	void CClientHandle::DisConnectSocket(int nSocket)
	{
		mMyEpoll.DelEvent(nSocket, false);
	}
}