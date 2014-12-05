//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Kong Huang
//		CS3113 - Introduction to Game Programming
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Vector.h"

Vector::Vector() {}
Vector::Vector(float x, float y, float z)
	: x(x), y(y), z(z)
{}

float Vector::length(){
	return sqrt(x*x + y*y + z*z);
}

void Vector::normalize() {
	float length = sqrt(x*x + y*y + z*z);
	x = x / length;
	y = y / length;
	z = z / length;
}