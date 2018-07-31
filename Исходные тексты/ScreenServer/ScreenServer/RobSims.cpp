#include "robsims.h"
#include <stdio.h>
#include <string.h>



namespace RS{

	// ���������� ������ ����������� ���������. RSHandle - ���������� ���� RobSim. Mask - ����� ����������� ���������, ������������ �� OR, ��. MM_xxx }
	void SetRSMessageMask( HWND RSHandle, int Mask)
	{
		SendMessage(RSHandle,RS_MSGMASK,Mask,0);
	}

	/*************************************************\
				   � � � � � � � � � �
	\*************************************************/
	// ������� ���������� ���������� ������� � ������ Name �� ������ ���������� �������� ������
	int GetCPObject( HWND RSHandle, char *Name)
	{
		SetWindowText(RSHandle,Name);
		return SendMessage(RSHandle,RS_GET_CP_OBJECT,0,0);
	}

	// ������� ���������� ��������� ����� n ��� ������� � ������������ Obj �� ������ �����������
	int GetCPObjectInState(HWND RSHandle, int Obj, int n)
	{
		return SendMessage(RSHandle,RS_GET_CP_INSTATE,Obj,n << 16);
	}

	// ������� ���������� ��������� ������ n ��� ������� � ������������ Obj �� ������ �����������
	int GetCPObjectOutState(HWND RSHandle, int Obj, int n)
	{
		return SendMessage(RSHandle,RS_GET_CP_OUTSTATE,Obj,n << 16);
	}

	// ������� ������������� ��������� ������ n ��� ������� � ������������ Obj �� ������ �����������
	void SetCPObjectOutState(HWND RSHandle, int Obj, int n, int Value)
	{
		SendMessage(RSHandle,RS_SET_CP_OUTSTATE,Obj,(n << 16) | (Value & 0xFFFF));
	}

	// ������� �������� ������ � RobSim
	void RSPostMessage(HWND RSHandle, int Channel, float Value)
	{
		PostMessage(RSHandle,RS_POSTMESSAGE, Channel, (LPARAM)Value);
	}

}