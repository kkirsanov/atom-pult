#pragma once
#include "stdafx.h"
#include <string>
#include <algorithm>
#include "Blocksock.h"
#include <vector>

#include <boost/date_time/date.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "CxImage/CxImage/xImage.h"
#include <boost/progress.hpp>

#include "ConfigReader.h"

#include "Joystic.h"

using namespace std;

class Client
{
public:
	string type;
	bool connected;
	int pause;
	int pausePult;
	bool flag;
	double zoom;
	int offsety;
	int offsetx;

	int windowSizeY;
	int windowSizeX;

	int serverX,serverY;
	int winX,winY;
	int winX2,winY2;
	void SendClick(XY ji);
	void SendLMU(XY ji);
	void SendLMD(XY ji);
	void SendMove(XY ji);
	void InitServerWindow(int startX=0);

	void rsim(){
		ReadCommandString();
		SendCommand("rsim");				
	}
	void ping(){
		boost::progress_timer t;
		string st = ReadCommandString();
		int i = rand();
		//SendCommand(boost::lexical_cast<string>(i));
		SendCommand("movemouse");
		SendCommand("10");
		SendCommand("20");
		//SendCommand("movemouse\r\n10\r\n22");
		//SendCommand("movemouse\r\n10\r\n22");
		//SendCommand("");
		//string st = ReadString();
		//st = ReadString();

	}
	boost::shared_ptr<CxImage> Client::DownloadWindow();

	Client(string remote,int port){
		connected=false;
		pause=50;
		zoom = 1;
		offsetx=2;
		offsety=10;
		pausePult=400;
		WSADATA wsd;
		VERIFY(WSAStartup(0x0101, &wsd) == 0);
		TRACE("WSAStartup -- min version = %x\n", wsd.wVersion);
		ip = remote;
		_port = port;
		flag=false;
		int windowSizeY=290;
		int windowSizeX=575;
	};
	void Connect();		
	~Client(void){
		connected=false;
		Disconnect();
		sok.Close();
	};
	void SendLJoy(XY);
	void SendRJoy(XY);
	void SendInput(char ch);
//private:
	string ip;
	int _port;
	
	void Disconnect(){
		try{
			if (connected){
				ReadCommandString();
				SendCommand("exit");
				connected=false;
			}
		}catch(...){
		};
	};
	CBlockingSocket sok;
	int SendMessage(string st, int timeout=10);
	int SendCommand(string st, int timeout=10);
	string ReadCommandString();
	string ReadString();
	string ReadData(unsigned size);
	int ReadDataFast(char *buf, unsigned size);
};
