#pragma once

#include "cmessage.h"
#include <string>
using namespace af;

class CLogicModule
{
public:
	//���س�ʼ��
	virtual void OnInit() = 0;
	//�������
	virtual void OnComplete(){}
	//�Ͽ�����
	virtual void OnTimer(){}
	//�Ͽ�����
	virtual void OnEnd() = 0;
	//������Ϣ
	virtual void OnSendClientMessage(int s, CMessage * pMsg) = 0;
	//�յ���Ϣ
	virtual void OnRecvServerMessage(int s, CMessage * pMsg) = 0;
};