#include "Battle.h"
#include <externals/imgui/imgui.h>
#include "Engine/Input/KeyInput/KeyInput.h"
#include <cmath>

Battle::Battle(Camera* camera)
{
	this->camera_ = camera;
	player_ = std::make_unique<Player>(camera);
}

void Battle::Initialize()
{
	player_->Initialize();
	player_->SetPlayerModel(playerModel_);
}

void Battle::Update()
{


	if (KeyInput::GetKey(DIK_RIGHTARROW)) {
		camera_->transform.rotation_.y += AngleToRadian(1.0f);
	}
	if (KeyInput::GetKey(DIK_LEFTARROW)) {
		camera_->transform.rotation_.y -= AngleToRadian(1.0f);
	}

	player_->Update();

}

void Battle::Draw3D(const Matrix4x4& viewProjection)
{
	player_->Draw(viewProjection);
}
