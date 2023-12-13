#pragma once
#include "math/Vector3.h"
#include "math/Matrix4x4.h"
#include "math/Quaternion.h"

class GameScene {
public:
	GameScene() = default;
	~GameScene() = default;

	static GameScene* GetInstance();

	void Initialize();

	void Update();

	void Draw();
private:
	void DrawImGui();

private:
	Quaternion rotation;
	Vector3 pointY;
	Matrix4x4 rotateMatrix;
	Vector3 rotateByQuaternion;
	Vector3 rotateByMatrix;

};
