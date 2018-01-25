#pragma once


class CPlayerData
{
public:
	CPlayerData()
	{
		mSocket = 0;
		mCharID = 0;
	}

public:
	int mSocket;
	int mCharID;
};