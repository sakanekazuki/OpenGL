/**
* @file Enemy1.cpp
*/

#include "Enemy1.h"

#include <glm/ext.hpp>

/**
* �R���X�g���N�^
* 
* @param position �����ʒu
* @param scale    �傫��
* @param sotation ��]
* @param type     ��Ԃ̎��
*/
Enemy1::Enemy1(glm::vec3 position,
	glm::vec3 scale,
	float roation,
	TankType type)
	:Enemy(position,
		scale,
		rotation,
		type)
{
	GameEngine& engine = GameEngine::Get();
	target = FindTag("player");
}

/**
* �ړ�
* 
* @param deltaTime �O�񂩂�̍X�V����
*/
void Enemy1::Move(float deltaTime)
{
	// �C�̈ʒu�̐ݒ�
	tankTop.position = position;
	// �C�̉�]�̐ݒ�
	tankTop.rotation = rotation;
	// �������ԂłȂ���Γ������Ȃ�
	if (!canMoving)
	{
		return;
	}
	// 
	if (target)
	{
		// ��Ԃ̌����x�N�g��
		glm::vec3 tankFront = glm::vec3(0, 0, 1);
		// ��]�������]�s��
		const glm::mat4 matRot = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
		// �����x�N�g���̉�]
		tankFront = matRot * glm::vec4(0, 0, 1, 1);
		// �������炩��^�[�Q�b�g�ւ̃x�N�g��d���v�Z
		glm::vec3 d = target->position - position;

		// ��������^�[�Q�b�g�ւ̋������v�Z
		float length = glm::length(d);

		// �����̏��ʃx�N�g���ƃ^�[�Q�b�g�ƁA�^�[�Q�b�g�̃x�N�g���̊p�x��
		float r = std::acos(glm::dot(tankFront, d));
		// �����̐��ʂƃ^�[�Q�b�g�̂�������̊p�x��10�x�����̏ꍇ
		if (r < glm::radians(10.0f))
		{
			// �^�[�Q�b�g�܂ł̋������˒�������艓���ꍇ�͑O�Ɉړ�
			if (length > rangeDistance)
			{
				// �ő呬�x���x���ꍇ����������
				if (!(glm::length(velocity) >= maxSpeed))
				{
					velocity += tankFront * 0.3f;
				}
				else
				{
					velocity = tankFront * maxSpeed;
				}
			}
			// �˒������ȉ��������ꍇ��~
			else
			{
				// �x���V�e�B�̒������v�Z
				float v = glm::dot(tankFront, velocity);
				// ������0.2�ȏ�Ȃ�0.2�������A����ȉ��Ȃ璷����������(=��~)����
				velocity = tankFront * glm::clamp(v, 0.2f, 0.2f);
			}
		}
		// �p�x��10�x�ȏ�̏ꍇ
		else
		{
			// �����̐��ʃx�N�g���ƁA�^�[�Q�b�g�̃x�N�g���̊O�ς��v�Z
			glm::vec3 n = glm::cross(tankFront, d);
			// y��0�ȏ�Ȃ甽���v���A0�����Ȃ玞�v���ɉ�]
			if (n.y >= 0)
			{
				rotation += glm::radians(90.0f) * deltaTime;
			}
			else
			{
				rotation -= glm::radians(90.0f) * deltaTime;
			}
		}
	}
}