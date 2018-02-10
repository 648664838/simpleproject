#include "cmessagemanager.h"
#include <string>
#include "cmainlogic.h"

void CMessageManager::Init()
{
	mSendLength = 0;
	memset(mSendBuff, 0, sizeof(mSendBuff));

	mRecvLength = 0;
	memset(mRecvBuff, 0, sizeof(mRecvBuff));
}
void CMessageManager::Final()
{

}

int CMessageManager::DispatchServerMessage()
{
	int nMsgNum = MAX_ONCE_MSG_LEGNTH;

	while (nMsgNum)
	{
		char pBuff[MAX_SOCKET_BUFF_SIZE];
		int nLen = 0;

		GetRecvOneMessageBuff(pBuff, nLen);
		if (nLen <= 0)
		{
			break;
		}

		CMainLogic::GetSingletonPtr()->ProcessMessage((CMessage*)pBuff);
	}
	return MAX_ONCE_MSG_LEGNTH - nMsgNum;
}

void CMessageManager::SendServerMessage(CMessage * pMsg)
{
	if (pMsg == NULL)
	{
		return;
	}
	pMsg->mSrcAddrType = emMessageAddrType_PlayerClient;
	OnSendServerBuff((char *)pMsg, pMsg->mSize);
}

void CMessageManager::OnSendServerBuff(char * pBuff, int nLen)
{
	if (pBuff == NULL || nLen + mSendLength >= MAX_SOCKET_BUFF_LENGTH)
	{
		//LOG
		return;
	}

	memmove(mSendBuff + mSendLength, pBuff, nLen);
	mSendLength += nLen;
}

void CMessageManager::OnReadServerBuff(char * pBuff, int nLen)
{
	if (pBuff == NULL || nLen + mRecvLength >= MAX_SOCKET_BUFF_LENGTH)
	{
		//LOG
		return;
	}

	memmove(mRecvBuff + mRecvLength, pBuff, nLen);
	mRecvLength += nLen;
}

void CMessageManager::GetSendOneMessageBuff(char * pBuff, int & nLen)
{
	GetOneMessageBuff(mSendBuff, mSendLength, pBuff, nLen);
}

void CMessageManager::GetRecvOneMessageBuff(char * pBuff, int & nLen)
{
	GetOneMessageBuff(mRecvBuff, mRecvLength, pBuff, nLen);
}

//等于Buff和message的转换
void CMessageManager::GetOneMessageBuff(char * pSrcBuff, int & rSrcLen, char * pDstBuff, int & rDstLen)
{
	if (pDstBuff == NULL || pSrcBuff == NULL || rSrcLen <= 0)
	{
		return;
	}
	
	CMessage * pMsg = (CMessage *)pSrcBuff;
	if (pMsg->mSize > rSrcLen)
	{
		return;
	}

	memmove(pDstBuff, pSrcBuff, pMsg->mSize);
	rDstLen = pMsg->mSize;

	rSrcLen -= pMsg->mSize;
	memmove(pSrcBuff, pSrcBuff + pMsg->mSize, rSrcLen);
}