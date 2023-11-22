#pragma once
#include "Engine/Camera/Camera.h"

#include "Engine/Texture/Model.h"
#include "Engine/Texture/Texture2D.h"
#include "Engine/Input/AudioInput/AudioInput.h"

#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Particle/Particle.h"
#include "Engine/Texture/Line/Line.h"

#include "Game/GameScene/Battle/Battle.h"
#include "Game/Ground/Ground.h"
#include "Game/Skydome/Skydome.h"

class GameScene
{
public:
	GameScene() = default;
	~GameScene() = default;

	enum class Scene {
		TITLE,
		BATTLE,
		RESULT
	};
	Scene scene = Scene::TITLE;
	Scene oldscene = Scene::TITLE;

	bool sceneChangeFlag = false;

	//	シングルトンインスタンス
	static GameScene* GetInstance();
	
private:
	
	//	カメラの生成
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Camera> camera2d;
	//	カメラ行列の生成
	Matrix4x4 viewProjectionMatrix{};
	Matrix4x4 viewProjectionMatrix2d{};

	//	シーン用インスタンス
	std::unique_ptr<Battle> battle_;

	//	モデル
	std::vector<std::shared_ptr<Model>> player_;
	std::vector<std::shared_ptr<Model>> enemy_;
	std::shared_ptr<Model> floor_;
	Skydome skydome_;
	Ground ground_;

	
public:

	void Initialize();

	void Update();
	
	void Draw();

	void Finalize();

private: // メンバ関数
	//	モデルのロード
	void ModelLoad();

};