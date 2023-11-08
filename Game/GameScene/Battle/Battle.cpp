#include "Battle.h"
#include <externals/imgui/imgui.h>
#include "Engine/Input/KeyInput/KeyInput.h"
#include <cmath>

Battle::Battle(Camera* camera)
{
	this->camera_ = camera;
	player_ = std::make_unique<Player>(camera);
	stage_ = std::make_shared<Stage>();
	enemys_.resize(1);
	for (auto& i : enemys_) {
		i = std::make_unique<Enemy>();
		i->ModelLoad();
	}
}

void Battle::Initialize()
{
	stage_->Initialize();
	stage_->SetFloorModel(floorModel_);
	player_->Initialize();
	player_->SetStagePtr(stage_.get());
	for (auto& i : enemys_) {
		i->Initialize();
	}
}

void Battle::Update()
{

	//	カメラの移動

	if (KeyInput::GetKey(DIK_RIGHTARROW)) {
		camera_->transform.rotation_.y += AngleToRadian(1.0f);
	}
	if (KeyInput::GetKey(DIK_LEFTARROW)) {
		camera_->transform.rotation_.y -= AngleToRadian(1.0f);
	}

	//	padが接続されているなら
	if (KeyInput::GetInstance()->GetPadConnect()) {
		Vector2 pMove(0.0f, 0.0f);
		pMove = KeyInput::GetInstance()->GetPadRStick();
		//	移動があれば代入する
		if (pMove.x < -0.5f)	{
			camera_->transform.rotation_.y -= AngleToRadian(1.0f);
		}else if (pMove.x > 0.5f)	{
			camera_->transform.rotation_.y += AngleToRadian(1.0f);
		}
		if (pMove.y < -0.5f)	{
			camera_->transform.rotation_.x -= AngleToRadian(1.0f);
		}else if (pMove.y > 0.5f)	{
			camera_->transform.rotation_.x += AngleToRadian(1.0f);
		}
	}

	stage_->Update();
	player_->Update();
	for (auto& i : enemys_) {
		i->Update();
		if (player_->EnemyColl(&i->aabb_)) {
			i->SetDead(true);
		}
	}
	//	デスフラグの立った敵を削除
	if (enemys_.remove_if([](const std::unique_ptr<Enemy>& ene)
		{
			return ene->IsDead();
		})
		) {
	};
	

}

void Battle::Draw3D(const Matrix4x4& viewProjection)
{
	stage_->Draw(viewProjection);
	player_->Draw(viewProjection);
	for (auto& i : enemys_) {
		i->Draw(viewProjection);
	}
}