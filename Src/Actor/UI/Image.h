/**
* @file Imaga.h
*/

#ifndef IMAGE_H_INCLUDE
#define IMAGE_H_INCLUDE

#include "UI.h"
#include <imgui.h>

class Image :
	public UI
{
public:
	Image(const std::string& name,
		const glm::vec2& position,
		const glm::vec2& size,
		float scale,
		const glm::vec2& imgSize,
		ImTextureID texId);
	~Image() = default;

	void OnUpdate(float deltatime)override;

private:
	// �摜�̑傫��
	glm::vec2 imgSize;
	// �\������e�N�X�`��
	ImTextureID id;

};

#endif // IMAGE_H_INCLUDE