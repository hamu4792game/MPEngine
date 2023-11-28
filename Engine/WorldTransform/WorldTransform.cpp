#include "WorldTransform.h"

decltype(WorldTransform::monocuro) WorldTransform::monocuro;
decltype(WorldTransform::light) WorldTransform::light;

WorldTransform::WorldTransform()
{
	cMat->wvp = MakeIdentity4x4();
	cMat->world = MakeIdentity4x4();
	*cColor = { 1.0f,1.0f,1.0f,1.0f };
	cMono->pibot = { 0.0f,0.0f };
	cMono->rate = 1.0f;
	cDirectionalLight->color = { 1.0f,1.0f,1.0f,1.0f };
	cDirectionalLight->direction = { 0.0f,-1.0f,0.0f };
	cDirectionalLight->intensity = 1.0f;
	light.color = cDirectionalLight->color;
	light.direction = cDirectionalLight->direction;
	light.intensity = cDirectionalLight->intensity;

	rotateMatrix = MakeIdentity4x4();
}

WorldTransform::WorldTransform(const WorldTransform& transform)
{
	*this = transform;
}

WorldTransform& WorldTransform::operator=(const WorldTransform& trans)
{
	this->translation_ = trans.translation_;
	this->rotation_ = trans.rotation_;
	this->scale_ = trans.rotation_;
	this->worldMatrix = trans.worldMatrix;
	this->parent_ = trans.parent_;
	this->cMat = trans.cMat;
	this->cColor = trans.cColor;
	this->cMono->pibot = trans.cMono->pibot;
	this->cMono->rate = trans.cMono->rate;

	return *this;
}

Matrix4x4 WorldTransform::UpdateMatrix()
{
	//	monocuroを動かせばcMonoに代入される
	//this->cMono->pibot = monocuro.pibot;
	this->cDirectionalLight->direction = light.direction;
	this->cDirectionalLight->intensity = light.intensity;

	//	スケール、回転、平行移動を合成して行列を計算する
	this->worldMatrix = MakeAffineMatrix(scale_, rotation_, translation_);

	Matrix4x4 identity = MakeIdentity4x4();
	if (rotateMatrix.m[0][0] != identity.m[0][0] || rotateMatrix.m[1][1] != identity.m[1][1] || rotateMatrix.m[2][2] != identity.m[2][2]) {
		this->worldMatrix = (MakeScaleMatrix(scale_) * (rotateMatrix /** MakeRotateMatrix(rotation_)*/)) * MakeTranslateMatrix(translation_);
	}

	//	親があれば親のワールド行列を掛ける
	if (parent_)
	{
		this->worldMatrix = this->worldMatrix * parent_->worldMatrix;
	}

	return this->worldMatrix;
}

void WorldTransform::Reset() {
	scale_ = Vector3(1.0f, 1.0f, 1.0f);
	rotation_ = Vector3(0.0f, 0.0f, 0.0f);
	translation_ = Vector3(0.0f, 0.0f, 0.0f);
	parent_ = nullptr;
	worldMatrix = MakeIdentity4x4();
}
