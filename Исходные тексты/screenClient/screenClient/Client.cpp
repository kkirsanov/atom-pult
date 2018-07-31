#include "StdAfx.h"
#include "Client.h"


boost::shared_ptr<CxImage> Client::DownloadPNG(){
	boost::shared_ptr<CxImage>img = boost::shared_ptr<CxImage>(new CxImage());
	string st = ReadCommandString();
	SendCommand("png");
	string sizeS = ReadString();
	long size =0;
	ATLTRACE(sizeS.c_str());
	ATLTRACE("\n");
	size = atol(sizeS.c_str());
	char* data= new char[size];;
	ReadDataFast(data, size);

	img->Decode((BYTE*)data, size,CXIMAGE_FORMAT_PNG);
	delete[] data;
	return img;
}
boost::shared_ptr<CxImage> Client::DownloadBMP(){
	boost::shared_ptr<CxImage> img = boost::shared_ptr<CxImage>(new CxImage());
	string st = ReadCommandString();
	SendCommand("bmp");
	string sizeS = ReadString();

	long size =0;
	size = atol(sizeS.c_str());
	char* data= new char[size];;
	ReadDataFast(data, size);
	img->Decode((BYTE*)data, size,CXIMAGE_FORMAT_BMP);
	delete[] data;
	return img;
}
boost::shared_ptr<CxImage> Client::DownloadPNGsmall(){
	boost::shared_ptr<CxImage>img = boost::shared_ptr<CxImage>(new CxImage());
	string st = ReadCommandString();
	SendCommand("pngsmall");
	string sizeS = ReadString();
	long size =0;
	ATLTRACE(sizeS.c_str());
	ATLTRACE("\n");
	size = atol(sizeS.c_str());
	char* data= new char[size];;
	ReadDataFast(data, size);

	img->Decode((BYTE*)data, size,CXIMAGE_FORMAT_PNG);
	delete[] data;
	return img;
}
boost::shared_ptr<CxImage> Client::DownloadBMPsmall(){
	boost::shared_ptr<CxImage>img = boost::shared_ptr<CxImage>(new CxImage());
	string st = ReadCommandString();
	SendCommand("bmpsmall");
	string sizeS = ReadString();

	long size =0;
	size = atol(sizeS.c_str());

	char* data = new char[size];
	ReadDataFast(data, size);
	img->Decode((BYTE*)data, size,CXIMAGE_FORMAT_BMP);
	delete [] data;
	return img;
}

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

void Client::Connect(){
	//CSockAddr addr(ip, _port);
	CSockAddr addr(ip.c_str(), _port);
	sok.Create();
	sok.Connect(addr);
}
string Client::ReadCommandString(){
	const int timeout = 100;
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
			 if(ch==':')
				 break;
		 //check timer
	 };
	// s.erase(remove(s.begin(), s.end(), '\n'),s.end());
	// s.erase(remove(s.begin(), s.end(), '\r'),s.end());
	// s.erase(remove(s.begin(), s.end(), ':'),s.end());
	 //return s;
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
		//if (!(s.size()==2 && s[0]=='\r' && s[1]=='\n')) //skip first \r\n
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
}
