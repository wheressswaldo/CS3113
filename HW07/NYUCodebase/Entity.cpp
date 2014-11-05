//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 7 - Asteroids
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Entity.h"

Entity::Entity() {
	x = 0.0f;
	y = 0.0f;
	scale_x = 1.0f;
	scale_y = 1.0f;
	rotation = 0.0f;
	velocity_x = 0.0f;
	velocity_y = 0.0f;
	acceleration_x = 0.0f;
	acceleration_y = 0.0f;
	friction_x = 0.0f;
	friction_y = 0.0f;
	mass = 0.0f;
	isStatic = false;
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
	isJumping = false;
}
Entity::~Entity() {}
void Entity::Update(float elapsed) {
	if (x < -1.4f || x >1.4f)
		velocity_x *= -1;
	if (y < -1.0f || y > 1.0f)
		velocity_y *= -1;
}
void Entity::Render() {
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	sprite.Draw(scale_x, scale_y);
	glPopMatrix();
}
void Entity::buildMatrix() {
	Matrix scaleMatrix;
	scaleMatrix.identity();
	scaleMatrix.m[0][0] = scale_x;
	scaleMatrix.m[1][1] = scale_y;
	scaleMatrix.m[2][2] = 1;
	Matrix rotateMatrix;
	rotateMatrix.identity();
	rotateMatrix.m[0][0] = cos(rotation);
	rotateMatrix.m[0][1] = sin(rotation);
	rotateMatrix.m[1][0] = -sin(rotation);
	rotateMatrix.m[1][1] = cos(rotation);
	Matrix translateMatrix;
	translateMatrix.identity();
	translateMatrix.m[3][0] = x;
	translateMatrix.m[3][1] = y;
	translateMatrix.m[3][2] = 1;
	matrix.identity();
	matrix = translateMatrix * rotateMatrix * scaleMatrix;
}