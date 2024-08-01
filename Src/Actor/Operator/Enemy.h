/**
* @file Enemy.h
*/

#ifndef ENEMY_H_INCLUDE
#define ENEMY_H_INCLUDE

#include "NonPlayerCharacter.h"

class Enemy :
	public NonPlayerCharacter
{
public:
	Enemy(glm::vec3 position,
		glm::vec3 scale,
		float roation,
		TankType type);
	~Enemy() = default;

	virtual void Move(float deltaTime)override;

	virtual void Attack(float deltaTime);

	void OnUpdate(float deltaTime)override;

private:
	// –Ú“I’n
	glm::vec3 distination = glm::vec3(0);

};

#endif // ENEMY_H_INCLUDE