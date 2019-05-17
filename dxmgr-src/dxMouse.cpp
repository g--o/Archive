
	#include "dxInput.h"

	dxMouse::dxMouse(LPDIRECTINPUT8 din,HWND wndHandle)
	{
			din->CreateDevice(GUID_SysMouse,&dinmouse,NULL);
			dinmouse->SetDataFormat(&c_dfDIMouse);
			dinmouse->SetCooperativeLevel(wndHandle, DISCL_EXCLUSIVE);
			dinmouse->Acquire();
	}

	dxMouse::~dxMouse()
	{
		// this is the function that closes DirectInput
		dinmouse->Unacquire();    // make sure the mouse in unacquired
	}

	// this is the function that gets the latest input data
	void dxMouse::detect_input(void)
	{
		// get access if we don't have it already
		dinmouse->Acquire();
		// get the input data
		dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mousestate);
	}

