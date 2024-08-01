/**
* @file MainGame.cpp
*/

#include "MainGame.h"
#include "../GameManager.h"
#include "../GameEngine.h"

/**
* コンストラクタ
*/
MainGame::MainGame()
	:Scene(MAINGAMESCENE,"Text/MapObject.txt")
{
	// ゲームエンジンの取得
	GameEngine& engine = GameEngine::Get();
	// ゲームマネージャーの取得
	GameManager& manager = GameManager::Get();

	std::shared_ptr<Texture>tex = engine.LoadTexture("Res/House1.tga");
	// プレイヤーを出す
	manager.GameStart();
	// シェーダーのライトの設定
	engine.GetPipeline()->SetFragUniform(1, glm::vec3(-0.70, -0.59, -0.41));
	engine.GetPipeline()->SetFragUniform(2, glm::vec3(1.94, 1.65, 1.24));
	engine.GetPipeline()->SetFragUniform(3, glm::vec3(0.15, 0.10, 0.20));
	// 背景色の設定
	engine.SetglClearColor(glm::vec3(0.2f, 0.9f, 0.9f));
	// カーソルをみえなくする
	engine.SetCursorType(GLFW_CURSOR_DISABLED);
}

/**
* 更新
*/
void MainGame::Update(float deltaTime)
{
	GameManager& manager = GameManager::Get();
	manager.Update(deltaTime);
	manager.UpdateUI();
}