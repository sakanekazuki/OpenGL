/**
* @file Stage.cpp
*/

#include "Stage.h"
#include "../StaticObject/Map.h"
#include "../StaticObject/Warehouse.h"
#include "../StaticObject/Tree.h"
#include "../../GameEngine.h"

/**
* �R���X�g���N�^
*/
Stage::Stage()
	:size_x(20)
	,size_y(20)
{

}

/**
* �f�X�g���N�^
*/
Stage::~Stage()
{

}

/**
* �X�e�[�W����
*/
void Stage::Create()
{
	for (int y = 0; y < size_y; ++y)
	{
		for (int x = 0; x < size_x; ++x)
		{
			// �`�悷�镨�̂̔ԍ����擾
			const int objectNo = objectMapData[y][x];
			if (objectNo <= 0 || objectNo >= 3)
			{
				continue;
			}

			// �l�p�`��4x4m�Ȃ̂ŁAx��y��4�{�ɂ����ʒu�ɕ\������
			const glm::vec3 position(x * 4 - 20, 0, y * 4 - 20);

			if (objectNo == 1)
			{
				GameEngine::Get().AddActors(std::shared_ptr<Actor>(new Tree(position)));
			}
			else if (objectNo == 2)
			{
				GameEngine::Get().AddActors(std::shared_ptr<Actor>(new Warehouse(position)));
			}
		}
	}

	for (int y = 0; y < 20; ++y)
	{
		for (int x = 0; x < 20; ++x)
		{
			// �l�p�`��4x4m�Ȃ̂ŁAx��y��4�{�����ʒu�ɕ\������
			const glm::vec3 position(x * 4 - 20, 0, y * 4 - 20);

			// �}�b�v
			GameEngine::Get().AddActors(std::shared_ptr<Actor>(
				new Map(position)));
		}
	}
}