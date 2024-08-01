/**
* @file Title.cpp
*/

#include "Title.h"
#include "../GameEngine.h"
#include "../Actor/UI/Button.h"
#include "../SceneManager.h"
#include "../Actor/UI/Image.h"

/**
* �R���X�g���N�^
*/
Title::Title()
	:Scene(TITLESCENE,"")
{
	// �Q�[���G���W���擾
	GameEngine& engine = GameEngine::Get();
	// �w�i�ݒ�
	engine.SetglClearColor(glm::vec3(0, 0, 0));
	// �J�[�\���̕\��
	engine.SetCursorType(GLFW_CURSOR_NORMAL);
}

/**
* �X�V
*/
void Title::Update(float deltaTime)
{
	// �Q�[���G���W���̎擾
	GameEngine& engine = GameEngine::Get();
	// �E�B���h�E�̑傫�����擾
	glm::vec2 windowSize = engine.GetWindowSize();
	// �V�[���}�l�[�W���[�擾
	SceneManager& sceneManager = SceneManager::Get();

	//------------------------------------------
	// �X�^�[�g�{�^���\��

	// �E�B���h�E�̑傫���w��
	ImGui::SetNextWindowSize(ImVec2(196.0f, 80.0f));
	// �E�B���h�E�̈ʒu�w��
	ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 95, windowSize.y / 2 - 200));
	//ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 95, windowSize.y / 2 - 150));

	// �E�B���h�E�̏����ݒ�
	ImGui::Begin("Button0", nullptr,
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	// �E�B���h�E�Ŏg�p����t�H���g�̑傫���w��
	ImGui::SetWindowFontScale(5);
	// �{�^���������ꂽ��Q�[���X�^�[�g
	if (ImGui::Button("Start"))
	{
		sceneManager.LoadScene("MainGame");
	}
	ImGui::End();

	//------------------------------------------


	//------------------------------------------
	// �L�[�K�C�h�Ɉړ�����{�^��

	// �E�B���h�E�̑傫���w��
	ImGui::SetNextWindowSize(ImVec2(300, 80.0f));
	// �E�B���h�E�̈ʒu�w��
	ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 130, windowSize.y / 2-50));
	// �E�B���h�E�̏����ݒ�
	ImGui::Begin("Button1", nullptr,
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	// �E�B���h�E�Ŏg�p����t�H���g�̑傫���w��
	ImGui::SetWindowFontScale(5);
	// �{�^���������ꂽ��Q�[���X�^�[�g
	if (ImGui::Button("KeyGuid"))
	{
		sceneManager.LoadScene("Tutorial");
	}
	ImGui::End();

	//------------------------------------------

	//------------------------------------------
	// �I���{�^���\��

	// �E�B���h�E�̑傫���w��
	ImGui::SetNextWindowSize(ImVec2(196.0f, 80.0f));
	// �E�B���h�E�̈ʒu�w��
	ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 80, windowSize.y / 2 + 100));
	//ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 80, windowSize.y / 2 + 50));


	// �E�B���h�E�̏����ݒ�
	ImGui::Begin("Button2", nullptr,
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
	// �E�B���h�E�Ŏg�p����t�H���g�̑傫���w��
	ImGui::SetWindowFontScale(5);
	// �{�^���������ꂽ��Q�[���I��
	if (ImGui::Button("Quit"))
	{
		engine.WindowClose();
	}
	ImGui::End();
	//------------------------------------------
}