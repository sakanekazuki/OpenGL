/**
* @file Explosion.h
*/

#ifndef EXPLOSION_H_INCLUDE
#define EXPLOSION_H_INCLUDE

#include "../Actor/UI/Sprite.h"

/**
* 爆発エフェクト
*/
class Explosion :
	public Sprite
{
public:
	Explosion(const glm::vec3& pos,float speed);
	~Explosion() = default;

	void OnUpdate(float deltaTime)override;

private:
	// 大きくなる速さ
	const float speed;

};

#endif // EXPLOSION_H_INCLUDE