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

	//hud = std::make_unique<Texture2D>();
	//hud->Texture("Resources/hud/titleText.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");

	//fence1 = std::make_unique<Particle>();
	line[0] = std::make_unique<Line>();
	line[1] = std::make_unique<Line>();
	fence2 = std::make_unique<Model>();
	//fence1->Texture("Resources/plane/plane.obj", "./Resources/Shader/Particle.VS.hlsl", "./Resources/Shader/Particle.PS.hlsl", 5);
	fence2->Texture("Resources/plane/plane.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");

	//fenceTrans2.rotation_.x = AngleToRadian(-90.0f);
	for (uint8_t i = 0; i < 5; i++)
	{
		fenceTrans1[i].translation_ = Vector3(0.1f * i, 0.0f, 0.0f);
		fenceTrans1[i].rotation_ = Vector3(0.6f, 3.0f, 0.0f);
		fenceTrans1[i].UpdateMatrix();
	}
	//fence1->blendType = BlendMode::Screen;

}

void GameScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("translate", &camera->transform.translation_.x, 1.0f);
	ImGui::DragFloat3("scale", &camera->transform.scale_.x, 0.1f);
	ImGui::DragFloat3("start", &start[0].x, 0.1f);
	ImGui::DragFloat3("end", &end[0].x, 0.1f);
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
		fence2->blendType = BlendMode::Dark;
	}

	
	for (auto& i : fenceTrans1) {
		i.UpdateMatrix();
	}
	
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

	//Model::ModelDraw(fenceTrans1[0], viewProjectionMatrix, 0xffffffff, fence1.get());
	//Particle::ParticleDraw(fenceTrans1, viewProjectionMatrix, 0xffffffff, fence1.get());

	line[0]->DrawLine(start[0], end[0], viewProjectionMatrix2d, 0xffffffff);
	line[1]->DrawLine(Vector3(1.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), viewProjectionMatrix, 0xff0000ff);

	Model::ModelDraw(fenceTrans1[0], viewProjectionMatrix, 0xffffffff, fence2.get());

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
