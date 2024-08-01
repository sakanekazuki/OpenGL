/**
* @file Intersect.h
*/

#ifndef INTERSECT_H_INCLUDE
#define INTERSECT_H_INCLUDE

#include <glm/glm.hpp> 
#include "Actor/Actor.h"

/**
* レイが当たった時の情報を入れる
*/
struct Hit
{
	// ヒットしたアクター
	std::shared_ptr<Actor> hitActor = nullptr;
	// 当たった位置
	glm::vec3 hitPoint = glm::vec3(0);
	// 当たった距離
	float distance = 0;

};

bool Intersect(const glm::vec3& start, const glm::vec3& end,
	const glm::vec3& q, const glm::vec3& normal, glm::vec3& p);

bool RayCast(glm::vec3 start, glm::vec3 end, Hit& hit);


#endif // INTERSECT_H_INCLUDE