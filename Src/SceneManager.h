/**
* @file SceneManager.h
*/

#ifndef SCENEMANAGER_H_INCLUDE
#define SCENEMANAGER_H_INCLUDE

#include "Scene/Scene.h"
#include <string>
#include <memory>

class SceneManager
{
public:
	static bool Initialize();
	static void Finalize();
	static SceneManager& Get();

	void SceneUpdate();
	void Update(float deltaTime);
	void LoadScene(std::string sceneName);

private:
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	// 現在のシーン
	std::shared_ptr<Scene> scene = nullptr;
	// 次に読み込むシーン
	std::string nextSceneName = "";

};

#endif // SCENEMANAGER_H_INCLUDE