#include "Quaternion.h"
#include <cmath>

Quaternion::Quaternion() {
	*this = IdentityQuaternion();
}

Quaternion::Quaternion(const float& x, const float& y, const float& z, const float& w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion& Quaternion::operator=(const Quaternion& quaternion) {
	this->x = quaternion.x;
	this->y = quaternion.y;
	this->z = quaternion.z;
	this->w = quaternion.w;
	return *this;
}

Quaternion& Quaternion::operator=(const Vector3& vec) {
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
	return *this;
}

Quaternion Quaternion::Multiply(const Quaternion& quaternion) const {
	Quaternion result;
	Vector3 qVec(this->x, this->y, this->z);
	Vector3 rVec(quaternion.x, quaternion.y, quaternion.z);
	Vector3 resultVec = Cross(qVec, rVec) + (qVec * quaternion.w) + (rVec * this->w);
	result = resultVec;
	result.w = this->w * quaternion.w - (this->x * quaternion.x + this->y * quaternion.y + this->z * quaternion.z);
	return result;
}

Quaternion Quaternion::IdentityQuaternion() const {
	return Quaternion(0.0f,0.0f,0.0f,1.0f);
}

Quaternion Quaternion::Conjugate() const {
	Vector3 qVec(this->x, this->y, this->z);
	Quaternion result;
	result = -qVec;
	result.w = this->w;
	return result;
}

float Quaternion::Norm() const {
	float result;
	result = std::sqrtf(this->w * this->w + this->x * this->x + this->y * this->y + this->z * this->z);
	return result;
}

Quaternion Quaternion::Normalize() const {
	Quaternion result;
	float length = this->Norm();
	if (length == 0.0f) { result = IdentityQuaternion(); }
	length = 1.0f / length;
	result.x = this->x * length;
	result.y = this->y * length;
	result.z = this->z * length;
	result.w = this->w * length;

	return result;
}

Quaternion Quaternion::Inverse() const {
	Quaternion result;
	Quaternion conj = this->Conjugate();
	float length = this->Norm() * this->Norm();
	result.x = conj.x / length;
	result.y = conj.y / length;
	result.z = conj.z / length;
	result.w = conj.w / length;
	return result;
}
