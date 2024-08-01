/**
* @file Tank.h
*/

#ifndef TANK_H_INCLUDE
#define TANK_H_INCLUDE

#include "../Actor.h"
#include "../../GameEngine.h"

/**
* 戦車の種類
*/
enum class TankType
{
	TIGER = 0,
	T34,
};

/**
* 戦車の情報素材
*/
struct TankMaterial
{
	// アクターの名前
	const char* name;
	// プリミティブ
	const char* primNameBottom;
	// プリミティブ
	const char* primNameTop;
	// テクスチャー
	const char* texName;
	// 当たり判定
	const std::shared_ptr<Collider> collider;
	// 中心までの距離
	glm::vec3 adjustment;
	// 最大速度
	float maxSpeed;
	// 攻撃力
	int atk;
	// 装甲
	int armor;
	// 重量
	float mass;
	// 射程距離
	float rangeDistance;

	// カメラの位置
	glm::vec3 cameraPosition;
};

class Tank :
	public Actor
{
public:
	Tank(
		const glm::vec3& position,
		const glm::vec3& scale,
		float rotation,
		TankType type
		);
	virtual ~Tank() = default;

	virtual void OnUpdate(float deltaTime)override;

	virtual void OnCollision(const Contact& contact)override;

	void m_Draw(const ProgramPipeline& pipeline, const glm::mat4& matProj, const glm::mat4& matView)override;

	void AcceptDamage(int damage);

	void SetCanMove(bool canMove);
	bool GetCanMoving()const;

protected:
	// 砲
	Actor tankTop;
	// true = 動ける、false = 動けない
	bool canMoving = true;
	// 装甲
	const int armor;
	// 攻撃力
	const int attack;
	// 速さ
	const float maxSpeed;
	// 砲の回転する速さ
	const float cannnonRotationSpeed = 0.01f;
	// カメラの位置
	const glm::vec3 cameraPosition;
	// 射程距離
	float rangeDistance;

};

#endif // TANK_H_INCLUDE