#pragma once
#include <vector>
#include <MMSystem.h>

using namespace std;

class Joystic{
public:
	Joystic(int num){
		n = num;
	};
	JOYINFO Update(){
		MMRESULT result = joyGetPos(n,  &ji );
		if (result != JOYERR_NOERROR)
			throw "Error in Joystic";
		return ji;
	};
	JOYINFO GetData(){
		return ji;
	};
	JOYINFO GetData255(){
		JOYINFO ji2 = ji;
		ji2.wXpos = ((long)ji.wXpos - 65536/2)/128; //-256..256
		ji2.wYpos= ((long)ji.wYpos - 65536/2)/128;
		return ji2;
	};
private:
	JOYINFO ji;
	int n;
};
class Joystics
{
public:
	Joystics(void);
	static int GetMaxCount();
	static vector<Joystic> GetConnected();
	~Joystics(void);
};
