#pragma once

#include"../Entity/Ent.hpp"
#include <unordered_map>
#include <string>
#include "../../Events/EventManager.hpp"

enum SystemSet
{
	PAUSED,
	UNPAUSED
};

enum SystemType
{
	DefaultSys = 0,
	CollisionSys,
	MovementSys,
	LayeredRenderingSys,
	ParticleSys,
	CounterKillerSys,
	
	//Zuma
	ShooterManagementSys,
	RouteManagementSys
};

class SystemBase : public EventObserver
{
public:
	SystemBase(SystemSet set = UNPAUSED, bool permanent = false, int type = DefaultSys);
	virtual ~SystemBase();

	virtual void to_json(nlohmann::json& j) const;
	virtual void from_json(nlohmann::json& j);

	virtual void earlyUpdate(float dt);
	virtual void update(float dt) = 0;
	virtual void lateUpdate(float dt);

	virtual bool addEntity(Ent* entity);
	virtual void removeEntity(int ID);
	virtual void handleEvent(Event& event) override;

	int getID();
	std::string getName();
	int getType();
	bool isPermanent();
protected:
	bool validateComponents(Ent* entity);

	std::string name;
	bool permanent;

	static int nextID;
	int ID;
	int type;

	std::vector<Ent*> entities;
	std::vector<CType> requiredComponents;
};