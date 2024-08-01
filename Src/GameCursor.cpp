/**
* @file GmaeCursor.cpp
*/

#include "GameCursor.h"
#include <iostream>

namespace
{
	GameCursor* cursor = nullptr;
}

/**
* ������
*/
bool GameCursor::Initialize()
{
	if (!cursor)
	{
		cursor = new GameCursor;
		if (!cursor)
		{
			std::cout << "GameCursor�̏������Ɏ��s\n";
			return false;
		}
	}
	// �J�[�\����^�񒆂Ɉړ�
	glm::vec2 center = GameEngine::Get().GetWindowSize();
	center /= 2;
	GameEngine::Get().SetCursorPos(center);

	return true;
}

/**
* �I��
*/
void GameCursor::Finalize()
{
	delete cursor;
	cursor = nullptr;
}

/**
* GameCursor�擾
*/
GameCursor& GameCursor::Get()
{
	return *cursor;
}

/**
* �J�[�\���̕\����\���؂�ւ�
* 
* @param cursorType
* GLFW_CURSOR_NORMAL �J�[�\�����\������A����ɓ��삷��悤�ɂȂ�܂��B
* GLFW_CURSOR_HIDDEN �E�B���h�E�̃R���e���c�̈��ɂ���Ƃ��ɃJ�[�\�����\���ɂ��܂����A�J�[�\��������邱�Ƃ𐧌����܂���B
* GLFW_CURSOR_DISABLED �J�[�\�����\���ɂ��Ă��݁A���z�I�Ŗ������̃J�[�\���ړ���񋟂��܂��B����́A���Ƃ���3D�J�����R���g���[������������ꍇ�ɖ𗧂��܂��B
*/
void GameCursor::SetCurosorStatus(int cursorType)
{
	GameEngine::Get().SetCursorType(cursorType);
}