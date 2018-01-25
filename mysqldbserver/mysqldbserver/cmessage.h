#pragma once
#include "cscenetype.h"

namespace af
{
	enum TmMessageID
	{
		emMessageID_None = 0,
		emMessageID_Connect = 1,
		emMessageID_DisConnectRequest = 2,
		emMessageID_LoginSceneRequest = 3,
		

	};

	enum TmMessageAddrType
	{
		emMessageAddrType_None = 0,
		emMessageAddrType_PlayerClient = 1,  //玩家客户端
		
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

	class CMessageLoginSceneRequest : public CMessage
	{
	public:
		char mAccount[MAX_ACCOUNT_LEGNTH];
		char mPassWord[MAX_PASSWORD_LEGNTH];
	};

};