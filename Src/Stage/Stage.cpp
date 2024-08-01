/**
* @file Stage.cpp
*/

#include "Stage.h"
#include "../Actor/StaticObject/House1.h"
#include "../Actor/StaticObject/House2.h"
#include "../Actor/StaticObject/House3.h"
#include "../Actor/StaticObject/House4.h"
#include "../Actor/StaticObject/House5.h"
#include "../Actor/StaticObject/House6.h"
#include "../Actor/StaticObject/House7.h"
#include "../Actor/StaticObject/House8.h"
#include "../Actor/StaticObject/Map.h"
#include "../Actor/StaticObject/RoadCloased.h"
#include "../GameEngine.h"
#include <fstream>
#include <iostream>

/**
* コンストラクタ
*/
Stage::Stage(std::string filename)
	:size_x(0)
	,size_y(0)
{
	if (filename == "")
		return;
	// txtでマップデータを読み込む
	std::fstream ifs(filename);
	if (ifs.fail())
	{
		std::cerr << "[エラー]" << filename << "を開けません";
	}
	// 最初の位置を保存
	auto g = ifs.tellg();
	// 読み込んだ文字を保存
	std::string str;

	int size_x = 0, size_y = 0;
	while (!ifs.eof())
	{
		getline(ifs, str);
		
		++size_y;
	}
	size_x = str.size() / 2 + 1;
	// 始めに戻す
	ifs.seekg(g);

	// サイズ変更
	mapdata.resize(size_y);

	for (int y = 0; y < size_y; ++y)
	{
		getline(ifs, str);
		// xの代入する番号
		int focus = 0;
		for (int x = 0; x < str.size(); ++x)
		{
			const char* pchar = str.c_str();
			const char mapNo = pchar[x];
			if (mapNo == ' ' || mapNo == ',')
			{
				continue;
			}
			
			mapdata[y].push_back(atoi(&mapNo));
		}
	}
	this->size_x = size_x - 1;
	this->size_y = size_y;
}

/**
* デストラクタ
*/
Stage::~Stage()
{
	
}

/**
* ステージ作成
*/
void Stage::Create()
{
	// 木と家のパラメータ
	for (int y = 0; y < size_y; ++y)
	{
		for (int x = 0; x < size_x; ++x)
		{
			// 描画する物体の番号を取得
			const int objectNo = mapdata[y][x];
			if (objectNo <= 0)
			{
				continue;
			}

			// 四角形が4x4mなので、xとyを4倍にした位置に表示する
			const glm::vec3 position(x * 4 - 65, 0, y * 4 - 65);

			if (objectNo == 1)
			{
				// 家の追加
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House1{ position }));
			}
			else if (objectNo == 2)
			{
				// 家の追加
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House2{ position }));
			}
			else if (objectNo == 3)
			{
				// 家の追加
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House3{ position }));
			}
			else if (objectNo == 4)
			{
				// 家の追加
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House4{ position }));
			}
			else if (objectNo == 5)
			{
				// 家の追加
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House5{ position }));
			}
			else if (objectNo == 6)
			{
				// 家の追加
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House6{ position }));
			}
			else if (objectNo == 7)
			{
				// 家の追加
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House7{ position }));
			}
			else if (objectNo == 8)
			{
				// 家の追加
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House8{ position }));
			}
			else if (objectNo == 9)
			{
				// 通行止めの追加
				std::shared_ptr<Actor> roadCloased(new RoadCloased{ position });

				Box& box = static_cast<Box&>(*roadCloased->collider);

				// 大きさ調節
				box.min *= glm::vec3(0.2f, 0.3f, 0.2f);
				box.max *= glm::vec3(0.2f, 0.3f, 0.2f);
				roadCloased->scale *= glm::vec3(0.2f, 0.3f, 0.2f);
				// アクター追加
				GameEngine::Get().AddActors(roadCloased);
			}
		}
	}

	// 地面の作成
	std::shared_ptr<Actor> ground(new Map(glm::vec3(0,0,0)));

	// 表示する大きさの設定
	ground->scale *= glm::vec3(size_x, 1, size_y);

	Box& box = static_cast<Box&>(*ground->collider);

	// コリジョンの大きさを設定
	box.max.x *= size_x;
	box.max.z *= size_y;
	box.min.x *= size_x;
	box.min.z *= size_y;

	// 地面の追加
	GameEngine::Get().AddActors(ground);
}