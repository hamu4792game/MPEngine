#include "InputHandle.h"
#include "Engine/Input/KeyInput/KeyInput.h"

ICommand* InputHandle::HandleInput() {
	if (KeyInput::GetInstance()->PressKey(DIK_A)) {
		return pressKeyA_;
	}
	else if (KeyInput::GetInstance()->PressKey(DIK_D)) {
		return pressKeyD_;
	}
	return nullptr;
}

void InputHandle::AssignMoveLeftCommand2PressKeyA() {
	ICommand* command = new MoveLeftCommand();
	this->pressKeyA_ = command;
}

void InputHandle::AssignMoveRightCommand2PressKeyD() {
	ICommand* command = new MoveRightCommand();
	this->pressKeyD_ = command;
}
