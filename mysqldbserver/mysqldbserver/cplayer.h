#pragma once


class CPlayerData
{
public:
	CPlayerData()
	{
		mSocket = 0;
		mCharID = 0;
		mMoney  = 0;
		mLastPingTime = 0;
	}


public:
	int mSocket;
	int mCharID;
	int mMoney;
	int mLastPingTime;
};