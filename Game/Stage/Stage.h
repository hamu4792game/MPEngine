#pragma once
#include "math/Matrix4x4.h"
#include "Game/Floor/Floor.h"
#include "Game/Floor/MoveFloor/MoveFloor.h"
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "math/AABB/AABB.h"

class Stage
{
public:
	Stage();
	~Stage() = default;

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

public:
	Floor floor_[2];
	MoveFloor moveFloor_;

	std::unique_ptr<Model> goal;
	WorldTransform goalTrans;
	std::unique_ptr<AABB> goalAABB;

public: // セッター
	void SetFloorModel(Model* model);

};
