#pragma once

class Particle
{
public:
	Particle(void);
	~Particle(void);
	// the current position of this particle
	D3DXVECTOR3 m_vCurPos;
	// the direction and velocity of this particle
    D3DXVECTOR3 m_vCurVel;
	// the color of this particle
	D3DCOLOR    m_vColor;
	// is this particle alive?
	bool m_bAlive;
	long m_vBorn;
		float m_vCurRan;
};





//The particle class


Particle::Particle(void)
{
	m_vCurPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// set the default color
	m_vColor = D3DCOLOR_ARGB(0, 0, 0, 0);
	// this particle is created alive
	m_bAlive = true;
	timeBeginPeriod(1);
	m_vBorn = timeGetTime();
}

Particle::~Particle(void)
{

}
