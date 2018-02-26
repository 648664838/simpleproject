#pragma once
#include "CSingleton.h"
#include "cplayerdata.h"

class CStaticData : public CMySingleton < CStaticData >
{
	friend CMySingleton < CStaticData > ;
private:
	CStaticData(){ InitData(); }
public:
	void InitData();
public:
	CPlayerData mPlayerData;
};