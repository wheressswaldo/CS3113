//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 3 - Space Invaders
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Player.h"

Player::Player(SpriteSheet sprite, float scale, float x, float y, float rotation, float speed, float direction_x)
	: Entity(sprite, scale, x, y, rotation, speed, direction_x)	 {}
void Player::Update(float elapsed) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_LEFT]) {
		direction_x = -speed;
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		direction_x = speed;
	}

	if ((!(x > 1.2) || direction_x < 0) && (!(x < -1.2) || direction_x > 0))
		x += direction_x * speed * elapsed;

	direction_x = 0;
}