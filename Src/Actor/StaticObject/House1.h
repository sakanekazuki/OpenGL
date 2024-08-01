/**
* @file House1.h
*/

#ifndef HOUSE1_H_INCLUDE
#define HOUSE1_H_INCLUDE

#include "../Actor.h"

class House1
	:public Actor
{
public:
	House1(const glm::vec3& position);
	~House1() = default;

};

#endif // HOUSE1_H_INCLUDE