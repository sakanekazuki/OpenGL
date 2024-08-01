/**
* @file AStar.h
*/

#ifndef ASTAR_H_INCLUDE
#define ASTAR_H_INCLUDE

#include <glm/vec3.hpp>
#include <vector>

/**
* 経路探索
*/
namespace AStar
{
	struct Node;

	/**
	* グラフのエッジ(辺)を表す型
	*/
	struct Edge
	{
		// 隣接先ノード
		Node* node = nullptr;
		// エッジ通過に必要なコスト
		float cost = 1.0f;
	};

	/*
	* グラフ(頂点)を表す型
	*/
	struct Node
	{
		// ノードの座標
		glm::vec3 position;
		// 隣接しているノードの配列
		std::vector<Edge> neighbors;

		// このノードを通ってスタートとゴールを結ぶ最短経路の予想距離
		float f=0;
		// スタートノードからこのノードまでの予想距離
		float g=0;
		// 親ノード
		Node* parent;

		// ノードの状態
		enum class Status
		{
			// 探索候補
			open,
			// 探索済み
			close,
		};
		// 現在のノードの状態
		Status status = Status::open;
	};

	// 経路を表す型
	using Route = std::vector<const Node*>;

	/**
	* グラフを表す型
	*/
	struct Graph
	{
		// ルート検索
		Route SearchRoute(Node* startNode, Node* goalNode);
		void IntiFromGridMap(int width, int height, const int* gtridMap);
		Node* GetNode(const glm::vec3& pos);

		// ノード配列
		std::vector<Node> nodes;
		// 格子状マップの幅
		int width = 0;
		// 格子状マップの高さ
		int height = 0;
	};

	//void Test();

} // namespace AStar

#endif // ASTAR_H_INCLUDE