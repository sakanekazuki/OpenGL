/**
* @file Renderer.h
*/

#ifndef RENDERER_H_INCLUDE
#define RENDERER_H_INCLUDE

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "glad/glad.h"
#include "../Primitive.h"

// 先行宣言
class ProgramPipeline;
class Actor;
class Texture;
class Renderer;
class PrimitiveRenderer;
class MeshRenderer;

using RendererPtr = std::shared_ptr<Renderer>;
using PrimitiveRendererPtr = std::shared_ptr<PrimitiveRenderer>;
using MeshRendererPtr = std::shared_ptr<MeshRenderer>;

/**
* 描画機能の基本クラス
*/
class Renderer
{
public:
	static const GLint locMatTRS = 0;
	static const GLint locMatModel = 1;
	static const GLint locMaterialColor = 10;
	static const GLint locMaterialTexture = 20;

	Renderer() = default;
	virtual ~Renderer() = default;
	virtual RendererPtr Clone()const = 0;
	virtual void Draw(const Actor& actor,
		const ProgramPipeline& pipeline, const glm::mat4& matVP) = 0;

};

/**
* プリミティブ描画クラス
*/
class PrimitiveRenderer :
	public Renderer
{
public:
	PrimitiveRenderer() = default;
	virtual ~PrimitiveRenderer() = default;
	virtual RendererPtr Clone()const override;
	virtual void Draw(const Actor& actor,
		const ProgramPipeline& pieplien,
		const glm::mat4& matVP)override;

	void SetPrimitive(const Primitive& p);
	const Primitive& GetPrimitive()const;
	
	void SetTexture(const std::shared_ptr<Texture>& t);
	std::shared_ptr<Texture> GetTexture()const;

private:
	Primitive prim;
	std::shared_ptr<Texture> tex;

};

/**
* メッシュ描画クラス
*/
class MeshRenderer :
	public Renderer
{
public:
	MeshRenderer() = default;
	virtual ~MeshRenderer() = default;
	virtual RendererPtr Clone() const override;
	virtual void Draw(const Actor& actor,
		const ProgramPipeline& pieplien,
		const glm::mat4& matVP)override;

	void SetMesh(const MeshPtr& p);
	const MeshPtr& GetMesh()const;

	void SetMaterialCount(size_t i,const Mesh::Material& m);
	const Mesh::Material& GetMaterial(size_t i)const;
	size_t GetMaterialCount()const;

private:
	MeshPtr mesh;
	std::vector<Mesh::Material> materials;

	bool materialChanged = true;
	TextureList textureIndices;
	std::vector<glm::vec4> colors;

};

#endif // RENDERER_H_INCLUDE