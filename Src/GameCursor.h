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
	* カーソルの位置の取得
	*
	* @retval カーソルの位置
	*/
	glm::vec2 GetCursorPos()
	{
		// 現在のカーソルの位置を取得
		cursorPos = GameEngine::Get().GetCursorPosition();
		return cursorPos;
	}

	/**
	* カーソルの動いた距離の取得
	*/
	glm::vec2 GetCursorMovePos()
	{
		// 前回のカーソルの位置を代入
		lastCursorPos = cursorPos;
		// 現在のカーソルの位置を代入
		cursorPos = GetCursorPos();

		return cursorPos - lastCursorPos;
	}

private:
	GameCursor() = default;
	~GameCursor() = default;
	GameCursor(const GameCursor&) = delete;
	GameCursor& operator=(const GameCursor&) = delete;

	// カーソルの位置
	glm::vec2 cursorPos = glm::vec2(0, 0);
	// 前回の位置
	glm::vec2 lastCursorPos = glm::vec2(0, 0);
	// カーソルの状態
	int cursorType = 0;

};

#endif // GAMECURSOR_H_INCLUDE