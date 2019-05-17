
#pragma once

#include <vector>
#include "dxCamera.h"
#include "Object.h"
#include "Sprites.h"
#include "ParticleManager.h"

class Scene // Scene class
{
public:
	dxCamera* dxCam; // Camera
	std::vector<Object*> vObjects; // Objects
	std::vector<dxMoveSprite*> vSprites; // Sprites
	particleManager* dxParticles; // particle manager


	void addObject(Object* o) // Add a mesh
	{
		vObjects.push_back(o);
	}

	void removeObject(int id) // Remove a mesh
	{
		vObjects.erase(vObjects.begin()+id);
	}

	void addSprite(dxMoveSprite* sprite)
	{
		vSprites.push_back(sprite);
	}

	void removeSprite(int id)
	{
		vSprites.erase(vSprites.begin()+id);
	}

	void run(LPDIRECT3DDEVICE9 dxDevice)
	{
		//Load identity matrix
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);
		dxDevice->SetTransform(D3DTS_WORLD, &mat);

		dxParticles->update();
		dxParticles->doomsDay();
		dxParticles->render();

		for(int i=0;i<vObjects.size(); i++) // Update each mesh individually
		{
			Object* mesh = vObjects.at(i);
			mesh->Run();
		}

		dxCam->createCamera(1,1000,dxDevice);
	}

	void update(LPDIRECT3DDEVICE9 dxDevice) // Update
	{
		// Particles update
		dxParticles->update();
		dxParticles->doomsDay();

		for(int i=0;i<vObjects.size(); i++) // Update each mesh individually
		{
			Object* mesh = vObjects.at(i);
			mesh->update();
		}

		for(int i=0;i<vSprites.size(); i++) // Update each mesh individually
		{
			dxMoveSprite* spr = vSprites.at(i);
			if(spr->alive)
				spr->Update();
		}

		dxCam->createCamera(1,1000,dxDevice);
	}

	void render() // Render
	{
		for(int i=0;i<vObjects.size(); i++) // Render each mesh individually
		{
			Object* mesh = vObjects.at(i);
			mesh->Render(mesh->generateWorldMatrix());
		}

		for(int i=0;i<vSprites.size(); i++) // Update each mesh individually
		{
			dxMoveSprite* spr = vSprites.at(i);
			if(spr->alive)
			{
				if(FAILED(spr->Render()))
					std::printf("Couldn't render sprite!");
			}
		}

		dxParticles->render();
	}

	Scene() //Create a camera
	{
		dxCam = new dxCamera(); // Creating a camera manager
		dxParticles = new particleManager();
		dxParticles->init();
	}

	~Scene() // Clean up
	{
		dxParticles->shutdown();
		vObjects.clear();
		delete dxCam;
	}

};
