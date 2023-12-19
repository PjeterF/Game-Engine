#pragma once

#include "SystemBase.hpp"
#include "../Components/BoxColliderC.hpp"
#include "../Components/TransformC.hpp"
#include "../../Graphics/GeneralRenderer.hpp"
#include "../../Utility/Utility.hpp"
#include <unordered_map>
#include <queue>

class CollisionECS
{
public:
	enum State
	{
		Enter, Stay, Exit
	};
	CollisionECS(BoxColliderC* c1, BoxColliderC* c2);
	~CollisionECS();
	int getID();
	BoxColliderC* getOtherCollider(int thisColID);

	BoxColliderC* c1, * c2;
	State state = Enter;
private:
	int ID;
};

class CollisionSystem : public SystemBase
{
public:
	static CollisionSystem& getInstance();
	static void initialize(int x, int y, int dimX, int dimY, float cellSize);
	static void reInitialize(int x, int y, int dimX, int dimY, float cellSize);

	virtual void to_json(nlohmann::json& j) const override;
	virtual void from_json(nlohmann::json& j) override;

	virtual void removeEntity(int ID) override;
	virtual void earlyUpdate(float dt) override;
	virtual void update(float dt) override;
	virtual void lateUpdate(float dt) override;
	virtual void handleEvent(Event& event) override;

	void checkCollisions();

	virtual bool addEntity(Ent* entity) override;

	std::unordered_map<int, CollisionECS*>& getCollisions();
	CollisionECS* getCollision(int collisionID);
	CollisionECS* getCollision(int colliderID1, int colliderID2);

	void drawGrid(RenderingAPI* API);
	void drawColliders(RenderingAPI* API);
private:
	static CollisionSystem& instanceInplementation(int x = 0, int y = 0, int dimX = 0, int dimY = 0, float cellSize = 0);
	CollisionSystem(int x, int y, int dimX, int dimY, float cellSize);

	bool checkCollision(BoxColliderC* c1, BoxColliderC* c2);
	glm::ivec2 getIndex(float x, float y);
	void insertToGrid(BoxColliderC* collider);
	void removeFromGrid(BoxColliderC* collider);
	void clearGrid();
	void insertCollidersOfStoredEntities();

	float cellSize;
	int gridDimX, gridDimY;
	float x, y;

	int nextColID = 0;

	std::vector<std::list<BoxColliderC*>> grid;
	std::unordered_map<int, CollisionECS*> collisions;
};