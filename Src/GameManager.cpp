/**
* @file GameManager.cpp
*/
#include "GameManager.h"
#include "GameEngine.h"
#include "Actor/Operator/Enemy.h"
#include "Actor/Operator/Player.h"
#include "SceneManager.h"
#include "Actor/UI/Image.h"
#include "GameCursor.h"
#include "AStar/AStar.h"
#include <iostream>
#include <mutex>
#include <imgui.h>
#include <sstream>

namespace
{
	GameManager* manager = nullptr;
}

/**
* ゲームマネージャーの初期化
*/
bool GameManager::Initialize()
{
	// ゲームマネージャーが存在していなければ作成
	if (!manager)
	{
		manager = new GameManager;
		// 作成してもない場合falseを返す
		if (!manager)
		{
			std::cout << "GameManagerの作成に失敗\n";
			return false;
		}
	}

	return true;
}

/**
* ゲームマネージャーの終了
*/
void GameManager::Finalize()
{
	// ゲームマネージャーが存在していれば消去
	if (manager)
	{
		delete manager;
		manager = nullptr;
	}
}

/**
* ゲームエンジンを取得する
*/
GameManager& GameManager::Get()
{
	return *manager;
}

/**
* ゲームの状態を更新する
*/
void GameManager::Update(float deltaTime)
{
	GameEngine& engine = GameEngine::Get();
	SceneManager& sceneManager = SceneManager::Get();

	switch (state)
	{
	case State::start:
	{	
		// ウィンドウの大きさ取得
		glm::vec2 windowSize = engine.GetWindowSize();

		// スタートとはじまるまでの時間を表示
		ImGui::SetNextWindowSize(ImVec2(1000, 1000));
		ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 300, windowSize.y / 2 - 100));
		ImGui::Begin("time", nullptr,
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
		
		ImGui::SetWindowFontScale(5);

		ImGui::Text("Destroy All Tanks");

		if (startDispTime > 1)
		{
			ImGui::SetCursorPos(ImVec2(280, 100));
			ImGui::Text("%d", static_cast<int>(startDispTime));
			// 敵を動けないようにする
			for (auto& e : enemies)
			{
				static_cast<Enemy&>(*e).SetCanMove(false);
			}
			// プレイヤーを動けない状態にする
			static_cast<Player&>(*playerTank).SetCanMove(false);
		}
		else
		{
			ImGui::SetCursorPos(ImVec2(210, 100));
			ImGui::Text("Start");
			// 敵を動けるようにする
			for (auto& e : enemies)
			{
				static_cast<Enemy&>(*e).SetCanMove(true);
			}
			// プレイヤーを動ける状態にする
			static_cast<Player&>(*playerTank).SetCanMove(true);
		}
		
		// スタートを表示する残り時間を減らす
		startDispTime -= deltaTime;

		ImGui::End();
		// 時間が過ぎたらゲームを始める
		if (startDispTime <= 0)
		{
			state = State::playing;
			std::cout << __func__ << ": start -> playing\n";
			// 時間をもとに戻す
			startDispTime = startDispMaxTime;
		}
		
		break;
	}
	case State::playing:
	{
		// ゲームプレイ中入り続ける
		// escが押されたらゲーム終了
		if (engine.GetKey(GLFW_KEY_ESCAPE))
		{
			engine.WindowClose();
		}

		Tank p = static_cast<Player&>(*playerTank);
		if (!p.GetCanMoving())
		{
			// プレイヤーが死んでいたらゲームオーバー
			state = State::gameover;
			std::cout << __func__ << ": playing -> gameover\n";
		}
		else
		{
			// エネミーの数を数える
			int surviverCount = 0;
			for (int i = 0; i < enemies.size(); ++i)
			{
				Tank e = static_cast<Enemy&>(*enemies[i]);
				if (!e.GetCanMoving())
				{
					// 動けない敵戦車を数える
					++surviverCount;
				}
			}
			// エネミーが全て動けない状態であればクリア
			if (surviverCount >= enemies.size())
			{
				state = State::gameclear;
				std::cout << __func__ << ": clear -> gameclear\n";
			}
		}
		break;
	}
	case State::gameclear:
	{
		GameClear();
		// ENTERでタイトルに戻る
		if (engine.GetKey(GLFW_KEY_ENTER))
		{
			state = State::start;
			std::cout << __func__ << ": gameclear -> start\n";
			sceneManager.LoadScene("Title");
		}

		break;
	}
	case State::gameover:
	{
		GameOver();
		// ENTERでタイトルに戻る
		if (engine.GetKey(GLFW_KEY_ENTER))
		{
			state = State::start;
			std::cout << __func__ << ": gameclear -> start\n";
			sceneManager.LoadScene("Title");
		}
		break;
	}
	case State::menu:
	{


		break;
	}
	}
}

/**
* UIの状態を更新
*/
void GameManager::UpdateUI()
{
	GameEngine& engine = GameEngine::Get();
	// スタイル構造体を取得
	ImGuiStyle& style = ImGui::GetStyle();
	// 元に戻すためのバックアップ
	const ImGuiStyle styleBackup = style;

	// プレイヤーのHP表示
	if (playerTank)
	{
		ImGui::SetWindowSize(ImVec2(100, 10));
		ImGui::SetNextWindowPos(ImVec2(450,600));
		ImGui::Begin("HP",nullptr,
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1, 0, 0, 1);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 0, 0, 0.5f);
		// 枠の太さ
		style.FrameBorderSize = 3.0f;
		// 枠の色
		style.Colors[ImGuiCol_Border] = ImVec4(1, 1, 1, 1);
		// ふちの丸さ
		style.FrameRounding = 8.0f;
		const float maxPlayerHealth = 100;
		const float f = playerTank->health / maxPlayerHealth;
		ImGui::ProgressBar(f, ImVec2(0, 0), "");
		// スタイルを元に戻す
		style = styleBackup;
		ImGui::End();
	}

	// 敵の数を表示
	{
		ImGui::SetWindowSize(ImVec2(10, 10));
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::Begin("EnemyCount", nullptr, ImGuiWindowFlags_NoBackground | 
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
		std::shared_ptr<Texture> tex = engine.LoadTexture("Res/IconEnemy.tga");
		const ImTextureID texId = reinterpret_cast<ImTextureID>(tex->GetID());
		ImGui::SameLine();
		// 敵画像表示
		ImGui::Image(texId, ImVec2(40, 40));

		ImGui::SameLine();

		// 敵の数を数える変数
		int enemyCount = 0;
		for (int i = 0; i < enemies.size(); ++i)
		{
			Tank e = static_cast<Enemy&>(*enemies[i]);
			if (e.GetCanMoving())
			{
				++enemyCount;
			}
		}
		std::stringstream sst;
		// intからchar*に変換
		sst << enemyCount;
		// フォントのサイズを大きくする
		ImGui::SetWindowFontScale(3);
		// テキスト表示
		ImGui::Text(sst.str().c_str());

		ImGui::End();
	}
}

/**
* プレイヤーを作成
*/
void GameManager::SpawnPlayerTank()
{
	// 以前のタイガーI戦車を消去
	if (playerTank)
	{
		playerTank->isDead = true;
	}

	// 新しいプレイヤーの作成
	playerTank.reset(
		new Player{
		glm::vec3(0,0,50),
		glm::vec3(1),
		3.45f,
		TankType::TIGER });
	// タイガーI戦車をゲームエンジンに登録
	GameEngine::Get().AddActors(playerTank);

}

/**
* 敵戦車の作成
*/
void GameManager::SpawnEnemiesTank()
{
	// ゲームエンジン取得
	GameEngine& engine = GameEngine::Get();

	// 敵の出現位置配列
	glm::vec3 enemySpawnPositions[] =
	{
		glm::vec3(0,0,-50),
		glm::vec3(50,0,-50),
		glm::vec3(-50,0,-50),
		glm::vec3(5,0,-50),
		glm::vec3(10,0,-50),
	};

	// 以前作成したT-34戦車を消去
	for (int i = 0; i < enemies.size(); ++i)
	{
		if (enemies[i])
		{
			enemies[i]->isDead = true;
		}
	}
	enemies.clear();

	for (int i = 0; i < 5; ++i)
	{
		// 新しいT-34戦車を作成
		enemies.push_back(std::shared_ptr<Actor>(
			new Enemy{
			enemySpawnPositions[i],
			glm::vec3(1),
			0.0f,
			TankType::T34 }));
	}	

	// T-34戦車をゲームエンジンに登録
	for (int i = 0; i < enemies.size(); ++i)
	{
		engine.AddActors(enemies[i]);
	}
}

/**
* ゲーム開始
*/
void GameManager::GameStart()
{
	GameEngine& engine = GameEngine::Get();
	SpawnPlayerTank();
	SpawnEnemiesTank();
}

/**
* ゲームクリア
*/
void GameManager::GameClear()
{
	// ゲームエンジン取得
	GameEngine& engine = GameEngine::Get();

	// ウィンドウのサイズの取得
	glm::vec2 windowSize = engine.GetWindowSize();
	// テクスチャ取得
	std::shared_ptr<Texture> tex = engine.LoadTexture("Res/GameClear.tga");
	// テクスチャのサイズの取得
	glm::vec2 texSize = glm::vec2(tex->GetWidth(),tex->GetHeight());
	
	
	ImGui::SetNextWindowSize(ImVec2(310, 80));
	ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 150, windowSize.y / 2 - 50));
	ImGui::Begin("GameClear", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	ImGui::SetWindowFontScale(5);
	ImGui::Text("GameClear");
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(144, 35));
	ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 50, windowSize.y / 2 + 150));
	ImGui::Begin("PressEnter", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	ImGui::SetWindowFontScale(2);
	ImGui::Text("PressEnter");
	ImGui::End();

	//// テクスチャをImGuiで使えるように変換
	//const ImTextureID texId = reinterpret_cast<ImTextureID>(tex->GetID());
	//// 画像作成
	//std::shared_ptr<UI> image = std::shared_ptr<UI>(new Image(
	//	"GameClear",
	//	glm::vec2(windowSize.x / 2, windowSize.y / 2),
	//	texSize, 1, texSize , texId));
	//// 画像を追加
	//engine.AddUI(image);
}

/**
* ゲームオーバー
*/
void GameManager::GameOver()
{
	// ゲームエンジン取得
	GameEngine& engine = GameEngine::Get();

	// ウィンドウのサイズの取得
	glm::vec2 windowSize = engine.GetWindowSize();
	// テクスチャ取得
	std::shared_ptr<Texture> tex = engine.LoadTexture("Res/GameOver.tga");
	// テクスチャのサイズの取得
	glm::vec2 texSize = glm::vec2(tex->GetWidth(), tex->GetHeight());

	ImGui::SetNextWindowSize(ImVec2(144, 35));
	ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 50, windowSize.y / 2 + 150));
	ImGui::Begin("GameOver", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	ImGui::SetWindowFontScale(5);
	ImGui::Text("GameOver");
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(144, 35));
	ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 50, windowSize.y / 2 + 150));
	ImGui::Begin("PressEnter", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	ImGui::SetWindowFontScale(2);
	ImGui::Text("PressEnter");
	ImGui::End();

	//// テクスチャをImGuiで使えるように変換
	//const ImTextureID texId = reinterpret_cast<ImTextureID>(tex->GetID());
	//// 画像作成
	//std::shared_ptr<UI> image = std::shared_ptr<UI>(new Image(
	//	"GameClear",
	//	glm::vec2(windowSize.x / 2, windowSize.y / 2),
	//	texSize, 1, texSize, texId));
	//// 画像を追加
	//engine.AddUI(image);
}