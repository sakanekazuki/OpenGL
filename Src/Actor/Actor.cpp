/*
* @file Actor.cpp
*/

#include "Actor.h"
#include "../GameEngine.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

/**
* �R���X�g���N�^
*/
Actor::Actor(const std::string& name,
	const Primitive& prim,
	std::shared_ptr<Texture> tex,
	const glm::vec3& position,
	const glm::vec3& scale,
	float rotation,
	const glm::vec3& adjustment)
	:name(name)
	,prim(prim)
	,tex(tex)
	,position(position)
	,scale(scale)
	,rotation(rotation)
	,adjustment(adjustment)
{

}

/**
* �f�X�g���N�^
*/
Actor::~Actor()
{

}

/*
* �A�N�^�[�̏�Ԃ��X�V����
* 
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void Actor::OnUpdate(float deltaTime)
{
	// �������Ȃ�
}

/**
* �Փ˂𔻒肷��
* 
* @param contact �Փˏ��
*/
void Actor::OnCollision(const struct Contact& contact)
{
	// �������Ȃ�
}

/**
* �Փ˂𔻒肷��
*/
void Actor::OnTrigger(const struct Contact& contact)
{
	// �������Ȃ�
}

/**
* �`��
*/
void Actor::m_Draw(const ProgramPipeline& pipeline,const glm::mat4& matProj,const glm::mat4& matView)
{
	Draw(*this, pipeline, matProj, matView);
}

/**
* ���̂�`�悷��
* 
* @param actor    �`��Ɏg������p�����[�^
* @param pipeline �`��Ɏg���v���O�����p�C�v���C��
* @param matProj  �`��Ɏg���v���W�F�N�g�s��
* @param matView  �`��Ɏg���r���[�s��
*/
void Draw(
	const Actor& actor,
	const ProgramPipeline& pipeline,
	const glm::mat4& matProj,
	const glm::mat4& matView
)
{
	// ���f���s����v�Z����
	glm::mat4 matT = glm::translate(glm::mat4(1), actor.position);
	glm::mat4 matR = glm::rotate(glm::mat4(1), actor.rotation, glm::vec3(0, 1, 0));
	glm::mat4 matS = glm::scale(glm::mat4(1), actor.scale);
	glm::mat4 matA = glm::translate(glm::mat4(1), actor.adjustment);
	glm::mat4 matModel = matT * matR * matS * matA;

	// MVP�s����v�Z����
	glm::mat4 matMVP = matProj * matView * matModel;

	// ���f���s���MVO�s���GPU�������ɃR�s�[����
	const GLint locMatTRS = 0;
	const GLint locMatModel = 1;
	pipeline.SetVertUniform(locMatTRS, matMVP);
	if (actor.layer == Layer::Default)
	{
		pipeline.SetVertUniform(locMatModel, matModel);
	}

	// �e�N�X�`�������蓖�Ă�
	actor.tex->Bind(0);
	// �v���~�e�B�u��`�悷��
	actor.prim.Draw();
}

/*
* �A�N�^�[�z�񂩂疼�O�̈�v����A�N�^�[����������
* 
* @param actors �����Ώۂ̔z��
* @param name   ��������A�N�^�[�̖��O
*/
Actor* FindName( const char* name)
{
	// gameengine�̃A�N�^�[�擾
	std::vector<std::shared_ptr<Actor>>& actors = GameEngine::Get().GetActors();
	// actors�z�񂩂�name����v����v�f�������ĕԂ�
	for (int i = 0; i < actors.size(); ++i)
	{
		if (actors[i]->name == name)
		{
			return actors[i].get();
		}
	}
	return nullptr;
}

/**
* �A�N�^�[�z�񂩂�^�O�̈�v����A�N�^�[����������
* 
* @param actors �����Ώۂ̔z��
* @param tag    ��������^�O�̖��O
*/
Actor* FindTag(const char* tag)
{
	// GameEngine�̃A�N�^�[�擾
	std::vector<std::shared_ptr<Actor>>& actors = GameEngine::Get().GetActors();
	// actors�z�񂩂�name����v����v�f�������ĕԂ�
	for (int i = 0; i < actors.size(); ++i)
	{
		if (actors[i]->tag == tag)
		{
			return actors[i].get();
		}
	}
	return nullptr;
}

/**
* �Փ˂���������
* 
* @param actorA  �Փ˂��Ă��邩���ׂ�A�N�^�[
* @param actorB  �Փ˂��Ă��邩���ׂ�A�N�^�[
* @param contact �Փˏ��
* 
* @retval true  �Փ˂��Ă���
* @retval false �Փ˂��Ă��Ȃ�
*/
bool DetectCollision(Actor& actorA, Actor& actorB, Contact& contact)
{
	// �������Ȃ����̓��m�͏Փ˂��Ȃ�
	if (actorA.isStatic && actorB.isStatic)
	{
		return false;
	}

	if (!actorA.collider || !actorB.collider)
	{
		return false;
	}

	// �֐��e�[�u��
	using CollisionFunc = bool(*)(Actor&, Actor&, Contact&);
	static const CollisionFunc funcTable[2][2] = {
		{CollisionBoxBox,CollisionBoxCylinder},
		{CollisionCylinderBox,CollisionCylinderCylinder},
	};

	// �}�`�̑g�ݍ��킹�ɑΉ�����֐������s����
	const int y = static_cast<int>(actorA.collider->GetShapeType());
	const int x = static_cast<int>(actorB.collider->GetShapeType());
	return funcTable[y][x](actorA, actorB, contact);
}

/**
* �d�Ȃ����������
* 
* @param contact �Փˏ��
*/
void SolveContact(Contact& contact)
{
	Actor& actorA = *contact.a;
	Actor& actorB = *contact.b;
	glm::vec3 penetration = contact.penetration;
	glm::vec3 normal = contact.normal;

	// �����W���̕��ϒl���v�Z
	float cor = (actorA.cor + actorB.cor) * 0.5f;

	// ���C�W���̕��ϒl���v�Z
	float friction =
		1.0f - (actorA.friction + actorB.friction) * 0.5f;

	// �u�A�N�^�[A�̑��΃x���V�e�B�v���v�Z
	glm::vec3 rv = actorA.velocity - actorB.velocity;

	// �Փ˖ʂƑ��΃x���V�e�B�ɕ��s�ȃx�N�g��(�^���W�F���g)���v�Z
	glm::vec3 tangent = glm::cross(normal, glm::cross(normal, rv));

	// �^���W�F���g�𐳋K��
	if (glm::length(tangent) > 0.000001f)
	{
		tangent = glm::normalize(tangent);
	}
	else
	{
		tangent = glm::vec3(0);
	}

	// ���C��
	float frictionForce = friction * 9.8f / 60.0f;

	// ���C�͂̍ő�l���v�Z
	float maxForce = std::abs(glm::dot(tangent, rv));

	// ���C�͂��ő�l�ɐ���
	frictionForce = std::min(frictionForce, maxForce);

	// �^���W�F���g�����̖��C�͂��v�Z
	glm::vec3 frictionVelocity = 
		normal.y * frictionForce * tangent;

	// �@�������̑��x���v�Z
	float ua = glm::dot(normal, actorA.velocity);
	float ub = glm::dot(normal, actorB.velocity);

	if (actorA.isStatic)
	{
		// �Փˌ�̑��x���v�Z
		float vb = ua + cor * (ua - ub);
		// �ՓˑO�̑��x��0�ɂ���
		actorB.velocity -= normal * ub;
		// �Փˌ�̑��x�����Z����
		actorB.velocity += normal * vb;
		// ���C�ɂ�鑬�x�����Z����
		actorB.velocity += frictionVelocity;

		// �d�Ȃ�̉���: �A�N�^�[A�͓������Ȃ��̂ŁA�A�N�^�[B����������
		actorB.position += penetration;
	}
	else if (actorB.isStatic)
	{
		// �Փˌ�̑��x���v�Z
		float va = ub + cor * (ub - ua);
		// �ՓˑO�̑��x��0�ɂ���
		actorA.velocity -= normal * ua;
		// �Փˌ�̑��x�����Z����
		actorA.velocity += normal * va;
		// ���C�ɂ�鑬�x���v�Z����
		actorA.velocity += frictionVelocity;

		// �d�Ȃ�̉���: �A�N�^�[B�͓������Ȃ��̂ŁA�A�N�^�[A����������
		actorA.position -= penetration;
	}
	else
	{
		// �Փˌ�̑��x���v�Z
		float massAB = actorA.mass + actorB.mass;
		float c = actorA.mass * ua + actorB.mass * ub;
		float va = (c + cor * actorB.mass * (ub - ua)) / massAB;
		float vb = (c + cor * actorA.mass * (ua - ub)) / massAB;

		// �ՓˑO�̑��x��0�ɂ���
		actorA.velocity -= normal * ua;
		actorB.velocity -= normal * ub;

		// �Փˌ�̑��x���v�Z����
		actorA.velocity += normal * va;
		actorB.velocity += normal * vb;

		// ���C�ɂ�鑬�x�����Z����
		actorA.velocity -= frictionVelocity;
		actorB.velocity += frictionVelocity;

		// �d�Ȃ�̉���
		actorA.position -= penetration * 0.5f;
		actorB.position += penetration * 0.5f;
	}
}

/**
* 2�̃R���^�N�g�\���̂����Ă��邩���ׂ�
* 
* @param ca ��r����R���^�N�g�\����A
* @param cb ��r����R���^�N�g�\����B
* 
* @retval true  ���Ă���
* @retval false ���Ă��Ȃ�
*/
bool Equal(const Contact& ca, const Contact& cb)
{
	// �Փ˖ʂ̋���������Ă���ꍇ�͎��Ă��Ȃ�
	if (glm::length(ca.position - cb.position) > 0.01f)
	{
		return false;
	}

	// �����Ȃ��A�N�^�[�̗L���ɂ���Ĕ���𕪂���
	bool hasStaticA = ca.a->isStatic || ca.b->isStatic;
	bool hasStaticB = cb.a->isStatic || cb.b->isStatic;
	switch (hasStaticA + hasStaticB * 2)
	{
	case 0b00: // A,B�Ƃ��ɓ����A�N�^�[�̂�
		// �A�N�^�[�������Ƃ���v�����玗�Ă���
		if (ca.a == cb.a && ca.b == cb.b)
		{
			break;
		}
		if (ca.a == cb.b && ca.b == cb.a)
		{
			break;
		}
		return false;

	case 0b01: // A=�����Ȃ��A�N�^�[���܂�,B=�����A�N�^�[�̂�
		// ��Ɏ��Ă��Ȃ��Ɣ��肷��
		return false;

	case 0b10: // A=�����A�N�^�[�̂�,B=�����Ȃ��A�N�^�[���܂�
		// ��Ɏ��Ă��Ȃ��Ɣ��肷��
		return false;

	case 0b11: // A,B�Ƃ��ɓ����Ȃ��A�N�^�[���܂�
	{
		// �����A�N�^�[���m����v�����玗�Ă���
		Actor* a = ca.a;
		if (ca.a->isStatic)
		{
			a = ca.b;
		}
		Actor* b = cb.a;
		if (cb.a->isStatic)
		{
			b = cb.b;
		}
		if (a == b)
		{
			break;
		}
	}
		return false;
	}

	// ���Ă���
	return true;
}