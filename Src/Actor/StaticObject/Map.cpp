/**
* @file Map.cpp
*/

#include "Map.h"
#include "../../GameEngine.h"

/**
* コンストラクタ
*/
Map::Map(glm::vec3 position)
	:Actor("Ground", GameEngine::Get().GetPrimitive("Res/Ground.obj"),
		GameEngine::Get().LoadTexture("Res/Ground1.tga"),
		position,glm::vec3(1),0.0f,glm::vec3(0))
{
	// タグの設定
	tag = "ground";
	// 当たり判定
	collider =
		Box::Create(glm::vec3(-2, -10, -2), glm::vec3(2, 0, 2));
	// 動かないオブジェクト
	isStatic = true;
}