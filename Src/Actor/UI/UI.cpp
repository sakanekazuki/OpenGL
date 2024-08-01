/**
* @file UI.cpp
*/

#include "UI.h"

/**
* �R���X�g���N�^
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
* �X�V
*/
void UI::OnUpdate(float deltaTime)
{
	// �������Ȃ�
}

/**
* �N���b�N������
*/
void UI::OnClick()
{
	// �������Ȃ�
}