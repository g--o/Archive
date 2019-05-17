
// Dialog stuff
  PTSTR psCurDir; 
  TCHAR aBuffer[MAX_PATH];
  TCHAR pDir[MAX_PATH];
  HWND Sta13;
  HWND CompList;
  HWND Client;

bool getDir(HWND hwnd)
{
		  psCurDir = aBuffer; 
		  GetCurrentDirectory(MAX_PATH, psCurDir); 
		  SetWindowText( Sta13,  psCurDir );

		  return DlgDirList(hwnd, psCurDir, 14, 14, DDL_DIRECTORY | DDL_ARCHIVE | DDL_READWRITE | DDL_READONLY );
}

bool go2Dir(HWND Hwnd,int ConId)
{

	 TCHAR szNewDirName[MAX_PATH+8];
	 DlgDirSelectEx(Hwnd, szNewDirName, MAX_PATH+8, ConId); 
	return SetCurrentDirectory(szNewDirName);
}