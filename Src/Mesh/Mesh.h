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
	* �}�e���A���f�[�^
	*/
	struct Material
	{
		// �}�e���A����
		std::string name;
		// �f�B�t���[�Y�F
		glm::vec4 color = glm::vec4(1);
		// �f�B�t���[�Y�e�N�X�`��
		std::shared_ptr<Texture> tex;
	};

	Mesh() = default;
	~Mesh() = default;
	Mesh(const Mesh&) = default;
	Mesh& operator=(const Mesh&) = default;
	Primitive primitive;
	// �}�e���A���z��
	std::vector<Material> materials;

private:

};

// ���b�V���N���X�̃|�C���^�^
using MeshPtr = std::shared_ptr<Mesh>;

#endif // MESH_H_INLCUDE