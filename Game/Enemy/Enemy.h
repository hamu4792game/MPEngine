#pragma once
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"

class Enemy
{
public:
	Enemy();
	~Enemy() = default;

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	void ModelLoad();

private: // モデル
	enum PARTS {
		body,
		weapon,
		MaxSize
	};
	std::vector<std::unique_ptr<Model>> model_;

private:
	std::vector<WorldTransform> parts_;

};
