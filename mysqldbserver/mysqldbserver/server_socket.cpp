#include "server_socket.h"
#include <stdlib.h>
#include "cmessage.h"
#include "clienthandle.h"
using namespace std;

namespace af
{
	//block socket
	int SetNonBlock(int fd)
	{
		int flags = 0;
		if ((flags = fcntl(fd, F_GETFL, 0)) < 0)
		{
			return -1;
		}
		flags |= O_NONBLOCK;
		if (fcntl(fd, F_SETFL, flags) < 0)
		{
			return -1;
		}
		return 0;
	}


	CMySocket::CMySocket()
	{
		memset(mIp,0,sizeof(mIp));
		mBlock = false;
		mSocket = 0;
		mPort = 0;
	}

	void CMySocket::SetIp(const char * Ip)
	{
		if (Ip == NULL)
		{
			return;
		}
		strncpy(mIp, Ip, sizeof(mIp) - 1);
	}

	//create and bind socket
	//if port is zero, random port;
	//if ip is Null, INADDR_ANY
	int CMySocket::InitSocket(const char * Ip, int port, int protoType)
	{
		int ret = 0;

		mSocket = socket(AF_INET, protoType, 0);
		if (mSocket < 0)
		{
			return -1;
		}

		SetIp(Ip);

		sockaddr_in tServerAddr;
		memset(&tServerAddr, 0, sizeof(tServerAddr));
		tServerAddr.sin_family = AF_INET;
		tServerAddr.sin_port = htons(port);
		tServerAddr.sin_addr.s_addr = inet_addr(mIp);

		ret = bind(mSocket, (struct sockaddr *) &tServerAddr, sizeof(tServerAddr));

		return ret;
	}

	void CMySocket::Close()
	{
		close(mSocket);
		mSocket = 0;
	}

	int CListenSocket::Listen()
	{
		int ret = listen(mSocket, 1024);
		if (ret < 0)
		{
			return ret;
		}

		if (mBlock == false)
		{
			ret = SetNonBlock(mSocket);
			if (ret < 0)
			{
				Close();
			}
		}
		return ret;
	}

	int CListenSocket::Accpet()
	{
		sockaddr_in tClentAddr;
		int nAddrLen = sizeof(sockaddr_in);
		return accept(mSocket, (sockaddr *)&tClentAddr, (socklen_t*)&nAddrLen);
	}

	int CNetSocket::ReadData()
	{
		if (mSocket <= 0)
		{
			return -1;
		}

		return read(mSocket, mBuff + mBuffLen, MAX_SOCKET_BUFF_SIZE * 2 + 1 - mBuffLen);
	}

	int CNetSocket::GetOneMessage(char * pBuff, int nLen)
	{
		if (pBuff == NULL || nLen <= 0)
		{
			return 0;
		}

		if (mBuffLen <= 0)
		{
			return 0;
		}
		int nMsgLen = 0;
		int nMsgSize = sizeof(CMessage);
		if (mBuffLen >= nMsgSize)
		{
			CMessage * pMsg = (CMessage *)mBuff;
			if (pMsg->mSize <= mBuffLen)
			{
				memmove(pBuff, mBuff, pMsg->mSize);
				nMsgLen = pMsg->mSize;
				mBuffLen -= pMsg->mSize;
			}
		}
		return nMsgLen;
	}

	int CNetSocket::WriteData(char * pBuff, int nLen)
	{
		if (pBuff == NULL || nLen <= 0)
		{
			return 1;
		}

		if (mSocket <= 0)
		{
			return 0;
		}

		return write(mSocket, pBuff, nLen);
	}

	int CNetSocket::Connect(const char * Ip, int port)
	{	
		sockaddr_in tServerAddr;
		tServerAddr.sin_family = AF_INET;
		tServerAddr.sin_port = htons(port);
		tServerAddr.sin_addr.s_addr = inet_addr(Ip == NULL?"127.0.0.1": Ip);

		connect(mSocket, (sockaddr *)&tServerAddr, sizeof(tServerAddr));
		if (errno != 0)
		{
			Close();
			return errno;
		}

		return 0;
	}

	CMyEpoll::CMyEpoll()
	{
		mEpollFd = 0;
		mClientHandle = NULL;
	}
	

	int CMyEpoll::InitEpoll()
	{
		mEpollFd = epoll_create(MAX_SOCKET_NUM);

		if (mEpollFd < 0)
		{
			return -1;
		}

		events_ = (struct epoll_event *)malloc(
			sizeof(struct epoll_event) * MAX_SOCKET_NUM);

		return 0;
	}

	CMyEpoll::~CMyEpoll()
	{
		if (events_)
		{
			delete events_;
			events_ = NULL;
		}

	}

	int CMyEpoll::InitEpoll(const char * Ip, int port, bool bBlock, int protoType)
	{
		InitEpoll();
		mListenSocket.InitSocket(Ip, port, protoType);
		mListenSocket.Listen();
		return 0;
	}

	int CMyEpoll::RunEpoll(const int timeout)
	{
		int retval, numevents = 0;
		retval = epoll_wait(mEpollFd, events_, MAX_SOCKET_NUM, timeout);
		if (retval > 0)
		{
			for (int i = 0; i < retval; ++i)
			{
				if (mListenSocket.GetSocket() > 0 && events_[i].data.fd == mListenSocket.GetSocket())
				{ 
					int nFd = 0;
					while ((nFd = mListenSocket.Accpet())> 0)
					{
						SetNonBlock(nFd);
						AddEvent(nFd, EPOLLIN | EPOLLERR | EPOLLHUP);

						if (mClientHandle)
						{
							mClientHandle->OnConnectSocket(nFd);
						}
					}
				}
				else if (events_[i].events & EPOLLIN)
				{
					CNetSocket * pNetSocket = GetNetSocket(events_[i].data.fd);
					if (pNetSocket == NULL)
					{
						DelEvent(events_[i].data.fd);
						continue;
					}
					char pBuff[MAX_SOCKET_BUFF_SIZE];
					int nLen = MAX_SOCKET_BUFF_SIZE;
					int tBuffNum = pNetSocket->ReadData();
					while (true)
					{
						int nBuffNum = pNetSocket->GetOneMessage(pBuff, nLen);
						if (nBuffNum <= 0)
						{
							break;
						}

						if (mClientHandle)
						{
							mClientHandle->OnRecvMessage(pNetSocket->GetSocket(), (CMessage *)pBuff);
						}
					}
					if (tBuffNum <= 0)
					{
						DelEvent(events_[i].data.fd);
						continue;
					}
				}
			}
		}
	}

	CNetSocket * CMyEpoll::GetNetSocket(const int fd)
	{
		CNetSocketList::iterator it = mNetSocket.find(fd);
		if (it == mNetSocket.end())
		{
			return NULL;
		}

		return  &(it->second);
	}

	int CMyEpoll::AddEvent(const int fd, const int event)
	{
		mNetSocket.insert(make_pair(fd, CNetSocket(fd)));

		struct epoll_event epollevent;
		epollevent.data.fd = fd;
		epollevent.events = event;
		return epoll_ctl(mEpollFd, EPOLL_CTL_ADD, fd, &epollevent);
	}
	int CMyEpoll::DelEvent(const int fd, bool bRecv)
	{
		CNetSocketList::iterator it = mNetSocket.find(fd);
		if (it != mNetSocket.end())
		{
			it->second.Close();
			mNetSocket.erase(fd);
		}

		if (bRecv && mClientHandle)
		{
			mClientHandle->OnDisConnectSokcet(fd);
		}
		struct epoll_event epollevent;
		epollevent.data.fd = fd;
		return epoll_ctl(mEpollFd, EPOLL_CTL_DEL, fd, &epollevent);
	}
	int CMyEpoll::ModEvent(const int fd, const int oldevent, const int newevent)
	{
		struct epoll_event epollevent;
		epollevent.data.fd = fd;
		epollevent.events = (oldevent | newevent);
		return epoll_ctl(mEpollFd, EPOLL_CTL_MOD, fd, &epollevent);
	}
}