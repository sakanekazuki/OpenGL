/**
* @file ProgramPipeline.cpp
*/

#include "ProgramPipeline.h"
#include "GLContext.h"
#include <iostream>

/**
* �R���X�g���N�^
* 
* @param vsCode ���_�V�F�[�_�[�t�@�C����
* @param fsCode �t���O�����g�V�F�[�_�[�t�@�C����
*/
ProgramPipeline::ProgramPipeline(const char* vsCode,const char* fsCode)
{
	vp = GLContext::CreateProgramFromFile(GL_VERTEX_SHADER, vsCode);
	fp = GLContext::CreateProgramFromFile(GL_FRAGMENT_SHADER, fsCode);
	pipeline = GLContext::CreatePipeline(vp, fp);
	if (pipeline)
	{
		std::cout << "[���]�v���O�����p�C�v���C�����쐬(id="
			<< pipeline << ",vp=" << vsCode << ",fp=" << fsCode << ")\n";
	}
}

/**
* �f�X�g���N�^
*/
ProgramPipeline::~ProgramPipeline()
{
	if (pipeline)
	{
		std::cout << "[���]�v���O�����p�C�v���C��������(id="
			<< pipeline << ")\n";
	}
	glDeleteProgramPipelines(1, &pipeline);
	glDeleteProgram(fp);
	glDeleteProgram(vp);
}

/**
* �I�u�W�F�N�g���g�����Ԃ��ǂ������ׂ�
* 
* @retval true  �g����
* @retval false �g���Ȃ�
*/
bool ProgramPipeline::IsValid()const
{
	return pipeline;
}



/**
* ���j�t�H�[���ϐ��Ƀf�[�^���R�s�[����
* 
* @param location ���j�t�H�[���ϐ��̈ʒu
* @param data     ���j�t�H�[���ϐ��ɃR�s�[����f�[�^
* 
* @retval true  �R�s�[����
* @retval false �R�s�[���s
*/
bool ProgramPipeline::SetVertUniform(GLint location, const glm::vec3& data)const
{
	glGetError(); // �G���[��Ԃ����Z�b�g

	glProgramUniform3f(vp, location, data.x, data.y, data.z);

	if (glGetError() != GL_NO_ERROR)
	{
		std::cerr << "[�G���[]" << __func__ << ":" << location << "�̃��j�t�H�[���ϐ��̐ݒ�Ɏ��s\n";
		return false;
	}
	return true;
}

bool ProgramPipeline::SetVertUniform(GLint location, const glm::mat4& data)const
{
	glGetError(); // �G���[��Ԃ����Z�b�g

	glProgramUniformMatrix4fv(vp, location, 1, GL_FALSE, &data[0][0]);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cerr << "[�G���[]" << __func__ << ":" << location << "�̃��j�t�H�[���ϐ��̐ݒ�Ɏ��s\n";
		return false;
	}
	return true;
}

bool ProgramPipeline::SetVertUniform(GLint location, const glm::vec4& data)const
{
	// �G���[��Ԃ̃��Z�b�g
	glGetError();

	glProgramUniform4fv(vp, location, 1, &data.x);

	if (glGetError() != GL_NO_ERROR)
	{
		std::cerr << "[�G���[]" << __func__ << ":���j�t�H�[���ϐ��̐ݒ�Ɏ��s\n";
		return false;
	}
	return true;
}

bool ProgramPipeline::SetVertUniform(GLint location, const glm::vec4* data, size_t size)const
{
	glGetError(); 

	glProgramUniform4fv(vp, location, static_cast<GLsizei>(size), &data->x);
	
	if (glGetError() != GL_NO_ERROR) {
		std::cerr << "[�G���[]" << __func__ << ":���j�t�H�[���ϐ��̐ݒ�Ɏ��s.\n";
		return false;
	}
	return true;
}

bool ProgramPipeline::SetVertUniform(GLint location, const glm::uint* data, size_t size)const
{
	glGetError();

	glProgramUniform1uiv(vp, location, static_cast<GLsizei>(size), data);
	if (glGetError() != GL_NO_ERROR) {
		std::cerr << "[�G���[]" << __func__ << ":���j�t�H�[���ϐ��̐ݒ�Ɏ��s.\n";
		return false;
	}
	return true;
}

bool ProgramPipeline::SetFragUniform(GLint location, const glm::vec3& data)const
{
	glGetError();
	
	glProgramUniform3f(fp, location, data.x, data.y, data.z);

	if (glGetError() != GL_NO_ERROR)
	{
		std::cerr << "[�G���[]" << __func__ << ":" << location << "�̃��j�t�H�[���ϐ��̐ݒ�Ɏ��s\n";
		return false;
	}
	return true;
}

bool ProgramPipeline::SetFragUniform(GLint location, const glm::mat4& data)const
{
	glGetError(); // �G���[��Ԃ����Z�b�g

	glProgramUniformMatrix4fv(fp, location, 1, GL_FALSE, &data[0][0]);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cerr << "[�G���[]" << __func__ << ":" << location << "�̃��j�t�H�[���ϐ��̐ݒ�Ɏ��s\n";
		return false;
	}
	return true;
}

bool ProgramPipeline::SetFragUniform(GLint location, const glm::vec4& data)const
{
	// �G���[��Ԃ̃��Z�b�g
	glGetError();

	glProgramUniform4fv(fp, location, 1, &data.x);

	if (glGetError() != GL_NO_ERROR)
	{
		std::cerr << "[�G���[]" << __func__ << ":���j�t�H�[���ϐ��̐ݒ�Ɏ��s\n";
		return false;
	}
	return true;
}

bool ProgramPipeline::SetFragUniform(GLint location, const glm::vec4* data, size_t size)const
{
	glGetError();

	glProgramUniform4fv(fp, location, static_cast<GLsizei>(size), &data->x);

	if (glGetError() != GL_NO_ERROR) {
		std::cerr << "[�G���[]" << __func__ << ":���j�t�H�[���ϐ��̐ݒ�Ɏ��s.\n";
		return false;
	}
	return true;
}

bool ProgramPipeline::SetFragUniform(GLint location, const glm::uint* data, size_t size)const
{
	glGetError();

	glProgramUniform1uiv(fp, location, static_cast<GLsizei>(size), data);
	if (glGetError() != GL_NO_ERROR) {
		std::cerr << "[�G���[]" << __func__ << ":���j�t�H�[���ϐ��̐ݒ�Ɏ��s.\n";
		return false;
	}
	return true;
}

/**
* �v���O�����p�C�v���C�����o�C���h����
*/
void ProgramPipeline::Bind()const
{
	glBindProgramPipeline(pipeline);
}

/**
* �v���O�����p�C�v���C���̃o�C���h����������
*/
void ProgramPipeline::Unbind()const
{
	glBindProgramPipeline(0);
}