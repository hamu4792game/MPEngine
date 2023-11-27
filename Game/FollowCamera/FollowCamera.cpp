#include "FollowCamera.h"
#include "Engine/Easing/Easing.h"
#include <algorithm>
#include "Game/LockOn/LockOn.h"

FollowCamera* FollowCamera::GetInstance() {
	static FollowCamera instance;
	return &instance;
}

void FollowCamera::SetTarget(const WorldTransform* target) {
	target_ = target;
	Reset();
}

void FollowCamera::Update() {

	offset_ = TargetOffset(offset_, camera_->transform.rotation_);

	//	追従座標の補間
	//	ダッシュの時間<frame>
	//workDash_.behaviorDashTime_ = 120u;
	

	if (target_) {
		cameraT_ += 1.0f / 20.0f;
		cameraT_ = std::clamp(cameraT_, 0.0f, 1.0f);
		float T = Easing::EaseOutBack(cameraT_);
		interTarget_ = Lerp(interTarget_, target_->translation_, T);
	}

	if (lockOn_) {
		Vector3 lockPos = lockOn_->GetTargetPosition();
		Vector3 sub = lockPos - target_->GetTranslate();
		camera_->transform.rotation_.y = std::atan2f(sub.x, sub.z);
	}
	camera_->transform.translation_ = interTarget_ + offset_;
	camera_->transform.UpdateMatrix();

}

void FollowCamera::Reset() {
	if (target_) {
		//	追従座標、角度の初期化
		interTarget_ = target_->translation_;
		camera_->transform.rotation_.y = target_->rotation_.y;
	}

	destinationAngleY_ = camera_->transform.rotation_.y;
	//	追従対象からのオフセット
	offset_ = TargetOffset(offset_, camera_->transform.rotation_);
	camera_->transform.translation_ = interTarget_ + offset_;
	camera_->transform.UpdateMatrix();

}
