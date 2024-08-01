/**
* @file Image.cpp
*/

#include "Image.h"
#include "../GameEngine.h"
#include <imgui.h>

/**
* コンストラクタ
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
* 更新
*/
void Image::OnUpdate(float deltaTime)
{
	// ウィドウの大きさを設定
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
	// ウィドウの位置設定
	ImGui::SetNextWindowPos(ImVec2(position.x,position.y));
	// ウィンドウの初期設定
	ImGui::Begin("GameClear", nullptr, ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	ImGui::Image(id, ImVec2(imgSize.x, imgSize.y));
	ImGui::End();
}