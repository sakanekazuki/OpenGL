/**
* @file Intersect.cpp
*/

#include "Intersect.h"
#include "GameEngine.h"
#include <algorithm>

/**
* �����ƕ��ʂ�����������W�����߂�
*
* @param start  �����̎n�_
* @param end    �����̏I�_
* @param q      ���ʏ�̔C�ӂ̓_
* @param normal ���ʂ̖@��
* @param p      ��_�̍��W����������
*
* @retval true  �������Ă���
* @retval false �������Ă��Ȃ�
*/
bool Intersect(const glm::vec3& start, const glm::vec3& end,
	const glm::vec3& q, const glm::vec3& normal, glm::vec3& p)
{
	const float distance = glm::dot(normal, q - start);
	const glm::vec3 v = end - start;

	// ���ꂪ�ق�0�̏ꍇ�A�����͕��ʂƕ��s�Ȃ̂Ō������Ȃ�
	const float denom = glm::dot(normal, v);
	if (std::abs(denom) < 0.0001f)
	{
		return false;
	}

	// ��_�܂ł̋���t��0�����܂���1���傫���ꍇ�A��_�͐����̊O���ɂ���̂Ŏ��ۂɂ͌������Ȃ�
	const float t = distance / denom;
	if (t < 0 || t > 1)
	{
		return false;
	}

	// ��_�͐�����ɂ���
	p = start + v * t;
	return true;
}

/**
* ���ƃA�N�^�[�̓����蔻����s��
*
* @param start       ���̍ŏ��̈ʒu
* @param end         ���̏I���̈ʒu
* @param rayPosition ���������ʒu
*/
bool RayCast(glm::vec3 start, glm::vec3 end, Hit& hit)
{
	// true=��������,false=�������ĂȂ�
	bool isHit = false;
	// �����蔻����s���A�N�^�[�̎擾
	std::vector<std::shared_ptr<Actor>> actor = GameEngine::Get().GetActors(Layer::Default);

	/**
	* �A�N�^�[����וς���
	*/
	struct actorDistance
	{
		std::shared_ptr<Actor> actor;
		float distance;
	};

	std::vector<actorDistance> actordistance;

	// �A�N�^�[�S�ē����
	for (int i = 0; i < actor.size(); ++i)
	{
		actordistance.push_back(actorDistance{ actor[i],glm::length(start - actor[i]->position) });
	}
	// �A�N�^�[�̕��בւ�
	std::sort(actordistance.begin(), actordistance.end(),
		[](const actorDistance& a1, const actorDistance& a2) {return a1.distance < a2.distance; });

	// ���ʂ̓��������ʒu�𒲂ׂ�
	for (int i = 0; i < actordistance.size(); ++i)
	{
		if (!actordistance[i].actor->collider)
		{
			continue;
		}
		Box box = static_cast<Box&>(*actordistance[i].actor->collider);

		// ���ׂ�{�b�N�X��Z���̕���
		glm::vec3 frontPosition = actordistance[i].actor->position + glm::vec3(0, 0, box.min.z);
		// ���ׂ�{�b�N�X��-Z���̕���
		glm::vec3 backPosition = actordistance[i].actor->position + glm::vec3(0, 0, box.max.z);
		// ���ׂ�{�b�N�X��X���̕���
		glm::vec3 rightPosition = actordistance[i].actor->position + glm::vec3(box.max.x, 0, 0);
		// ���ׂ�{�b�N�X��-X���̕���
		glm::vec3 leftPosition = actordistance[i].actor->position + glm::vec3(box.min.x, 0, 0);

		// ���ׂ�{�b�N�X��Z���̕��ʂƂ̋���
		float frontLength = glm::length(start - frontPosition);
		// ���ׂ�{�b�N�X��-Z���̕��ʂƂ̋���
		float backLength = glm::length(start - backPosition);
		// ���ׂ�{�b�N�X��X���̕��ʂƂ̋���
		float rightLength = glm::length(start - rightPosition);
		// ���ׂ�{�b�N�X��-X���̕��ʂƂ̋���
		float leftLegth = glm::length(start - leftPosition);
		// �A�N�^�[�̓�����͈�
		glm::vec3 max = actordistance[i].actor->position + box.max;
		glm::vec3 min = actordistance[i].actor->position + box.min;

		// ���ʂ̏��
		struct planeDistance
		{
			float distance;
			glm::vec3 planePos;
			glm::vec3 normal;
		};
		// ���ʂ̏�������
		std::vector<planeDistance> planeDis =
		{
			{glm::length(start - frontLength),frontPosition,glm::vec3(0,0,1)},
			{glm::length(start - backLength),backPosition,glm::vec3(0,0,-1)},
			{glm::length(start - rightLength),rightPosition,glm::vec3(1,0,0)},
			{glm::length(start - leftLegth),leftPosition,glm::vec3(-1,0,0)},
		};

		// ���ʂ̕��בւ�
		std::sort(planeDis.begin(), planeDis.end(),
			[](const planeDistance& p1, const planeDistance& p2) {return p1.distance < p2.distance; });
		// ���C�̓��������ʒu
		glm::vec3 rayPos;

		for (int j = 0; j < planeDis.size(); ++j)
		{
			// �ʂ��������Ă��邩���ׂ�
			if (Intersect(start, end, planeDis[j].planePos,
				planeDis[j].normal, rayPos))
			{
				// �������Ă���͈͂ɖʂ����邩���ׂ�
				if ((rayPos.x <= max.x && rayPos.y <= max.y && rayPos.z <= max.z) &&
					(rayPos.x >= min.x && rayPos.y >= min.y && rayPos.z >= min.z))
				{
					// ���������̂�isHit��true�ɂ���
					isHit = true;
					// �q�b�g���ɃA�N�^�[������
					hit.hitActor = actordistance[i].actor;
					// �q�b�g���ɋ���������
					hit.distance = planeDis[j].distance;
					// �q�b�g���ɓ��������ʒu������
					hit.hitPoint = rayPos;
				}
			}
		}
		// �������Ă�����true��Ԃ�
		if (isHit)
		{
			return true;
		}
	}
	// �������Ă��Ȃ����false��Ԃ�
	return false;
}