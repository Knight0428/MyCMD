﻿
// MyCmdDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MyCmd.h"
#include "MyCmdDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyCmdDlg 对话框



CMyCmdDlg::CMyCmdDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYCMD_DIALOG, pParent)
	, m_strShow(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDT_READ, m_strShow);
	DDX_Control(pDX, EDT_READ, m_edtShow);
}

BEGIN_MESSAGE_MAP(CMyCmdDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_OK, &CMyCmdDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMyCmdDlg 消息处理程序

BOOL CMyCmdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	//创建管道
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	BOOL bRet = CreatePipe(
		&m_hCmdRead, //读取句柄，用于从管道读取数据
		&m_hWrite, //写入句柄，向管道写入数据
		&sa,
		0 //使用默认的缓冲区大小
	);
	if (!bRet)
	{
		AfxMessageBox("创建管道失败");
	}

	bRet = CreatePipe(
		&m_hRead, //读取句柄，用于从管道读取数据
		&m_hCmdWrite, //写入句柄，向管道写入数据
		&sa,
		0 //使用默认的缓冲区大小
	);
	if (!bRet)
	{
		AfxMessageBox("创建管道失败");
	}

	//启动cmd进程
	STARTUPINFO si;
	//结构体置零，然后再赋值
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = m_hCmdRead; //子进程读数据
	si.hStdOutput = m_hCmdWrite;//子进程写数据
	si.hStdError = m_hCmdWrite;//子进程写错误反馈

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	bRet = ::CreateProcess(
		NULL,
		"CMD.exe",
		NULL,
		NULL,
		TRUE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si,
		&pi);
	if (!bRet)
	{
		AfxMessageBox("自进程创建失败");
	}

	//启动定时器
	SetTimer(1, 100, NULL);
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyCmdDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyCmdDlg::OnPaint()
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
HCURSOR CMyCmdDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyCmdDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	//发数据
	CString strDir;
	GetDlgItemText(EDT_WRITE, strDir); 
	strDir += "\r\n";

	//写入管道
	DWORD dwBytesWrited = 0;
	BOOL bRet = ::WriteFile(
		m_hWrite,
		strDir.GetBuffer(0),
		strDir.GetLength(),
		&dwBytesWrited,
		NULL);
	if (!bRet)
	{
		AfxMessageBox("写入管道失败");
	}

	/*
	//不能直接读，要等cmd执行完指令再读，所以要延迟读取，并且有些指令如ping不是一次性执行完的，
	//所以最好使用定时器一直从管道读取数据
	Sleep(1000);

	//读数据
	CString csBuff;

	DWORD dwBytesAvail = 0;
	DWORD dwBytesLeft = 0;
	bRet = PeekNamedPipe(
		m_hRead,
		NULL,
		0,
		NULL,
		&dwBytesAvail,
		&dwBytesLeft
	);
	if (!bRet)
	{
		AfxMessageBox("获取管道信息失败");
	}

	if (dwBytesAvail > 0)
	{
		DWORD dwBytesRead = 0;
		bRet = ::ReadFile(
			m_hRead,
			csBuff.GetBufferSetLength(MAXBYTE),
			MAXBYTE,
			&dwBytesRead,
			NULL);
		csBuff.ReleaseBuffer(dwBytesRead);
		if (!bRet)
		{
			AfxMessageBox("读取管道失败");
		}

	}

	//显示界面
	SetDlgItemText(EDT_READ, csBuff);
	
	*/

}


void CMyCmdDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//读数据
	CString csBuff;

	DWORD dwBytesAvail = 0;
	DWORD dwBytesLeft = 0;
	BOOL bRet = PeekNamedPipe(
		m_hRead,
		NULL,
		0,
		NULL,
		&dwBytesAvail,
		&dwBytesLeft
	);
	if (!bRet)
	{
		AfxMessageBox("获取管道信息失败");
	}

	if (dwBytesAvail > 0)
	{
		DWORD dwBytesRead = 0;
		bRet = ::ReadFile(
			m_hRead,
			csBuff.GetBufferSetLength(MAXBYTE),
			MAXBYTE,
			&dwBytesRead,
			NULL);
		csBuff.ReleaseBuffer(dwBytesRead);
		if (!bRet)
		{
			AfxMessageBox("读取管道失败");
		}

	}

	//显示界面
	//SetDlgItemText(EDT_READ, csBuff);
	//使用成员保留之前的数据
	m_strShow += csBuff;
	//有新增字符才刷新界面，否则会一直闪
	if (!csBuff.IsEmpty())
	{
		UpdateData(FALSE);
	}

	//设置滚动条一直处于输出末尾
	m_edtShow.SetSel(m_strShow.GetLength(), m_strShow.GetLength());

	CDialogEx::OnTimer(nIDEvent);
}
