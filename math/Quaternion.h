#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

class Quaternion {
public:
	Quaternion();
	Quaternion(const float& x, const float& y, const float& z, const float& w);
	~Quaternion() = default;

public:
	float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;

public:
	Quaternion operator*(const Quaternion& quaternion) const;
	Quaternion& operator=(const Quaternion& quaternion);
	Quaternion& operator=(const Vector3& vec);

public:
	// 積
	Quaternion Multiply(const Quaternion& quaternion) const;
	// 単位Quaternionを返す
	Quaternion IdentityQuaternion() const;
	// 共役Quaternionを返す
	Quaternion Conjugate() const;
	// Quaternionのnormを返す
	float Norm() const;
	// 正規化したQuaternionを返す
	Quaternion Normalize() const;
	// 逆Quaternionを返す
	Quaternion Inverse() const;
	// 任意軸回転を表すQuaternionの生成
	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& vector, const float& angle);
	// ベクトルをQuaternionで回転させた結果のベクトルを求める
	static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
	// Quaternionから回転行列を求める
	static Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

};