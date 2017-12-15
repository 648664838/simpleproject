#include "server_socket.h"
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>
#include "cmessage.h"
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

	CMySocket::CMySocket(char * Ip, bool bBlock) : mBlock(bBlock), mSocket(0), mPort(0)
	{
		strncpy(mIp, Ip, sizeof(mIp) - 1);
	}

	//create and bind socket
	//if port is zero, random port;
	//if ip is Null, INADDR_ANY
	int CMySocket::InitSocket(int port, int protoType)
	{
		int ret = 0;
		mSocket = socket(AF_INET, protoType, 0);
		if (mSocket < 0)
		{
			return INVALID_SOCKET;
		}
		
		sockaddr_in tServerAddr;
		memset(&tServerAddr, 0, sizeof(tServerAddr));
		tServerAddr.sin_family = AF_INET;
		tServerAddr.sin_port = htons(port);
		tServerAddr.sin_addr.s_addr = inet_addr(mIp == NULL ? INADDR_ANY : mIp);

		ret = bind(mSocket, (struct sockaddr *) &tServerAddr, sizeof(tServerAddr));

		return ret
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
		return accept(mSocket, (sockaddr *)&tClentAddr, (socklen_t*)&nAddrLen));
	}

	int CNetSocket::ReadData(char * pBuff, int nLen)
	{
		if (pBuff == NULL || nLen <= 0 )
		{
			return 0;  
		}

		if (mSocket <= 0)
		{
			return -1;
		}

		memset(pBuff, 0, sizeof(pBuff));

		return read(mSocket, mBuff + mBuffLen, nLen);
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

		int nMsgSize = sizeof(CMessage);
		if (mBuffLen >= nMsgSize)
		{
			CMessage * pMsg = (CMessage *)mBuff;
			if (pMsg->mSize <= mBuffLen)
			{
				memmove(pBuff, mBuff, pMsg->mSize);
				mBuffLen -= pMsg->mSize;
			}
		}
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

	int CNetSocket::Connect(char * Ip, int port)
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

	CMyMessage::CMyMessage()
	{
		mLen = 0;
		memset(mBuff, 0, sizeof(mBuff));
	}

	int CMyMessage::ParseBuffToMessage(char * pBuff, int nLen)
	{
		mLen = nLen;
		strncpy(mBuff, pBuff, sizeof(mBuff) - 1);
	}
	int CMyMessage::ParseMessageToBuff(char * pBuff, int& rLen)
	{
		memset(pBuff, 0, sizeof(pBuff));
		strncpy(pBuff, mBuff, mLen);
		rLen = mLen;
	}

	CMyEpoll::CMyEpoll(char * Ip, CMessageManger * pDisPatcher, bool bBlock = false) : mListenSocket(Ip, bBlock)
	{
		mEpollFd = 0;
		mMessageManger = pDisPatcher;
	}

	int CMyEpoll::InitEpoll(int port, int protoType)
	{
		mEpollFd = epoll_create(MAX_SOCKET_NUM);

		if (mEpollFd < 0)
		{
			return -1;
		}

		events_ = (struct epoll_event *)malloc(
			sizeof(struct epoll_event) * MAX_SOCKET_NUM);

		mListenSocket.InitSocket( port,  protoType);
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
				if (events_[i].data.fd == mListenSocket.GetSocket())
				{ 
					int nFd = 0;
					while ((nFd = mListenSocket.Accpet())> 0)
					{
						SetNonBlock(nFd);
						AddEvent(nFd, EPOLLIN | EPOLLERR | EPOLLHUP);
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
					int nBuffNum = pNetSocket->ReadData();
					while (true)
					{
						nBuffNum = pNetSocket->GetOneMessage(pBuff, nLen);
						if (nBuffNum <= 0)
						{
							break;
						}

						if (mMessageManger)
						{
							mMessageManger->OnRecvCharMessage(pNetSocket, pBuff, nBuffNum);
						}
					}
					if (nBuffNum <= 0)
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
		mNetSocket.insert(make_pair(fd,CNetSocket(fd))

		struct epoll_event epollevent;
		epollevent.data.fd = fd;
		epollevent.events = event;
		return epoll_ctl(mEpollFd, EPOLL_CTL_ADD, fd, &epollevent);
	}
	int CMyEpoll::DelEvent(const int fd)
	{
		CNetSocketList::iterator it = mNetSocket.find(fd);
		if (it != mNetSocket.end())
		{
			it->second.Close();
			mNetSocket.erase(fd);
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