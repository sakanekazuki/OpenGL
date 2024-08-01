/**
* @file Button.cpp
*/

#include "Button.h"
#include "../../GameEngine.h"
#include <imgui.h>

/**
* �R���X�g���N�^
*/
Button::Button(const std::string name,
	const glm::vec2& position,
	const glm::vec2& size,
	float scale,
	std::function<void(void)> click)
	:UI{ name,position,size,scale }
	,click(click)
{

}

/**
* �X�V
*/
void Button::OnUpdate(float deltaTime)
{
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	ImGui::SetNextWindowPos(ImVec2(position.x, position.y));
	ImGui::Begin(name.c_str(), nullptr,
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	ImGui::SetWindowFontScale(scale);
	if (ImGui::Button(name.c_str()))
	{
		OnClick();
	}
	ImGui::End();
}

/**
* �N���b�N���ꂽ���ɌĂ�
*/
void Button::OnClick()
{
	click();
}