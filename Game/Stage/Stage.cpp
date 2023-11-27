#include "Stage.h"

Stage::Stage()
{
	goal = std::make_unique<Model>();
	goal->Texture("Resources/box/box.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/uvChecker.png");
	goalAABB = std::make_unique<AABB>();
}

void Stage::Initialize()
{
	floor_[0].Initialize(Vector3(0.0f, 0.0f, 0.0f));
	floor_[1].Initialize(Vector3(0.0f, 0.0f, 50.0f));
	moveFloor_.Initialize(Vector3(0.0f, 0.0f, 25.0f));

	goalTrans.translation_ = floor_[1].transform_.translation_;
	goalTrans.translation_.y = 1.0f;
}

void Stage::Update()
{

	for (uint8_t i = 0; i < 2; i++) {
		floor_[i].Update();
	}
	moveFloor_.Update();
	goalTrans.UpdateMatrix();
	goalAABB->Update(goalTrans);
}

void Stage::Draw(const Matrix4x4& viewProjection)
{
	for (uint8_t i = 0; i < 2; i++) {
		floor_[i].Draw(viewProjection);
	}
	moveFloor_.Draw(viewProjection);

	Model::ModelDraw(goalTrans, viewProjection, 0x00ff00ff, goal.get());
	goalAABB->DrawAABB(viewProjection, 0xffffffff);
}

void Stage::SetFloorModel(Model* model)
{
	floor_[0].SetFloorModel(model);
	floor_[1].SetFloorModel(model);
	moveFloor_.SetFloorModel(model);
}
