/**
* @file Primitive.h
**/

#ifndef PRIMITIVE_H_INCLUDED
#define PRIMITIVE_H_INCLUDED

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include "Texture.h"

/**
* �v���~�e�B�u�f�[�^
*/
class Primitive
{
public:
	Primitive() = default;
	Primitive(const char* name,GLenum m,GLsizei c,size_t o,GLint b):
		name(name),mode(m),count(c),
		indices(reinterpret_cast<GLvoid*>(o)),baseVertex(b)
	{}
	~Primitive() = default;

	void Draw() const;

	const std::string& GetName()const
	{
		return name;
	}

private:
	// �v���~�e�B�u��
	std::string name;
	// �v���~�e�B�u�̎��
	GLenum mode = GL_TRIANGLES;
	// �`�悷��C���f�b�N�X��
	GLsizei count = 0;
	// �`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g
	const GLvoid* indices = 0;
	// �C���f�b�N�X0�ԂƂ݂Ȃ���钸�_�z����̈ʒu
	GLint baseVertex = 0;

};

/**
* �����̃v���~�e�B�u���Ǘ�����N���X
*/
class PrimitiveBuffer
{
public:
	PrimitiveBuffer(GLsizei maxVertexCount, GLsizei maxIndexCount);
	~PrimitiveBuffer();

	// �v���~�e�B�u�̒ǉ�
	bool Add(size_t vertexCount, const glm::vec3* pPosition,
		const glm::vec4* pColor, const glm::vec2* pTexcoord,
		const glm::vec3* pNormal, 
		size_t indexCount, const GLushort* pIndex, const char* name = nullptr,
		GLenum type = GL_TRIANGLES);
	bool AddFromObjFile(const char* filename);

	// �v���~�e�B�u�̎擾
	const Primitive& Get(size_t n)const;
	const Primitive& Find(const char* name)const;

	// VAO�o�C���h�Ǘ�
	void BindVertexArray()const;
	void UnbindVectexArray()const;

private:
	// �`��f�[�^�̔z��
	std::vector<Primitive> primitives;

	// �o�b�t�@ID
	GLuint vboPosition = 0;
	GLuint vboColor = 0;
	GLuint vboTexcoord = 0;
	GLuint vboNormal = 0;
	GLuint ibo = 0;
	GLuint vao = 0;

	// �i�[�ł���ő吔
	GLsizei maxVertexCount = 0;
	// �i�[�ς݂̒��_��
	GLsizei curVertexCount = 0;
	// �i�[�ł���ő�C���f�b�N�X��
	GLsizei maxIndexCount = 0;
	// �i�[�ς݃C���f�b�N�X��
	GLsizei curIndexCount = 0;

};

bool CopyData(GLuint writeBuffer, GLsizei unitSize, GLsizei offsetCount, size_t count, const void* data);

#endif // PRIMITIVE_H_INCLUDED