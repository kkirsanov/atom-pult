#include "StdAfx.h"
#include "ClientThrad.h"

/*
struct downloadWorker{
	downloadWorker(ClientThread* pTh){
		pClietnThread = pTh;
	}

	ClientThread* pClietnThread;
	void operator()(){
		Sleep(2000);
		while(1){
			Sleep(pClietnThread->clientBMP.pausePult);

			boost::shared_ptr<CxImage> pTmpImage;
			pClietnThread->picTimer.restart();			
			boost::mutex::scoped_lock lock1(pClietnThread->bmpDownloadMutex);
			pTmpImage= pClietnThread->clientBMP.DownloadWindow();
			lock1.unlock();
			boost::mutex::scoped_lock lock2(pClietnThread->bmpMutex);
			pClietnThread->pBitmap = pTmpImage;
		}
	}
};

*/
void ClientThread::work(){
	//boost::mutex::scoped_lock lock(processingMessage);
	//events.clear();
	//lock.unlock();
//	while(0){
//		client.ping();
//	}

	while(1){
		if (!client.connected){
			AfxMessageBox("Связь потерянна.");
			break;	
		}
		if (picTimer.elapsed()>((double)client.pausePult)/1000){//мсек
			picTimer.restart();
			boost::shared_ptr<CxImage> pTmpImage;
			pTmpImage= client.DownloadWindow();
			boost::mutex::scoped_lock lock(bmpMutex);
			pBitmap = pTmpImage;
			client.rsim();
		}else{
			Sleep(client.pause);
		}

		if (joystics.size()>0){
			joystics[0].Update();
			XY ji = joystics[0].GetData255();
			client.SendRJoy(ji);
		}
		if (joystics.size()>1){
			joystics[1].Update();
			XY ji = joystics[1].GetData255();
			client.SendLJoy(ji);
		}
		//
		boost::mutex::scoped_lock lock(processingMessage);
		while(!events.empty()){
			ClientEvent newEvent = 	events.front();
			events.pop_front();

			XY xy;
			boost::shared_ptr<CxImage> pTmpImage;
			//boost::mutex::scoped_lock lock(bmpMutex);

			switch(newEvent.etype){
				case WM_MOUSEMOVE:
					xy.x= newEvent.x + client.winX;
					xy.y= newEvent.y + client.winY;
					client.SendMove(xy);
				break;
				
				case WM_LBUTTONUP:
					xy.x= newEvent.x + client.winX;
					xy.y= newEvent.y + client.winY;
					client.SendLMU(xy);	
					//pTmpImage= clientBMP.DownloadWindow();
					//pBitmap = pTmpImage;
				break;
				case WM_LBUTTONDOWN:
					xy.x= newEvent.x + client.winX;
					xy.y= newEvent.y + client.winY;
					client.SendLMD(xy);
				break;
				case WM_KEYDOWN:
					client.SendInput(newEvent.ch);
				break;
			}
			if (newEvent.etype ==WM_MOVE){
				boost::mutex::scoped_lock lock1(bmpMutex);
				client.InitServerWindow(newEvent.x);
				lock1.unlock();
			}
		}
	}
};