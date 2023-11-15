#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"

GameScene* GameScene::GetInstance() {
	static GameScene instance;
	return &instance;
}

void GameScene::Initialize() {
	axis = Normalize(Vector3::one);
	angle = 0.44f;
	rotateMat = MakeRotateAxisAngle(axis, angle);
}

void GameScene::Update() {
#ifdef _DEBUG
	DrawImGui();

#endif DEBUG
	

}

void GameScene::Draw() {
	
}

void GameScene::DrawMatrix4x4(Matrix4x4 mat) {
	ImGui::Text("%.3f : %.3f : %.3f : %.3f", rotateMat.m[0][0], rotateMat.m[0][1], rotateMat.m[0][2], rotateMat.m[0][3]);
	ImGui::Text("%.3f : %.3f : %.3f : %.3f", rotateMat.m[1][0], rotateMat.m[1][1], rotateMat.m[1][2], rotateMat.m[1][3]);
	ImGui::Text("%.3f : %.3f : %.3f : %.3f", rotateMat.m[2][0], rotateMat.m[2][1], rotateMat.m[2][2], rotateMat.m[2][3]);
	ImGui::Text("%.3f : %.3f : %.3f : %.3f", rotateMat.m[3][0], rotateMat.m[3][1], rotateMat.m[3][2], rotateMat.m[3][3]);
}

void GameScene::DrawImGui() {
	ImGui::Begin("RotateMat");
	DrawMatrix4x4(rotateMat);
	ImGui::End();
}
