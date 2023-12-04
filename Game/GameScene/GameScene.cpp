#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "Engine/Texture/Line/Line.h"
#include "Engine/Texture/Particle/Particle.h"

GameScene* GameScene::GetInstance()
{
	static GameScene instance;
	return &instance;
}

void GameScene::Initialize()
{
	//	カメラの読み込みと生成
	camera = std::make_shared<Camera>(2000.0f, true);
	camera2d = std::make_shared<Camera>();
	//	カメラ行列の更新
	camera->transform.translation_.z = -10.0f;
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();

	inputHandler_ = new InputHandle();

	inputHandler_->AssignMoveLeftCommand2PressKeyA();
	inputHandler_->AssignMoveRightCommand2PressKeyD();

	player_ = new Player();
	player_->Init();

	//	変数の初期化
	scene = Scene::BATTLE;
	oldscene = Scene::RESULT;


	//	モデルの引き渡し

}

void GameScene::Update()
{
#ifdef _DEBUG
	//ImGui::Begin("camera");
	//ImGui::DragFloat3("scale", &camera->transform.scale_.x, 0.1f);
	//ImGui::DragFloat3("translate", &camera->transform.translation_.x, 1.0f);
	//ImGui::DragFloat3("rotate", &camera->transform.rotation_.x, AngleToRadian(1.0f));
	//ImGui::End();
#endif DEBUG

	command_ = inputHandler_->HandleInput();

	if (this->command_) {
		command_->Exec(*player_);
	}
	player_->Update();

	//	カメラ行列の更新
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();

}

void GameScene::Draw() {
	player_->Draw(viewProjectionMatrix2d);
}

void GameScene::Finalize()
{
	Line::Finalize();
	Model::Finalize();
	Texture2D::Finalize();
	Particle::Finalize();

	delete player_;
	delete inputHandler_;
}


void GameScene::ModelLoad()
{
	
}
