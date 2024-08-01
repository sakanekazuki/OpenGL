/**
* Actor.h
*/

#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED

#include <glad/glad.h>
#include "../Primitive.h"
#include "../Texture.h"
#include "../ProgramPipeline.h"
#include "../Collision/Collision.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <memory>

/*
* ��]���Ă��鎞�̓����蔻��
*/
struct OBB
{
	// �ʒu
	glm::vec3 position;
	// �����x�N�g��
	glm::vec3 rotation[3];
	// ���S����ʂ܂ł̋���
	glm::vec3 lenght;
};

/**
* �\�����C���[
*/
enum class Layer
{
	Default,
	Sprite,
	UI,
};

// ���C���[�̐�
static const size_t layerCount = 3;

/**
* ���̂𐧌䂷��p�����[�^
* 
* ���O(string)
* �v���~�e�B�u(Primitive)
* �e�N�X�`��(Texture)
* �ʒu(glm::vec3)
* �g��k��(glm::vec3)
* ��]�p�x(float)
* ���_�܂ł̋���(glm::vec3)
* 
* ���x(glm::vec3)
* ����(float)
* ���S�t���O(bool)
* �ϋv�l(float)
* 
* �Փ˔���(Box)
* ����(float)
* �����W��(float)
* ���C�W��(float)
* ��������ǂ���(bool)
*/
class Actor
{
public:
	Actor(
		const std::string& name,
		const Primitive& prim,
		std::shared_ptr<Texture> tex,
		const glm::vec3& position,
		const glm::vec3& scale,
		float rotation,
		const glm::vec3& adjustment
	);

	virtual ~Actor();
	virtual void OnUpdate(float deltaTime);
	virtual void OnCollision(const struct Contact& contact);
	virtual void OnTrigger(const struct Contact& contact);

	virtual void m_Draw(const ProgramPipeline& piepline, const glm::mat4& matProj, const glm::mat4& matView);

	// �A�N�^�[�̖��O
	std::string name;
	// �`�悷��v���~�e�B�u
	Primitive prim;
	// �`��Ɏg���e�N�X�`��
	std::shared_ptr<Texture> tex;
	// ���̂̈ʒu
	glm::vec3 position;
	// ���̂̊g��k��
	glm::vec3 scale;
	// ���̂̉�]�p�x
	float rotation;
	// ���̂����_�Ɉړ����邽�߂̋���
	glm::vec3 adjustment;
	// �e�N�X�`���ɍ�������F
	glm::vec4 color = glm::vec4(1);
	// �d�͂̉e���x
	float gravityScale = 1;

	// ���x(���[�g�����b)
	glm::vec3 velocity = glm::vec3(0);
	// ����(�b�A0�ȉ��Ȃ�����Ȃ�)
	float lifespan = 0;
	// true=���S(�����҂�) false=������
	bool isDead = false;
	// �ϋv�l
	float health = 10;

	// �Փ˔���
	std::shared_ptr<Collider> collider;
	// ����(kg)
	float mass = 1;
	// �����W��(0.0�`1.0)
	float cor = 0.7f;
	// ���C�W��(0.0�`1.0)
	float friction = 0.7f;
	// false=�������镨�� true=�������Ȃ�����
	bool isStatic = false;
	// true=�d�͂��󂯂� false=�d�͂��󂯂Ȃ�
	bool useGravity = true;
	// �^�O
	std::string tag = "none";
	// �\�����C���[
	Layer layer = Layer::Default;

};

void Draw(
	const Actor& actor,
	const ProgramPipeline& pipeline,
	const glm::mat4& matProj,
	const glm::mat4& matView
);

Actor* FindName(const char* name);
Actor* FindTag(const char* tag);

/**
* �Փˏ��
*/
struct Contact
{
	Actor* a = nullptr;
	Actor* b = nullptr;
	// �Փˎ��_�ł̃A�N�^�[A�̃x���V�e�B
	glm::vec3 velocityA = glm::vec3(0);
	// �Փˎ��_�ł̃A�N�^�[B�̃x���V�e�B
	glm::vec3 velocityB = glm::vec3(0);
	// �Z������
	glm::vec3 penetration = glm::vec3(0);
	// �Փ˖ʂ̖@��
	glm::vec3 normal = glm::vec3(0);
	//�Փ˖ʂ̍��W
	glm::vec3 position = glm::vec3(0);
	// �Z�������̒���
	float penLength = 0;
};

bool DetectCollision(Actor& actorA, Actor& actorB, Contact& contact);
void SolveContact(Contact& contact);
bool Equal(const Contact& ca, const Contact& cb);

#endif // ACTOR_H_INCLUDED