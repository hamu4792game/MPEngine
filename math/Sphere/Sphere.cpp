#include "Sphere.h"
#include <numbers>

void Sphere::Update(WorldTransform& transform) {
	transform.UpdateMatrix();
	center = transform.GetTranslate();
	radius = 5.0f;
}
