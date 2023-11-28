#include "Floor.h"
#include <externals/imgui/imgui.h>

Floor::Floor() {
	aabb = std::make_unique<AABB>();
}

void Floor::Initialize(Vector3 translate) {
	transform_.scale_ = Vector3(30.0f, 0.1f, 25.0f);
	transform_.translation_ = translate;
	//	座標 - scale * size
	aabb->Update(transform_);
}

void Floor::Update() {

#ifdef _DEBUG
	ImGui::Begin("floor");
	ImGui::DragFloat3("scale", &transform_.scale_.x, 0.1f);
	ImGui::DragFloat3("translate", &transform_.translation_.x, 1.0f);
	ImGui::DragFloat3("rotate", &transform_.rotation_.x, AngleToRadian(1.0f));
	ImGui::End();
#endif DEBUG

	transform_.UpdateMatrix();
	aabb->Update(transform_);
}

void Floor::Draw(const Matrix4x4& viewProjection) {
	Model::ModelDraw(transform_, viewProjection, 0xffffffff, floorModel_);
	aabb->DrawAABB(viewProjection, 0xffffffff);
}
