#include <vector>
#include <map>
#include "EventDetails.h"
#include "GameObject.h"

class EventManager
{
public:
	static EventManager& getInstance()
	{
		static EventManager instance;
		return instance;
	}

	void QueueEvent(EventDetails* details);
	void TriggerEvent(EventDetails* details);
	void Update(double delta);
	void AddListener(std::string id, Common::ComponentBase* listener);
	void RemoveListener(std::string id, Common::ComponentBase* listener);

private:
	EventManager() {}

public:
	EventManager(EventManager const&) = delete;
	void operator=(EventManager const&) = delete;

private:
	std::vector<EventDetails*> m_eventQueue;
	std::map<std::string, std::vector<Common::ComponentBase*>> m_listeners;
};