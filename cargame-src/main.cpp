//load external librarys

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <cstdio>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>  //if max was used
#include <cassert>
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "Winmm.lib" )
//load internal librarys
#include"meshmanager.h"
#include "DirectInput.h"
#include"game.h"

LRESULT CALLBACK WindowProcedure(HWND winhan,UINT uint_Message,WPARAM parameter1,LPARAM parameter2)
{
	switch(uint_Message)
	{

		case 16://case window's exist button is pressed
		{
			GameState = 0;
			break;
		}	

}

return DefWindowProc(winhan,uint_Message,parameter1,parameter2);
}

HWND NewWindow(LPCTSTR str_Title,int int_XPos, int int_YPos, int int_Width, int int_Height)
{
	WNDCLASSEX wnd_Structure;

	wnd_Structure.cbSize = sizeof(WNDCLASSEX);
	wnd_Structure.style = CS_HREDRAW | CS_VREDRAW;
	wnd_Structure.lpfnWndProc = WindowProcedure;
	wnd_Structure.cbClsExtra = 0;
	wnd_Structure.cbWndExtra = 0;
	wnd_Structure.hInstance = GetModuleHandle(NULL);
	wnd_Structure.hIcon = NULL;
	wnd_Structure.hCursor = LoadCursor(NULL,IDC_ARROW);;
	wnd_Structure.hbrBackground = GetSysColorBrush(NULL);
	wnd_Structure.lpszMenuName = NULL;
	wnd_Structure.lpszClassName = "WindowClassName";
	wnd_Structure.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

	RegisterClassEx(&wnd_Structure);

	return CreateWindowEx(WS_EX_CONTROLPARENT,wnd_Structure.lpszClassName, str_Title, WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE, int_XPos, int_YPos, int_Width, int_Height, NULL, NULL, GetModuleHandle(NULL), NULL);
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPreviousInstance,LPSTR lpcmdline,int nCmdShow)
{

	MSG msg_Message;
	//create a new window
	HWND MainWindow = NewWindow("Car Physics Test",0,0,width,height);
	CreateDevice(MainWindow);
	Device->SetRenderState( D3DRS_ZENABLE, TRUE );
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Device->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(50,50,50) );
	DisplaySomeText();
	Car = new MeshManager(Device);
	SkyBox = new MeshManager(Device);
	Environment = new MeshManager(Device);
	lamp = new MeshManager(Device);
	grass = new MeshManager(Device);
	grass->Load("grass.x",true);
	Car->Load("car.x",true);
	SkyBox->Load("sky03.x",false);
	Environment->Load("123.x",false);
	lamp->Load("lamppost.x",false);
	lamp->CalBB();
	Car->CalBB();
	cdepth = Car->getDepth();
	cwidth = Car->getWidth();
	initDirectInput();
	Keyboard = new dxKeyboard(MainWindow);

	while(GameState != 0)//loop start
	{
		if(PeekMessage(&msg_Message,MainWindow,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg_Message);
			DispatchMessage(&msg_Message); 
		}
		Updates();
		LockFrameRate(60);
		DrawScene();
	}//loop end

	//loop is completed, time to release everything and close the window
	cleanUp();
	DestroyWindow(MainWindow);

	return 0;
}
