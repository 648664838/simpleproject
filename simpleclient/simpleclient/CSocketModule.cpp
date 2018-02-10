#include "CSocketModule.h"
#include "error.h"
#include "tinyxml.h"
#include "errno.h"
#include "cmessagemanager.h"

void CSocketModule::OnInit()
{
	memset(&mConnectInfo, 0, sizeof(mConnectInfo));
	mState = emConnect_Init;
	mCurNum = 0;
	memset(&mServerAddr, 0, sizeof(mServerAddr));
}

void CSocketModule::OnEnd()
{
	WSACleanup();
}

void CSocketModule::OnSendClientMessage(int s, CMessage * pMsg)
{

}
void CSocketModule::OnRecvServerMessage(int s, CMessage * pMsg)
{

}

int CSocketModule::InitClientSocket()
{
	//初始化winsocket环境
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData))
	{
		return EMCONERROR_WIN_SOCKET_INIT;
	}

	//初始化连接信息
	int tRet = InitConnectInfo();
	if (tRet != SUCCESS)
	{
		return tRet;
	}

	//初始化服务器地址
	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_port = htons(mConnectInfo.mPort);
	mServerAddr.sin_addr.s_addr = inet_addr(mConnectInfo.mIp);

	//初始化客户端socket，select列表
	FD_ZERO(&mUserSocket);

	//return ClientConnectServer();
	return SUCCESS;
}
int CSocketModule::InitConnectInfo()
{
	if (mState >= emConnect_Info)
	{
		return EMCONERROR_HAS_LOAD_CONINFO;
	}

	TiXmlDocument tXmlDoc;
	if (tXmlDoc.LoadFile(PATH_SERVER_ADDR) == false)
	{
		return EMCONERROR_SERVER_NOT_OPEN;
	}

	TiXmlElement * pRoot = tXmlDoc.RootElement();
	if (pRoot == NULL)
	{
		return EMCONERROR_SERVERFILE_IS_NULL;
	}

	int nTestServerIndex = -1;
	if (pRoot->Attribute("test") != NULL)
	{
		nTestServerIndex = atoi(pRoot->Attribute("test"));
	}
	if (nTestServerIndex < 0)
	{
		return	EMCONERROR_SERVER_INDEX_IS_NULL;
	}

	TiXmlElement * pElemServer = NULL;
	for (TiXmlElement * pElem = pRoot->FirstChildElement("server"); pElem != NULL; pElem = pElem->NextSiblingElement("server"))
	{
		if (nTestServerIndex <= 0)
		{
			pElemServer = pElem;
			break;
		}
		--nTestServerIndex;
	}

	if (pElemServer == NULL)
	{
		return EMCONERROR_SERVER_DATA_IS_NULL;
	}

	if (pElemServer->Attribute("name") == NULL ||
		pElemServer->Attribute("id") == NULL ||
		pElemServer->Attribute("ip") == NULL ||
		pElemServer->Attribute("port") == NULL 
		//|| pElemServer->Attribute("channel") == NULL
		)
	{
		return EMCONERROR_SERVER_DATA_NOT_FULL;
	}

	//mConnectInfo.mID = atoi(pElemServer->Attribute("id"));
	mConnectInfo.mPort = atoi(pElemServer->Attribute("port"));
	//mConnectInfo.mChannel = atoi(pElemServer->Attribute("channel"));
	strncpy(mConnectInfo.mName, pElemServer->Attribute("name"), sizeof(mConnectInfo.mName) - 1);
	strncpy(mConnectInfo.mIp, pElemServer->Attribute("ip"), sizeof(mConnectInfo.mIp) - 1);
	if (pElemServer->Attribute("status"))
	{
		mConnectInfo.mStatus = atoi(pElemServer->Attribute("status"));
	}

	mState = emConnect_Info;

	return SUCCESS;
}

bool CSocketModule::IsConnectServer()
{
	return mState == emConnect_Con;
}

int CSocketModule::ClientConnectServer()
{
	if (mState != emConnect_Info)
	{
		return EMCONERROR_NET_NOT_CON;
	}
	
	if (mCurNum >= FD_SETSIZE)
	{
		return EMCONERROR_CONNECT_USER_MAX;
	}
	int tRet = SUCCESS;
	mSocketFb = socket(AF_INET, SOCK_STREAM, 0);
	int nError = WSAGetLastError();
	if (nError != SUCCESS)
	{
		return nError;
	}

	if (mSocketFb < 0)
	{
		return EMCONERROR_SOCKET_CREATE;
	}

	unsigned long ul = 1;//1为非阻塞,0为阻塞
	tRet = ioctlsocket(mSocketFb, FIONBIO, (unsigned long*)&ul);
	if (tRet == SOCKET_ERROR)
	{
		return tRet;
	}

	connect(mSocketFb, (sockaddr *)&mServerAddr, sizeof(mServerAddr));
	nError = WSAGetLastError();
	FD_SET(mSocketFb, &mUserSocket);

	++mCurNum;

	mState = emConnect_Con;
	return SUCCESS;
}

int CSocketModule::HandleMessage()
{
	if (mState != emConnect_Con)
	{
		return EMCONERROR_NET_NOT_CON;
	}

	if (mCurNum <= 0)
	{
		return SUCCESS;
	}
	//TODO：暂时只有一个用户
	FD_SET readFds;
	FD_SET writeFds;
	FD_ZERO(&readFds);
	FD_ZERO(&writeFds);
	readFds = mUserSocket;
	writeFds = mUserSocket;

	FD_SET errorFds;
	FD_ZERO(&errorFds);
	errorFds = mUserSocket;

	int tRet = select(0, &readFds, &writeFds, &errorFds, NULL);

	if (tRet <= 0)
	{
		return ERROR_SELECT_MESSAGE_HADNLE;
	}

	for (int i = 0; i < readFds.fd_count; ++i)
	{
		char tBuff[MAX_SOCKET_BUFF_SIZE] = { 0 };
		int nBytes = recv(readFds.fd_array[i], tBuff, MAX_SOCKET_BUFF_SIZE, 0);
		if (nBytes <= 0)
		{
			ShutDown(readFds.fd_array[i]);
			continue;
		}

		CMessageManager::GetSingletonPtr()->OnReadServerBuff(tBuff, nBytes);
	}

	for (int i = 0; i < writeFds.fd_count; ++i)
	{
		while (true)
		{
			int nMessageLen = 0;
			char tBuff[MAX_SOCKET_BUFF_SIZE] = { 0 };

			CMessageManager::GetSingletonPtr()->GetSendOneMessageBuff(tBuff, nMessageLen);

			if (nMessageLen <= 0)
			{
				break;
			}
	
			tRet = send(writeFds.fd_array[i], tBuff, nMessageLen, 0);
			if (tRet <= 0 && errno != EINTR && errno != EAGAIN)
			{
				ShutDown(writeFds.fd_array[i]);
				break;
			}

			if (tRet != nMessageLen)
			{
				//LOG
			}
		}

	}

	return SUCCESS;
}

void CSocketModule::DispatchServerMessage()
{
	CMessageManager::GetSingletonPtr()->DispatchServerMessage();
}

void CSocketModule::SendServerMessage(CMessage * pMsg)
{
	CMessageManager::GetSingletonPtr()->SendServerMessage(pMsg);
}

void CSocketModule::ShutDown(SOCKET s)
{
	FD_CLR(s, &mUserSocket);
	shutdown(s, SD_BOTH);
	closesocket(s);
}