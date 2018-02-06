#include "scenelogic.h"
#include "cmessage.h"
#include "cplayer.h"
#include <time.h>
#include "cscenetype.h"
#include "error.h"
#include "tinyxml.h"

namespace af
{
	int  CSceneLogic::Init()
	{
		bool nResult = LoadSceneConfig("./config/sceneconfig.xml");
		if (!nResult)
		{
			return -1;
		}

		mClientHandle.Init(mSceneConfig.mIp,mSceneConfig.mPort);
		mDataBase.Initialize("./config/mysqlinfo.xml");

		nResult = InitSceneData();
		if (!nResult)
		{
			return -2;
		}

		return SUCCESS;
	}

	void CSceneLogic::Final()
	{

	}

	bool CSceneLogic::LoadSceneConfig(const char * pPath)
	{
		if (pPath == NULL)
		{
			return false;
		}

		TiXmlDocument tDoc;
		if (!tDoc.LoadFile(pPath))
		{
			return false;
		}

		TiXmlElement *tpRoot = tDoc.FirstChildElement();
		if (tpRoot == NULL)
		{
			return false;
		}

		TiXmlElement *tpSceneInfo = tpRoot->FirstChildElement("sceneinfo");
		if (tpSceneInfo != NULL)
		{
			tpSceneInfo->Attribute("rolestartid", &mSceneConfig.mStartCharID);	
			tpSceneInfo->Attribute("port", &mSceneConfig.mPort);
			if (tpSceneInfo->Attribute("ip") != NULL)
			{
				strncpy(mSceneConfig.mIp, tpSceneInfo->Attribute("ip"), sizeof(mSceneConfig.mIp) - 1);
			}
		}

		return true;
	}

	bool CSceneLogic::InitSceneData()
	{
		bool bExecute = mDataBase.ExecuteSql("select count(*) from tb_account ");
		if (!bExecute || mDataBase.GetQueryResult().GetRowCount() <= 0)
		{	
			return false;
		}

		int nResult = mDataBase.GetQueryResult().NextRow();
		if (!nResult)
		{
			return false;
		}

		CField * pRegisterField = mDataBase.GetQueryResult().GetCurRowFieldByIndex(0);
		if (pRegisterField != NULL)
		{
			mRegisterPlayerNum = pRegisterField->GetInt32();
		}

		return true;
	}

	void CSceneLogic::Run()
	{
		mClientHandle.Recv();
	}

	void CSceneLogic::ProcessMessage(int nSocketID, CMessage * pMessage)
	{
		switch (pMessage->mSrcAddrType)
		{
		case emMessageAddrType_PlayerClient:
		{
			ProcessPlayerClientMessage(nSocketID, pMessage);
			break;
		}
		default:
			break;
		}

	}

	void CSceneLogic::ProcessPlayerClientMessage(int nSocketID, CMessage * pMessage)
	{
		switch (pMessage->mID)
		{
		case emMessageID_Connect:
		{
			CPlayerData * pPlayerData = GetScenePlayerData(nSocketID);
			if (pPlayerData != NULL)
			{//�ѵ�¼
				return;
			}
			CSocketInfo * tpInfo = GetSceneConnectInfo(nSocketID);
			if (tpInfo == NULL)
			{
				CSocketInfo tInfo;
				tInfo.mSocketID = nSocketID;
				tInfo.mLastTime = time(NULL);
				mConnectSocket.insert(make_pair(nSocketID,tInfo));
			}

			return;
		}
		case emMessageID_DisConnect:
		{
			OnMessageDisConnect(nSocketID);
			return;
		}
		case emMessageID_LoginSceneRequest:
		{
			OnMessageLoginSceneRequest(nSocketID,pMessage);
			return;
		}
		case emMessageID_CreateAccountRequest:
		{
			OnMessageCreateAccountRequest(nSocketID, pMessage);
			return;
		}
		default:
			break;
		}	

		CPlayerData * pPlayerData = GetScenePlayerData(nSocketID);
		if (pPlayerData == NULL)
		{
			return;
		}

		//for (int i = 0;i < mClientHandle)
	}


	void CSceneLogic::OnMessageDisConnect(int nSocket)
	{
		mPlayerData.erase(nSocket);
	}

	void CSceneLogic::OnMessageLoginSceneRequest(int nSocket, CMessage * pMessage)
	{
		if (nSocket < 0 || pMessage == NULL)
		{
			return;
		}

		CSocketInfo * tpInfo = GetSceneConnectInfo(nSocket);
		if (tpInfo == NULL)
		{
			SendLoginSceneResponse(nSocket, ERROR_LOGIN_SCENE_NOT_CONNECT);
			return;
		}
		CMessageLoginSceneRequest * pRequest = (CMessageLoginSceneRequest *)pMessage;
		//У���˺�
		bool bExecute = mDataBase.ExecuteSql("select role_id from tb_account where user_name ='%s' and password ='%s'", pRequest->mAccount, pRequest->mPassWord);
		if (!bExecute || mDataBase.GetQueryResult().GetRowCount() <= 0)
		{
			SendLoginSceneResponse(nSocket, ERROR_LOGIN_SCENE_ACOUNT_INVAILD);
			return;
		}

		int nRoleID = 0;
		int nResult = mDataBase.GetQueryResult().NextRow();
		if (nResult)
		{
			CField * pRoleIdField = mDataBase.GetQueryResult().GetCurRowFieldByIndex(0);
			if (pRoleIdField != NULL)
			{
				nRoleID = pRoleIdField->GetInt32();
			}
		}

		if (nRoleID <= 0)
		{
			SendLoginSceneResponse(nSocket, ERROR_LOGIN_SCENE_ROLEID_ZERO);
			return;
		}
		CPlayerData tPlayerData;
		nResult = LoadPlayerData(nRoleID, tPlayerData);
		if (nResult != SUCCESS)
		{
			SendLoginSceneResponse(nSocket, nResult);
			return;
		}
	
		mConnectSocket.erase(tpInfo->mSocketID);
		mPlayerData.insert(make_pair(nSocket, tPlayerData));

		SendLoginSceneResponse(nSocket, nResult);
	}

	int CSceneLogic::LoadPlayerData( int nRoleId, CPlayerData & rPlayerData)
	{
		if (nRoleId <= 0)
		{
			return ERROR_LOGIN_SCENE_ROLEID_ZERO;
		}
		//�����������
		bool bExecute = mDataBase.ExecuteSql("select role_money from tb_role where role_id ='%d'", nRoleId);
		if (!bExecute || mDataBase.GetQueryResult().GetRowCount() <= 0)
		{
			return ERROR_LOGIN_SCENE_ROLEDATA_NULL;
		}

		bool bResult = mDataBase.GetQueryResult().NextRow();
		if (!bResult)
		{
			return ERROR_LOGIN_SCENE_ROLEDATA_RESULT;
		}

		if(mDataBase.GetQueryResult().GetFieldCount() != 1  //�������
			||mDataBase.GetQueryResult().GetRowCount() <= 0
		  )
		{
			return ERROR_LOGIN_SCENE_ROLEDATA_INVALLD;
		}

		CField * pRoleIdField = mDataBase.GetQueryResult().Fetch();
		if (pRoleIdField == NULL)
		{
			return ERROR_LOGIN_SCENE_ROLEDATA_FECTH;
		}

		rPlayerData.mCharID = nRoleId;
		rPlayerData.mMoney = pRoleIdField[0].GetInt32();
		return SUCCESS;
	}

	void CSceneLogic::SendLoginSceneResponse(int nSocket, int nResult)
	{
		CMessageLoginSceneResponse tResponse;
		tResponse.mResult = nResult;
		mClientHandle.SendClientMessage(nSocket, &tResponse);
	}

	void CSceneLogic::OnMessageCreateAccountRequest(int nSocket, CMessage * pMessage)
	{
		if (nSocket < 0 || pMessage == NULL)
		{
			return;
		}
		CMessageCreateAccountRequest * pRequest = (CMessageCreateAccountRequest *)pMessage;
		bool bExecute = mDataBase.ExecuteSql("select role_id from tb_account where user_name ='%s'", pRequest->mAccount);
		if (bExecute && mDataBase.GetQueryResult().GetRowCount() > 0)
		{
			SendCreateAccountResponse(nSocket, ERROR_CREATE_ACCOUNT_EXIST);
			return;
		}

		//����
		int nRoleID = mSceneConfig.mStartCharID + mRegisterPlayerNum + 1;
		bExecute = mDataBase.ExecuteSql("insert into tb_account (role_id,user_name,password,create_time)  value(%d,%s,%s,%d) ", nRoleID, pRequest->mAccount, pRequest->mPassWord, time(NULL));
		if (!bExecute)
		{
			SendCreateAccountResponse(nSocket, ERROR_CREATE_ACCOUNT_EXIST);
			return;
		}
		//�����������
		bExecute = mDataBase.ExecuteSql("insert into tb_role(role_id) value(%d) ", nRoleID);
		if (!bExecute)
		{
			//LOG
			SendCreateAccountResponse(nSocket, ERROR_CREATE_ACCOUNT_ROLE);
			return;
		}
		++mRegisterPlayerNum;
		SendCreateAccountResponse(nSocket, SUCCESS);
	}
	void CSceneLogic::SendCreateAccountResponse(int nSocket, int nResult)
	{
		CMessageCreateAccountResponse tResponse;
		tResponse.mResult = nResult;
		mClientHandle.SendClientMessage(nSocket, &tResponse);
	}

	CPlayerData * CSceneLogic::GetScenePlayerData(int nSocket)
	{
		PlayerDataIter it = mPlayerData.find(nSocket);
		if (it == mPlayerData.end())
		{
			return NULL;
		}

		return &it->second;
	}

	CSceneLogic::CSocketInfo * CSceneLogic::GetSceneConnectInfo(int nSocket)
	{
		ConnectSocketIter it = mConnectSocket.find(nSocket);
		if (it == mConnectSocket.end())
		{
			return NULL;
		}

		return &it->second;
	}

}
