#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "Engine/Base/MultipathRendering/MultipathRendering.h"

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

	//	シーンの生成
	battle_ = std::make_unique<Battle>(camera.get());

	//	モデルのロード
	for (uint8_t i = 0u; i < 5u; i++) {
		player_.push_back(std::make_shared<Model>());
	}
	for (uint8_t i = 0u; i < 2u; i++) {
		enemy_.push_back(std::make_shared<Model>());
	}
	particle_ = std::make_shared<Particle>();

	floor_ = std::make_shared<Model>();

	player_[0]->Texture("Resources/player/body/body.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	player_[1]->Texture("Resources/player/head/head.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	player_[2]->Texture("Resources/player/RArm/RArm.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	player_[3]->Texture("Resources/player/LArm/LArm.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	player_[4]->Texture("Resources/player/weapon/weapon.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	floor_->Texture("Resources/floor/floor.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/uvChecker.png");
	
	enemy_[0]->Texture("Resources/enemyBody/enemyBody.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	enemy_[1]->Texture("Resources/enemyWeapon/enemyWeapon.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");

	particle_->Texture("Resources/plane/plane.obj", "./Resources/Shader/Particle.VS.hlsl", "./Resources/Shader/Particle.PS.hlsl", "Resources/uvChecker.png", 100);

	skydome_.Initialize();
	skydome_.ModelLoad();
	ground_.Initialize();
	ground_.ModelLoad();
	
	//	モデルの引き渡し
	battle_->SetPlayerModel(player_);
	battle_->SetEnemyModel(enemy_);
	battle_->SetFloorModel(floor_.get());

	//	シーンの初期化
	battle_->Initialize();
	
	//	変数の初期化
	scene = Scene::BATTLE;
	oldscene = Scene::RESULT;


}

void GameScene::Update() {
#ifdef _DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("scale", &camera->transform.scale_.x, 0.1f);
	ImGui::DragFloat3("translate", &camera->transform.translation_.x, 1.0f);
	ImGui::DragFloat3("rotate", &camera->transform.rotation_.x, AngleToRadian(1.0f));
	ImGui::End();

	ImGui::Begin("aaaaa");
	ImGui::DragFloat2("pos", &MultipathRendering::GetInstance()->cEffectParameters->centerPosition.x);
	ImGui::DragFloat("rate", &MultipathRendering::GetInstance()->cEffectParameters->parameterRate);
	ImGui::SliderInt("type", &MultipathRendering::GetInstance()->cEffectParameters->type, 0, 10);
	ImGui::End();
#endif DEBUG

	

	//	シーン切替わり時の初期化
	if (oldscene != scene) {
		switch (scene)
		{
		case GameScene::Scene::TITLE:
			break;
		case GameScene::Scene::BATTLE:
			battle_->Initialize();
			break;
		case GameScene::Scene::RESULT:
			break;
		}
	}
	oldscene = scene;

	switch (scene)
	{
	case GameScene::Scene::TITLE:
		break;
	case GameScene::Scene::BATTLE:
		battle_->Update();
		break;
	case GameScene::Scene::RESULT:
		break;
	}

	//	カメラ行列の更新
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();
	billborld_ = camera->GetBillboardMat();

}

void GameScene::Draw()
{
	skydome_.Draw(viewProjectionMatrix);
	//ground_.Draw(viewProjectionMatrix);

	//	3D描画
	switch (scene)
	{
	case GameScene::Scene::TITLE:
		break;
	case GameScene::Scene::BATTLE:
		battle_->Draw3D(viewProjectionMatrix);
		break;
	case GameScene::Scene::RESULT:
		break;
	}

	//	2D描画
	switch (scene)
	{
	case GameScene::Scene::TITLE:
		break;
	case GameScene::Scene::BATTLE:
		battle_->Draw2D(viewProjectionMatrix2d);
		break;
	case GameScene::Scene::RESULT:
		break;
	}

}

void GameScene::Finalize()
{
	Line::Finalize();
	Model::Finalize();
	Texture2D::Finalize();
	Particle::Finalize();
}


void GameScene::ModelLoad()
{
	
}
