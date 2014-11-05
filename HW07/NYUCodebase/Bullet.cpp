//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 7 - Asteroids
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Bullet.h"

Bullet::Bullet() {
	//Make bullet start away from screen
	x = -2.0f;
	y = -2.0f;
	scale_x = 0.2f;
	scale_y = 0.2f;
	velocity_x = 3.0f;
	velocity_y = 3.0f;
}
void Bullet::Update(float elapsed) {
	x += velocity_x * sin(rotation) * elapsed;
	y += velocity_y * -cos(rotation) * elapsed;
}
void Bullet::Render() {
	if (visible) {
		buildMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixf(matrix.ml);
		sprite.Draw(scale_x, scale_y);
		glPopMatrix();
	}
}