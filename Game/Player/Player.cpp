#include "Player.h"

void Player::Init() {
	pos_ = Vector2(0.0f, 0.0f);
	speed_ = 1.0f;
	box_ = std::make_unique<Texture2D>();
	box_->Texture("Resources/block.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
}

void Player::Update() {
	transform_.translation_ = Vector3(pos_.x, pos_.y, 0.0f);
	transform_.UpdateMatrix();
}

void Player::Draw(const Matrix4x4& viewProjection) {
	Texture2D::TextureDraw(transform_, viewProjection, 0xffffffff, box_.get());
}

void Player::MoveRight() {
	this->pos_.x += this->speed_;
}

void Player::MoveLeft() {
	this->pos_.x -= this->speed_;
}
