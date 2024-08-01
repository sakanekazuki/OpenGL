/**
* @file Texture.cpp
*/
#include "Texture.h"
#include <iostream>

/**
* コンストラクタ
*/
Texture::Texture(const char* filename)
{
	id = GLContext::CreateImage2D(filename);
	if (id)
	{
		name = filename;
		glGetTextureLevelParameteriv(id, 0, GL_TEXTURE_WIDTH, &width);
		glGetTextureLevelParameteriv(id, 0, GL_TEXTURE_HEIGHT, &height);
		std::cout << "[情報]" << __func__ << "テクスチャ" << filename << "を作成\n";
	}
	else
	{
		std::cout << "[エラー]" << __func__ << "テクスチャ" << filename << "の作成に失敗\n";
	}
}

/**
* コンストラクタ
*/
Texture::Texture(const char* name, GLsizei width, GLsizei height,
	const void* data, GLenum pixelFormat, GLenum type)
{
	id = GLContext::CreateImage2D(width, height, data, pixelFormat, type);
	if (id)
	{
		this->name = name;
		this->width = width;
		this->height = height;
		std::cout << "[情報]" << __func__ << "テクスチャ" << name << "を作成.\n";

	}
}

/**
* デストラクタ
*/
Texture::~Texture()
{
	if (id)
	{
		std::cout << "[情報]" << __func__ << "テクスチャ" << name << "を削除\n";
	}
	glDeleteTextures(1, &id);
}

/**
* オブジェクトが使える状態かどうかを調べる
* 
* @retval true  使える
* @retval false 使えない(初期化に失敗している)
*/
bool Texture::IsValid()const
{
	return id;
}

/**
* テクスチャをグラフィックスパイプラインに割り当てる
* 
* @param unit 割り当てるテクスチャイメージユニットの番号
*/
void Texture::Bind(GLuint unit)const
{
	glBindTextureUnit(unit, id);
}

/**
* テクスチャ割り当てを解除する
* 
* @param unit 割り当て解除するテクスチャイメージユニットの番号
*/
void Texture::Unbind(GLuint unit)const
{
	glBindTextureUnit(unit, 0);
}

/**
* テクスチャIDを取得
*/
GLuint Texture::GetID()const
{
	return id;
}

/**
* 幅取得
*/
GLsizei Texture::GetWidth()const
{
	return width;
}

/**
* 高さ取得
*/
GLsizei Texture::GetHeight()const
{
	return height;
}