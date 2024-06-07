#pragma once

#include "SysBase.hpp"

#include "../Components/AABB.hpp"
#include "../Components/Transform.hpp"

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "../../Utility/Utility.hpp"

class CollisionS : public SysBase
{
public:
	static void initialize(float x, float y, float cellSize);
	static CollisionS& getInstance();
	void update(float dt) override;
	void lateUpdate(float dt) override;
	virtual bool addEntity(int ID) override;
	virtual bool addEntity(Entity entity) override;
private:
	struct Collision
	{
		Collision(int otherID);
		enum class State{entry, stay, exit};
		State state=State::entry;
		int otherID;

	};

	CollisionS(float x, float y, float cellSize);
	static CollisionS& instanceImp(float x, float y, float cellSize);

	void addToGrid(int ID);
	bool collided(glm::vec2 pos1, glm::vec2 dim1, glm::vec2 pos2, glm::vec2 dim2);
	float x, y, cellSize;

	std::unordered_map<int, std::unordered_set<int>> grid;
	std::unordered_map<int, std::unordered_set<int>> collisions;
};