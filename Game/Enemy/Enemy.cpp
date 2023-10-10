#include "Enemy.h"

Enemy::Enemy()
{
	model_.resize(PARTS::MaxSize);
	for (auto& i : model_) {
		i = std::make_unique<Model>();
	}
	
}

void Enemy::Initialize()
{
	parts_.resize(2);
	parts_[weapon].parent_ = &parts_[body];

}

void Enemy::Update()
{
	for (auto& i : parts_) {
		i.UpdateMatrix();
	}
}

void Enemy::Draw(const Matrix4x4& viewProjection)
{
	for (uint8_t i = 0; i < parts_.size(); i++) {
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, model_[i].get());
	}
}

void Enemy::ModelLoad()
{
	model_[body]->Texture("Resources/enemyBody/enemyBody.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	model_[weapon]->Texture("Resources/enemyWeapon/enemyWeapon.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");

}
