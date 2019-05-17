// main.cpp - implements the managers
// Loading required stuff - the managers
#include "Win32Manager.h" // The Win32 C++ Manager header
#include "DxManager.h" // The DirectX C++ Manager header
#include "main.h" // The main.cpp header - defining stuff

// Getting our message
LRESULT CALLBACK handleMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
			case WM_SIZE:
			{
				if(dxMgr != NULL)
				{
					if( dxMgr->dxDevice != NULL && wParam != SIZE_MINIMIZED)
					{
						dxMgr->resetDevice(LOWORD(lParam),HIWORD(lParam));
					}
				}
				break;
			}

		case WM_KEYDOWN:
		{
			if(wParam == VK_RETURN)
				explode();
			if(wParam == VK_ESCAPE)
				PostQuitMessage(0);
			if(wParam == VK_SPACE)
				dxMgr->getCurScene()->vObjects.at(0)->nextSet();
			if(wParam==VK_RIGHT)
				dxMgr->getCurScene()->dxCam->camera_position.x++;
			if(wParam==VK_LEFT)
				dxMgr->getCurScene()->dxCam->camera_position.x--;
			if(wParam==VK_UP)
				dxMgr->getCurScene()->dxCam->camera_position.z++;
			if(wParam==VK_DOWN)
				dxMgr->getCurScene()->dxCam->camera_position.z--;
			break;
		}

		case WM_MOUSEMOVE:
		{
			D3DXVECTOR3 pos = dxMgr->pickingOnAir(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
			dxMgr->getCurScene()->vObjects.at(0)->Move(pos.x,pos.y,pos.z);
			break;
		}

		case WM_DESTROY:
			PostQuitMessage(0); //Quiting
			break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Run()
{
	// call our render function
	aScene(); // Calling our scene
}

void cleanUp()
{
	// Shutdown the directx manager
	dxMgr->shutdown();
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	bool result;
	Application app("dxManager", 60, handleMessages, result, hInstance );

	if (!result)
	{
		return false;
	}

    if(!initDX(app))
	{
		dxMgr->shutdown();
		return false;
	}

	app.run(Run, cleanUp);
}

