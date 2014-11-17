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
#include <SDL_mixer.h>
#include <vector>
#include <math.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Entity.h"
#include "PerlinNoise.h"
#include "ParticleSystem.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

#define SPRITE_COUNT_X 14
#define SPRITE_COUNT_Y 8
#define TILE_SIZE 0.2f

using namespace std;

class SideScroller{
public:
	SideScroller();
	~SideScroller();

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

	//level building
	void buildLevel();
	bool readHeader(ifstream& stream);
	bool readLayerData(ifstream& stream);
	void loadLevel();

	// tile collision
	bool isSolid(unsigned char tile);
	void worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY);
	void SideScroller::collisionX(Entity* entity);
	void SideScroller::collisionY(Entity* entity);

	// lerp and drawing text
	void DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a);
	float lerp(float v0, float v1, float t);

private:
	// general privates
	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

	// textures
	GLuint characterSpriteSheetTexture;
	GLuint layoutSpriteSheetTexture;
	GLuint fontSheetTexture;

	// gravity
	float gravity_x;
	float gravity_y;

	// game states
	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL };
	int state;

	// vectors
	vector<Entity*> entities;
	vector<float> vertexData;
	vector<float> texCoordData;

	// map stats
	unsigned int mapWidth;
	unsigned int mapHeight;
	unsigned char** levelData;

	// effects
	float perlinValue;

	// timer to control when it shakes
	float screenShake = 0;

	// how strong the shake is
	float screenShakeValue = 0;
	float screenShakeSpeed = 100;
	float screenShakeIntensity = 0.08;

	// check if its shaking to only shake when jumping
	bool isShaking;

	// particle system
	ParticleSystem testParticleSystem = ParticleSystem();
};