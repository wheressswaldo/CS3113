//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 3 - Space Invaders
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Bullet.h"

Bullet::Bullet() {
	//Make bullet start away from screen
	x = -2.0f;
	y = -2.0f;
}
Bullet::Bullet(SpriteSheet sprite, float scale, float x, float y, float rotation, float speed)
	: sprite(sprite), scale(scale), x(x), y(y), rotation(rotation), speed(speed), playerBullet(true) {}
void Bullet::Update(float elapsed) {
	y += speed * elapsed;
}
void Bullet::Render() {
	if (visible) {
		sprite.Draw(scale, x, y, rotation);
	}
}