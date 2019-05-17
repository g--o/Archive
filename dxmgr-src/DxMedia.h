


LPDIRECT3DTEXTURE9 LoadTexture(LPCSTR pSrcFile, LPDIRECT3DDEVICE9 pDevice); // Useful for loading textures

class dxTexture:public dxResource // DirectX texture class
{
public:
	LPDIRECT3DTEXTURE9 m_Texture; // The texture itself
	float width, height; // The texture dimensions

	dxTexture(URI pPath); // Initing the dxTexture object
	~dxTexture(); // Destriying the dxTexture object
	bool loadTex(LPCSTR path, LPDIRECT3DDEVICE9 p_mDevice); // Loading the texture

};

LPDIRECT3DTEXTURE9 LoadTexture(LPCSTR pSrcFile, LPDIRECT3DDEVICE9 pDevice) // Useful for loading textures
{
	LPDIRECT3DTEXTURE9 gTexture = NULL; // Creating a temporary texture
	if(FAILED(D3DXCreateTextureFromFile(pDevice,pSrcFile,&gTexture))) // Loading the texture file
	{
		return NULL; // In case of failure - return NULL
	}
		return gTexture; // Otherwise return the temporary loaded texture
}

	dxTexture::dxTexture(std::string pPath):dxResource(pPath) // Initing the dxTexture object
	{
		m_Texture = NULL;
		width = 0, height = 0;
	}

	dxTexture::~dxTexture() // Destriying the dxTexture object
	{
		if(m_Texture != NULL)
		{
			m_Texture->Release();
			m_Texture = NULL;
		}

		width = NULL, height = NULL;
	}

	bool dxTexture::loadTex(LPCSTR path, LPDIRECT3DDEVICE9 p_mDevice) // Loading the texture
	{
		m_Texture = LoadTexture(path,p_mDevice);
		D3DXIMAGE_INFO Info;
		ZeroMemory(&Info,sizeof(D3DXIMAGE_INFO));
		if(SUCCEEDED(D3DXGetImageInfoFromFile(path, &Info)))
		{
			width = Info.Width, height = Info.Height;
			return true;
		}
		else
		{
			return false;
		}

	}