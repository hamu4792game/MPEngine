#include "Engine/Camera/Camera.h"
#include "Engine/Engine.h"
#include <numbers>

Camera::Camera(float farClip_, bool proType) {
	farZ = farClip_;
	type = proType;
	if (type) {
		projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(Engine::GetInstance()->WindowWidth) / float(Engine::GetInstance()->WindowHeight), 0.1f, farZ);
	}
	else {
		projectionMatrix = MakeOrthographicMatrix(-float(Engine::GetInstance()->WindowWidth / 2), float(Engine::GetInstance()->WindowHeight / 2),
			float(Engine::GetInstance()->WindowWidth / 2), -float(Engine::GetInstance()->WindowHeight / 2), 0.01f, farZ);
	}
}

void Camera::CreateBillboard() {
	//	180度回転させる
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	//	行列の生成
	billboardMatrix = backToFrontMatrix * cameraMatrix;
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;
}

void Camera::Update() {

}

Matrix4x4 Camera::GetViewProMat() {
	//	行列の計算
	cameraMatrix = transform.UpdateMatrix();
	viewMatrix = Inverse(cameraMatrix);
	viewProjectionMatrix = viewMatrix * projectionMatrix;
	//	ビルボードも作る
	CreateBillboard();

	return viewProjectionMatrix;
}

void Camera::SetPositionWork(Vector3 translate, Vector3 rotate) {
	finishTranslate_ = translate;
	finishRotate_ = rotate;
}
