/**
* @file Button.h
*/

#ifndef BUTTON_H_INCLUDE
#define BUTTON_H_INCLUDE

#include "UI.h"
#include <functional>

class Button :
	public UI
{
public:
	Button(const std::string name,
		const glm::vec2& position,
		const glm::vec2& size,
		float scale,
		std::function<void(void)> click);
	~Button() = default;

	void OnUpdate(float deltaTime)override;
	void OnClick()override;

private:
	// クリックした時に実行する関数
	std::function<void(void)> click;

};

#endif // BUTTON_H_INCLUDE