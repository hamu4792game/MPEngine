#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "Engine/Camera/Camera.h"
#include "Game/Player/Player.h"
#include "Game/Stage/Stage.h"
#include "Game/Enemy/Enemy.h"
#include "Game/LockOn/LockOn.h"

#include <list>

class Battle
{
public:
	Battle(Camera* camera, Camera* camera2d);
	~Battle() = default;

	void Initialize();

	void Update();

	void Draw3D(const Matrix4x4& viewProjection);
	void Draw2D(const Matrix4x4& viewProjection);

private: // モデル
	Model* playerModel_;
	std::vector<std::shared_ptr<Model>> enemyModel_;
	Model* floorModel_;

private: // メンバ変数
	std::unique_ptr<Player> player_;
	std::shared_ptr<Stage> stage_;
	std::list<std::unique_ptr<Enemy>> enemys_;
	std::unique_ptr<LockOn> lockOn_;

	//	カメラのポインタ
	Camera* camera_;
	Camera* camera2d_;

private: //	メンバ関数

	void EnemySet();

public: // ゲッター
	void SetPlayerModel(std::vector<std::shared_ptr<Model>> model) { player_->SetPlayerModel(model); }
	void SetEnemyModel(std::vector<std::shared_ptr<Model>> model) { enemyModel_ = model; }
	void SetFloorModel(Model* model) { floorModel_ = model; };


};

