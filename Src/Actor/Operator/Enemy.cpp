/**
* @file Enemy.cpp
*/

#include "Enemy.h"
#include "../Actor/Item/Bullet.h"
#include "../GameEngine.h"
#include "../Intersect.h"
#include "../GameManager.h"
#include <glm/ext.hpp>

/**
* �R���X�g���N�^
*/
Enemy::Enemy(glm::vec3 position,
	glm::vec3 scale,
	float rotation,
	TankType type)
	:NonPlayerCharacter{position,
	scale,
	rotation,
	type}
{
	tag = "enemy";
	enemyTag = "player";
}

/**
* �G�̓���
*/
void Enemy::Move(float deltaTime)
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

	// �擾������
	Hit hit;

	// ��Ԃ̌����x�N�g��
	glm::vec3 tankFront = glm::vec3(0, 0, 1);
	// ��]�������]�s��
	const glm::mat4 matRot = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
	// �����x�N�g���̉�]
	tankFront = matRot * glm::vec4(0, 0, 1, 1);

	// ���C�̍ŏ��̈ʒu(�n�ʂɓ�����Ȃ��悤��y��1����,�����ɓ�����Ȃ��悤�ɏ����O�ɏo��)
	glm::vec3 start = position + glm::vec3(0, 1, 0) + tankFront * 2.6f;
	// ���C�̍Ō�̈ʒu(�n�ʂɓ�����Ȃ��悤��y��1����,�����ɓ�����Ȃ��悤�ɏ����O�ɏo��,50m��܂Ŕ�΂�)
	glm::vec3 end = position + tankFront * 50.0f + glm::vec3(0, 1, 0);

	// �G��T�����߂ɉ�]����̂ōŏ��̉�]��ۑ�����ϐ�
	static float rota;

	// ���C���΂�

	// �A�N�^�[�Ƀ��C���������Ă�����G�����ׂ�
	if (RayCast(start, end, hit))
	{
		// �G�Ƀ��C���������Ă�����^�[�Q�b�g�ɓ����
		if (hit.hitActor)
		{
			if (hit.hitActor->tag == enemyTag)
			{
				target = hit.hitActor.get();
				// �c�莞�Ԃ�ݒ�
				interval = intervalTime;
			}
			else
			{
				// �������܂ł̎��Ԃ�����
				interval -= deltaTime;
				// �c�莞�Ԃ������Ȃ�����^�[�Q�b�g����ɂ���
				if (interval <= 0)
				{
					target = nullptr;
					// �G�����������̂Ō��݂̉�]��ۑ�
					rota = rotation;
				}
			}
		}
	}
	// �^�[�Q�b�g��������ǂ�������
	if (target != nullptr)
	{
		// ���ʕ����̃x�N�g�����v�Z
		glm::mat4 matR = glm::rotate(
			glm::mat4(1), rotation, glm::vec3(0, 1, 0));
		glm::vec3 t34Front = matR * glm::vec4(0, 0, 1, 1);

		// �������炩��^�[�Q�b�g�ւ̃x�N�g��d���v�Z
		glm::vec3 d = target->position - position;

		// ��������^�[�Q�b�g�ւ̋������v�Z
		float length = glm::length(d);

		// �x�N�g��d�𐳋K��
		d = glm::normalize(d);

		// �����̐��ʃx�N�g���ƁA�^�[�Q�b�g�ւ̃x�N�g���̊p�x��
		float r = std::acos(glm::dot(tankFront, d));

		// �����̐��ʂƃ^�[�Q�b�g�̂�������̊p�x��10�x�����̏ꍇ
		if (r < glm::radians(10.0f))
		{
			// �^�[�Q�b�g�܂ł̋�����15m��艓���ꍇ�͑O�ɉ���
			if (length > 15.0f)
			{
				// �ő呬�x���x���ꍇ����������
				if (!(glm::length(velocity) >= maxSpeed))
				{
					velocity += tankFront * 0.3f;
				}
				// �ő呬�x�𒴂���ƍő呬�x����
				else
				{
					velocity = tankFront * maxSpeed;
				}
			}
			// 15m�����̏ꍇ�͌�������~
			else
			{
				// �x���V�e�B��t34Front�����̒������v�Z
				float v = glm::dot(tankFront, velocity);
				// ������0.2�ȏ�Ȃ�0.2�������A����ȉ��Ȃ璷����������(=��~)����
				velocity -= tankFront * glm::clamp(v, -0.2f, 0.2f);
			}
		}
		// �p�x��10�x�ȏ�̏ꍇ
		else
		{
			// �����̐��ʃx�N�g���ƁA�^�[�Q�b�g�ւ̃x�N�g���̊O�ς��v�Z
			glm::vec3 n = glm::cross(tankFront, d);
			// y��0�ȏ�Ȃ甽���v���A0�����Ȃ玞�v���ɉ�]����ق����߂�
			if (n.y >= 0)
			{
				rotation += glm::radians(90.0f) * deltaTime;
			}
			else
			{
				rotation -= glm::radians(90.0f) * deltaTime;
			}
		}

		// �C���^�[�o�������炷
		shotTimer -= deltaTime;
		// �e�𔭎˂���
		if (shotTimer <= 0)
		{
			// �C���^�[�o���̐ݒ�
			shotTimer = 5;
			// ���ˈʒu��C�̐�[�ɐݒ�
			glm::vec3 position = this->position + tankFront * 3.0f;
			position.y += 2.0f;

			// �Q�[���G���W���̎擾
			GameEngine& engine = GameEngine::Get();
			// �e�A�N�^�[���쐬
			std::shared_ptr<Actor> bullet(new Bullet{ position ,attack });
			// 1.5�b��ɒe������
			bullet->lifespan = 1.5f;
			// ��Ԃ̌����Ă��������20m/s�̑��x�ňړ�������
			bullet->velocity = tankFront * 20.0f;
			// �A�N�^�[��ǉ�
			engine.AddActors(bullet);
		}
	}
	// �^�[�Q�b�g�����Ȃ���΃^�[�Q�b�g��T��
	else
	{
		// �Ȃ������
		static float dir = 0;
		// ��]���I���p�x
		static float finishRotation = 0;
		// �O�i���鎞��
		static float time = moveTime;
		if (isMoving)
		{
			// ���߂������Ɍ���
			if (dir < 0)
			{
				// �^���������܂ŉ�]������
				if (rotation > finishRotation)
				{
					rotation -= glm::radians(90.0f) * deltaTime;
				}
				else
				{
					// �O�i����悤�ɂ���
					dir = 0;
				}
			}
			else if (dir > 0)
			{
				// �^���������܂ŉ�]������
				if (rotation < finishRotation)
				{
					rotation += glm::radians(90.0f) * deltaTime;
				}
				else
				{
					// �O�i����悤�ɂ���
					dir = 0;
				}
			}
			// �O�����ł���ΑO�ɐi��
			else
			{
				// �ő呬�x�ɂȂ�܂ő��x�����Z
				if (glm::length(velocity) < maxSpeed)
				{
					velocity += tankFront * 0.3f;
				}

				if (time < 0)
				{
					// �����Ȃ���Ԃɂ���
					isMoving = false;
					// �����鎞�Ԃ�������
					time = moveTime;
				}
				else
				{
					// �����鎞�Ԃ����炷
					time -= deltaTime;
				}
			}
		}
		else
		{
			// �������Ƃ̂ł������
			bool canMoveFront = false;
			bool canMoveRight = false;
			bool canMoveLeft = false;

			// ��]�p�x������ϐ�
			std::vector<float> direction;

			// �E���������߂�
			glm::vec3 right(1, 0, 0);
			// ��]�s��
			const glm::mat4 matRotR = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
			right = matRotR * glm::vec4(right, 1);

			// ���C���΂��ŏ��̈ʒu��ݒ�(�����ɓ�����Ȃ��悤�ɏ����E�ɏo���āA�n�ʂɓ�����Ȃ��悤��y��1����)
			glm::vec3 startRight = position + glm::vec3(0, 1, 0) + right * 3.0f;
			// ���C�̏I���ʒu��ݒ�(�n�ʂɓ�����Ȃ��悤��y��1����,�E��100m��΂�)
			glm::vec3 endRight = position + glm::vec3(0, 1, 0) + right * 5.0f;

			// �����������߂�
			glm::vec3 left(-1, 0, 0);
			// ��]�s��
			const glm::mat4 matRotL = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
			left = matRotL * glm::vec4(left, 1);
			// ���C���΂��ŏ��̈ʒu��ݒ�(�����ɓ�����Ȃ��悤�ɏ������ɏo���āA�n�ʂɓ�����Ȃ��悤��y��1����)
			glm::vec3 startLeft = position + glm::vec3(0, 1, 0) + left * 3.0f;
			// ���C�̏I���ʒu��ݒ�(�n�ʂɓ�����Ȃ��悤��y��1����,����100m��΂�)
			glm::vec3 endLeft = position + glm::vec3(0, 1, 0) + left * 5.0f;

			// �O���̃��C�̏��
			Hit frontHit;
			// �E�����̃��C�̏��
			Hit rightHit;
			// �������̃��C�̏��
			Hit leftHit;

			// �O���ɓG��Ԃ����Ȃ����Ƃ͒��ׂĂ���̂œG��Ԃ��͒��ׂȂ�
			// �O�ɃI�u�W�F�N�g���Ȃ���ΑO�ɓ�����
			if (!RayCast(start, end, frontHit))
			{
				canMoveFront = true;
				direction.push_back(0);
			}
			// �E�ɃI�u�W�F�N�g���Ȃ���ΉE�ɓ�����
			if (!RayCast(startRight, endRight, rightHit))
			{
				canMoveRight = true;
				direction.push_back(-90);
			}
			else
			{
				// �E�ɂ���I�u�W�F�N�g���G��Ԃł���΃^�[�Q�b�g�ɓ����
				if (rightHit.hitActor->tag == enemyTag)
				{
					target = rightHit.hitActor.get();
				}
			}

			// �E�ɓG��Ԃ����Ȃ���΍��𒲂ׂ�
			// ���ɓG��Ԃ������ꍇ target �� nullptr �ȊO�������Ă���
			if (target == nullptr)
			{
				// ���ɃI�u�W�F�N�g��������΍��ɓ�����
				if (!RayCast(startLeft, endLeft, leftHit))
				{
					canMoveLeft = true;
					direction.push_back(90);
				}
				else
				{
					// ���ɂ���I�u�W�F�N�g���G��Ԃł���΃^�[�Q�b�g�ɓ����
					if (leftHit.hitActor->tag == enemyTag)
					{
						target = leftHit.hitActor.get();
					}
				}
			}

			// �E�ɂ����ɂ��G��Ԃ��Ȃ��ꍇ�̂ݓ���
			// ���E�ǂ��炩�ɓG��Ԃ������ꍇ target �� nullptr �ȊO�������Ă���
			if (target == nullptr)
			{
				// ���E�ǂ��炩�ɓ������Ԃł���΁A�O���E�Ƀ����_���œ���
				if (canMoveFront || canMoveRight || canMoveLeft)
				{
					// �񌈒�I�ȗ����𐶐�
					std::random_device rand;
					// �����Z���k�E�c�C�X�^����
					std::mt19937 mt(rand());
					// �Ȃ�����������_���Ō���
					int turn = mt() % direction.size();
					// �Ȃ��������ۑ�
					dir = glm::radians(direction[turn]);
					// �Ȃ��������������
					direction.clear();
					// ��]���Ă����Ԃɂ���
					isMoving = true;
					// ��]�̏I����ݒ�
					finishRotation = rotation + dir;
				}
			}
		}
	}
}

/**
* �U��
* 
* @param deltaTime �O�񂩂�̌o�ߎ���
*/
void Enemy::Attack(float deltaTime)
{
	shotTimer -= deltaTime;
	// ���˂ł���܂ł̎��Ԃ�0�łȂ��Ȃ�U���ł��Ȃ�
	if (shotTimer >= 0)
	{
		return;
	}
	// ��Ԃ̌����x�N�g��
	glm::vec3 tankFront = glm::vec3(0, 0, 1);
	// ��]�������]�s��
	const glm::mat4 matRot = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
	// �����x�N�g���̉�]
	tankFront = matRot * glm::vec4(0, 0, 1, 1);
	
	// �C���^�[�o���̐ݒ�
	shotTimer = shotMaxTimer;
	// ���ˈʒu��C�̐�[�ɐݒ�
	glm::vec3 position = this->position + tankFront * 3.0f;
	position.y += 2.0f;

	// �Q�[���G���W���̎擾
	GameEngine& engine = GameEngine::Get();
	// �e�A�N�^�[���쐬
	std::shared_ptr<Actor> bullet(new Bullet{ position ,attack });
	// 1.5�b��ɒe������
	bullet->lifespan = 1.5f;
	// ��Ԃ̌����Ă��������20m/s�̑��x�ňړ�������
	bullet->velocity = tankFront * 20.0f;
	// �A�N�^�[��ǉ�
	engine.AddActors(bullet);
}

/** 
* ��Ԃ̍X�V
*/
void Enemy::OnUpdate(float deltaTime)
{	
	Move(deltaTime);
	Attack(deltaTime);
}