#pragma once
#include "stdafx.h"
#include "Resource.h"

class CLoginDialog : public CDialogEx
{
public:
	CLoginDialog();

	// �Ի�������
	enum { IDD = IDD_DIALOG_LOGIN };
	void  LoginServerResponse(int nResult);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


private:
	CString mAccount;
	CString mPassWord;
	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLogin();
};

