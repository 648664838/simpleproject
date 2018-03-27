#include "cmessage.h"
#include <string>
#include "cplayer.h"
using namespace af;

class CLogicModule
{
public:
	//���س�ʼ��
	virtual void OnInit() = 0;
	//�������
	virtual void OnComplete(){}
	//�Ͽ�����
	virtual void OnTimer(int nOffset){}
	//�Ͽ�����
	virtual void OnEnd() = 0;
	//�յ���Ϣ
	virtual void OnRecvMessage(CPlayerData * pData, CMessage * pMsg) = 0;
};