/**
* @file Bullet.cpp
*/

#include "Bullet.h"
#include "../../GameEngine.h"

/**
* コンストラクタ
* 
* @param primitive 描画するプリミティブ
* @param texture   描画に使うテクスチャ
* @param rotation  回転
* @param tankFront 戦車の向いている方向
*/
Bullet::Bullet(glm::vec3 position, int power)
	:Actor(Actor{ "Bullet",GameEngine::Get().GetPrimitive("Res/Bullet.obj"),
		GameEngine::Get().LoadTexture("Res/Bullet.tga"),
			position,glm::vec3(0.25f),
			0.0f,glm::vec3(0) })
	, power(power)
{
	// 寿命の設定
	lifespan = 5.0f;
	// 当たり判定をつける
	collider = Box::Create(glm::vec3(-0.25f), glm::vec3(0.25f));
	mass = 6.5f;
	friction = 1.0f;
	tag = "bullet";
	// 重力を受けない
	useGravity = false;
}

/**
* 当たり判定
* 
* @param contact 衝突情報
*/
void Bullet::OnCollision(const struct Contact& contact)
{
	// 何かに当たったら消える
	isDead = true;
}

/**
* 威力の取得
*/
int Bullet::GetPower()const
{
	return power;
}