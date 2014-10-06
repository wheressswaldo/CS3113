//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 4 - Simple Screen Platformer
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Player.h"

Player::Player() {}

void Player::Update(float elapsed) {

}
void Player::Render() {
	sprite.Draw(0.1, x, y, rotation);
}
void Player::jump() {
	collidedBottom = false;
	isJumping = true;
	velocity_y = 3.5f;
}
void Player::setWalkRight() {
	face_left = false;
	if (velocity_x < 0.0f)
		velocity_x = 0.0f;
	if (velocity_x < 2.0f)
		acceleration_x = 9.0f;
}
void Player::setWalkLeft() {
	face_left = true;
	if (velocity_x > 0.0f)
		velocity_x = 0.0f;
	if (velocity_x > -2.0f)
		acceleration_x = -9.0f;
}
void Player::setIdle() {
	velocity_x = 0.0f;
	acceleration_x = 0.0f;
}