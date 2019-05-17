#include "dxInput.h"

void dxInput::initDirectInput()
	{
				DirectInput8Create(GetModuleHandle(NULL),    // the handle to the application
						   DIRECTINPUT_VERSION,    // the compatible version
						   IID_IDirectInput8,    // the DirectInput interface version
						   (void**)&din,    // the pointer to the interface
						   NULL);    // COM stuff, so we'll set it to NULL
	}

	dxInput::dxInput(HWND hwnd)
	{
		wndHandle = hwnd;
		initDirectInput();
		pMouse=NULL;
		pKeyboard=NULL;
	}

	void dxInput::update()
	{
		if(pMouse!=NULL)
		{
			pMouse->detect_input();
		}
		if(pKeyboard!=NULL)
		{
			pKeyboard->detect_input();
		}
	}

	void dxInput::createMouseInput()
	{
		pMouse = new dxMouse(din,wndHandle);
	}

	void dxInput::createKeyboardInput()
	{
		pKeyboard = new dxKeyboard(din,wndHandle);
	}

	void dxInput::cleanDirectInput()
	{
		din->Release();    // close DirectInput before exiting
	}