/**
* @file Sampler.h
*/
#ifndef SAMPLER_H_INCLUDE
#define SAMPLER_H_INCLUDE

#include "GLContext.h"

class Sampler
{
private:
	GLuint sampler;

public:
	// コンストラクタ
	Sampler(int type);
	// デストラクタ
	~Sampler();

	bool IsValid();

	GLuint GetSampler();

};

#endif // SAMPLER_H_INCLUDE