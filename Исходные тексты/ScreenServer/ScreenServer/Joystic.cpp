#include "StdAfx.h"
#include "Joystic.h"

Joystics::Joystics(void)
{
}

Joystics::~Joystics(void)
{
}

int Joystics::GetMaxCount(){
	return joyGetNumDevs();
}

vector<Joystic> Joystics::GetConnected(){
	vector<Joystic> joys;
	int count = GetMaxCount();
	for(int i = 0; i<count; i++){
		//JOYERR_NOERROR if the specified device is attached. Otherwise JOYERR_UNPLUGGED. 
		JOYINFO info;
		MMRESULT result = joyGetPos(i,  &info );
		if (result == JOYERR_NOERROR)
			joys.push_back(Joystic(i));
	}
	return joys;
}