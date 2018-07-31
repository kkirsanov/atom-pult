/*****************************************\
           ���������� ��� �������
          RobSim ��-��� ����������
           �� Microsoft Visual C++
        (c) 2004 ���������� �.�.
\*****************************************/




#ifndef __ROBSIMS__
#define __ROBSIMS__
#include <windows.h>

namespace RS{


/* ������ ���������, ������� �������� RobSim � ���������� ������������ */

const int RS_READY = 0x3000;     // RobSim �����. wParam - ���������� ���� RobSim; lParam=0;
const int RS_DONE  = 0x3001;     // RobSim ��������. wParam - ���������� ���� RobSim; lParam=0;
const int RS_RENDER= 0x3002;     // RobSim ���������� ���� ����������������. wParam - ���������� ���� RobSim.
                              //    lParam - ���������� ���� ��� ����������� 3D.
const int RS_GETMESSAGE =0x3010; // ������ �� Robsim:
                              //     wParam        - ����� ������
                              //     (float)Single - ������

/* ������ ���������, ������� �������� ���������� ������������ � RobSim */
//        WM_CLOSE                   // ������� RobSim
const int   RS_MSGMASK   =  0x3003;  // ��������/��������� ����� ���������. wParam = ����� (��. MM_xxx)
const int   RS_GET_CP_OBJECT   =0x3004;  // �������� ���������� ������������� ������� �� ������ ����������. ��� �������� �������� �������� ����� �������� ���� RobSim
const int   RS_GET_CP_INSTATE  =0x3005;  // ��������� ��������� ����� ��� ������� �� ������ ����������. wParam - ���������� �������. HIWORD(lParam) - ����� ����� (�� 0).
const int   RS_GET_CP_OUTSTATE =0x3006;  // ��������� ��������� ������ ��� ������� �� ������ ����������. wParam - ���������� �������. HIWORD(lParam) - ����� ����� (�� 0).
const int   RS_SET_CP_OUTSTATE =0x3007;  // ���������� ��������� ������ ��� ������� �� ������ ����������. wParam - ���������� �������. HIWORD(lParam) - ����� ����� (�� 0). LOWWORD(lParam) - ����� ���������.
const int   RS_POSTMESSAGE     =0x3011;  // ������� ������ � Robsim:
                                     //     wParam         - ����� ������
                                     //     (float)lParam  - ������

/* ����� ����������� ��������� ��� ������� SetRSMessageMask */
	const int   MM_RENDER   =0x01;          // ��������� ����� ��������� RS_RENDER
	const int   MM_MESSAGES =0x02;          // ��������� ����� ��������� RS_GETMESSAGE




	// ���������� ������ ����������� ���������. RSHandle - ���������� ���� RobSim. Mask - ����� ����������� ���������, ������������ �� OR, ��. MM_xxx }
	void SetRSMessageMask( HWND RSHandle, int Mask);


	/*************************************************\
				   � � � � � � � � � �
	\*************************************************/
	// ������� ���������� ���������� ������� � ������ Name �� ������ ���������� �������� ������
	int GetCPObject( HWND RSHandle, char *Name);

	// ������� ���������� ��������� ����� n ��� ������� � ������������ Obj �� ������ �����������
	int GetCPObjectInState(HWND RSHandle, int Obj, int n);

	// ������� ���������� ��������� ������ n ��� ������� � ������������ Obj �� ������ �����������
	int GetCPObjectOutState(HWND RSHandle, int Obj, int n);

	// ������� ������������� ��������� ������ n ��� ������� � ������������ Obj �� ������ �����������
	void SetCPObjectOutState(HWND RSHandle, int Obj, int n, int Value);

	// ������� �������� ������ � RobSim
	void RSPostMessage(HWND RSHandle, int Channel, float Value);

}
#endif // __ROBSIMS__