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
* プリミティブデータ
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
	// プリミティブ名
	std::string name;
	// プリミティブの種類
	GLenum mode = GL_TRIANGLES;
	// 描画するインデックス数
	GLsizei count = 0;
	// 描画開始インデックスのバイトオフセット
	const GLvoid* indices = 0;
	// インデックス0番とみなされる頂点配列内の位置
	GLint baseVertex = 0;

};

/**
* 複数のプリミティブを管理するクラス
*/
class PrimitiveBuffer
{
public:
	PrimitiveBuffer(GLsizei maxVertexCount, GLsizei maxIndexCount);
	~PrimitiveBuffer();

	// プリミティブの追加
	bool Add(size_t vertexCount, const glm::vec3* pPosition,
		const glm::vec4* pColor, const glm::vec2* pTexcoord,
		const glm::vec3* pNormal, 
		size_t indexCount, const GLushort* pIndex, const char* name = nullptr,
		GLenum type = GL_TRIANGLES);
	bool AddFromObjFile(const char* filename);

	// プリミティブの取得
	const Primitive& Get(size_t n)const;
	const Primitive& Find(const char* name)const;

	// VAOバインド管理
	void BindVertexArray()const;
	void UnbindVectexArray()const;

private:
	// 描画データの配列
	std::vector<Primitive> primitives;

	// バッファID
	GLuint vboPosition = 0;
	GLuint vboColor = 0;
	GLuint vboTexcoord = 0;
	GLuint vboNormal = 0;
	GLuint ibo = 0;
	GLuint vao = 0;

	// 格納できる最大数
	GLsizei maxVertexCount = 0;
	// 格納済みの頂点数
	GLsizei curVertexCount = 0;
	// 格納できる最大インデックス数
	GLsizei maxIndexCount = 0;
	// 格納済みインデックス数
	GLsizei curIndexCount = 0;

};

bool CopyData(GLuint writeBuffer, GLsizei unitSize, GLsizei offsetCount, size_t count, const void* data);

#endif // PRIMITIVE_H_INCLUDED