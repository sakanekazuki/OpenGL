/**
* @file Enemy2.cpp
*/

#include "Enemy2.h"

/**
* �R���X�g���N�^
* 
* @param position �����ʒu
* @param scale    �傫��
* @param rotation ��]
* @param type     ��Ԃ̎��
*/
Enemy2::Enemy2(glm::vec3 position,
	glm::vec3 scale,
	float rotation,
	TankType type)
	:Enemy(position, scale,
		rotation, type)
{

}

/**
* �ړ�
* 
* @param deltaTime �O�񂩂�̍X�V����
*/
void Enemy2::Move(float deltaTime)
{

}