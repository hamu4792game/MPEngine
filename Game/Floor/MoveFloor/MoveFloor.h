#pragma once
#include "Game/Floor/Floor.h"
#include "math/Vector3.h"

class MoveFloor : public Floor
{
public:
	MoveFloor() = default;
	~MoveFloor() = default;

	void Initialize(Vector3 translate);

	void Update();

	Vector3 move;
private:
	bool flag;

};
