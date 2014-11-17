//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 5 - Sidescroller
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
#include "Bullet.h"
#include "Entity.h"
#include "Player.h"
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define MAX_BRICKS 200
#define MAX_ENEMIES 2
#define SPRITE_COUNT_X 14
#define SPRITE_COUNT_Y 8
#define TILE_SIZE 0.2f
using namespace std;

class SideScroller{
public:
	SideScroller();
	~SideScroller();

	void Init();
	void Update(float elapsed);
	void updateMainMenu(float elapsed);
	void updateGameLevel(float elapsed);
	void FixedUpdate();
	void Render();
	void renderMainMenu();
	void renderGameLevel();
	bool UpdateAndRender();

	void buildLevel();
	bool readHeader(ifstream& stream);
	bool readLayerData(ifstream& stream);
	bool readEntityData(ifstream& stream);
	void placeEntity(string& type, float placeX, float placeY);
	void RenderLevel();
	void shootBullet();
	bool isSolid(unsigned char tile);
	void worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY);
	float SideScroller::checkPointForGridCollisionX(float x, float y);
	float SideScroller::checkPointForGridCollisionY(float x, float y);
	void SideScroller::doLevelCollisionX(Entity* entity);
	void SideScroller::doLevelCollisionY(Entity* entity);
	void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);
	

private:
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	GLuint characterSpriteSheetTexture;
	GLuint brickSpriteSheetTexture;
	GLuint fontSheetTexture;
	SpriteSheet bulletSprite;

	float gravity_x;
	float gravity_y;

	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL };
	int state;

	float bulletTime;
	Bullet playerBullet;

	int brickIndex;
	Entity bricks[MAX_BRICKS];

	int enemyIndex;
	Entity enemies[MAX_ENEMIES];

	unsigned int mapWidth;
	unsigned int mapHeight;

	unsigned char** levelData;

	Player* player;
	vector<Entity*> entities;

	float enemySpawnTimer;

};
float lerp(float v0, float v1, float t);