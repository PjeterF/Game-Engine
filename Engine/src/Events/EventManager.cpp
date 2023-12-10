#include "EventManager.hpp"

void EventManager::notify(Event event, ObsBin obsBin)
{
	auto it = observerBins[obsBin].begin();
	while (it != observerBins[obsBin].end())
	{
		(*it)->handleEvent(event);
		it++;
	}
}

void EventManager::addObserver(EventObserver* observer, ObsBin obsBin)
{
	observerBins[obsBin].push_back(observer);
	observer->bins.push_back(obsBin);
}
void EventManager::removeObserver(EventObserver* observer, ObsBin obsBin)
{
	observerBins[obsBin].remove(observer);
	observer->bins.erase(std::find(observer->bins.begin(), observer->bins.end(), obsBin));
}