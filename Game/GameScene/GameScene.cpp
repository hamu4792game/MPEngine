#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include <string>

GameScene* GameScene::GetInstance() {
	static GameScene instance;
	return &instance;
}

void GameScene::Initialize() {
	rotation0 = Quaternion::MakeRotateAxisAngleQuaternion(Vector3(0.71f, 0.71f, 0.0f).Normalize(), 0.3f);
	rotation1 = Quaternion::MakeRotateAxisAngleQuaternion(Vector3(0.71f, 0.0f, 0.71f).Normalize(), 3.141592f);

	interpolate0 = Quaternion::Slerp(rotation0, rotation1, 0.0f);
	interpolate1 = Quaternion::Slerp(rotation0, rotation1, 0.3f);
	interpolate2 = Quaternion::Slerp(rotation0, rotation1, 0.5f);
	interpolate3 = Quaternion::Slerp(rotation0, rotation1, 0.7f);
	interpolate4 = Quaternion::Slerp(rotation0, rotation1, 1.0f);

}

void GameScene::Update() {
#ifdef _DEBUG
	DrawImGui();

#endif DEBUG
	

}

void GameScene::Draw() {
	
}

void GameScene::DrawImGui() {
	ImGui::Begin("Quaternion");
	ImGui::Text("%.2f : %.2f : %.2f : %.2f   : interpolate0, Sleap( q0, q1, 0.0f)", interpolate0.x, interpolate0.y, interpolate0.z, interpolate0.w);
	ImGui::Text("%.2f : %.2f : %.2f : %.2f   : interpolate1, Sleap( q0, q1, 0.3f)", interpolate1.x, interpolate1.y, interpolate1.z, interpolate1.w);
	ImGui::Text("%.2f : %.2f : %.2f : %.2f   : interpolate2, Sleap( q0, q1, 0.5f)", interpolate2.x, interpolate2.y, interpolate2.z, interpolate2.w);
	ImGui::Text("%.2f : %.2f : %.2f : %.2f   : interpolate3, Sleap( q0, q1, 0.7f)", interpolate3.x, interpolate3.y, interpolate3.z, interpolate3.w);
	ImGui::Text("%.2f : %.2f : %.2f : %.2f   : interpolate4, Sleap( q0, q1, 1.0f)", interpolate4.x, interpolate4.y, interpolate4.z, interpolate4.w);
	ImGui::End();
}
