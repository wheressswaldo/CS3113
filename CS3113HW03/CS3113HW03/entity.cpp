//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 3 - Space Invaders
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Entity.h"

Entity::~Entity() {}
Entity::Entity(SpriteSheet sprite, float scale, float x, float y, float rotation, float speed, float score, float direction_x, float direction_y)
: sprite(sprite), scale(scale), x(x), y(y), rotation(rotation), speed(speed), score(score), direction_x(direction_x), direction_y(direction_y) {}
const float Entity::getX() { return x; }
const float Entity::getY() { return y; }
void Entity::Update(float elapsed) {
	if ((!(x > 1.2) || direction_x < 0) && (!(x < -1.2) || direction_x > 0)) {
		x += direction_x * speed * elapsed;
	}
}
void Entity::Render() {
	sprite.Draw(scale, x, y, rotation);
}