#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "Engine/Camera/Camera.h"
#include "Game/Player/Player.h"

class Battle
{
public:
	Battle(Camera* camera);
	~Battle() = default;

	void Initialize();

	void Update();

	void Draw3D(const Matrix4x4& viewProjection);

private: // モデル
	Model* playerModel_;

private: // メンバ変数
	std::unique_ptr<Player> player_;

	//	カメラのポインタ
	Camera* camera_;

private: //	メンバ関数


public: // ゲッター
	void SetPlayerModel(Model* model) { playerModel_ = model; };


};

