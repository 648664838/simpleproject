
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <mysql.h>
#include <iostream>
#include "scenelogic.h"

using namespace std;

#include "simplebasedata.h"

int main(int argc, const char * argv[])
{
	//CDataBase tDataBase;
	//tDataBase.Initialize("./config/mysqlinfo.xml");
	//bool bQuery = tDataBase.Query("select * from ccc");
	//CQueryResult & rQueryResult = tDataBase.GetQueryResult();
	//while (bQuery && rQueryResult.NextRow())
	//{
	//	CField* pField = rQueryResult.Fetch();
	//	if (pField != NULL)
	//	{
	//		for (int i = 0; i < rQueryResult.GetFieldCount(); ++i)
	//		{
	//			cout << pField[i].GetType() << "\t";
	//			cout << pField[i].GetString() << endl;
	//		}
	//	}
	//}

	af::CSceneLogic::GetSingletonPtr()->Init();

	return 0;
}
