#include "transform.h"
#include "vector.h"
#include "matrix.h"

void Transform::UpdateMatrix()
{
	matrix.SetTranslationAndRotation(position, yaw);
}