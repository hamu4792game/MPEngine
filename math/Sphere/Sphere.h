#pragma once
#include "math/Vector3.h"
#include <array>

class Line;

class Sphere {
public:
	Sphere() = default;
	~Sphere() = default;

private:
	Vector3 center;
	Vector3 rotate;
	float radius;
	//Line *line;
	std::array<Line,16>line;

};