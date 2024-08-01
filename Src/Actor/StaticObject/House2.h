/**
* @file House2.h
*/

#ifndef HOUSE2_H_INCLUDE
#define HOUSE2_H_INCLUDE

#include "../Actor.h"

class House2
	:public Actor
{
public:
	House2(const glm::vec3& position);
	~House2() = default;

};

#endif // HOUSE2_H_INCLUDE