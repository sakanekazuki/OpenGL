/**
* @file Renderer.cpp
*/

#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"
#include "../ProgramPipeline.h"
#include "../Texture.h"
#include "../Actor/Actor.h"

/**
* �N���[�����쐬
*/
RendererPtr PrimitiveRenderer::Clone()const
{
	return std::make_shared<PrimitiveRenderer>(*this);
}

/**
* �v���~�e�B�u��`��
*/
void PrimitiveRenderer::Draw(const Actor& actor,
	const ProgramPipeline& pipeline, const glm::mat4& matVP)
{
	// ���f���s����v�Z����
	glm::mat4 matT = glm::translate(glm::mat4(1), actor.position);
	glm::mat4 matR = glm::rotate(glm::mat4(1), actor.rotation, glm::vec3(0, 1, 0));
	glm::mat4 matS = glm::scale(glm::mat4(1), actor.scale);
	glm::mat4 matA = glm::translate(glm::mat4(1), actor.adjustment);
	glm::mat4 matModel = matT * matR * matS * matA;

	// MVP�s����v�Z����
	glm::mat4 matMVP = matVP * matModel;

	// ���f���s���MVO�s���GPU�������ɃR�s�[����
	const GLint locMatTRS = 0;
	const GLint locMatModel = 1;
	pipeline.SetVertUniform(locMatTRS, matMVP);
	if (actor.layer == Layer::Default)
	{
		pipeline.SetVertUniform(locMatModel, matModel);
	}

	// �}�e���A���f�[�^��ݒ�
	const glm::uint tecture = 0;


	// �e�N�X�`�������蓖�Ă�
	actor.tex->Bind(0);
	// �v���~�e�B�u��`�悷��
	actor.prim.Draw();
}