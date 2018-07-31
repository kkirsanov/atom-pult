#pragma once

#include "Client.h"
#include <boost/thread.hpp>
#include "Joystic.h"
#include <deque>
#include <boost/shared_ptr.hpp>
#include <boost/timer.hpp>
#include <algorithm>


enum ImageType {
	BMP,
	PNG,
	BMPSMALL,
	PNGSMALL
};

class ClientEvent{
public:
	int x;
	int y;
	int key;
	char ch;
	int etype;
	ClientEvent(){
		x=y=key=etype=0;
	};
};

class ClientThread{
public:
	deque<ClientEvent> events;
	boost::shared_ptr<CxImage> pBitmap;
	int id;
	int port;
	int oldX, oldY;
	string remote;
	Client client;
	vector<Joystic> joystics;
	
	boost::timer picTimer;	
	boost::mutex processingMessage;
	boost::mutex bmpMutex;
	ClientThread(string remote, int port):
		client(remote, port)
	{
		picTimer.restart();
		ClientThread::remote=remote;
		ClientThread::port=port;
		try{
			client.Connect();
		}catch(...){
			exit(1);
		}
		ConfigReader cr;
		try{
			client.pause = boost::lexical_cast<int>(cr["COMMANDTIME"]);
		}catch(...){
			AfxMessageBox("COMMANDTIME не заданна в config.ini");
			client.pause = 30;
		};
		try{
			client.pausePult = boost::lexical_cast<int>(cr["PULTTIME"]);
		}catch(...){
			AfxMessageBox("PULTTIME не заданна в config.ini");
			client.pausePult = 10000;
		};

		try{
			client.windowSizeX = boost::lexical_cast<int>(cr["WINDOWSIZEX"]);
		}catch(...){
			AfxMessageBox("WINDOWSIZEX не заданна в config.ini");
		};

		try{
			client.windowSizeY = boost::lexical_cast<int>(cr["WINDOWSIZEY"]);
		}catch(...){
			AfxMessageBox("WINDOWSIZEY не заданна в config.ini");
		};
		
		try{
			client.zoom = boost::lexical_cast<double>(cr["ZOOM"]);
		}catch(...){
			AfxMessageBox("ZOOM не заданна в config.ini (для значения по умолчанию напишите ZOOM=1.0)");
		};
		try{
			client.offsetx = boost::lexical_cast<int>(cr["OFFSETX"]);
			client.offsety = boost::lexical_cast<int>(cr["OFFSETY"]);
		}catch(...){
		};

		client.InitServerWindow();
		Joystics joys;
		try{
			joystics = joys.GetConnected();
			if (joystics.size()==2){
				ConfigReader cr;
				if (cr["JOYSWAP"]=="yes"){
					reverse(joystics.begin(), joystics.end());
				}
			}
			if (joystics.size()==1){
				AfxMessageBox("Обнаружен лишь один джойстик.");
			}
			if (joystics.empty()){
				AfxMessageBox("Джойстики не обнаруженны.");
			}
		}catch(...){
			;
		}
		oldX=0;
		oldY=0;
	};
	void work();
	boost::mutex mutex;
};