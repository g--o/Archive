
#pragma once

#include "Emitter.h"

// forward class declarations
class Particle;
class Emitter;

class particleManager
{
public:
	particleManager(void);
	~particleManager(void);

	bool init(void);
	void shutdown(void);
	void killAll(void);
	bool createEmitter(LPDIRECT3DDEVICE9 pDevice, int numParticles, dxTexture* texture, D3DXVECTOR3 position, D3DXVECTOR3 movement,int flag, D3DCOLOR color);
	bool createEmitter(LPDIRECT3DDEVICE9 pDevice, int numParticles, dxTexture* textureName,D3DXVECTOR3 (*position_function)(), D3DCOLOR color, D3DXVECTOR3 (*callback_function)());
	void removeEmitter(int emitterNum);
	void removeEmitter(Emitter *which);
	void doomsDay();
	void render();
	void update(void);
	void render(LPDIRECT3DDEVICE9 pDevice);

	std::vector <Emitter*> emitters;
};



//The particle manager class


particleManager::particleManager(void)
{
	emitters.clear();
}

particleManager::~particleManager(void)
{
	emitters.clear();
}

bool particleManager::init(void)
{
	// clear the emitters
	emitters.clear();

	return true;
}

void particleManager::shutdown(void)
{
	// clear out the emitters vector
	emitters.clear();
	delete this;
}

bool particleManager::createEmitter(LPDIRECT3DDEVICE9 pDevice, int numParticles, dxTexture* texture, D3DXVECTOR3 position, D3DXVECTOR3 movement,int flag, D3DCOLOR color)
{
	// create a new emitter
	Emitter *tempEmitter = new Emitter(pDevice);

	// load in the texture
	if (!tempEmitter->addTexture(texture))
	{
		MessageBox(NULL,"Couldn't set textures","Error",NULL);
		return false;
	}

	// set the number of particles
	tempEmitter->setNumParticles(numParticles);

	tempEmitter->initParticles(position, movement,flag, color);

	// add this emitter to the vector
	emitters.push_back(tempEmitter);

	return true;
}

bool particleManager::createEmitter(LPDIRECT3DDEVICE9 pDevice, int numParticles, dxTexture* textureName,D3DXVECTOR3 (*position_function)(), D3DCOLOR color, D3DXVECTOR3 (*callback_function)())
{
		// create a new emitter
	Emitter *tempEmitter = new Emitter(pDevice);

	// load in the texture
	if (!tempEmitter->addTexture(textureName))
	{
					MessageBox(NULL,"","",NULL);
		return false;
	}

	// set the number of particles
	tempEmitter->setNumParticles(numParticles);

	tempEmitter->initParticles(position_function, color, callback_function);

	// add this emitter to the vector
	emitters.push_back(tempEmitter);

	return true;
}

void particleManager::update(void)
{
	for (unsigned int i=0; i<emitters.size(); i++)
	{
		// check whether this emitter is active
		if (emitters[i]->getAlive())
		{
			// if so, then update it
			emitters[i]->update();
		}
		else
		{
			emitters.erase(emitters.begin()+i); // Deleting emitter
		}
	}
}

void particleManager::doomsDay()
{
	for (int i=0; i<emitters.size(); i++)
	{
		// check whether this emitter is active
		if (emitters[i]->getAlive())
		{
			// if so, then update it
			emitters[i]->checkAlive();
		}
	}
}

void particleManager::render()
{
	for (unsigned int i=0; i<emitters.size(); i++)
	{
		// check whether this emitter is active
		if (emitters[i]->getAlive())
		{
			// if so, render this emitter
			emitters[i]->render();
		}
		else
		{
			emitters.erase(emitters.begin()+i); // Deleting emitter
		}
	}
}

void particleManager::killAll(void)
{
	for (int i=0; i<emitters.size(); i++)
	{
		emitters[i]->kill();
	}
}