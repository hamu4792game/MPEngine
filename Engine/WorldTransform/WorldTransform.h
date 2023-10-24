#pragma once
#include "math/Matrix4x4.h"
#include "math/Vector4.h"
#include "math/Vector3.h"
#include "math/Vector2.h"
#include "Engine/Base/ConstantBuffer.h"

class WorldTransform
{
public:
	WorldTransform();
	~WorldTransform() = default;
	WorldTransform(const WorldTransform& transform);

	WorldTransform& operator=(const WorldTransform &trans);

public:

	ConstantBuffer<Matrix4x4> cMat;
	ConstantBuffer<Vector4> cColor;
	struct Mono {
		Vector2 pibot;
		float rate;
	};
	//	全て動かしたい時用の変数
	static Mono monocuro;

	ConstantBuffer<Mono> cMono;

	// ローカルスケール
	Vector3 scale_ = { 1.0f, 1.0f, 1.0f };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0.0f, 0.0f, 0.0f };
	// ローカル座標
	Vector3 translation_ = { 0.0f, 0.0f, 0.0f };
	// ローカル → ワールド変換行列
	Matrix4x4 worldMatrix;
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;

	//	更新
	Matrix4x4 UpdateMatrix();
	//	リセット
	void Reset();

	//	平行移動行列の平行移動の取得
	Vector3 GetTranslate() const { return Vector3(worldMatrix.m[3][0], worldMatrix.m[3][1], worldMatrix.m[3][2]); };

};
