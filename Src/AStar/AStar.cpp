/**
* @file AStar.cpp
*/

#include "AStar.h"
#include <queue>
#include <glm/glm.hpp>
#include <iostream>

namespace AStar
{
	/**
	* 2�_�Ԃ̃}���n�b�^���������v�Z
	*/
	float ManhattanDistance(const Node& a, const Node& b)
	{
		const glm::vec3 v = glm::abs(b.position - a.position);
		return v.x + v.y + v.z;
	}

	/**
	* Node�|�C���^�̗D�揇�ʂ̔�����s���N���X
	*/
	struct CompareNodePointer
	{
		bool operator()(const Node* lhs, const Node* rhs)const
		{
			if (lhs->f == rhs->f)
			{
				return lhs->g > rhs->g;
			}
			return lhs->f > rhs->f;
		}
	};

	/**
	* �ŒZ������T��
	* 
	* @param graph     �T���Ώۂ̃m�[�h�O���t
	* @param startNode �T���J�n�m�[�h
	* @param goalNode  �T���ڕW�m�[�h
	* 
	* @return startNode����goalNode�ւ̍ŒZ������Ԃ�
	*/
	Route Graph::SearchRoute(Node* startNode, Node* goalNode)
	{
		// �m�[�h�̏�Ԃ�������
		for (auto& e : nodes)
		{
			e.f = FLT_MAX;
			e.g = FLT_MAX;
			e.parent = nullptr;
			e.status = Node::Status::open;
		}
		startNode->f = 0;
		startNode->g = 0;

		// �I�[�v�����X�g��������
		std::priority_queue<Node*, std::vector<Node*>, CompareNodePointer> openList;// �����Fstd::priority_queue���בւ����s���N���X
		// �I�[�v�����X�g�ɒǉ�
		openList.push(startNode);

		// �I�[�v�����X�g�Ɋ܂܂��m�[�h���Af�����������Ƀ`�F�b�N
		while (!openList.empty())
		{
			// �m�[�h�̎擾
			Node* n = openList.top();

			// �S�[���ɓ��B������T���I��
			if (n == goalNode)
				break;

			// ����
			openList.pop();

			// �u�T���ς݁v�m�[�h�Ȃ牽�����Ȃ�
			if (n->status == Node::Status::close)
				continue;

			// �m�[�h���u�T���ς݁v�ɂ���
			n->status = Node::Status::close;

			// �אڃm�[�h���I�[�v�����X�g�ɒǉ�
			for (const Edge& e : n->neighbors)
			{
				// �G�b�W�̃m�[�h���擾
				Node* m = e.node;
				// �����̒ǉ�
				const float g = n->g + e.cost;
				// ���݂̌o�H�̕��������אڃm�[�hm�ɂ��ꍇ�Am�̏����X�V���ăI�[�v�����X�g�ɒǉ�
				if (g < m->g)
				{
					// ����
					m->g = g;
					// �\�z�����ɋ���+�}���n�b�^����������
					m->f = g + ManhattanDistance(*m, *goalNode);
					// �m�[�h��e�ɂ���
					m->parent = n;
					// �X�e�[�^�X��open�ɂ���
					m->status = Node::Status::open;
					// m��ǉ�
					openList.push(m);
				}
			}
		}

		// �I�[�v�����X�g����̏ꍇ�͓��B�\�Ȍo�H�����݂��Ȃ�
		if (openList.empty())
			// ��̌o�H��Ԃ�
			return Route();

		// �e�m�[�h�����ǂ��ăS�[������X�^�[�g�ւ̍ŒZ�o�H���쐬
		Route route;
		Node* p = goalNode;
		do
		{
			route.push_back(p);
			p = p->parent;
		} while (p && p != startNode);
		
		route.push_back(startNode);

		return route;
	}

	/**
	* 2D�̊i�q��}�b�v����O���t���쐬
	* 
	* @param width   �}�b�v�̕�
	* @param height  �}�b�v�̍���
	* @param gridMap �}�b�v�f�[�^�̃A�h���X
	*/
	void Graph::IntiFromGridMap(int width, int height, const int* gridMap)
	{
		// ���̐ݒ�
		this->width = width;
		// �����̐ݒ�
		this->height = height;

		// �m�[�h�z���������
		nodes.clear();
		nodes.resize(width * height);

		// �אڃm�[�h�̈ʒu��\���z��
		const glm::ivec2 offset[] = { {1, 0}, {0,-1},{-1, 0}, {0, 1} };

		// �i�q��}�b�v���m�[�h�ɕϊ�
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				Node& node = nodes[x + y * width];

				// ���W��ݒ�
				node.position = glm::vec3(x, 0, y);

				// �אڃm�[�h�̃G�b�W��ݒ�
				for (auto& e : offset)
				{
					// ���W���}�b�v�͈͊O�̏ꍇ�͈ړ��s�Ƃ���
					const glm::ivec2 pos(x + e.x, y + e.y);
					if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height)
						continue;

					// ���W���}�b�v�͈͊O�̏ꍇ�͈ړ��s�Ƃ���(�ǂȂ�)
					const int i = pos.x + pos.y * width;
					if (gridMap[i] != 0)
						continue;

					// �ړ��\�ȃ}�X�Ȃ̂ŃG�b�W���쐬
					node.neighbors.push_back(Edge{ &nodes[i],1.0f });
				}

			} // for x
		} // for y
	}

	/**
	* ���W�ɑΉ�����m�[�h���擾
	*/
	Node* Graph::GetNode(const glm::vec3& pos)
	{
		if (pos.x < 0 || pos.x >= width || pos.z < 0 || pos.z >= height)
			return nullptr;
		return &nodes[static_cast<size_t>(pos.x + pos.z * width)];
	}

	/**
	* A*�A���S���Y���̓���e�X�g
	*/
	//void Test()
	//{
	//	// �}�b�v�f�[�^
	//	const int mapSizeX = 5;
	//	const int mapSizeY = 5;
	//	const int objectMapData[mapSizeY][mapSizeX] =
	//	{
	//		{0,0,0,0,0},
	//		{0,0,1,1,0},
	//		{0,0,0,1,0},
	//		{0,0,0,1,0},
	//		{0,0,0,0,0},
	//	};
	//	// �X�^�[�g�n�_
	//	const glm::vec3 start(2, 0, 2);
	//	// �S�[���n�_
	//	const glm::vec3 goal(4, 0, 0);
	//
	//	// �}�b�v�f�[�^����O���t���쐬
	//	Graph graph;
	//	graph.IntiFromGridMap(mapSizeX, mapSizeY, objectMapData[0]);
	//
	//	// �X�^�[�g����S�[���܂ł̍ŒZ������T��
	//	Node* startNode = graph.GetNode(start);
	//	Node* goalNode = graph.GetNode(goal);
	//	Route route = graph.SearchRoute(startNode, goalNode);
	//	if (route.empty())
	//	{
	//		std::cout << "[���]" << __func__ << ": �o�H��������܂���\n";
	//		return;
	//	}
	//
	//	// �ŒZ�������o��
	//	std::cout << "[���]" << __func__ << ": �ŒZ�o�H��������܂���\n";
	//	for (int y = 0; y < mapSizeY; ++y)
	//	{
	//		for (int x = 0; x < mapSizeX; ++x)
	//		{
	//			if (start.x == x && start.z == y)
	//			{
	//				std::cout << "S ";
	//				continue;
	//			}
	//			else if (goal.x == x && goal.z == y)
	//			{
	//				std::cout << "G ";
	//				continue;
	//			}
	//			auto i = std::find_if(route.begin(), route.end(),
	//				[x, y](const Node* n) {return n->position.x == x && n->position.z == y; });
	//			if (i == route.end())
	//			{
	//				std::cout << objectMapData[y][x] << " ";
	//			}
	//			else
	//			{
	//				std::cout << "* ";
	//			}
	//		}
	//		std::cout << "\n";
	//	}
	//}

} // namespace AStar