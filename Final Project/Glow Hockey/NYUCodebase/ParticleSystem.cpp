#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "GlowHockey.h"
#include "ParticleSystem.h"
#include  <random>

void ParticleSystem::Render(){
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vector<float> particleVertices;

	for (unsigned int i = 0; i < particles.size(); i++) {
		particleVertices.push_back(particles[i].position.x);
		particleVertices.push_back(particles[i].position.y);
	}

	vector<float> particleColors;
	for (unsigned int i = 0; i < particles.size(); i++) {
		float m = (particles[i].lifetime / maxLifetime);
		particleColors.push_back(lerp(red, red, m));
		particleColors.push_back(lerp(green, green, m));
		particleColors.push_back(lerp(blue, blue, m));
		particleColors.push_back(lerp(1.0f, 0.0f, m));
	}

	glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, particleColors.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glPointSize(5.0f);
	glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void ParticleSystem::Update(float elapsed){
	for (unsigned int i = 0; i < particles.size(); i++){
		particles[i].lifetime += elapsed;
		particles[i].position.x += particles[i].velocity.x*elapsed;
		particles[i].position.y += particles[i].velocity.y*elapsed;
	}
}