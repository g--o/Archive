


class dxMouse
{
public:
	LPDIRECTINPUTDEVICE8 dinmouse;    // the pointer to the mouse device
	DIMOUSESTATE mousestate;    // the storage for the mouse-information

	dxMouse(LPDIRECTINPUT8 din,HWND wndHandle);

	~dxMouse();

	// this is the function that gets the latest input data
	void detect_input(void);
};
