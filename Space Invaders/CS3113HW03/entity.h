//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 3 - Space Invaders
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "spritesheet.h"

class Entity {
public:
	~Entity();
	Entity(SpriteSheet sprite, float scale, float x, float y, float rotation, float speed, float score, float direction_x = 0, float direction_y = 0);

	const float getX();
	const float getY();
	virtual void Update(float elapsed);
	void Render();

	SpriteSheet sprite;

	float scale;
	float x;
	float y;
	float rotation;

	float speed;
	float direction_x;
	float direction_y;
	float score;
};