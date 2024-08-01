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
* �G���g���[�|�C���g
*/
int main()
{
#ifdef NDEBUG

	FreeConsole();

#endif // NDEBUG
	
	// �Q�[���G���W���̏�����
	GameEngine::Initialize();
	GameEngine& engine = GameEngine::Get();

	// 1/60�b�Ԋu�Ń��[�v�������邽�߂̎����ϐ�
	double loopTime = engine.GetTime();
	// �����̍���
	double diffLoopTime = 0;
	// �X�V�Ԋu
	const float deltaTime = 1.0f / 60.0f;

	// �Q�[���}�l�[�W���[�̏�����
	GameManager::Initialize();
	GameManager& manager = GameManager::Get();

	GameCursor::Initialize();
	GameCursor& cursor = GameCursor::Get();

	// �Q�[�����̃J�[�\���̏�Ԃ̐ݒ�
	// GLFW_CURSOR_NORMAL
	// GLFW_CURSOR_HIDDEN
	// GLFW_CURSOR_DISABLED
	cursor.SetCurosorStatus(GLFW_CURSOR_NORMAL);

	// �V�[���ǂݍ���
	SceneManager& sceneManager = SceneManager::Get();
	sceneManager.LoadScene("Title");

	// ���C�����[�v
	while (!engine.WindowShouldClose())
	{
		// Esc�ŏI��
		if (engine.GetKey(GLFW_KEY_ESCAPE))
		{
			break;
		}
		// �V�[�����X�V
		sceneManager.SceneUpdate();

		// ���ݎ������擾
		const double curLoopTime = engine.GetTime();
		// ���ݎ����ƑO�񎞍��̍����A�����̍����ɉ��Z
		diffLoopTime += curLoopTime - loopTime;
		// �O�񎞍������ݎ����ɍX�V
		loopTime = curLoopTime;
		// �����̍�����1/60�b�����Ȃ�A���[�v�̐擪�ɖ߂�
		if (diffLoopTime < 1.0 / 60.0)
		{
			continue;
		}

		if (diffLoopTime > 0.5f) 
		{
			diffLoopTime = deltaTime;
		}

		//! 
		//! �Q�[����Ԃ��X�V����
		//! 

		engine.NewFrame();

		float times = 0;

		// �A�N�^�[�̈ړ�
		for (; diffLoopTime >= deltaTime; diffLoopTime -= deltaTime)
		{
			engine.UpdateActors(deltaTime);
			engine.PostUpdateActors();
			engine.UpdatePhysics(deltaTime);
			// �����҂��̃A�N�^�[����������
			engine.RemoveDeadActors();
			
			times += deltaTime;
		}

		sceneManager.Update(times);
		engine.UpdateCamera();
		
		engine.UpdateUI(times);

		//!
		//! �Q�[����Ԃ�`�悷��
		//! 
		
		engine.RenderDefault();
		engine.RenderSprite();
		engine.RenderUI();
		engine.PostRender();
		engine.SwapBuffers();
	}
	// �J�[�\���̏I��
	cursor.Finalize();
	// �Q�[���}�l�[�W���̏I��
	manager.Finalize();
	// �Q�[���G���W�����I��
	engine.Finalize();

	return 0;
}