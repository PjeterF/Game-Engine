#include "Entity.hpp"
#include "../Managers/EntityManager.hpp"

Entity::Entity(int layer)
{
    this->layer = layer;
    this->ID = generateID();
    EntityManager::getInstance().addEntity(this, layer);
}

Entity::~Entity()
{
    EntityManager::getInstance().removeEntity(this->getID());
}

void Entity::destroy()
{
    this->pendingDelete = true;
}

bool Entity::setLayer(int targetLayer)
{
    if (layer >= 0 && layer < EntityManager::getInstance().layers.size())
    {
        layer = targetLayer;
        EntityManager::getInstance().moveEntityToLayer(this->getID(), targetLayer);
        return true;
    }
    else
    {
        return false;
    }
}

int Entity::getID()
{
    return ID;
}

int Entity::generateID()
{
    static int ID_gen = 0;
    return ID_gen++;
}
