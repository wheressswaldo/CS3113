#pragma once

#include "GlowHockey.h"

using namespace std;

class Particle {
public:
	Vector position;
	Vector velocity;
	float lifetime;
	Particle() :lifetime(0.0f){
		float temp = (float)rand() / (float)RAND_MAX - 0.5f;
		float temp2 = (float)rand() / (float)RAND_MAX - 0.5f;
		velocity = Vector(temp, temp2, 0.0f);
		position = Vector(0.0, 0.0f, 0.0f);
	};
};

class ParticleSystem {
public:
	ParticleSystem(unsigned int particleCount) :maxLifetime(1.0f), red(1.0f), green(1.0f), blue(1.0f), particles(vector<Particle>(particleCount)){};
	ParticleSystem(){};
	~ParticleSystem(){};
	void Update(float elapsed);
	void Render();
	Vector position;
	float maxLifetime;
	vector<Particle> particles;

	float red;
	float green;
	float blue;
};
