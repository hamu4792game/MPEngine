#pragma once
#include "Engine/Camera/Camera.h"

#include "Engine/Texture/Model.h"
#include "Engine/Texture/Texture2D.h"
#include "Engine/Input/AudioInput/AudioInput.h"

#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Particle/Particle.h"

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

	//std::unique_ptr<Texture2D> hud;
	//WorldTransform hudTrans;

	std::unique_ptr<Particle> fence1;
	//std::unique_ptr<Model> fence2;
	WorldTransform fenceTrans1[10];
	//WorldTransform fenceTrans2;

public:

	void Initialize();


	void Update();
	
	void Draw();

private: // メンバ関数
	//	モデルのロード
	void ModelLoad();

};