/**
* @file Collision.cpp
*/

#include "Collision.h"
#include "../Actor/Actor.h"

// ���������_��0�Ƃ݂Ȃ��l
static const float epsilon = FLT_EPSILON * 8;

/**
* �R���X�g���N�^
*/
Collider::Collider(ShapeTye type)
	:shapeType(type)
{

}

/**
* ShapeType�̎擾
*/
ShapeTye Collider::GetShapeType()const
{
	return shapeType;
}

/**
* �R���X�g���N�^
*/
Box::Box() :Collider(ShapeTye::box)
{

}

/**
* �R���X�g���N�^
* 
* @param min �ŏ��l
* @param max �ő�l
*/
Box::Box(const glm::vec3& min, const glm::vec3& max)
	:Collider(ShapeTye::box)
	,min(min)
	,max(max)
{

}

/**
* �����̃R���C�_�[�̍쐬
*/
std::shared_ptr<Box> Box::Create(const glm::vec3& min, const glm::vec3& max)
{
	return std::make_shared<Box>(min, max);
}

/**
* �N���[���쐬
*/
std::shared_ptr<Collider> Box::Clone()const
{
	return std::make_shared<Box>(*this);
}

/**
* �R���X�g���N�^�@
*/
Cylinder::Cylinder()
	:Collider(ShapeTye::cylinder)
{

}

/**
* �R���X�g���N�^
* 
* @param bottom ���̍��W
* @param radius ���a
* @param height ����
*/
Cylinder::Cylinder(const glm::vec3& bottom, float radius, float height)
	:Collider(ShapeTye::cylinder)
{

}

/**
* �����R���C�_�[�̍쐬
*/
std::shared_ptr<Cylinder> Cylinder::Create(const glm::vec3& bottom,
	float radius, float height)
{
	return std::make_shared<Cylinder>(bottom, radius, height);
}

/**
* �N���[���쐬
*/
std::shared_ptr<Collider> Cylinder::Clone()const
{
	return std::make_shared<Cylinder>(*this);
}

/**
* �����̂ƒ����̂̏Փ�
*/
bool CollisionBoxBox(Actor& actorA, Actor& actorB, Contact& contact)
{
	// ���[���h���W�n�̃R���C�_�[���v�Z����
	Box a = static_cast<Box&>(*actorA.collider);
	a.min += actorA.position;
	a.max += actorA.position;

	Box b = static_cast<Box&>(*actorB.collider);
	b.min += actorB.position;
	b.max += actorB.position;

	// a�̍����ʂ�b�̉E���ʂ��E�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dx0 = b.max.x - a.min.x;
	if (dx0 <= 0)
	{
		return false;
	}

	// a�̉E���ʂ�b�̍����ʂ�荶�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dx1 = a.max.x - b.min.x;
	if (dx1 <= 0)
	{
		return false;
	}

	// a�̉��ʂ�b�̏�ʂ���ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dy0 = b.max.y - a.min.y;
	if (dy0 <= 0)
	{
		return false;
	}

	// a�̏�ʂ�b�̉��ʂ�艺�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dy1 = a.max.y - b.min.y;
	if (dy1 <= 0)
	{
		return false;
	}

	// a�̉����ʂ�b�̎�O���ʂ���O�ɂ���Ȃ�Փ˂��Ă��Ȃ�
	const float dz0 = b.max.z - a.min.z;
	if (dz0 <= 0)
	{
		return false;
	}

	// a�̎�O���ʂ��b�̉����ʂ����ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dz1 = a.max.z - b.min.z;
	if (dz1 <= 0)
	{
		return false;
	}

	// XYZ�̊e���ɂ��āu�Z������(�d�Ȃ��Ă��镔���̒���)�v���Z��������I������
	// �Փ˖�(�A�N�^�[B�̂����ꂩ�̖�)�̖@��
	glm::vec3 normal;
	// �Z�������ƕ���
	glm::vec3 penetration;
	if (dx0 <= dx1)
	{
		penetration.x = -dx0;
		normal.x = 1;
	}
	else
	{
		penetration.x = dx1;
		normal.x = -1;
	}
	if (dy0 <= dy1)
	{
		penetration.y = -dy0;
		normal.y = 1;
	}
	else
	{
		penetration.y = dy1;
		normal.y = -1;
	}
	if (dz0 <= dz1)
	{
		penetration.z = -dz0;
		normal.z = 1;
	}
	else
	{
		penetration.z = dz1;
		normal.z = -1;
	}

	// �Z�������̐�Βl
	glm::vec3 absPenetration = glm::abs(penetration);

	// �Փ˖ʂɂȂ�\���̍���
	glm::vec3 score = glm::vec3(0);

	// �Z���������Z�������̂ق����Փ˖ʂł���\��������
	for (int a = 0; a < 2; ++a)
	{
		for (int b = a + 1; b < 3; ++b)
		{
			if (absPenetration[a] < absPenetration[b])
			{
				++score[a];
			}
			else
			{
				++score[b];
			}
		}
	}

	// ���΃x���V�e�B���v�Z����
	glm::vec3 rv = actorA.velocity - actorB.velocity;

	// �Z�����n�܂�������t���v�Z����
	glm::vec3 t = glm::vec3(-FLT_MAX);
	for (int i = 0; i < 3; ++i)
	{
		if (rv[i])
		{
			t[i] = penetration[i] / rv[i];
		}
	}

	// �Z�����n�܂������Ԃ��傫���قǁA��葁�����_�ŐZ�����n�܂����ƍl������
	const float deltaTime = 1.0f / 60.0f;
	for (int a = 0; a < 2; ++a)
	{
		for (int b = a + 1; b < 3; ++b)
		{
			int i = a;
			if (t[a] < t[b])
			{
				i = b;
			}
			if (t[i] > 0 && t[i] <= deltaTime)
			{
				score[i] += 1.5f;
			}
		}
	}

	// ���\�����Ⴂ���������O����
	// �l���������ꍇ�AZ,X,Y�̏��ŗD��I�ɏ��O����
	if (score.x <= score.y)
	{
		normal.x = 0;
		if (score.z <= score.y)
		{
			normal.z = 0;
		}
		else
		{
			normal.y = 0;
		}
	}
	else
	{
		normal.y = 0;
		if (score.z <= score.x)
		{
			normal.z = 0;
		}
		else
		{
			normal.x = 0;
		}
	}

	// XYZ���̂����A�Z���������ł��Z�����̐����������c��
	if (absPenetration.x >= absPenetration.y)
	{
		penetration.x = 0;
		if (absPenetration.z >= absPenetration.y)
		{
			penetration.z = 0;
		}
		else
		{
			penetration.y = 0;
		}
	}
	else
	{
		penetration.y = 0;
		if (absPenetration.x >= absPenetration.z)
		{
			penetration.x = 0;
		}
		else
		{
			penetration.z = 0;
		}
	}

	// �Փˏ���ݒ肷��
	contact.a = &actorA;
	contact.b = &actorB;
	contact.velocityA = actorA.velocity;
	contact.velocityB = actorB.velocity;
	contact.penetration = penetration;
	contact.normal = normal;

	// �Փ˖ʂ̍��W���v�Z����
	{
		// ��{�I�ɃA�N�^�[B�̍��W���g�����A�A�N�^�[B���Õ��̏ꍇ�̓A�N�^�[A�̍��W���g��
		Actor* target = &actorB;
		glm::vec3 targetNormal = normal;
		if (actorB.isStatic)
		{
			target = &actorA;
			// �@���̌����𔽓]����
			targetNormal *= -1;
		}
		// �R���C�_�[�̔��a���v�Z����
		const Box& targetBox = static_cast<Box&>(*target->collider);
		glm::vec3 halfSize = (targetBox.max + targetBox.min) * 0.5f;
		// �R���C�_�[�̒��S���W���v�Z����
		glm::vec3 center =
			(targetBox.max + targetBox.min) * 0.5f;
		// �Փ˖ʂ̍��W���v�Z����
		contact.position =
			target->position + center - halfSize * targetNormal;
	}

	// �Z�������̒������v�Z����
	contact.penLength = glm::length(penetration);

	// �Փ˂��Ă���
	return true;
}

/**
* �����̂Ɖ~���̏Փ�
*/
bool CollisionBoxCylinder(Actor& actorA, Actor& actorB, Contact& contact)
{
	// ���[���h���W�n�̏Փː}�`���v�Z����

	// �{�b�N�X�ɕϊ�
	Box a = static_cast<Box&>(*actorA.collider);
	// ������ʒu�̌v�Z
	a.min += actorA.position;
	a.max += actorA.position;

	// �����~���ɕϊ�
	Cylinder b = static_cast<Cylinder&>(*actorB.collider);
	// ������ʒu�̌v�Z
	b.bottom += actorB.position;

	// a�̉��ʂ�b�̏�ʂ���ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dy0 = (b.bottom.y + b.height) - a.min.y;
	if (dy0 <= 0)
	{
		return false;
	}

	// a�̏�ʂ�b�̉��ʂ�艺�ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dy1 = a.max.y - b.bottom.y;
	if (dy1 <= 0)
	{
		return false;
	}

	// �~���̒��S�ɍł��߂������̓��̍��W�n(�ŋߐړ_)�����߂�
	const float cx = glm::clamp(b.bottom.x, a.min.x, a.max.x);
	const float cz = glm::clamp(b.bottom.z, a.min.z, a.max.z);
	const glm::vec3 closestPointXZ(cx, 0, cz);

	// �~���̒��S����ŋߐړ_�܂ł̋������v�Z����
	const float dx = closestPointXZ.x - b.bottom.x;
	const float dz = closestPointXZ.z - b.bottom.z;
	const float d2 = dx * dx + dz * dz;

	// �ŋߐړ_�܂ł̋������~���̔��a��蒷����΁A�Փ˂��Ă��Ȃ�
	if (d2 > b.radius * b.radius)
	{
		return false;
	}

	// Y�����̐Z�������ƕ������v�Z
	glm::vec3 penetration(0);
	glm::vec3 normal(0);
	if (dy0 < dy1)
	{
		penetration.y = -dy0;
		normal.y = 1;
	}
	else
	{
		penetration.y = dy1;
		normal.y = -1;
	}

	// XZ�����̌������������𒲂ׂ�(�ŋߐړ_���܂܂��ʂɑΉ�����r�b�g�𗧂Ă�)
	
	// min�����̃r�b�g�t���O
	int flagMin = 0;
	// max�����̃r�b�g�t���O
	int flagMax = 0;

	for (int i = 0; i < 3; i += 2)
	{
		if (closestPointXZ[i] <= a.min[i])
		{
			flagMin |= (1 << i);
		}
	}

	const int flag = flagMin | flagMax;

	if (flag)
	{
		// XZ�ŋߐړ_�������̂̕\�ʂɂ���ꍇ�AXZ�ŋߐړ_�Ɖ~�����Ԓ�����@���Ƃ���
		if (d2 >= epsilon)
		{
			normal.x = dx;
			normal.z = dz;
		}
		else
		{
			// �ŋߐړ_�Ɖ~���̒��S���߂�����Ɩ@�����v�Z
			// �����ɍŋߐړ_���܂܂��ʂ��狁�߂�
			for (int i = 0; i < 3; i += 2)
			{
				if (flagMin & (1 << i))
				{
					normal[i] = -1;
				}
			}
		}
		// XZ�����̖@���𐳋K��
		const float invD =
			1.0f / std::sqrt(normal.x * normal.x + normal.z * normal.z);
		normal.x *= invD;
		normal.z *= invD;
	}
	else
	{
		// XZ�ŋߐړ_�������̂̓����ɂ���ꍇ�A
		// �x���V�e�B����Փ˂̉\��������Ɣ��f�����ʂ̂����A�ł��߂��ʂŏՓ˂����ƍl����
		// �x���V�e�B��0�̏ꍇ�A�S�Ă̖ʂɏՓ˂̉\��������Ɣ��f����

		// ���΃x���V�e�B���v�Z
		const glm::vec3 rv = actorA.velocity - actorB.velocity;
		const bool noVelocity = glm::dot(rv, rv) < epsilon;

		float dmin = FLT_MAX;
		// �ł��߂���
		int nearestFace = 0;
		for (int i = 0; i < 3; i += 2)
		{
			if (rv[i] < 0 || noVelocity)
			{
				float d = closestPointXZ[i] - a.min[i];
				if (d < dmin)
				{
					dmin = d;
					nearestFace = i;
				}
			}
			if (rv[i] > 0 || noVelocity)
			{
				float d = a.max[i] - closestPointXZ[i];
				if (d < dmin)
				{
					dmin = d;
					nearestFace = i + 3;
				}
			}
		}
		// �ł��߂��ʂ̖@����ݒ�
		if (nearestFace < 3)
		{
			normal[nearestFace] = 1;
		}
		else
		{
			normal[nearestFace] = -1;
		}
	}

	// XZ�����̐Z���������v�Z
	float distance = b.radius;
	if (d2 >= epsilon)
	{
		distance -= std::sqrt(d2);
	}
	penetration.x = -normal.x * distance;
	penetration.z = -normal.z * distance;

	// �Z���������������������O
	// ���ʂ̏Փ˂�����ꍇ�AXZ�x�N�g���̒�����Y���r
	const glm::vec3 absPenetration = glm::abs(penetration);
	if (flag && absPenetration.y > distance)
	{
		penetration.y = 0;
		normal.y = 0;
	}
	else
	{
		// ���ʏՓˈȊO�̏ꍇ�A�ł��Z���������Z�����������c��
		float pmin = FLT_MAX;
		int axisMin = 0;
		for (int i = 0; i < 3; ++i)
		{
			if (absPenetration[i] < pmin)
			{
				pmin = absPenetration[i];
				axisMin = i;
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			if (i != axisMin)
			{
				penetration[i] = 0;
				normal[i] = 0;
			}
		}
	}

	// �Փˏ���ݒ�
	contact.a = &actorA;
	contact.b = &actorB;
	contact.velocityA = actorA.velocity;
	contact.velocityB = actorB.velocity;
	contact.penetration = penetration;
	contact.normal = normal;
	contact.penLength = glm::length(penetration);

	// �Փ˖ʂ̍��W���v�Z
	{
		// ��{�I�ɃA�N�^�[B�̍��W���g�����A�A�N�^�[B���Õ��̏ꍇ�̓A�N�^�[A�̍��W���g��
		const glm::vec3 center = (a.min + a.max) * 0.5f;
		const glm::vec3 size = (a.max - a.min) * 0.5f;
		if (actorB.isStatic)
		{
			contact.position = center;
			if (normal.y)
			{
				// Y�����̏Փ˂̏ꍇ
				contact.position.y -= size.y * normal.y;
			}
			else
			{
				// XZ�����̏Փ˂̏ꍇ
				contact.position.x -= size.x * normal.x;
				contact.position.z -= size.z * normal.z;
			}
		}
		else
		{
			contact.position = b.bottom;
			if (normal.y)
			{
				// Y�����̏Փ˂̏ꍇ
				contact.position.y += b.height * (0.5f + 0.5f * normal.y);
			}
			else
			{
				// XZ�����̏Փ˂̏ꍇ�E�E�E
				contact.position.x -= normal.x * b.radius;
				contact.position.z -= normal.z * b.radius;
			}
		}
	}
	// �Փ˂��Ă���
	return true;
}

/**
* �~���Ɖ~���̏Փ�
*/
bool CollisionCylinderCylinder(Actor& actorA, Actor& actorB, Contact& contact)
{
	// ���[���h���W�n�̏Փː}�`���v�Z
	Cylinder a = static_cast<Cylinder&>(*actorA.collider);
	a.bottom += actorA.position;

	Cylinder b = static_cast<Cylinder&>(*actorB.collider);
	b.bottom += actorB.position;

	// a�̉��ʂ�b�̏�ʂ���ɂ���Ȃ�A�Փ˂��Ă��Ȃ�
	const float dy0 = (b.bottom.y + b.height) - a.bottom.y;
	if (dy0 <= 0)
	{
		return false;
	}

	const float dy1 = (a.bottom.y + a.height) - b.bottom.y;
	if (dy1 <= 0)
	{
		return false;
	}

	// XZ���ʏ�̋��������a�̍��v��艓����΁A�Փ˂��Ă��Ȃ�
	const float dx = b.bottom.x - a.bottom.x;
	const float dz = b.bottom.z - a.bottom.z;
	const float d2 = dx * dx + dz * dz;
	const float r = a.radius + b.radius;
	if (d2 > r * r)
	{
		return false;
	}

	// Y�����̐Z�������ƕ������v�Z
	glm::vec3 normal(0);
	glm::vec3 penetration(0);
	if (dy0 < dy1)
	{
		penetration.y = -dy0;
		normal.y = 1;
	}
	else
	{
		penetration.y = dy1;
		normal.y = -1;
	}

	// XZ�����̐Z�������ƕ������v�Z
	float lenghtXZ;
	if (d2 >= epsilon)
	{
		const float d = std::sqrt(d2);
		const float invD = 1.0f / d;
		normal.x = -dx * invD;
		normal.z = -dz * invD;
		lenghtXZ = r - d;
	}
	else
	{
		// XZ���W���d�Ȃ��Ă���ꍇ�A�@�����v�Z�ł��Ȃ��̂Ńx���V�e�B�ő�p
		lenghtXZ = r;
		normal.x = actorA.velocity.x - actorB.velocity.x;
		normal.z = actorA.velocity.z - actorB.velocity.z;
		if (normal.x || normal.z)
		{
			const float invD =
				1.0f * std::sqrt(normal.x * normal.x + normal.z * normal.z);
			normal.x *= invD;
			normal.z *= invD;
		}
		else
		{
			// �x���V�e�B��0�̏ꍇ�͕������m��ł��Ȃ��̂�;X�����ɂ��Ă���
			normal.x = 1;
		}
	}

	penetration.x = -lenghtXZ * normal.x;
	penetration.z = -lenghtXZ * normal.z;

	// �Z�������̒������������O
	if (std::abs(penetration.y) <= lenghtXZ)
	{
		penetration.x = penetration.z = 0;
		normal.x = normal.z = 0;
	}
	else
	{
		penetration.y = 0;
		normal.y = 0;
	}

	// �Փˏ���ݒ�
	contact.a = &actorA;
	contact.b = &actorB;
	contact.velocityA = actorA.velocity;
	contact.velocityB = actorB.velocity;
	contact.normal = normal;
	contact.penLength = glm::length(penetration);

	// �Փ˖ʂ̍��W���v�Z
	{
		// ��{�I�ɃA�N�^�[B�̍��W���g�����A�A�N�^�[B���Õ��̏ꍇ�̓A�N�^�[A�̍��W���g��
		Actor* target = &actorB;
		Cylinder* targetCollider = &b;
		glm::vec3 targetNormal = normal;
		if (actorB.isStatic)
		{
			target = &actorA;
			targetCollider = &a;
			// �@���̌����𔽓]
			targetNormal *= -1;
		}

		// �Փ˖ʂ̍��W���v�Z
		contact.position = targetCollider->bottom;
		if (normal.y)
		{
			// Y�����̏Փ˂̏ꍇ
			if(targetNormal.y>=0)
			{
				contact.position.y += targetCollider->height;
			}
			else
			{
				// XZ�����̏Փ˂̏ꍇ
				contact.position.x -= targetNormal.x * targetCollider->radius;
				contact.position.y += targetCollider->height * 0.5f;
				contact.position.z -= targetNormal.z * targetCollider->radius;
			}
		}
	}
	// �Փ˂��Ă���
	return true;
}

/**
* �~���ƒ����̂̏Փ�
*/
bool CollisionCylinderBox(Actor& actorA, Actor& actorB, Contact& contact)
{
	return CollisionBoxCylinder(actorB, actorA, contact);
}