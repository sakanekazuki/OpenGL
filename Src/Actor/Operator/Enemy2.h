/**
* @file Enemy2.h
*/

#ifndef ENEMY2_H_INCLUDE
#define ENEMY2_H_INCLUDE

#include "Enemy.h"

/**
* “G
* 
* “®‚«‰ñ‚é“G
*/
class Enemy2 :
	public Enemy
{
public:
	Enemy2(glm::vec3 position,
		glm::vec3 scale,
		float roation,
		TankType type);

	~Enemy2() = default;

	void Move(float deltaTime)override;

};

#endif // ENEMY2_H_INCLUDE