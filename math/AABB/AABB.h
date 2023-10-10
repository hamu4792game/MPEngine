#pragma once
#include "math/Vector3.h"
#include "Engine/Texture/Line/Line.h"

//	AABB
class AABB
{
public:
	AABB() = default;
	~AABB() = default;

	Vector3 min;	// 最小点
	Vector3 max;	// 最大点
	Line line[12];

public:
	void Update(WorldTransform& transform);
	//	AABB同士の衝突判定
	bool IsCollision(AABB* aabb2);
	//	AABBの描画
	void DrawAABB(const Matrix4x4& viewProjectionMatrix, uint32_t color);

};

