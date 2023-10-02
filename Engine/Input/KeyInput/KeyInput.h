#pragma once

#define DIRECTINPUT_VERSION 0x0800	// DirectInputのバージョン指定
#include <dinput.h>

#include <wrl.h>
#include <cstdint>
#include <Xinput.h>
#include "math/Vector2.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "xinput.lib")

class KeyInput
{
public:
	KeyInput() = default;
	~KeyInput() = default;

	/// <summary>
	/// シングルトンインスタンスの取得
	/// これにより1つしか生成されなくなる
	/// </summary>
	/// <returns>KeyInputのシングルトンインスタンス</returns>
	static KeyInput* GetInstance();

private:
	//	通常初期化処理
	void Initialize();
public:
	//	静的初期化処理
	static void InputInitialize();

	// 更新処理
	static void Update();

private:	//変数
	//	DirectInputの初期化
	Microsoft::WRL::ComPtr<IDirectInput8> directInput;
	//	キーボードの生成
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	//	キーの用意
	BYTE key[256] = {};
	BYTE oldKey[256] = {};
	//	コントローラーの用意
	XINPUT_STATE xInputState = {};
	XINPUT_STATE oldXInputState = {};
	bool isConnectPad = false;

public:	// キー
	
	// キーが押されているか
	static bool GetKey(uint8_t keynumber);

	//	キーを押した瞬間
	static bool PushKey(uint8_t keynumber);
	
	//	キーを離した瞬間
	static bool ReleaseKey(uint8_t keynumber);


public: // コントローラー

	//パッドに接続されてるか
	bool GetPadConnect();

	//パッドのボタンが押されているか
	bool GetPadButton(UINT button);

	//パッドのボタンが離れた瞬間か
	bool GetPadButtonUp(UINT button);

	//パッドのボタンが押された瞬間か
	bool GetPadButtonDown(UINT button);

	//パッドの左スティック
	Vector2 GetPadLStick();

	//パッドの右スティック
	Vector2 GetPadRStick();

	//左トリガーを押し込んだ瞬間か
	bool GetLTriggerDown();

	//右トリガーを押し込んだ瞬間か
	bool GetRTriggerDown();

	//左トリガーが押されているか
	bool GetLTrigger();

	//右トリガーが押されているか
	bool GetRTrigger();


};
