#pragma once

#include <nlohmann/json.hpp>

#include "../Components/Transform.hpp"
#include "../Components/Velocity.hpp"
#include "../Components/Sprite.hpp"
#include "../Components/AABB.hpp"
#include "../Components/Animation.hpp"
#include "../Components/Counter.hpp"

namespace ComponentSerialization
{
	static std::unordered_map<std::type_index, std::string> typeIdentifierMap = std::unordered_map<std::type_index, std::string>();

	static bool addPairing(std::type_index type, std::string identifier)
	{
		if (typeIdentifierMap.find(type) != typeIdentifierMap.end())
			return false;

		typeIdentifierMap[type] = identifier;

		return true;
	}
}

template<typename T>
struct ComponentSerializationTraits
{
	static nlohmann::json serialize(T& c);
	static void deSerialize(T& c, nlohmann::json j);
};

template<typename T>
inline nlohmann::json ComponentSerializationTraits<T>::serialize(T& c)
{
	return nlohmann::json();
}

template<typename T>
inline void ComponentSerializationTraits<T>::deSerialize(T& c, nlohmann::json j)
{
	
}

template<>
struct ComponentSerializationTraits<Transform>
{
	static nlohmann::json serialize(Transform& c);
	static void deSerialize(Transform& c, nlohmann::json j);
};

inline nlohmann::json ComponentSerializationTraits<Transform>::serialize(Transform& c)
{
	nlohmann::json j;
	j["type"] = CBase::Transform;

	j["position"] = { c.x, c.y };
	j["size"] = { c.width, c.height };
	j["rot"] = c.rot;
	j["rotateToDir"] = c.rotateToDir;

	return j;
}

inline void ComponentSerializationTraits<Transform>::deSerialize(Transform& c, nlohmann::json j)
{
	c.x = j["position"][0];
	c.y = j["position"][1];
	c.width = j["size"][0];
	c.height = j["size"][1];
	c.rot = j["rot"];
	c.rotateToDir = j["rotateToDir"];
}


template<>
struct ComponentSerializationTraits<Velocity>
{
	static nlohmann::json serialize(Velocity& c);
	static void deSerialize(Velocity& c, nlohmann::json j);
};

inline nlohmann::json ComponentSerializationTraits<Velocity>::serialize(Velocity& c)
{
	nlohmann::json j;
	j["type"] = CBase::Velocity;

	j["velocity"] = { c.x, c.y };
	j["acceleration"] = { c.ax, c.ay };
	j["decay"] = c.decay;

	return j;
}

inline void ComponentSerializationTraits<Velocity>::deSerialize(Velocity& c, nlohmann::json j)
{
	c.x = j["velocity"][0];
	c.y = j["velocity"][1];
	c.ax = j["acceleration"][0];
	c.ay = j["acceleration"][1];
	c.decay = j["decay"];
}

template<>
struct ComponentSerializationTraits<Sprite>
{
	static nlohmann::json serialize(Sprite& c);
	static void deSerialize(Sprite& c, nlohmann::json j);
};

inline nlohmann::json ComponentSerializationTraits<Sprite>::serialize(Sprite& c)
{
	nlohmann::json j;
	j["type"] = CBase::Sprite;

	j["texture"] = c.getTexture()->getContents()->getFilepath();
	j["textureSample"] = { c.textureSample.x, c.textureSample.y , c.textureSample.z , c.textureSample.w };
	j["flipHorizontally"] = c.flipHorizontally;

	return j;
}

inline void ComponentSerializationTraits<Sprite>::deSerialize(Sprite& c, nlohmann::json j)
{
	c.setTexture(ResourceManager::getInstance().getResource<Texture>(j["texture"]));
	c.textureSample = { j["textureSample"][0], j["textureSample"][1] , j["textureSample"][2] , j["textureSample"][3] };
	c.flipHorizontally = j["flipHorizontally"];
}

template<>
struct ComponentSerializationTraits<AABB>
{
	static nlohmann::json serialize(AABB& c);
	static void deSerialize(AABB& c, nlohmann::json j);
};

inline nlohmann::json ComponentSerializationTraits<AABB>::serialize(AABB& c)
{
	nlohmann::json j;
	j["type"] = CBase::AABB;

	j["size"] = { c.width, c.height };
	j["mass"] = c.mass;
	j["enabled"] = c.enabled;

	return j;
}

inline void ComponentSerializationTraits<AABB>::deSerialize(AABB& c, nlohmann::json j)
{
	c.width = j["size"][0];
	c.height = j["size"][1];
	c.mass = j["mass"];
	c.enabled = j["enabled"];

	c.collidingEntIDs.clear();
}

template<>
struct ComponentSerializationTraits<Animation>
{
	static nlohmann::json serialize(Animation& c);
	static void deSerialize(Animation& c, nlohmann::json j);
};

inline nlohmann::json ComponentSerializationTraits<Animation>::serialize(Animation& c)
{
	nlohmann::json j;
	j["type"] = CBase::Animation;

	j["frameDuration"] = c.frameDuration;
	j["state"] = c.state;
	j["currentFrame"] = c.currentFrame;

	j["frames"];
	for (auto& item : c.framesMap)
	{
		j["frames"][std::to_string(item.first)] = nlohmann::json::array();
		for (auto& division : item.second)
		{
			j["frames"][std::to_string(item.first)].push_back({ division.x, division.y, division.z, division.w });
		}
	}

	return j;
}

inline void ComponentSerializationTraits<Animation>::deSerialize(Animation& c, nlohmann::json j)
{
	c.counter = 0;
	c.frameDuration = j["frameDuration"];
	c.state = j["state"];
	c.currentFrame = j["currentFrame"];

	//not finished
}

template<>
struct ComponentSerializationTraits<Counter>
{
	static nlohmann::json serialize(Counter& c);
	static void deSerialize(Counter& c, nlohmann::json j);
};

inline nlohmann::json ComponentSerializationTraits<Counter>::serialize(Counter& c)
{
	nlohmann::json j;
	j["type"] = CBase::Counter;

	j["val"] = c.val;
	j["maxValue"] = c.maxValue;

	return j;
}

inline void ComponentSerializationTraits<Counter>::deSerialize(Counter& c, nlohmann::json j)
{
	c.val = j["val"];
	c.maxValue = j["maxValue"];
}