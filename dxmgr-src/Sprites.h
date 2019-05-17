// Sprites Manager
#pragma once

// The dxSprite class
class dxSprite
{
public:
	float posX, posY;
	LPDIRECT3DDEVICE9 m_pDevice; // Holding the DirectX device
	RECT m_SrcRect; // Holding the whole rect
	dxTexture* m_sTexture; // Holding a dxTexture object, which is used for holding the sprite's texture
	LPD3DXSPRITE m_Sprite; // The sprite itself
	DWORD flag;
	bool alive;
	float alpha,factor;

	void setFadeFactor(float fac)
	{
		factor=fac;
	}

	void fade() // Fade in effect
	{
		if(factor>0)
			alpha = (alpha+factor>255)?alpha:alpha+factor;
		else
			alpha = (alpha+factor<0)?alpha:alpha+factor;
	}


	void setRect(int x, int y, int width, int heignt) // Setting the RECT vertices
	{
		m_SrcRect.left = x, m_SrcRect.top = y, m_SrcRect.right = width, m_SrcRect.bottom = heignt;
	}

	dxSprite(LPDIRECT3DDEVICE9 pDevice,float x,float y,DWORD pflag) // Constructor - inits the sprite
	{
		factor=0;
		alpha=255;
		flag=pflag;
		setPos(x,y);
		alive = true;
		m_Sprite = NULL; // Our sprite temporarly will be equal to NULL
		m_pDevice = pDevice; // Setting the DirectX device
		setRect(0,0,0,0); // Setting rect to deafult
		D3DXCreateSprite(m_pDevice, &m_Sprite); // Creating the sprite
	}

	~dxSprite() // Destructor - destroys our sprite
	{
		delete m_sTexture;
		if(m_Sprite != NULL)
		{
			m_Sprite->Release();
			m_Sprite = NULL;
		}
	}

	void setAllImage() // Setting the RECT to hold the entire image
	{
		if( m_sTexture->width != 0 && m_sTexture->height != 0)
			setRect(0,0,m_sTexture->width, m_sTexture->height); // Setting the RECT
	}


	void startRender(DWORD flag){ // Start rendering
		m_Sprite->Begin(flag); // Beginning to render (with/out alphablend)
	}

	void endRender(){ // End rendering
		m_Sprite->End(); // Ending to render
	}

	void setPos(float x,float y){
		posX = x;
		posY = y;
	}

	void Update()
	{
		fade();
	}

	HRESULT Draw(){
		if(alive)
		{
			startRender(flag); // Starting to render
			HRESULT r = DrawTexture(); // Drawing the sprite
			endRender(); // Ending to render
			return r; // returning result
		}
		return E_FAIL;
	}

	HRESULT DrawTexture() // Drawing the sprite
	{
		if((m_sTexture->m_Texture != NULL) && (m_Sprite != NULL)) // Making sure everything already inited
		{
			HRESULT Result = m_Sprite->Draw(m_sTexture->m_Texture,&m_SrcRect, NULL, &D3DXVECTOR3(posX,posY,0),0xFFFFFFFF); // drawing the sprite
			return Result; // Returning the result
		}
		else
		{
			return E_FAIL; // Returning failure
		}
	}

};

class dxBackground : public dxSprite { // Background class
	public:
		D3DXVECTOR2 m_Scaling; // Holding the scaling vector

		void adjust() // Adjust to screen
		{
			setAllImage();
			m_Scaling = D3DXVECTOR2(screen_width/m_sTexture->width,screen_height/m_sTexture->height);
		}

		dxBackground(LPDIRECT3DDEVICE9 pDevice,LPCSTR texture,DWORD flag):dxSprite(pDevice,0,0,flag) // Constructor
		{
			m_Scaling = D3DXVECTOR2(1,1);
			if(texture!=0)
			{
				m_sTexture->loadTex(texture,pDevice);
				adjust();
			}
		}

		HRESULT Render(){ // Optimized render function
			D3DXMATRIX Mat; // Our Matrix
			D3DXMatrixTransformation2D(&Mat, NULL, 0,&m_Scaling,0,0,0); // Scaling only
			startRender(flag); // Starting to render
			m_Sprite->SetTransform(&Mat); // Updating changes
			HRESULT r = Draw(); // Drawing the sprite
						// Set transform back to identity for sprite
			D3DXMatrixIdentity(&Mat);
			m_Sprite->SetTransform(&Mat);
			endRender(); // Ending to render

			return r;
	}

};


class dxMoveSprite : public dxSprite { // Moveable sprites class
	public:
		D3DXVECTOR2 m_RotationCenter; // Holding the rotation center
		D3DXVECTOR2 m_Translation; // Holding the position 
		D3DXVECTOR2 m_Movement; // Holding the movement vector 
		D3DXVECTOR2 m_Scaling; // Holding the scaling vector
		float m_Rotation; // Holding the rotation angle
		int frame, anim,isAnim; // Holding the frame number, and the animation status
		float spaceBetFrames; // Holding the pauses between frames
		DWORD lastFrame; // Last frame updated

	dxMoveSprite(LPDIRECT3DDEVICE9 pDevice, DWORD flag,float x,float y):dxSprite(pDevice,x,y,flag) // Constructor + Super
	{
		D3DXVECTOR2 Vec; // Holding temporarly the deafult vector (0,0)
		Vec.x = 0, Vec.y = 0; // Defining the vector to (0,0)
		m_RotationCenter = Vec, m_Translation = Vec, m_Movement = Vec,
			m_Scaling = D3DXVECTOR2(1,1), m_Rotation = 0.0f; // Set all vars to the thier deafult value
		frame = 0, spaceBetFrames = 0
			, anim = 0; // Going to the first frame,space between frames equals to 0, and the animation status is the first deafult one
		lastFrame = timeGetTime(); // Updating the last frame which is the first one, for now
		isAnim=false;
	}
	
	bool isColliding(dxMoveSprite* Other){
		float x1 = Other->m_Translation.x;
		float y1 = Other->m_Translation.y;
		float width1 = Other->m_SrcRect.right-Other->m_SrcRect.left; 
		float height1 = Other->m_SrcRect.bottom-Other->m_SrcRect.top; 
		width1*=Other->m_Scaling.x;
		height1*=Other->m_Scaling.y;
		float x2=m_Translation.x;
		float y2=m_Translation.y;
		float width2 = m_SrcRect.right-m_SrcRect.left; 
		float height2 = m_SrcRect.bottom-m_SrcRect.top; 
		width2*=m_Scaling.x;
		height2*=m_Scaling.y;
		return !(x1 > x2+width2 || x1+width1 < x2 || y1 > y2+height2 || y1+height1 < y2);
	}

	void anima() // Taking care of our animation
	{
		if(isAnim == true)
		{
			nextFrame();
		}
	}

	void nextFrame() // Move to next frame
	{
		long now = timeGetTime();

			if((now-lastFrame) > spaceBetFrames)
			{
				frame++;
				lastFrame = now;
			}
		}

	void updateFrame() // Updating frame
	{
		int rectWidth = m_SrcRect.right-m_SrcRect.left,
			rectHeight = m_SrcRect.bottom - m_SrcRect.top; // Holding the height and width of any frame
		m_SrcRect.left = frame * rectWidth;
		m_SrcRect.right = m_SrcRect.left + rectWidth;
		m_SrcRect.top = anim * (rectHeight+1) , m_SrcRect.bottom = m_SrcRect.top + rectHeight;
		if(m_SrcRect.right > m_sTexture->width)
			{
				m_SrcRect.left = 0, m_SrcRect.right = rectWidth, frame = 0;
			}
		}
	
	void move(float x, float y) // Move function
	{
		m_Movement = D3DXVECTOR2(x,y);
	}

	void Update() // Update function
	{
		updateFrame(); // Updating frame
		m_Translation += m_Movement; // Moving
	}

	void moveTo(float x,float y){
		m_Translation = D3DXVECTOR2(x,y);
	}

	HRESULT Render(){ // Render function
			D3DXMATRIX Mat; // Our Matrix
			D3DXMatrixTransformation2D(&Mat, NULL, 0,&m_Scaling,
			&m_RotationCenter, m_Rotation,&m_Translation); // Converting our vectors and floats to matrix - saved in Mat matrix
			startRender(flag); // Starting to render
			m_Sprite->SetTransform(&Mat); // Updating changes
			HRESULT r = Draw(); // Drawing the sprite
						// Set transform back to identity for sprite
			D3DXMatrixIdentity(&Mat);
			m_Sprite->SetTransform(&Mat);
			endRender(); // Ending to render
				return r;
	}
};