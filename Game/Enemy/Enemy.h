#pragma once
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "math/AABB/AABB.h"

class Enemy
{
public:
	Enemy();
	~Enemy() = default;

	void Initialize(const Vector3& startPos);

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	bool IsDead() { return isAlive_; }

	void SetDead(bool flag) { isAlive_ = flag; }

	void SetEnemyModel(std::vector<std::shared_ptr<Model>> enemy) { model_ = enemy; parts_.resize(model_.size()); }

	Vector3 GetPosition() const { return parts_[0].GetTranslate(); }

private: // モデル
	enum PARTS {
		body,
		weapon,
		MaxSize
	};
	std::vector<std::shared_ptr<Model>> model_;

private:
	std::vector<WorldTransform> parts_;
	bool isAlive_ = false;
public:
	AABB aabb_;

};
