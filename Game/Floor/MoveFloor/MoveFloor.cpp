#include "MoveFloor.h"
#include <externals/imgui/imgui.h>

void MoveFloor::Initialize(Vector3 translate)
{
	transform_.scale_ = Vector3(10.0f, 0.1f, 10.0f);
	transform_.translation_ = translate;
	//	座標 - scale * size
	aabb->Update(transform_);
	move = Vector3(0.0f, 0.0f, 0.0f);
	flag = false;
}

void MoveFloor::Update()
{
#ifdef _DEBUG
	ImGui::Begin("movefloor");
	ImGui::DragFloat3("scale", &transform_.scale_.x, 0.1f);
	ImGui::DragFloat3("translate", &transform_.translation_.x, 1.0f);
	ImGui::DragFloat3("rotate", &transform_.rotation_.x, AngleToRadian(1.0f));
	ImGui::End();
#endif DEBUG

	move.x = 0.0f;
	if (transform_.translation_.x <= 0.0f) {
		flag = true;
	}
	else if (transform_.translation_.x >= 10.0f) {
		flag = false;
	}

	if (flag) {
		move.x += 0.1f;
	}
	else {
		move.x -= 0.1f;
	}
	transform_.translation_ += move;
	transform_.UpdateMatrix();
	aabb->Update(transform_);
}
