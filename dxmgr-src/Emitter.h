#pragma once

#include "Particle.h"

#define ADD_RAND 0
#define RAND_ONLY 1
#define NORMAL 2
#define CUSTOM 3

class Particle;

class Emitter
{
	// setup the vertex structure for the pointsprites
	struct CUSTOMVERTEX
	{
		D3DXVECTOR3 psPosition;
		D3DCOLOR color;
	};

	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

public:
	Emitter(void);
	Emitter(LPDIRECT3DDEVICE9 pDevice);
	~Emitter(void);

	// add a texture to this emitter
	bool addTexture(dxTexture* texture);

	// set the number of particles and size the vector
	void setNumParticles(int nParticles);

	// init the particles and set the position of the emitter
	void initParticles(D3DXVECTOR3 position, D3DXVECTOR3 movement,int flag, D3DCOLOR color, D3DXVECTOR3* velSetter()=0);
	void initParticles(D3DXVECTOR3 (*position_function)(), D3DCOLOR color,D3DXVECTOR3 (*callback_setter)());
	// update all the particles in this emitter
	void update(void);
	// render the particles in this emitter
	void render();
	void checkAlive();
	void setSpeRand();
	void moveTo(D3DXVECTOR3 Target);
	// inline functions
	inline bool getAlive(void) { return m_bAlive; }
	inline void kill(){ m_bAlive = false; }
	// inline function that converts a float to a DWORD value
	inline DWORD FLOAT_TO_DWORD( FLOAT f ) { return *((DWORD*)&f); }

private:
	LPDIRECT3DDEVICE9 emitterDevice;

	// the current position of this particle
	D3DXVECTOR3 m_vCurPos;
	// the direction and velocity of this particle
    D3DXVECTOR3 m_vCurVel;

	// vertex buffer to hold the pointsprites
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
	// the texture that will be applied to each pointsprite
	dxTexture*      pTexture;

	Particle *m_particles;

	int numParticles;

	bool m_bAlive;



	// private functions
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(unsigned int size, DWORD usage, DWORD fvf);
};




	// setup the vertex structure for the pointsprites
	struct CUSTOMVERTEX
	{
		D3DXVECTOR3 psPosition;
		D3DCOLOR color;
	};

	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)


// inline functions
// inline function that converts a float to a DWORD value
inline DWORD FLOAT_TO_DWORD( FLOAT f ) { return *((DWORD*)&f); }

Emitter::Emitter(void)
{
	m_bAlive=true;
	emitterDevice = NULL;
}

Emitter::Emitter(LPDIRECT3DDEVICE9 pDevice)
{
	emitterDevice = pDevice;
	m_bAlive=true;
	// set the default position for this emitter
	m_vCurPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// set the default velocity and direction
     m_vCurVel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

Emitter::~Emitter(void)
{
	// remove the array of particles
	if (m_particles)
		delete m_particles;

	// release the vertex buffer that was used
	if (pVertexBuffer)
		pVertexBuffer->Release();
}

bool Emitter::addTexture(dxTexture* texture)
{
	
	pTexture = texture;
	emitterDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	emitterDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return true;
}

void Emitter::setNumParticles(int nParticles)
{ 
	numParticles = nParticles; 

	m_particles = new Particle[numParticles];
}

void Emitter::initParticles(D3DXVECTOR3 position, D3DXVECTOR3 movement,int flag, D3DCOLOR color, D3DXVECTOR3* velSetter())
{
	pVertexBuffer = createVertexBuffer(numParticles * sizeof(CUSTOMVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, D3DFVF_CUSTOMVERTEX);

	for (int i=0; i<numParticles; i++)
	{
		m_particles[i].m_bAlive = true;
		m_particles[i].m_vColor = color;
		m_particles[i].m_vCurPos = position;
		
		if(flag == NORMAL)
		{
			m_particles[i].m_vCurVel = movement;
		}
		else
		{
		// generate a random value for each part of the direction/velocity vector
		float vecX = ((float)rand() / RAND_MAX)-0.5;
		float vecY = ((float)rand() / RAND_MAX)-0.5;
		float vecZ = ((float)rand() / RAND_MAX)-0.5;

			if(flag == ADD_RAND)
			{
				m_particles[i].m_vCurVel = D3DXVECTOR3(vecX,vecY,vecZ)+movement;
			}
			else if(flag == RAND_ONLY)
			{
				m_particles[i].m_vCurVel = D3DXVECTOR3(vecX,vecY,vecZ);
			}
			else
			{
				m_particles[i].m_vCurVel = *velSetter();
			}
		}

	}
}


void Emitter::initParticles(D3DXVECTOR3 (*position_function)(), D3DCOLOR color,D3DXVECTOR3 (*callback_setter)())
{
	pVertexBuffer = createVertexBuffer(numParticles * sizeof(CUSTOMVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, D3DFVF_CUSTOMVERTEX);

	for (int i=0; i<numParticles; i++)
	{
		m_particles[i].m_bAlive = true;
		m_particles[i].m_vColor = color;
		m_particles[i].m_vCurPos = position_function();

		m_particles[i].m_vCurVel = callback_setter();
	}
}


/**********************************************************************
* createVertexBuffer
**********************************************************************/
LPDIRECT3DVERTEXBUFFER9 Emitter::createVertexBuffer(unsigned int size, DWORD usage, DWORD fvf)
{
	LPDIRECT3DVERTEXBUFFER9 tempBuffer;

	emitterDevice->CreateVertexBuffer(size, usage, fvf, D3DPOOL_DEFAULT, &tempBuffer, NULL);

	if (tempBuffer)
		return tempBuffer;

	return NULL;
}

void Emitter::checkAlive()
{
	int numOfdead = 0;

	 long new_time = timeGetTime();
for (int i=0; i<numParticles; i++)
	{
	if((new_time - m_particles[i].m_vBorn) > 5000)
		{
			m_particles[i].m_bAlive = false;
			numOfdead++;
		}
	}
		if(numOfdead == numParticles)
	{
		m_bAlive = false;
	}

}

void Emitter::setSpeRand()
{
	for (int i=0; i<numParticles; i++)
	{
		if(m_particles[i].m_bAlive == true)
		{
			m_particles[i].m_vCurRan = rand()%100+1;
		}
	}
}

void Emitter::moveTo(D3DXVECTOR3 Target)
{
// loop through and update the position of the particles
for (int i=0; i<numParticles; i++)
	{
		if(m_particles[i].m_bAlive == true)
		{
			// generate a random value for each part of the direction/velocity vector
			m_particles[i].m_vCurVel = Target-m_particles[i].m_vCurPos;
			m_particles[i].m_vCurVel /= m_particles[i].m_vCurRan;
		}
	}
}

void Emitter::update(void)
{
	// loop through and update the position of the particles
for (int i=0; i<numParticles; i++)
	{
		if(m_particles[i].m_bAlive == true)
		{
			m_particles[i].m_vCurPos += m_particles[i].m_vCurVel;
		}
	}
}

void Emitter::render()
{
	emitterDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

    emitterDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    emitterDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// enable pointsprite render states

	// turn on pointsprites
    emitterDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, TRUE );
	// enable scaling
    emitterDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  TRUE );
	// the point size to use when the vertex does not include this information
    emitterDevice->SetRenderState( D3DRS_POINTSIZE,     FLOAT_TO_DWORD(1.0f) );
	// the minimum size of the points
    emitterDevice->SetRenderState( D3DRS_POINTSIZE_MIN, FLOAT_TO_DWORD(1.0f) );
	// these three renderstates control the scaling of the pointsprite
    emitterDevice->SetRenderState( D3DRS_POINTSCALE_A,  FLOAT_TO_DWORD(0.0f) );
    emitterDevice->SetRenderState( D3DRS_POINTSCALE_B,  FLOAT_TO_DWORD(0.0f) );
    emitterDevice->SetRenderState( D3DRS_POINTSCALE_C,  FLOAT_TO_DWORD(1.0f) );

	// Lock the vertex buffer, and set up our point sprites in accordance with 
	// our particles that we're keeping track of in our application.
	CUSTOMVERTEX *pPointVertices;
	pVertexBuffer->Lock( 0, numParticles * sizeof(CUSTOMVERTEX),
		                   (void**)&pPointVertices, D3DLOCK_DISCARD );
	
	for( int i = 0; i < numParticles; ++i )
    {
		if(m_particles[i].m_bAlive == true)
		{
		  pPointVertices->psPosition = m_particles[i].m_vCurPos;
		  pPointVertices->color = m_particles[i].m_vColor;
		  pPointVertices++;
		}
    }
	
    pVertexBuffer->Unlock();

	// draw the point sprites
	emitterDevice->SetTexture( 0, pTexture->m_Texture );
    emitterDevice->SetStreamSource( 0, pVertexBuffer, 0, sizeof(CUSTOMVERTEX) );
    emitterDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	emitterDevice->DrawPrimitive( D3DPT_POINTLIST, 0, numParticles );

    // set the render states back
	emitterDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
    emitterDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    emitterDevice->SetRenderState( D3DRS_POINTSPRITEENABLE, FALSE );
    emitterDevice->SetRenderState( D3DRS_POINTSCALEENABLE,  FALSE );
}





