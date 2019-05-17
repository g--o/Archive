
class FTPManager
{
public:
  HINTERNET hInternet, hFtpSession, hFind;
  PTSTR pszCurDir; 
  TCHAR achBuffer[MAX_PATH], file[MAX_PATH+8], ftppath[MAX_PATH+8]; 
  DWORD buffer[MAX_PATH+8];
  HWND CurDic, hwndList;
  WIN32_FIND_DATA dirInfo;

  FTPManager::FTPManager(HWND pCurDic, HWND phwndList)
  {
	CurDic = pCurDic;
	hwndList = phwndList;
	hFtpSession = NULL;
	hInternet = NULL;
  }


  void GetDic()
{
	if(hFtpSession != NULL)
	{
DWORD           dwError;
TCHAR           szMsgBuffer[MAX_PATH+8];
TCHAR           szFName[MAX_PATH+8];

		if( FtpGetCurrentDirectory(hFtpSession, ftppath,buffer))
		{
		if(CurDic != NULL)
		{
		SetWindowText( CurDic,  ftppath );
		}
		if(hwndList != NULL)
		{
	  SendMessage( hwndList, LB_RESETCONTENT, 0, 0 );
		}
  hFind = FtpFindFirstFile( hFtpSession, TEXT( "*.*" ), 
                            &dirInfo, FTP_TRANSFER_TYPE_BINARY , 0 );
  		if(hwndList != NULL)
		{
 do
  {
    if( FAILED( StringCchCopy( szFName, MAX_PATH+8,dirInfo.cFileName ) ) ||( ( dirInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&( FAILED( StringCchCat( szFName, MAX_PATH+8, TEXT( " <DIR> " ) ) ) ) ) )
    {
	  MessageBox(NULL, "Couldn't get data from server", "Refresh", MB_ICONERROR);
    }
    SendMessage( hwndList, LB_ADDSTRING, 0, (LPARAM) szFName );
  }
  while( InternetFindNextFile( hFind, (LPVOID) &dirInfo ) );

  if( ( dwError = GetLastError( ) ) == ERROR_NO_MORE_FILES )
  {
    InternetCloseHandle(hFind);
  }
		}
		}
		else
		{
			GetDic();
		}
	}
}

bool GetFile()
{
	return false;
}

void Go2Dic(HWND hWnD, int DlgId)
{
  TCHAR szNewDirName[MAX_PATH+8];
  std::string Change;
  DlgDirSelectEx(hWnD, szNewDirName, MAX_PATH+8, DlgId); 

if(szNewDirName != "")
{
	Change = szNewDirName;
FtpSetCurrentDirectory( hFtpSession, Change.c_str());
	if(Change.find(" <DIR>") != std::string::npos)
	{
	Change.replace(Change.find(" <DIR>"), Change.length(), "");
	FtpSetCurrentDirectory( hFtpSession, Change.c_str() );
	}

	if(Change.find(".") == Change.length()-1)
	{
	Change.replace(Change.length()-1, Change.length(), "");
	FtpSetCurrentDirectory( hFtpSession, Change.c_str() );
	}

	if(Change.find(".") == 0)
	{
	Change.replace(0, 1, "");
	FtpSetCurrentDirectory( hFtpSession, Change.c_str() );
	}
}
GetDic();
}


bool CloseConnection()
{

		if(hFtpSession == NULL && hInternet == NULL)
		{
		return false;
		}
		else
		{
		InternetCloseHandle(hFtpSession);
		InternetCloseHandle(hInternet);
		hFtpSession = NULL;
		hInternet = NULL;
		SetWindowText(CurDic, "Directory");
		 SendMessage( hwndList, LB_RESETCONTENT, 0, 0 );
		 return true;
		}
}

bool Connect(LPCSTR Server, LPCSTR UserName, LPCSTR PassWord, LPCSTR PortName)
{
if(hFtpSession != NULL && hInternet!= NULL)
{
	CloseConnection();
}
		 
	  hInternet = InternetOpen(NULL,INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0);
	  hFtpSession = InternetConnect(hInternet, Server, atoi(PortName), UserName, PassWord, INTERNET_SERVICE_FTP, 0, 0);
		if(hFtpSession == NULL)
		{
			CloseConnection();
			return false;
		}
		else
		{
		GetDic();
		return true;
		}


}

bool Upload( HWND hWnD)
{
	std::string FP;
	FtpGetCurrentDirectory(hFtpSession, ftppath,buffer);
	DlgDirSelectEx(hWnD, file, MAX_PATH+8, 14); 
	FP = ftppath;
	FP += "/";
	FP+= file;
	if(file != "" && hFtpSession != NULL && FP.compare("") != 0)
	{
		if(FtpPutFile(hFtpSession, file, FP.c_str(), FTP_TRANSFER_TYPE_UNKNOWN, 0))
  {
	  MessageBox(NULL, "Upload success!", "Upload", MB_ICONINFORMATION);
	  GetDic();
	  return true;
  }
  else
  {
	  MessageBox(NULL, "Upload failed!", "Upload", MB_ICONERROR);
	  GetDic();
	  return false;
  }
	}
				
}



bool Delete(HWND hWnD)
{
	std::string DFP;
	FtpGetCurrentDirectory(hFtpSession, ftppath,buffer);
	DlgDirSelectEx(hWnD, file, MAX_PATH+8, 11); 
	DFP = ftppath;
	DFP += "/";
	DFP+= file;
	if(file != "" && hFtpSession != NULL && DFP.compare("") != 0)
	{
if(FtpDeleteFile( hFtpSession, DFP.c_str() ))
{
	  MessageBox(NULL, "Delete success!", "Delete", MB_ICONINFORMATION);
	  GetDic();
	  return true;
}
else
{
	  MessageBox(NULL, "Delete failed!", "Delete", MB_ICONERROR);
}
			GetDic();
	 return false;
	}
}


bool DownFile(HWND hwd)
{

	std::string tFile;
	FtpGetCurrentDirectory(hFtpSession, ftppath,buffer);
	DlgDirSelectEx(hwd, file, MAX_PATH+8, 11); 
	tFile = ftppath;
	tFile += "/";
	tFile+= file;
	if(file != "" && hFtpSession != NULL && tFile.compare("") != 0)
	{
		WIN32_FIND_DATA FindFileData;
		if (FindFirstFile(file, &FindFileData) != INVALID_HANDLE_VALUE)
		{
			if(MessageBox(NULL,"File already exists - replace?", "Warning", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
			{
				goto download;
			}
			else
			{
				MessageBox(NULL, TEXT("Download aborted!"), TEXT("Download"), MB_OK|MB_ICONEXCLAMATION);
				return false;
			}
		}
		else
		{
	download:
		{
			if(FtpGetFile(hFtpSession,tFile.c_str(),file,false,INTERNET_FLAG_RELOAD | FTP_TRANSFER_TYPE_UNKNOWN,NULL,NULL))
			{
				MessageBox(NULL,"Download complete!","Info",MB_ICONINFORMATION);
			}
			else
			{
				MessageBox(NULL, TEXT("Download failed!"), TEXT("Download"), MB_OK|MB_ICONSTOP);
			}
		}
		}
	}
	else
	{
		return false;
	}
}

};