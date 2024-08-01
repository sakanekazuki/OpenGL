/**
* @file House1.cpp
*/

#include "House1.h"
#include "../../GameEngine.h"

/**
* コンストラクタ
*/
House1::House1(const glm::vec3& position)
	:Actor("家1", GameEngine::Get().GetPrimitive("Res/House/Brick_House.obj"),
		GameEngine::Get().LoadTexture("Res/House/brick_house_D.tga"),
		position, glm::vec3(0.006f, 0.006f, 0.006f), 0.0f, glm::vec3(0, 0, 0))
{
	// 当たり判定
	collider = Box::Create(glm::vec3(-4.5f, 0, -2.8f), glm::vec3(3.5f, 4, 4.5f));
	// 動かないオブジェクトにする
	isStatic = true;
}