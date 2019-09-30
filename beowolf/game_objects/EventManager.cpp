#include "EventManager.h"
#include <algorithm>

void EventManager::QueueEvent(EventDetails* details)
{
	if (m_listeners.find(details->id) == m_listeners.end())
	{
		m_listeners[details->id] = std::vector<Common::ComponentBase*>();
	}

	m_eventQueue.push_back(details);
}

void EventManager::TriggerEvent(EventDetails* details)
{
	for (int i = 0; i < m_listeners[details->id].size(); i++)
	{
		m_listeners[details->id][i]->HandleEvent((void*)details);
	}
}

void EventManager::Update(double delta)
{
	for (int i = 0; i < m_eventQueue.size(); i++)
	{
		m_eventQueue[i]->time -= delta;
		if (m_eventQueue[i]->time <= 0)
		{
			TriggerEvent(m_eventQueue[i]);
			m_eventQueue.erase(m_eventQueue.begin() + i);
			i--; // quick adjustment to offset deletion
		}
	}
}

void EventManager::AddListener(std::string id, Common::ComponentBase* listener)
{
	if (m_listeners.find(id) == m_listeners.end())
	{
		m_listeners[id] = std::vector<Common::ComponentBase*>();
	}

	m_listeners[id].push_back(listener);
}

void EventManager::RemoveListener(std::string id, Common::ComponentBase* listener)
{
	m_listeners[id].erase(std::remove(m_listeners[id].begin(), m_listeners[id].end(), listener), m_listeners[id].end());
}