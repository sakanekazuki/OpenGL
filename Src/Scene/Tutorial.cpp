/**
* @file Tutorial.cpp
*/

#include "Tutorial.h"
#include "../../GameEngine.h"
#include "../Actor/UI/Image.h"
#include "../Actor/UI/Button.h"
#include "../../SceneManager.h"
#include <imgui.h>

/**
* �R���X�g���N�^
*/
Tutorial::Tutorial()
	:Scene(TUTORIALSCENE, "")
{
	// �Q�[���G���W���擾
	GameEngine& engine = GameEngine::Get();
	// �w�i�F�ݒ�
	engine.SetglClearColor(glm::vec3(0.5f, 0.5f, 0.5f));
}

/**
* �X�V
*/
void Tutorial::Update(float deltaTime)
{
	// �Q�[���G���W���擾
	GameEngine& engine = GameEngine::Get();
	// �E�B���h�E�̑傫���擾
	glm::vec2 windowSize = engine.GetWindowSize();

	// �E�B���h�E�̑傫���w��
	ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
	// �E�B���h�E�̈ʒu�w��
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	// �����ݒ�
	ImGui::Begin("KeyGuid", nullptr,
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);	

	//----------------------------------
	// ����

	ImGui::SetCursorPos(ImVec2(275, 100));

	// �e�N�X�`�����擾
	std::shared_ptr<Texture> upTex = engine.LoadTexture("Res/KeyGuid/up.tga");
	// �e�N�X�`����ϊ�
	ImTextureID upTexId = reinterpret_cast<ImTextureID>(upTex->GetID());
	// �摜�\��
	ImGui::Image(upTexId, ImVec2(100, 100));
	
	//----------------------------------


	//----------------------------------
	// W�L�[�摜

	ImGui::SetCursorPos(ImVec2(300, ImGui::GetCursorPosY() - 10));
	
	// �摜�擾
	std::shared_ptr<Texture> wTex = engine.LoadTexture("Res/KeyGuid/W.tga");
	// �摜��ϊ�
	ImTextureID wTexId = reinterpret_cast<ImTextureID>(wTex->GetID());
	// �摜�\��
	ImGui::Image(wTexId, ImVec2(50, 50));

	//----------------------------------


	//----------------------------------
	// ����]�摜

	ImGui::SetCursorPos(ImVec2(159, ImGui::GetCursorPosY() + 10));

	// �摜�擾
	std::shared_ptr<Texture> leftTex = engine.LoadTexture("Res/KeyGuid/left.tga");
	// �摜��ϊ�
	ImTextureID leftTexId = reinterpret_cast<ImTextureID>(leftTex->GetID());
	// �摜�\��
	ImGui::Image(leftTexId, ImVec2(75, 75));
	
	//----------------------------------


	//----------------------------------
	// A�L�[�摜

	// ���ɕ��ׂ�
	ImGui::SameLine();
	// �摜�擾
	std::shared_ptr<Texture> ATex = engine.LoadTexture("Res/KeyGuid/A.tga");
	// �摜�ϊ�
	ImTextureID ATexId = reinterpret_cast<ImTextureID>(ATex->GetID());
	// �摜�\��
	ImGui::Image(ATexId, ImVec2(50, 50));

	//----------------------------------


	//----------------------------------
	// S�L�[�摜

	// ���ɕ��ׂ�
	ImGui::SameLine();
	
	// �摜�擾
	std::shared_ptr<Texture> STex = engine.LoadTexture("Res/KeyGuid/S.tga");
	// �摜�ϊ�
	ImTextureID STexId = reinterpret_cast<ImTextureID>(STex->GetID());
	// �摜�\��
	ImGui::Image(STexId, ImVec2(50, 50));

	//----------------------------------


	//----------------------------------
	// D�L�[�摜

	// ���ɕ��ׂ�
	ImGui::SameLine();

	// �摜�擾
	std::shared_ptr<Texture> DTex = engine.LoadTexture("Res/KeyGuid/D.tga");
	// �摜�ϊ�
	ImTextureID DTexId = reinterpret_cast<ImTextureID>(DTex->GetID());
	// �摜�\��
	ImGui::Image(DTexId, ImVec2(50, 50));

	//----------------------------------


	//----------------------------------
	// �E��]

	// ���ɕ��ׂ�
	ImGui::SameLine();

	// �摜�擾
	std::shared_ptr<Texture> rightTex = engine.LoadTexture("Res/KeyGuid/right.tga");
	// �摜�ϊ�
	ImTextureID rightTexId = reinterpret_cast<ImTextureID>(rightTex->GetID());
	// �摜�\��
	ImGui::Image(rightTexId, ImVec2(75, 75));

	//----------------------------------


	//----------------------------------
	// �����

	ImGui::SetCursorPos(ImVec2(275, ImGui::GetCursorPosY() - 25));

	// �摜�擾
	std::shared_ptr<Texture> downTex = engine.LoadTexture("Res/KeyGuid/down.tga");
	// �摜�ϊ�
	ImTextureID downTexId = reinterpret_cast<ImTextureID>(downTex->GetID());
	// �摜�\��
	ImGui::Image(downTexId, ImVec2(100, 100));

	//----------------------------------


	//----------------------------------
	// �U���̕���

	// �\������ʒu�̎w��
	ImGui::SetCursorPos(ImVec2(750, 150));
	// �E�B���h�E�Ŏg�p���镶���̑傫��
	ImGui::SetWindowFontScale(3);
	// �e�L�X�g�\��
	ImGui::Text("Attack");

	//----------------------------------


	//----------------------------------
	// �C����]

	// �\���ʒu�ݒ�
	ImGui::SetCursorPos(ImVec2(550, 200));
	// �摜�擾
	std::shared_ptr<Texture> cannonLeftRotTex = engine.LoadTexture("Res/KeyGuid/tankCannonLeftRotation.tga");
	// �摜�ϊ�
	ImTextureID cannonLeftRotTexId = reinterpret_cast<ImTextureID>(cannonLeftRotTex->GetID());
	// �\��
	ImGui::Image(cannonLeftRotTexId, ImVec2(200, 200));

	//----------------------------------


	//----------------------------------
	// �����

	// �\���ʒu�w��
	ImGui::SetCursorPos(ImVec2(725, 250));
	// �摜�擾
	std::shared_ptr<Texture> leftArrowTex = engine.LoadTexture("Res/KeyGuid/leftArrow.tga");
	// �摜�ϊ�
	ImTextureID leftArrowTexId = reinterpret_cast<ImTextureID>(leftArrowTex->GetID());
	// �摜�\��
	ImGui::Image(leftArrowTexId, ImVec2(75, 75));

	//----------------------------------


	//----------------------------------
	// �}�E�X�̉摜

	// �摜�擾
	std::shared_ptr<Texture> mouseTex = engine.LoadTexture("Res/KeyGuid/mouse.tga");
	// �摜�ϊ�
	ImTextureID mouseTexId = reinterpret_cast<ImTextureID>(mouseTex->GetID());
	// �摜�\��
	ImGui::GetWindowDrawList()->AddImage(mouseTexId, ImVec2(800, 200), ImVec2(1000, 400));

	//----------------------------------

	
	//----------------------------------
	// �E�����

	// �\������ʒu�̎w��
	ImGui::SetCursorPos(ImVec2(830, 200));

	// �摜�擾
	std::shared_ptr<Texture> line = engine.LoadTexture("Res/KeyGuid/�E�΂߉���.tga");
	// �摜�ϊ�
	ImTextureID lineId = reinterpret_cast<ImTextureID>(line->GetID());
	// �摜�\��
	ImGui::Image(lineId, ImVec2(50, 50));

	//----------------------------------


	//----------------------------------
	// �E���

	// �\���ʒu�w��
	ImGui::SetCursorPos(ImVec2(1000, 250));
	// �摜�擾
	std::shared_ptr<Texture> rightArrowTex = engine.LoadTexture("Res/KeyGuid/rightArrow.tga");
	// �摜�ϊ�
	ImTextureID rightArrowTexId = reinterpret_cast<ImTextureID>(rightArrowTex->GetID());
	// �\��
	ImGui::Image(rightArrowTexId, ImVec2(75, 75));

	//----------------------------------


	//----------------------------------
	// �C����]

	// �\���ʒu�ݒ�
	ImGui::SetCursorPos(ImVec2(1050, 200));
	// �摜�擾
	std::shared_ptr<Texture> cannonRightRotTex = engine.LoadTexture("Res/KeyGuid/tankCannonRightRotation.tga");
	// �摜�ϊ�
	ImTextureID cannonRightRotTexId = reinterpret_cast<ImTextureID>(cannonRightRotTex->GetID());
	// �\��
	ImGui::Image(cannonLeftRotTexId, ImVec2(200, 200));

	//----------------------------------


	//----------------------------------
	// �^�C�g���ɖ߂�{�^��

	// �\������ʒu�w��
	ImGui::SetCursorPos(ImVec2(windowSize.x / 2 - 175, windowSize.y - 100));
	// �E�B���h�E�Ŏg�p���镶���̑傫���w��
	ImGui::SetWindowFontScale(5);
	// �{�^���������ꂽ��^�C�g���ɖ߂�
	if (ImGui::Button("Back to Title"))
	{
		SceneManager::Get().LoadScene("Title");
	}

	//----------------------------------

	ImGui::End();
}