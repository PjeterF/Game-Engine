#pragma once

#include "SysBase.hpp"

#include "../Components/AABB.hpp"
#include "../Components/Transform.hpp"

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>

#include "../../Utility/Utility.hpp"

class CollisionS : public SysBase
{
public:
	static void initialize(float cellSize);
	static CollisionS& getInstance();
	void update(float dt) override;
	void lateUpdate(float dt) override;
	void updateResponse(float dt);
	std::vector<int> pointPick(glm::vec2 point);
	virtual bool addEntity(int ID) override;
	virtual bool addEntity(Entity entity) override;
private:
	struct Collision
	{
		Collision(int ID1=-1, int ID2=-1);
		enum class State{entry, stay, exit};
		State state=State::entry;
		int ID1, ID2;
	};

	CollisionS(float cellSize);
	static CollisionS& instanceImp(float cellSize);

	void addToGrid(int ID);
	bool collided(glm::vec2 pos1, glm::vec2 dim1, glm::vec2 pos2, glm::vec2 dim2);
	float cellSize;
	int framesToProcess = 4;

	std::unordered_map<int, std::vector<int>> grid;
	std::unordered_map<int, Collision> collisions;
	std::vector<int> collisionToRemove;
	std::vector<int> cellToRemove;

	friend class CollisionResolutionS;
};