/**
* @file NonPlayerCharacter.cpp
*/

#include "NonPlayerCharacter.h"
#include "../Intersect.h"
#include "../Actor/Item/Bullet.h"
#include "../GameEngine.h"
#include <glm/ext.hpp>
#include <random>

/**
* コンストラクタ
* 
* @param position 座標
* @param scale    サイズ
* @param rotation 回転
* @param type     戦車の種類
*/
NonPlayerCharacter::NonPlayerCharacter(
	const glm::vec3& position,
	const glm::vec3& scale,
	float rotation,
	TankType type)
	:Tank{
	position,
	scale,
	rotation,
	type }
{

}

/**
* 戦車の操作
*/
void NonPlayerCharacter::OnUpdate(float deltaTime)
{
	// 何もしない
}

/**
* 動き
*/
void NonPlayerCharacter::Move(float deltaTime)
{	
	
}

/**
* 当たり判定
* 
* @param contact 衝突情報
*/
void NonPlayerCharacter::OnCollision(const Contact& contact)
{
	// 当たったものが弾だったらダメージを受ける
	if (contact.a->tag == "bullet"|| contact.b->tag == "bullet")
	{
		// 弾に変換
		Bullet bullet = static_cast<Bullet&>(*contact.a);
		// 弾の威力のダメージを受ける
		AcceptDamage(bullet.GetPower());
		// 体力が残っていない場合は動けない状態にする
		if (health <= 0)
		{
			canMoving = false;
		}
	}
	// 敵戦車に当たった場合その敵戦車をターゲットに入れる
	else if (contact.a->tag == enemyTag || contact.b->tag == enemyTag)
	{
		(contact.a->tag == enemyTag) ?
			target = contact.a :
			target = contact.b;
	}
	// 他の物(地面以外)に当たったら方向転換
	else if (contact.a->tag != "ground" && contact.b->tag != "ground")
	{
		// 方向転換する
		isMoving = false;
	}
}