/**
* @file Bullet.cpp
*/

#include "Bullet.h"
#include "../../GameEngine.h"

/**
* �R���X�g���N�^
* 
* @param primitive �`�悷��v���~�e�B�u
* @param texture   �`��Ɏg���e�N�X�`��
* @param rotation  ��]
* @param tankFront ��Ԃ̌����Ă������
*/
Bullet::Bullet(glm::vec3 position, int power)
	:Actor(Actor{ "Bullet",GameEngine::Get().GetPrimitive("Res/Bullet.obj"),
		GameEngine::Get().LoadTexture("Res/Bullet.tga"),
			position,glm::vec3(0.25f),
			0.0f,glm::vec3(0) })
	, power(power)
{
	// �����̐ݒ�
	lifespan = 5.0f;
	// �����蔻�������
	collider = Box::Create(glm::vec3(-0.25f), glm::vec3(0.25f));
	mass = 6.5f;
	friction = 1.0f;
	tag = "bullet";
	// �d�͂��󂯂Ȃ�
	useGravity = false;
}

/**
* �����蔻��
* 
* @param contact �Փˏ��
*/
void Bullet::OnCollision(const struct Contact& contact)
{
	// �����ɓ��������������
	isDead = true;
}

/**
* �З͂̎擾
*/
int Bullet::GetPower()const
{
	return power;
}