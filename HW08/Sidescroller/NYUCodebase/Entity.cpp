//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 5 - Sidescroller REMADE
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Entity.h"

Entity::Entity() {}
Entity::~Entity() {}

// draw the entity
void Entity::Render() {
	sprite.Draw(width, height, x, y, rotation);
}

// check collision with another entity
bool Entity::collidesWith(Entity* entity) {
	float entity1_top = y + height / 2;
	float entity1_bottom = y - height / 2;
	float entity1_right = x + width / 2;
	float entity1_left = x - width / 2;

	float entity2_top = entity->y + entity->height / 2;
	float entity2_bottom = entity->y - entity->height / 2;
	float entity2_right = entity->x + entity->width / 2;
	float entity2_left = entity->x - entity->width / 2;

	if (entity1_bottom > entity2_top)
		return false;
	if (entity1_top < entity2_bottom)
		return false;
	if (entity1_left > entity1_right)
		return false;
	if (entity1_right < entity2_left)
		return false;
		
	return true;
}

// jump, set jump boolean to true so entity can't jump infinitely
// set y velocity, stop collidingbottom
void Entity::jump() {
	collidedBottom = false;
	isJumping = true;
	velocity_y = 4.0f;
}

// walk right
void Entity::walkRight() {
	faceLeft = false;
	if (velocity_x < 0.0f)
		velocity_x = 0.0f;
	if (velocity_x < 2.0f)
		acceleration_x = 9.0f;
}

// walk left
void Entity::walkLeft() {
	faceLeft = true;
	if (velocity_x > 0.0f)
		velocity_x = 0.0f;
	if (velocity_x > -2.0f)
		acceleration_x = -9.0f;
}

// idle
void Entity::idle() {
	velocity_x = 0.0f;
	acceleration_x = 0.0f;
}