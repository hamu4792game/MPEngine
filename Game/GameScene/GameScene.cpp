#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"

GameScene* GameScene::GetInstance() {
	static GameScene instance;
	return &instance;
}

void GameScene::Initialize() {

}

void GameScene::Update() {
#ifdef _DEBUG
	ImGui::Begin("camera");
	//ImGui::DragFloat3("scale", &camera->transform.scale_.x, 0.1f);
	//ImGui::DragFloat3("translate", &camera->transform.translation_.x, 1.0f);
	//ImGui::DragFloat3("rotate", &camera->transform.rotation_.x, AngleToRadian(1.0f));
	ImGui::End();
#endif DEBUG


}

void GameScene::Draw() {
	
}
