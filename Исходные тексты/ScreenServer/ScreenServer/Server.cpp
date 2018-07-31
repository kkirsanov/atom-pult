#include "StdAfx.h"
#include "Server.h"
#include <windows.h>
#include <iostream>
#include <algorithm>
#include <boost/date_time/date.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
#include <afxconv.h>
#include <boost/progress.hpp>

#include "CxImage/CxImage/xImage.h"

#include "RobSims.h"

using namespace boost::gregorian;

const DWORD TIMEOUT = 700000;

HWND GLOBALWND = 0;

Server::Server(UINT port){
	try{	
		Server::port = port;
		servA = CSockAddr(INADDR_ANY, port);
		sockServer.Create();
		sockServer.Bind(servA);
	}catch(...){
		cout << "Can`t bind port " <<port << endl;
		exit(1);
	};	
};

Server::~Server()
{
};

void Server::Listen(){

	try{		
		sockServer.Listen();		
		CBlockingSocket * pClientSock = new CBlockingSocket();
		BOOL z =sockServer.Accept(*pClientSock , servA);

		ServiceThread *pST = new ServiceThread(pClientSock);
		boost::thread th(*pST);
	}catch(CBlockingSocketException* e){
		cout<< "Port " << port << " blocked!\n";
		exit(1);		
	}

}

ServiceThread::ServiceThread(CBlockingSocket *sok)
{
	int RS_LJoy;
	int RS_RJoy;
	winX=0;
	winY=0;
	winX2 = GetSystemMetrics(SM_CXSCREEN);
	winY2 = GetSystemMetrics(SM_CYSCREEN);

	cout<<"starting  Connection\n";
	pClient = sok;
	pcr = new ConfigReader();
	try{
		xsmall = boost::lexical_cast<int>(pcr->get("XSMALL").c_str());
		ysmall = boost::lexical_cast<int>(pcr->get("YSMALL").c_str());

		xnormal = boost::lexical_cast<int>(pcr->get("XNORMAL").c_str());
		ynormal = boost::lexical_cast<int>(pcr->get("YNORMAL").c_str());
	}catch(...){
		xsmall=320;
		xsmall=240;
		xnormal=800;
		ynormal=600;
		cout << "check resolution settings: XSMALL, YSMALL, XNORMAL, YNORMAL";
	}
}

void ServiceThread::Stop(){
	cout<<"Closig Connection\n";
	try{
		pClient->Close();
	}catch(...){

	};
	//	pClient=NULL;
};

string  ServiceThread::ReciveSokString(DWORD timeout){
	string s;
	s.reserve(50);

	DWORD tickCntPrev = GetTickCount();
	while(1){

		int res=9;
		char ch=0;
		try{
			res = pClient->Receive(&ch,1,timeout/1000);
		}catch(CBlockingSocketException *e){
			break;			
		};

		if (res!=0){
			s.push_back(ch);
		};
		if (!s.empty())
			if(ch=='\n')
				break;
	};
	s.erase(remove(s.begin(), s.end(), '\n'),s.end());
	s.erase(remove(s.begin(), s.end(), '\r'),s.end());

	cout <<"Readstring: "<< s <<endl;
	return s;
}

void ServiceThread::LogOut(string st){
	try{
		int res = pClient->Write(st.c_str(),st.length(),1000);
		st = "Goobye!\r\n";
		res = pClient->Write(st.c_str(),st.length(),1000);
	}catch(...){

	}
	Stop();
}
void ServiceThread::operator ()(void)
{

	cout<<"Running Connection\r\n";
	string st;
	string rnStr = "\r\n";
	string okrnStr = "ok\r\n";

	string setdStr;
	while(1){
		st.clear();
		boost::progress_timer t;

		try{
			SendSokMessage(setdStr + string("Type command:"));
			st = ReciveSokString(TIMEOUT);
			cout <<"*****Command: "<< st <<endl;
		}catch(...){
			st="exit";
		}
		setdStr.clear();

		if (st=="ping"){
			setdStr = "ok\r\n";
			continue;
		}
			
		if (st=="getres"){
			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);
			setdStr = boost::lexical_cast<string>(x) + rnStr + boost::lexical_cast<string>(y) + rnStr;
			continue;
		}
		if (st=="keyboard"){
			if (RSimWND){
				//::SetForegroundWindow(RSimWND);
				INPUT inp = { INPUT_KEYBOARD };
				string key = ReciveSokString(TIMEOUT);
				if (!key.empty()){
					try{
						inp.ki.wVk = 0x30 +boost::lexical_cast<int>(key.c_str());
						SendInput(1, &inp, sizeof(inp));
					}catch(...){

					}
				}
				setdStr= okrnStr;
			}
			continue;
		}
		if (st=="joyr"){
			string xS = ReciveSokString(TIMEOUT);
			string yS = ReciveSokString(TIMEOUT);
			if (RSimWND){
				int xPos = atoi(xS.c_str());
				int yPos = atoi(yS.c_str());
				RS::SetCPObjectOutState(RSimWND, RS_RJoy, 0, xPos);
				RS::SetCPObjectOutState(RSimWND, RS_RJoy, 1, yPos);
				setdStr = okrnStr;
			}else{
				setdStr= "failed\r\n";
			}
			continue;
		}
		if (st=="joyl"){
			string xS = ReciveSokString(TIMEOUT);
			string yS = ReciveSokString(TIMEOUT);
			if (RSimWND){
				int xPos = atoi(xS.c_str());
				int yPos = atoi(yS.c_str());
				RS::SetCPObjectOutState(RSimWND, RS_LJoy, 0, xPos);
				RS::SetCPObjectOutState(RSimWND, RS_LJoy, 1, yPos);
				setdStr = okrnStr;
			}else{
				setdStr= "failed";
			}
			continue;
		}

		if (st=="movemouse"){
			string xS = ReciveSokString(TIMEOUT);
			string yS = ReciveSokString(TIMEOUT);
			int xPos = atoi(xS.c_str());
			int yPos = atoi(yS.c_str());

			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);

			DWORD dx=((double)0xFFFF)/x;
			DWORD dy=((double)0xFFFF)/y;

			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, dx*xPos, dy*yPos, 0, 0);
			setdStr= okrnStr;
			continue;
		}
		if (st=="clickmouse"){
			string xS = ReciveSokString(TIMEOUT);
			string yS = ReciveSokString(TIMEOUT);
			int xPos = atoi(xS.c_str());
			int yPos = atoi(yS.c_str());

			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);

			DWORD dx=((double)0xFFFF)/x;
			DWORD dy=((double)0xFFFF)/y;

			//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, dx*xPos, dy*yPos, 0, 0);
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN, dx*xPos, dy*yPos, 0, 0);
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP, dx*xPos, dy*yPos, 0, 0);

			setdStr= okrnStr;
			continue;
		}
		if (st=="downmouse"){
			string xS = ReciveSokString(TIMEOUT);
			string yS = ReciveSokString(TIMEOUT);
			int xPos = atoi(xS.c_str());
			int yPos = atoi(yS.c_str());

			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);

			DWORD dx=((double)0xFFFF)/x;
			DWORD dy=((double)0xFFFF)/y;

			//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, dx*xPos, dy*yPos, 0, 0);
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN, dx*xPos, dy*yPos, 0, 0);
			setdStr= okrnStr;
			continue;
		}
		if (st=="upmouse"){
			string xS = ReciveSokString(TIMEOUT);
			string yS = ReciveSokString(TIMEOUT);
			int xPos = atoi(xS.c_str());
			int yPos = atoi(yS.c_str());

			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);

			DWORD dx=((double)0xFFFF)/x;
			DWORD dy=((double)0xFFFF)/y;

			//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, dx*xPos, dy*yPos, 0, 0);
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP, dx*xPos, dy*yPos, 0, 0);
			setdStr= okrnStr;
			continue;
		}
		if (st=="setwindow"){
			string sx = ReciveSokString(TIMEOUT);
			string sy = ReciveSokString(TIMEOUT);
			string sx2 = ReciveSokString(TIMEOUT);
			string sy2 = ReciveSokString(TIMEOUT);
			
			winX = atoi(sx.c_str());
			winY = atoi(sy.c_str());
			winX2 = atoi(sx2.c_str());
			winY2 = atoi(sy2.c_str());
			if ((winX>=winX2) || (winY>=winY2)){
				winX=0;
				winY=0;
				winX2 = GetSystemMetrics(SM_CXSCREEN);
				winY2 = GetSystemMetrics(SM_CYSCREEN);
			}
			setdStr= okrnStr;
			continue;
		}
		if (st=="getwindowpng"){
			
			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);
			HDC hDDC = GetDC(0);
			HDC hCDC = CreateCompatibleDC(hDDC);
			HBITMAP hbmp = CreateCompatibleBitmap(hDDC, x, y);
			SelectObject(hCDC, hbmp);
			BitBlt(hCDC,0,0,x,y,hDDC,0,0,SRCCOPY|CAPTUREBLT);
			
			ReleaseDC(0, hDDC);
			DeleteDC(hCDC);

			CxImage *img = new CxImage();
			img->CreateFromHBITMAP(hbmp);
			img->Crop(winX,winY,winX2,winY2);
			DeleteObject(hbmp);

			BYTE *buf = 0;
			long size=0;
			bool res = img->Encode(buf,size, CXIMAGE_FORMAT_BMP);

			char z[20];
			ltoa(size, z,10);

			SendSokMessage(string(z));
			SendSokMessage(string("\r\n"));
			SendSokMessage((char*)buf,size);
			//	SendMessage("\r\n");

			delete img;
			delete[] buf;
			continue;
		}

		if (st=="rsim"){			
			HWND tmpWin = ::FindWindow("TMainForm","RobSim");
			if (tmpWin!=0)
				GLOBALWND = tmpWin;

			if (GLOBALWND==0){
				RSimWND = ::FindWindow("TMainForm","RobSim");
				if (RSimWND!=0){
					GLOBALWND=RSimWND;
				}
			}else{
				RSimWND = GLOBALWND;
			}
			if (GLOBALWND){
				RS_LJoy=0;
				RS_RJoy=0;
				string jName = pcr->get("JOYR");
				string stRes;
				RS_RJoy = RS::GetCPObject(RSimWND, const_cast<char*>(pcr->get("JOYR").c_str()));
				if (RS_RJoy)
					stRes+="JoyR ";
				RS_LJoy = RS::GetCPObject(RSimWND, const_cast<char*>(pcr->get("JOYL").c_str()));
				if (RS_LJoy)
					stRes+="JoyL";
				stRes+=" ok\r\n";
				setdStr = stRes;
			}else{
				setdStr = "failed\r\n";
			}
			continue;
		}
		if (st=="exit"){
			LogOut ("");
			break;
		}
		if (st=="restartserver"){
			LogOut ("");
			exit(1);
			break;
		}
		if (st=="date" || st=="today"){
			date today = day_clock::local_day();					
			setdStr= to_simple_string(today)+ rnStr;
			continue;
		}
		if (st=="name"){
			setdStr = pcr->get("NAME") + rnStr;
			continue;
		}
		if (st=="png"){
			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);
			HDC hDDC = GetDC(0);
			HDC hCDC = CreateCompatibleDC(hDDC);
			HBITMAP hbmp = CreateCompatibleBitmap(hDDC, x, y);
			SelectObject(hCDC, hbmp);
			BitBlt(hCDC,0,0,x,y,hDDC,0,0,SRCCOPY|CAPTUREBLT);
			
			ReleaseDC(0, hDDC);
			DeleteDC(hCDC);

			CxImage *img = new CxImage();
			img->CreateFromHBITMAP(hbmp);
			img->Resample(xnormal, ynormal);
			DeleteObject(hbmp);
			BYTE *buf = 0;
			long size=0;
			bool res = img->Encode(buf,size, CXIMAGE_FORMAT_PNG);
			
			char z[20];
			ltoa(size, z,10);

			SendSokMessage(string(z));
			SendSokMessage(rnStr);
			SendSokMessage((char*)buf,size);
		//	SendMessage("\r\n");

			delete img;
			delete[] buf;
			continue;
		};
		if (st=="bmp"){
			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);
			HDC hDDC = GetDC(0);
			HDC hCDC = CreateCompatibleDC(hDDC);
			HBITMAP hbmp = CreateCompatibleBitmap(hDDC, x, y);
			SelectObject(hCDC, hbmp);
			BitBlt(hCDC,0,0,x,y,hDDC,0,0,SRCCOPY|CAPTUREBLT);
			ReleaseDC(0, hDDC);
			DeleteDC(hCDC);

			CxImage *img = new CxImage();
			img->CreateFromHBITMAP(hbmp);
			img->Resample(xnormal, ynormal);
			DeleteObject(hbmp);

			BYTE *buf = 0;
			long size=0;
			bool res = img->Encode(buf,size, CXIMAGE_FORMAT_BMP);

			char z[20];
			ltoa(size, z,10);

			SendSokMessage(string(z));
			SendSokMessage(string("\r\n"));
			SendSokMessage((char*)buf,size);
		//	SendMessage("\r\n");

			delete img;
			delete[] buf;
			continue;
		};
		
		if (st=="pngsmall"){
			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);
			HDC hDDC = GetDC(0);
			HDC hCDC = CreateCompatibleDC(hDDC);
			
			HBITMAP hbmp = CreateCompatibleBitmap(hDDC, xsmall, ysmall);
			SelectObject(hCDC, hbmp);
			StretchBlt(hCDC, 
				0, 0, 
				xsmall, ysmall, 
				hDDC, 
				0, 0, 
				x, y, 
				SRCCOPY|CAPTUREBLT); 
			ReleaseDC(0, hDDC);
			DeleteDC(hCDC);

			CxImage *img = new CxImage();
			img->CreateFromHBITMAP(hbmp);
			img->Crop(0,0,xsmall,ysmall);

			DeleteObject(hbmp);

			BYTE *buf = 0;
			long size=0;
			bool res = img->Encode(buf,size, CXIMAGE_FORMAT_PNG);

			char z[20];
			ltoa(size, z,10);

			SendSokMessage(string(z));
			SendSokMessage(rnStr);
			SendSokMessage((char*)buf,size);

			delete img;
			delete[] buf;
			continue;
		}
		if (st=="bmpsmall"){
			int x = GetSystemMetrics(SM_CXSCREEN);
			int y = GetSystemMetrics(SM_CYSCREEN);
			HDC hDDC = GetDC(0);
			HDC hCDC = CreateCompatibleDC(hDDC);

			HBITMAP hbmp = CreateCompatibleBitmap(hDDC, xsmall, ysmall);
			SelectObject(hCDC, hbmp);
			StretchBlt(hCDC, 
				0, 0, 
				xsmall, ysmall, 
				hDDC, 
				0, 0, 
				x, y, 
				SRCCOPY|CAPTUREBLT); 
			ReleaseDC(0, hDDC);
			DeleteDC(hCDC);

			CxImage *img = new CxImage();
			img->CreateFromHBITMAP(hbmp);
			img->Crop(0,0,xsmall,ysmall);

			DeleteObject(hbmp);

			BYTE *buf = 0;
			long size=0;
			bool res = img->Encode(buf,size, CXIMAGE_FORMAT_BMP);

			char z[20];
			ltoa(size, z,10);

			SendSokMessage(string(z));
			SendSokMessage(rnStr);
			SendSokMessage((char*)buf,size);

			delete img;
			delete[] buf;
			continue;
		}
		DWORD tickCntNew = GetTickCount();
	}			
	
};