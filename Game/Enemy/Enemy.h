#pragma once
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "math/AABB/AABB.h"
#include "Game/Enemy/EnemyParticle.h"

class Enemy
{
public:
	Enemy();
	~Enemy() = default;

	void Initialize(const Vector3& startPos);

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	bool IsDead() const { return isDeth_; }

	void SetDead(bool flag) { isAlive_ = flag; }

	void SetEnemyModel(std::vector<std::shared_ptr<Model>> enemy) { model_ = enemy; parts_.resize(model_.size()); }

	Vector3 GetPosition() const { return parts_[0].GetTranslate(); }

	void HitDamage(const int& damage, const Vector3& hit);

private: // モデル
	enum PARTS {
		body,
		weapon,
		MaxSize
	};
	std::vector<std::shared_ptr<Model>> model_;

	std::shared_ptr<EnemyParticle> particle_;

	void UpdateTrans();

private:
	std::vector<WorldTransform> parts_;
	std::vector<WorldTransform> particleTrans_;

	bool isAlive_ = false;
	bool isDeth_ = false;

	int Hp_ = 100;

public:
	AABB aabb_;

};
