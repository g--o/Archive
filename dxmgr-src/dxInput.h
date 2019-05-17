#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "dxMouse.h"
#include "dxKeyboard.h"

class dxInput
{
public:
	LPDIRECTINPUT8 din;    // the pointer to our DirectInput interface
	HWND wndHandle;
	dxMouse* pMouse; // Our mouse
	dxKeyboard* pKeyboard; // Our keyboard

	void initDirectInput();
	dxInput(HWND hwnd);
	void update();
	void createMouseInput();
	void createKeyboardInput();
	void cleanDirectInput();
};