/**
* @file Map.cpp
*/

#include "Map.h"
#include "../../GameEngine.h"

/**
* �R���X�g���N�^
*/
Map::Map(glm::vec3 position)
	:Actor("Ground", GameEngine::Get().GetPrimitive("Res/Ground.obj"),
		GameEngine::Get().LoadTexture("Res/Ground1.tga"),
		position,glm::vec3(1),0.0f,glm::vec3(0))
{
	// �^�O�̐ݒ�
	tag = "ground";
	// �����蔻��
	collider =
		Box::Create(glm::vec3(-2, -10, -2), glm::vec3(2, 0, 2));
	// �����Ȃ��I�u�W�F�N�g
	isStatic = true;
}