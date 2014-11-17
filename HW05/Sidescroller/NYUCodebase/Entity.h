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
#include "SpriteSheet.h"

class Entity {
public:
	Entity();
	~Entity();
	
	// rendering the entity
	virtual void Render();

	// checking collision
	bool collidesWith(Entity* entity);

	// movement of entities
	void jump();
	void walkRight();
	void walkLeft();
	void idle();

	SpriteSheet sprite;

	// location
	float x = 0.0f;
	float y = 0.0f;
	float x_penetrated = 0.0f;
	float y_penetrated = 0.0f;
	float scale = 0.0f;
	float rotation = 0.0f;

	float width = 0.2f;
	float height = 0.2f;
	
	// movement
	float velocity_x = 0.0f;
	float velocity_y = 0.0f;
	float acceleration_x = 0.0f;
	float acceleration_y = 0.0f;
	float friction_x = 0.0f;
	float friction_y = 0.0f;
	float mass  = 0.0f;

	// collision
	bool isStatic = false;
	bool collidedTop = false;
	bool collidedBottom = false;
	bool collidedLeft = false;
	bool collidedRight = false;

	// types
	bool isPlayer = false;
	bool isEnemy = false;

	// status
	bool isDead = false;
	bool isJumping = false;
	bool faceLeft = false;
};