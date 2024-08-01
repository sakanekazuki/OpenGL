/**
* @file Scene.cpp
*/

#include "Scene.h"
#include "../GameEngine.h"

/**
* コンストラクタ
*/
Scene::Scene(std::string name,std::string stageName)
	:name(name)
	,stage(stageName)
{
	// ステージ作成
	stage.Create();
}

/**
* デストラクタ
*/
Scene::~Scene()
{
	GameEngine& engine = GameEngine::Get();

	// アクターを全て消す
	engine.GetActors().clear();
	engine.GetActors(Layer::UI).clear();
	engine.GetUI().clear();
}

/**
* シーン更新
*/
void Scene::Update(float deltaTime)
{
	// 何もしない
}