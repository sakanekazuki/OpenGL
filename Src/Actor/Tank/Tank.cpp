/**
* @file Tank.cpp
*/

#include "Tank.h"
#include "../Actor/Item/Bullet.h"

const static int tankTypeNum = 2;

// ��Ԃ̏��
TankMaterial tankMaterials[tankTypeNum] =
{
	{
		"Tiger-I",
		"Res/Tank/Tiger_bottom.obj",
		"Res/Tank/Tiger_top.obj",
		"Res/Tiger_I.tga",
		Box::Create(glm::vec3(-1.8f, 0, -1.8f), glm::vec3(1.8f, 2.8f, 1.8f)),
		glm::vec3(-0.12f,0.0f,0.0f), 4.0f, 135, 100, 50000,0,
		glm::vec3(0, 3, 0)
	},
	{
		"T34",
		"Res/Tank/T34_bottom.obj",
		"Res/Tank/T34_top.obj",
		"Res/T34.tga",
		Box::Create(glm::vec3(-1.5f,0, -1.5f), glm::vec3(1.5f, 2.5f, 1.5f)),
		glm::vec3(0,0,0), 5.0f, 120, 90, 32000,0,
		glm::vec3(0,3,0)
	},
};

/**
* �R���X�g���N�^
*/
Tank::Tank(
	const glm::vec3& position,
	const glm::vec3& scale,
	float rotation,
	TankType type)
	:Actor(tankMaterials[static_cast<int>(type)].name,
		GameEngine::Get().GetPrimitiveBuffer().Find(tankMaterials[static_cast<int>(type)].primNameBottom),
		GameEngine::Get().LoadTexture(tankMaterials[static_cast<int>(type)].texName),
		position, scale, rotation,
		tankMaterials[static_cast<int>(type)].adjustment)
	, tankTop(tankMaterials[static_cast<int>(type)].name,
		GameEngine::Get().GetPrimitiveBuffer().Find(tankMaterials[static_cast<int>(type)].primNameTop),
		GameEngine::Get().LoadTexture(tankMaterials[static_cast<int>(type)].texName),
		position, scale, rotation,
		tankMaterials[static_cast<int>(type)].adjustment)
	, armor(tankMaterials[static_cast<int>(type)].armor)
	, attack(tankMaterials[static_cast<int>(type)].atk)
	, maxSpeed(tankMaterials[static_cast<int>(type)].maxSpeed)
	, cameraPosition(tankMaterials[static_cast<int>(type)].cameraPosition)
{
	mass = tankMaterials[static_cast<int>(type)].mass;
	collider = tankMaterials[static_cast<int>(type)].collider;
	health = 100;
}

/**
* ��ԍX�V
*/
void Tank::OnUpdate(float deltaTime)
{
	// �������Ȃ�
}

/**
* �����蔻��
* 
* @param contact �Փˏ��
*/
void Tank::OnCollision(const Contact& contact)
{
	// �e�ɓ���������_���[�W���󂯂�
	if (contact.a->tag == "bullet"|| contact.b->tag == "bullet")
	{
		// �e�ɕϊ�
		Bullet bullet = static_cast<Bullet&>(*contact.a);
		// �e�̈З͂̃_���[�W���󂯂�
		AcceptDamage(bullet.GetPower());
	}
}

/**
* �`��
*/
void Tank::m_Draw(const ProgramPipeline& pipeline, const glm::mat4& matProj, const glm::mat4& matView)
{
	Draw(*this, pipeline, matProj, matView);
	tankTop.m_Draw(pipeline, matProj, matView);
}

/**
* �_���[�W���󂯂�
* 
* @param damage �󂯂�_���[�W
*/
void Tank::AcceptDamage(int damage)
{
	// ( �U���� - �h��� ) �̃_���[�W���󂯂�
	health -= (damage - armor);
}

/**
* ��Ԃ��������Ƃ��ł��邩�ǂ���
*/
bool Tank::GetCanMoving()const
{
	return canMoving;
}

/**
* ����
* 
* @param canMove true  = ������
*                false = �����Ȃ�
*/
void Tank::SetCanMove(bool camMove)
{
	canMoving = camMove;
}