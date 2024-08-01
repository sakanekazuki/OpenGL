/**
* @file Tutorial.h
*/

#ifndef TUTORIAL_H_INCLUDE
#define TUTORIAL_H_INCLUDE

#define TUTORIALSCENE "Tutorial"

#include "Scene.h"

/**
* チュートリアルシーン
*/
class Tutorial
	:public Scene
{
public:
	Tutorial();
	~Tutorial() = default;

	void Update(float deltaTime)override;

};

#endif // TUTORIAL_H_INCLUDE