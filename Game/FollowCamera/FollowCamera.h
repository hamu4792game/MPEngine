#pragma once
#include "Engine/Camera/Camera.h"

class LockOn;

class FollowCamera {
public:
	FollowCamera() = default;
	~FollowCamera() = default;
	
	static FollowCamera* GetInstance();
	void SetCamera(Camera* camera) { camera_ = camera; }
	void SetTarget(const WorldTransform* target);
	void SetLockOn(const LockOn* target) { lockOn_ = target; };
	WorldTransform GetTransform() { return camera_->transform; }

public:
	void Update();
	void Reset();

private:
	Camera* camera_ = nullptr;
	const WorldTransform* target_;
	Vector3 offset_ = Vector3(0.0f, 2.0f, -20.0f);
	const LockOn* lockOn_ = nullptr;

	Vector3 interTarget_;
	float destinationAngleY_ = 0.0f;
	float cameraT_ = 0.0f;

};
