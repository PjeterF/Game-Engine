#pragma once

#include "UIWindow.hpp"

#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <array>
#include <string>

#include <glm/glm.hpp>

class MarbleEditor : public UIWindow
{
public:
	MarbleEditor(std::string name, int x, int y, int width, int height, std::string archetypeDirectoryPath);
	virtual void render() override;
private:
	std::string archetypeDirectoryPath;
	std::string selectedArchetypePath="";
	char newName[100];
	nlohmann::json j;
	std::ifstream file;

	struct MarbleArchetype
	{
		MarbleArchetype(std::array<float, 3> color = {0, 0, 0}, float frameDuration = 1, float size = 0, int type = -1, std::string texture = "", std::vector<glm::vec4> frames = {})
			: frameDuration(frameDuration), size(size), type(type), texture(texture), frames(frames)
		{
			this->color[0] = color[0];
			this->color[1] = color[1];
			this->color[2] = color[2];
		}

		nlohmann::json serialize();

		float color[3];
		float frameDuration;
		float size;
		std::string texture;
		int type;
		std::vector<glm::vec4> frames;
	};

	MarbleArchetype marbleArchetype;
};