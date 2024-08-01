#include "Sampler.h"
#include <iostream>

/**
* コンストラクタ
*/
Sampler::Sampler(int type)
{
	sampler = GLContext::CreateSampler(type);
	if (sampler)
	{
		std::cout << "[情報]" << __func__ << "サンプラを作成に成功\n";
	}
	else
	{
		std::cout << "[情報]" << __func__ << "サンプラの作成に失敗\n";
	}
}

/**
* デストラクタ
*/
Sampler::~Sampler()
{
	glDeleteSamplers(1, &sampler);
}

/**
* サンプラーが使えるか調べる
* 
* @retval true  使える
* @retval false 使えない
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