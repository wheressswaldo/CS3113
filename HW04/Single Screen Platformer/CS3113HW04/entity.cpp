//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 4 - Simple Screen Platformer
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Entity.h"

Entity::Entity() {
	x = 0.0f;
	y = 0.0f;
	width = 0.2f;
	height = 0.2f;
	rotation = 0.0f;
	scale = 0.0f;

	velocity_x = 0.0f;
	velocity_y = 0.0f;
	acceleration_x = 0.0f;
	acceleration_y = 0.0f;
	friction_x = 0.0f;
	friction_y = 0.0f;
	mass = 0.0f;
	isStatic = false;
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
	isJumping = false;
}
Entity::~Entity() {}
void Entity::Render() {
	sprite.Draw(scale, x, y, rotation);
}
bool Entity::collidesWith(Entity* entity) {
	float e1_y_max = y + height / 2;
	float e1_y_min = y - height / 2;
	float e1_x_max = x + width / 2;
	float e1_x_min = x - width / 2;

	float e2_y_max = entity->y + entity->height / 2;
	float e2_y_min = entity->y - entity->height / 2;
	float e2_x_max = entity->x + entity->width / 2;
	float e2_x_min = entity->x - entity->width / 2;

	if (e1_y_min > e2_y_max)
		return false;
	if (e1_y_max < e2_y_min)
		return false;
	if (e1_x_min > e2_x_max)
		return false;
	if (e1_x_max < e2_x_min)
		return false;

	return true;
}
