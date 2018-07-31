#pragma once
#include <vector>
#include <MMSystem.h>

using namespace std;

struct XY{
	int x;
	int y;
};
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
	XY GetData255(){
		XY z;	
		z.x = ((int)ji.wXpos - 65536/2)/128; //-256..256
		z.y= ((int)ji.wYpos - 65536/2)/128;
		return z;
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
