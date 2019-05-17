#include <windows.h>
#include <iostream>
#include <Wininet.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <Strsafe.h>
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "Wininet.lib" )
#include "VisualStyles.h"
#include "FTP.h"
#include "Dialog.h"
#include "resource.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, int nCmdShow )
{
  MSG  msg ;    
  HWND hwnd;
  WNDCLASS wc;
	
  wc.style         = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.lpszClassName = TEXT( "FtpManager" );
  wc.hInstance     = hInstance ;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.lpszMenuName  = NULL;
  wc.lpfnWndProc   = WndProc;
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  
  RegisterClass(&wc);
  hwnd = CreateWindow( wc.lpszClassName, TEXT("Ftp Manager"),
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                100, 100, 600, 500, NULL, NULL, hInstance, NULL);  
Ftp = new FTPManager(CurDic, hwndList);

  while( GetMessage(&msg, NULL, 0, 0)) {
	TranslateMessage(&msg);
    DispatchMessage(&msg);

    LockFrameRate(PFPS);
  }
Ftp->CloseConnection();
  return (int) msg.wParam;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch(msg)  
  {


    case WM_DESTROY:
      {
        PostQuitMessage(0);
        return 0;
      }
	      case WM_KEYDOWN:
      {
		  		  if(wParam == VK_ESCAPE)
		  {
        PostQuitMessage(0);
        return 0;
			}
	  }
	    case WM_CREATE:
	  {
		  HICON hIcon, hIconSm;
			hIcon = (HICON)LoadImage(NULL, "Icon.ico", IMAGE_ICON, 64, 64, LR_LOADFROMFILE);
        if(hIcon)
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        else
            MessageBox(hwnd, "Could not load large icon!", "Error", MB_OK | MB_ICONERROR);

        hIconSm = (HICON)LoadImage(NULL, "mico.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
        if(hIconSm)
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
        else
            MessageBox(hwnd, "Could not load small icon!", "Error", MB_OK | MB_ICONERROR);


	   Sta7 =  CreateWindow(TEXT("static"), "FTP Manager", 
		    WS_CHILD | WS_VISIBLE | SS_RIGHT,
		    200, 20, 60, 40,
		    hwnd, (HMENU) 1, NULL, NULL);
			 Sta8 = CreateWindow(TEXT("static"), "FTP Server", 
		    WS_CHILD | WS_VISIBLE | SS_RIGHT ,
		    50, 50, 70, 40,
		    hwnd, (HMENU) 2, NULL, NULL);
ServerEdit = CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE ,
				150, 50, 160, 20, hwnd, (HMENU) 3,
				NULL, NULL);
			    Sta9 = CreateWindow(TEXT("static"), "Username", 
		    WS_CHILD | WS_VISIBLE | SS_RIGHT,
		    50, 80, 70, 40,
		    hwnd, (HMENU) 4, NULL, NULL);
UserEdit = CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE ,
				150, 80, 160, 20, hwnd, (HMENU) 5,
				NULL, NULL);
			   Sta10 = CreateWindow(TEXT("static"), "Password", 
		    WS_CHILD | WS_VISIBLE | SS_RIGHT,
		    50, 110, 70, 40,
		    hwnd, (HMENU) 6, NULL, NULL);
PassWordEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE |ES_PASSWORD ,
				150, 110, 160, 20, hwnd, (HMENU) 7,
				NULL, NULL);

Sta = CreateWindow(
		TEXT("button"), TEXT("Login"),       
		WS_VISIBLE | WS_CHILD,  
		200, 150, 80, 25,        
		hwnd, (HMENU) 8, NULL, NULL); 
Sta2 = CreateWindow(
		TEXT("button"), TEXT("Logout"),       
		WS_VISIBLE | WS_CHILD,  
		200, 180, 80, 25,        
		hwnd, (HMENU) 9, NULL, NULL); 
	     Sta3 = CreateWindow(TEXT("button"), TEXT("60 Fps"),
                     WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
                     20, 430, 100, 35,        
                      hwnd, (HMENU) 10, NULL, NULL);

            hwndList = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox") , NULL, WS_CHILD | WS_VISIBLE |LBS_NOTIFY | LBS_SORT | WS_VSCROLL ,
               10, 230, 200, 200, hwnd,(HMENU) 11, NULL, NULL);
			CurDic = CreateWindow(TEXT("static"), "Directory", 
		    WS_CHILD | WS_VISIBLE | SS_LEFT,
		    10, 210, 150, 20,
		    hwnd, (HMENU) 12, NULL, NULL);
            CompList = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox") , NULL, WS_CHILD | WS_VISIBLE |LBS_NOTIFY | LBS_SORT | WS_VSCROLL ,
               280, 230, 200, 200, hwnd,(HMENU) 14, NULL, NULL);
	Sta4 = CreateWindow(
		TEXT("button"), TEXT("<-"),       
		WS_VISIBLE | WS_CHILD,  
		210, 270, 70, 40,        
		hwnd, (HMENU) 13, NULL, NULL); 
		Sta5 = CreateWindow(
		TEXT("button"), TEXT("X"),       
		WS_VISIBLE | WS_CHILD,  
		210, 230, 70, 40,         
		hwnd, (HMENU) 15, NULL, NULL); 
		Sta6 = CreateWindow(TEXT("static"), "Port", 
		    WS_CHILD | WS_VISIBLE | SS_RIGHT,
		    320, 50, 70, 40,
		    hwnd, (HMENU) 15, NULL, NULL);
Port = CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE  ,
				320, 80, 160, 20, hwnd, (HMENU) 16,
				NULL, NULL);
AcName = CreateWindowEx(WS_EX_CLIENTEDGE,TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE  ,
				320, 110, 160, 20, hwnd, (HMENU) 23,
				NULL, NULL);
	Sta11 = CreateWindow(
		TEXT("button"), TEXT("->"),       
		WS_VISIBLE | WS_CHILD,  
		210, 310, 70, 40,        
		hwnd, (HMENU) 17, NULL, NULL); 
		Sta12 = CreateWindow(
		TEXT("button"), TEXT("Refresh"),       
		WS_VISIBLE | WS_CHILD,  
		210, 350, 70, 40,        
		hwnd, (HMENU) 18, NULL, NULL);
		Sta13 = CreateWindow(TEXT("static"), "", 
		    WS_CHILD | WS_VISIBLE | SS_RIGHT,
		    150, 205, 400, 25,
		    hwnd, (HMENU) 15, NULL, NULL);
		 Client = CreateWindow(TEXT("combobox"),NULL, 
                  WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
                  10, 10, 120, 110, hwnd, (HMENU) 21, NULL, NULL);   
		 HWND CreateAc = CreateWindow(
		TEXT("button"), TEXT("Save Account"),       
		WS_VISIBLE | WS_CHILD,  
		350, 150, 80, 25,        
		hwnd, (HMENU) 22, NULL, NULL); 
		 	HWND Del = 	 CreateWindow(
		TEXT("button"), TEXT("Delete Accout"),       
		WS_VISIBLE | WS_CHILD,  
		450, 150, 80, 25,        
		hwnd, (HMENU) 25, NULL, NULL); 
		 HWND Sta14 = CreateWindow(TEXT("static"), "Username", 
		    WS_CHILD | WS_VISIBLE | SS_RIGHT,
		    480, 110, 70, 20,
		    hwnd, (HMENU) 24, NULL, NULL);
		  SetFont(DeafultFont,Sta);
		  SetFont(DeafultFont,Sta2);
		  SetFont(DeafultFont,Sta3);
		  SetFont(DeafultFont,Sta4);
		  SetFont(DeafultFont,Sta5);
		  SetFont(DeafultFont,Sta6);
		  SetFont(DeafultFont,Sta7);
		  SetFont(DeafultFont,Sta8);
		  SetFont(DeafultFont,UserEdit);
		  SetFont(DeafultFont,ServerEdit);
		  SetFont(DeafultFont,PassWordEdit);
		  SetFont(DeafultFont,hwndList);
		  SetFont(DeafultFont,CurDic);
		  SetFont(DeafultFont,Port);
		  SetFont(DeafultFont,Sta9);
		  SetFont(DeafultFont,Sta10);
		  SetFont(DeafultFont,CompList);
		  SetFont(DeafultFont,Sta11);
		  SetFont(DeafultFont,Sta12);
		  SetFont(DeafultFont,Sta13);
		  SetFont(DeafultFont,Sta14);
		  SetFont(DeafultFont,Client);
		  SetFont(DeafultFont,CreateAc);
		  SetFont(DeafultFont,AcName);
		  SetFont(DeafultFont,Del);
		  SendMessage(Client, CB_ADDSTRING, 0, (LPARAM) "Load Account");
		  SendMessage(Client, CB_SETCURSEL , 0, (LPARAM) "Load Account");
		  GetCurrentDirectory(MAX_PATH, pDir); 
		  setAccounts(); // Setting our accounts list
		  // Setting our dir to the desktop - C:\Users\UserName\Desktop
		  std::string Desk;
		  Desk = "C:\\Users\\";
		  // Getting the username
		  TCHAR name [ 257 ];
		  DWORD size = 257;
		  GetUserName((TCHAR*)name, &size );
		
		  Desk+= name;
		  Desk+= "\\Desktop";
		  SetCurrentDirectory(Desk.c_str()); // Setting the dir
		  getDir(hwnd); // Updating

	  }


		case WM_COMMAND:
			{
				if ( HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == 21)
				{
					if(Ftp->hFtpSession != NULL && Ftp->hInternet != NULL)
					{
						Ftp->CloseConnection();
					}
						LoadAccount();
				}

				if( HIWORD(wParam) == LBN_DBLCLK )
				{
					if(LOWORD(wParam) == 11)
						{
							Ftp->Go2Dic(hwnd,11);
						}
					if(LOWORD(wParam) == 14)
						{
							go2Dir(hwnd,14);
							getDir(hwnd);
						}
				}

				if( HIWORD(wParam) == BN_CLICKED)
				{
						if(wParam == 18)
					{
						Ftp->GetDic();
						getDir(hwnd); 
					}
						if(wParam == 17)
						{
							if(Ftp->DownFile(hwnd))
							{
								
							}
							else
							{
								
							}
							getDir(hwnd); 
						}
						if(wParam == 22)
						{
							CreateAccount();
						}
						if(wParam == 25)
						{
							DelAc();
						}

					if(wParam == 8)
					{
					if(GetWindowTextLength(PassWordEdit) == 0 || GetWindowTextLength(UserEdit) == 0 || GetWindowTextLength(ServerEdit) == 0 || GetWindowTextLength(Port) == 0)
					{
						MessageBox(NULL, TEXT("Please fill all fields"), TEXT("Login failed"), MB_OK|MB_ICONEXCLAMATION);
					}
					else
					{
					saveInput();
					if(Ftp->Connect(ServName,User,Pass,PortName))
					{
						MessageBox(NULL, TEXT("Login success"), TEXT("Login success"), MB_OK|MB_ICONINFORMATION);
					}
					else
					{
						MessageBox(NULL, TEXT("Login failed, check fields"), TEXT("Login failed"), MB_OK|MB_ICONSTOP);
					}
					}
				}
					else if(wParam == 13)
					{
						Ftp->Upload(hwnd);
					}
					else if(wParam == 15)
					{
						Ftp->Delete(hwnd);
					}
					else if(wParam == 9)
					{
					if(Ftp->CloseConnection())
					{
					
					MessageBox(NULL, TEXT("Logout success"), TEXT("Logout"), MB_OK|MB_ICONINFORMATION);
					}
					else
					{
					MessageBox(NULL, TEXT("You are not connected"), TEXT("Logout"), MB_OK|MB_ICONSTOP);
					}
					}
					else if(wParam == 10)
					{
		 BOOL checked = IsDlgButtonChecked(hwnd, 10);
		
	    if (checked) {
	        CheckDlgButton(hwnd, 10, BST_UNCHECKED);
			PFPS = 60;
	    } else {
		 CheckDlgButton(hwnd, 10, BST_CHECKED);
			PFPS = 0;
	    }
					}
				}
				
			}

  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}
