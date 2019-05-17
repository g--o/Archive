short width =  GetSystemMetrics (SM_CXSCREEN);
short height =  GetSystemMetrics (SM_CYSCREEN);
float cdepth = 25, cwidth = 9;
char GameState = 1;
LPD3DXFONT m_font;
D3DCOLOR fontColor = D3DCOLOR_ARGB(255,255,255,0);    
RECT rct;
D3DXVECTOR3 camera_upvector( 0.0f, 1.0f, 0.0f );
D3DXMATRIXA16 camera_view;
D3DXMATRIX camera_Projection;
LPDIRECT3DDEVICE9 Device; //will be our main DirectX device
LPDIRECT3D9 DXObject;
D3DXVECTOR3 position(0,40,400);
float speed = 0,speedY = 0 ;
dxKeyboard* Keyboard;
MeshManager* Car,* SkyBox,* Environment, *lamp, *grass;
float angle = 90;
D3DXVECTOR3 vP = D3DXVECTOR3(1,1,1);
float alpha = 0;
std::string txt = "";

template <class T>
std::string toString(const T & t)
{
    std::ostringstream oss; // create a stream
    oss << t;              // insert value to stream 
    return oss.str();      // return as a string
}


float Distance(const D3DXVECTOR3& point1,const D3DXVECTOR3& point2)
{
    float distance = sqrt( (point1.x - point2.x) * (point1.x - point2.x) +
                            (point1.y - point2.y) * (point1.y - point2.y) +
                            (point1.z - point2.z) * (point1.z - point2.z) );
    return distance;
}


bool CreateDevice(HWND Wind)
{
	DXObject = Direct3DCreate9(D3D_SDK_VERSION);
	if (DXObject==NULL)
	{
	return false;
	}

	D3DPRESENT_PARAMETERS PP;
	ZeroMemory( &PP, sizeof(PP) );

	PP.Windowed = false;
	PP.BackBufferCount=1;
	PP.BackBufferHeight=height;
	PP.BackBufferWidth=width; 
	PP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	PP.BackBufferFormat = D3DFMT_X8R8G8B8;
	PP.EnableAutoDepthStencil = TRUE;
	PP.AutoDepthStencilFormat = D3DFMT_D16;


	if(FAILED(DXObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Wind, D3DCREATE_HARDWARE_VERTEXPROCESSING, &PP, &Device))){
		MessageBox(Wind,"error","Error Creating DirectX Device",MB_OK);
		GameState = 0;
		return false;
}
	/*D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type       = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r  = 0.5f;
	light.Diffuse.g  = 0.5f;
	light.Diffuse.b  = 0.5f;
	light.Diffuse.a  = 0.5f;
	light.Range      = 100.0f;
	// Create a direction for our light - it must be normalized  
	D3DXVECTOR3 vecDir;
	vecDir = D3DXVECTOR3(-2.0f,-1.0f,0.0f);
	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
	*/
	D3DLIGHT9 light;
	D3DMATERIAL9 material;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_POINT;    // make the light type 'point light'
	light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	light.Specular = D3DXCOLOR(255.0f, 255.0f, 255.0f, 1.0f); 
	light.Position = D3DXVECTOR3(40,20,-20);
	light.Range = 1000.0f;    // a range of 100
	light.Attenuation0 = 1.0f;    // no constant inverse attenuation
	light.Attenuation1 = 0.0f;    // only .125 inverse attenuation
	light.Attenuation2 = 0.0f;    // no square inverse attenuation
	Device->SetLight(0, &light);
	Device->LightEnable(0, TRUE);
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Device->SetMaterial(&material);

	// Tell the device about the light and turn it on
	Device->SetLight( 0, &light );
	Device->LightEnable( 0, TRUE ); 

	return true;
}

void UpdateCamera()
{
	D3DXVECTOR3 camera_position( 0.0f+position.x,  Environment->GetHeightBelow(D3DXVECTOR3(position.x,position.y+15,position.z+100.0f),D3DXVECTOR3(0,-1,0))+45.0f,100.0f+position.z );
	D3DXVECTOR3 camera_lootat(position.x, position.y, position.z );
	D3DXMatrixLookAtLH(&camera_view, &camera_position, &camera_lootat, &camera_upvector);
	D3DXMatrixPerspectiveFovLH(&camera_Projection, D3DX_PI/4, 1, 1, 10000000);
	Device->SetTransform(D3DTS_VIEW, &(camera_view));
	Device->SetTransform(D3DTS_PROJECTION, &camera_Projection);
}

void handleTerrain(){
	//float heightAve = Environment->GetHeightBelow(D3DXVECTOR3(position.x,position.y-4,position.z),D3DXVECTOR3(0,-1,0));
	D3DXVECTOR3 frontPos = D3DXVECTOR3(position.x+4+(cdepth/2-2)*cos(D3DXToRadian(angle)),0,position.z+3-(cdepth/2-2)*sin(D3DXToRadian(angle)));
	D3DXVECTOR3 backPos = D3DXVECTOR3(position.x+4-(cdepth/2-2)*cos(D3DXToRadian(angle)),0,position.z+(cdepth/2-2)*sin(D3DXToRadian(angle)));
	float heightFront = Environment->GetHeightBelow(frontPos,D3DXVECTOR3(0,1,0));
	float heightBack = Environment->GetHeightBelow(backPos,D3DXVECTOR3(0,1,0));

	//vP=backPos;
	float heightMax = -min(heightFront, heightBack);
	float alpha = 0,beta = 0;
	if(position.y - 6 > heightMax)
		speedY+=-9.8/60;
	else {
		//float heightRight = Environment->GetHeightBelow(D3DXVECTOR3(position.x+4+(cwidth-1)/2*sin(D3DXToRadian(180-angle)),0,position.z+3+(cwidth-1)/2*cos(D3DXToRadian(180-angle))),D3DXVECTOR3(0,1,0));
		//float heightLeft = Environment->GetHeightBelow(D3DXVECTOR3(position.x+4-(cwidth-1)/2*sin(D3DXToRadian(180-angle)),0,position.z+3-(cwidth-1)/2*cos(D3DXToRadian(180-angle))),D3DXVECTOR3(0,1,0));
		alpha = asin((double)((heightBack-heightFront)/(Distance(frontPos,backPos))));
		//beta = asin((double)(heightRight-heightLeft)/cwidth);

		speedY=speed*sin(alpha);
		if(heightMax - position.y - 4 > 0.1)
			position.y+= heightMax - position.y - 4;
	}

	//speedY+=5*sin(alpha);
	Car->Rotate('z',beta);
	Car->Rotate('x',alpha);

	txt="Speed:"+toString((int)speed)+" Speed Y:"+toString((int)speedY)+" alpha:"+toString((int)D3DXToDegree(alpha));
}

void Updates()
{
	if(abs(speed) > 0){
		if(speed>0)
			speed-=0.003;
		if(speed<0)
			speed+=0.003;
	}
	speed-=speedY*sin(alpha);
	handleTerrain();
	position.z -= speed*sin(D3DXToRadian(angle));
	position.x += speed*cos(D3DXToRadian(angle));
	position.y += speedY;
	Environment->Scale(50,50,50);
	Environment->Rotate('y', D3DX_PI/2);
	Environment->Move(0,-8,-18);
	Car->Move(position.x,position.y,position.z);
	Car->Rotate('y',D3DXToRadian(angle-90));
	lamp->Scale(0.2,0.2,0.2);
	lamp->Move(/*40+*/vP.x,/*-7+*/vP.y,/*-20+*/vP.z);
	grass->Move(-90,-5,50);
	grass->Scale(10,10,10);

	if(position.x>38+cwidth){
		speed = 0;
		position.x = 38;
	}

	Car->UpdateBB();
	//lamp->UpdateBB();
	if(Car->CheckCol(&lamp->minBounds, &lamp->maxBounds)){
		lamp->Rotate('x', D3DXToRadian(90*(speed)/2*cos(angle)));
		//vP.z -= 20*speed*sin(D3DXToRadian(angle));
		speed -= 200/100*0.05;
		D3DXMATRIX Bla;
		D3DXMatrixRotationZ(&Bla,D3DXToRadian(90*(speed)/5*sin(angle)));
		lamp->RotMatX *= Bla;
		//vP.x += 20*speed*cos(D3DXToRadian(angle));
	}
}



void handleInput(){
	Keyboard->detect_input();

	if(Keyboard->isPressed(DIK_UP))
		speed+=0.05;
	if(Keyboard->isPressed(DIK_DOWN))
		speed-=0.05;
	if(Keyboard->isPressed(DIK_RIGHT))
		angle += 2;
	if(Keyboard->isPressed(DIK_LEFT))
		angle -= 2;
	if(Keyboard->isPressed(DIK_ESCAPE))
		GameState=0;
}

 void DisplaySomeText()
{
	rct.left=2;
	rct.right=width-rct.left;
	rct.top=5;
	rct.bottom=height-rct.top;
	D3DXCreateFont(Device, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );

}

 void SetSkybox()
 {
	Device->SetRenderState( D3DRS_ZENABLE, FALSE );
	Device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	Device->SetSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_CLAMP );
	Device->SetSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_CLAMP );
	// Disable lighting
	Device->SetRenderState( D3DRS_LIGHTING, FALSE );
		 D3DXMATRIX matView, matViewSave, matWorld;
 
	// Remember the old view matrix
	Device->GetTransform( D3DTS_VIEW, &matViewSave );

	// Alter the view matrix so its centred for the skybox
	// The -0.4f for the y value is just to lower the horizon slightly - requires experimentation
	matView = matViewSave;
	matView._41 = 0.0f;
	matView._42 = 1.0f;
	matView._43 = 6.0f;

	Device->SetTransform( D3DTS_VIEW, &matView );

	// Set a default world matrix
	D3DXMatrixIdentity(&matWorld);
	Device->SetTransform( D3DTS_WORLD, &matWorld);

	// ... Render our skybox as for an x file
	SkyBox->Render();
	// Restore the original view matrix
	Device->SetTransform( D3DTS_VIEW, &matViewSave );
	Device->SetRenderState( D3DRS_ZENABLE, TRUE );
	Device->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	Device->SetRenderState( D3DRS_LIGHTING, TRUE );
 }

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
		GameState = 0;
	}
}

void DrawScene()
{
	handleInput();
	UpdateCamera();
	Device->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 255, 230), 1.0f, 0);
	Device->BeginScene();
	SetSkybox();
	m_font->DrawText(NULL, txt.c_str(), -1, &rct, 0, fontColor );
	Environment->Render();
	Car->Render();
	lamp->Render();
	grass->Render();
	Device->EndScene();
	Device->Present(NULL, NULL, NULL, NULL);
}

void cleanUp(){
	m_font->Release();
	delete Keyboard;
	cleanDirectInput();
	Car->Shutdown();
	SkyBox->Shutdown();
	Environment->Shutdown();
	lamp->Shutdown();
	grass->Shutdown();
	ShutMeshManager();
	Device->Release();
	DXObject->Release();
}