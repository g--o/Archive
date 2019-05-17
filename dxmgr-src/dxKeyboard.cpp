#include "dxInput.h"

dxKeyboard::dxKeyboard(LPDIRECTINPUT8 din,HWND wndHandle)
	{
	   // create the keyboard device
	   din->CreateDevice(GUID_SysKeyboard,    // the default keyboard ID being used
                       &dinkeyboard,    // the pointer to the device interface
                       NULL);    // COM stuff, so we'll set it to NULL
	       // set the data format to keyboard format
     dinkeyboard->SetDataFormat(&c_dfDIKeyboard);

     // set the control you will have over the keyboard
     dinkeyboard->SetCooperativeLevel(wndHandle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	}

	// this is the function that gets the latest input data
 void dxKeyboard::detect_input(void)
 {
     // get access if we don't have it already
     dinkeyboard->Acquire();

     // get the input data
     dinkeyboard->GetDeviceState(256, (LPVOID)keystate);
 }

 bool dxKeyboard::isDown(DWORD key)
 {
	 return (keystate[key] & 0x80);
 }


 // this is the function that closes DirectInput
 dxKeyboard::~dxKeyboard()
 {
     dinkeyboard->Unacquire();    // make sure the keyboard is unacquired
 }