//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 3 - Space Invaders
//		Notes:
//			- Code on enemy shooting is hard coded using random number generation, maybe base it on time elapsed?
//			- Inefficient object pools, separate pools for player bullets and enemy bullets, maybe can combine
//			- More states 
//			- Make game keep going until you die?
//			- Bullet graphics got messed up somewhere, will fix if time
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "spaceinvaders.h"

SDL_Window* displayWindow;
int main(int argc, char *argv[])
{
	SpaceInvaders app;
	while (!app.UpdateAndRender()) {}
	return 0;
}