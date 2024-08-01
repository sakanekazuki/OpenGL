/**
* @file Collision.h
*/

#ifndef COLLISION_H_INCLUDE
#define COLLISION_H_INCLUDE

#include <glm/glm.hpp>
#include <memory>

// 先行宣言
class Actor;
struct Contact;

/**
* 図形の種類
*/
enum class ShapeTye
{
	// 直方体
	box,
	// 円柱
	cylinder,
};

/**
* コライダーの基底クラス
*/
class Collider
{
public:
	Collider(ShapeTye type);
	virtual ~Collider() = default;

	virtual std::shared_ptr<Collider> Clone()const = 0;
	ShapeTye GetShapeType()const;

private:
	ShapeTye shapeType;

};

/**
* 直方体
*/
class Box 
	: public Collider
{
public:
	Box();
	Box(const glm::vec3& min, const glm::vec3& max);
	virtual ~Box() = default;
	
	static std::shared_ptr<Box> Create(
		const glm::vec3& min, const glm::vec3& max);
	
	virtual std::shared_ptr<Collider> Clone()const override;

	glm::vec3 min = glm::vec3(0);
	glm::vec3 max = glm::vec3(0);

	bool isTrigger = false;
};

/**
* 垂直円柱
*/
class Cylinder
	: public Collider
{
public:
	Cylinder();
	Cylinder(const glm::vec3& bottom, float radius, float height);
	~Cylinder() = default;

	static std::shared_ptr<Cylinder> Create(const glm::vec3& bottom, float radius, float height);

	virtual std::shared_ptr<Collider> Clone()const override;

	// 下端の座標
	glm::vec3 bottom = glm::vec3(0);
	// 半径
	float radius = 1.0f;
	// 高さ
	float height = 1.0f;

};

bool CollisionBoxBox(Actor& a, Actor& b, Contact& contact);
bool CollisionBoxCylinder(Actor& a, Actor& b, Contact& contact);
bool CollisionCylinderCylinder(Actor & a, Actor & b, Contact & contact);
bool CollisionCylinderBox(Actor & a, Actor & b, Contact & contact);

#endif // !COLLISION_H_INCLUDE
