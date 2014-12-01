//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 7 - Asteroids
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <math.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include "Entity.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

using namespace std;

class Asteroids{
public:
	Asteroids();
	~Asteroids();

	// setup
	void Init();

	// all updates
	void Update(float elapsed);
	void updateMainMenu(float elapsed);
	void updateGameLevel(float elapsed);
	void FixedUpdate();

	// all renders
	void Render();
	void renderMainMenu();
	void renderGameLevel();

	// update and render (elapsed)
	bool UpdateAndRender();

	// collision
	bool collision(Entity* entity1, Entity* entity2);

	// lerp, draw text, and random generation
	float lerp(float v0, float v1, float t);
	void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);
	float generateRandomNumber(float min, float max);

private:
	// general privates
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	// textures
	GLuint spriteSheetTexture;
	GLuint fontSheetTexture;

	// gravity
	float gravity_x;
	float gravity_y;

	// game states
	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL };
	int state;

	// vectors
	vector<Entity*> entities;

	// map stats
	unsigned int mapWidth;
	unsigned int mapHeight;
};