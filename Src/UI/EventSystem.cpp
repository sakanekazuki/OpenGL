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
* 初期化
*/
bool EventSystem::Initialize()
{
	// イベントシステムが無ければ作成
	if (!eventsystem)
	{
		eventsystem = new EventSystem;
		// 作成してもなければ作成失敗
		if (!eventsystem)
		{
			std::cout << "イベントシステム作成に失敗\n";
			return false;
		}
	}

	return true;
}

/**
* 終了
*/
void  EventSystem::Finalize()
{
	// イベントシステムがあれば消去
	if (eventsystem)
	{
		delete eventsystem;
		eventsystem = nullptr;
	}
}

/**
* EventSystemの取得
*/
EventSystem& EventSystem::Get()
{
	return *eventsystem;
}

/**
* 更新
*/
void EventSystem::Update(float deltaTime)
{
	if (eventsystem->buttons.size() <= focus)
		return;

	GameEngine& engine = GameEngine::Get();
	// カーソルの位置を画面の位置に合わせる
	glm::vec2 cursorPos = engine.GetCursorPosition();
	cursorPos.x -= (engine.GetWindowSize().x / 2);
	cursorPos.y -= (engine.GetWindowSize().y / 2);
	// カーソルのY軸を反転
	cursorPos.y *= -1;

	for (int i = 0; i < eventsystem->buttons.size(); ++i)
	{
		Box box = static_cast<Box&>(*buttons[i]->collider);

		// ボタンの当たり判定の位置を取得
		glm::vec3 min = buttons[i]->position + box.min;
		glm::vec3 max = buttons[i]->position + box.max;

		// ボタンに当たっているかを調べる
		if (min.x < cursorPos.x && min.y < cursorPos.y)
		{
			if (max.x > cursorPos.x && max.y > cursorPos.y)
			{
				// カーソルが合っている場合Hoveredを呼ぶ
				buttons[i]->Hovered();
				// フォーカスを設定
				focus = i;
				// カーソルが合っていてクリックされたらOnClickを呼ぶ
				if (engine.GetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
					buttons[i]->OnClick();
				break;
			}
			else
				if (focus == i)
					// カーソルが外れた場合Releaseを呼ぶ
					buttons[i]->Release();
		}
		else
			if (focus == i)
				// カーソルが外れた場合Releaseを呼ぶ
				buttons[i]->Release();
	}	
}

/**
* UIの更新
*/
void EventSystem::ResetUI()
{
	ui = GameEngine::Get().GetActors(Layer::UI);
	
	// 一度ボタンを全て消す
	buttons.erase(buttons.begin(), buttons.end());

	// ButtonをActorからUIにダウンキャスト
	for (int i = 0; i < eventsystem->ui.size(); ++i)
	{
		auto p = std::dynamic_pointer_cast<Button>(eventsystem->ui[i]);
		if (p)
		{
			eventsystem->buttons.push_back(p);
		}
	}
}