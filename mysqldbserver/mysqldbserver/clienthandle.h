#pragma once
#include "server_socket.h"
#include "cmessage.h"
#include <set>

using namespace std;


namespace af
{
	class CClientHandle
	{

	public:
		void Init();
		void Init(const char * Ip,int nPort);
		void Recv();
		void SendClientMessage(int nScoket, CMessage * pMsg);
		virtual void OnRecvMessage(int nSocket, CMessage * pMsg);
		virtual void OnConnectSocket(int nSocket);
		virtual void OnDisConnectSokcet(int nSocket);
		virtual void DisConnectSocket(int nSocket);
	private:
		CMyEpoll  mMyEpoll;
	};
}

