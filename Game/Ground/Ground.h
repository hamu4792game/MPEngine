#pragma once
#include "math/Matrix4x4.h"
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"

class Ground
{
public:
	Ground();
	~Ground() = default;

	void Initialize();

	void ModelLoad();

	void Draw(const Matrix4x4& viewProjection);

private:

	std::unique_ptr<Model> ground;
	std::vector<WorldTransform>transform;

};
