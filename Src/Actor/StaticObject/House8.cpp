/**
* @file House8.cpp
*/

#include "House8.h"
#include "../../GameEngine.h"

/**
* コンストラクタ
*/
House8::House8(const glm::vec3& position)
	:Actor("家8", GameEngine::Get().GetPrimitive("Res/House/House7.obj"),
		GameEngine::Get().LoadTexture("Res/House/Houses Colorscheme 2.tga"),
		position, glm::vec3(0.01f, 0.01f, 0.01f), 0.0f, glm::vec3(0, 0, 0))
{
	// 当たり判定
	collider = Box::Create(glm::vec3(-5.5f, 0, -5.5f), glm::vec3(5.5f, 4, 5.5f));
	// 動かないオブジェクトにする
	isStatic = true;
}