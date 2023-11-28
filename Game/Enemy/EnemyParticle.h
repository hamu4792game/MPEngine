#pragma once

#include "Engine/Texture/Particle/Particle.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include <memory>

class EnemyParticle {
public:
	EnemyParticle();
	~EnemyParticle() = default;

	void initialize(const Vector3& position);
	void Update();
	void Draw(const Matrix4x4& viewProjection);

	bool FinishEffect();

private:
	std::shared_ptr<Particle> particle_ = nullptr;

	const uint32_t kMaxNum = 20u;

	WorldTransform trans_[20];
	struct Param {
		Vector3 direction_;
		Vector3 scalr_;
		bool isAlive_;
	};
	Param parame_[20];
	

};
