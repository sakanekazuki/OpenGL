/**
* @file NonPlayerCharacter.h
*/

#ifndef NONPLAYERCHARACTER_H_INCLUDE
#define NONPLAYERCHARACTER_H_INCLUDE

#include "../Actor.h"
#include "../Actor/Tank/Tank.h"

class NonPlayerCharacter:
	public Tank
{
public:
	NonPlayerCharacter(
		const glm::vec3& position,
		const glm::vec3& scale,
		float rotation,
		TankType type);
	~NonPlayerCharacter() = default;

	virtual void OnUpdate(float deltaTime)override;
	virtual void Move(float deltaTime);
	virtual void OnCollision(const Contact& contact)override;

protected:
	// �e�𔭎˂��Ă��玟�ɔ��˂ł���܂ł̍ő厞��
	const float shotMaxTimer = 5;
	// �e�𔭎˂��Ă��玟�ɔ��˂ł���܂ł̎���
	float shotTimer = shotMaxTimer;
	// �ǂ���������
	Actor* target = nullptr;
	// �ǂ��������Ԃ̃^�O
	std::string enemyTag;

	// �������Ă��ǂ������鎞��
	const float intervalTime = 10;
	// �ǂ�������c�莞��
	float interval = 0;
	// ��]���Ă���t���O
	bool isMoving = false;
	// ���G���ɑS�g���鎞��
	float moveTime = 3.0f;

};

#endif // NONPLAYERCHARACTER_H_INCLUDE