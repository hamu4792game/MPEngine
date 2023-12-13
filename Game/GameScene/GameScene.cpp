#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include <string>

GameScene* GameScene::GetInstance() {
	static GameScene instance;
	return &instance;
}

void GameScene::Initialize() {
	rotation = Quaternion::MakeRotateAxisAngleQuaternion(Vector3(1.0f, 0.4f, -0.2f).Normalize(), 0.45f);
	pointY = Vector3(2.1f, -0.9f, 1.3f);
	rotateMatrix = Quaternion::MakeRotateMatrix(rotation);
	rotateByQuaternion = Quaternion::RotateVector(pointY, rotation);
	rotateByMatrix = Transform(pointY, rotateMatrix);
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
	ImGui::Text("%.2f : %.2f : %.2f : %.2f : rotation", rotation.x, rotation.y, rotation.z, rotation.w);
	ImGui::Text("rotateMatrix");
	ImGui::Text("%.3f : %.3f : %.3f : %.3f", rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3]);
	ImGui::Text("%.3f : %.3f : %.3f : %.3f", rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3]);
	ImGui::Text("%.3f : %.3f : %.3f : %.3f", rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3]);
	ImGui::Text("%.3f : %.3f : %.3f : %.3f", rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);
	ImGui::Text("%.2f : %.2f : %.2f : rotateByQuaternion", rotateByQuaternion.x, rotateByQuaternion.y, rotateByQuaternion.z);
	ImGui::Text("%.2f : %.2f : %.2f : rotateByMatrix", rotateByMatrix.x, rotateByMatrix.y, rotateByMatrix.z);
	ImGui::End();
}
