#include "Battle.h"
#include <externals/imgui/imgui.h>
#include "Engine/Input/KeyInput/KeyInput.h"
#include <cmath>

Battle::Battle(Camera* camera)
{
	this->camera_ = camera;
	player_ = std::make_unique<Player>(camera);
	stage_ = std::make_shared<Stage>();
	enemy_ = std::make_shared<Enemy>();
	enemy_->ModelLoad();
}

void Battle::Initialize()
{
	stage_->Initialize();
	stage_->SetFloorModel(floorModel_);
	player_->Initialize();
	player_->SetPlayerModel(playerModel_);
	player_->SetStagePtr(stage_.get());
	enemy_->Initialize();
}

void Battle::Update()
{


	if (KeyInput::GetKey(DIK_RIGHTARROW)) {
		camera_->transform.rotation_.y += AngleToRadian(1.0f);
	}
	if (KeyInput::GetKey(DIK_LEFTARROW)) {
		camera_->transform.rotation_.y -= AngleToRadian(1.0f);
	}

	stage_->Update();
	player_->Update();
	enemy_->Update();

}

void Battle::Draw3D(const Matrix4x4& viewProjection)
{
	stage_->Draw(viewProjection);
	player_->Draw(viewProjection);
	enemy_->Draw(viewProjection);
}
