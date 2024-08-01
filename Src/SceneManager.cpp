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
* シーンマネージャー初期化
*/
bool SceneManager::Initialize()
{
	if (!scenemanager)
	{
		scenemanager = new SceneManager;
		if (!scenemanager)
		{
			std::cout << "SceneManagerの初期化に失敗\n";
			return false;
		}
	}
	std::cout << "SceneManagerの初期化に成功\n";

	return true;
}

/**
* シーンマネージャー終了
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
* シーンマネージャーの取得
*/
SceneManager& SceneManager::Get()
{
	return *scenemanager;
}

/**
* シーン更新
*/
void SceneManager::SceneUpdate()
{
	// 読み込むシーンに何も入っていなければ関数を出る
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
	// 読み込むシーンの名前を初期化
	nextSceneName = "";
}

/**
* シーンマネージャー更新
*/
void SceneManager::Update(float deltaTime)
{
	scene->Update(deltaTime);
}

/**
* 次に読み込むシーンの設定
* 
* @param sceneName 読み込むシーンの名前
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