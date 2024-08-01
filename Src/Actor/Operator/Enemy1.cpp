/**
* @file Enemy1.cpp
*/

#include "Enemy1.h"

#include <glm/ext.hpp>

/**
* コンストラクタ
* 
* @param position 初期位置
* @param scale    大きさ
* @param sotation 回転
* @param type     戦車の種類
*/
Enemy1::Enemy1(glm::vec3 position,
	glm::vec3 scale,
	float roation,
	TankType type)
	:Enemy(position,
		scale,
		rotation,
		type)
{
	GameEngine& engine = GameEngine::Get();
	target = FindTag("player");
}

/**
* 移動
* 
* @param deltaTime 前回からの更新時間
*/
void Enemy1::Move(float deltaTime)
{
	// 砲の位置の設定
	tankTop.position = position;
	// 砲の回転の設定
	tankTop.rotation = rotation;
	// 動ける状態でなければ動かさない
	if (!canMoving)
	{
		return;
	}
	// 
	if (target)
	{
		// 戦車の向きベクトル
		glm::vec3 tankFront = glm::vec3(0, 0, 1);
		// 回転させる回転行列
		const glm::mat4 matRot = glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
		// 向きベクトルの回転
		tankFront = matRot * glm::vec4(0, 0, 1, 1);
		// 自分からからターゲットへのベクトルdを計算
		glm::vec3 d = target->position - position;

		// 自分からターゲットへの距離を計算
		float length = glm::length(d);

		// 自分の書面ベクトルとターゲットと、ターゲットのベクトルの角度差
		float r = std::acos(glm::dot(tankFront, d));
		// 自分の正面とターゲットのいる方向の角度が10度未満の場合
		if (r < glm::radians(10.0f))
		{
			// ターゲットまでの距離が射程距離より遠い場合は前に移動
			if (length > rangeDistance)
			{
				// 最大速度より遅い場合少しずつ加速
				if (!(glm::length(velocity) >= maxSpeed))
				{
					velocity += tankFront * 0.3f;
				}
				else
				{
					velocity = tankFront * maxSpeed;
				}
			}
			// 射程距離以下だった場合停止
			else
			{
				// ベロシティの長さを計算
				float v = glm::dot(tankFront, velocity);
				// 長さが0.2以上なら0.2を減速、それ以下なら長さ分を減速(=停止)する
				velocity = tankFront * glm::clamp(v, 0.2f, 0.2f);
			}
		}
		// 角度が10度以上の場合
		else
		{
			// 自分の正面ベクトルと、ターゲットのベクトルの外積を計算
			glm::vec3 n = glm::cross(tankFront, d);
			// yが0以上なら反時計回り、0未満なら時計回りに回転
			if (n.y >= 0)
			{
				rotation += glm::radians(90.0f) * deltaTime;
			}
			else
			{
				rotation -= glm::radians(90.0f) * deltaTime;
			}
		}
	}
}