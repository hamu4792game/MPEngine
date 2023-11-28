#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "Engine/Camera/Camera.h"
#include "Game/Stage/Stage.h"
#include "math/AABB/AABB.h"
//#include "math/OBB/OBB.h"
#include "math/Sphere/Sphere.h"
#include <vector>
#include <optional>
#include <array>

class LockOn;

class Player
{
public:
	Player(Camera* camera);
	~Player() = default;

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	bool EnemyColl(AABB* enemy);

private: // メンバ変数
	// player関連
	WorldTransform playerTrans_;
	//	宙に浮いているか
	bool isFloating_ = false;
	//	ジャンプしているか
	bool isJamp_ = false;
	//	playerのモデル
	std::vector<std::shared_ptr<Model>> model_;
	//	parts
	std::vector<WorldTransform> parts_;

	//	武器の座標
	WorldTransform weaponTrans_;

	float velocity_ = 0.0f;

	//	
	float destinationAngleY_ = 0.0f;
	//	追従対象の残像座標
	Vector3 interTarget_;
	//	補間レートT
	float cameraT_ = 0.0f;

	//	aabb
	AABB aabb_;
	//OBB obb_;
	Sphere sphere_;

	//	カメラのポインタ
	Camera* camera_ = nullptr;
	//	床のポインタ
	Stage* stage_ = nullptr;

	//	振る舞い
	enum class Behavior {
		kRoot,	// 通常攻撃
		kAttack,// 攻撃中
		kDash,	// ダッシュ中
	};
	Behavior behavior_ = Behavior::kRoot;
	//	次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	uint16_t attackFrame = 0u;

	//	ダッシュ用ワーク : ダッシュ用のメンバ変数
	struct WorkDash	{
		// ダッシュ用の媒介変数
		uint32_t dashParameter_ = 0u; // 現在のフレーム
		float velocity_ = 1.0f; // ダッシュ速度
		uint32_t behaviorDashTime_ = 30u; // ダッシュ時間
		uint32_t delayTime_ = 15u;
	};
	WorkDash workDash_;
	Vector3 offset_ = Vector3(0.0f, 2.0f, -30.0f);

	bool resetFlag_ = false;

	// 攻撃用定数
	struct ConstAttack {
		uint32_t anticipationTime; // 振りかぶりの時間<frame>
		uint32_t chargeTime; // ため時間<frame>
		uint32_t swingTime; // 攻撃振りの時間
		uint32_t recoveryTime; // 硬直時間
		float anticipationSpeed; // 振りかぶりの移動速度
		float chargeSpeed; // ための移動速度
		float swingSpeed; // 攻撃振りの移動速度
	};
	// 攻撃用ワーク
	struct WorkAttack {
		// 攻撃ギミックの媒介変数
		uint32_t attackParameter_ = 0;
		int32_t comboIndex_ = 0;
		int32_t inComboPhase_ = 0;
		bool comboNext_ = false;
	};
	WorkAttack workAttack_;

	const LockOn* lockOn_ = nullptr;

	Vector3 preMove_;

public:
	// コンボの数
	static const int kComboNum = 3;
	// コンボ定数表
	static const std::array<ConstAttack, kComboNum> kConstAttacks_;

private: // メンバ関数
	//	プレイヤーの移動
	void Move();
	//	ジャンプした時の処理
	void Jamp();
	//	衝突判定
	void Collision();
	//	移動制御
	void MoveLimit();
	//	カメラの移動
	void CameraMove();

	//	プレイヤーリセット
	void PlayerReset();
	
	//	通常行動初期化
	void InitializeRoot();
	//	攻撃行動初期化
	void InitializeAttack();
	//	ダッシュ行動初期化
	void InitializeDash();


	//	通常行動更新
	void BehaviorRootUpdate();
	//	攻撃行動更新
	void BehaviorAttackUpdate();
	//	ダッシュ行動更新
	void BehaviorDashUpdate();

	//	更新
	void UpdateTransform();

	void ApplyGlobalVariables();

	void GetPhase();

public: // セッター
	void SetPlayerModel(std::vector<std::shared_ptr<Model>> player) { model_ = player; };
	void SetStagePtr(Stage* stage) { stage_ = stage; };
	bool GetResetFlag() { return resetFlag_; }

	void SetLockOn(const LockOn* target) { lockOn_ = target; }

};
