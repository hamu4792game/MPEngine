#include "Battle.h"
#include <externals/imgui/imgui.h>
#include "Engine/Input/KeyInput/KeyInput.h"
#include <cmath>

Battle::Battle(Camera* camera, Camera* camera2d)
{
	this->camera_ = camera;
	this->camera2d_ = camera2d;
	player_ = std::make_unique<Player>(camera);
	stage_ = std::make_shared<Stage>();
	enemys_.resize(5);
	for (auto& i : enemys_) {
		i = std::make_unique<Enemy>();
	}
	lockOn_ = std::make_unique<LockOn>();
}

Vector3 enePos[5]{
		Vector3(8.0f,0.0f,0.0f),
		Vector3(8.0f,1.0f,0.0f),
		Vector3(8.0f,2.0f,0.0f),
		Vector3(8.0f,3.0f,0.0f),
		Vector3(-3.0f, 1.0f, 50.0f),
};

void Battle::Initialize()
{
	stage_->Initialize();
	stage_->SetFloorModel(floorModel_);
	player_->Initialize();
	player_->SetStagePtr(stage_.get());

	int index = 0;
	for (auto& i : enemys_) {
		i->SetEnemyModel(enemyModel_);
		i->Initialize(enePos[index]);
		index++;
	}
	lockOn_->Initialize();

	player_->SetLockOn(lockOn_.get());

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

	if (player_->GetResetFlag()) {
		int index = 0;
		for (auto& i : enemys_) {
			i->SetDead(false);
			i->Initialize(enePos[index]);
			index++;
		}
	}
	camera_->GetViewProMat();
	camera2d_->GetViewProMat();
	lockOn_->Update(enemys_, camera_, camera_->GetViewMat());

}

void Battle::Draw3D(const Matrix4x4& viewProjection)
{
	stage_->Draw(viewProjection);
	player_->Draw(viewProjection);
	for (auto& i : enemys_) {
		i->Draw(viewProjection);
	}
}

void Battle::Draw2D(const Matrix4x4& viewProjection) {
	lockOn_->Draw2D(viewProjection);
}

void Battle::EnemySet() {

}
