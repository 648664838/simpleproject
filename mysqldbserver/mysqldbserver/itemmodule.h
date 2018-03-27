#include "clogicmodule.h"
#include "CSingleton.h"


class CItemModule : public CMySingleton<CItemModule>, public CLogicModule
{
	friend CMySingleton < CItemModule >;
private:
	CItemModule()
	{
	}

	~CItemModule()
	{
	}

public:
	void OnInit();
	void OnEnd();
	//void OnTimer(int nOffset);
	//�յ���Ϣ
	virtual void OnRecvMessage(CPlayerData * pData, CMessage * pMsg) = 0;
public:
	
};