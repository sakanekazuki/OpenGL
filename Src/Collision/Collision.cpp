/**
* @file Collision.cpp
*/

#include "Collision.h"
#include "../Actor/Actor.h"

// 浮動小数点の0とみなす値
static const float epsilon = FLT_EPSILON * 8;

/**
* コンストラクタ
*/
Collider::Collider(ShapeTye type)
	:shapeType(type)
{

}

/**
* ShapeTypeの取得
*/
ShapeTye Collider::GetShapeType()const
{
	return shapeType;
}

/**
* コンストラクタ
*/
Box::Box() :Collider(ShapeTye::box)
{

}

/**
* コンストラクタ
* 
* @param min 最小値
* @param max 最大値
*/
Box::Box(const glm::vec3& min, const glm::vec3& max)
	:Collider(ShapeTye::box)
	,min(min)
	,max(max)
{

}

/**
* 直方体コライダーの作成
*/
std::shared_ptr<Box> Box::Create(const glm::vec3& min, const glm::vec3& max)
{
	return std::make_shared<Box>(min, max);
}

/**
* クローン作成
*/
std::shared_ptr<Collider> Box::Clone()const
{
	return std::make_shared<Box>(*this);
}

/**
* コンストラクタ　
*/
Cylinder::Cylinder()
	:Collider(ShapeTye::cylinder)
{

}

/**
* コンストラクタ
* 
* @param bottom 下の座標
* @param radius 半径
* @param height 高さ
*/
Cylinder::Cylinder(const glm::vec3& bottom, float radius, float height)
	:Collider(ShapeTye::cylinder)
{

}

/**
* 垂直コライダーの作成
*/
std::shared_ptr<Cylinder> Cylinder::Create(const glm::vec3& bottom,
	float radius, float height)
{
	return std::make_shared<Cylinder>(bottom, radius, height);
}

/**
* クローン作成
*/
std::shared_ptr<Collider> Cylinder::Clone()const
{
	return std::make_shared<Cylinder>(*this);
}

/**
* 直方体と直方体の衝突
*/
bool CollisionBoxBox(Actor& actorA, Actor& actorB, Contact& contact)
{
	// ワールド座標系のコライダーを計算する
	Box a = static_cast<Box&>(*actorA.collider);
	a.min += actorA.position;
	a.max += actorA.position;

	Box b = static_cast<Box&>(*actorB.collider);
	b.min += actorB.position;
	b.max += actorB.position;

	// aの左側面がbの右側面より右にあるなら、衝突していない
	const float dx0 = b.max.x - a.min.x;
	if (dx0 <= 0)
	{
		return false;
	}

	// aの右側面がbの左側面より左にあるなら、衝突していない
	const float dx1 = a.max.x - b.min.x;
	if (dx1 <= 0)
	{
		return false;
	}

	// aの下面がbの上面より上にあるなら、衝突していない
	const float dy0 = b.max.y - a.min.y;
	if (dy0 <= 0)
	{
		return false;
	}

	// aの上面がbの下面より下にあるなら、衝突していない
	const float dy1 = a.max.y - b.min.y;
	if (dy1 <= 0)
	{
		return false;
	}

	// aの奥側面がbの手前側面より手前にあるなら衝突していない
	const float dz0 = b.max.z - a.min.z;
	if (dz0 <= 0)
	{
		return false;
	}

	// aの手前側面よりbの奥側面が奥にあるなら、衝突していない
	const float dz1 = a.max.z - b.min.z;
	if (dz1 <= 0)
	{
		return false;
	}

	// XYZの各軸について「浸透距離(重なっている部分の長さ)」が短い方向を選択する
	// 衝突面(アクターBのいずれかの面)の法線
	glm::vec3 normal;
	// 浸透距離と方向
	glm::vec3 penetration;
	if (dx0 <= dx1)
	{
		penetration.x = -dx0;
		normal.x = 1;
	}
	else
	{
		penetration.x = dx1;
		normal.x = -1;
	}
	if (dy0 <= dy1)
	{
		penetration.y = -dy0;
		normal.y = 1;
	}
	else
	{
		penetration.y = dy1;
		normal.y = -1;
	}
	if (dz0 <= dz1)
	{
		penetration.z = -dz0;
		normal.z = 1;
	}
	else
	{
		penetration.z = dz1;
		normal.z = -1;
	}

	// 浸透距離の絶対値
	glm::vec3 absPenetration = glm::abs(penetration);

	// 衝突面になる可能性の高さ
	glm::vec3 score = glm::vec3(0);

	// 浸透距離が短い方向のほうが衝突面である可能性が高い
	for (int a = 0; a < 2; ++a)
	{
		for (int b = a + 1; b < 3; ++b)
		{
			if (absPenetration[a] < absPenetration[b])
			{
				++score[a];
			}
			else
			{
				++score[b];
			}
		}
	}

	// 相対ベロシティを計算する
	glm::vec3 rv = actorA.velocity - actorB.velocity;

	// 浸透が始まった時間tを計算する
	glm::vec3 t = glm::vec3(-FLT_MAX);
	for (int i = 0; i < 3; ++i)
	{
		if (rv[i])
		{
			t[i] = penetration[i] / rv[i];
		}
	}

	// 浸透が始まった時間が大きいほど、より早い時点で浸透が始まったと考えられる
	const float deltaTime = 1.0f / 60.0f;
	for (int a = 0; a < 2; ++a)
	{
		for (int b = a + 1; b < 3; ++b)
		{
			int i = a;
			if (t[a] < t[b])
			{
				i = b;
			}
			if (t[i] > 0 && t[i] <= deltaTime)
			{
				score[i] += 1.5f;
			}
		}
	}

	// より可能性が低い方向を除外する
	// 値が等しい場合、Z,X,Yの順で優先的に除外する
	if (score.x <= score.y)
	{
		normal.x = 0;
		if (score.z <= score.y)
		{
			normal.z = 0;
		}
		else
		{
			normal.y = 0;
		}
	}
	else
	{
		normal.y = 0;
		if (score.z <= score.x)
		{
			normal.z = 0;
		}
		else
		{
			normal.x = 0;
		}
	}

	// XYZ軸のうち、浸透距離が最も短い軸の成分だけを残す
	if (absPenetration.x >= absPenetration.y)
	{
		penetration.x = 0;
		if (absPenetration.z >= absPenetration.y)
		{
			penetration.z = 0;
		}
		else
		{
			penetration.y = 0;
		}
	}
	else
	{
		penetration.y = 0;
		if (absPenetration.x >= absPenetration.z)
		{
			penetration.x = 0;
		}
		else
		{
			penetration.z = 0;
		}
	}

	// 衝突情報を設定する
	contact.a = &actorA;
	contact.b = &actorB;
	contact.velocityA = actorA.velocity;
	contact.velocityB = actorB.velocity;
	contact.penetration = penetration;
	contact.normal = normal;

	// 衝突面の座標を計算する
	{
		// 基本的にアクターBの座標を使うが、アクターBが静物の場合はアクターAの座標を使う
		Actor* target = &actorB;
		glm::vec3 targetNormal = normal;
		if (actorB.isStatic)
		{
			target = &actorA;
			// 法線の向きを反転する
			targetNormal *= -1;
		}
		// コライダーの半径を計算する
		const Box& targetBox = static_cast<Box&>(*target->collider);
		glm::vec3 halfSize = (targetBox.max + targetBox.min) * 0.5f;
		// コライダーの中心座標を計算する
		glm::vec3 center =
			(targetBox.max + targetBox.min) * 0.5f;
		// 衝突面の座標を計算する
		contact.position =
			target->position + center - halfSize * targetNormal;
	}

	// 浸透距離の長さを計算する
	contact.penLength = glm::length(penetration);

	// 衝突している
	return true;
}

/**
* 直方体と円柱の衝突
*/
bool CollisionBoxCylinder(Actor& actorA, Actor& actorB, Contact& contact)
{
	// ワールド座標系の衝突図形を計算する

	// ボックスに変換
	Box a = static_cast<Box&>(*actorA.collider);
	// 当たる位置の計算
	a.min += actorA.position;
	a.max += actorA.position;

	// 垂直円柱に変換
	Cylinder b = static_cast<Cylinder&>(*actorB.collider);
	// 当たる位置の計算
	b.bottom += actorB.position;

	// aの下面がbの上面より上にあるなら、衝突していない
	const float dy0 = (b.bottom.y + b.height) - a.min.y;
	if (dy0 <= 0)
	{
		return false;
	}

	// aの上面がbの下面より下にあるなら、衝突していない
	const float dy1 = a.max.y - b.bottom.y;
	if (dy1 <= 0)
	{
		return false;
	}

	// 円柱の中心に最も近い直方体内の座標系(最近接点)を求める
	const float cx = glm::clamp(b.bottom.x, a.min.x, a.max.x);
	const float cz = glm::clamp(b.bottom.z, a.min.z, a.max.z);
	const glm::vec3 closestPointXZ(cx, 0, cz);

	// 円柱の中心から最近接点までの距離を計算する
	const float dx = closestPointXZ.x - b.bottom.x;
	const float dz = closestPointXZ.z - b.bottom.z;
	const float d2 = dx * dx + dz * dz;

	// 最近接点までの距離が円柱の半径より長ければ、衝突していない
	if (d2 > b.radius * b.radius)
	{
		return false;
	}

	// Y方向の浸透距離と方向を計算
	glm::vec3 penetration(0);
	glm::vec3 normal(0);
	if (dy0 < dy1)
	{
		penetration.y = -dy0;
		normal.y = 1;
	}
	else
	{
		penetration.y = dy1;
		normal.y = -1;
	}

	// XZ方向の交差した方向を調べる(最近接点が含まれる面に対応するビットを立てる)
	
	// min方向のビットフラグ
	int flagMin = 0;
	// max方向のビットフラグ
	int flagMax = 0;

	for (int i = 0; i < 3; i += 2)
	{
		if (closestPointXZ[i] <= a.min[i])
		{
			flagMin |= (1 << i);
		}
	}

	const int flag = flagMin | flagMax;

	if (flag)
	{
		// XZ最近接点が直方体の表面にある場合、XZ最近接点と円を結ぶ直線を法線とする
		if (d2 >= epsilon)
		{
			normal.x = dx;
			normal.z = dz;
		}
		else
		{
			// 最近接点と円柱の中心が近すぎると法線を計算
			// かわりに最近接点が含まれる面から求める
			for (int i = 0; i < 3; i += 2)
			{
				if (flagMin & (1 << i))
				{
					normal[i] = -1;
				}
			}
		}
		// XZ方向の法線を正規化
		const float invD =
			1.0f / std::sqrt(normal.x * normal.x + normal.z * normal.z);
		normal.x *= invD;
		normal.z *= invD;
	}
	else
	{
		// XZ最近接点が直方体の内側にある場合、
		// ベロシティから衝突の可能性があると判断される面のうち、最も近い面で衝突したと考える
		// ベルシティが0の場合、全ての面に衝突の可能性があると判断する

		// 相対ベロシティを計算
		const glm::vec3 rv = actorA.velocity - actorB.velocity;
		const bool noVelocity = glm::dot(rv, rv) < epsilon;

		float dmin = FLT_MAX;
		// 最も近い面
		int nearestFace = 0;
		for (int i = 0; i < 3; i += 2)
		{
			if (rv[i] < 0 || noVelocity)
			{
				float d = closestPointXZ[i] - a.min[i];
				if (d < dmin)
				{
					dmin = d;
					nearestFace = i;
				}
			}
			if (rv[i] > 0 || noVelocity)
			{
				float d = a.max[i] - closestPointXZ[i];
				if (d < dmin)
				{
					dmin = d;
					nearestFace = i + 3;
				}
			}
		}
		// 最も近い面の法線を設定
		if (nearestFace < 3)
		{
			normal[nearestFace] = 1;
		}
		else
		{
			normal[nearestFace] = -1;
		}
	}

	// XZ方向の浸透距離を計算
	float distance = b.radius;
	if (d2 >= epsilon)
	{
		distance -= std::sqrt(d2);
	}
	penetration.x = -normal.x * distance;
	penetration.z = -normal.z * distance;

	// 浸透距離が長い方向を除外
	// 側面の衝突がある場合、XZベクトルの長さとYを比較
	const glm::vec3 absPenetration = glm::abs(penetration);
	if (flag && absPenetration.y > distance)
	{
		penetration.y = 0;
		normal.y = 0;
	}
	else
	{
		// 側面衝突以外の場合、最も浸透距離が短い軸だけを残す
		float pmin = FLT_MAX;
		int axisMin = 0;
		for (int i = 0; i < 3; ++i)
		{
			if (absPenetration[i] < pmin)
			{
				pmin = absPenetration[i];
				axisMin = i;
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			if (i != axisMin)
			{
				penetration[i] = 0;
				normal[i] = 0;
			}
		}
	}

	// 衝突情報を設定
	contact.a = &actorA;
	contact.b = &actorB;
	contact.velocityA = actorA.velocity;
	contact.velocityB = actorB.velocity;
	contact.penetration = penetration;
	contact.normal = normal;
	contact.penLength = glm::length(penetration);

	// 衝突面の座標を計算
	{
		// 基本的にアクターBの座標を使うが、アクターBが静物の場合はアクターAの座標を使う
		const glm::vec3 center = (a.min + a.max) * 0.5f;
		const glm::vec3 size = (a.max - a.min) * 0.5f;
		if (actorB.isStatic)
		{
			contact.position = center;
			if (normal.y)
			{
				// Y方向の衝突の場合
				contact.position.y -= size.y * normal.y;
			}
			else
			{
				// XZ方向の衝突の場合
				contact.position.x -= size.x * normal.x;
				contact.position.z -= size.z * normal.z;
			}
		}
		else
		{
			contact.position = b.bottom;
			if (normal.y)
			{
				// Y方向の衝突の場合
				contact.position.y += b.height * (0.5f + 0.5f * normal.y);
			}
			else
			{
				// XZ方向の衝突の場合・・・
				contact.position.x -= normal.x * b.radius;
				contact.position.z -= normal.z * b.radius;
			}
		}
	}
	// 衝突している
	return true;
}

/**
* 円柱と円柱の衝突
*/
bool CollisionCylinderCylinder(Actor& actorA, Actor& actorB, Contact& contact)
{
	// ワールド座標系の衝突図形を計算
	Cylinder a = static_cast<Cylinder&>(*actorA.collider);
	a.bottom += actorA.position;

	Cylinder b = static_cast<Cylinder&>(*actorB.collider);
	b.bottom += actorB.position;

	// aの下面がbの上面より上にあるなら、衝突していない
	const float dy0 = (b.bottom.y + b.height) - a.bottom.y;
	if (dy0 <= 0)
	{
		return false;
	}

	const float dy1 = (a.bottom.y + a.height) - b.bottom.y;
	if (dy1 <= 0)
	{
		return false;
	}

	// XZ平面上の距離が半径の合計より遠ければ、衝突していない
	const float dx = b.bottom.x - a.bottom.x;
	const float dz = b.bottom.z - a.bottom.z;
	const float d2 = dx * dx + dz * dz;
	const float r = a.radius + b.radius;
	if (d2 > r * r)
	{
		return false;
	}

	// Y方向の浸透距離と方向を計算
	glm::vec3 normal(0);
	glm::vec3 penetration(0);
	if (dy0 < dy1)
	{
		penetration.y = -dy0;
		normal.y = 1;
	}
	else
	{
		penetration.y = dy1;
		normal.y = -1;
	}

	// XZ方向の浸透距離と方向を計算
	float lenghtXZ;
	if (d2 >= epsilon)
	{
		const float d = std::sqrt(d2);
		const float invD = 1.0f / d;
		normal.x = -dx * invD;
		normal.z = -dz * invD;
		lenghtXZ = r - d;
	}
	else
	{
		// XZ座標が重なっている場合、法線を計算できないのでベロシティで代用
		lenghtXZ = r;
		normal.x = actorA.velocity.x - actorB.velocity.x;
		normal.z = actorA.velocity.z - actorB.velocity.z;
		if (normal.x || normal.z)
		{
			const float invD =
				1.0f * std::sqrt(normal.x * normal.x + normal.z * normal.z);
			normal.x *= invD;
			normal.z *= invD;
		}
		else
		{
			// ベロシティが0の場合は方向を確定できないので;X方向にしておく
			normal.x = 1;
		}
	}

	penetration.x = -lenghtXZ * normal.x;
	penetration.z = -lenghtXZ * normal.z;

	// 浸透距離の長い方向を除外
	if (std::abs(penetration.y) <= lenghtXZ)
	{
		penetration.x = penetration.z = 0;
		normal.x = normal.z = 0;
	}
	else
	{
		penetration.y = 0;
		normal.y = 0;
	}

	// 衝突情報を設定
	contact.a = &actorA;
	contact.b = &actorB;
	contact.velocityA = actorA.velocity;
	contact.velocityB = actorB.velocity;
	contact.normal = normal;
	contact.penLength = glm::length(penetration);

	// 衝突面の座標を計算
	{
		// 基本的にアクターBの座標を使うが、アクターBが静物の場合はアクターAの座標を使う
		Actor* target = &actorB;
		Cylinder* targetCollider = &b;
		glm::vec3 targetNormal = normal;
		if (actorB.isStatic)
		{
			target = &actorA;
			targetCollider = &a;
			// 法線の向きを反転
			targetNormal *= -1;
		}

		// 衝突面の座標を計算
		contact.position = targetCollider->bottom;
		if (normal.y)
		{
			// Y方向の衝突の場合
			if(targetNormal.y>=0)
			{
				contact.position.y += targetCollider->height;
			}
			else
			{
				// XZ方向の衝突の場合
				contact.position.x -= targetNormal.x * targetCollider->radius;
				contact.position.y += targetCollider->height * 0.5f;
				contact.position.z -= targetNormal.z * targetCollider->radius;
			}
		}
	}
	// 衝突している
	return true;
}

/**
* 円柱と直方体の衝突
*/
bool CollisionCylinderBox(Actor& actorA, Actor& actorB, Contact& contact)
{
	return CollisionBoxCylinder(actorB, actorA, contact);
}