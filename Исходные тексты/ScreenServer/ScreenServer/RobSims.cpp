#include "robsims.h"
#include <stdio.h>
#include <string.h>



namespace RS{

	// установить шаблон принимаемых сообщений. RSHandle - дескриптор окна RobSim. Mask - маска принимаемых сообщений, объедиденных по OR, см. MM_xxx }
	void SetRSMessageMask( HWND RSHandle, int Mask)
	{
		SendMessage(RSHandle,RS_MSGMASK,Mask,0);
	}

	/*************************************************\
				   У П Р А В Л Е Н И Е
	\*************************************************/
	// функция возвращает дескриптор объекта с именем Name на панели управления текущего робота
	int GetCPObject( HWND RSHandle, char *Name)
	{
		SetWindowText(RSHandle,Name);
		return SendMessage(RSHandle,RS_GET_CP_OBJECT,0,0);
	}

	// функция возвращает состояние входа n для объекта с дескриптором Obj на панели упроавления
	int GetCPObjectInState(HWND RSHandle, int Obj, int n)
	{
		return SendMessage(RSHandle,RS_GET_CP_INSTATE,Obj,n << 16);
	}

	// функция возвращает состояние выхода n для объекта с дескриптором Obj на панели упроавления
	int GetCPObjectOutState(HWND RSHandle, int Obj, int n)
	{
		return SendMessage(RSHandle,RS_GET_CP_OUTSTATE,Obj,n << 16);
	}

	// функция устанавливает состояние выхода n для объекта с дескриптором Obj на панели упроавления
	void SetCPObjectOutState(HWND RSHandle, int Obj, int n, int Value)
	{
		SendMessage(RSHandle,RS_SET_CP_OUTSTATE,Obj,(n << 16) | (Value & 0xFFFF));
	}

	// функция посылает данные в RobSim
	void RSPostMessage(HWND RSHandle, int Channel, float Value)
	{
		PostMessage(RSHandle,RS_POSTMESSAGE, Channel, (LPARAM)Value);
	}

}