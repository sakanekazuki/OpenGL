/**
* @file House5.h
*/

#ifndef HOUSE5_H_INCLUDE
#define HOUSE5_H_INCLUDE

#include "../Actor.h"

class House5
	:public Actor
{
public:
	House5(const glm::vec3& position);
	~House5() = default;

};

#endif // HOUSE5_H_INCLUDE