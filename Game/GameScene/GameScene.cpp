#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"

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

	//	モデルのロード
	player_ = std::make_shared<Model>();
	player_->Texture("Resources/player/body.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	skydome_.Initialize();
	skydome_.ModelLoad();
	ground_.Initialize();
	ground_.ModelLoad();
	
	//	シーンの生成と初期化
	battle_ = std::make_unique<Battle>(camera.get());
	battle_->Initialize();

	//	変数の初期化
	scene = Scene::BATTLE;
	oldscene = Scene::RESULT;


	//	モデルの引き渡し
	battle_->SetPlayerModel(player_.get());

}

void GameScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("scale", &camera->transform.scale_.x, 0.1f);
	ImGui::DragFloat3("translate", &camera->transform.translation_.x, 1.0f);
	ImGui::DragFloat3("rotate", &camera->transform.rotation_.x, AngleToRadian(1.0f));
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

}

void GameScene::Draw()
{
	skydome_.Draw(viewProjectionMatrix);
	ground_.Draw(viewProjectionMatrix);

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
