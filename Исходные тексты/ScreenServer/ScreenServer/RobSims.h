/*****************************************\
           Библиотека для запуска
          RobSim из-под приложений
           на Microsoft Visual C++
        (c) 2004 Евстигнеев Д.В.
\*****************************************/




#ifndef __ROBSIMS__
#define __ROBSIMS__
#include <windows.h>

namespace RS{


/* номера сообщений, которые посылает RobSim в приложение пользователя */

const int RS_READY = 0x3000;     // RobSim готов. wParam - дескриптор окна RobSim; lParam=0;
const int RS_DONE  = 0x3001;     // RobSim выгружен. wParam - дескриптор окна RobSim; lParam=0;
const int RS_RENDER= 0x3002;     // RobSim подготовил кадр видеоизображения. wParam - дескриптор окна RobSim.
                              //    lParam - дескриптор окна для отображения 3D.
const int RS_GETMESSAGE =0x3010; // данные от Robsim:
                              //     wParam        - номер канала
                              //     (float)Single - данные

/* номера сообщений, которые посылает приложение пользователя в RobSim */
//        WM_CLOSE                   // закрыть RobSim
const int   RS_MSGMASK   =  0x3003;  // включить/отключить маску сообщений. wParam = маска (см. MM_xxx)
const int   RS_GET_CP_OBJECT   =0x3004;  // получить дескриптор интерфейсного объекта на панели управления. Имя искомого элемента задается через название окна RobSim
const int   RS_GET_CP_INSTATE  =0x3005;  // прочитать состояние входа для объекта на панели управления. wParam - дескриптор объекта. HIWORD(lParam) - номер входа (от 0).
const int   RS_GET_CP_OUTSTATE =0x3006;  // прочитать состояние выхода для объекта на панели управления. wParam - дескриптор объекта. HIWORD(lParam) - номер входа (от 0).
const int   RS_SET_CP_OUTSTATE =0x3007;  // установить состояние выхода для объекта на панели управления. wParam - дескриптор объекта. HIWORD(lParam) - номер входа (от 0). LOWWORD(lParam) - новое состояние.
const int   RS_POSTMESSAGE     =0x3011;  // Послать данные в Robsim:
                                     //     wParam         - номер канала
                                     //     (float)lParam  - данные

/* маски принимаемых сообщений для функции SetRSMessageMask */
	const int   MM_RENDER   =0x01;          // разрешить прием сообщений RS_RENDER
	const int   MM_MESSAGES =0x02;          // разрешить прием сообщений RS_GETMESSAGE




	// установить шаблон принимаемых сообщений. RSHandle - дескриптор окна RobSim. Mask - маска принимаемых сообщений, объедиденных по OR, см. MM_xxx }
	void SetRSMessageMask( HWND RSHandle, int Mask);


	/*************************************************\
				   У П Р А В Л Е Н И Е
	\*************************************************/
	// функция возвращает дескриптор объекта с именем Name на панели управления текущего робота
	int GetCPObject( HWND RSHandle, char *Name);

	// функция возвращает состояние входа n для объекта с дескриптором Obj на панели упроавления
	int GetCPObjectInState(HWND RSHandle, int Obj, int n);

	// функция возвращает состояние выхода n для объекта с дескриптором Obj на панели упроавления
	int GetCPObjectOutState(HWND RSHandle, int Obj, int n);

	// функция устанавливает состояние выхода n для объекта с дескриптором Obj на панели упроавления
	void SetCPObjectOutState(HWND RSHandle, int Obj, int n, int Value);

	// функция посылает данные в RobSim
	void RSPostMessage(HWND RSHandle, int Channel, float Value);

}
#endif // __ROBSIMS__