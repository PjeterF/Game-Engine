#pragma once

#include "Event.hpp"
#include "EventObserver.hpp"
#include <list>
#include <unordered_map>

class EventManager
{
public:
	EventManager() {}
	static EventManager& getInstance()
	{
		static EventManager instance;
		return instance;
	}
	void notify(Event event, ObsBin obsBin = ObsBin::Defualt);
	void notifyAllBins(Event event);
	void addObserver(EventObserver* observer, ObsBin obsBin = ObsBin::Defualt);
	void removeObserver(EventObserver* observer, ObsBin obsBin = ObsBin::Defualt);
private:
	std::unordered_map<ObsBin, std::list<EventObserver*>> observerBins;
};

