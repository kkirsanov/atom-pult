#pragma once

#include "Client.h"
#include <boost/thread.hpp>

enum ImageType {
	BMP,
	PNG,
	BMPSMALL,
	PNGSMALL
};


class ClientThread{
public:
	int id;
	int port;
	string remote;
	Client client;

	ClientThread(string remote, int port):
		client(remote, port)
	{
		itype=BMPSMALL;
		ClientThread::remote=remote;
		ClientThread::port=port;
		client.Connect();
	};
	void work();

	ImageType itype;;
	boost::mutex mutex;
	boost::shared_ptr<CxImage> pBitmap;
};