//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 4 - Simple Screen Platformer
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <math.h>
#include <string>
#include <algorithm>
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
using namespace std;

class platformer{
public:
	platformer();
	~platformer();

	void init();
	void shootPlayerBullet();
	void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);
	void update(float elapsed);
	void updateMainMenu(float elapsed);
	void updateGameLevel(float elapsed);
	void fixedUpdate();
	void render();
	void renderMainMenu();
	void renderGameLevel();
	bool updateAndRender();

private:
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	GLuint charactersheet;
	GLuint enemysheet;
	GLuint mapsheet;
	GLuint fontSheetTexture;

	float gravity_x;
	float gravity_y;

	enum GameState {STATE_MAIN_MENU, STATE_GAME_LEVEL};
	int state;

	Bullet playerBullet;
	int tileIndex;
	Entity tiles[200];
	int enemyIndex;
	Entity enemies[4];
	Player* player;
	vector<Entity*> entities;

	float enemySpawnTimer;
	int score;

};
float lerp(float v0, float v1, float t);