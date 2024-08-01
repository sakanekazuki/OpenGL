/**
* @file House6.h
*/

#ifndef HOUSE6_H_INCLUDE
#define HOUSE6_H_INCLUDE

#include "../Actor.h"

class House6
	:public Actor
{
public:
	House6(const glm::vec3& position);
	~House6() = default;

};

#endif // HOUSE6_H_INCLUDE