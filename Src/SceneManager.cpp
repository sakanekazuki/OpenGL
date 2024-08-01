/**
* @file SceneManager.cpp
*/

#include "SceneManager.h"
#include "Scene/TItle.h"
#include "Scene/MainGame.h"
#include "Scene/Tutorial.h"
#include <iostream>

namespace
{
	SceneManager* scenemanager = nullptr;
}

/**
* �V�[���}�l�[�W���[������
*/
bool SceneManager::Initialize()
{
	if (!scenemanager)
	{
		scenemanager = new SceneManager;
		if (!scenemanager)
		{
			std::cout << "SceneManager�̏������Ɏ��s\n";
			return false;
		}
	}
	std::cout << "SceneManager�̏������ɐ���\n";

	return true;
}

/**
* �V�[���}�l�[�W���[�I��
*/
void SceneManager::Finalize()
{
	if (scenemanager)
	{
		delete scenemanager;
		scenemanager = nullptr;
	}
}

/**
* �V�[���}�l�[�W���[�̎擾
*/
SceneManager& SceneManager::Get()
{
	return *scenemanager;
}

/**
* �V�[���X�V
*/
void SceneManager::SceneUpdate()
{
	// �ǂݍ��ރV�[���ɉ��������Ă��Ȃ���Ί֐����o��
	if (nextSceneName == "")
	{
		return;
	}

	if (nextSceneName == MAINGAMESCENE)
	{
		scene.reset();
		scene.reset(new MainGame());
	}
	else if (nextSceneName == TITLESCENE)
	{
		scene.reset();
		scene.reset(new Title());	
	}
	else if (nextSceneName == TUTORIALSCENE)
	{
		scene.reset();
		scene.reset(new Tutorial());
	}
	// �ǂݍ��ރV�[���̖��O��������
	nextSceneName = "";
}

/**
* �V�[���}�l�[�W���[�X�V
*/
void SceneManager::Update(float deltaTime)
{
	scene->Update(deltaTime);
}

/**
* ���ɓǂݍ��ރV�[���̐ݒ�
* 
* @param sceneName �ǂݍ��ރV�[���̖��O
*/
void SceneManager::LoadScene(std::string sceneName)
{
	if (scene)
	{
		if (sceneName == scene->GetName())
		{
			return;
		}
	}
	
	nextSceneName = sceneName;
}