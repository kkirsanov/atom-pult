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

#include "cxImage/CxImage/ximage.h"
#include "ConfigReader.h"

using namespace std;

class Client
{
public:
  string type;
  int pause;
  
  boost::shared_ptr<CxImage> DownloadPNG();
  boost::shared_ptr<CxImage> DownloadPNGsmall();
  boost::shared_ptr<CxImage> DownloadBMP();
  boost::shared_ptr<CxImage> DownloadBMPsmall();
  //image->pBitmap->Decode(data2, size,CXIMAGE_FORMAT_BMP);

  Client(string remote,int port){
    pause=1000;
    WSADATA wsd;
    type = "PNGSMALL";
    VERIFY(WSAStartup(0x0101, &wsd) == 0);
    TRACE("WSAStartup -- min version = %x\n", wsd.wVersion);
    ip = remote;
    _port = port;
  };
  void Connect();   
  ~Client(void){
    Disconnect();
    sok.Close();
  };
private:
  string ip;
  int _port;
  void Disconnect(){
    ReadCommandString();
    SendCommand("exit");
    sok.Close();
  };
  CBlockingSocket sok;
  int SendMessage(string st, int timeout=10);
  int SendCommand(string st, int timeout=10);
  string ReadCommandString();
  string ReadString();
  string ReadData(unsigned size);
  int ReadDataFast(char *buf, unsigned size);
};
