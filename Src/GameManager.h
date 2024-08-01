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
		// �Q�[���J�n
		start,
		// �v���C��
		playing,
		// �Q�[���N���A
		gameclear,
		// �Q�[���I�[�o�[
		gameover,
		// ���j���[
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

	// ���݂̓�����
	State state = State::start;
	std::shared_ptr<Actor> playerTank;
	std::vector<std::shared_ptr<Actor>> enemies;

	// �X�^�[�g��\������ő厞��
	const float startDispMaxTime = 6;
	// �X�^�[�g��\������c�莞��
	float startDispTime = startDispMaxTime;

};

#endif // GAMEMANAGER_H_INCLUDE