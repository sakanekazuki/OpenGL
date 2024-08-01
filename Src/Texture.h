/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <glad/glad.h>
#include "GLContext.h"
#include <string>

/**
* �e�N�X�`�����Ǘ�����N���X
*/
class Texture
{
public:
	// �R���X�g���N�^
	Texture(const char* filename);
	Texture(const char* name, GLsizei width, GLsizei height,
		const void* data, GLenum pixelFormat, GLenum type);
	// �f�X�g���N�^
	~Texture();

	// �I�u�W�F�N�g�̗L�����𔻒肷��
	bool IsValid()const;

	// �o�C���h�Ǘ�
	void Bind(GLuint uint)const;
	void Unbind(GLuint uint)const;

	GLuint GetID()const;

	GLsizei GetWidth()const;
	GLsizei GetHeight()const;

private:
	// �摜�t�@�C����
	std::string name;
	// �I�u�W�F�N�gID
	GLuint id = 0;
	// ��
	GLsizei width = 0;
	// ����
	GLsizei height = 0;

};

#endif // TEXTURE_H_INCLUDED