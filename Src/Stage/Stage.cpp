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
* �R���X�g���N�^
*/
Stage::Stage(std::string filename)
	:size_x(0)
	,size_y(0)
{
	if (filename == "")
		return;
	// txt�Ń}�b�v�f�[�^��ǂݍ���
	std::fstream ifs(filename);
	if (ifs.fail())
	{
		std::cerr << "[�G���[]" << filename << "���J���܂���";
	}
	// �ŏ��̈ʒu��ۑ�
	auto g = ifs.tellg();
	// �ǂݍ��񂾕�����ۑ�
	std::string str;

	int size_x = 0, size_y = 0;
	while (!ifs.eof())
	{
		getline(ifs, str);
		
		++size_y;
	}
	size_x = str.size() / 2 + 1;
	// �n�߂ɖ߂�
	ifs.seekg(g);

	// �T�C�Y�ύX
	mapdata.resize(size_y);

	for (int y = 0; y < size_y; ++y)
	{
		getline(ifs, str);
		// x�̑������ԍ�
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
* �f�X�g���N�^
*/
Stage::~Stage()
{
	
}

/**
* �X�e�[�W�쐬
*/
void Stage::Create()
{
	// �؂ƉƂ̃p�����[�^
	for (int y = 0; y < size_y; ++y)
	{
		for (int x = 0; x < size_x; ++x)
		{
			// �`�悷�镨�̂̔ԍ����擾
			const int objectNo = mapdata[y][x];
			if (objectNo <= 0)
			{
				continue;
			}

			// �l�p�`��4x4m�Ȃ̂ŁAx��y��4�{�ɂ����ʒu�ɕ\������
			const glm::vec3 position(x * 4 - 65, 0, y * 4 - 65);

			if (objectNo == 1)
			{
				// �Ƃ̒ǉ�
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House1{ position }));
			}
			else if (objectNo == 2)
			{
				// �Ƃ̒ǉ�
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House2{ position }));
			}
			else if (objectNo == 3)
			{
				// �Ƃ̒ǉ�
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House3{ position }));
			}
			else if (objectNo == 4)
			{
				// �Ƃ̒ǉ�
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House4{ position }));
			}
			else if (objectNo == 5)
			{
				// �Ƃ̒ǉ�
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House5{ position }));
			}
			else if (objectNo == 6)
			{
				// �Ƃ̒ǉ�
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House6{ position }));
			}
			else if (objectNo == 7)
			{
				// �Ƃ̒ǉ�
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House7{ position }));
			}
			else if (objectNo == 8)
			{
				// �Ƃ̒ǉ�
				GameEngine::Get().AddActors(
					std::shared_ptr<Actor>(new House8{ position }));
			}
			else if (objectNo == 9)
			{
				// �ʍs�~�߂̒ǉ�
				std::shared_ptr<Actor> roadCloased(new RoadCloased{ position });

				Box& box = static_cast<Box&>(*roadCloased->collider);

				// �傫������
				box.min *= glm::vec3(0.2f, 0.3f, 0.2f);
				box.max *= glm::vec3(0.2f, 0.3f, 0.2f);
				roadCloased->scale *= glm::vec3(0.2f, 0.3f, 0.2f);
				// �A�N�^�[�ǉ�
				GameEngine::Get().AddActors(roadCloased);
			}
		}
	}

	// �n�ʂ̍쐬
	std::shared_ptr<Actor> ground(new Map(glm::vec3(0,0,0)));

	// �\������傫���̐ݒ�
	ground->scale *= glm::vec3(size_x, 1, size_y);

	Box& box = static_cast<Box&>(*ground->collider);

	// �R���W�����̑傫����ݒ�
	box.max.x *= size_x;
	box.max.z *= size_y;
	box.min.x *= size_x;
	box.min.z *= size_y;

	// �n�ʂ̒ǉ�
	GameEngine::Get().AddActors(ground);
}