/////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS311 - Introduction to Game Programming
//		Assignment 2 - Pong
//
/////////////////////////////////////////////////////////////////////////

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity{
public:
	Entity(float x, float y, float w, float h, float r);
	~Entity();

	void LoadTexture(const char *image_path);
	void DrawSprite();
	
	GLuint textureID;
	float xPosition, yPosition, width, height, rotation;
	// xDirection and yDirection is a combination of both speed and direction
	float xDirection;
	float yDirection;
	int score;
};