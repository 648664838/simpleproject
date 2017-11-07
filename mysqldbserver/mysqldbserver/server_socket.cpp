#include "server_socket.h"
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
using namespace std;

namespace af
{
	void CMySocket::SetNonBlock()
	{
		int flags = 0;
		if ((flags = fcntl(mSocket, F_GETFL, 0)) < 0)
		{
			Close();
			return;
		}
		flags |= O_NONBLOCK;
		if (fcntl(mSocket, F_SETFL, flags) < 0)
		{
			Close();
			return;
		}
		return;
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

	int CListenSocket::InitSocket(int port, int protoType)
	{ 
		int ret = CMySocket::InitSocket(port, protoType);
		if (ret < 0)
		{
			return ret;
		}

		ret = listen(mSocket, 1024);
		if (ret < 0)
		{
			return ret;
		}

		if (mBlock == false)
		{
			SetNonBlock();
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
			return 1; 
		}

		if (mSocket <= 0)
		{
			return 0;
		}

		memset(pBuff, 0, sizeof(pBuff));
		int nBuffNum = read(mSocket, pBuff, nLen);
		if (nBuffNum <= 0)
		{
			Close();
		}
		return nBuffNum;
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

		int nBuffNum = write(mSocket, pBuff, nLen);
		if (nBuffNum <= 0)
		{
			Close();
		}
		return nBuffNum;
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


}