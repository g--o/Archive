#include <windows.h>
#include <commctrl.h>
#include <Urlmon.h>
#include <fstream>
#include <string>
#pragma comment(lib,"Urlmon.lib")
#pragma comment(lib,"Comctl32.lib")
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
HWND txt;


bool isNeeded(){
	URLDownloadToFile(0,"http://www.domain.net/new.txt","new.txt",0,0);
	  string line1 ="0",line2="0";
	  ifstream myfile ("version.txt");
	  if (myfile.is_open()){
		while ( myfile.good() )
		{
		 getline(myfile,line1);
		}
	  }
	  myfile.close();
	  myfile.open("new.txt");
	 if (myfile.is_open()){
		while ( myfile.good() )
		{
		 getline(myfile,line2);
		}
	  }
	  myfile.close();
	  if(atoi(line1.c_str())==atoi(line2.c_str())){
		  return false;
	  }

	  	ofstream file2("version.txt");
	   if (file2.is_open())
		   file2<<line2;
	   
	   file2.close();

	return true;
}

void update(){
	URLDownloadToFile(0,"http://www.domain.net/new.txt","update.exe",0,0);
	ShellExecute(0, "open","update.exe", NULL, NULL, SW_SHOWNORMAL);
	}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, int nCmdShow)
{

  HWND hwnd;
  MSG  msg ;    
  WNDCLASS wc = {0};
  wc.lpszClassName = TEXT("Updater");
  wc.hInstance     = hInstance ;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.lpfnWndProc   = WndProc ;
  wc.hCursor       = LoadCursor(0,IDC_ARROW);

  g_hinst = hInstance;
  
  RegisterClass(&wc);
  hwnd = CreateWindow(wc.lpszClassName, TEXT("Updater"),
                WS_POPUP | WS_VISIBLE,
                480, 360, 260, 50, 0, 0, hInstance, 0);  

  	  if(!isNeeded()){
	  ShellExecute(0, "open","Game.exe", NULL, NULL, SW_SHOWNORMAL);
	  return false;
  } 
	   update();
	   SetWindowText(txt,"installing updates please wait...");
  
    while( GetMessage(&msg, NULL, 0, 0)) {
    DispatchMessage(&msg);
	// replace with actual progress bar
	Sleep(1000);
		break;
  }

  return (int) msg.wParam;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
 

  switch(msg)  
  {
	   case WM_CREATE:
      txt=  CreateWindowW(L"STATIC", L"checking for updates", 
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            20, 20, 260, 50,
            hwnd, (HMENU) 1, NULL, NULL);
        break;



      case WM_DESTROY:
          PostQuitMessage(0);
          break; 
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}
