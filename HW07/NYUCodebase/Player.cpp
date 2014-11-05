//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 7 - Asteroids
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Player.h"

Player::Player() {}
void Player::Update(float elapsed) {
	if (x < -1.33f)
		x = -1.33f;
	else if (x > 1.33f)
		x = 1.33f;
	if (y < -1.0f)
		y = -1.0f;
	else if (y > 1.0f)
		y = 1.0f;
}
void Player::Render() {
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	sprite.Draw(scale_x, scale_y);
	glPopMatrix();
}
