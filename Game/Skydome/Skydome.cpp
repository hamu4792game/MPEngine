#include "Skydome.h"

Skydome::Skydome()
{
	skydome = std::make_unique<Model>();
}

void Skydome::SetParent(const WorldTransform& world)
{
	transform.parent_ = &world;
}

void Skydome::NullParent()
{
	transform.parent_ = nullptr;
}

void Skydome::Initialize()
{
	transform.scale_ = Vector3(400.0f, 400.0f, 400.0f);
	transform.translation_ = Vector3(0.0f, 0.0f, 0.0f);
}

void Skydome::ModelLoad()
{
	skydome->Texture("Resources/boxSky/boxSky.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
}

void Skydome::Draw(const Matrix4x4& viewProjection)
{
	transform.rotation_.y += AngleToRadian(0.1f);
	transform.UpdateMatrix();
	Model::ModelDraw(transform, viewProjection, 0xffffffff, skydome.get());
}
