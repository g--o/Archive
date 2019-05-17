// Defining stuff
dxManager *dxMgr; // DirectX Manager
Object* skyBox; // Some mesh

  void loadModels(){ // Load some models
	  // Loading ...
	  dxMgr->createMeshObject("Resources\\Models\\skeleton.x"); // We can now use it as object 0 on the meshes vector
	  skyBox = dxMgr->makeMeshObject("Resources\\Skyboxes\\sky.x"); // We can now use the pointer skyBox
	  skyBox->Scale(5,5,5); // And do stuff as always
  }

  bool initDX(Application& app) // Inits  DirectX
  {
	dxMgr = new dxManager(app); // Creating a DirectX manager

	if(!dxMgr->init()) // Inits the DirectX manager (in case of failure - return false)
		return false;

	loadModels(); // Loading models

	return true;
  }

  void explode() // Explosion effect! 
  {
	  dxMgr->createEmitter(100,"Resources\\Particles\\explosion.png",D3DXVECTOR3(0,0,0),D3DXVECTOR3(0,0,0),RAND_ONLY,D3DCOLOR_RGBA(125,125,125,255));
  }

  void aScene() // A scene
  {
	  dxMgr->beginRender(D3DCOLOR_XRGB(0,0,0)); // Start to render our scene
	  dxMgr->SetSkybox(skyBox); // Set skybox
	  dxMgr->showText("Fps:"+toString(FPS)); // Printing whatever we want to the screen
	  dxMgr->run(); // Run the 'engine'
	  dxMgr->endRender(); // End to render our scene
  }

