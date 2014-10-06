//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 3 - Space Invaders
//		Notes:
//			- Code on enemy shooting is hard coded using random number generation, maybe base it on time elapsed?
//			- Animations on bullet deaths
//			- Keeps track of high score
//			- EXTRA IF TIME: More states, more levels
//			- EXTRA IF TIME: Make game keep going until you die instead of just ending abruptly
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