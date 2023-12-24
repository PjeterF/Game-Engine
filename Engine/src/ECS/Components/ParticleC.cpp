#include "ParticleC.hpp"

ParticleC::ParticleC(float x, float y, int maxParticles, int emitPeriod, int emitCount) : ComponentBase(Particle)
{
	emitter = ParticeEmitter(x, y, maxParticles);
	this->emitPeriod = emitPeriod;
	this->emitCount = emitCount;
}

void ParticleC::to_json(nlohmann::json& j) const
{
	ComponentBase::to_json(j);
	j["emitPeriod"] = emitPeriod;
	j["emitCount"] = emitCount;
	j["emitter"]["maxParticles"] = emitter.getParticleLimit();

	j["emitter"]["defualtProperties"]["startColour"] = {
		emitter.defaultProperties.startColour.x,
		emitter.defaultProperties.startColour.y,
		emitter.defaultProperties.startColour.z,
		emitter.defaultProperties.startColour.w
	};
	j["emitter"]["defualtProperties"]["endColour"] = {
		emitter.defaultProperties.endColour.x,
		emitter.defaultProperties.endColour.y,
		emitter.defaultProperties.endColour.z,
		emitter.defaultProperties.endColour.w
	};
	j["emitter"]["defualtProperties"]["startSize"] = emitter.defaultProperties.startSize;
	j["emitter"]["defualtProperties"]["endSize"] = emitter.defaultProperties.endSize;
	j["emitter"]["defualtProperties"]["lifetime"] = {
		emitter.defaultProperties.particleLifetime.x,
		emitter.defaultProperties.particleLifetime.y,
	};
	j["emitter"]["defualtProperties"]["rotVar"] = {
		emitter.defaultProperties.rotVar.x,
		emitter.defaultProperties.rotVar.y,
	};
	j["emitter"]["defualtProperties"]["xPosVar"] = {
		emitter.defaultProperties.xPosVar.x,
		emitter.defaultProperties.xPosVar.y,
	};
	j["emitter"]["defualtProperties"]["yPosVar"] = {
		emitter.defaultProperties.yPosVar.x,
		emitter.defaultProperties.yPosVar.y,
	};
	j["emitter"]["defualtProperties"]["xVelVar"] = {
		emitter.defaultProperties.xVelVar.x,
		emitter.defaultProperties.xVelVar.y,
	};
	j["emitter"]["defualtProperties"]["yVelVar"] = {
		emitter.defaultProperties.yVelVar.x,
		emitter.defaultProperties.yVelVar.y,
	};
}

void ParticleC::from_json(nlohmann::json& j)
{
	emitPeriod = j["emitPeriod"];
	emitCount = j["emitCount"];
	emitter.resize(j["emitter"]["maxParticles"]);

	emitter.defaultProperties.startColour = {
		j["emitter"]["defualtProperties"]["startColour"][0],
		j["emitter"]["defualtProperties"]["startColour"][1],
		j["emitter"]["defualtProperties"]["startColour"][2],
		j["emitter"]["defualtProperties"]["startColour"][3]
	}; 
	emitter.defaultProperties.endColour = {
		j["emitter"]["defualtProperties"]["endColour"][0],
		j["emitter"]["defualtProperties"]["endColour"][1],
		j["emitter"]["defualtProperties"]["endColour"][2],
		j["emitter"]["defualtProperties"]["endColour"][3]
	};
	emitter.defaultProperties.startSize = j["emitter"]["defualtProperties"]["startSize"];
	emitter.defaultProperties.endSize = j["emitter"]["defualtProperties"]["endSize"];
	emitter.defaultProperties.particleLifetime = {
		j["emitter"]["defualtProperties"]["lifetime"][0],
		j["emitter"]["defualtProperties"]["lifetime"][1],
	};
	emitter.defaultProperties.rotVar = {
		j["emitter"]["defualtProperties"]["rotVar"][0],
		j["emitter"]["defualtProperties"]["rotVar"][1],
	};
	emitter.defaultProperties.xPosVar = {
		j["emitter"]["defualtProperties"]["xPosVar"][0],
		j["emitter"]["defualtProperties"]["xPosVar"][1],
	};
	emitter.defaultProperties.yPosVar = {
		j["emitter"]["defualtProperties"]["yPosVar"][0],
		j["emitter"]["defualtProperties"]["yPosVar"][1],
	};
	emitter.defaultProperties.xVelVar = {
		j["emitter"]["defualtProperties"]["xVelVar"][0],
		j["emitter"]["defualtProperties"]["xVelVar"][1],
	};
	emitter.defaultProperties.yVelVar = {
		j["emitter"]["defualtProperties"]["yVelVar"][0],
		j["emitter"]["defualtProperties"]["yVelVar"][1],
	};
}
