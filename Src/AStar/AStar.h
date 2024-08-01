/**
* @file AStar.h
*/

#ifndef ASTAR_H_INCLUDE
#define ASTAR_H_INCLUDE

#include <glm/vec3.hpp>
#include <vector>

/**
* �o�H�T��
*/
namespace AStar
{
	struct Node;

	/**
	* �O���t�̃G�b�W(��)��\���^
	*/
	struct Edge
	{
		// �אڐ�m�[�h
		Node* node = nullptr;
		// �G�b�W�ʉ߂ɕK�v�ȃR�X�g
		float cost = 1.0f;
	};

	/*
	* �O���t(���_)��\���^
	*/
	struct Node
	{
		// �m�[�h�̍��W
		glm::vec3 position;
		// �אڂ��Ă���m�[�h�̔z��
		std::vector<Edge> neighbors;

		// ���̃m�[�h��ʂ��ăX�^�[�g�ƃS�[�������ԍŒZ�o�H�̗\�z����
		float f=0;
		// �X�^�[�g�m�[�h���炱�̃m�[�h�܂ł̗\�z����
		float g=0;
		// �e�m�[�h
		Node* parent;

		// �m�[�h�̏��
		enum class Status
		{
			// �T�����
			open,
			// �T���ς�
			close,
		};
		// ���݂̃m�[�h�̏��
		Status status = Status::open;
	};

	// �o�H��\���^
	using Route = std::vector<const Node*>;

	/**
	* �O���t��\���^
	*/
	struct Graph
	{
		// ���[�g����
		Route SearchRoute(Node* startNode, Node* goalNode);
		void IntiFromGridMap(int width, int height, const int* gtridMap);
		Node* GetNode(const glm::vec3& pos);

		// �m�[�h�z��
		std::vector<Node> nodes;
		// �i�q��}�b�v�̕�
		int width = 0;
		// �i�q��}�b�v�̍���
		int height = 0;
	};

	//void Test();

} // namespace AStar

#endif // ASTAR_H_INCLUDE