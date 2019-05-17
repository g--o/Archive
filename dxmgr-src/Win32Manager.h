//Loading required stuff
#pragma comment( lib, "Winmm.lib" )
#include <windowsx.h>
#include <windows.h>


int screen_height, screen_width; 
HWND wndHandle;
int FPS;

class Application
{
private:
	//Defining stuff
	LPCSTR name;
public:

	Application(LPCSTR Name, int fps, LRESULT (CALLBACK *WndProck)(HWND,UINT,WPARAM,LPARAM) , bool& result, bool window=true, HINSTANCE hinstance=NULL)
	{
		wndHandle = NULL;
		FPS = fps;
		name = Name;
		screen_height = GetSystemMetrics (SM_CYSCREEN);
		screen_width = GetSystemMetrics (SM_CXSCREEN);

		if(window) {
			if(!initWindow(hinstance,WndProck))
				MessageBox(0,"Couldn't init window", "ERROR", MB_OK);		
				result = false;
		}

		result = true;
	}

	// Inits our window
	bool initWindow(HINSTANCE hInstance,LRESULT (CALLBACK *WndProc)(HWND,UINT,WPARAM,LPARAM), bool ask=true)
	{
		// Setting screen height and width
		/*	screen_height = 600;
			screen_width  = 800;*/

		WNDCLASSEX wcex;

		wcex.cbSize			= sizeof(WNDCLASSEX);
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= LoadIcon( hInstance, ( LPCTSTR )IDI_APPLICATION );
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= name;
		wcex.hIconSm		= 0;
		RegisterClassEx(&wcex);

		DWORD Style = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE;

		if(MessageBox(0, "Full Screen?",name,MB_YESNO|MB_ICONQUESTION)==IDNO)
			Style = WS_OVERLAPPEDWINDOW;

		wndHandle = CreateWindowEx(WS_EX_CONTROLPARENT, name, 
								 name, 
								 Style,
								 CW_USEDEFAULT, 
								 CW_USEDEFAULT, 
								 screen_width, 
								 screen_height, 
								 NULL, 
								 NULL, 
								 hInstance, 
								 NULL);
	   if (!wndHandle)
	   {
		  MessageBox(NULL, "Failed  to create the window!", "Error", MB_OK);
		  return false;
	   }
   
	   ShowWindow(wndHandle, SW_SHOW);
	   UpdateWindow(wndHandle);

	   return true;
	}

	void LockFrameRate( int fps ) // Locking the frame per second velocity (TNX iYossi from fxp.co.il)
	{
		FPS = fps;

		if(FPS>0)
		{
			timeBeginPeriod(1);

			static long old_time = 0, new_time = 0;
			static int num_frames = 0;
			 long elapsed;

			new_time = timeGetTime();

			elapsed = new_time - old_time;

			while(elapsed< 1000/FPS)
			{
				Sleep(1);

				new_time = timeGetTime();
				elapsed = new_time - old_time;
			}

			num_frames++;
			old_time = new_time;
		}
	}

	int WINAPI run(void(*Do)(), void(*Clean)())
	{
		// Main message loop:
		// Enter the message loop
		MSG msg; 
		ZeroMemory( &msg, sizeof(msg) );

		while( msg.message!=WM_QUIT )
		{
			// Check for messages
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				// Handling our messages
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			// this is called when no messages are pending
			else
			{
				Do();
				LockFrameRate(60); // Locking FPS 
			}
		}

		Clean();

		return (int) msg.wParam;
	}

};
