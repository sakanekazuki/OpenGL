/**
* @file MainGame.cpp
*/

#include "MainGame.h"
#include "../GameManager.h"
#include "../GameEngine.h"

/**
* �R���X�g���N�^
*/
MainGame::MainGame()
	:Scene(MAINGAMESCENE,"Text/MapObject.txt")
{
	// �Q�[���G���W���̎擾
	GameEngine& engine = GameEngine::Get();
	// �Q�[���}�l�[�W���[�̎擾
	GameManager& manager = GameManager::Get();

	std::shared_ptr<Texture>tex = engine.LoadTexture("Res/House1.tga");
	// �v���C���[���o��
	manager.GameStart();
	// �V�F�[�_�[�̃��C�g�̐ݒ�
	engine.GetPipeline()->SetFragUniform(1, glm::vec3(-0.70, -0.59, -0.41));
	engine.GetPipeline()->SetFragUniform(2, glm::vec3(1.94, 1.65, 1.24));
	engine.GetPipeline()->SetFragUniform(3, glm::vec3(0.15, 0.10, 0.20));
	// �w�i�F�̐ݒ�
	engine.SetglClearColor(glm::vec3(0.2f, 0.9f, 0.9f));
	// �J�[�\�����݂��Ȃ�����
	engine.SetCursorType(GLFW_CURSOR_DISABLED);
}

/**
* �X�V
*/
void MainGame::Update(float deltaTime)
{
	GameManager& manager = GameManager::Get();
	manager.Update(deltaTime);
	manager.UpdateUI();
}