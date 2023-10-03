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
	
	//	変数の初期化
	scene = Scene::TITLE;
	oldscene = Scene::RESULT;

	hud = std::make_unique<Texture2D>();
	hud->Texture("Resources/hud/titleText.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");

	fence1 = std::make_unique<Model>();
	fence2 = std::make_unique<Model>();
	fence1->Texture("Resources/fence/fence.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	fence2->Texture("Resources/plane/plane.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");

	//fenceTrans2.rotation_.x = AngleToRadian(-90.0f);
	fenceTrans1.rotation_.x = 0.6f;
	fenceTrans1.rotation_.y = 3.0f;

}

void GameScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("translate", &camera->transform.translation_.x, 1.0f);
	ImGui::DragFloat3("scale", &camera->transform.scale_.x, 0.1f);
	ImGui::DragFloat3("transform", &fenceTrans1.rotation_.x, 0.1f);
	ImGui::DragFloat3("transform2", &fenceTrans1.scale_.x, 0.1f);
	ImGui::End();
#endif DEBUG

	

	//	シーン切替わり時の初期化
	if (oldscene != scene) {
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
	oldscene = scene;

	switch (scene)
	{
	case GameScene::Scene::TITLE:
		break;
	case GameScene::Scene::BATTLE:
		break;
	case GameScene::Scene::RESULT:
		break;
	}

	if (KeyInput::PushKey(DIK_SPACE)) {
		hud->SetAnchorPoint(Texture2D::AnchorPoint::LeftTop);
	}


	hudTrans.UpdateMatrix();
	fenceTrans1.UpdateMatrix();
	fenceTrans2.UpdateMatrix();
	
	//	カメラ行列の更新
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();

}

void GameScene::Draw()
{

	//	3D描画
	switch (scene)
	{
	case GameScene::Scene::TITLE:
		break;
	case GameScene::Scene::BATTLE:
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

	Model::ModelDraw(fenceTrans1, viewProjectionMatrix, 0xffffffff, fence1.get());
	//Model::ModelDraw(fenceTrans2, viewProjectionMatrix, 0xffffffff, fence2.get());

	//Texture2D::TextureDraw(hudTrans, viewProjectionMatrix2d, 0xffffffff, hud.get());

}


void GameScene::ModelLoad()
{
	
}
