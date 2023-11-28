#include "LockOn.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "externals/imgui/imgui.h"

LockOn::LockOn() {
	lockOnMark_ = std::make_unique<Texture2D>();
	lockOnMark_->Texture("Resources/hud/target.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	
}

void LockOn::Initialize() {
	lockOnMarkTransform_.scale_ = Vector3(0.2f, 0.2f, 0.1f);
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, Camera* camera, const Matrix4x4& view) {
	ImGui::DragFloat3("LockOn", &lockOnMarkTransform_.translation_.x, 0.1f);
	
	// ロックオン状態なら
	if (target_) {
		// ロックオンボタンをトリガーしたらロックオンを外す
		if (KeyInput::PushKey(DIK_M)) {
			target_ = nullptr;
		}
		else if (DeterminationWithOutArea(view)) {
			// ロックオンを外す
			target_ = nullptr;
		}
		else if (target_->IsDead()) {
			target_ = nullptr;
		}
		if (KeyInput::PushKey(DIK_0)) {
			auto iterator = targets.begin();
			num++;
			if (num >= targets.size()) {
				num = 0;
			}
			for (int8_t i = 0; i < num; i++) {
				iterator++;
			}
			target_ = iterator->second;
		}

	}
	else {
		// ロックオン対象の検索
		if (KeyInput::PushKey(DIK_M)) {
			Search(enemies, view);
			num = 0;
		}
	}

	// ロックオン状態なら
	if (target_) {
		// 敵のロックオン座標取得
		Vector3 positionWorld = target_->GetPosition();
		// ワールド座標からスクリーン座標に変換
		Vector3 positionScreen = ChangeScreen(positionWorld, camera);
		// 
		//positionScreen.y = -positionScreen.y;
		lockOnMarkTransform_.translation_ = positionScreen;
		lockOnMarkTransform_.UpdateMatrix();
	}

}

void LockOn::Draw2D(const Matrix4x4& viewProjection) {
	if (target_) {
		// 描画
		
		Texture2D::TextureDraw(lockOnMarkTransform_, viewProjection, 0xffffffff, lockOnMark_.get());
	}
}

Vector3 LockOn::GetTargetPosition() const {
	if (target_) {
		return target_->GetPosition();
	}
	return Vector3::zero;
}

void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies, const Matrix4x4& view) {
	// ロックオン対象の絞り込み

	targets.clear();
	// 全ての敵に対してロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {

		if (enemy->IsDead()) {
			continue;
		}

		Vector3 positionWorld = enemy->GetPosition();
		// ワールドからビューへ変換
		Vector3 positionView = Transform(positionWorld, view);

		// 距離条件チェック
		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
			// カメラ前方との角度を計算
			Vector3 viewAngle = Vector3(view.m[3][0], view.m[3][1], view.m[3][2]);
			//float arcTangent = Dot(Normalize(positionView), Normalize(viewAngle));
			float arcTangent = std::atan2f(std::sqrtf(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);
			// 角度条件チェック
			if (std::fabsf(arcTangent) <= std::cosf(angleRange_)) {
				targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
			}
		}
	}

	target_ = nullptr;
	if (targets.size() != 0) {
		// 距離で昇順にソート
		targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
		// ソートの結果一番近い敵をロックオン対象とする
		target_ = targets.front().second;
	}


}

bool LockOn::DeterminationWithOutArea(const Matrix4x4& view) {
	
	Vector3 positionWorld = target_->GetPosition();
	// ワールドからビューへ変換
	Vector3 positionView = Transform(positionWorld, view);

	// 距離条件チェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		// カメラ前方との角度を計算
		Vector3 viewAngle = Vector3(view.m[3][0], view.m[3][1], view.m[3][2]);
		float arcTangent = Dot(Normalize(positionView), Normalize(viewAngle));
		// 角度条件チェック
		if (std::fabsf(arcTangent) <= angleRange_) {
			// 範囲内なので
			return false;
		}
	}
	return true;
}

Vector3 LockOn::ChangeScreen(const Vector3& worldPos, Camera* camera) {
	//	ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//	ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = camera->GetViewProMat() * matViewport;
	//	ワールドからスクリーン座標へ変換（ここで2Dから3Dになる）
	Vector3 result = Transform(worldPos, matViewProjectionViewport);

	Matrix4x4 ortho = MakeOrthographicMatrix(-float(Engine::GetInstance()->WindowWidth / 2), float(Engine::GetInstance()->WindowHeight / 2),
		float(Engine::GetInstance()->WindowWidth / 2), -float(Engine::GetInstance()->WindowHeight / 2), 0.01f, 100.0f);
	Matrix4x4 in = Inverse(MakeIdentity4x4() * ortho * matViewport);

	result = Transform(result, in);

	return result;
}
