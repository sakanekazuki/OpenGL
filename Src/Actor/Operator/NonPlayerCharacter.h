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
	// 弾を発射してから次に発射できるまでの最大時間
	const float shotMaxTimer = 5;
	// 弾を発射してから次に発射できるまでの時間
	float shotTimer = shotMaxTimer;
	// 追いかける戦車
	Actor* target = nullptr;
	// 追いかける戦車のタグ
	std::string enemyTag;

	// 見失っても追いかける時間
	const float intervalTime = 10;
	// 追いかける残り時間
	float interval = 0;
	// 回転しているフラグ
	bool isMoving = false;
	// 索敵時に全身する時間
	float moveTime = 3.0f;

};

#endif // NONPLAYERCHARACTER_H_INCLUDE