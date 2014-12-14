//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		GlowHockey
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <GL/glew.h>
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
#include "ParticleSystem.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

using namespace std;

class GlowHockey{
public:
	GlowHockey();
	~GlowHockey();

	// setup
	void Init();

	// all updates
	void Update(float elapsed);
	void UpdateMainMenu(float elapsed);
	void UpdateGameLevel(float elapsed);
	void UpdateGameOver(float elapsed);
	void FixedUpdate();

	// all renders
	void Render();
	void RenderMainMenu();
	void RenderGameLevel();
	void RenderGameOver();

	// update and render (elapsed)
	bool UpdateAndRender();

	// collision
	bool collision(Entity* entity1, Entity* entity2);

	// lerp, draw text, and random generation
	void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);
	float generateRandomNumber(float min, float max);

private:
	// general privates
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	// textures
	GLuint glowSheet;
	GLuint fontSheetTexture;

	// gravity
	float gravity_x;
	float gravity_y;

	// game states
	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	int state;

	// vectors
	vector<Entity*> entities;
	
	// players and puck
	Entity* player1;
	Entity* player2;
	Entity* puck;

	// score!
	int player1Score;
	int player2Score;

	// tempXY
	float tempSpeedX = 0.0f;
	float tempSpeedY = 0.0f;

	// AI State
	bool aiOn;
	string aiState = "Tracking";
	float AIDifficulty;

	// stats on or off
	bool stats = "true";

	// particles
	ParticleSystem p;
	int particleCount;

	// menu control flow
	int menuControl;
	Entity* controlIndicator;

	// music
	Mix_Chunk *hitWall;
	Mix_Chunk *hitPuck;
	Mix_Chunk *moveMenu;
	Mix_Chunk *selectMenu;
	Mix_Chunk *cancelMenu;
	Mix_Chunk *goal;
	int volumeLevels;

};

float lerp(float v0, float v1, float t);