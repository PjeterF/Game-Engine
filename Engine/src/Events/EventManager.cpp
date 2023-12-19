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

void EventManager::notifyAllBins(Event event)
{
	for (auto& bin : observerBins)
	{
		for (auto& observer : bin.second)
		{
			observer->handleEvent(event);
		}
	}
}

void EventManager::addObserver(EventObserver* observer, ObsBin obsBin)
{
	observerBins[obsBin].push_back(observer);
	observer->bins.push_back(obsBin);
}
void EventManager::removeObserver(EventObserver* observer, ObsBin obsBin)
{
	auto it = std::find(observer->bins.begin(), observer->bins.end(), obsBin);
	if(it!=observer->bins.end())
		observer->bins.erase(it);
	observerBins[obsBin].remove(observer);
}