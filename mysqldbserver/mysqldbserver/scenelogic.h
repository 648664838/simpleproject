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

	class CSceneConfig
	{
	public:
		CSceneConfig()
		{
			mStartCharID = 0;
			mPort = 0;
			memset(mIp, 0, sizeof(mIp));
		}
	public:
		int mStartCharID;
		char mIp[MAX_IP_CHAR_LEN];
		int mPort;
	};
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
			mRegisterPlayerNum = 0;
		}

		~CSceneLogic()
		{

		}
	public:
		int  Init();
		void Final();

		void Run();



		void ProcessMessage(int nSocketID,CMessage * pMessage);

		CPlayerData * GetScenePlayerData(int nSocket);
		CSocketInfo * GetSceneConnectInfo(int nSocket);
		CSocketInfo * GetSceneVerifyData(int nSocket);
		void OnMessageLoginSceneRequest(int nSocket,CMessage * pMessage);
		void SendLoginSceneResponse(int nSocket, int nResult);

		void OnMessageCreateAccountRequest(int nSocket, CMessage * pMessage);
		void SendCreateAccountResponse(int nSocket, int nResult);

		int  LoadPlayerData(int nRoleId, CPlayerData & rPkayerData);
		void ProcessPlayerClientMessage(int nSocket, CMessage * pMessage);

		void OnMessageDisConnect(int nSocket);
	protected:
		bool LoadSceneConfig(const char * pPath); //���ط���������
		bool InitSceneData();

	private:
		CClientHandle mClientHandle;

		map<int,CSocketInfo>	mConnectSocket; //�������б�
		PlayerDataMap mPlayerData;  //�ѵ�½�б�

		CSceneConfig mSceneConfig;  //��ϷһЩȫ������
		int		  mRegisterPlayerNum; //��ע������
		CDataBase mDataBase;   //���ݿ�
	};
}

