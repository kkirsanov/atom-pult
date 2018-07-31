#include "StdAfx.h"
#include "Client.h"


string ReadUntill(string::iterator &from, string::iterator end, char ch){
	string tmp;
	while(from!=end){
		if (*from==ch){
			from++;
			break;
		};
		tmp.push_back(*from);
		from++;
	}
	return tmp;
}

boost::shared_ptr<CxImage> Client::DownloadWindow(){
	boost::shared_ptr<CxImage>img = boost::shared_ptr<CxImage>(new CxImage());
	string st = ReadCommandString();
	SendCommand("getwindowpng");
	string sizeS = ReadString();

	long size =0;
	size = atol(sizeS.c_str());

	char* data = new char[size];
	ReadDataFast(data, size);
	img->Decode((BYTE*)data, size,CXIMAGE_FORMAT_BMP);
	delete [] data;
	int imx = (double)img->GetWidth() * zoom;
	int imy = (double)img->GetHeight()* zoom;
	img->Resample(imx, imy, CxImage::IM_LANCZOS);
	return img;
}

void Client::InitServerWindow(int startX){
	ReadCommandString();
	SendCommand("getres");
	string xS = ReadString();
	string yS = ReadString();
	serverX = atoi(xS.c_str());
	serverY = atoi(yS.c_str());


	int xCenter = serverX / 2;
	winY = serverY-windowSizeY;// 290 - размер пульта
	winY2 = serverY-5;
	winX = xCenter-windowSizeX/2; // 575- размер типового пульта по ширине
	winX2 = xCenter+windowSizeX/2;

	if (startX>0){
		winX =startX;
		winX2 =winX+windowSizeX;
	}
	ReadCommandString();
	SendCommand("setwindow");
	SendCommand(boost::lexical_cast<string>(winX));
	SendCommand(boost::lexical_cast<string>(winY));
	SendCommand(boost::lexical_cast<string>(winX2));
	SendCommand(boost::lexical_cast<string>(winY2));

}
void Client::Connect(){
	//CSockAddr addr(ip, _port);
	CSockAddr addr(ip.c_str(), _port);
	try{
		sok.Create();
		sok.Connect(addr);
	}catch(...){
		AfxMessageBox("Не удалось подключится к серверу");
		throw "err";
		return;
	}
	ReadCommandString();
	SendCommand("rsim");
	string st = ReadString();
	if (st.find("failed")!=string::npos){
		AfxMessageBox("Сервер не обнуружил запущенный RSim");
		exit(1);
		Disconnect();
	}else{
		connected=true;
	}
}
string Client::ReadCommandString(){
	const int timeout = 20000;
	 string newS;
	 string s;
	 s.reserve(50);

	 while(1){
		 int res=9;
		 char ch=0;
		 res = sok.Receive(&ch,1,timeout);
		 if (res!=0){
		//	 s.push_back(ch);
		 };
		 //if (!s.empty())
			 if(ch==':')
				 break;
		 //check timer
	 };
	 //KIRs.erase(remove(s.begin(), s.end(), '\n'),s.end());
	 //KIRs.erase(remove(s.begin(), s.end(), '\r'),s.end());
	 //KIRs.erase(remove(s.begin(), s.end(), ':'),s.end());
	 //KIRreturn s;
	 return s;
}

int Client::ReadDataFast(char *buf, unsigned size){
	unsigned recived=0;	
	while(recived<size) {
		int count= sok.Receive(&(buf[recived]), size-recived, 1);
		if (count==0) break;
		recived+=count;
	};
	return 1;
}
string Client::ReadData(unsigned size){
	string st;
	st.reserve(size);
	int i=1;
	char ch;
	while(size && i){
		size--;
		i = sok.Receive(&ch,1,0);
		st.push_back(ch);
	}
	//read for ending \r\n
	char z[2];sok.Receive(z,2,0);

	return st;
}
string Client::ReadString(){
	const int timeout = 500;
	string newS;
	string s;
	s.reserve(50);

	DWORD tickCntPrev = GetTickCount();
	while(1){
		int res=9;
		char ch=0;
		try{
			res = sok.Receive(&ch,1,timeout);
		}catch(CBlockingSocketException *e){};

		if (res!=0){
			s.push_back(ch);
		};
		
		if (s.size()>0)
			if(ch=='\n' )
				break;
		//check timer
	};
	s.erase(remove(s.begin(), s.end(), '\n'),s.end());
	s.erase(remove(s.begin(), s.end(), '\r'),s.end());
	return s;
}

int Client::SendMessage(string st, int timeout){
	return sok.Write(st.c_str(),st.length(),timeout);
}
int Client::SendCommand(string st, int timeout){
	st+="\r\n";
	return sok.Write(st.c_str(),st.length(),timeout);
};

void Client::SendMove(XY ji){
	string st = ReadCommandString();
	SendCommand("movemouse");
	
	SendCommand(boost::lexical_cast<string>((int)((double)ji.x)));
	SendCommand(boost::lexical_cast<string>((int)((double)ji.y)));
	ReadString();
};
void Client::SendInput(char ch){
	string st = ReadCommandString();
	SendCommand("keyboard");
	st.clear();
	st.push_back(ch);
	SendCommand(st);
	ReadString();
};
void Client::SendLMD(XY ji){
	string st = ReadCommandString();
	SendCommand("downmouse");
	SendCommand(boost::lexical_cast<string>((int)((double)ji.x)));
	SendCommand(boost::lexical_cast<string>((int)((double)ji.y)));
	ReadString();
};

void Client::SendLMU(XY ji){
	string st = ReadCommandString();
	SendCommand("upmouse");
	SendCommand(boost::lexical_cast<string>((int)((double)ji.x)));
	SendCommand(boost::lexical_cast<string>((int)((double)ji.y)));
	ReadString();
};
void Client::SendClick(XY ji){
	string st = ReadCommandString();
	SendCommand("clickmouse");
	SendCommand(boost::lexical_cast<string>((int)((double)ji.x)));
	SendCommand(boost::lexical_cast<string>((int)((double)ji.y)));
	ReadString();
};

void Client::SendLJoy(XY ji){
	string st = ReadCommandString();
	SendCommand("joyl");
	SendCommand(boost::lexical_cast<string>(ji.x));
	SendCommand(boost::lexical_cast<string>(ji.y));
	ReadString();
};
void Client::SendRJoy(XY ji){
	string st = ReadCommandString();
	SendCommand("joyr");
	SendCommand(boost::lexical_cast<string>(ji.x));
	SendCommand(boost::lexical_cast<string>(ji.y));
	ReadString();
};