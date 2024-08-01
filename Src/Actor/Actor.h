/**
* Actor.h
*/

#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED

#include <glad/glad.h>
#include "../Primitive.h"
#include "../Texture.h"
#include "../ProgramPipeline.h"
#include "../Collision/Collision.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <memory>

/*
* 回転している時の当たり判定
*/
struct OBB
{
	// 位置
	glm::vec3 position;
	// 方向ベクトル
	glm::vec3 rotation[3];
	// 中心から面までの距離
	glm::vec3 lenght;
};

/**
* 表示レイヤー
*/
enum class Layer
{
	Default,
	Sprite,
	UI,
};

// レイヤーの数
static const size_t layerCount = 3;

/**
* 物体を制御するパラメータ
* 
* 名前(string)
* プリミティブ(Primitive)
* テクスチャ(Texture)
* 位置(glm::vec3)
* 拡大縮小(glm::vec3)
* 回転角度(float)
* 原点までの距離(glm::vec3)
* 
* 速度(glm::vec3)
* 寿命(float)
* 死亡フラグ(bool)
* 耐久値(float)
* 
* 衝突判定(Box)
* 質量(float)
* 反発係数(float)
* 摩擦係数(float)
* 動かせるどうか(bool)
*/
class Actor
{
public:
	Actor(
		const std::string& name,
		const Primitive& prim,
		std::shared_ptr<Texture> tex,
		const glm::vec3& position,
		const glm::vec3& scale,
		float rotation,
		const glm::vec3& adjustment
	);

	virtual ~Actor();
	virtual void OnUpdate(float deltaTime);
	virtual void OnCollision(const struct Contact& contact);
	virtual void OnTrigger(const struct Contact& contact);

	virtual void m_Draw(const ProgramPipeline& piepline, const glm::mat4& matProj, const glm::mat4& matView);

	// アクターの名前
	std::string name;
	// 描画するプリミティブ
	Primitive prim;
	// 描画に使うテクスチャ
	std::shared_ptr<Texture> tex;
	// 物体の位置
	glm::vec3 position;
	// 物体の拡大縮小
	glm::vec3 scale;
	// 物体の回転角度
	float rotation;
	// 物体を原点に移動するための距離
	glm::vec3 adjustment;
	// テクスチャに合成する色
	glm::vec4 color = glm::vec4(1);
	// 重力の影響度
	float gravityScale = 1;

	// 速度(メートル毎秒)
	glm::vec3 velocity = glm::vec3(0);
	// 寿命(秒、0以下なら寿命なし)
	float lifespan = 0;
	// true=死亡(消去待ち) false=生存中
	bool isDead = false;
	// 耐久値
	float health = 10;

	// 衝突判定
	std::shared_ptr<Collider> collider;
	// 質量(kg)
	float mass = 1;
	// 反発係数(0.0～1.0)
	float cor = 0.7f;
	// 摩擦係数(0.0～1.0)
	float friction = 0.7f;
	// false=動かせる物体 true=動かせない物体
	bool isStatic = false;
	// true=重力を受ける false=重力を受けない
	bool useGravity = true;
	// タグ
	std::string tag = "none";
	// 表示レイヤー
	Layer layer = Layer::Default;

};

void Draw(
	const Actor& actor,
	const ProgramPipeline& pipeline,
	const glm::mat4& matProj,
	const glm::mat4& matView
);

Actor* FindName(const char* name);
Actor* FindTag(const char* tag);

/**
* 衝突情報
*/
struct Contact
{
	Actor* a = nullptr;
	Actor* b = nullptr;
	// 衝突時点でのアクターAのベロシティ
	glm::vec3 velocityA = glm::vec3(0);
	// 衝突時点でのアクターBのベロシティ
	glm::vec3 velocityB = glm::vec3(0);
	// 浸透距離
	glm::vec3 penetration = glm::vec3(0);
	// 衝突面の法線
	glm::vec3 normal = glm::vec3(0);
	//衝突面の座標
	glm::vec3 position = glm::vec3(0);
	// 浸透距離の長さ
	float penLength = 0;
};

bool DetectCollision(Actor& actorA, Actor& actorB, Contact& contact);
void SolveContact(Contact& contact);
bool Equal(const Contact& ca, const Contact& cb);

#endif // ACTOR_H_INCLUDED