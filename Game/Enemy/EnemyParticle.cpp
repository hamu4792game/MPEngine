#include "EnemyParticle.h"
#include "math/Vector3.h"
#include "Engine/Camera/Camera.h"
#include "Game/GameScene/GameScene.h"

EnemyParticle::EnemyParticle() {
	particle_ = std::make_shared<Particle>();
	particle_->Texture("Resources/plane/plane.obj", "./Resources/Shader/Particle.VS.hlsl", "./Resources/Shader/Particle.PS.hlsl", "Resources/uvChecker.png", kMaxNum);

}

void EnemyParticle::initialize(const Vector3& position) {
	for (auto& i : trans_) {
		i.translation_ = position;
		i.scale_ = Vector3(0.5f, 0.5f, 0.1f);
	}
	for (auto& i : parame_) {
		i.direction_.x = RandNum(-0.1f, 0.1f);
		i.direction_.y = RandNum(-0.1f, 0.1f);
		i.direction_.z = RandNum(-0.1f, 0.1f);
		i.scalr_.x = RandNum(0.01f, 0.05f);
		i.scalr_.y = i.scalr_.x;
		i.isAlive_ = true;
	}
}

void EnemyParticle::Update() {
	auto billborld = GameScene::GetInstance()->billborld_;
	for (uint32_t i = 0; i < kMaxNum; i++) {
		if (!parame_[i].isAlive_) { continue; }
		trans_[i].translation_ += parame_[i].direction_;
		trans_[i].scale_ -= parame_[i].scalr_;
		if (trans_[i].scale_.x <= 0.0f || trans_[i].scale_.y <= 0.0f) {
			parame_[i].isAlive_ = false;
		}
	}
	for (uint32_t i = 0; i < kMaxNum; i++) {
		if (!parame_[i].isAlive_) { continue; }
		trans_[i].worldMatrix = MakeScaleMatrix(trans_[i].scale_) * billborld * MakeTranslateMatrix(trans_[i].translation_);
	}
}

void EnemyParticle::Draw(const Matrix4x4& viewProjection) {
	for (uint32_t i = 0; i < kMaxNum; i++) {
		if (!parame_[i].isAlive_) { continue; }
		Particle::ParticleDraw(trans_, viewProjection, 0xffffffff, particle_.get());
	}
}

bool EnemyParticle::FinishEffect() {
	for (auto& i : parame_) {
		if (i.isAlive_) { return false; }
	}
	return true;
}
