
// simpleclientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "simpleclient.h"
#include "simpleclientDlg.h"
#include "afxdialogex.h"
#include "cmainlogic.h"
#include "cdata.h"
#include "CLoginDialog.h"
#include "cdialogstatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsimpleclientDlg 对话框


CsimpleclientDlg::CsimpleclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsimpleclientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsimpleclientDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDC_EDIT1, CStaticData::GetSingletonPtr()->mPlayerData.mMoney);

	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CsimpleclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CsimpleclientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CsimpleclientDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CsimpleclientDlg 消息处理程序

BOOL CsimpleclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	// 添加登录系统命令
	ASSERT((IDM_DIALOG_LOGIN & 0xFFF0) == IDM_DIALOG_LOGIN);
	ASSERT(IDM_DIALOG_LOGIN < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
			pSysMenu->AppendMenu(MF_STRING, IDM_DIALOG_LOGIN, _T("登录"));
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	int tRet = CMainLogic::GetSingletonPtr()->MyMain();

	SetTimer(WM_TIMER, 1000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CsimpleclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == IDM_DIALOG_LOGIN)
	{
		CLoginDialog dlgLogin;
		CDialogStatic::mCurLoginDlg = &dlgLogin;
		dlgLogin.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsimpleclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CsimpleclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CsimpleclientDlg::OnTimer(UINT_PTR nIDEvent)
{
	int tRet = CMainLogic::GetSingletonPtr()->OnTimer(nIDEvent);
	if (tRet != SUCCESS)
	{

	}


	UpdateData(FALSE);

	CDialog::OnTimer(nIDEvent);
}



//TODO：暂时
void CsimpleclientDlg::OnBnClickedOk()
{
	 CDialogEx::OnOK();
}

//TODO：暂时
void CsimpleclientDlg::OnBnClickedCancel()
{
	//CMainLogic::GetSingletonPtr()->LoginServer("125", "321");

	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
