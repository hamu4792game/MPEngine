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
	Quaternion q1;
	Quaternion q2;
	Quaternion identity;
	Quaternion conj;
	Quaternion inv;
	Quaternion normal;
	Quaternion mul1;
	Quaternion mul2;
	float norm = 0.0f;

};
