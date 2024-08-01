/**
* @file Tank.h
*/

#ifndef TANK_H_INCLUDE
#define TANK_H_INCLUDE

#include "../Actor.h"
#include "../../GameEngine.h"

/**
* ��Ԃ̎��
*/
enum class TankType
{
	TIGER = 0,
	T34,
};

/**
* ��Ԃ̏��f��
*/
struct TankMaterial
{
	// �A�N�^�[�̖��O
	const char* name;
	// �v���~�e�B�u
	const char* primNameBottom;
	// �v���~�e�B�u
	const char* primNameTop;
	// �e�N�X�`���[
	const char* texName;
	// �����蔻��
	const std::shared_ptr<Collider> collider;
	// ���S�܂ł̋���
	glm::vec3 adjustment;
	// �ő呬�x
	float maxSpeed;
	// �U����
	int atk;
	// ���b
	int armor;
	// �d��
	float mass;
	// �˒�����
	float rangeDistance;

	// �J�����̈ʒu
	glm::vec3 cameraPosition;
};

class Tank :
	public Actor
{
public:
	Tank(
		const glm::vec3& position,
		const glm::vec3& scale,
		float rotation,
		TankType type
		);
	virtual ~Tank() = default;

	virtual void OnUpdate(float deltaTime)override;

	virtual void OnCollision(const Contact& contact)override;

	void m_Draw(const ProgramPipeline& pipeline, const glm::mat4& matProj, const glm::mat4& matView)override;

	void AcceptDamage(int damage);

	void SetCanMove(bool canMove);
	bool GetCanMoving()const;

protected:
	// �C
	Actor tankTop;
	// true = ������Afalse = �����Ȃ�
	bool canMoving = true;
	// ���b
	const int armor;
	// �U����
	const int attack;
	// ����
	const float maxSpeed;
	// �C�̉�]���鑬��
	const float cannnonRotationSpeed = 0.01f;
	// �J�����̈ʒu
	const glm::vec3 cameraPosition;
	// �˒�����
	float rangeDistance;

};

#endif // TANK_H_INCLUDE