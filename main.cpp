#include <Windows.h>
#include "Engine/Engine.h"
#include "Engine/TimeBaseLoopExecuter/TimeBaseLoopExecuter.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "externals/imgui/imgui.h"

#include "Game/GameScene/GameScene.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	static D3DResourceLeakChecker leak;
	int32_t windowWidth = 1280; int32_t windowHeight = 720;
	Engine::Initialize("えとせとら", windowWidth, windowHeight);
	
	GameScene::GetInstance()->Initialize();

	//	ウィンドウの×ボタンが押されるまでループ
	while (!WinApp::ProcessMessage()) {
		//	フレームの開始
		TimeBaseLoopExecuter fpsManager(60);
		
		Engine::BeginFrame();
#ifdef _DEBUG
		//	ImGui のフレームに一つ目の ImGui のウィンドウを描く
		ImGui::Begin("Control panel");
		ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
		ImGui::End();
#endif FrameRate

		GameScene::GetInstance()->Update();

		//GameScene::GetInstance()->Draw();

		//	フレームの終了
		Engine::EndFrame();
		fpsManager.TimeAdjustment();
		if (KeyInput::PushKey(DIK_ESCAPE)) {
			break;
		}
	}

	GameScene::GetInstance()->Finalize();

	Engine::Finalize();

	return 0;
}