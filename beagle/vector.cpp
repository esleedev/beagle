#include "vector.h"

Vector Vector::operator-()
{
	Vector vector = *this;
	vector.x = -vector.x;
	vector.y = -vector.y;
	vector.z = -vector.z;
	return vector;
}