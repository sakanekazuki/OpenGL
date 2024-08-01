/**
* @file GameCursor.h
*/

#ifndef GAMECURSOR_H_INCLUDE
#define GAMECURSOR_H_INCLUDE

#include "GameEngine.h"

class GameCursor
{
public:
	static bool Initialize();
	static void Finalize();
	static GameCursor& Get();

	void SetCurosorStatus(int cursorType);

	/**
	* �J�[�\���̈ʒu�̎擾
	*
	* @retval �J�[�\���̈ʒu
	*/
	glm::vec2 GetCursorPos()
	{
		// ���݂̃J�[�\���̈ʒu���擾
		cursorPos = GameEngine::Get().GetCursorPosition();
		return cursorPos;
	}

	/**
	* �J�[�\���̓����������̎擾
	*/
	glm::vec2 GetCursorMovePos()
	{
		// �O��̃J�[�\���̈ʒu����
		lastCursorPos = cursorPos;
		// ���݂̃J�[�\���̈ʒu����
		cursorPos = GetCursorPos();

		return cursorPos - lastCursorPos;
	}

private:
	GameCursor() = default;
	~GameCursor() = default;
	GameCursor(const GameCursor&) = delete;
	GameCursor& operator=(const GameCursor&) = delete;

	// �J�[�\���̈ʒu
	glm::vec2 cursorPos = glm::vec2(0, 0);
	// �O��̈ʒu
	glm::vec2 lastCursorPos = glm::vec2(0, 0);
	// �J�[�\���̏��
	int cursorType = 0;

};

#endif // GAMECURSOR_H_INCLUDE