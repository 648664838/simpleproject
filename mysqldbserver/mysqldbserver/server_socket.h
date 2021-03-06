#pragma once
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <map>


using namespace std;



namespace af
{

	class CClientHandle;

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
		virtual ~CMySocket(){}
		virtual int InitSocket(const char * Ip, int port, int protoType = 1);
		virtual void Close();

	public:
		void SetIp(const char * Ip);
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
		int Connect(const char * Ip, int port);
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
		CMyEpoll();
		~CMyEpoll();
		int InitEpoll();
		int InitEpoll(const char * Ip, int port, bool bBlock = false, int protoType = 1);
		void SetMessageManger(CClientHandle * pClientHandle){ mClientHandle = pClientHandle; }
		CNetSocket * GetNetSocket(const int fd);

		int Send(const int nSocket, char * pBuff, int nLen);
		int DelEvent(const int fd , bool bRecv = true);
		int RunEpoll(const int timeout);
	protected:

		int AddEvent(const int fd, const int event);
		int ModEvent(const int fd, const int oldevent, const int newevent);

	private:
		CNetSocketList mNetSocket;
		int mEpollFd;
		CListenSocket mListenSocket;
		CClientHandle * mClientHandle;
	};
}