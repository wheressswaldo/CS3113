//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 5 - Sidescroller REMADE
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SpriteSheet{
public:
	SpriteSheet();
	SpriteSheet(GLuint textureID, unsigned int spriteX, unsigned int spriteY, unsigned int index);

	// draws whatever is passed in on the screen
	void Draw(float width, float height, float x, float y, float rotation);

	// owned variables
	GLuint textureID;
	unsigned int spriteX;
	unsigned int spriteY;
	unsigned int index;
};

// function for loading textures
GLuint LoadTexture(const char *image_path);