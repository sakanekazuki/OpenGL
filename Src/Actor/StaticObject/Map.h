/**
* @file Map.h
*/

#ifndef MAP_H_INCLUDE
#define MAP_H_INCLUDE

#include "../Actor.h"

class Map:
	public Actor
{
public:
	Map(glm::vec3 position);
	~Map() = default;

};

#endif // MAP_H_INCLUDE