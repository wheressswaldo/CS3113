//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//		Assignment 7 - Asteroids
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
	if (isPlayer){
		if (x < -1.33f)
			x = -1.33f;
		else if (x > 1.33f)
			x = 1.33f;
		if (y < -1.0f)
			y = -1.0f;
		else if (y > 1.0f)
			y = 1.0f;
	}
}

// draw the entity
void Entity::Render() {
	buildMatrixAndSetUp();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
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

//// check collision with another entity
//bool Entity::collidesWith(Entity* entity) {
//	float entity1_top = y + height / 2;
//	float entity1_bottom = y - height / 2;
//	float entity1_right = x + width / 2;
//	float entity1_left = x - width / 2;
//
//	float entity2_top = entity->y + entity->height / 2;
//	float entity2_bottom = entity->y - entity->height / 2;
//	float entity2_right = entity->x + entity->width / 2;
//	float entity2_left = entity->x - entity->width / 2;
//
//	if (entity1_bottom > entity2_top)
//		return false;
//	if (entity1_top < entity2_bottom)
//		return false;
//	if (entity1_left > entity1_right)
//		return false;
//	if (entity1_right < entity2_left)
//		return false;
//
//	return true;
//}

// jump, set jump boolean to true so entity can't jump infinitely
// set y velocity, stop collidingbottom
//void Entity::jump() {
//	collidedBottom = false;
//	isJumping = true;
//	velocity_y = 4.0f;
//}
//
//// walk right
//void Entity::walkRight() {
//	faceLeft = false;
//	if (velocity_x < 0.0f)
//		velocity_x = 0.0f;
//	if (velocity_x < 2.0f)
//		acceleration_x = 9.0f;
//}
//
//// walk left
//void Entity::walkLeft() {
//	faceLeft = true;
//	if (velocity_x > 0.0f)
//		velocity_x = 0.0f;
//	if (velocity_x > -2.0f)
//		acceleration_x = -9.0f;
//}
//
//// idle
//void Entity::idle() {
//	velocity_x = 0.0f;
//	acceleration_x = 0.0f;
//}
