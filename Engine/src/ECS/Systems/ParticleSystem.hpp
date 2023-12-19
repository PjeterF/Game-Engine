#pragma once

#include "SystemBase.hpp"
#include "../../Graphics/GeneralRenderer.hpp"

class ParticleSystem : public SystemBase
{
public:
	static ParticleSystem& getInstance();

	virtual void to_json(nlohmann::json& j) const;
	virtual void from_json(nlohmann::json& j);

	virtual void update(float dt) override;
private:
	ParticleSystem();
};