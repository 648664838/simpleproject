#pragma once


class CPlayerData
{
public:
	CPlayerData()
	{
		mCharID = 0;
		mMoney  = 0;
	}


public:
	int mCharID;
	int mMoney;
	int mLastPingTime;
};