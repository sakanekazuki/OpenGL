/**
* @file Enemy2.cpp
*/

#include "Enemy2.h"

/**
* コンストラクタ
* 
* @param position 初期位置
* @param scale    大きさ
* @param rotation 回転
* @param type     戦車の種類
*/
Enemy2::Enemy2(glm::vec3 position,
	glm::vec3 scale,
	float rotation,
	TankType type)
	:Enemy(position, scale,
		rotation, type)
{

}

/**
* 移動
* 
* @param deltaTime 前回からの更新時間
*/
void Enemy2::Move(float deltaTime)
{

}