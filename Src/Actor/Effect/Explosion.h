/**
* @file Explosion.h
*/

#ifndef EXPLOSION_H_INCLUDE
#define EXPLOSION_H_INCLUDE

#include "../Actor/UI/Sprite.h"

/**
* �����G�t�F�N�g
*/
class Explosion :
	public Sprite
{
public:
	Explosion(const glm::vec3& pos,float speed);
	~Explosion() = default;

	void OnUpdate(float deltaTime)override;

private:
	// �傫���Ȃ鑬��
	const float speed;

};

#endif // EXPLOSION_H_INCLUDE