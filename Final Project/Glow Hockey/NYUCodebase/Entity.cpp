//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Entity.h"

Entity::Entity() {}
Entity::~Entity() {}

// update
void Entity::Update(float elapsed) {
	// so stuff doesn't fly off the screen
	// adjustments
	if (x < -1.4f || x >1.4f)
		velocity_x *= -1;
	if (y < -1.0f || y > 1.0f)
		velocity_y *= -1;

	// adjustments
	if (isPlayer1){
		if (x < -0.46f)
			x = -0.46f;
		else if (x > 0.46f)
			x = 0.46f;
		if (y < -0.0f)
			y = -0.0f;
		else if (y > 0.715f)
			y = 0.715f;
	}

	if (isPlayer2){
		if (x < -0.46f)
			x = -0.46f;
		else if (x > 0.46f)
			x = 0.46f;
		if (y < -0.715f)
			y = -0.715f;
		else if (y > 0.0f)
			y = 0.0f;
	}
}

// draw the entity
void Entity::Render() {
	buildMatrixAndSetUp();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	if (glowing){
		glow.Draw(scale_x, scale_y);
	}
	sprite.Draw(scale_x, scale_y);
	glPopMatrix();
}

// build the matrix and setup up all matrix operations
void Entity::buildMatrixAndSetUp() {
	Matrix translateMatrix;
	Matrix rotateMatrix;
	Matrix scaleMatrix;
	matrix.identity();

	// move the matrix
	translateMatrix.identity();
	// 0	0	 0
	// 0	0	 0
	// x    y    1
	translateMatrix.m[3][0] = x;
	translateMatrix.m[3][1] = y;
	translateMatrix.m[3][2] = 1;

	// rotate the matrix
	rotateMatrix.identity();
	//  cos (rotation)	sin(rotation)
	// -sin (rotation)	cos(rotation)
	rotateMatrix.m[0][0] = cos(rotation); 	rotateMatrix.m[0][1] = sin(rotation);
	rotateMatrix.m[1][0] = -sin(rotation);	rotateMatrix.m[1][1] = cos(rotation);

	// scale to size
	scaleMatrix.identity();
	// scale_x		 0		0
	//		 0 scale_y		0
	//		 0		 0		1
	scaleMatrix.m[0][0] = scale_x;
	scaleMatrix.m[1][1] = scale_y;
	scaleMatrix.m[2][2] = 1;

	// translate, rotate, then scale
	matrix = translateMatrix * rotateMatrix * scaleMatrix;

	// set up inverse
	inverse = matrix.inverse();

	// setup tl, tr, bl, br
	TL = Vector(-sprite.width * scale_x, sprite.height * scale_y, 0.0);
	TR = Vector(sprite.width * scale_x, sprite.height * scale_y, 0.0);
	BL = Vector(-sprite.width * scale_x, -sprite.height * scale_y, 0.0);
	BR = Vector(sprite.width * scale_x, -sprite.height * scale_y, 0.0);
}
