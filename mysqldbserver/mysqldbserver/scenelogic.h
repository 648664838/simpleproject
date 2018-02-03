#pragma once

#include "clienthandle.h"
#include "cplayer.h"
#include "CSingleton.h"
#include "simplebasedata.h"
#include <map>

using namespace std;

namespace af
{
	class CMessage;

	class CSceneLogic : public CMySingleton<CSceneLogic>
	{
		friend class CMySingleton < CSceneLogic > ;
	public:
		class CSocketInfo
		{
		public:
			int mSocketID;
			int mLastTime;

			bool operator < (const CSocketInfo& other)  const
			{
				return mSocketID < other.mSocketID;
			}
		};

	public:

		typedef map<int, CPlayerData> PlayerDataMap;
		typedef PlayerDataMap::iterator PlayerDataIter;

		typedef	map<int, CSocketInfo>	ConnectSocketMap;
		typedef	ConnectSocketMap::iterator	ConnectSocketIter;

	private:
		CSceneLogic()
		{
			Init();
		}

		~CSceneLogic()
		{
			Final();
		}
	public:
		void Init();
		void Final();

		void Run();

		void ProcessMessage(int nSocketID,CMessage * pMessage);

		CPlayerData * GetScenePlayerData(int nSocket);
		CSocketInfo * GetSceneConnectInfo(int nSocket);
		CSocketInfo * GetSceneVerifyData(int nSocket);
		void OnMessageLoginSceneRequest(int nSocket,CMessage * pMessage);
		void SendLoginSceneResponse(int nSocket, int nResult);
		int  LoadPlayerData(int nSocket, int nRoleId, CPlayerData & rPkayerData);
	protected:
		void ProcessPlayerClientMessage(int nSocket, CMessage * pMessage);
	private:
		CClientHandle mClientHandle;

		map<int,CSocketInfo>	mConnectSocket; //已连接列表
		PlayerDataMap mPlayerData;  //已登陆列表

		CDataBase mDataBase;   //数据库
	};
}

