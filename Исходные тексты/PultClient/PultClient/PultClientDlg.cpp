// PultClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PultClient.h"
#include "PultClientDlg.h"
#include "Client.h"
#include "ClientThrad.h"
#include "ConfigReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

struct workF;

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


// CPultClientDlg dialog

struct workF{
	workF(boost::shared_ptr<ClientThread> pTh){
		pClietnThread = pTh;
	}

	boost::shared_ptr<ClientThread> pClietnThread;
	void operator()(){
		pClietnThread->work();
		//AfxMessageBox("done!");
	}
};

CPultClientDlg::CPultClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPultClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPultClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, slideCtrl);
	DDX_Control(pDX, IDC_BUTTON1, btn1);
	DDX_Control(pDX, IDC_BUTTON2, btn2);
	DDX_Control(pDX, IDC_BUTTON3, btn3);
	DDX_Control(pDX, IDC_BUTTON4, btn4);
}

BEGIN_MESSAGE_MAP(CPultClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPultClientDlg::OnBnClickedButton1)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDOK, &CPultClientDlg::OnBnClickedOk)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CPultClientDlg::OnNMReleasedcaptureSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CPultClientDlg::OnNMCustomdrawSlider1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &CPultClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPultClientDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPultClientDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CPultClientDlg message handlers

BOOL CPultClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

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
	SetTimer(0,300,0);

	ConfigReader cr;
	string remote = cr["REMOTE"];
	int port =boost::lexical_cast<int>(cr["PORT"].c_str());
	pCT = boost::shared_ptr<ClientThread>(new ClientThread(remote, port));
	tg.create_thread(workF(pCT));
	Sleep(500);
	int total = pCT->client.windowSizeX;
	slideCtrl.SetRange(0, total);

	slideCtrl.SetPos(total/2);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPultClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPultClientDlg::OnPaint()
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
	}
	else
	{

		CPaintDC dc(this);
		int y=0;
		if (pCT){
			boost::mutex::scoped_lock lock(pCT->bmpMutex);
			if (pCT->pBitmap.get()){
				//resize if needed;
				pCT->pBitmap.get()->Draw(dc,0,0);			
			}
		}

		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPultClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPultClientDlg::OnMouseMove(UINT nFlags, CPoint point)
{

	if (pCT.get()!=NULL){
		// TODO: Add your message handler code here and/or call default
		if (point.y < pCT->client.windowSizeY*pCT->client.zoom && point.x < pCT->client.windowSizeX*pCT->client.zoom){
			boost::mutex::scoped_lock lock(pCT->processingMessage);

			ClientEvent ct;
			ct.etype=WM_MOUSEMOVE;
			ct.x=(double)point.x/pCT->client.zoom+ pCT->client.offsetx;
			ct.y=(double)point.y/pCT->client.zoom+ pCT->client.offsety;
			pCT->events.push_back(ct);
		}

	}
	CDialog::OnMouseMove(nFlags, point);
}

void CPultClientDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CPultClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (pCT.get()!=NULL){
		// TODO: Add your message handler code here and/or call default
		if (point.y < pCT->client.windowSizeY*pCT->client.zoom && point.x < pCT->client.windowSizeX*pCT->client.zoom){
			boost::mutex::scoped_lock lock(pCT->processingMessage);

			ClientEvent ct;
			ct.etype=WM_LBUTTONDOWN;
			ct.x=(double)point.x/pCT->client.zoom+ pCT->client.offsetx;
			ct.y=(double)point.y/pCT->client.zoom;+ pCT->client.offsety;
			pCT->events.push_back(ct);
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CPultClientDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (pCT.get()!=NULL){
		if (point.y < pCT->client.windowSizeY*pCT->client.zoom && point.x < pCT->client.windowSizeX*pCT->client.zoom){
			boost::mutex::scoped_lock lock(pCT->processingMessage);

			ClientEvent ct;
			ct.etype=WM_LBUTTONUP;
			ct.x=(double)point.x/pCT->client.zoom + pCT->client.offsetx;
			ct.y=(double)point.y/pCT->client.zoom+ pCT->client.offsety;
			pCT->events.push_back(ct);
		}
	}


	CDialog::OnLButtonUp(nFlags, point);
}

void CPultClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	Invalidate(FALSE);

	CDialog::OnTimer(nIDEvent);
}

void CPultClientDlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int pos = slideCtrl.GetPos();
	if (pCT.get()!=NULL){
		ClientEvent ct;
		ct.etype=WM_MOVE;

		ct.x=pos;
		ct.y=0;

		boost::mutex::scoped_lock lock(pCT->processingMessage);
		pCT->events.push_back(ct);
	}

	*pResult = 0;
}

void CPultClientDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CPultClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (pCT.get()!=NULL){
		slideCtrl.SetWindowPos(0,0,cy-40,cx,40,0);
		btn1.SetWindowPos(0,0,cy-80,30,40,0);
		btn2.SetWindowPos(0,50,cy-80,30,40,0);
		btn3.SetWindowPos(0,100,cy-80,30,40,0);
		btn4.SetWindowPos(0,150,cy-80,30,40,0);
	};
}
void CPultClientDlg::OnBnClickedButton1()
{
	boost::mutex::scoped_lock lock(pCT->processingMessage);
	ClientEvent ct;
	ct.etype=WM_KEYDOWN;
	ct.ch='1';
	pCT->events.push_back(ct);
}

void CPultClientDlg::OnBnClickedButton2()
{
	boost::mutex::scoped_lock lock(pCT->processingMessage);
	ClientEvent ct;
	ct.etype=WM_KEYDOWN;
	ct.ch='2';
	pCT->events.push_back(ct);
}

void CPultClientDlg::OnBnClickedButton3()
{
	boost::mutex::scoped_lock lock(pCT->processingMessage);
	ClientEvent ct;
	ct.etype=WM_KEYDOWN;
	ct.ch='3';
	pCT->events.push_back(ct);
}

void CPultClientDlg::OnBnClickedButton4()
{
	boost::mutex::scoped_lock lock(pCT->processingMessage);
	ClientEvent ct;
	ct.etype=WM_KEYDOWN;
	ct.x='4';
	pCT->events.push_back(ct);
}
