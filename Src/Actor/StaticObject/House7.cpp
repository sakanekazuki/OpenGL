/**
* @file House7.cpp
*/

#include "House7.h"
#include "../../GameEngine.h"

/**
* �R���X�g���N�^
*/
House7::House7(const glm::vec3& position)
	:Actor("��7", GameEngine::Get().GetPrimitive("Res/House/House6.obj"),
		GameEngine::Get().LoadTexture("Res/House/Houses Colorscheme 5.tga"),
		position, glm::vec3(1, 1, 1), 0.0f, glm::vec3(0, 0, 0))
{
	// �����蔻��
	collider = Box::Create(glm::vec3(-5.5f, 0, -5.5f), glm::vec3(5.5f, 4, 5.5f));
	// �����Ȃ��I�u�W�F�N�g�ɂ���
	isStatic = true;
}