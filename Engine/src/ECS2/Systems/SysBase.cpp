#include "SysBase.hpp"
#include "../ComponentPoolManager.hpp"

SysBase::~SysBase()
{
}

void SysBase::handleEvent(Event& event)
{
    switch (event.getType())
    {
    case Event::EntityRemoval:
    {
        int* ID = (int*)event.getPayload();
        this->removeEntity(*ID);
    }
    break;
    }
}

bool SysBase::addEntity(int ID)
{
    if (entities.find(ID) != entities.end())
        return false;

    for (auto& compType : requiredComponents)
    {
        if(!ComponentPoolManager::getInstance().hasComponentTID(ID, compType))
            return false;
    }

    entities.insert(ID);
    return true;
}

bool SysBase::addEntity(Entity entity)
{
    return addEntity(entity.getID());
}

void SysBase::removeEntity(int ID)
{
    entities.erase(ID);
}

void SysBase::lateUpdate(float dt)
{
}

const std::unordered_set<int>& SysBase::getEntitySet()
{
    return entities;
}

bool SysBase::entityIsIncluded(int ID)
{
    if (entities.find(ID) != entities.end())
        return true;
    else
        return false;
}

SysBase::SysBase(bool deleteOnSceneEnd) : deleteOnSceneEnd(deleteOnSceneEnd), EventObserver(ECS2)
{
}
