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
	// UIの名前
	std::string name;
	// UIの位置
	glm::vec2 position;
	// ウィンドのサイズ
	glm::vec2 size;
	// 大きさ
	float scale;

};

#endif // UI_H_INCLUDE