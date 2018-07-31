#pragma once

#include <boost/thread/thread.hpp>
#include <string>
#include "Blocksock.h"
#include "ConfigReader.h"

using namespace std;

class Server
{
public:
	Server(UINT port);
	void Listen();
	BOOL working;
	~Server();
private:
	boost::thread_group thG;
	UINT port;
	CSockAddr servA;
	CBlockingSocket  sockServer;
};

class ServiceThread
{
public:
	HWND RSimWND;
	int RS_LJoy;
	int RS_RJoy;
	int winX,winY,winX2,winY2;
	ServiceThread(CBlockingSocket *pClient);
	void operator()(void);
	void Stop();
	string ReciveSokString(DWORD timeout);
	int xsmall, ysmall;
	int xnormal, ynormal;

private:
	int SendSokMessage(string &st, int timeout=2000){
		return pClient->Send(&st[0],st.length(),timeout);
		//return pClient->Write(st.c_str(),st.length(),timeout);
	}
	int SendSokMessage(char *buf,long size, int timeout=2000){

		return pClient->Send(buf,size,timeout);
	}
	ConfigReader *pcr;
	void LogOut(string st);
	CBlockingSocket *pClient;
};
