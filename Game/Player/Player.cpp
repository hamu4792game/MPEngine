#include "Player.h"
#include <externals/imgui/imgui.h>
#include "Engine/Input/KeyInput/KeyInput.h"
#include "GlobalVariables/GlobalVariables.h"
#include <algorithm>
#include "Engine/Easing/Easing.h"
#include "Game/LockOn/LockOn.h"

// コンボ定数表
const std::array<Player::ConstAttack, Player::kComboNum> Player::kConstAttacks_ = {
	{
		{0,0,20,20,0.0f,0.0f,0.0f},
		{0,0,20,20,0.0f,0.0f,0.0f},
		{10,20,20,30,0.0f,0.0f,0.0f},
	}
};

Player::Player(Camera* camera)
{
	camera_ = camera;
	camera_->transform.rotation_.x = AngleToRadian(20.0f);
}

void Player::Initialize() {
	//	メンバ変数の初期化
	playerTrans_.translation_ = Vector3(0.0f, 1.0f, 0.0f);
	playerTrans_.rotation_ = Vector3::zero;
	isFloating_ = false;
	isJamp_ = false;
	destinationAngleY_ = 0.0f;

	parts_.resize(model_.size());
	for (auto& i : parts_) {
		i.parent_ = &parts_[0];
	}
	parts_[0].parent_ = &playerTrans_;
	//	パーツ毎の設定
	//parts_[0].translation_ = Vector3{ 0.0f,-1.3f,0.0f };
	//parts_[1].translation_ = Vector3{ 0.0f,1.5f,0.0f };
	//parts_[2].translation_ = Vector3{ -0.2f,1.7f,0.0f };
	//parts_[3].translation_ = Vector3{ 0.2f,1.7f,0.0f };
	parts_[4].translation_ = Vector3{ 0.0f,2.0f,0.0f };
	parts_[4].scale_ = Vector3{ 0.4f,0.4f,0.4f };

	weaponTrans_.parent_ = nullptr;
	weaponTrans_.translation_.y = 15.0f;

	const char* groupName = "Player";
	//	グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	GlobalVariables::GetInstance()->AddItem(groupName, "Body Translation", parts_[0].translation_);
	GlobalVariables::GetInstance()->AddItem(groupName, "Head Translation", parts_[1].translation_);
	GlobalVariables::GetInstance()->AddItem(groupName, "Rarm Translation", parts_[2].translation_);
	GlobalVariables::GetInstance()->AddItem(groupName, "Larm Translation", parts_[3].translation_);

	GlobalVariables::GetInstance()->AddItem(groupName, "Dash Time", static_cast<int>(workDash_.behaviorDashTime_));
	GlobalVariables::GetInstance()->AddItem(groupName, "Deray Time", static_cast<int>(workDash_.delayTime_));

	//	座標 - scale * size
	aabb_.Update(playerTrans_);
	//obb_.Update(playerTrans_);
	sphere_.Initialize(3.0f);
	sphere_.Update(weaponTrans_);

}

void Player::Update() {
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("scale", &playerTrans_.scale_.x, 0.1f);
	ImGui::DragFloat3("translate", &playerTrans_.translation_.x, 1.0f);
	ImGui::DragFloat3("rotate", &playerTrans_.rotation_.x, AngleToRadian(1.0f));
	ImGui::DragFloat3("direction", &playerTrans_.light.direction.x, 0.1f);
	ImGui::DragFloat("intensity", &playerTrans_.light.intensity, 0.1f);

	int dash = static_cast<int>(workDash_.behaviorDashTime_);
	ImGui::DragInt("dash", &dash, 1, 0, 500);
	workDash_.behaviorDashTime_ = dash;

	dash = static_cast<int>(workDash_.delayTime_);
	ImGui::DragInt("delay", &dash, 1, 0, 500);
	workDash_.delayTime_ = dash;

	ImGui::End();
#endif //DEBUG

	resetFlag_ = false;
	ApplyGlobalVariables();

	//	std::nullopt以外の時通る
	if (behaviorRequest_)
	{
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		//　振る舞いごとの初期化を実行
		switch (behavior_)
		{
		case Behavior::kRoot:
			InitializeRoot();
			break;
		case Behavior::kAttack:
			InitializeAttack();
			break;
		case Behavior::kDash:
			InitializeDash();
			break;
		}
		//	振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_)
	{
	case Behavior::kRoot:
	default:
		//	通常行動
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		//	攻撃処理
		BehaviorAttackUpdate();
		break;
	case Behavior::kDash:
		//	ダッシュ処理
		BehaviorDashUpdate();
		break;
	}

	Collision();
	MoveLimit();
	
	UpdateTransform();

	//	カメラの移動と更新
	CameraMove();

}


void Player::Draw(const Matrix4x4& viewProjection) {
	for (uint8_t i = 0; i < parts_.size() - 1u; i++) {
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, model_[i].get());
	}
	if (behavior_ == Behavior::kAttack) {
		Model::ModelDraw(parts_[4], viewProjection, 0xffffffff, model_[4].get());
		sphere_.Draw(viewProjection, 0xff0000ff);
	}

	aabb_.DrawAABB(viewProjection, 0xff0000ff);
	//obb_.DrawOBB(viewProjection, 0xff0000ff);
}

bool Player::EnemyColl(AABB* enemy) {
	if (aabb_.IsCollision(enemy)) {
		PlayerReset();
	}
	if (enemy->IsCollision(&sphere_)) {
		return true;
	}
	return false;
}


void Player::Move() {
	//	移動フラグ
	bool isMove = false;
	//	移動量
	Vector3 move(0.0f, 0.0f, 0.0f);
	const float speed = 0.5f;

	//	キーの取得
	if (KeyInput::GetKey(DIK_W) /*&& distance >= 15.0f*/)
	{
		move.z += speed;
		isMove = true;
	}
	if (KeyInput::GetKey(DIK_S))
	{
		move.z -= speed;
		isMove = true;
	}
	if (KeyInput::GetKey(DIK_D))
	{
		move.x += speed;
		isMove = true;
	}
	if (KeyInput::GetKey(DIK_A))
	{
		move.x -= speed;
		isMove = true;
	}
	//	padが接続されているなら
	if (KeyInput::GetInstance()->GetPadConnect()) {
		Vector2 pMove(0.0f, 0.0f);
		pMove = KeyInput::GetInstance()->GetPadLStick();
		//	移動があれば代入する
		if (pMove.x != 0.0f || pMove.y != 0.0f)
		{
			move.x = pMove.x;
			move.z = pMove.y;
			isMove = true;
		}
	}

	//	移動があれば更新
	if (isMove) {
		//	移動量の正規化
		move = Normalize(move);

		//	移動ベクトルをカメラの角度だけ回転させる
		move = TargetOffset(move, camera_->transform.rotation_);
		//	ジャンプを加味していないため
		move.y = 0.0f;
		move*= speed;
		
		//	移動方向に見た目を合わせる
		// atan2fは消せませんでした(泣
		playerTrans_.rotation_.y = FindAngle(move,Vector3(0.0f,0.0f,1.0f));
		destinationAngleY_ = playerTrans_.rotation_.y;
		//playerTrans_.rotateMatrix = MakeRotateAxisAngle(Vector3(0.0f,1.0f,0.0f), AngleToRadian(move.x));
		//playerTrans_.rotateMatrix = DirectionToDirection(move, preMove_);
		preMove_ = move;
		
		//	座標更新
		playerTrans_.translation_ += move;
		playerTrans_.UpdateMatrix();
	}
}

void Player::Jamp() {

	if (!isJamp_ && !isFloating_) {
		if (KeyInput::PushKey(DIK_SPACE)) {
			isJamp_ = true;
			//	初速度を与える
			velocity_ = 1.0f;
		}
		else if (KeyInput::GetInstance()->GetPadConnect()) {
			if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_A)) {
				isJamp_ = true;
				//	初速度を与える
				velocity_ = 1.0f;
			}
		}
		else {
			velocity_ = 0.0f;
		}
	}
	else {
		//	重力
		const float gravity_ = 0.1f;
		//	重力を足していく
		velocity_ -= gravity_;
	}
	playerTrans_.translation_.y += velocity_;
	playerTrans_.UpdateMatrix();
}

void Player::Collision() {
	/*
		地面に床があった時はisFloating = true;
		xz軸の矩形の判定を取り判断
		動く床はisJampがfalseなら親子関係
	*/
	
	//	座標 - scale * size
	aabb_.Update(playerTrans_);

	//	ゴールとの判定
	if (aabb_.IsCollision(stage_->goalAABB.get())) {
		PlayerReset();
		return;
	}

	//	動く床との処理
	if (aabb_.IsCollision(stage_->moveFloor_.aabb.get())) {
		isFloating_ = false;
		playerTrans_.translation_ += stage_->moveFloor_.move;
		playerTrans_.UpdateMatrix();
		return;
	}
	else {
		isFloating_ = true;
	}

	//	止まっている床との処理
	for (uint8_t i = 0; i < 2; i++)	{
		if (aabb_.IsCollision(stage_->floor_[i].aabb.get())) {
			isFloating_ = false;
			return;
		}
		else {
			isFloating_ = true;
		}
	}	
}

void Player::MoveLimit() {
	//	リスタート
	if (playerTrans_.translation_.y <= -20.0f) {
		PlayerReset();
		return;
	}

	//	宙に浮いている場合
	if (isFloating_) {
		return;
	}

	//	移動制御
	if (isJamp_ && (playerTrans_.translation_.y <= 1.0f)) {
		playerTrans_.translation_.y = 1.0f;
		isJamp_ = false;
	}

}

void Player::PlayerReset() {
	//	メンバ変数の初期化
	playerTrans_.translation_ = Vector3(0.0f, 1.0f, 0.0f);
	isFloating_ = false;
	isJamp_ = false;
	destinationAngleY_ = 0.0f;
	behaviorRequest_ = Behavior::kRoot;
	resetFlag_ = true;
}

void Player::CameraMove() {
	Vector3 offset;
	offset = TargetOffset(offset_, camera_->transform.rotation_);

	//	追従座標の補間
	//	ダッシュの時間<frame>
	//workDash_.behaviorDashTime_ = 30u;
	cameraT_ += 1.0f / workDash_.behaviorDashTime_;
	cameraT_ = std::clamp(cameraT_, 0.0f, 1.0f);
	//float T = Easing::EaseInSine(cameraT_);
	interTarget_ = Lerp(interTarget_, playerTrans_.translation_, cameraT_);

	camera_->transform.translation_ = interTarget_ + offset;

	if (lockOn_->ExistTarget()) {
		Vector3 lockPos = lockOn_->GetTargetPosition();
		Vector3 sub = lockPos - playerTrans_.translation_;
		camera_->transform.rotation_.y = FindAngle(sub, Vector3(0.0f, 0.0f, 1.0f));
	}
	
	camera_->transform.UpdateMatrix();
}

void Player::InitializeAttack() {
	attackFrame = 0;
	parts_[2].rotation_.x = -3.5f;
	parts_[3].rotation_.x = -3.5f;
	parts_[4].rotation_.x = 0.0f;

	weaponTrans_.parent_ = &parts_[4];

	// コンボ継続フラグをリセットする
	workAttack_.comboNext_ = false;
	// 攻撃のパラメーターリセット
	workAttack_.attackParameter_ = 0;
	workAttack_.inComboPhase_ = 0;
	workAttack_.comboIndex_ = 0;

}

void Player::InitializeRoot() {
	parts_[2].rotation_.x = 0.0f;
	parts_[3].rotation_.x = 0.0f;

	weaponTrans_.parent_ = nullptr;
}

void Player::InitializeDash() {
	workDash_.dashParameter_ = 0u;
	playerTrans_.rotation_.y = destinationAngleY_;
	//	追従座標、角度の初期化
	interTarget_ = playerTrans_.translation_;
	cameraT_ = 0.0f;
}

void Player::BehaviorRootUpdate() {
	Move();
	Jamp();
	if (KeyInput::PushKey(DIK_V)) {
		behaviorRequest_ = Behavior::kAttack;
	}
	// ダッシュボタンを押したら
	else if (KeyInput::PushKey(DIK_B) && !isFloating_) {
		// ダッシュリクエスト
		behaviorRequest_ = Behavior::kDash;
	}

	if (KeyInput::GetInstance()->GetPadConnect()) {
		if (KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B)) {
			// ダッシュリクエスト
			behaviorRequest_ = Behavior::kDash;
		}
		else if (KeyInput::GetInstance()->GetRTriggerDown()) {
			behaviorRequest_ = Behavior::kAttack;
		}
	}
}

void Player::BehaviorAttackUpdate() {
	// 予備動作の時間
	auto input = KeyInput::GetInstance();

	// コンボ上限に達していない
	if (workAttack_.comboIndex_ < kComboNum - 1) {
		// 攻撃ボタンをトリガーしたら
		if (KeyInput::PushKey(DIK_V)) {
			workAttack_.comboNext_ = true;
		}
	}
	
	// 既定の時間経過で通常行動に戻る
	uint32_t totalTime = kConstAttacks_[workAttack_.comboIndex_].anticipationTime + kConstAttacks_[workAttack_.comboIndex_].chargeTime
		+ kConstAttacks_[workAttack_.comboIndex_].recoveryTime + kConstAttacks_[workAttack_.comboIndex_].swingTime;
	if (++workAttack_.attackParameter_ >= totalTime) {
		// コンボ継続なら次のコンボに進む
		if (workAttack_.comboNext_) {
			// コンボ継続フラグをリセットする
			workAttack_.comboNext_ = false;
			workAttack_.comboIndex_++;
			if (workAttack_.comboIndex_ >= kComboNum) {
				workAttack_.comboIndex_ = 0;
			};
			// 攻撃のパラメーターリセット
			workAttack_.attackParameter_ = 0;
			workAttack_.inComboPhase_ = 0;
		}
		// コンボ継続出ないなら終了
		else {
			behaviorRequest_ = Behavior::kRoot;
		}
	}

	GetPhase();

	// コンボ段階によってモーションを分岐
	switch (workAttack_.comboIndex_) {
	case 0:
		if (workAttack_.inComboPhase_ == 2) {
			parts_[2].rotation_.x += 0.1f;
			parts_[3].rotation_.x += 0.1f;
			parts_[4].rotation_.x += 0.1f;
		}
		break;
	case 1:
		if (workAttack_.inComboPhase_ == 2) {
			parts_[2].rotation_.x -= 0.1f;
			parts_[3].rotation_.x -= 0.1f;
			parts_[4].rotation_.x -= 0.1f;
		}
		break;
	case 2:

		if (workAttack_.inComboPhase_ == 0) {
			float a = 2.0f / kConstAttacks_[workAttack_.comboIndex_].anticipationTime;
			parts_[2].rotation_.x += a;
			parts_[3].rotation_.x += a;
			parts_[4].rotation_.x += a;
			playerTrans_.rotation_.y -= AngleToRadian(90.0f / kConstAttacks_[workAttack_.comboIndex_].anticipationTime);
		}
		else if (workAttack_.inComboPhase_ == 1) {
			playerTrans_.rotation_.y += AngleToRadian(270.0f / kConstAttacks_[workAttack_.comboIndex_].chargeTime);
		}
		else if (workAttack_.inComboPhase_ == 2) {
			float a = 1.5f / kConstAttacks_[workAttack_.comboIndex_].swingTime;
			parts_[2].rotation_.x -= a;
			parts_[3].rotation_.x -= a;
			parts_[4].rotation_.x -= a;
			playerTrans_.rotation_.y += AngleToRadian(180.0f / kConstAttacks_[workAttack_.comboIndex_].swingTime);
		}
		break;
	}

}

void Player::BehaviorDashUpdate() {
	//	自キャラの向いている方向に移動する処理
	//	移動量の正規化
	Vector3 move = Vector3(0.0f, 0.0f, workDash_.velocity_);
	move = Normalize(move) * workDash_.velocity_;
	//	移動ベクトルをカメラの角度だけ回転させる
	move = TransformNormal(move, MakeRotateYMatrix(destinationAngleY_));
	//	ジャンプを加味していないため
	move.y = 0.0f;

	playerTrans_.translation_ += move;
	playerTrans_.UpdateMatrix();

	workDash_.dashParameter_++;
	//	規定の時間経過で通常行動に戻る
	if (workDash_.dashParameter_ >= workDash_.behaviorDashTime_) {
		behaviorRequest_ = Behavior::kRoot;
	}
	else if (workDash_.dashParameter_ <= workDash_.delayTime_) {
		cameraT_ = 0u;
	}

}

void Player::ApplyGlobalVariables() {
	GlobalVariables* globalManagement = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	parts_[0].translation_ = globalManagement->GetVector3Value(groupName, "Body Translation");
	parts_[1].translation_ = globalManagement->GetVector3Value(groupName, "Head Translation");
	parts_[2].translation_ = globalManagement->GetVector3Value(groupName, "Rarm Translation");
	parts_[3].translation_ = globalManagement->GetVector3Value(groupName, "Larm Translation");


	workDash_.behaviorDashTime_ = globalManagement->GetIntValue(groupName, "Dash Time");
	workDash_.delayTime_ = globalManagement->GetIntValue(groupName, "Deray Time");

}

void Player::GetPhase() {
	uint32_t totalTime = kConstAttacks_[workAttack_.comboIndex_].anticipationTime;
	if (workAttack_.attackParameter_ < totalTime) {
		workAttack_.inComboPhase_ = 0;
		return;
	}
	totalTime += kConstAttacks_[workAttack_.comboIndex_].chargeTime;
	if (workAttack_.attackParameter_ < totalTime) {
		workAttack_.inComboPhase_ = 1;
		return;
	}
	totalTime += kConstAttacks_[workAttack_.comboIndex_].swingTime;
	if (workAttack_.attackParameter_ < totalTime) {
		workAttack_.inComboPhase_ = 2;
		return;
	}
	totalTime += kConstAttacks_[workAttack_.comboIndex_].recoveryTime;
	if (workAttack_.attackParameter_ < totalTime) {
		workAttack_.inComboPhase_ = 3;
		return;
	}
}

void Player::UpdateTransform() {
	//	座標更新
	playerTrans_.UpdateMatrix();
	for (auto& i : parts_) {
		i.UpdateMatrix();
	}
	weaponTrans_.UpdateMatrix();

	//	座標 - scale * size
	aabb_.Update(playerTrans_);
	//obb_.Update(playerTrans_);
	sphere_.Update(weaponTrans_);
}
