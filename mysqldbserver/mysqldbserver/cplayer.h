#pragma once


class CPlayerData
{
public:
	CPlayerData()
	{
		mSocket = 0;
		mCharID = 0;
	}

	bool operator < (const CPlayerData& other)  const
	{
		return mSocket < other.mSocket;
	}

public:
	int mSocket;
	int mCharID;
};