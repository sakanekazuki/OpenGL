/**
* @file Player.h
*/

#ifndef PLAYER_H_INCLUDE
#define PLAYER_H_INCLUDE

#include "../Actor/Tank/Tank.h"
#include "glm/gtc/matrix_transform.hpp"

class Player :
	public Tank
{
public:
	Player(glm::vec3 position,
		glm::vec3 scale,
		float rotation,
		TankType playerType);
	~Player() = default;

	void OnUpdate(float deltaTime)override;
	virtual void OnCollision(const Contact& contact)override;

private:
	// �O�񉟂����{�^��
	int oldShotButton = 0;
	// ���ɒe�𔭎˂���܂ł̍ő厞��
	const float maxInterval = 3;
	// ���ɒe�𔭎˂���܂ł̎���
	float interval;

	void CameraMove();

};

#endif // PLAYER_H_INCLUDE