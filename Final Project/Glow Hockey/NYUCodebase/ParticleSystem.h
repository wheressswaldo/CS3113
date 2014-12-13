//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "GlowHockey.h"
using namespace std;

class Particle {
public:
	Vector position;
	Vector velocity;
	float lifetime;

	// default setup for particles, all 0
	Particle() :lifetime(0.0f){
		velocity = Vector(0.0f, 0.0f, 0.0f);
		position = Vector(0.0f, 0.0f, 0.0f);
	};
};

class ParticleSystem {
public:
	// default setup for the particle system, 1.0 life, color white, vector of particles
	ParticleSystem(unsigned int particleCount) :maxLifetime(1.0f), red(1.0f), green(1.0f), blue(1.0f), particles(vector<Particle>(particleCount)){};
	ParticleSystem(){};
	~ParticleSystem(){};
	// updates the lifetime and position
	void Update(float elapsed);
	// draws the particles
	void Render();
	// resets particles
	void Reset(int direction);

	float maxLifetime;
	vector<Particle> particles;
	float red;
	float green;
	float blue;
};
