/**
* @file UI.cpp
*/

#include "UI.h"

/**
* コンストラクタ
*/
UI::UI(const std::string name,
	const glm::vec2& position,
	const glm::vec2& size,
	float scale)
	:name(name)
	,position(position)
	,size(size)
	,scale(scale)
{
	this->position -= size / 2.0f;
}

/**
* 更新
*/
void UI::OnUpdate(float deltaTime)
{
	// 何もしない
}

/**
* クリックした時
*/
void UI::OnClick()
{
	// 何もしない
}