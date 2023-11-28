#include "Enemy.h"
#include <externals/imgui/imgui.h>
#include <numbers>

Enemy::Enemy() {
	// model_.resize(PARTS::MaxSize);	
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

	for (auto& i : parts_) {
		i.UpdateMatrix();
	}
	aabb_.Update(parts_[body]);
}

void Enemy::Draw(const Matrix4x4& viewProjection)
{
	if (isAlive_) {
		return;
	}

	for (uint8_t i = 0; i < parts_.size(); i++) {
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, model_[i].get());
	}
	aabb_.DrawAABB(viewProjection, 0xffffffff);
}
