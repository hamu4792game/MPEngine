#pragma once
#include "math/Vector3.h"
#include "Engine/Texture/Line/Line.h"

class Sphere {
public:
	Sphere() = default;
	~Sphere() = default;

public:
	Vector3 center;
	Vector3 rotate;
	float radius;

public:
	void Update(WorldTransform& transform);

};