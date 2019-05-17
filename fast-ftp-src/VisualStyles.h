HFONT DeafultFont = CreateFont(-11,0,0,0,FW_NORMAL,false,false,false,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_DONTCARE,"Microsoft Sans Serif");

void SetFont(HFONT font, HWND HwNd)
{
SendMessage(HwNd,WM_SETFONT,(WPARAM)font,MAKELPARAM(true,0));
}
