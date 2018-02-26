#pragma once
#include "stdafx.h"
#include "Resource.h"

class CLoginDialog : public CDialogEx
{
public:
	CLoginDialog();

	// 对话框数据
	enum { IDD = IDD_DIALOG_LOGIN };
	void  LoginServerResponse(int nResult);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


private:
	CString mAccount;
	CString mPassWord;
	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLogin();
};

