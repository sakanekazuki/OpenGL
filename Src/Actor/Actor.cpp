/*
* @file Actor.cpp
*/

#include "Actor.h"
#include "../GameEngine.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

/**
* コンストラクタ
*/
Actor::Actor(const std::string& name,
	const Primitive& prim,
	std::shared_ptr<Texture> tex,
	const glm::vec3& position,
	const glm::vec3& scale,
	float rotation,
	const glm::vec3& adjustment)
	:name(name)
	,prim(prim)
	,tex(tex)
	,position(position)
	,scale(scale)
	,rotation(rotation)
	,adjustment(adjustment)
{

}

/**
* デストラクタ
*/
Actor::~Actor()
{

}

/*
* アクターの状態を更新する
* 
* @param deltaTime 前回の更新からの経過時間(秒)
*/
void Actor::OnUpdate(float deltaTime)
{
	// 何もしない
}

/**
* 衝突を判定する
* 
* @param contact 衝突情報
*/
void Actor::OnCollision(const struct Contact& contact)
{
	// 何もしない
}

/**
* 衝突を判定する
*/
void Actor::OnTrigger(const struct Contact& contact)
{
	// 何もしない
}

/**
* 描画
*/
void Actor::m_Draw(const ProgramPipeline& pipeline,const glm::mat4& matProj,const glm::mat4& matView)
{
	Draw(*this, pipeline, matProj, matView);
}

/**
* 物体を描画する
* 
* @param actor    描画に使う制御パラメータ
* @param pipeline 描画に使うプログラムパイプライン
* @param matProj  描画に使うプロジェクト行列
* @param matView  描画に使うビュー行列
*/
void Draw(
	const Actor& actor,
	const ProgramPipeline& pipeline,
	const glm::mat4& matProj,
	const glm::mat4& matView
)
{
	// モデル行列を計算する
	glm::mat4 matT = glm::translate(glm::mat4(1), actor.position);
	glm::mat4 matR = glm::rotate(glm::mat4(1), actor.rotation, glm::vec3(0, 1, 0));
	glm::mat4 matS = glm::scale(glm::mat4(1), actor.scale);
	glm::mat4 matA = glm::translate(glm::mat4(1), actor.adjustment);
	glm::mat4 matModel = matT * matR * matS * matA;

	// MVP行列を計算する
	glm::mat4 matMVP = matProj * matView * matModel;

	// モデル行列とMVO行列をGPUメモリにコピーする
	const GLint locMatTRS = 0;
	const GLint locMatModel = 1;
	pipeline.SetVertUniform(locMatTRS, matMVP);
	if (actor.layer == Layer::Default)
	{
		pipeline.SetVertUniform(locMatModel, matModel);
	}

	// テクスチャを割り当てる
	actor.tex->Bind(0);
	// プリミティブを描画する
	actor.prim.Draw();
}

/*
* アクター配列から名前の一致するアクターを検索する
* 
* @param actors 検索対象の配列
* @param name   検索するアクターの名前
*/
Actor* FindName( const char* name)
{
	// gameengineのアクター取得
	std::vector<std::shared_ptr<Actor>>& actors = GameEngine::Get().GetActors();
	// actors配列からnameが一致する要素を見つけて返す
	for (int i = 0; i < actors.size(); ++i)
	{
		if (actors[i]->name == name)
		{
			return actors[i].get();
		}
	}
	return nullptr;
}

/**
* アクター配列からタグの一致するアクターを検索する
* 
* @param actors 検索対象の配列
* @param tag    検索するタグの名前
*/
Actor* FindTag(const char* tag)
{
	// GameEngineのアクター取得
	std::vector<std::shared_ptr<Actor>>& actors = GameEngine::Get().GetActors();
	// actors配列からnameが一致する要素を見つけて返す
	for (int i = 0; i < actors.size(); ++i)
	{
		if (actors[i]->tag == tag)
		{
			return actors[i].get();
		}
	}
	return nullptr;
}

/**
* 衝突を処理する
* 
* @param actorA  衝突しているか調べるアクター
* @param actorB  衝突しているか調べるアクター
* @param contact 衝突情報
* 
* @retval true  衝突している
* @retval false 衝突していない
*/
bool DetectCollision(Actor& actorA, Actor& actorB, Contact& contact)
{
	// 動かせない物体同士は衝突しない
	if (actorA.isStatic && actorB.isStatic)
	{
		return false;
	}

	if (!actorA.collider || !actorB.collider)
	{
		return false;
	}

	// 関数テーブル
	using CollisionFunc = bool(*)(Actor&, Actor&, Contact&);
	static const CollisionFunc funcTable[2][2] = {
		{CollisionBoxBox,CollisionBoxCylinder},
		{CollisionCylinderBox,CollisionCylinderCylinder},
	};

	// 図形の組み合わせに対応する関数を実行する
	const int y = static_cast<int>(actorA.collider->GetShapeType());
	const int x = static_cast<int>(actorB.collider->GetShapeType());
	return funcTable[y][x](actorA, actorB, contact);
}

/**
* 重なりを解決する
* 
* @param contact 衝突情報
*/
void SolveContact(Contact& contact)
{
	Actor& actorA = *contact.a;
	Actor& actorB = *contact.b;
	glm::vec3 penetration = contact.penetration;
	glm::vec3 normal = contact.normal;

	// 反発係数の平均値を計算
	float cor = (actorA.cor + actorB.cor) * 0.5f;

	// 摩擦係数の平均値を計算
	float friction =
		1.0f - (actorA.friction + actorB.friction) * 0.5f;

	// 「アクターAの相対ベロシティ」を計算
	glm::vec3 rv = actorA.velocity - actorB.velocity;

	// 衝突面と相対ベロシティに平行なベクトル(タンジェント)を計算
	glm::vec3 tangent = glm::cross(normal, glm::cross(normal, rv));

	// タンジェントを正規化
	if (glm::length(tangent) > 0.000001f)
	{
		tangent = glm::normalize(tangent);
	}
	else
	{
		tangent = glm::vec3(0);
	}

	// 摩擦力
	float frictionForce = friction * 9.8f / 60.0f;

	// 摩擦力の最大値を計算
	float maxForce = std::abs(glm::dot(tangent, rv));

	// 摩擦力を最大値に制限
	frictionForce = std::min(frictionForce, maxForce);

	// タンジェント方向の摩擦力を計算
	glm::vec3 frictionVelocity = 
		normal.y * frictionForce * tangent;

	// 法線方向の速度を計算
	float ua = glm::dot(normal, actorA.velocity);
	float ub = glm::dot(normal, actorB.velocity);

	if (actorA.isStatic)
	{
		// 衝突後の速度を計算
		float vb = ua + cor * (ua - ub);
		// 衝突前の速度を0にする
		actorB.velocity -= normal * ub;
		// 衝突後の速度を加算する
		actorB.velocity += normal * vb;
		// 摩擦による速度を加算する
		actorB.velocity += frictionVelocity;

		// 重なりの解決: アクターAは動かせないので、アクターBだけ動かす
		actorB.position += penetration;
	}
	else if (actorB.isStatic)
	{
		// 衝突後の速度を計算
		float va = ub + cor * (ub - ua);
		// 衝突前の速度を0にする
		actorA.velocity -= normal * ua;
		// 衝突後の速度を加算する
		actorA.velocity += normal * va;
		// 摩擦による速度を計算する
		actorA.velocity += frictionVelocity;

		// 重なりの解決: アクターBは動かせないので、アクターAだけ動かす
		actorA.position -= penetration;
	}
	else
	{
		// 衝突後の速度を計算
		float massAB = actorA.mass + actorB.mass;
		float c = actorA.mass * ua + actorB.mass * ub;
		float va = (c + cor * actorB.mass * (ub - ua)) / massAB;
		float vb = (c + cor * actorA.mass * (ua - ub)) / massAB;

		// 衝突前の速度を0にする
		actorA.velocity -= normal * ua;
		actorB.velocity -= normal * ub;

		// 衝突後の速度を計算する
		actorA.velocity += normal * va;
		actorB.velocity += normal * vb;

		// 摩擦による速度を加算する
		actorA.velocity -= frictionVelocity;
		actorB.velocity += frictionVelocity;

		// 重なりの解決
		actorA.position -= penetration * 0.5f;
		actorB.position += penetration * 0.5f;
	}
}

/**
* 2つのコンタクト構造体が似ているか調べる
* 
* @param ca 比較するコンタクト構造体A
* @param cb 比較するコンタクト構造体B
* 
* @retval true  似ている
* @retval false 似ていない
*/
bool Equal(const Contact& ca, const Contact& cb)
{
	// 衝突面の距離が離れている場合は似ていない
	if (glm::length(ca.position - cb.position) > 0.01f)
	{
		return false;
	}

	// 動かないアクターの有無によって判定を分ける
	bool hasStaticA = ca.a->isStatic || ca.b->isStatic;
	bool hasStaticB = cb.a->isStatic || cb.b->isStatic;
	switch (hasStaticA + hasStaticB * 2)
	{
	case 0b00: // A,Bともに動くアクターのみ
		// アクターが両方とも一致したら似ている
		if (ca.a == cb.a && ca.b == cb.b)
		{
			break;
		}
		if (ca.a == cb.b && ca.b == cb.a)
		{
			break;
		}
		return false;

	case 0b01: // A=動かないアクターを含む,B=動くアクターのみ
		// 常に似ていないと判定する
		return false;

	case 0b10: // A=動くアクターのみ,B=動かないアクターを含む
		// 常に似ていないと判定する
		return false;

	case 0b11: // A,Bともに動かないアクターを含む
	{
		// 動くアクター同士が一致したら似ている
		Actor* a = ca.a;
		if (ca.a->isStatic)
		{
			a = ca.b;
		}
		Actor* b = cb.a;
		if (cb.a->isStatic)
		{
			b = cb.b;
		}
		if (a == b)
		{
			break;
		}
	}
		return false;
	}

	// 似ている
	return true;
}