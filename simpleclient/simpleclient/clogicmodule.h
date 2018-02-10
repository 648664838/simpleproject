#pragma once

#include "cmessage.h"
#include <string>
using namespace af;

class CLogicModule
{
public:
	//加载初始化
	virtual void OnInit() = 0;
	//加载完成
	virtual void OnComplete(){}
	//断开连接
	virtual void OnTimer(){}
	//断开连接
	virtual void OnEnd() = 0;
	//发送消息
	virtual void OnSendClientMessage(int s, CMessage * pMsg) = 0;
	//收到消息
	virtual void OnRecvServerMessage(int s, CMessage * pMsg) = 0;
};