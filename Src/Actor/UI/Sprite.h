/**
* @file Sprite.h
*/

#ifndef SPRITE_H_INCLUDE
#define SPRITE_H_INCLUDE

#include "../Actor.h"

/**
* �X�v���C�g
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

	// �摜�̍����e�N�X�`�����W
	glm::vec2 uv0 = glm::vec2(0);
	// �摜�̉E��e�N�X�`�����W
	glm::vec2 uv1 = glm::vec2(1);
	float pixelsPerMeter = 100.0f;

};

/**
* �X�v���C�g��`�悷��N���X
*/
class SpriteRenderer
{
public:
	SpriteRenderer() = default;
	~SpriteRenderer() = default;

	SpriteRenderer(const SpriteRenderer&) = delete;
	SpriteRenderer& operator=(const SpriteRenderer&) = delete;

	// �o�b�t�@�̊Ǘ�
	bool Allocate(size_t maxSpriteCount);
	void Deallocate();

	// �v���~�e�B�u�̍X�V
	void Update(const std::vector<std::shared_ptr<Actor>>& sprites, const glm::mat4& matView);

	void Draw(std::shared_ptr<ProgramPipeline> pipeline, const glm::mat4& matVP);

private:
	// �o�b�t�@
	GLuint ibo = 0;
	struct Buffer
	{
		GLuint vbo = 0;
		GLuint vao = 0;
		GLsync sync = 0;
	}buffer[3];
	// �`��ł���ő�X�v���C�g��
	size_t maxSpriteCount = 0;
	// �X�V�ł���o�b�t�@�̔ԍ�
	size_t updatingBufferIndex = 0;

	// �v���~�e�B�u�̕`����
	struct Primitive
	{
		// �C���f�b�N�X��
		GLsizei count;
		// �C���f�b�N�X0�ɑΉ����钸�_�f�[�^�̈ʒu
		GLint baseVertex;
		// �`��Ɏg���e�N�X�`��
		std::shared_ptr<Texture> texture;
	};
	// �v���~�e�B�u�z��
	std::vector<Primitive> primitives;
};

#endif // SPRITE_H_INCLUDE