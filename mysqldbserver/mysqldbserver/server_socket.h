#pragma once

#include <map>

using namespace std;

namespace af
{
	enum
	{
		MAX_SOCKET_BUFF_SIZE = 64000,
	};
	int SetNonBlock(int fd);
	class CMySocket
	{
	public:
		enum
		{
			ENUM_IP_STR_LENGTH = 15,
		};
	public:
		CMySocket();
		explicit CMySocket(char * Ip, bool bBlock = false);
		virtual ~CMySocket();
		virtual int InitSocket(int port, int protoType = SOCK_STREAM);
		virtual void Close();

	public:
		int GetSocket(){ return mSocket; }
		void SetSocket(int socket){ mSocket = socket; }
	protected:
		char   mIp[ENUM_IP_STR_LENGTH];
		int    mPort;
		bool   mBlock;
		int    mSocket;
	};
	class CListenSocket : public CMySocket
	{
	public:
		CListenSocket() :CMySocket(){}
		explicit CListenSocket(char * Ip, bool bBlock = false) :CMySocket(Ip,bBlock){}

		int Listen();
		int Accpet();
	};

	class CNetSocket : public CMySocket
	{
	public:
		CNetSocket() :CMySocket(){}
		explicit CNetSocket(char * Ip, bool bBlock = false) :CMySocket(Ip, bBlock){}
		CNetSocket(int socket) :CMySocket(){ mSocket = socket;};
		int Connect(char * Ip, int port);
		int ReadData(char * pBuff, int nLen);
		int WriteData(char * pBuff, int nLen);
	};

	typedef map<int, CNetSocket> CNetSocketList;

	class CMyMessage
	{
	public:
		enum 
		{
			MAX_SOCKET_BUFF_SIZE = 64000,
		};

		CMyMessage();

	public:
	   virtual	int ParseMessageToBuff(char * pBuff, int& rLen);
	   virtual	int ParseBuffToMessage(char * pBuff, int nLen);
	public:
		int mLen;
		char mBuff[MAX_SOCKET_BUFF_SIZE];
	};

	typedef map<int, CNetSocket> CNetSocketList;

	class CDispatcher
	{
	public:
		void OnRecvCharMessage(char * pBuff, int nLen);

	};

	class CMyEpoll
	{
	public:
		enum
		{
			MAX_SOCKET_NUM = 1024,
		};

	public:
		CMyEpoll(char * Ip, CDispatcher * pDisPatcher, bool bBlock = false);
		~CMyEpoll();
		int InitEpoll(int port, int protoType = SOCK_STREAM);
		int RunEpoll(const int timeout);
		int AddEvent(const int fd, const int event);
		int DelEvent(const int fd);
		int ModEvent(const int fd, const int oldevent, const int newevent);


	public:
		CNetSocket * GetNetSocket(const int fd);
	public:
		struct epoll_event *events_;
		CNetSocketList mNetSocket;
		int mEpollFd;
		CListenSocket mListenSocket;
		CDispatcher * mDispatcher;
	};
}