#pragma once
#include "Game/Command/Command.h"

class InputHandle {
public:
	InputHandle() = default;
	~InputHandle() = default;

	ICommand* HandleInput();

	void AssignMoveLeftCommand2PressKeyA();
	void AssignMoveRightCommand2PressKeyD();

private:
	ICommand* pressKeyA_;
	ICommand* pressKeyD_;

};
