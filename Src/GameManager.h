/**
* @file GameManager.h
*/

#ifndef GAMEMANAGER_H_INCLUDE
#define GAMEMANAGER_H_INCLUDE

#include "Actor/Actor.h"
#include "Stage/Stage.h"
#include <vector>

class GameManager
{
public:
	static bool Initialize();
	static void Finalize();
	static GameManager& Get();

	enum class State
	{
		// ゲーム開始
		start,
		// プレイ中
		playing,
		// ゲームクリア
		gameclear,
		// ゲームオーバー
		gameover,
		// メニュー
		menu,
	};

	void Update(float deltaTime);
	void UpdateUI();

	void GameStart();
	void GameClear();
	void GameOver();

private:
	GameManager() = default;
	~GameManager() = default;
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;

	void SpawnPlayerTank();
	void SpawnEnemiesTank();

	// 現在の動作状態
	State state = State::start;
	std::shared_ptr<Actor> playerTank;
	std::vector<std::shared_ptr<Actor>> enemies;

	// スタートを表示する最大時間
	const float startDispMaxTime = 6;
	// スタートを表示する残り時間
	float startDispTime = startDispMaxTime;

};

#endif // GAMEMANAGER_H_INCLUDE