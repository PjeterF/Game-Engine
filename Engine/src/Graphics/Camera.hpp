#pragma once

#include "glm/glm.hpp"
#include "../Events/EventPackage.hpp"

class Camera
{
public:
	Camera(float frustumX_min, float frustumX_max, float frustumY_min, float frustumY_max) : frustumX_original(glm::vec2(frustumX_min, frustumX_max)), frustumY_original(glm::vec2(frustumY_min, frustumY_max)){}
	
	void pan(float x, float y);
	glm::vec2 getPosition() { return position; }
	void setPosition(float x, float y) { position=glm::vec2(x,y); }

	void changeZoom(float increment);
	void setZoom(float newZoom);
	float getZoom() { return zoom; }

	void setFrustrumX(float min, float max) { frustumX_original = glm::vec2(min, max); }
	void setFrustrumY(float min, float max) { frustumY_original = glm::vec2(min, max); }

	glm::vec2 getOriginalFrustrumX() { return frustumX_original; }
	glm::vec2 getOriginalFrustrumY() { return frustumY_original; }

	glm::vec2 getFrustrumX() { return zoom * frustumX_original; }
	glm::vec2 getFrustrumY() { return zoom * frustumY_original; }
private:
	float zoom = 1;
	float panningSpeedScale = 0.2;
	glm::vec2 position=glm::vec2(0,0);
	glm::vec2 frustumX_original;
	glm::vec2 frustumY_original;
};