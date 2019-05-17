#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

LPDIRECTINPUT8 din;    // the pointer to our DirectInput interface

void initDirectInput()
{
		    DirectInput8Create(GetModuleHandle(NULL),    // the handle to the application
                       DIRECTINPUT_VERSION,    // the compatible version
                       IID_IDirectInput8,    // the DirectInput interface version
                       (void**)&din,    // the pointer to the interface
                       NULL);    // COM stuff, so we'll set it to NULL
}

void cleanDirectInput()
{
	din->Release();    // close DirectInput before exiting
}


class dxKeyboard
{
public:
	LPDIRECTINPUTDEVICE8 dinkeyboard;    // the pointer to the keyboard device
	BYTE keystate[256];    // the storage for the key-information

	dxKeyboard(HWND wndHandle)
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
 void detect_input(void)
 {
     // get access if we don't have it already
     dinkeyboard->Acquire();

     // get the input data
     dinkeyboard->GetDeviceState(256, (LPVOID)keystate);
 }

 bool isPressed(WORD key){
	 return (keystate[key]&0x80);
 }

 // this is the function that closes DirectInput
 ~dxKeyboard()
 {
     dinkeyboard->Unacquire();    // make sure the keyboard is unacquired
 }
};