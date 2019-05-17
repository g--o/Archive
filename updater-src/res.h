HINTERNET hInternet,hFile;
HWND hwndPrgBar;
LPCSTR filename="logo.png";

bool Dumper (HINTERNET hResource)
{
    BYTE* lpszData;    // buffer for the data
    DWORD  dwSize;       // size of the data available
    DWORD  dwDownloaded; // size of the downloaded data
	FILE *file;
	file = fopen(filename, "wb");
	 if (file == NULL)
        return false;

    // Set the cursor to an hourglass.
    SetCursor(LoadCursor(NULL,IDC_WAIT));

    // This loop handles reading the data.
    do
    {
        // The call to InternetQueryDataAvailable determines the
        // amount of data available to download.
        if (!InternetQueryDataAvailable(hResource,&dwSize,0,0))
        {
            SetCursor(LoadCursor(NULL,IDC_ARROW));
            return false;
        }
        else
        {
            // Allocate a buffer of the size returned by
            // InternetQueryDataAvailable.
            lpszData = (BYTE*)malloc(dwSize+1);

            // Read the data from the HINTERNET handle.
            if(!InternetReadFile(hResource,
                                 (LPVOID)lpszData,
                                 dwSize,
                                 &dwDownloaded))
            {
                printf("InternetReadFile failed (%d)\n", GetLastError());
                free(lpszData);
                break;
            }
            else
            {
					fwrite(lpszData, dwDownloaded, 1, file);
                   free(lpszData);

					//SendMessage( hwndPrgBar, PBM_SETPOS, 0, 0 );
                    if (dwDownloaded == 0){
                        break;
					}

                }
            }
	} 
    while(TRUE);
    // Close the HINTERNET handle.
    InternetCloseHandle(hResource);

    // Set the cursor back to an arrow.
    SetCursor(LoadCursor(NULL,IDC_ARROW));

    return TRUE;
}


bool begin(){
	
  LPCSTR accept[2]={"*/*", NULL};
  hInternet = InternetOpen("http generic",
		INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
  std::string url = "http://www.domain.net/";
  url+=filename;
     hFile = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, 0, 0);
	 if(hFile==NULL){
		 MessageBox(0,"couldn't connect","error",0);
		 return false;
	 }
	
		return true;
}

bool update(){
	return Dumper(hFile );
}
