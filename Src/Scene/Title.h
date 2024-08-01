/**
* @file Title
*/

#ifndef TITLE_H_INCLUDE
#define TITLE_H_INCLUDE

#include "Scene.h"
#define TITLESCENE "Title"

class Title :
	public Scene
{
public:
	Title();
	~Title() = default;

	void Update(float deltaTime)override;

private:

};

#endif // TITLE_H_INCLUDE