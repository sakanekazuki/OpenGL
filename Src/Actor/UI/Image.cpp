/**
* @file Image.cpp
*/

#include "Image.h"
#include "../GameEngine.h"
#include <imgui.h>

/**
* �R���X�g���N�^
*/
Image::Image(const std::string& name,
	const glm::vec2& position,
	const glm::vec2& size,
	float scale,
	const glm::vec2& imgSize,
	ImTextureID texId)
	:UI{ name,position,size,scale }
	,id(texId)
	,imgSize(imgSize)
{

}

/**
* �X�V
*/
void Image::OnUpdate(float deltaTime)
{
	// �E�B�h�E�̑傫����ݒ�
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	// �E�B�h�E�̈ʒu�ݒ�
	ImGui::SetNextWindowPos(ImVec2(position.x,position.y));
	// �E�B���h�E�̏����ݒ�
	ImGui::Begin("GameClear", nullptr, ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	ImGui::Image(id, ImVec2(imgSize.x, imgSize.y));
	ImGui::End();
}