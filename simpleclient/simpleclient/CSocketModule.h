#pragma once
#include "CSingleton.h"
#include <WinSock2.h>
#include "cscenetype.h"
#include "cmessage.h"
#include "clogicmodule.h"

#define PATH_SERVER_ADDR  "serverinfo.xml"

class CConnectInfo
{
public:
	char mName[MAX_NAME_LEN];
	
	char mIp[MAX_IP_CHAR_LEN];
	int mPort;
	int mChannel;
	int mStatus;
};


enum emConnectState
{
	emConnect_Init = 0,
	emConnect_Info = 1,
	emConnect_Con = 2,
};

class CSocketModule : public CMySingleton<CSocketModule>, public CLogicModule
{
	friend CMySingleton < CSocketModule > ;
private:
	CSocketModule()
	{
		OnInit();
	}

	~CSocketModule()
	{
		OnEnd();
	}

public:
	void OnInit();
	void OnEnd();
	//发送消息
	virtual void OnSendClientMessage(int s, CMessage * pMsg);
	//收到消息
	virtual void OnRecvServerMessage(int s, CMessage * pMsg);

public:
	int InitClientSocket();
	int InitConnectInfo();
	int ClientConnectServer();
	bool IsConnectServer();
	int GetSocketFb(){ return mSocketFb; }

	void SendServerMessage(CMessage * pMsg);
	int HandleMessage();
	void DispatchServerMessage();
public:
	void ShutDown(SOCKET s);
private:
	CConnectInfo mConnectInfo;
	int mState;
	int mCurNum; //当前用户数量
	sockaddr_in mServerAddr;
	int mSocketFb;
	FD_SET  mUserSocket;
};