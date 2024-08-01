/**
* @file Tutorial.cpp
*/

#include "Tutorial.h"
#include "../../GameEngine.h"
#include "../Actor/UI/Image.h"
#include "../Actor/UI/Button.h"
#include "../../SceneManager.h"
#include <imgui.h>

/**
* コンストラクタ
*/
Tutorial::Tutorial()
	:Scene(TUTORIALSCENE, "")
{
	// ゲームエンジン取得
	GameEngine& engine = GameEngine::Get();
	// 背景色設定
	engine.SetglClearColor(glm::vec3(0.5f, 0.5f, 0.5f));
}

/**
* 更新
*/
void Tutorial::Update(float deltaTime)
{
	// ゲームエンジン取得
	GameEngine& engine = GameEngine::Get();
	// ウィンドウの大きさ取得
	glm::vec2 windowSize = engine.GetWindowSize();

	// ウィンドウの大きさ指定
	ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
	// ウィンドウの位置指定
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	// 初期設定
	ImGui::Begin("KeyGuid", nullptr,
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);	

	//----------------------------------
	// 上矢印

	ImGui::SetCursorPos(ImVec2(275, 100));

	// テクスチャを取得
	std::shared_ptr<Texture> upTex = engine.LoadTexture("Res/KeyGuid/up.tga");
	// テクスチャを変換
	ImTextureID upTexId = reinterpret_cast<ImTextureID>(upTex->GetID());
	// 画像表示
	ImGui::Image(upTexId, ImVec2(100, 100));
	
	//----------------------------------


	//----------------------------------
	// Wキー画像

	ImGui::SetCursorPos(ImVec2(300, ImGui::GetCursorPosY() - 10));
	
	// 画像取得
	std::shared_ptr<Texture> wTex = engine.LoadTexture("Res/KeyGuid/W.tga");
	// 画像を変換
	ImTextureID wTexId = reinterpret_cast<ImTextureID>(wTex->GetID());
	// 画像表示
	ImGui::Image(wTexId, ImVec2(50, 50));

	//----------------------------------


	//----------------------------------
	// 左回転画像

	ImGui::SetCursorPos(ImVec2(159, ImGui::GetCursorPosY() + 10));

	// 画像取得
	std::shared_ptr<Texture> leftTex = engine.LoadTexture("Res/KeyGuid/left.tga");
	// 画像を変換
	ImTextureID leftTexId = reinterpret_cast<ImTextureID>(leftTex->GetID());
	// 画像表示
	ImGui::Image(leftTexId, ImVec2(75, 75));
	
	//----------------------------------


	//----------------------------------
	// Aキー画像

	// 横に並べる
	ImGui::SameLine();
	// 画像取得
	std::shared_ptr<Texture> ATex = engine.LoadTexture("Res/KeyGuid/A.tga");
	// 画像変換
	ImTextureID ATexId = reinterpret_cast<ImTextureID>(ATex->GetID());
	// 画像表示
	ImGui::Image(ATexId, ImVec2(50, 50));

	//----------------------------------


	//----------------------------------
	// Sキー画像

	// 横に並べる
	ImGui::SameLine();
	
	// 画像取得
	std::shared_ptr<Texture> STex = engine.LoadTexture("Res/KeyGuid/S.tga");
	// 画像変換
	ImTextureID STexId = reinterpret_cast<ImTextureID>(STex->GetID());
	// 画像表示
	ImGui::Image(STexId, ImVec2(50, 50));

	//----------------------------------


	//----------------------------------
	// Dキー画像

	// 横に並べる
	ImGui::SameLine();

	// 画像取得
	std::shared_ptr<Texture> DTex = engine.LoadTexture("Res/KeyGuid/D.tga");
	// 画像変換
	ImTextureID DTexId = reinterpret_cast<ImTextureID>(DTex->GetID());
	// 画像表示
	ImGui::Image(DTexId, ImVec2(50, 50));

	//----------------------------------


	//----------------------------------
	// 右回転

	// 横に並べる
	ImGui::SameLine();

	// 画像取得
	std::shared_ptr<Texture> rightTex = engine.LoadTexture("Res/KeyGuid/right.tga");
	// 画像変換
	ImTextureID rightTexId = reinterpret_cast<ImTextureID>(rightTex->GetID());
	// 画像表示
	ImGui::Image(rightTexId, ImVec2(75, 75));

	//----------------------------------


	//----------------------------------
	// 下矢印

	ImGui::SetCursorPos(ImVec2(275, ImGui::GetCursorPosY() - 25));

	// 画像取得
	std::shared_ptr<Texture> downTex = engine.LoadTexture("Res/KeyGuid/down.tga");
	// 画像変換
	ImTextureID downTexId = reinterpret_cast<ImTextureID>(downTex->GetID());
	// 画像表示
	ImGui::Image(downTexId, ImVec2(100, 100));

	//----------------------------------


	//----------------------------------
	// 攻撃の文字

	// 表示する位置の指定
	ImGui::SetCursorPos(ImVec2(750, 150));
	// ウィンドウで使用する文字の大きさ
	ImGui::SetWindowFontScale(3);
	// テキスト表示
	ImGui::Text("Attack");

	//----------------------------------


	//----------------------------------
	// 砲左回転

	// 表示位置設定
	ImGui::SetCursorPos(ImVec2(550, 200));
	// 画像取得
	std::shared_ptr<Texture> cannonLeftRotTex = engine.LoadTexture("Res/KeyGuid/tankCannonLeftRotation.tga");
	// 画像変換
	ImTextureID cannonLeftRotTexId = reinterpret_cast<ImTextureID>(cannonLeftRotTex->GetID());
	// 表示
	ImGui::Image(cannonLeftRotTexId, ImVec2(200, 200));

	//----------------------------------


	//----------------------------------
	// 左矢印

	// 表示位置指定
	ImGui::SetCursorPos(ImVec2(725, 250));
	// 画像取得
	std::shared_ptr<Texture> leftArrowTex = engine.LoadTexture("Res/KeyGuid/leftArrow.tga");
	// 画像変換
	ImTextureID leftArrowTexId = reinterpret_cast<ImTextureID>(leftArrowTex->GetID());
	// 画像表示
	ImGui::Image(leftArrowTexId, ImVec2(75, 75));

	//----------------------------------


	//----------------------------------
	// マウスの画像

	// 画像取得
	std::shared_ptr<Texture> mouseTex = engine.LoadTexture("Res/KeyGuid/mouse.tga");
	// 画像変換
	ImTextureID mouseTexId = reinterpret_cast<ImTextureID>(mouseTex->GetID());
	// 画像表示
	ImGui::GetWindowDrawList()->AddImage(mouseTexId, ImVec2(800, 200), ImVec2(1000, 400));

	//----------------------------------

	
	//----------------------------------
	// 右下矢印

	// 表示する位置の指定
	ImGui::SetCursorPos(ImVec2(830, 200));

	// 画像取得
	std::shared_ptr<Texture> line = engine.LoadTexture("Res/KeyGuid/右斜め下線.tga");
	// 画像変換
	ImTextureID lineId = reinterpret_cast<ImTextureID>(line->GetID());
	// 画像表示
	ImGui::Image(lineId, ImVec2(50, 50));

	//----------------------------------


	//----------------------------------
	// 右矢印

	// 表示位置指定
	ImGui::SetCursorPos(ImVec2(1000, 250));
	// 画像取得
	std::shared_ptr<Texture> rightArrowTex = engine.LoadTexture("Res/KeyGuid/rightArrow.tga");
	// 画像変換
	ImTextureID rightArrowTexId = reinterpret_cast<ImTextureID>(rightArrowTex->GetID());
	// 表示
	ImGui::Image(rightArrowTexId, ImVec2(75, 75));

	//----------------------------------


	//----------------------------------
	// 砲左回転

	// 表示位置設定
	ImGui::SetCursorPos(ImVec2(1050, 200));
	// 画像取得
	std::shared_ptr<Texture> cannonRightRotTex = engine.LoadTexture("Res/KeyGuid/tankCannonRightRotation.tga");
	// 画像変換
	ImTextureID cannonRightRotTexId = reinterpret_cast<ImTextureID>(cannonRightRotTex->GetID());
	// 表示
	ImGui::Image(cannonLeftRotTexId, ImVec2(200, 200));

	//----------------------------------


	//----------------------------------
	// タイトルに戻るボタン

	// 表示する位置指定
	ImGui::SetCursorPos(ImVec2(windowSize.x / 2 - 175, windowSize.y - 100));
	// ウィンドウで使用する文字の大きさ指定
	ImGui::SetWindowFontScale(5);
	// ボタンが押されたらタイトルに戻る
	if (ImGui::Button("Back to Title"))
	{
		SceneManager::Get().LoadScene("Title");
	}

	//----------------------------------

	ImGui::End();
}