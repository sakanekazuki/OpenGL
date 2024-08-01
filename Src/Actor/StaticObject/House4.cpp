/**
* @file House4.cpp
*/

#include "House4.h"
#include "../../GameEngine.h"

/**
* コンストラクタ
*/
House4::House4(const glm::vec3& position)
	:Actor("家4", GameEngine::Get().GetPrimitive("Res/House/House3.obj"),
		GameEngine::Get().LoadTexture("Res/House/Houses Colorscheme 4.tga"),
		position, glm::vec3(1, 1, 1), 0.0f, glm::vec3(0, 0, 0))
{
	// 当たり判定
	collider = Box::Create(glm::vec3(-5.5f, 0, -5.5f), glm::vec3(5.5f, 4, 5.5f));
	// 動かないオブジェクトにする
	isStatic = true;
}