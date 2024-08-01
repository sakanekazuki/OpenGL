/**
* @file House3.h
*/

#ifndef HOUSE3_H_INCLUDE
#define HOUSE3_H_INCLUDE

#include "../Actor.h"

class House3
	:public Actor
{
public:
	House3(const glm::vec3& position);
	~House3() = default;

};

#endif // HOUSE3_H_INCLUDE