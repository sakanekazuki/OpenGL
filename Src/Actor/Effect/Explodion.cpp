/**
* @file Explosion.cpp
*/

#include "Explosion.h"
#include "../../GameEngine.h"

/**
* �R���X�g���N�^
*/
Explosion::Explosion(const glm::vec3& pos, float speed)
	:Sprite(pos, GameEngine::Get().LoadTexture("Res/Effects/Explosion.tga"))
	, speed(speed)
{
	color = glm::vec4(3, 3, 3, 1);
}

/**
* �X�V
*/
void Explosion::OnUpdate(float deltaTime)
{
	scale += speed;

	if (lifespan <= 0.01f)
	{
		color = glm::vec4(1, 1, 1, color.a);
	}
	color.a = lifespan * 10;
}