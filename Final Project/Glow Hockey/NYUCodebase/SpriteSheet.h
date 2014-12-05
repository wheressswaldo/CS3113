//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SpriteSheet{
public:
	SpriteSheet();
	SpriteSheet(GLuint textureID, float u, float v, float width, float height);
	void Draw(float scale_x, float scale_y);

	GLuint textureID;
	float u;
	float v;
	float width;
	float height;
};
GLuint LoadTexture(const char *image_path);