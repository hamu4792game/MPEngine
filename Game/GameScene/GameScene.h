#pragma once
#include "math/Matrix4x4.h"

class GameScene
{
public:
	GameScene() = default;
	~GameScene() = default;

	static GameScene* GetInstance();

	void Initialize();

	void Update();

	void Draw();
private:


};
