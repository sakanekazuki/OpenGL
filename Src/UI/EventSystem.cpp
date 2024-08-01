/**
* @file EventSystem.cpp
*/

#include "EventSystem.h"
#include "../GameEngine.h"
#include "../Actor/UI/Button.h"
#include <iostream>

namespace
{
	EventSystem* eventsystem = nullptr;
}

/**
* ������
*/
bool EventSystem::Initialize()
{
	// �C�x���g�V�X�e����������΍쐬
	if (!eventsystem)
	{
		eventsystem = new EventSystem;
		// �쐬���Ă��Ȃ���΍쐬���s
		if (!eventsystem)
		{
			std::cout << "�C�x���g�V�X�e���쐬�Ɏ��s\n";
			return false;
		}
	}

	return true;
}

/**
* �I��
*/
void  EventSystem::Finalize()
{
	// �C�x���g�V�X�e��������Ώ���
	if (eventsystem)
	{
		delete eventsystem;
		eventsystem = nullptr;
	}
}

/**
* EventSystem�̎擾
*/
EventSystem& EventSystem::Get()
{
	return *eventsystem;
}

/**
* �X�V
*/
void EventSystem::Update(float deltaTime)
{
	if (eventsystem->buttons.size() <= focus)
		return;

	GameEngine& engine = GameEngine::Get();
	// �J�[�\���̈ʒu����ʂ̈ʒu�ɍ��킹��
	glm::vec2 cursorPos = engine.GetCursorPosition();
	cursorPos.x -= (engine.GetWindowSize().x / 2);
	cursorPos.y -= (engine.GetWindowSize().y / 2);
	// �J�[�\����Y���𔽓]
	cursorPos.y *= -1;

	for (int i = 0; i < eventsystem->buttons.size(); ++i)
	{
		Box box = static_cast<Box&>(*buttons[i]->collider);

		// �{�^���̓����蔻��̈ʒu���擾
		glm::vec3 min = buttons[i]->position + box.min;
		glm::vec3 max = buttons[i]->position + box.max;

		// �{�^���ɓ������Ă��邩�𒲂ׂ�
		if (min.x < cursorPos.x && min.y < cursorPos.y)
		{
			if (max.x > cursorPos.x && max.y > cursorPos.y)
			{
				// �J�[�\���������Ă���ꍇHovered���Ă�
				buttons[i]->Hovered();
				// �t�H�[�J�X��ݒ�
				focus = i;
				// �J�[�\���������Ă��ăN���b�N���ꂽ��OnClick���Ă�
				if (engine.GetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
					buttons[i]->OnClick();
				break;
			}
			else
				if (focus == i)
					// �J�[�\�����O�ꂽ�ꍇRelease���Ă�
					buttons[i]->Release();
		}
		else
			if (focus == i)
				// �J�[�\�����O�ꂽ�ꍇRelease���Ă�
				buttons[i]->Release();
	}	
}

/**
* UI�̍X�V
*/
void EventSystem::ResetUI()
{
	ui = GameEngine::Get().GetActors(Layer::UI);
	
	// ��x�{�^����S�ď���
	buttons.erase(buttons.begin(), buttons.end());

	// Button��Actor����UI�Ƀ_�E���L���X�g
	for (int i = 0; i < eventsystem->ui.size(); ++i)
	{
		auto p = std::dynamic_pointer_cast<Button>(eventsystem->ui[i]);
		if (p)
		{
			eventsystem->buttons.push_back(p);
		}
	}
}