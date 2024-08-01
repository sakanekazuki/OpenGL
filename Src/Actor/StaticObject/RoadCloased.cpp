/**
* @file RoadCloased.cpp
*/

#include "RoadCloased.h"
#include "../../GameEngine.h"

/**
* コンストラクタ
*/
RoadCloased::RoadCloased(glm::vec3 position)
	:Actor("RoadClosed",GameEngine::Get().GetPrimitive("Res/RoadCloased.obj"),
		GameEngine::Get().LoadTexture("Res/RoadCloased.tga"),
		position,glm::vec3(1.5f),0.0f,glm::vec3(0,-0.2,0))
{
	isStatic = true;
	collider =
		Box::Create(glm::vec3(-8.5f, 0, -2), glm::vec3(8.5f, 4, 2));
}