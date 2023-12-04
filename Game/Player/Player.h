#pragma once
#include "math/Vector2.h"
#include "math/Matrix4x4.h"
#include "Engine/Texture/Texture2D.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include <memory>

class Player {
public:
	Player() = default;
	~Player() = default;
public:
	void Init();
	void Update();
	void Draw(const Matrix4x4& viewProjection);

	// キーごとにコマンドを作る
	void MoveRight();
	void MoveLeft();

private:
	Vector2 pos_;
	float speed_;
	WorldTransform transform_;
	std::unique_ptr<Texture2D> box_;

};