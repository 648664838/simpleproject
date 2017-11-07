#pragma once

namespace af
{
	class CMySocket
	{
	public:
		enum
		{
			ENUM_IP_STR_LENGTH = 15,
		};
	public:
		CMySocket();
		explicit CMySocket(char * Ip, bool bBlock == false);
		virtual ~CMySocket();
		virtual int InitSocket(int port, int protoType == SOCK_STREAM);
		virtual void Close();

	public:
		int GetSocket(){ return mSocket; }
		void SetSocket(int socket){ mSocket = socket; }
	protected:
		void SetNonBlock();
	private:
		char   mIp[ENUM_IP_STR_LENGTH];
		int    mPort;
		bool   mBlock;
		int    mSocket;
	};
	class CListenSocket : public CMySocket
	{
	public:
		using CMySocket::CMySocket;

		int InitSocket(int port, int protoType == SOCK_STREAM);
		int Accpet();
	};

	class CNetSocket : public CMySocket
	{
	public:
		using CMySocket;
		int Connect(char * Ip, int port);
		int ReadData(char * pBuff, int nLen);
		int WriteData(char * pBuff, int nLen);
	};

	class CMyMessage
	{
	public:
		enum 
		{
			MAX_SOCKET_BUFF_SIZE = 64000,
		};

		CMyMessage();

	public:
	   virtual	int ParseMessageToBuff(char * pBuff, int nLen);
	   virtual	int ParseBuffToMessage(char * pBuff, int nLen);
	public:
		char mBuff[MAX_SOCKET_BUFF_SIZE];
	};

	//class C

}