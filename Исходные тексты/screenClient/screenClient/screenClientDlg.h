// screenClientDlg.h : header file
//

#pragma once

#include <boost/thread.hpp>
#include "cxImage/CxImage/ximage.h"
#include "Client.h"
#include "ConfigReader.h"
#include "afxcmn.h"
#include "ClientThrad.h"
// CscreenClientDlg dialog


class CscreenClientDlg : public CDialog
{
// Construction
public:
	int oldX;
	int screensX;
	void SetZoom(int i);
	boost::thread_group tg;
	ConfigReader cr;
	~CscreenClientDlg(){}
	CscreenClientDlg(CWnd* pParent = NULL);	// standard constructor

	vector<boost::shared_ptr<ClientThread>> clients;
// Dialog Data
	enum { IDD = IDD_SCREENCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CSliderCtrl m_slider;
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
