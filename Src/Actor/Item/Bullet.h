/**
* @file Bullet.h
*/

#ifndef BULLET_H_INCLUDE
#define BULLET_H_INCLUDE

#include "../Actor.h"

class Bullet:
	public Actor
{
public:
	Bullet(glm::vec3 position, int power);
	~Bullet() = default;

	void OnCollision(const struct Contact& contact)override;

	int GetPower()const;

private:
	// �З�
	const int power;

};

#endif // BULLET_H_INCLUDE