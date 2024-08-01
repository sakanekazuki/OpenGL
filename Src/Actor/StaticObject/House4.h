/**
* @file House4.h
*/

#ifndef HOUSE4_H_INCLUDE
#define HOUSE4_H_INCLUDE

#include "../Actor.h"

class House4
	:public Actor
{
public:
	House4(const glm::vec3& position);
	~House4() = default;

};

#endif // HOUSE4_H_INCLUDE