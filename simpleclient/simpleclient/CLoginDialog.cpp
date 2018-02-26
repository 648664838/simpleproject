
#include "CLoginDialog.h"
#include "cclienttype.h"
#include "cclienttool.h"
#include "cloginmodule.h"
#include <string>
#include "cdialogstatic.h"

BEGIN_MESSAGE_MAP(CLoginDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDialog::OnBnClickedLogin)
END_MESSAGE_MAP()

CLoginDialog::CLoginDialog() : CDialogEx(CLoginDialog::IDD)
{
}

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, mAccount);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, mPassWord);
	CDialogEx::DoDataExchange(pDX);
}

void CLoginDialog::LoginServerResponse(int nResult)
{
	if (nResult == SUCCESS)
	{
		MessageBox(_T("µÇÂ¼³É¹¦"));
		CDialogEx::OnCancel();
		CDialogStatic::mCurLoginDlg = NULL;
	}
	else
	{
		MessageBox(_T("µÇÂ¼Ê§°Ü %d",nResult));
	}
}

void CLoginDialog::OnBnClickedLogin()
{
	UpdateData(TRUE);

	char pAccount[MAX_CSTRING_BUFF_SIZE] = { 0 };
	UnicodeToUTF8(mAccount, pAccount, MAX_CSTRING_BUFF_SIZE);

	char pPassWord[MAX_CSTRING_BUFF_SIZE] = { 0 };
	UnicodeToUTF8(mPassWord, pPassWord, MAX_CSTRING_BUFF_SIZE);
	CLoginModule::GetSingletonPtr()->LoginServer(pAccount, pPassWord);
}
