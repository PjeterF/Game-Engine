#pragma once

#include "Event.hpp"
#include <vector>

class EventObserver
{
public:
	EventObserver(ObsBin bin = ObsBin::Defualt);
	~EventObserver();
	virtual void handleEvent(Event& event)=0;
private:
	std::vector<ObsBin> bins;

	friend class EventManager;
};

