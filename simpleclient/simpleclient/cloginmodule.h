#pragma once
#include "CSingleton.h"
#include "clogicmodule.h"

class CLoginModule : public CMySingleton<CLoginModule>, public CLogicModule
{
	friend CMySingleton < CLoginModule >;
private:
	CLoginModule()
	{
		OnInit();
	}

	~CLoginModule()
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
	void LoginServer(char * acount, char * password);

	void OnLoginServerResponse(CMessage * pMsg);
	void OnCreateRoleResponse(CMessage * pMsg);
	void OnPlayerClientDataNotify(CMessage * pMsg);
};