#pragma once

#include "../Components/ComponentBase.hpp"
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

enum ETag
{
	Default
};

class Ent
{
public:
	Ent(int ID, ETag tag = Default) { this->ID = ID; this->tag = tag; }
	virtual ~Ent();
	int getID() { return ID; }
	ETag getTag() { return tag; }
	void destroy();
	bool isDestroyed() { return deleted; }

	void to_json(nlohmann::json& j) const;
	void from_json(nlohmann::json& j);

	bool addComponent(ComponentBase* component);
	void removeComponent(CType type);
	bool hasComponent(CType type);
	ComponentBase* getComponent(CType type);
	void deleteAllComponents();
private:
	ETag tag = Default;
	int ID;
	bool deleted = false;
	std::unordered_map<int, ComponentBase*> components;

	friend class EntityPropertiesMenu;
};
