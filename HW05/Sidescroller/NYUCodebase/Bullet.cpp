//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 5 - Sidescroller
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Bullet.h"

Bullet::Bullet() {
	x = -2.0f;
	y = -2.0f;

	width = 0.025f;
	height = 0.025f;

	velocity_x = 3.0f;
}
void Bullet::Update(float elapsed) {
	x += velocity_x * elapsed;
}
void Bullet::Render() {
	if (visible) {
		sprite.Draw(width, height, x, y, rotation);
	}
}