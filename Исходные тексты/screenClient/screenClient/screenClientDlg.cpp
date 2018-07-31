// screenClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "screenClient.h"
#include "screenClientDlg.h"
#include <iostream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CscreenClientDlg dialog


CscreenClientDlg::CscreenClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CscreenClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	screensX = -160;
	oldX = 0;
}

void CscreenClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
}

BEGIN_MESSAGE_MAP(CscreenClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CscreenClientDlg::OnBnClickedButton1)
//	ON_WM_TIMER()
ON_WM_TIMER()
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CscreenClientDlg::OnNMReleasedcaptureSlider1)
ON_WM_MOUSEMOVE()
//ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CscreenClientDlg message handlers

struct workF{
	workF(boost::shared_ptr<ClientThread> pTh){
		pClietnThread = pTh;
	}

	boost::shared_ptr<ClientThread> pClietnThread;
	void operator()(){
		Sleep(rand()%3000+1000);
		while(1)
			pClietnThread->work();
	}
};
BOOL CscreenClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int counter=0;
	for (int i=0;i<100;i++){
		char z[10];
		itoa(i, z, 10);
		string stR = string("REMOTE") + string (z);
		if(cr[stR].size()>1){
			string remote = cr[stR];

			int port =boost::lexical_cast<int>(cr["PORT"].c_str());
			clients.push_back(boost::shared_ptr<ClientThread>(new ClientThread(remote, port)));
			tg.create_thread(workF(clients.back()));
		}
	}
	m_slider.SetRange(0, clients.size());
	m_slider.SetPos(clients.size());

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	int freq = 1000 / atoi(cr["FPS"].c_str());
	SetTimer(MYTIMER, freq, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CscreenClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CscreenClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}else{
		CPaintDC dc(this);
		int y=0;

		for(int i = 0;i<clients.size();i++){
			boost::mutex::scoped_lock lock(clients[i]->mutex);
			if (clients[i]->itype== BMPSMALL || clients[i]->itype==PNGSMALL){
			}else{
				if(clients[i]->pBitmap.get()){
					clients[i]->pBitmap->Draw(dc,screensX+160,5);
				}
			}
		}
		for(int i = 0;i<clients.size();i++){
			boost::mutex::scoped_lock lock(clients[i]->mutex);
			
			if (clients[i]->itype== BMPSMALL || clients[i]->itype==PNGSMALL){
				//paint	
				if(clients[i]->pBitmap.get()){
					clients[i]->pBitmap->Draw(dc,screensX,y,160,120);
					y+=clients[i]->pBitmap->GetHeight();
				}else{
					CBrush bl;
					bl.CreateSolidBrush(RGB(0,0,0));
					dc.FillRect(CRect(screensX,y,screensX+160,y+120),&bl);
					y+=120;
				}
			}else{
				CBrush bl;
				bl.CreateSolidBrush(RGB(40,50,60));
				dc.FillRect(CRect(screensX,y,screensX+160,y+120),&bl);
				y+=120;

				if(clients[i]->pBitmap.get()){
					//clients[i]->pBitmap->Draw(dc,0,5);
				}
			}
		}
	
		CDialog::OnPaint();
	}
}

void CscreenClientDlg::SetZoom(int i){
	if (i>clients.size())
		return;
	int k=0;
	string st = cr["FORMAT"];
	for(vector<boost::shared_ptr<ClientThread>>::iterator it = clients.begin();it!=clients.end();it++){
		if (k==i){
			if (st[0]=='P'){
				(*it)->itype = PNG;
			}else{
				(*it)->itype = BMP;
			}
			(*it)->client.pause = 1000 / atoi(cr["FPS"].c_str());;
		}else{
			(*it)->client.pause = 1000;
			if (st[0]=='P'){
				(*it)->itype = PNGSMALL;
			}else{
				(*it)->itype = BMPSMALL;
			}
		}			
		k++;
	}
	Invalidate(false);
};

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CscreenClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CscreenClientDlg::OnBnClickedButton1()
{
}

void CscreenClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==MYTIMER)
		Invalidate(FALSE);

	CDialog::OnTimer(nIDEvent);
}
void CscreenClientDlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int val = m_slider.GetPos();	
	SetZoom(val);

	*pResult = 0;
}

void CscreenClientDlg::OnMouseMove(UINT nFlags, CPoint point)
{	
	screensX=-160;
	if (point.x<170){
		screensX=0;
		InvalidateRect(CRect(0,0,160, 1024), FALSE);
	}
	if (oldX != screensX)
		Invalidate(TRUE);
	oldX = screensX;
	
	CDialog::OnMouseMove(nFlags, point);
}

//void CscreenClientDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	
//	CDialog::OnLButtonDblClk(nFlags, point);
//}

void CscreenClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (point.x<160){
		int z = point.y / 120;
		if (z<clients.size())
			SetZoom(z);

		InvalidateRect(CRect(0,0,160, 1024));
	}

	CDialog::OnLButtonDown(nFlags, point);
}