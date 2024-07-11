#include "SysBase.hpp"
#include "../ComponentPoolManager.hpp"

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

SysBase::SysBase(bool deleteOnSceneEnd) : deleteOnSceneEnd(deleteOnSceneEnd), EventObserver(ECS2)
{
}
