#include<string>
#include<sstream>

#define normal FW_NORMAL
#define bold 
#define aH 20
#define aW 8

template <class T>
std::string toString(const T & t)
{
    std::ostringstream oss; // create a stream
    oss << t;              // insert value to stream 
    return oss.str();      // return as a string
}

/*LPD3DXFONT cFont(LPCTSTR  Name, int h, int w, LPDIRECT3DDEVICE9  pDevice, UINT style, bool italic )
{
	LPD3DXFONT font;
	D3DXCreateFont( pDevice, h, w, style, 0, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, Name, &font );
	return font;
}*/
 
class dxText
{
public:
	RECT rct;
	D3DCOLOR color;
	LPD3DXFONT font;

	dxText::dxText(LPD3DXFONT m_font,D3DCOLOR Color)
	{
		font = m_font;
		color = Color;
		rct.top = 0, rct.bottom = 0 , rct.left = 0, rct.right = 0;
	 }

	void shutdown()
	{
		delete this;
	}

	void setPos( int x, int y)
	{
		// Create a rectangle to indicate where on the screen it should be drawn

		rct.left=x;
		rct.top=y;

	}

	void DisplayText(std::string Text)
	{   
		// Draw some text 
		D3DXFONT_DESC desc;
		font->GetDesc(&desc);
		int w = desc.Width*Text.length()+1;
		int h = desc.Height*Text.length()+1;
		rct.right=rct.left+w;
		rct.bottom=rct.right+h;
		font->DrawText(NULL, Text.c_str(), -1, &rct, 0, color );
	}


};