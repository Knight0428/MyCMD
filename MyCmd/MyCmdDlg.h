﻿
// MyCmdDlg.h: 头文件
//

#pragma once


// CMyCmdDlg 对话框
class CMyCmdDlg : public CDialogEx
{
// 构造
public:
	CMyCmdDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYCMD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	HANDLE m_hRead = NULL;
	HANDLE m_hWrite = NULL;

	HANDLE m_hCmdRead = NULL;
	HANDLE m_hCmdWrite = NULL;


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strShow;
	CEdit m_edtShow;
};
