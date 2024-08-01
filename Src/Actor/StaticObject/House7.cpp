/**
* @file House7.cpp
*/

#include "House7.h"
#include "../../GameEngine.h"

/**
* コンストラクタ
*/
House7::House7(const glm::vec3& position)
	:Actor("家7", GameEngine::Get().GetPrimitive("Res/House/House6.obj"),
		GameEngine::Get().LoadTexture("Res/House/Houses Colorscheme 5.tga"),
		position, glm::vec3(1, 1, 1), 0.0f, glm::vec3(0, 0, 0))
{
	// 当たり判定
	collider = Box::Create(glm::vec3(-5.5f, 0, -5.5f), glm::vec3(5.5f, 4, 5.5f));
	// 動かないオブジェクトにする
	isStatic = true;
}