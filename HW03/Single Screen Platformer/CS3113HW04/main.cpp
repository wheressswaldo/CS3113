//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 4 - Simple Screen Platformer
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "platformer.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	platformer app;
	while (!app.updateAndRender()) {}
	return 0;
}