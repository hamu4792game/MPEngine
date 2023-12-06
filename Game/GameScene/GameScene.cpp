#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include <string>

GameScene* GameScene::GetInstance() {
	static GameScene instance;
	return &instance;
}

void GameScene::Initialize() {
	q1 = Quaternion(2.0f, 3.0f, 4.0f, 1.0f);
	q2 = Quaternion(1.0f, 3.0f, 5.0f, 2.0f);
	identity = identity.IdentityQuaternion();
	conj = q1.Conjugate();
	inv = q1.Inverse();
	normal = q1.Normalize();
	mul1 = q1.Multiply(q2);
	mul2 = q2.Multiply(q1);
	norm = q1.Norm();
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
	ImGui::Text("%.2f : %.2f : %.2f : %.2f : Identity", identity.x, identity.y, identity.z, identity.w);
	ImGui::Text("%.2f : %.2f : %.2f : %.2f : Conjugate", conj.x, conj.y, conj.z, conj.w);
	ImGui::Text("%.2f : %.2f : %.2f : %.2f : Inverse", inv.x, inv.y, inv.z, inv.w);
	ImGui::Text("%.2f : %.2f : %.2f : %.2f : Normal", normal.x, normal.y, normal.z, normal.w);
	ImGui::Text("%.2f : %.2f : %.2f : %.2f : Multiply(q1,q2)", mul1.x, mul1.y, mul1.z, mul1.w);
	ImGui::Text("%.2f : %.2f : %.2f : %.2f : Multiply(q2,q1)", mul2.x, mul2.y, mul2.z, mul2.w);
	ImGui::Text("%.2f : Norm", norm);
	ImGui::End();
}
