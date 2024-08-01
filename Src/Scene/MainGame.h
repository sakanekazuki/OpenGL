/**
* @file MainGame.h
*/

#ifndef MAINGAME_H_INCLUDE
#define MAINGAME_H_INCLUDE

#include "Scene.h"
#define MAINGAMESCENE "MainGame"

class MainGame :
	public Scene
{
public:
	MainGame();
	~MainGame() = default;

	void Update(float deltaTime)override;

private:

};

#endif // MAINGAME_H_INCLUDE