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
	// ステージ
	Stage stage;
	// シーンの名前
	std::string name;

};

#endif // SCENE_H_INCLUDE