class dxKeyboard
{
public:
	LPDIRECTINPUTDEVICE8 dinkeyboard;    // the pointer to the keyboard device
	BYTE keystate[256];    // the storage for the key-information

	dxKeyboard(LPDIRECTINPUT8 din,HWND wndHandle);

	// this is the function that gets the latest input data
 void detect_input(void);


 bool isDown(DWORD key);


 // this is the function that closes DirectInput
 ~dxKeyboard();
};