//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 4 - Simple Screen Platformer
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Bullet.h"

Bullet::Bullet() {
	x = -2.0f;
	y = -2.0f;

	width = 0.05f;
	height = 0.05f;

	velocity_x = 3.0f;
}
void Bullet::Update(float elapsed) {
	x += velocity_x * elapsed;
}
void Bullet::Render() {
	if (visible) {
		sprite.Draw(scale, x, y, rotation);
	}
}