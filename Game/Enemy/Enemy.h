#pragma once
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "math/AABB/AABB.h"

class Enemy
{
public:
	Enemy();
	~Enemy() = default;

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	void ModelLoad();

	bool IsDead() { return isAlive_; }

	void SetDead(bool flag) { isAlive_ = flag; }

private: // モデル
	enum PARTS {
		body,
		weapon,
		MaxSize
	};
	std::vector<std::unique_ptr<Model>> model_;

private:
	std::vector<WorldTransform> parts_;
	bool isAlive_ = false;
public:
	AABB aabb_;

};
