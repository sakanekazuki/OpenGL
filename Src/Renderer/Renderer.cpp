/**
* @file Renderer.cpp
*/

#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"
#include "../ProgramPipeline.h"
#include "../Texture.h"
#include "../Actor/Actor.h"

/**
* クローンを作成
*/
RendererPtr PrimitiveRenderer::Clone()const
{
	return std::make_shared<PrimitiveRenderer>(*this);
}

/**
* プリミティブを描画
*/
void PrimitiveRenderer::Draw(const Actor& actor,
	const ProgramPipeline& pipeline, const glm::mat4& matVP)
{
	// モデル行列を計算する
	glm::mat4 matT = glm::translate(glm::mat4(1), actor.position);
	glm::mat4 matR = glm::rotate(glm::mat4(1), actor.rotation, glm::vec3(0, 1, 0));
	glm::mat4 matS = glm::scale(glm::mat4(1), actor.scale);
	glm::mat4 matA = glm::translate(glm::mat4(1), actor.adjustment);
	glm::mat4 matModel = matT * matR * matS * matA;

	// MVP行列を計算する
	glm::mat4 matMVP = matVP * matModel;

	// モデル行列とMVO行列をGPUメモリにコピーする
	const GLint locMatTRS = 0;
	const GLint locMatModel = 1;
	pipeline.SetVertUniform(locMatTRS, matMVP);
	if (actor.layer == Layer::Default)
	{
		pipeline.SetVertUniform(locMatModel, matModel);
	}

	// マテリアルデータを設定
	const glm::uint tecture = 0;


	// テクスチャを割り当てる
	actor.tex->Bind(0);
	// プリミティブを描画する
	actor.prim.Draw();
}