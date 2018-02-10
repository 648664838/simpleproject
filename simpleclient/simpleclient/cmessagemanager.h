#pragma once
#include "CSingleton.h"
#include "cscenetype.h"
#include "cmessage.h"

using namespace af;

class CMessageManager : public CMySingleton < CMessageManager >
{
	friend CMySingleton < CMessageManager > ;
private:
	CMessageManager()
	{
		Init();
	}

	~CMessageManager()
	{
		Final();
	}

public:
	void Init();
	void Final();
public:
	int DispatchServerMessage();
	void SendServerMessage(CMessage * pMsg);
	void OnReadServerBuff(char * pBuff, int nLen);
	void GetSendOneMessageBuff(char * pBuff, int & nLen);
	void GetRecvOneMessageBuff(char * pBuff, int & nLen);
	void GetOneMessageBuff(char * pSrcBuff, int & rSrcLen, char * pDstBuff, int & rDstLen);
protected:
	void OnSendServerBuff(char * pBuff, int nLen);
private:
	int mSendLength;
	char mSendBuff[MAX_SOCKET_BUFF_LENGTH];

	int mRecvLength;
	char mRecvBuff[MAX_SOCKET_BUFF_LENGTH];
};

