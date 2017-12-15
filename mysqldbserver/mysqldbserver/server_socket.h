#pragma once

#include <map>

using namespace std;

namespace af
{
	enum
	{
		MAX_SOCKET_BUFF_SIZE = 64 * 1024,
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
		CNetSocket() :CMySocket()
		{ 
			Initial();
		}
		explicit CNetSocket(char * Ip, bool bBlock = false) :CMySocket(Ip, bBlock)
		{ 
			Initial();
		}
		CNetSocket(int socket) :CMySocket()
		{ 
			Initial();
			mSocket = socket; 
		};
		void Initial()
		{
			mBuffLen = 0;
			memset(mBuff, 0, sizeof(mBuff));
		}
		int Connect(char * Ip, int port);
		int ReadData();
		int WriteData(char * pBuff, int nLen);
	public:
		int GetOneMessage(char * pBuff, int nLen);
	private:
		int mBuffLen;
		char mBuff[MAX_SOCKET_BUFF_SIZE * 2 + 1];
	};

	typedef map<int, CNetSocket> CNetSocketList;



	class CMyEpoll
	{
	public:
		enum
		{
			MAX_SOCKET_NUM = 1024,
		};

	public:
		CMyEpoll(char * Ip, CMessageManger * pDisPatcher, bool bBlock = false);
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
		CMessageManger * mMessageManger;
	};
}