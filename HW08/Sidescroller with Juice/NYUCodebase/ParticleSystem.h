//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 5 - Sidescroller REMADE
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>

using namespace std;

struct Particle {
	float x;
	float y;
	float velocity;
	float lifetime;
};

class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem();
	void Update(float elapsed);
	void Render();

	float x;
	float y;
	float movement;

	bool faceLeft;
	std::vector<Particle> particles;
};
