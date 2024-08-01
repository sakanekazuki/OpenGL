/**
* @file Main.cpp
*/
#include <glad/glad.h>
#include "GLContext.h"
#include "ProgramPipeline.h"
#include "Sampler.h"
#include "GameManager.h"
#include "GameCursor.h"
#include "Actor/Operator/Player.h"
#include "Actor/Operator/Enemy.h"
#include "Actor/Item/Bullet.h"
#include "Stage/Stage.h"
#include "GameEngine.h"
#include "SceneManager.h"
#include "UI/EventSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>
#include <memory>
#include <Windows.h>

#pragma comment(lib, "opengl32.lib")

/**
* エントリーポイント
*/
int main()
{
#ifdef NDEBUG

	FreeConsole();

#endif // NDEBUG
	
	// ゲームエンジンの初期化
	GameEngine::Initialize();
	GameEngine& engine = GameEngine::Get();

	// 1/60秒間隔でループ処理するための時刻変数
	double loopTime = engine.GetTime();
	// 時刻の差分
	double diffLoopTime = 0;
	// 更新間隔
	const float deltaTime = 1.0f / 60.0f;

	// ゲームマネージャーの初期化
	GameManager::Initialize();
	GameManager& manager = GameManager::Get();

	GameCursor::Initialize();
	GameCursor& cursor = GameCursor::Get();

	// ゲーム中のカーソルの状態の設定
	// GLFW_CURSOR_NORMAL
	// GLFW_CURSOR_HIDDEN
	// GLFW_CURSOR_DISABLED
	cursor.SetCurosorStatus(GLFW_CURSOR_NORMAL);

	// シーン読み込み
	SceneManager& sceneManager = SceneManager::Get();
	sceneManager.LoadScene("Title");

	// メインループ
	while (!engine.WindowShouldClose())
	{
		// Escで終了
		if (engine.GetKey(GLFW_KEY_ESCAPE))
		{
			break;
		}
		// シーンを更新
		sceneManager.SceneUpdate();

		// 現在時刻を取得
		const double curLoopTime = engine.GetTime();
		// 現在時刻と前回時刻の差を、時刻の差分に加算
		diffLoopTime += curLoopTime - loopTime;
		// 前回時刻を現在時刻に更新
		loopTime = curLoopTime;
		// 時刻の差分が1/60秒未満なら、ループの先頭に戻る
		if (diffLoopTime < 1.0 / 60.0)
		{
			continue;
		}

		if (diffLoopTime > 0.5f) 
		{
			diffLoopTime = deltaTime;
		}

		//! 
		//! ゲーム状態を更新する
		//! 

		engine.NewFrame();

		float times = 0;

		// アクターの移動
		for (; diffLoopTime >= deltaTime; diffLoopTime -= deltaTime)
		{
			engine.UpdateActors(deltaTime);
			engine.PostUpdateActors();
			engine.UpdatePhysics(deltaTime);
			// 消去待ちのアクターを消去する
			engine.RemoveDeadActors();
			
			times += deltaTime;
		}

		sceneManager.Update(times);
		engine.UpdateCamera();
		
		engine.UpdateUI(times);

		//!
		//! ゲーム状態を描画する
		//! 
		
		engine.RenderDefault();
		engine.RenderSprite();
		engine.RenderUI();
		engine.PostRender();
		engine.SwapBuffers();
	}
	// カーソルの終了
	cursor.Finalize();
	// ゲームマネージャの終了
	manager.Finalize();
	// ゲームエンジンを終了
	engine.Finalize();

	return 0;
}