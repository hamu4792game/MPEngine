#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "Engine/Camera/Camera.h"
#include "Game/Stage/Stage.h"
#include "math/AABB/AABB.h"
#include <vector>
#include <optional>

class Player
{
public:
	Player(Camera* camera);
	~Player() = default;

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	void EnemyColl(AABB* enemy);

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

	float velocity_ = 0.0f;

	//	aabb
	AABB aabb_;

	//	カメラのポインタ
	Camera* camera_ = nullptr;
	//	床のポインタ
	Stage* stage_ = nullptr;

	//	振る舞い
	enum class Behavior {
		kRoot,	// 通常攻撃
		kAttack,// 攻撃中
	};
	Behavior behavior_ = Behavior::kRoot;
	//	次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	uint16_t attackFrame = 0u;

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

	void InitializeRoot();
	void InitializeAttack();

	//	通常行動更新
	void BehaviorRootUpdate();
	//	攻撃行動更新
	void BehaviorAttackUpdate();

	void ApplyGlobalVariables();

public: // セッター
	void SetPlayerModel(std::vector<std::shared_ptr<Model>> player) { model_ = player; };
	void SetStagePtr(Stage* stage) { stage_ = stage; };

};
