#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Texture/Model.h"

class Player
{
public:
	Player(Camera* camera);
	~Player() = default;

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);


private: // メンバ変数
	// player関連
	WorldTransform playerTrans_;
	//	宙に浮いているか
	bool isFloating_ = false;
	//	ジャンプしているか
	bool isJamp_ = false;
	//	playerのモデル
	Model* model_;

	float velocity_;

	//	カメラのポインタ
	Camera* camera_;

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


public: // セッター
	void SetPlayerModel(Model* player) { model_ = player; };


};
