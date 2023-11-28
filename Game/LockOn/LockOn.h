#pragma once
#include "math/Matrix4x4.h"
#include "Engine/Texture/Texture2D.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Game/Enemy/Enemy.h"
#include "Engine/Camera/Camera.h"

class LockOn {
public:
	LockOn();
	~LockOn() = default;

	void Initialize();
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, Camera* camera, const Matrix4x4& view);
	void Draw2D(const Matrix4x4& viewProjection);

	Vector3 GetTargetPosition() const;
	bool ExistTarget() const { return target_ ? true : false; }

	// true : 自動/false : 手動
	bool isAutoLockOn_ = false;

private:
	// ロックオンマーク用スプライト
	std::unique_ptr<Texture2D> lockOnMark_;
	WorldTransform lockOnMarkTransform_;

	// ロックオン対象
	const Enemy* target_ = nullptr;
	// 目標
	std::list<std::pair<float, const Enemy*>> targets;
	int num = 0;

	float minDistance_ = 20.0f; // 最小距離
	float maxDistance_ = 80.0f; // 最大距離
	float angleRange_ = AngleToRadian(90.0f); // 角度範囲
private:
	void Search(const std::list<std::unique_ptr<Enemy>>& enemies, const Matrix4x4& view);

	bool DeterminationWithOutArea(const Matrix4x4& viewProjecton);

	Vector3 ChangeScreen(const Vector3& worldPos, Camera* camera);

};
