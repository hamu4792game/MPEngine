#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "Engine/Camera/Camera.h"
#include "Game/Player/Player.h"
#include "Game/Stage/Stage.h"
#include "Game/Enemy/Enemy.h"

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
	Model* floorModel_;

private: // メンバ変数
	std::unique_ptr<Player> player_;
	std::shared_ptr<Stage> stage_;
	std::shared_ptr<Enemy> enemy_;

	//	カメラのポインタ
	Camera* camera_;

private: //	メンバ関数


public: // ゲッター
	void SetPlayerModel(std::vector<std::shared_ptr<Model>> model) { player_->SetPlayerModel(model); }
	void SetFloorModel(Model* model) { floorModel_ = model; };


};

