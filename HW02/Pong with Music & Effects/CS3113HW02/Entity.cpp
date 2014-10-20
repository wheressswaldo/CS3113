/////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS311 - Introduction to Game Programming
//		Assignment 2 - Pong
//
/////////////////////////////////////////////////////////////////////////

#include "Entity.h"

Entity::Entity(float x, float y, float w, float h, float r) {
	xPosition = x; 
	yPosition = y; 
	width = w; 
	height = h;
	rotation = r;
	xDirection = 0.0f;
	yDirection = 0.0f;
	score = 0;
}

Entity::~Entity() { }

//loads texture
void Entity::LoadTexture(const char *image_path){
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint tempTextureID;
	glGenTextures(1, &tempTextureID);
	glBindTexture(GL_TEXTURE_2D, tempTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	textureID = tempTextureID;
}

// draws texture
void Entity::DrawSprite(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(xPosition, yPosition, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = {0.0f, 0.0+height/600.0f, 0.0f, 0.0f, 0.0+width/800.0f, 0.0f, 0.0+width/800.0f, 0.0+height/600.0f};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}