#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include <string>

GameScene* GameScene::GetInstance() {
	static GameScene instance;
	return &instance;
}

void GameScene::Initialize() {
	axis = Normalize(Vector3::one);
	angle = 0.44f;
	rotateMat = MakeRotateAxisAngle(axis, angle);

	from0 = Normalize(Vector3(1.0f, 0.7f, 0.5f));
	to0 = -from0;
	from1 = Normalize(Vector3(-0.6f, 0.9f, 0.2f));
	to1 = Normalize(Vector3(0.4f, 0.7f, -0.5f));
	rotateMatrix0 = DirectionToDirection(Normalize(Vector3(1.0f, 0.0f, 0.0f)), Normalize(Vector3(-1.0f, 0.0f, 0.0f)));
	rotateMatrix1 = DirectionToDirection(from0, to0);
	rotateMatrix2 = DirectionToDirection(from1, to1);
	
}

void GameScene::Update() {
#ifdef _DEBUG
	DrawImGui();

#endif DEBUG
	

}

void GameScene::Draw() {
	
}

void GameScene::DrawMatrix4x4(Matrix4x4 mat) {
	ImGui::Text("%.3f : %.3f : %.3f : %.3f", mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3]);
	ImGui::Text("%.3f : %.3f : %.3f : %.3f", mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3]);
	ImGui::Text("%.3f : %.3f : %.3f : %.3f", mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3]);
	ImGui::Text("%.3f : %.3f : %.3f : %.3f", mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3]);
}

void GameScene::DrawImGui() {
	ImGui::Begin("Matrix");
	ImGui::Text("rotateMatrix0");
	DrawMatrix4x4(rotateMatrix0);
	ImGui::Text("rotateMatrix1");
	DrawMatrix4x4(rotateMatrix1);
	ImGui::Text("rotateMatrix2");
	DrawMatrix4x4(rotateMatrix2);
	ImGui::End();
}
