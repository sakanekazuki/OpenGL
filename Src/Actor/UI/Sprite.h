/**
* @file Sprite.h
*/

#ifndef SPRITE_H_INCLUDE
#define SPRITE_H_INCLUDE

#include "../Actor.h"

/**
* スプライト
*/
class Sprite :
	public Actor
{
public:
	Sprite(const glm::vec3& position, std::shared_ptr<Texture> tex,
		const glm::vec2& uv0 = glm::vec2(0), const glm::vec2& uv1 = glm::vec2(1),
		float pixelsPerMeter = 100.0f);
	Sprite(const Sprite&) = default;
	virtual ~Sprite() = default;
	Sprite& operator=(const Sprite&) = default;

	virtual std::shared_ptr<Actor> Clone()const;

	// 画像の左下テクスチャ座標
	glm::vec2 uv0 = glm::vec2(0);
	// 画像の右上テクスチャ座標
	glm::vec2 uv1 = glm::vec2(1);
	float pixelsPerMeter = 100.0f;

};

/**
* スプライトを描画するクラス
*/
class SpriteRenderer
{
public:
	SpriteRenderer() = default;
	~SpriteRenderer() = default;

	SpriteRenderer(const SpriteRenderer&) = delete;
	SpriteRenderer& operator=(const SpriteRenderer&) = delete;

	// バッファの管理
	bool Allocate(size_t maxSpriteCount);
	void Deallocate();

	// プリミティブの更新
	void Update(const std::vector<std::shared_ptr<Actor>>& sprites, const glm::mat4& matView);

	void Draw(std::shared_ptr<ProgramPipeline> pipeline, const glm::mat4& matVP);

private:
	// バッファ
	GLuint ibo = 0;
	struct Buffer
	{
		GLuint vbo = 0;
		GLuint vao = 0;
		GLsync sync = 0;
	}buffer[3];
	// 描画できる最大スプライト数
	size_t maxSpriteCount = 0;
	// 更新できるバッファの番号
	size_t updatingBufferIndex = 0;

	// プリミティブの描画情報
	struct Primitive
	{
		// インデックス数
		GLsizei count;
		// インデックス0に対応する頂点データの位置
		GLint baseVertex;
		// 描画に使うテクスチャ
		std::shared_ptr<Texture> texture;
	};
	// プリミティブ配列
	std::vector<Primitive> primitives;
};

#endif // SPRITE_H_INCLUDE