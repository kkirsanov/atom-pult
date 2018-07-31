// ScreenServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ScreenServer.h"
#include "RobSims.h"

#include "Joystic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

#include "Server.h"
#include "ConfigReader.h"
#include <boost/lexical_cast.hpp>

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}else{
		/*
		HWND RSimWND = ::FindWindow("TMainForm","RobSim");
		if (RSimWND){
			Joystics joy;
			vector<Joystic> joys = joy.GetConnected();
			int RS_RJoy = RS::GetCPObject(RSimWND, "Джойстик27");
			while(1){
				if (!joys.empty()){
					joys[0].Update();
					JOYINFO data = joys[0].GetData255();
					RS::SetCPObjectOutState(RSimWND, RS_RJoy, 0, data.wXpos);
					RS::SetCPObjectOutState(RSimWND, RS_RJoy, 1, data.wYpos);
					Sleep(100);
				}
			}
		}
		return 0;
		*/

		WSADATA wsd;
		VERIFY(WSAStartup(0x0101, &wsd) == 0);
		TRACE("WSAStartup -- min version = %x\n", wsd.wVersion);

		ConfigReader cr;
		if (cr["PORT"].size()==0){
			cout << "PORT not assigned in cinfig.ini!";
			exit(1);
		}
		Server server(boost::lexical_cast<int>(cr["PORT"]));			

		cout << "Using port " << cr["PORT"] << endl;

		while(1){
			server.Listen();
		}
	}

	return nRetCode;
}
