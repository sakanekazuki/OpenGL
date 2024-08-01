/**
* @file NonPlayerCharacter.cpp
*/

#include "NonPlayerCharacter.h"
#include "../Intersect.h"
#include "../Actor/Item/Bullet.h"
#include "../GameEngine.h"
#include <glm/ext.hpp>
#include <random>

/**
* �R���X�g���N�^
* 
* @param position ���W
* @param scale    �T�C�Y
* @param rotation ��]
* @param type     ��Ԃ̎��
*/
NonPlayerCharacter::NonPlayerCharacter(
	const glm::vec3& position,
	const glm::vec3& scale,
	float rotation,
	TankType type)
	:Tank{
	position,
	scale,
	rotation,
	type }
{

}

/**
* ��Ԃ̑���
*/
void NonPlayerCharacter::OnUpdate(float deltaTime)
{
	// �������Ȃ�
}

/**
* ����
*/
void NonPlayerCharacter::Move(float deltaTime)
{	
	
}

/**
* �����蔻��
* 
* @param contact �Փˏ��
*/
void NonPlayerCharacter::OnCollision(const Contact& contact)
{
	// �����������̂��e��������_���[�W���󂯂�
	if (contact.a->tag == "bullet"|| contact.b->tag == "bullet")
	{
		// �e�ɕϊ�
		Bullet bullet = static_cast<Bullet&>(*contact.a);
		// �e�̈З͂̃_���[�W���󂯂�
		AcceptDamage(bullet.GetPower());
		// �̗͂��c���Ă��Ȃ��ꍇ�͓����Ȃ���Ԃɂ���
		if (health <= 0)
		{
			canMoving = false;
		}
	}
	// �G��Ԃɓ��������ꍇ���̓G��Ԃ��^�[�Q�b�g�ɓ����
	else if (contact.a->tag == enemyTag || contact.b->tag == enemyTag)
	{
		(contact.a->tag == enemyTag) ?
			target = contact.a :
			target = contact.b;
	}
	// ���̕�(�n�ʈȊO)�ɓ�������������]��
	else if (contact.a->tag != "ground" && contact.b->tag != "ground")
	{
		// �����]������
		isMoving = false;
	}
}