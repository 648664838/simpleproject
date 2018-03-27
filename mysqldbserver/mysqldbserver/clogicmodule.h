#include "cmessage.h"
#include <string>
#include "cplayer.h"
using namespace af;

class CLogicModule
{
public:
	//加载初始化
	virtual void OnInit() = 0;
	//加载完成
	virtual void OnComplete(){}
	//断开连接
	virtual void OnTimer(int nOffset){}
	//断开连接
	virtual void OnEnd() = 0;
	//收到消息
	virtual void OnRecvMessage(CPlayerData * pData, CMessage * pMsg) = 0;
};