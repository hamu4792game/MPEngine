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
	Quaternion rotation0;
	Quaternion rotation1;

	Quaternion interpolate0;
	Quaternion interpolate1;
	Quaternion interpolate2;
	Quaternion interpolate3;
	Quaternion interpolate4;
	
};
