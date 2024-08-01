/**
* @file EventSystem.h
*/

#ifndef EVENTSYSTEM_H_INCLUDE
#define EVENTSYSTEM_H_INCLUDE

#include "../Actor/Actor.h"
#include "../Actor/UI/UI.h"
#include <map>
#include <vector>
#include <memory>

/**
* UIÇä«óùÇ∑ÇÈÉNÉâÉX
*/
class EventSystem
{
public:
	static bool Initialize();
	static void Finalize();
	static EventSystem& Get();

	void Update(float deltaTime);
	void ResetUI();

private:
	EventSystem() = default;
	~EventSystem() = default;
	EventSystem(const EventSystem&) = delete;
	EventSystem& operator=(const EventSystem&) = delete;

	std::vector<std::shared_ptr<Actor>> ui;

	int focus = 0;
	std::vector<std::shared_ptr<UI>> buttons;

};

#endif // EVENTSYSTEM_H_INCLUDE