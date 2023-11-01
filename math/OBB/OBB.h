#pragma once
#include "Engine/Texture/Line/Line.h"

class OBB {
public:
	OBB();
	~OBB();
	Line line[12];
	Vector3 center;	// 中心点
	Vector3 orientations[3]; // 座標軸。正規化・直交必須
	Vector3 size = Vector3(1.0f, 1.0f, 1.0f);	// 座標軸方向の長さの半分。中心から面までの距離

public:

	//	衝突判定の描画
	void DrawOBB(const Matrix4x4& viewProjectionMatrix, uint32_t color);

};