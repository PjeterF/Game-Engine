#include "EventObserver.hpp"
#include "EventManager.hpp"

EventObserver::EventObserver(ObsBin bin)
{
	EventManager::getInstance().addObserver(this, bin);
	bins.push_back(bin);
}

EventObserver::~EventObserver()
{
	for (auto& bin : bins)
		EventManager::getInstance().removeObserver(this, bin);
}
