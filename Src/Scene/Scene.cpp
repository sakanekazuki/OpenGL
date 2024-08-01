/**
* @file Scene.cpp
*/

#include "Scene.h"
#include "../GameEngine.h"

/**
* �R���X�g���N�^
*/
Scene::Scene(std::string name,std::string stageName)
	:name(name)
	,stage(stageName)
{
	// �X�e�[�W�쐬
	stage.Create();
}

/**
* �f�X�g���N�^
*/
Scene::~Scene()
{
	GameEngine& engine = GameEngine::Get();

	// �A�N�^�[��S�ď���
	engine.GetActors().clear();
	engine.GetActors(Layer::UI).clear();
	engine.GetUI().clear();
}

/**
* �V�[���X�V
*/
void Scene::Update(float deltaTime)
{
	// �������Ȃ�
}