#include "Enemy.h"
#include <externals/imgui/imgui.h>
#include <numbers>

Enemy::Enemy() {
	particle_ = std::make_shared<EnemyParticle>();
}

void Enemy::Initialize(const Vector3& startPos)
{
	parts_[weapon].parent_ = &parts_[body];
	//parts_[body].translation_ = Vector3(-3.0f, 1.0f, 50.0f);
	parts_[body].translation_ = startPos;
	parts_[body].rotation_ = Vector3::zero;
	parts_[weapon].translation_ = Vector3(0.0f, 2.5f, 0.0f);
	parts_[weapon].rotation_ = Vector3(AngleToRadian(90.0f), 0.0f, 0.0f);

	for (auto& i : parts_) {
		i.UpdateMatrix();
	}
	aabb_.Update(parts_[body]);
	particle_->initialize(parts_[body].translation_);
	isAlive_ = false;
	isDeth_ = false;
	Hp_ = 100;
}

void Enemy::Update()
{
#ifdef _DEBUG
	ImGui::Begin("enemy");
	ImGui::DragFloat3("scale", &parts_[weapon].scale_.x, 0.1f);
	ImGui::DragFloat3("translate", &parts_[weapon].translation_.x, 1.0f);
	ImGui::DragFloat3("rotate", &parts_[weapon].rotation_.x, AngleToRadian(1.0f));
	ImGui::End();
#endif DEBUG
	if (isAlive_) {

		particle_->Update();
		isDeth_ = particle_->FinishEffect();
		return;
	}

	if (true)
	{
		Vector3 move{ 0.0f,0.0f,0.1f };
		move = TransformNormal(move, parts_[body].worldMatrix);

		parts_[body].rotation_.y += 2.0f * std::numbers::pi_v<float> / 180.0f;

		if (parts_[body].rotation_.y >= std::numbers::pi_v<float> *2.0f) {
			parts_[body].rotation_.y -= std::numbers::pi_v<float> *2.0f;
		}
		parts_[body].translation_ += move;

		parts_[weapon].rotation_.y -= AngleToRadian(10.0f);
	}

	UpdateTrans();
}

void Enemy::Draw(const Matrix4x4& viewProjection)
{
	if (isAlive_) {
		particle_->Draw(viewProjection);
		return;
	}

	for (uint8_t i = 0; i < parts_.size(); i++) {
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, model_[i].get());
	}
	aabb_.DrawAABB(viewProjection, 0xffffffff);
}

void Enemy::HitDamage(const int& damage, const Vector3& hit) {
	Hp_ -= damage;
	Vector3 knockBack = Normalize(hit) * 10.0f;
	knockBack.y = 0.0f;
	parts_[body].translation_ += knockBack;
	if (Hp_ <= 0) {
		isAlive_ = true;
		particle_->initialize(parts_[body].translation_);
	}
	UpdateTrans();
}

void Enemy::UpdateTrans() {
	for (auto& i : parts_) {
		i.UpdateMatrix();
	}
	aabb_.Update(parts_[body]);
}
