#pragma once
#include "cscenetype.h"

#include <string.h>

namespace af
{
	enum TmMessageID
	{
		emMessageID_None			   = 0,
		emMessageID_Connect			   = 1,
		emMessageID_DisConnect		   = 2,
		emMessageID_LoginSceneRequest  = 3, //登陆请求
		emMessageID_LoginSceneResponse = 4, //登陆回应
		emMessageID_KickPlayerNotify   = 5,	//踢掉玩家
		emMessageID_CreateAccountRequest  = 6, //创建账号请求
		emMessageID_CreateAccountResponse = 7, //创建账号回应
	};

	enum TmMessageAddrType
	{
		emMessageAddrType_None = 0,
		emMessageAddrType_PlayerClient = 1,  //玩家客户端
		emMessageAddrType_GameServer = 2,    //游戏服务器 
	};



	class CMessage
	{
	public:
		CMessage()
		{
			mID = 0;
			mSize = 0;
			mSrcAddrType = 0;
		}
	public:
		int mID;
		int mSize;
		int mSrcAddrType;  // 源地址
	};

	class CConnectMessage : public CMessage
	{
	public:
		CConnectMessage()
		{
			mID = emMessageID_Connect;
			mSize = sizeof(CConnectMessage);
		}
	};

	class CDisConnectMessage : public CMessage
	{
	public:
		CDisConnectMessage()
		{
			mID = emMessageID_DisConnect;
			mSize = sizeof(CDisConnectMessage);
		}
	};

	class CKickPlayerNotifyMessage : public CMessage
	{
	public:
		CKickPlayerNotifyMessage()
		{
			mID = emMessageID_KickPlayerNotify;
			mSize = sizeof(CKickPlayerNotifyMessage);
		}
	public:
		int mReason;
	};

	class CMessageLoginSceneRequest : public CMessage
	{
	public:
		CMessageLoginSceneRequest()
		{
			mID = emMessageID_LoginSceneRequest;
			mSize = sizeof(CMessageLoginSceneRequest);
			memset(mAccount, 0, sizeof(mAccount));
			memset(mPassWord, 0, sizeof(mPassWord));
		}
	public:
		char mAccount[MAX_ACCOUNT_LEGNTH];
		char mPassWord[MAX_PASSWORD_LEGNTH];
	};

	class CMessageLoginSceneResponse : public CMessage
	{
	public:
		CMessageLoginSceneResponse()
		{
			mID = emMessageID_LoginSceneResponse;
			mSize = sizeof(CMessageLoginSceneResponse);
			mResult = 0;
		}
	public:
		int mResult;
	};

	class CMessageCreateAccountRequest : public CMessage
	{
	public:
		CMessageCreateAccountRequest()
		{
			mID = emMessageID_CreateAccountRequest;
			mSize = sizeof(CMessageCreateAccountRequest);
			memset(mAccount, 0, sizeof(mAccount));
			memset(mPassWord, 0, sizeof(mPassWord));
		}
	public:
		char mAccount[MAX_ACCOUNT_LEGNTH];
		char mPassWord[MAX_PASSWORD_LEGNTH];
	};

	class CMessageCreateAccountResponse : public CMessage
	{
	public:
		CMessageCreateAccountResponse()
		{
			mID = emMessageID_CreateAccountResponse;
			mSize = sizeof(CMessageCreateAccountResponse);
			mResult = 0;
		}
	public:
		int mResult;
	};
};