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
* �Q�[���}�l�[�W���[�̏�����
*/
bool GameManager::Initialize()
{
	// �Q�[���}�l�[�W���[�����݂��Ă��Ȃ���΍쐬
	if (!manager)
	{
		manager = new GameManager;
		// �쐬���Ă��Ȃ��ꍇfalse��Ԃ�
		if (!manager)
		{
			std::cout << "GameManager�̍쐬�Ɏ��s\n";
			return false;
		}
	}

	return true;
}

/**
* �Q�[���}�l�[�W���[�̏I��
*/
void GameManager::Finalize()
{
	// �Q�[���}�l�[�W���[�����݂��Ă���Ώ���
	if (manager)
	{
		delete manager;
		manager = nullptr;
	}
}

/**
* �Q�[���G���W�����擾����
*/
GameManager& GameManager::Get()
{
	return *manager;
}

/**
* �Q�[���̏�Ԃ��X�V����
*/
void GameManager::Update(float deltaTime)
{
	GameEngine& engine = GameEngine::Get();
	SceneManager& sceneManager = SceneManager::Get();

	switch (state)
	{
	case State::start:
	{	
		// �E�B���h�E�̑傫���擾
		glm::vec2 windowSize = engine.GetWindowSize();

		// �X�^�[�g�Ƃ͂��܂�܂ł̎��Ԃ�\��
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
			// �G�𓮂��Ȃ��悤�ɂ���
			for (auto& e : enemies)
			{
				static_cast<Enemy&>(*e).SetCanMove(false);
			}
			// �v���C���[�𓮂��Ȃ���Ԃɂ���
			static_cast<Player&>(*playerTank).SetCanMove(false);
		}
		else
		{
			ImGui::SetCursorPos(ImVec2(210, 100));
			ImGui::Text("Start");
			// �G�𓮂���悤�ɂ���
			for (auto& e : enemies)
			{
				static_cast<Enemy&>(*e).SetCanMove(true);
			}
			// �v���C���[�𓮂����Ԃɂ���
			static_cast<Player&>(*playerTank).SetCanMove(true);
		}
		
		// �X�^�[�g��\������c�莞�Ԃ����炷
		startDispTime -= deltaTime;

		ImGui::End();
		// ���Ԃ��߂�����Q�[�����n�߂�
		if (startDispTime <= 0)
		{
			state = State::playing;
			std::cout << __func__ << ": start -> playing\n";
			// ���Ԃ����Ƃɖ߂�
			startDispTime = startDispMaxTime;
		}
		
		break;
	}
	case State::playing:
	{
		// �Q�[���v���C�����葱����
		// esc�������ꂽ��Q�[���I��
		if (engine.GetKey(GLFW_KEY_ESCAPE))
		{
			engine.WindowClose();
		}

		Tank p = static_cast<Player&>(*playerTank);
		if (!p.GetCanMoving())
		{
			// �v���C���[������ł�����Q�[���I�[�o�[
			state = State::gameover;
			std::cout << __func__ << ": playing -> gameover\n";
		}
		else
		{
			// �G�l�~�[�̐��𐔂���
			int surviverCount = 0;
			for (int i = 0; i < enemies.size(); ++i)
			{
				Tank e = static_cast<Enemy&>(*enemies[i]);
				if (!e.GetCanMoving())
				{
					// �����Ȃ��G��Ԃ𐔂���
					++surviverCount;
				}
			}
			// �G�l�~�[���S�ē����Ȃ���Ԃł���΃N���A
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
		// ENTER�Ń^�C�g���ɖ߂�
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
		// ENTER�Ń^�C�g���ɖ߂�
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
* UI�̏�Ԃ��X�V
*/
void GameManager::UpdateUI()
{
	GameEngine& engine = GameEngine::Get();
	// �X�^�C���\���̂��擾
	ImGuiStyle& style = ImGui::GetStyle();
	// ���ɖ߂����߂̃o�b�N�A�b�v
	const ImGuiStyle styleBackup = style;

	// �v���C���[��HP�\��
	if (playerTank)
	{
		ImGui::SetWindowSize(ImVec2(100, 10));
		ImGui::SetNextWindowPos(ImVec2(450,600));
		ImGui::Begin("HP",nullptr,
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1, 0, 0, 1);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 0, 0, 0.5f);
		// �g�̑���
		style.FrameBorderSize = 3.0f;
		// �g�̐F
		style.Colors[ImGuiCol_Border] = ImVec4(1, 1, 1, 1);
		// �ӂ��̊ۂ�
		style.FrameRounding = 8.0f;
		const float maxPlayerHealth = 100;
		const float f = playerTank->health / maxPlayerHealth;
		ImGui::ProgressBar(f, ImVec2(0, 0), "");
		// �X�^�C�������ɖ߂�
		style = styleBackup;
		ImGui::End();
	}

	// �G�̐���\��
	{
		ImGui::SetWindowSize(ImVec2(10, 10));
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::Begin("EnemyCount", nullptr, ImGuiWindowFlags_NoBackground | 
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
		std::shared_ptr<Texture> tex = engine.LoadTexture("Res/IconEnemy.tga");
		const ImTextureID texId = reinterpret_cast<ImTextureID>(tex->GetID());
		ImGui::SameLine();
		// �G�摜�\��
		ImGui::Image(texId, ImVec2(40, 40));

		ImGui::SameLine();

		// �G�̐��𐔂���ϐ�
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
		// int����char*�ɕϊ�
		sst << enemyCount;
		// �t�H���g�̃T�C�Y��傫������
		ImGui::SetWindowFontScale(3);
		// �e�L�X�g�\��
		ImGui::Text(sst.str().c_str());

		ImGui::End();
	}
}

/**
* �v���C���[���쐬
*/
void GameManager::SpawnPlayerTank()
{
	// �ȑO�̃^�C�K�[I��Ԃ�����
	if (playerTank)
	{
		playerTank->isDead = true;
	}

	// �V�����v���C���[�̍쐬
	playerTank.reset(
		new Player{
		glm::vec3(0,0,50),
		glm::vec3(1),
		3.45f,
		TankType::TIGER });
	// �^�C�K�[I��Ԃ��Q�[���G���W���ɓo�^
	GameEngine::Get().AddActors(playerTank);

}

/**
* �G��Ԃ̍쐬
*/
void GameManager::SpawnEnemiesTank()
{
	// �Q�[���G���W���擾
	GameEngine& engine = GameEngine::Get();

	// �G�̏o���ʒu�z��
	glm::vec3 enemySpawnPositions[] =
	{
		glm::vec3(0,0,-50),
		glm::vec3(50,0,-50),
		glm::vec3(-50,0,-50),
		glm::vec3(5,0,-50),
		glm::vec3(10,0,-50),
	};

	// �ȑO�쐬����T-34��Ԃ�����
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
		// �V����T-34��Ԃ��쐬
		enemies.push_back(std::shared_ptr<Actor>(
			new Enemy{
			enemySpawnPositions[i],
			glm::vec3(1),
			0.0f,
			TankType::T34 }));
	}	

	// T-34��Ԃ��Q�[���G���W���ɓo�^
	for (int i = 0; i < enemies.size(); ++i)
	{
		engine.AddActors(enemies[i]);
	}
}

/**
* �Q�[���J�n
*/
void GameManager::GameStart()
{
	GameEngine& engine = GameEngine::Get();
	SpawnPlayerTank();
	SpawnEnemiesTank();
}

/**
* �Q�[���N���A
*/
void GameManager::GameClear()
{
	// �Q�[���G���W���擾
	GameEngine& engine = GameEngine::Get();

	// �E�B���h�E�̃T�C�Y�̎擾
	glm::vec2 windowSize = engine.GetWindowSize();
	// �e�N�X�`���擾
	std::shared_ptr<Texture> tex = engine.LoadTexture("Res/GameClear.tga");
	// �e�N�X�`���̃T�C�Y�̎擾
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

	//// �e�N�X�`����ImGui�Ŏg����悤�ɕϊ�
	//const ImTextureID texId = reinterpret_cast<ImTextureID>(tex->GetID());
	//// �摜�쐬
	//std::shared_ptr<UI> image = std::shared_ptr<UI>(new Image(
	//	"GameClear",
	//	glm::vec2(windowSize.x / 2, windowSize.y / 2),
	//	texSize, 1, texSize , texId));
	//// �摜��ǉ�
	//engine.AddUI(image);
}

/**
* �Q�[���I�[�o�[
*/
void GameManager::GameOver()
{
	// �Q�[���G���W���擾
	GameEngine& engine = GameEngine::Get();

	// �E�B���h�E�̃T�C�Y�̎擾
	glm::vec2 windowSize = engine.GetWindowSize();
	// �e�N�X�`���擾
	std::shared_ptr<Texture> tex = engine.LoadTexture("Res/GameOver.tga");
	// �e�N�X�`���̃T�C�Y�̎擾
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

	//// �e�N�X�`����ImGui�Ŏg����悤�ɕϊ�
	//const ImTextureID texId = reinterpret_cast<ImTextureID>(tex->GetID());
	//// �摜�쐬
	//std::shared_ptr<UI> image = std::shared_ptr<UI>(new Image(
	//	"GameClear",
	//	glm::vec2(windowSize.x / 2, windowSize.y / 2),
	//	texSize, 1, texSize, texId));
	//// �摜��ǉ�
	//engine.AddUI(image);
}