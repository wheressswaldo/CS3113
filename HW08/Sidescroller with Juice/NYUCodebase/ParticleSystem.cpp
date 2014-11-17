//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 5 - Sidescroller REMADE
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ParticleSystem.h"
#include  <random>


ParticleSystem::ParticleSystem() {
	// set how many particles there are
	particles.resize(500);
	// set particle movement coefficient
	movement = -3.0f;
	// set direction
	faceLeft = true;

	// set all particles to a random lifetime to begin with
	for (int i = 0; i < particles.size(); i++) {
		particles[i].lifetime = ((float)rand() / (float)RAND_MAX);
	}
}

ParticleSystem::~ParticleSystem() {
}

void ParticleSystem::Update(float elapsed) {
	// check which direction player is facing
	if (faceLeft == true){
		for (int i = 0; i < particles.size(); i++) {
			// if left, particles fly to the right
			particles[i].velocity += movement * elapsed;
			particles[i].x += particles[i].velocity * elapsed;
			// increase lifetime
			particles[i].lifetime += elapsed;

			// if lifetime is fulfilled, reset to the system location
			if (particles[i].lifetime > 1) {
				particles[i].lifetime = 0.0f;
				particles[i].velocity = 2.0f;
				particles[i].x = x;
				particles[i].y = y;
			}
		}
	}
	else {
		for (int i = 0; i < particles.size(); i++) {
			// if right, particles fly to the left
			particles[i].velocity += movement * elapsed;
			particles[i].x -= particles[i].velocity * elapsed;
			// increase lifetime
			particles[i].lifetime += elapsed;

			// if lifetime is fulfilled, reset to system location
			if (particles[i].lifetime > 1) {
				particles[i].lifetime = 0.0f;
				particles[i].velocity = 2.0f;
				particles[i].x = x;
				particles[i].y = y;
			}
		}
	}
}

void ParticleSystem::Render() {
	// vector of the particle locations
	std::vector<float> particleLocs;
	// add the particle locations
	for (int i = 0; i < particles.size(); i++) {
		particleLocs.push_back(particles[i].x);
		particleLocs.push_back(particles[i].y);
	}

	// pass in location data
	glVertexPointer(2, GL_FLOAT, 0, particleLocs.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	// set how big the particles are
	glPointSize(6.0f);
	// draw the particles
	glDrawArrays(GL_POINTS, 0, particleLocs.size() / 2);
}