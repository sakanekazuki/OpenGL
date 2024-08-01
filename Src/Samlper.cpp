#include "Sampler.h"
#include <iostream>

/**
* �R���X�g���N�^
*/
Sampler::Sampler(int type)
{
	sampler = GLContext::CreateSampler(type);
	if (sampler)
	{
		std::cout << "[���]" << __func__ << "�T���v�����쐬�ɐ���\n";
	}
	else
	{
		std::cout << "[���]" << __func__ << "�T���v���̍쐬�Ɏ��s\n";
	}
}

/**
* �f�X�g���N�^
*/
Sampler::~Sampler()
{
	glDeleteSamplers(1, &sampler);
}

/**
* �T���v���[���g���邩���ׂ�
* 
* @retval true  �g����
* @retval false �g���Ȃ�
*/
bool Sampler::IsValid()
{
	return sampler;
}

/**
* 
*/
GLuint Sampler::GetSampler()
{
	return sampler;
}