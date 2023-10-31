#include "Player.h"
#include <externals/imgui/imgui.h>
#include "Engine/Input/KeyInput/KeyInput.h"
#include "GlobalVariables/GlobalVariables.h"


Player::Player(Camera* camera)
{
	camera_ = camera;
	camera_->transform.rotation_.x = AngleToRadian(20.0f);
}

void Player::Initialize()
{
	playerTrans_.translation_ = Vector3(0.0f, 1.0f, 0.0f);
	isFloating_ = false;
	isJamp_ = false;

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

	const char* groupName = "Player";
	//	グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	GlobalVariables::GetInstance()->AddItem(groupName, "Body Translation", parts_[0].translation_);
	GlobalVariables::GetInstance()->AddItem(groupName, "Head Translation", parts_[1].translation_);
	GlobalVariables::GetInstance()->AddItem(groupName, "Rarm Translation", parts_[2].translation_);
	GlobalVariables::GetInstance()->AddItem(groupName, "Larm Translation", parts_[3].translation_);

	

	//	座標 - scale * size
	aabb_.Update(playerTrans_);
}

void Player::Update()
{
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("scale", &playerTrans_.scale_.x, 0.1f);
	ImGui::DragFloat3("translate", &playerTrans_.translation_.x, 1.0f);
	ImGui::DragFloat3("rotate", &playerTrans_.rotation_.x, AngleToRadian(1.0f));
	ImGui::End();
#endif DEBUG

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
	}

	Collision();
	MoveLimit();
	
	//	座標更新
	playerTrans_.UpdateMatrix();
	for (auto& i : parts_) {
		i.UpdateMatrix();
	}

	//	座標 - scale * size
	aabb_.Update(playerTrans_);

	//	カメラの移動と更新
	CameraMove();

}


void Player::Draw(const Matrix4x4& viewProjection) {
	for (uint8_t i = 0; i < parts_.size() - 1u; i++) {
		Model::ModelDraw(parts_[i], viewProjection, 0xffffffff, model_[i].get());
	}
	if (behavior_ == Behavior::kAttack) {
		Model::ModelDraw(parts_[4], viewProjection, 0xffffffff, model_[4].get());
	}

	aabb_.DrawAABB(viewProjection, 0xff0000ff);
}

void Player::EnemyColl(AABB* enemy) {
	if (aabb_.IsCollision(enemy)) {
		Initialize();
	}
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
		move = Normalize(move) /** speed*/;
		//	移動ベクトルをカメラの角度だけ回転させる
		move = TransformNormal(move, MakeRotateMatrix(camera_->transform.rotation_));
		//	移動方向に見た目を合わせる
		playerTrans_.rotation_.y = std::atan2f(move.x, move.z);
		//	ジャンプを加味していないため
		move.y = 0.0f;

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
		Initialize();
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
		Initialize();
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

void Player::CameraMove()
{
	Vector3 offset(0.0f, 2.0f, -50.0f);

	Matrix4x4 rotate = MakeRotateMatrix(camera_->transform.rotation_);
	offset = TransformNormal(offset, rotate);

	camera_->transform.translation_ = playerTrans_.translation_ + offset;
	camera_->transform.UpdateMatrix();
}

void Player::InitializeAttack() {
	attackFrame = 0;
	parts_[2].rotation_.x = -3.5f;
	parts_[3].rotation_.x = -3.5f;
	parts_[4].rotation_.x = 0.0f;

}

void Player::InitializeRoot() {
	parts_[2].rotation_.x = 0.0f;
	parts_[3].rotation_.x = 0.0f;
}

void Player::InitializeDash() {
	workDash_.dashParameter_ = 0u;
	//playerTrans_.rotation_.y = destinationAngleY_;
}

void Player::BehaviorRootUpdate() {
	Move();
	Jamp();
	if (KeyInput::PushKey(DIK_V)) {
		behaviorRequest_ = Behavior::kAttack;
	}
}

void Player::BehaviorAttackUpdate() {
	if (attackFrame != 0) {
		if (attackFrame < 20) {
			parts_[2].rotation_.x += 0.1f;
			parts_[3].rotation_.x += 0.1f;
			parts_[4].rotation_.x += 0.1f;
		}
		else if (attackFrame > 30) {
			behaviorRequest_ = Behavior::kRoot;
			return;
		}
	}
	attackFrame++;
}

void Player::BehaviorDashUpdate() {
	// ダッシュボタンを押したら
	if (KeyInput::PushKey(DIK_B)) {
		// ダッシュリクエスト
		behaviorRequest_ = Behavior::kDash;
	}
}

void Player::ApplyGlobalVariables() {
	GlobalVariables* globalManagement = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	parts_[0].translation_ = globalManagement->GetVector3Value(groupName, "Body Translation");
	parts_[1].translation_ = globalManagement->GetVector3Value(groupName, "Head Translation");
	parts_[2].translation_ = globalManagement->GetVector3Value(groupName, "Rarm Translation");
	parts_[3].translation_ = globalManagement->GetVector3Value(groupName, "Larm Translation");

}
