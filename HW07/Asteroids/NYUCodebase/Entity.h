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
#include "SpriteSheet.h"
#include "Matrix.h"

class Entity {
public:
	Entity();
	~Entity();

	// updating the entity
	virtual void Update(float elapsed);
	// rendering the entity
	virtual void Render();

	//building the matrix
	void buildMatrixAndSetUp();

	// checking collision
	//bool collidesWith(Entity* entity);

	// movement of entities
	//void jump();
	//void walkRight();
	//void walkLeft();
	//void idle();

	SpriteSheet sprite;

	// location
	float x = 0.0f;
	float y = 0.0f;
	float scale_x = 1.0f;
	float scale_y = 1.0f;
	float rotation = 0.0f;

	//float width = 0.2f;
	//float height = 0.2f;

	// movement
	float velocity_x = 0.0f;
	float velocity_y = 0.0f;
	float acceleration_x = 0.0f;
	float acceleration_y = 0.0f;
	float friction_x = 0.0f;
	float friction_y = 0.0f;
	float mass = 0.0f;

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

	// matrix operations
	Matrix matrix;
	Matrix inverse;
	Vector TL;
	Vector TR;
	Vector BL;
	Vector BR;
};