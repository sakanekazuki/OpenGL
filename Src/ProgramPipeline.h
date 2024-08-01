/**
* @file ProgramPipeline.h
*/

#ifndef PROGRAMPIPELINE_H_INCLUDED
#define PROGRAMPIPELINE_H_INCLUDED

#include <glad/glad.h>
#include <glm/glm.hpp>

/**
* シェーダープログラムを管理するクラス
*/
class ProgramPipeline
{
public:
	ProgramPipeline(const char* vsCode, const char* fsCode);
	~ProgramPipeline();

	// オブジェクトの有効性を判定する
	bool IsValid()const;

	// ユニフォームの変数
	bool SetVertUniform(GLint, const glm::vec3&)const;
	bool SetVertUniform(GLint, const glm::mat4&)const;
	bool SetVertUniform(GLint, const glm::vec4&)const;
	bool SetVertUniform(GLint, const glm::vec4*,size_t)const;
	bool SetVertUniform(GLint, const glm::uint*,size_t)const;

	bool SetFragUniform(GLint, const glm::vec3&)const;
	bool SetFragUniform(GLint, const glm::mat4&)const;
	bool SetFragUniform(GLint, const glm::vec4&)const;
	bool SetFragUniform(GLint, const glm::vec4*, size_t)const;
	bool SetFragUniform(GLint, const glm::uint*, size_t)const;

	// バインド管理
	void Bind()const;
	void Unbind()const;

private:
	// プログラム・オブジェクト(頂点シェーダー)
	GLuint vp = 0;
	// プログラム・オブジェクト(フラグメントシェーダー)
	GLuint fp = 0;
	// プログラム・プイプライン・オブジェクト
	GLuint pipeline = 0;

};

#endif  // PROGRAMPIPELINE_H_INCLUDED