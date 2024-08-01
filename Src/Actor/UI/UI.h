/**
* @file UI.h
*/

#ifndef UI_H_INCLUDE
#define UI_H_INCLUDE

#include <string>
#include <glm/glm.hpp>

class UI
{
public:
	UI(const std::string name,
		const glm::vec2& position,
		const glm::vec2& size,
		float scale);
	~UI() = default;

	virtual void OnUpdate(float deltaTime);
	virtual void OnClick();

protected:
	// UI�̖��O
	std::string name;
	// UI�̈ʒu
	glm::vec2 position;
	// �E�B���h�̃T�C�Y
	glm::vec2 size;
	// �傫��
	float scale;

};

#endif // UI_H_INCLUDE