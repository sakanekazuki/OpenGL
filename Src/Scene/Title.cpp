/**
* @file Title.cpp
*/

#include "Title.h"
#include "../GameEngine.h"
#include "../Actor/UI/Button.h"
#include "../SceneManager.h"
#include "../Actor/UI/Image.h"

/**
* コンストラクタ
*/
Title::Title()
	:Scene(TITLESCENE,"")
{
	// ゲームエンジン取得
	GameEngine& engine = GameEngine::Get();
	// 背景設定
	engine.SetglClearColor(glm::vec3(0, 0, 0));
	// カーソルの表示
	engine.SetCursorType(GLFW_CURSOR_NORMAL);
}

/**
* 更新
*/
void Title::Update(float deltaTime)
{
	// ゲームエンジンの取得
	GameEngine& engine = GameEngine::Get();
	// ウィンドウの大きさを取得
	glm::vec2 windowSize = engine.GetWindowSize();
	// シーンマネージャー取得
	SceneManager& sceneManager = SceneManager::Get();

	//------------------------------------------
	// スタートボタン表示

	// ウィンドウの大きさ指定
	ImGui::SetNextWindowSize(ImVec2(196.0f, 80.0f));
	// ウィンドウの位置指定
	ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 95, windowSize.y / 2 - 200));
	//ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 95, windowSize.y / 2 - 150));

	// ウィンドウの初期設定
	ImGui::Begin("Button0", nullptr,
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	// ウィンドウで使用するフォントの大きさ指定
	ImGui::SetWindowFontScale(5);
	// ボタンが押されたらゲームスタート
	if (ImGui::Button("Start"))
	{
		sceneManager.LoadScene("MainGame");
	}
	ImGui::End();

	//------------------------------------------


	//------------------------------------------
	// キーガイドに移動するボタン

	// ウィンドウの大きさ指定
	ImGui::SetNextWindowSize(ImVec2(300, 80.0f));
	// ウィンドウの位置指定
	ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 130, windowSize.y / 2-50));
	// ウィンドウの初期設定
	ImGui::Begin("Button1", nullptr,
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	// ウィンドウで使用するフォントの大きさ指定
	ImGui::SetWindowFontScale(5);
	// ボタンが押されたらゲームスタート
	if (ImGui::Button("KeyGuid"))
	{
		sceneManager.LoadScene("Tutorial");
	}
	ImGui::End();

	//------------------------------------------

	//------------------------------------------
	// 終了ボタン表示

	// ウィンドウの大きさ指定
	ImGui::SetNextWindowSize(ImVec2(196.0f, 80.0f));
	// ウィンドウの位置指定
	ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 80, windowSize.y / 2 + 100));
	//ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 80, windowSize.y / 2 + 50));


	// ウィンドウの初期設定
	ImGui::Begin("Button2", nullptr,
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	// ウィンドウで使用するフォントの大きさ指定
	ImGui::SetWindowFontScale(5);
	// ボタンが押されたらゲーム終了
	if (ImGui::Button("Quit"))
	{
		engine.WindowClose();
	}
	ImGui::End();
	//------------------------------------------
}