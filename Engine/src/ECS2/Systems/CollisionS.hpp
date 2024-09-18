#pragma once

#include "SysBase.hpp"

#include "../Components/AABB.hpp"
#include "../Components/Transform.hpp"

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>

#include "../../Utility/Utility.hpp"
#include "../../Graphics/Camera.hpp"
#include "../../Graphics/GeneralRenderer.hpp"

class CollisionS : public SysBase
{
public:
	struct Collision
	{
		Collision(int ID1 = -1, int ID2 = -1);
		enum class State { entry, stay, exit };
		State state = State::entry;
		int ID1, ID2;
	};

	CollisionS(float cellSize);
	virtual void handleEvent(Event& event) override;
	void update(float dt) override;
	void lateUpdate(float dt) override;
	std::vector<int> pointPick(glm::vec2 point);
	virtual bool addEntity(int ID) override;
	virtual bool addEntity(Entity entity) override;
	CollisionS::Collision& getCollision(int ID1, int ID2);
	void drawGrid(RenderingAPI& rAPI, Camera& camera, float width, glm::vec4 color, bool commisionDrawing);
	void drawCellsWithColliders(RenderingAPI& rAPI, Camera& camera, float width, glm::vec4 color, bool commisionDrawing);

	float cellSize;
private:

	void addToGrid(int ID);
	bool collided(glm::vec2 pos1, glm::vec2 dim1, glm::vec2 pos2, glm::vec2 dim2);
	bool pointCollision(glm::vec2 colPos, glm::vec2 colDim, glm::vec2 point);

	std::unordered_map<int, std::vector<int>> grid;
	std::unordered_map<int, Collision> collisions;
	std::vector<int> collisionToRemove;
	std::vector<int> cellToRemove;

	friend class CollisionResolutionS;
};