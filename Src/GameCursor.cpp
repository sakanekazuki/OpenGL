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
* 初期化
*/
bool GameCursor::Initialize()
{
	if (!cursor)
	{
		cursor = new GameCursor;
		if (!cursor)
		{
			std::cout << "GameCursorの初期化に失敗\n";
			return false;
		}
	}
	// カーソルを真ん中に移動
	glm::vec2 center = GameEngine::Get().GetWindowSize();
	center /= 2;
	GameEngine::Get().SetCursorPos(center);

	return true;
}

/**
* 終了
*/
void GameCursor::Finalize()
{
	delete cursor;
	cursor = nullptr;
}

/**
* GameCursor取得
*/
GameCursor& GameCursor::Get()
{
	return *cursor;
}

/**
* カーソルの表示非表示切り替え
* 
* @param cursorType
* GLFW_CURSOR_NORMAL カーソルが表示され、正常に動作するようになります。
* GLFW_CURSOR_HIDDEN ウィンドウのコンテンツ領域上にあるときにカーソルを非表示にしますが、カーソルが離れることを制限しません。
* GLFW_CURSOR_DISABLED カーソルを非表示にしてつかみ、仮想的で無制限のカーソル移動を提供します。これは、たとえば3Dカメラコントロールを実装する場合に役立ちます。
*/
void GameCursor::SetCurosorStatus(int cursorType)
{
	GameEngine::Get().SetCursorType(cursorType);
}