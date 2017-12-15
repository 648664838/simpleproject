#pragma once

namespace af
{

	class CMessage
	{
	public:
		int mID;
		int mSize;
	};

	class CMessageManger
	{
	public:
		void OnRecvCharMessage(CNetSocket * pSocket, char * pBuff, int nLen);

	};
};