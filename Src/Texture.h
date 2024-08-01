/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <glad/glad.h>
#include "GLContext.h"
#include <string>

/**
* テクスチャを管理するクラス
*/
class Texture
{
public:
	// コンストラクタ
	Texture(const char* filename);
	Texture(const char* name, GLsizei width, GLsizei height,
		const void* data, GLenum pixelFormat, GLenum type);
	// デストラクタ
	~Texture();

	// オブジェクトの有効性を判定する
	bool IsValid()const;

	// バインド管理
	void Bind(GLuint uint)const;
	void Unbind(GLuint uint)const;

	GLuint GetID()const;

	GLsizei GetWidth()const;
	GLsizei GetHeight()const;

private:
	// 画像ファイル名
	std::string name;
	// オブジェクトID
	GLuint id = 0;
	// 幅
	GLsizei width = 0;
	// 高さ
	GLsizei height = 0;

};

#endif // TEXTURE_H_INCLUDED