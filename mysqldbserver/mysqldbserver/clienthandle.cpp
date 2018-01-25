#include "clienthandle.h"

#include "scenelogic.h"

namespace af
{
	void CClientHandle::Init()
	{

	}
	void CClientHandle::Init(char * Ip, int nPort)
	{
		mMyEpoll.InitEpoll(Ip,nPort);
		mMyEpoll.SetMessageManger(this);
	}

	void CClientHandle::Recv()
	{
		mMyEpoll.RunEpoll(0);
	}

	void CClientHandle::OnRecvMessage(int pSocket, CMessage * pMsg)
	{
		if (pSocket <= 0 || pMsg == NULL )
		{
			return;
		}

		CSceneLogic::GetSingletonPtr()->ProcessMessage(pSocket, pMsg);
	}

	void CClientHandle::OnConnectSocket(int nSocket)
	{

	}

	void CClientHandle::OnDisConnectSokcet(int nSocket)
	{
		CMessage tMsg;
		tMsg.mID = emMessageID_DisConnectRequest;
		tMsg.mSrcAddrType = emMessageAddrType_PlayerClient;
		CSceneLogic::GetSingletonPtr()->ProcessMessage(nSocket, &tMsg);
	}

	void CClientHandle::DisConnectSocket(int nSocket)
	{
		mMyEpoll.DelEvent(nSocket, false);
	}
}