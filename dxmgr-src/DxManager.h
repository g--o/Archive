// Loading required stuff

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"dxerr.lib") 
#include<d3d9.h>
#include<d3dx9.h>
#include<vector>

#include "resourceManagerB.h" // Resource manager
#include "dxInput.h" // DirectInput
#include "dxMedia.h" // DirectMedia
#include "Object.h" // Mesh class
#include "Sprites.h" // Sprites
#include "ParticleManager.h" // Particles
#include "dxCamera.h" // Camera
#include "Text.h" // The text drawer
#include "Scene.h" // The Scene Handler

// The DirectX C++ Manager
class dxManager
{
private:
	D3DPRESENT_PARAMETERS d3dpp; // Holds the Direct3D Present Parameters
	std::vector<Scene*> vScenes; // Holds our scenes
	//resourceManager* resMgr; // Our resource manager

public:
	int SceneState; // Scene state
	dxInput* pInput; // DirectX Input
	LPDIRECT3D9             dxObject;			// the Direct3D Object
	LPDIRECT3DDEVICE9       dxDevice;			// the Direct3D Device
	IXAnimator*	xAnimator; // DirectX Animator
	ResourceManagerB<dxResource>* resMgr; // Our resource manager
	dxText* basicText;
	LPD3DXFONT defaultFont;
	Application& app;

	/**
		Constructor
	*/
	dxManager(Application& in_app): app(in_app)
	{
		resMgr = new ResourceManagerB<dxResource>();
		SceneState = 1;
		dxObject = NULL;
		xAnimator = NULL;
		dxDevice = NULL;
	}

	LPD3DXFONT createFont(LPCTSTR  Name, int h, int w, UINT style, bool italic )
	{
		LPD3DXFONT font;
		D3DXCreateFont( dxDevice, h, w, style, 0, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, Name, &font );
		return font;
	}

	void resetDevice(DWORD width, DWORD height) // Reset device
	{
		basicText->font->OnLostDevice();
		//Set backbuffer resolution
		screen_width = d3dpp.BackBufferWidth = width;
		screen_height = d3dpp.BackBufferHeight = height;
		//Reset device & settings
		dxDevice->Reset(&d3dpp);
		deviceSettings();
		basicText->font->OnResetDevice();
	}

	void showText(std::string text)
	{
		basicText->DisplayText(text);
	}

	bool resourceExists(URI path)
	{
		dxResource* tmp;

		try
		{
			tmp = resMgr->GetPtr(path);
		} catch(Exceptions::URINotFound e)
		{
			return false;
		}
		return true;
	}

	void createEmitter(int numParticles,std::string textureName, D3DXVECTOR3 position, D3DXVECTOR3 movement, int flag, D3DXCOLOR color)
	{
		if(!resourceExists(textureName))
			loadTexture(textureName);
		getCurScene()->dxParticles->createEmitter(dxDevice,numParticles,(dxTexture*)resMgr->GetPtr(textureName),position,movement,flag,color);
	}

	void deviceSettings() // Set the device settings
	{
		dxDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); // Setting the Culling
		dxDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); // Setting the Filling Mode to solid
		// dxDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); - optional WireFrame Filling Mode
		dxDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true); // Normalize
		dxDevice->SetRenderState( D3DRS_ZENABLE, true ); // Enabling the Z-Buffer
		dxDevice->SetRenderState( D3DRS_ZWRITEENABLE, true );
		// Create and turn on the lights for the scene
		createLights(); // Creating lights
	} 

	dxTexture* loadTexture(std::string path)
	{
		if(resourceExists(path))
			(dxTexture*)resMgr->GetPtr(path);
		dxTexture* tmp = new dxTexture(path);
		tmp->loadTex(path.c_str(),dxDevice);
		resMgr->Load(path,tmp);
		return (dxTexture*)resMgr->GetPtr(path);
	}

	dxMoveSprite* loadTexSprite(float x,float y,DWORD flag,std::string texturePath) // Load a textured sprite
	{
		dxMoveSprite* tmp = loadSprite(x,y,flag);
		if(resourceExists(texturePath))
			tmp->m_sTexture = (dxTexture*)resMgr->GetPtr(texturePath);
		else
			tmp->m_sTexture = loadTexture(texturePath);

		return tmp;
	}

	dxMoveSprite* loadSprite(float x,float y,DWORD flag) // Load a sprite
	{
		dxMoveSprite* tmp = new dxMoveSprite(dxDevice,flag,x,y);
		getCurScene()->addSprite(tmp);
		return tmp;
	}

	void unloadResource(URI path)
	{
		resMgr->Unload(path);
	}

	void render() // Render
	{
		getCurScene()->render();
	}
	
	void run() // Run as usual (render&update)
	{
		getCurScene()->run(dxDevice);
	}

	void Project2Dto3D(POINT& pos, D3DXVECTOR3* MousePos, D3DXVECTOR3* MouseDir) // Get mouse direction and location
	{
		D3DXMATRIX matProj;
		matProj = getCurScene()->dxCam->camera_Projection;
		D3DXVECTOR3 v;
		v.x =  ( ( ( 2.0f * pos.x ) / screen_width  ) - 1 ) / matProj._11;
		v.y = -( ( ( 2.0f * pos.y ) / screen_height ) - 1 ) / matProj._22;
		//v.x =  ( ( ( 2.0f * p.x ) / gEngine->GetScreenWidth()  ) - 1 ) / matProj._11;
		//v.y = -( ( ( 2.0f * p.y ) / gEngine->GetScreenHeight() ) - 1 ) / matProj._22;
		v.z =  1.0f;

		D3DXMATRIX matView = getCurScene()->dxCam->camera_view;

		D3DXMATRIX matViewInverse;

		D3DXMatrixInverse( &matViewInverse, NULL, &matView );

		// Transform the screen space pick ray into 3D space
		MouseDir->x  = v.x*matViewInverse._11 + v.y*matViewInverse._21 + v.z*matViewInverse._31;
		MouseDir->y  = v.x*matViewInverse._12 + v.y*matViewInverse._22 + v.z*matViewInverse._32;
		MouseDir->z  = v.x*matViewInverse._13 + v.y*matViewInverse._23 + v.z*matViewInverse._33;
		MousePos->x = matViewInverse._41;
		MousePos->y = matViewInverse._42;
		MousePos->z = matViewInverse._43;

		D3DXVec3Normalize(MouseDir, MouseDir);

	}

	D3DXVECTOR3 pickingOnAir(int x, int y) // setting item to mouse location
	{

		D3DXVECTOR3 rayOrigin,rayDir;
		// grab the projection matrix
		D3DXMATRIX matProj;
		dxDevice->GetTransform(D3DTS_PROJECTION,&matProj);

		// ray tracing to project a ray from the camera into the screen following the cursor
		D3DXVECTOR3 v;
		v.x = ( ( ( 2.0f * x ) / screen_width ) - 1 ) / matProj._11;
		v.y = -( ( ( 2.0f * y ) / screen_height ) - 1 ) / matProj._22;
		v.z = 1.0f;

		D3DXMATRIX m;

		// inverse of the view matrix
		D3DXMatrixInverse( &m, NULL,&getCurScene()->dxCam->camera_view ); 

		// Transform the screen space pick ray into 3D space
		rayDir.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
		rayDir.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
		rayDir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;
		rayOrigin.x = m._41;
		rayOrigin.y = m._42;
		rayOrigin.z = m._43;


		D3DXVECTOR3 vFloorNorm;
		vFloorNorm = D3DXVECTOR3(0, 1, 0);

		// distance from the ray origin to colliding with the floor

		float fDist = - (D3DXVec3Dot(&vFloorNorm,&rayOrigin)) / D3DXVec3Dot(&vFloorNorm,&rayDir);
		D3DXVECTOR3  m_vTravelPos;     
		// calculate mouse cursor in screen coords
		m_vTravelPos.x = rayOrigin.x + (rayDir.x*fDist);
		m_vTravelPos.y = rayOrigin.y + (rayDir.y*fDist);
		m_vTravelPos.z = rayOrigin.z + (rayDir.z*fDist);

		return m_vTravelPos;
	}

	bool isMouseHit(LPD3DXMESH g_pMesh) // Has hit?
	{
		D3DXVECTOR3 rayOrigin,rayDir;
		// Use inverse of matrix
		D3DXMATRIX matInverse;
		D3DXMatrixIdentity(&matInverse);
		// Transform ray origin and direction by inv matrix
		D3DXVECTOR3 rayObjOrigin,rayObjDirection;
		D3DXVec3TransformCoord(&rayObjOrigin,&rayOrigin,&matInverse);
		D3DXVec3TransformNormal(&rayObjDirection,&rayDir,&matInverse);
		D3DXVec3Normalize(&rayObjDirection,&rayObjDirection);
		BOOL hasHit;
		D3DXIntersect(g_pMesh, &rayObjOrigin, &rayObjDirection, &hasHit, NULL, NULL, NULL, NULL, NULL, NULL);

		return hasHit;
	}

	void createScene() // Create a scene
	{
		vScenes.push_back(new Scene());
	}

	
	Object* makeMeshObject(std::string path)
	{
		Object* n = new Object(dxDevice,xAnimator);
		n->setMesh(path);
		return n;
	}

	Object* makeObject()
	{
		return new Object(dxDevice, xAnimator);
	}

	Object* createMeshObject(std::string path) // Create a mesh in the current scene
	{
		Object* newMesh = makeMeshObject(path);
		getCurScene()->addObject(newMesh);
		return newMesh;
	}

	void deleteScene(int id) // Delete a scene
	{
		if(id>=0 && id<vScenes.size())
			vScenes.erase(vScenes.begin()+id);
	}

	Scene* getCurScene() // get the current scene
	{
		return getScene(SceneState-1);
	}

	Scene* getScene(int id) // get a scene
	{
		if(id>=0 && id<vScenes.size())
			return vScenes.at(id);
		return NULL;
	}

	void update() // update
	{
		pInput->update();
		getCurScene()->update(dxDevice);
	}

	/**
	Basic Lights settings function
	*/
	void createLights() // Creating Basic lights
	{
		dxDevice->SetRenderState(D3DRS_LIGHTING,false);
	}

	/**
		Set a skybox (3D sky)
	*/
	void SetSkybox(Object* SkyBox)
	{
		dxDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
		dxDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
		dxDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_CLAMP );
		dxDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_CLAMP );
		// Disable lighting
		dxDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
			 D3DXMATRIX matView, matViewSave, matWorld;
 
		// Remember the old view matrix
		dxDevice->GetTransform( D3DTS_VIEW, &matViewSave );

		// Alter the view matrix so its centred for the skybox
		// The -0.4f for the y value is just to lower the horizon slightly - requires experimentation
		matView = matViewSave;
		matView._41 = 0.0f;
		matView._42 = 1.0f;
		matView._43 = 6.0f;

		dxDevice->SetTransform( D3DTS_VIEW, &matView );

		// Set a default world matrix
		D3DXMatrixIdentity(&matWorld);
		dxDevice->SetTransform( D3DTS_WORLD, &matWorld);

		// ... Render our skybox as for an x file
		SkyBox->Render(SkyBox->generateWorldMatrix());
		// Restore the original view matrix
		dxDevice->SetTransform( D3DTS_VIEW, &matViewSave );
		deviceSettings();
	}

	/********************************************************************
	* init
	* Initializes DirectX
	********************************************************************/
	bool init()
	{

		if( (dxObject = Direct3DCreate9( D3D_SDK_VERSION )) == NULL) // Setting the DirectX Object
		{
			MessageBox(NULL, "Failed  to create the DirectX Object!", "Error", MB_OK);
			return false;
		}

		// Setting the Present Parameters
		ZeroMemory( &d3dpp, sizeof(d3dpp) );  
		d3dpp.Windowed = true;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.BackBufferCount  = 1;
		d3dpp.BackBufferHeight = screen_height;
		d3dpp.BackBufferWidth  = screen_width;
		d3dpp.hDeviceWindow    = wndHandle;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.EnableAutoDepthStencil = true;

		if( FAILED( dxObject->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, wndHandle,
										  D3DCREATE_HARDWARE_VERTEXPROCESSING,
										  &d3dpp, &dxDevice ) ) ) // Setting the DirectX Device
		{
			MessageBox(NULL, "Failed  to create the DirectX Device!", "Error", MB_OK);
			return false;
		}

		deviceSettings(); // Set up device & settings
		xAnimator = CreateXAnimator(dxDevice); // Setting it up
		pInput = new dxInput(wndHandle); // Set up input
		createScene(); // Because first scene is free!
		//Handle font
		defaultFont = createFont("arial", aH,aW,normal,false); 
		basicText = new dxText(defaultFont,D3DCOLOR_XRGB(255,255,255)); // Creating a deafult font
		basicText->setPos(10,10);

		return true;
	}

	/********************************************************************
	* beginRender
	* Prepares DirectX for rendering each frame
	********************************************************************/
	void beginRender(D3DCOLOR bgColor)
	{
		if( NULL == dxDevice )
			return;

		// Clear the backbuffer to a black color
		dxDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, bgColor, 1.0f, 0 );

		dxDevice->BeginScene(); // Beginning the scene
	}

	/********************************************************************
	* endRender
	* Called after rendering has completed. Presents the result to the
	* screen.
	********************************************************************/
	void endRender(void)
	{	
		dxDevice->EndScene(); // Ending the scene
		// Present the backbuffer contents to the display
		dxDevice->Present( NULL, NULL, NULL, NULL );
	}


	/********************************************************************
	* shutdown
	* Releases the DirectX interfaces
	********************************************************************/
	void shutdown()
	{
		if( dxDevice != NULL) 
		{
			dxDevice->Release();
			dxDevice = NULL;
		}
		if( dxObject != NULL)
		{
			dxObject->Release();
			dxObject = NULL;
		}
		SceneState = NULL;

		basicText->shutdown();

		vScenes.clear();
		delete xAnimator;
		pInput->cleanDirectInput();
		delete pInput;
		delete resMgr;
	}

};
