/**
* @file Mesh.h
*/

#ifndef MESH_H_INCLUDE
#define MESH_H_INCLUDE

#include <string>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "../Texture.h"

class Primitive;

class Mesh
{
public:
	/**
	* マテリアルデータ
	*/
	struct Material
	{
		// マテリアル名
		std::string name;
		// ディフューズ色
		glm::vec4 color = glm::vec4(1);
		// ディフューズテクスチャ
		std::shared_ptr<Texture> tex;
	};

	Mesh() = default;
	~Mesh() = default;
	Mesh(const Mesh&) = default;
	Mesh& operator=(const Mesh&) = default;
	Primitive primitive;
	// マテリアル配列
	std::vector<Material> materials;

private:

};

// メッシュクラスのポインタ型
using MeshPtr = std::shared_ptr<Mesh>;

#endif // MESH_H_INLCUDE