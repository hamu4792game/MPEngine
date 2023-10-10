#pragma once
#include "Engine/Texture/Model.h"
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include "math/AABB/AABB.h"
#include "Engine/Texture/Line/Line.h"

class Floor
{
public:
	Floor();
	~Floor() = default;

	void Initialize(Vector3 translate);

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	

private: 
	//	モデル
	Model* floorModel_;

public:
	//　座標
	WorldTransform transform_;
	std::unique_ptr<AABB> aabb;

public: // セッター
	void SetFloorModel(Model* model) { floorModel_ = model; };

};
