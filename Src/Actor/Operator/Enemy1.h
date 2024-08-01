/**
* @file Enemy1.h
*/

#ifndef ENEMY1_H_INCLUDE
#define ENEMY1_H_INCLUDE

#include "Enemy.h"

/**
* �G
* 
* �˒�������艓���Ƌ߂Â�
* �Ȃ�ׂ���������U������
*/
class Enemy1 :
	public Enemy
{
public:
	Enemy1(glm::vec3 position,
		glm::vec3 scale,
		float roation,
		TankType type);
	~Enemy1() = default;

	void Move(float deltaTime);

};

#endif // ENEMY1_H_INCLUDE