#pragma  once


#include "CSingleton.h"
#include "cmessage.h"
#include "clogicmodule.h"
#include <string>
#define MAX_MODULE_NUM 30

using namespace af;

class CMainLogic : public CMySingleton < CMainLogic >
{
public:
	CMainLogic(){}
	int MyMain();
	int OnTimer(unsigned int nIDEvent);
	int MyFinal();
public:
	void RegistModule();
	void InitModule();
	void LaunchComplete();
public:
	void ProcessMessage(CMessage * pMsg);
	void SendTimerSignal();

private:
	int mModuleNum;
	CLogicModule * mLogicModule[MAX_MODULE_NUM];
};