/**
* @file Tank.cpp
*/

#include "Tank.h"
#include "../Actor/Item/Bullet.h"

const static int tankTypeNum = 2;

// 戦車の情報
TankMaterial tankMaterials[tankTypeNum] =
{
	{
		"Tiger-I",
		"Res/Tank/Tiger_bottom.obj",
		"Res/Tank/Tiger_top.obj",
		"Res/Tiger_I.tga",
		Box::Create(glm::vec3(-1.8f, 0, -1.8f), glm::vec3(1.8f, 2.8f, 1.8f)),
		glm::vec3(-0.12f,0.0f,0.0f), 4.0f, 135, 100, 50000,0,
		glm::vec3(0, 3, 0)
	},
	{
		"T34",
		"Res/Tank/T34_bottom.obj",
		"Res/Tank/T34_top.obj",
		"Res/T34.tga",
		Box::Create(glm::vec3(-1.5f,0, -1.5f), glm::vec3(1.5f, 2.5f, 1.5f)),
		glm::vec3(0,0,0), 5.0f, 120, 90, 32000,0,
		glm::vec3(0,3,0)
	},
};

/**
* コンストラクタ
*/
Tank::Tank(
	const glm::vec3& position,
	const glm::vec3& scale,
	float rotation,
	TankType type)
	:Actor(tankMaterials[static_cast<int>(type)].name,
		GameEngine::Get().GetPrimitiveBuffer().Find(tankMaterials[static_cast<int>(type)].primNameBottom),
		GameEngine::Get().LoadTexture(tankMaterials[static_cast<int>(type)].texName),
		position, scale, rotation,
		tankMaterials[static_cast<int>(type)].adjustment)
	, tankTop(tankMaterials[static_cast<int>(type)].name,
		GameEngine::Get().GetPrimitiveBuffer().Find(tankMaterials[static_cast<int>(type)].primNameTop),
		GameEngine::Get().LoadTexture(tankMaterials[static_cast<int>(type)].texName),
		position, scale, rotation,
		tankMaterials[static_cast<int>(type)].adjustment)
	, armor(tankMaterials[static_cast<int>(type)].armor)
	, attack(tankMaterials[static_cast<int>(type)].atk)
	, maxSpeed(tankMaterials[static_cast<int>(type)].maxSpeed)
	, cameraPosition(tankMaterials[static_cast<int>(type)].cameraPosition)
{
	mass = tankMaterials[static_cast<int>(type)].mass;
	collider = tankMaterials[static_cast<int>(type)].collider;
	health = 100;
}

/**
* 戦車更新
*/
void Tank::OnUpdate(float deltaTime)
{
	// 何もしない
}

/**
* 当たり判定
* 
* @param contact 衝突情報
*/
void Tank::OnCollision(const Contact& contact)
{
	// 弾に当たったらダメージを受ける
	if (contact.a->tag == "bullet"|| contact.b->tag == "bullet")
	{
		// 弾に変換
		Bullet bullet = static_cast<Bullet&>(*contact.a);
		// 弾の威力のダメージを受ける
		AcceptDamage(bullet.GetPower());
	}
}

/**
* 描画
*/
void Tank::m_Draw(const ProgramPipeline& pipeline, const glm::mat4& matProj, const glm::mat4& matView)
{
	Draw(*this, pipeline, matProj, matView);
	tankTop.m_Draw(pipeline, matProj, matView);
}

/**
* ダメージを受ける
* 
* @param damage 受けるダメージ
*/
void Tank::AcceptDamage(int damage)
{
	// ( 攻撃力 - 防御力 ) のダメージを受ける
	health -= (damage - armor);
}

/**
* 戦車が動くことができるかどうか
*/
bool Tank::GetCanMoving()const
{
	return canMoving;
}

/**
* 動く
* 
* @param canMove true  = 動ける
*                false = 動けない
*/
void Tank::SetCanMove(bool camMove)
{
	canMoving = camMove;
}