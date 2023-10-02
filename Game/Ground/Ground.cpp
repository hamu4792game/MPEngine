#include "Ground.h"

Ground::Ground()
{
	ground = std::make_unique<Model>();
	transform.resize(3);
}

void Ground::Initialize()
{
	transform[0].scale_ = Vector3(50.0f, 1.0f, 300.0f);
	transform[0].translation_ = Vector3(0.0f, 0.0f, 250.0f);
	transform[1].scale_ = Vector3(50.0f, 1.0f, 300.0f);
	transform[1].translation_ = Vector3(0.0f, 0.0f, 750.0f);
	transform[2].scale_ = Vector3(50.0f, 1.0f, 300.0f);
	transform[2].translation_ = Vector3(0.0f, 0.0f, 1250.0f);
}

void Ground::ModelLoad()
{
	ground->Texture("Resources/plane/plane.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "ground/ground.png");
}

void Ground::Draw(const Matrix4x4& viewProjection)
{
	for (uint16_t i = 0u; i < transform.size(); i++) {
		transform[i].UpdateMatrix();
		Model::ModelDraw(transform[i], viewProjection, 0xffffffff, ground.get());
	}
	
}
