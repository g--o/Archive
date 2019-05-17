#define _WIN32_WINNT 0x0502
#define _WIN32_IE 0x0600
#define _WINVER 0x0502 
#define ISOLATION_AWARE_ENABLED 1
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='X86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif 


TCHAR ServName[30];
TCHAR User[30];
TCHAR Pass[30];
TCHAR PortName[30];
TCHAR Name[51];
HWND ServerEdit;
HWND UserEdit;
HWND PassWordEdit;
HWND hwndList;
HWND Port;
HWND Sta;
HWND Sta2;
HWND Sta3;
HWND Sta4;
HWND Sta5;
HWND Sta6;
HWND Sta7;
HWND Sta8;
HWND Sta9;
HWND Sta10;
HWND Sta11;
HWND Sta12;
HWND CurDic;
HWND AcName;
std::string items[51];
int PFPS = 0;
HFONT hFont;
FTPManager* Ftp;




void LockFrameRate( int FPS )
{
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
	else
	{
	}
}

void setAccounts()
{

	items[0] = "Load Account";
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile("*.fa*", &FindFileData);
	   if (hFind != INVALID_HANDLE_VALUE) 
	   {
		 int i = 1;
		 do
		 {
			std::string Cha = FindFileData.cFileName;
			Cha.replace(Cha.rfind(".fa"), Cha.length(), "");
			SendMessage(Client, CB_ADDSTRING, 0, (LPARAM) Cha.c_str());
			items[i] = Cha;
			i++;
		   }
		 while(FindNextFile( hFind,&FindFileData ));
			FindClose(hFind);

		}
}

void saveInput()
{
		GetWindowText(ServerEdit,ServName,GetWindowTextLength(ServerEdit)+1);
		GetWindowText(UserEdit,User,GetWindowTextLength(UserEdit)+1);
		GetWindowText(PassWordEdit,Pass,GetWindowTextLength(PassWordEdit)+1);
		GetWindowText(Port,PortName,GetWindowTextLength(Port)+1);
		GetWindowText(AcName,Name,GetWindowTextLength(AcName)+1);
}

bool CreateAccount()
{
	saveInput();
	if(GetWindowTextLength(AcName) != 0 && GetWindowTextLength(PassWordEdit) != 0 && GetWindowTextLength(UserEdit) != 0 && GetWindowTextLength(ServerEdit) != 0 && GetWindowTextLength(Port) != 0 )
	{
		std::string fName = pDir;
		fName+= "\\";
		fName += Name;
		fName+= ".fa";
		std::ofstream faFile (fName);

		if(faFile.is_open())
			{
			faFile<<ServName<<std::endl;
			faFile<<User<<std::endl;
			faFile<<Pass<<std::endl;
			faFile<<PortName<<std::endl;
			faFile.close();
			MessageBox(NULL,"Account created, restart the program to update","Create Account",MB_ICONINFORMATION);
			return true;
			}
			else
			{
				MessageBox(NULL,"Account wasn't created, access denied?","Create Account",MB_ICONERROR);
				return false;
			}
	}
	else
	{
		MessageBox(NULL,"Please fill all fields","Create Account", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
}

bool DelAc()
{
	saveInput();

	if(GetWindowTextLength(AcName) != 0)
	{

		std::string fName = pDir;
		fName+= "\\";
		fName += Name;
		fName+= ".fa";
			if( remove( fName.c_str() ) != 0 )
			{
				MessageBox(NULL, "Couldn't delete account, access denied", "Error", MB_OK|MB_ICONERROR);
				return false;
			}
			else
			{
				MessageBox(NULL, "Account was deleted please restart", "Account Delete", MB_OK|MB_ICONINFORMATION);
				return true;
			}
	}
		else
	{
		MessageBox(NULL,"Please fill all fields","Account Delete", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	
}

bool LoadAccount()
{
	LRESULT sel;
	sel = SendMessage(Client, CB_GETCURSEL, 0, 0);
	if(items[sel] != "Load Account")
	{
	std::string change = pDir;
	change+="\\";
	change+= items[sel];
	change+= ".fa";

	std::ifstream ftpa (change);
	if (ftpa.is_open())
  {
		   getline (ftpa,change);
		   SetWindowText(ServerEdit,change.c_str());
		   getline (ftpa,change);
		   SetWindowText(UserEdit,change.c_str());
		   getline (ftpa,change);
		   SetWindowText(PassWordEdit,change.c_str());
		   getline (ftpa,change);
		   SetWindowText(Port,change.c_str());

		ftpa.close();
		return true;
  }
	else
	{
		MessageBox(NULL,"Failed to load account","Error",MB_ICONERROR);
		return false;
	}
		}
		else
		{
		   SetWindowText(ServerEdit,"");
		   SetWindowText(UserEdit,"");
		   SetWindowText(PassWordEdit,"");
		   SetWindowText(Port,"");
		   return true;
		}
}

bool OpenDialog(HWND hwnd) 
{
  OPENFILENAME ofn;
  TCHAR szFile[MAX_PATH];

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.lpstrFile = szFile;
  ofn.lpstrFile[0] = '\0';
  ofn.hwndOwner = hwnd;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = TEXT("fa files(*.*)\0*.fa*\0");
  ofn.nFilterIndex = 1;
  ofn.lpstrInitialDir = NULL;
  ofn.lpstrFileTitle = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  

  if(GetOpenFileName(&ofn))
  {
      return LoadAccount();
  }
  else
  {
	  return false;
  }
}


