/**
* @file Stage.cpp
*/

#include "Stage.h"
#include "../StaticObject/Map.h"
#include "../StaticObject/Warehouse.h"
#include "../StaticObject/Tree.h"
#include "../../GameEngine.h"

/**
* コンストラクタ
*/
Stage::Stage()
	:size_x(20)
	,size_y(20)
{

}

/**
* デストラクタ
*/
Stage::~Stage()
{

}

/**
* ステージ生成
*/
void Stage::Create()
{
	for (int y = 0; y < size_y; ++y)
	{
		for (int x = 0; x < size_x; ++x)
		{
			// 描画する物体の番号を取得
			const int objectNo = objectMapData[y][x];
			if (objectNo <= 0 || objectNo >= 3)
			{
				continue;
			}

			// 四角形が4x4mなので、xとyを4倍にした位置に表示する
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
			// 四角形が4x4mなので、xとyを4倍した位置に表示する
			const glm::vec3 position(x * 4 - 20, 0, y * 4 - 20);

			// マップ
			GameEngine::Get().AddActors(std::shared_ptr<Actor>(
				new Map(position)));
		}
	}
}