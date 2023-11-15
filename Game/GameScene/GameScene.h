#pragma once
#include "math/Vector3.h"
#include "math/Matrix4x4.h"

class GameScene {
public:
	GameScene() = default;
	~GameScene() = default;

	static GameScene* GetInstance();

	void Initialize();

	void Update();

	void Draw();
private:
	void DrawMatrix4x4(Matrix4x4 mat);
	void DrawImGui();

private:
	Vector3 axis;
	float angle = 0.44f;
	Matrix4x4 rotateMat;

};
