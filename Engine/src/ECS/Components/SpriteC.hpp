#pragma once

#include "ComponentBase.hpp"
#include "../../Managers/Resource/ResourceManager.hpp"

class SpriteC : public ComponentBase
{
public:
	SpriteC(Resource<Texture>* texture = nullptr);
	~SpriteC();
	virtual nlohmann::json serialize() override;
	virtual void to_json(nlohmann::json& j) const;
	virtual void from_json(nlohmann::json& j);

	void setTexture(Resource<Texture>* texture);
	Texture* getTexture();
protected:
	Resource<Texture>* texture;
};