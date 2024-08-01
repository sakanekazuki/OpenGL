/**
* @file Player.cpp
*/

#include "Player.h"
#include "../GLContext.h"
#include "../Actor/Item/Bullet.h"
#include "../GameEngine.h"
#include "../Intersect.h"
#include "../GameCursor.h"
#include "../Effect/Explosion.h"
#include "../Audio/Audio.h"
#include "../Audio/SE.h"
#include <iostream>

/**
* �R���X�g���N�^
*/
Player::Player(glm::vec3 position,
	glm::vec3 scale,
	float rotation,
	TankType type)
	:Tank{ position,
	scale,
	rotation,
	type }
	,oldShotButton(0)
{
	// �^�O�̐ݒ�
	tag = "player";
	collider = Box::Create(
		glm::vec3(-1.8f, 0, -1.8f),
		glm::vec3(1.8f, 2.8f, 1.8f));
	interval = maxInterval;
}

/**
* ��Ԃ̑���
*/
void Player::OnUpdate(float deltaTime)
{
	// �Q�[���G���W���̎擾
	GameEngine& engine = GameEngine::Get();

	// �����Ȃ���Ԃł���΁A�������Ȃ�
	if (!canMoving)
	{
		return;
	}	

	// A��D�������ꂽ�����
	if (engine.GetKey(GLFW_KEY_A))
	{
		rotation += glm::radians(90.0f) * deltaTime;
		tankTop.rotation += glm::radians(90.0f) * deltaTime;
	}
	else if (engine.GetKey(GLFW_KEY_D))
	{
		rotation -= glm::radians(90.0f) * deltaTime;
		tankTop.rotation -= glm::radians(90.0f) * deltaTime;
	}

	// ��Ԃ̌����x�N�g��
	glm::vec3 tankFront(0, 0, 1);
	// ��]�s��
	const glm::mat4 matRot = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
	tankFront = matRot * glm::vec4(tankFront, 1);

	// ��Ԃ̉����x
	float tankAccel = 0.2f;

	// �ړ�
	if (engine.GetKey(GLFW_KEY_W))
	{
		if (!(glm::length(velocity) >= maxSpeed))
		{
			// �O�i
			velocity += tankFront * tankAccel;
		}
	}
	else if (engine.GetKey(GLFW_KEY_S))
	{
		if (!(glm::abs(glm::length(velocity)) >= maxSpeed))
		{
			// ���
			velocity -= tankFront * tankAccel;
		}
	}
	// �����Ă����Ԃ��Ɖ����o��
	if (std::abs(glm::length(velocity)) >= 1 && Audio::Get().IsStop(2))
	{
		Audio::Get().Play(2, CRI_SE_TANK_MOVE);
	}
	else if(std::abs(glm::length(velocity)) <= 1)
	{
		Audio::Get().Stop(2);
	}

	// �C�̉�]

	// �J�[�\���擾
	GameCursor& cursor = GameCursor::Get();

	tankTop.rotation -= cursor.GetCursorMovePos().x / 2000;

	// �C�̌����x�N�g��
	glm::vec3 cannonFront(0, 0, 1);
	// ��]�s��
	const glm::mat4 matCannonRot = glm::rotate(glm::mat4(1), tankTop.rotation, glm::vec3(0, 1, 0));
	cannonFront = matCannonRot * glm::vec4(cannonFront, 1);

	// �}�E�X���{�^���̏�Ԃ��擾����
	int shotButton = engine.GetMouseButton(GLFW_MOUSE_BUTTON_LEFT);

	// �}�E�X���{�^���������ꂽ�u�Ԃɒe�A�N�^�[��ǉ�����
	if (shotButton != 0 && oldShotButton == 0 && interval <= 0)
	{
		// ���ˈʒu��C�̐�[�ɐݒ�
		glm::vec3 position = this->position + cannonFront * 7.5f;
		// �����̏����O�ɏo��
		position.y += 2.0f;
		// �e�̍쐬
		std::shared_ptr<Actor> bullet(new Bullet(position, attack));
		// �e�̈ړ���������
		bullet->velocity = cannonFront * 30.0f;
		// �e�̒ǉ�
		GameEngine::Get().AddActors(bullet);

		const glm::vec3 pos = this->position + glm::vec3(0, 2.2f, 0) + cannonFront * 7.0f;
		std::shared_ptr<Sprite> sprite = std::shared_ptr<Sprite>(new Explosion(pos, 0.05f));
		sprite->scale *= 0.2f;
		sprite->lifespan = 0.1f;
		sprite->gravityScale = 0;
		sprite->pixelsPerMeter = 10;
		engine.AddActors(sprite);

		interval = maxInterval;

		Audio::Get().Play(0, CRI_SE_SHOT2);
	}
	// �C���^�[�o�������炷
	interval -= deltaTime;

	//�u�O��̃V���b�g�{�^���̏�ԁv���X�V����
	oldShotButton = shotButton;

	// �C�̈ړ�
	tankTop.position = position;

	CameraMove();
}

/**
* �J�����̈ړ�
*/
void Player::CameraMove()
{
	// �Q�[���G���W���擾
	GameEngine& engine = GameEngine::Get();
	// �J�����擾
	Camera& camera = engine.GetCamera();
	// �J�[�\���擾
	GameCursor& cursor = GameCursor::Get();
	// �����ʒu�̏�����ɃJ�����̈ʒu���Œ�
	camera.position = this->position + cameraPosition;
	
	// ��Ԋm�F�p
	//camera.position = this->position + glm::vec3(0, 2, -10);

	// ALT��������Ă�����J��������]
	if (engine.GetKey(GLFW_KEY_LEFT_ALT))
	{
		// �J�[�\���̈ړ�������������]
		camera.Angle_y -= static_cast<float>(cursor.GetCursorMovePos().x) / 2000;
		// �J�[�\���̈ʒu���Œ�
		engine.SetCursorPos(glm::vec2(0, 0));
	}
	else
	{
		// ������Ă��Ȃ���ΌŒ�
		camera.Angle_y = tankTop.rotation;
	}
	camera.rotation_y = glm::rotate(glm::mat4(1), camera.Angle_y, glm::vec3(0, 1, 0));
	camera.Angle_x += static_cast<float>(cursor.GetCursorMovePos().y) / 2000;

	camera.rotation_x = glm::rotate(glm::mat4(1), camera.Angle_x, glm::vec3(1, 0, 0));
}

/**
* �����蔻��
* 
* @param contact �Փˏ��
*/
void Player::OnCollision(const Contact& contact)
{
	// �e�ɓ���������_���[�W���󂯂�
	if (contact.a->tag == "bullet"|| contact.b->tag == "bullet")
	{
		// �e�ɕϊ�
		Bullet bullet = static_cast<Bullet&>(*contact.a);
		// �e�̈З͂̃_���[�W���󂯂�
		AcceptDamage(bullet.GetPower());

		// HP��0�ł���΁A�����Ȃ��悤�ɂ���
		if (health <= 0)
		{
			canMoving = false;
		}
	}
}