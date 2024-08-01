/**
* @file Texture.cpp
*/
#include "Texture.h"
#include <iostream>

/**
* �R���X�g���N�^
*/
Texture::Texture(const char* filename)
{
	id = GLContext::CreateImage2D(filename);
	if (id)
	{
		name = filename;
		glGetTextureLevelParameteriv(id, 0, GL_TEXTURE_WIDTH, &width);
		glGetTextureLevelParameteriv(id, 0, GL_TEXTURE_HEIGHT, &height);
		std::cout << "[���]" << __func__ << "�e�N�X�`��" << filename << "���쐬\n";
	}
	else
	{
		std::cout << "[�G���[]" << __func__ << "�e�N�X�`��" << filename << "�̍쐬�Ɏ��s\n";
	}
}

/**
* �R���X�g���N�^
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
		std::cout << "[���]" << __func__ << "�e�N�X�`��" << name << "���쐬.\n";

	}
}

/**
* �f�X�g���N�^
*/
Texture::~Texture()
{
	if (id)
	{
		std::cout << "[���]" << __func__ << "�e�N�X�`��" << name << "���폜\n";
	}
	glDeleteTextures(1, &id);
}

/**
* �I�u�W�F�N�g���g�����Ԃ��ǂ����𒲂ׂ�
* 
* @retval true  �g����
* @retval false �g���Ȃ�(�������Ɏ��s���Ă���)
*/
bool Texture::IsValid()const
{
	return id;
}

/**
* �e�N�X�`�����O���t�B�b�N�X�p�C�v���C���Ɋ��蓖�Ă�
* 
* @param unit ���蓖�Ă�e�N�X�`���C���[�W���j�b�g�̔ԍ�
*/
void Texture::Bind(GLuint unit)const
{
	glBindTextureUnit(unit, id);
}

/**
* �e�N�X�`�����蓖�Ă���������
* 
* @param unit ���蓖�ĉ�������e�N�X�`���C���[�W���j�b�g�̔ԍ�
*/
void Texture::Unbind(GLuint unit)const
{
	glBindTextureUnit(unit, 0);
}

/**
* �e�N�X�`��ID���擾
*/
GLuint Texture::GetID()const
{
	return id;
}

/**
* ���擾
*/
GLsizei Texture::GetWidth()const
{
	return width;
}

/**
* �����擾
*/
GLsizei Texture::GetHeight()const
{
	return height;
}