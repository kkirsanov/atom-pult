#include "StdAfx.h"
#include "ClientThrad.h"

void ClientThread::work(){
	boost::shared_ptr<CxImage> pTmpImage;
	
	Sleep(client.pause);

	if (itype==BMP){
		pTmpImage= client.DownloadBMP();
	}
	if (itype==PNG){		
		pTmpImage= client.DownloadPNG();
	}
	if (itype==BMPSMALL){
		pTmpImage= client.DownloadBMPsmall();
	}
	if (itype==PNGSMALL){
		pTmpImage= client.DownloadPNGsmall();
	}
	boost::mutex::scoped_lock lock(mutex);
	pBitmap = pTmpImage;
};