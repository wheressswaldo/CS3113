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
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
using namespace std;

class Asteroids{
public:
	Asteroids();
	~Asteroids();
	void Init();
	void Update(float elapsed);
	void updateMainMenu(float elapsed);
	void updateGameLevel(float elapsed);
	void FixedUpdate();
	void Render();
	void renderMainMenu();
	void renderGameLevel();
	bool UpdateAndRender();
	bool checkCollision(Entity* entity1, Entity* entity2);
	void shootBullet();
	void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);

private:
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	GLuint spriteSheetTexture;
	GLuint fontSheetTexture;
	SpriteSheet bulletSprite;
	
	float gravity_x;
	float gravity_y;

	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL };
	int state;

	float bulletTime;
	Bullet playerBullet;

	unsigned int mapWidth;
	unsigned int mapHeight;

	Player* player;
	vector<Entity*> entities;

	float enemySpawnTimer;
};
float lerp(float v0, float v1, float t);
float generateRandomNumber(float low, float high);