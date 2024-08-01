/**
* @file Intersect.cpp
*/

#include "Intersect.h"
#include "GameEngine.h"
#include <algorithm>

/**
* 線分と平面が交差する座標を求める
*
* @param start  線分の始点
* @param end    線分の終点
* @param q      平面上の任意の点
* @param normal 平面の法線
* @param p      交点の座標が代入される
*
* @retval true  交差している
* @retval false 交差していない
*/
bool Intersect(const glm::vec3& start, const glm::vec3& end,
	const glm::vec3& q, const glm::vec3& normal, glm::vec3& p)
{
	const float distance = glm::dot(normal, q - start);
	const glm::vec3 v = end - start;

	// 分母がほぼ0の場合、線分は平面と平行なので交差しない
	const float denom = glm::dot(normal, v);
	if (std::abs(denom) < 0.0001f)
	{
		return false;
	}

	// 交点までの距離tが0未満または1より大きい場合、交点は線分の外側にあるので実際には交差しない
	const float t = distance / denom;
	if (t < 0 || t > 1)
	{
		return false;
	}

	// 交点は線分上にある
	p = start + v * t;
	return true;
}

/**
* 線とアクターの当たり判定を行う
*
* @param start       線の最初の位置
* @param end         線の終わりの位置
* @param rayPosition 当たった位置
*/
bool RayCast(glm::vec3 start, glm::vec3 end, Hit& hit)
{
	// true=当たった,false=当たってない
	bool isHit = false;
	// 当たり判定を行うアクターの取得
	std::vector<std::shared_ptr<Actor>> actor = GameEngine::Get().GetActors(Layer::Default);

	/**
	* アクターを並べ変える
	*/
	struct actorDistance
	{
		std::shared_ptr<Actor> actor;
		float distance;
	};

	std::vector<actorDistance> actordistance;

	// アクター全て入れる
	for (int i = 0; i < actor.size(); ++i)
	{
		actordistance.push_back(actorDistance{ actor[i],glm::length(start - actor[i]->position) });
	}
	// アクターの並べ替え
	std::sort(actordistance.begin(), actordistance.end(),
		[](const actorDistance& a1, const actorDistance& a2) {return a1.distance < a2.distance; });

	// 平面の当たった位置を調べる
	for (int i = 0; i < actordistance.size(); ++i)
	{
		if (!actordistance[i].actor->collider)
		{
			continue;
		}
		Box box = static_cast<Box&>(*actordistance[i].actor->collider);

		// 調べるボックスのZ側の平面
		glm::vec3 frontPosition = actordistance[i].actor->position + glm::vec3(0, 0, box.min.z);
		// 調べるボックスの-Z側の平面
		glm::vec3 backPosition = actordistance[i].actor->position + glm::vec3(0, 0, box.max.z);
		// 調べるボックスのX側の平面
		glm::vec3 rightPosition = actordistance[i].actor->position + glm::vec3(box.max.x, 0, 0);
		// 調べるボックスの-X側の平面
		glm::vec3 leftPosition = actordistance[i].actor->position + glm::vec3(box.min.x, 0, 0);

		// 調べるボックスのZ側の平面との距離
		float frontLength = glm::length(start - frontPosition);
		// 調べるボックスの-Z側の平面との距離
		float backLength = glm::length(start - backPosition);
		// 調べるボックスのX側の平面との距離
		float rightLength = glm::length(start - rightPosition);
		// 調べるボックスの-X側の平面との距離
		float leftLegth = glm::length(start - leftPosition);
		// アクターの当たる範囲
		glm::vec3 max = actordistance[i].actor->position + box.max;
		glm::vec3 min = actordistance[i].actor->position + box.min;

		// 平面の情報
		struct planeDistance
		{
			float distance;
			glm::vec3 planePos;
			glm::vec3 normal;
		};
		// 平面の情報を入れる
		std::vector<planeDistance> planeDis =
		{
			{glm::length(start - frontLength),frontPosition,glm::vec3(0,0,1)},
			{glm::length(start - backLength),backPosition,glm::vec3(0,0,-1)},
			{glm::length(start - rightLength),rightPosition,glm::vec3(1,0,0)},
			{glm::length(start - leftLegth),leftPosition,glm::vec3(-1,0,0)},
		};

		// 平面の並べ替え
		std::sort(planeDis.begin(), planeDis.end(),
			[](const planeDistance& p1, const planeDistance& p2) {return p1.distance < p2.distance; });
		// レイの当たった位置
		glm::vec3 rayPos;

		for (int j = 0; j < planeDis.size(); ++j)
		{
			// 面が当たっているか調べる
			if (Intersect(start, end, planeDis[j].planePos,
				planeDis[j].normal, rayPos))
			{
				// 当たっている範囲に面があるか調べる
				if ((rayPos.x <= max.x && rayPos.y <= max.y && rayPos.z <= max.z) &&
					(rayPos.x >= min.x && rayPos.y >= min.y && rayPos.z >= min.z))
				{
					// 当たったのでisHitをtrueにする
					isHit = true;
					// ヒット情報にアクターを入れる
					hit.hitActor = actordistance[i].actor;
					// ヒット情報に距離を入れる
					hit.distance = planeDis[j].distance;
					// ヒット情報に当たった位置を入れる
					hit.hitPoint = rayPos;
				}
			}
		}
		// 当たっていたらtrueを返す
		if (isHit)
		{
			return true;
		}
	}
	// 当たっていなければfalseを返す
	return false;
}