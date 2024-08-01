/**
* @file Scene.h
*/

#ifndef SCENE_H_INCLUDE
#define SCENE_H_INCLUDE

#include "../Stage/Stage.h"
#include <string>

class Scene
{
public:
	Scene(std::string name,std::string stageName);
	virtual ~Scene();

	virtual void Update(float deltaTime);

	std::string GetName()
	{
		return name;
	}

private:
	// �X�e�[�W
	Stage stage;
	// �V�[���̖��O
	std::string name;

};

#endif // SCENE_H_INCLUDE